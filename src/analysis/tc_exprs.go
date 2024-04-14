package analysis

import (
	"fmt"

	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
	"github.com/tlaceby/bedrock/src/lexer"
)

func tc_symbol_expr(e ast.SymbolExpr, env *SymbolTable) Type {
	typeInfo, table := env.findNearestSymbolEnv(e.Value)

	if typeInfo != nil {
		typeInfo.AccessedCount += 1
		table.Symbols[e.Value] = *typeInfo
		return typeInfo.Type
	}

	panic(ErrType(fmt.Sprintf("Symbol %s does not exist in the current env", e.Value)).str())
}

func tc_binary_expr(e ast.BinaryExpr, env *SymbolTable) Type {
	leftType := typecheck_expr(e.Left, env)
	rightType := typecheck_expr(e.Right, env)

	// Default error if all checks fail
	err := ErrType("Invalid use of operator " + e.Operator.Value + " with " + leftType.str() + " and " + rightType.str())

	isComparisonOp := e.Operator.IsOneOfMany(lexer.LESS, lexer.LESS_EQUALS, lexer.GREATER, lexer.GREATER_EQUALS)
	isArithmeticOp := e.Operator.IsOneOfMany(lexer.DASH, lexer.SLASH, lexer.STAR, lexer.PERCENT)
	isLogical := e.Operator.IsOneOfMany(lexer.AND, lexer.OR)

	// Both operands must be numbers for arithmetic
	if isArithmeticOp && helpers.TypesMatchT[NumType](leftType, rightType) {
		return NumType{}
	}

	// Both types must be a number for < <= > >= :-> bool
	if isComparisonOp && helpers.TypesMatchT[NumType](leftType) && helpers.TypesMatch(leftType, rightType) {
		return BoolType{}
	}

	// for == or != both types must be the same on either side :-> bool
	if e.Operator.IsOneOfMany(lexer.EQUALS, lexer.NOT_EQUALS) && helpers.TypesMatch(leftType, rightType) {
		return BoolType{}
	}

	// && || must both be booleans on left/right side :-> bool
	if isLogical && CastableToBool(leftType) && CastableToBool(rightType) {
		return BoolType{}
	}

	// Check String Concatenation vs Addition
	if e.Operator.Kind == lexer.PLUS {
		// number + number
		if helpers.TypesMatchT[NumType](leftType) {
			if helpers.TypesMatchT[NumType](rightType) { // number + number
				return NumType{}
			}
		}

		// string + string
		if helpers.TypesMatchT[StrType](leftType) { // string + string
			if helpers.TypesMatchT[StrType](rightType) {
				return StrType{}
			}
		}
	}

	panic(err.str())
}

func tc_struct_instantation_expr(e ast.StructInstantiationExpr, env *SymbolTable) Type {
	structName := e.StructName
	initialValues := e.Objects

	// verify struct has been defined

	T, definedScope := env.findNearestTypeEnv(structName)

	if definedScope == nil {
		panic(ErrType(fmt.Sprintf("Cannot instantiate struct %s as it has not yet been defined.", structName)))
	}

	// Validate struct type is indeed a struct type
	if !helpers.TypesMatchT[StructType](T) {
		panic(ErrType(fmt.Sprintf("Cannot perform struct inbstantiation on %s{} as it is not a struct.", structName)))
	}

	structType := helpers.ExpectType[StructType](T)

	// Validate each field is instantiated which is expected in the struct
	var errors = []ErrorType{} // array of error messages to display

	for expectedPropertyName, expectedProperty := range structType.Properties {
		var foundProperty ast.ObjectField
		var found = false

		for _, val := range initialValues {
			if expectedPropertyName == val.PropertyName {
				foundProperty = val
				found = true
				break
			}
		}

		if !found {
			errors = append(errors, ErrType(fmt.Sprintf("Missing value for %s in struct instantiation of %s{}", expectedPropertyName, structName)))
			continue
		}

		// Validate the given type against the expected one for that propertyname
		givenType := typecheck_expr(foundProperty.PropertyValue, env)
		if !helpers.TypesMatch(givenType, expectedProperty) {
			errors = append(errors, ErrType(fmt.Sprintf("Property %s of struct %s{} expects type %s but was given %s instead.", expectedPropertyName, structName, expectedProperty.str(), givenType.str())))
		}
	}

	if len(errors) > 0 {
		for _, err := range errors {
			println(err.Message)
		}

		panic("Typechecking failed at struct instantiation")
	}

	return structType
}

func tc_member_expr(e ast.MemberExpr, env *SymbolTable) Type {
	memberType := typecheck_expr(e.Member, env)
	propertyName := e.Property

	switch member := memberType.(type) {
	case StructType:
		var propertyType Type

		// Check struct.properties for propertyname
		propertyType, propertyExists := member.Properties[propertyName]
		if propertyExists {
			return propertyType
		}

		// then check struct.methods for propertyname
		propertyType, propertyExists = member.Methods[propertyName]
		if propertyExists {
			return propertyType
		}

		panic(fmt.Sprintf("Member %s does not contain property %s. Attempted to access %s but it does not exist on object\n", memberType.str(), propertyName, propertyName))
	}

	panic(fmt.Sprintf("Member expression with %s.%s not handled\n", memberType.str(), propertyName))
}

func tc_static_member_expr(e ast.StaticMemberExpr, env *SymbolTable) Type {
	structName := e.StructName
	propertyName := e.MethodName
	memberType, memberEnv := env.findNearestTypeEnv(structName)

	if memberEnv == nil {
		panic(ErrType(fmt.Sprintf("Invalid member access using resolution operator on %s::%s. Type %s Cannot be resolved.", structName, propertyName, structName)).str())
	}

	switch member := memberType.(type) {
	case StructType:

		// Check struct.properties for propertyname
		propertyType, propertyExists := member.StaticMethods[propertyName]
		if propertyExists {
			return propertyType
		}

		panic(fmt.Sprintf("Member %s does not contain static property %s. Attempted to access static member %s but it does not exist on object\n", memberType.str(), propertyName, propertyName))
	}

	panic(fmt.Sprintf("Static member expression with %s::%s not handled\n", memberType.str(), propertyName))
}

func tc_call_expr(e ast.CallExpr, env *SymbolTable) Type {
	var method = typecheck_expr(e.Method, env)

	// Verify method is indeed a function
	if !IsAFnType(method) {
		panic("Attempted to call function in call expression with non function.")
	}

	var fnInstanceType FnType
	var recievedArgs = []Type{}

	// If the method is not generic then simply cast type away
	if IsNormalFnType(method) {
		fnInstanceType = helpers.ExpectType[FnType](method)
	}

	// If the function is generic then we need to validate body aswell.
	if IsGenericFnType(method) {
		var genericFn = helpers.ExpectType[GenericFnType](method)
		var fnEnv = CreateSymbolTable(genericFn.Closure, true, false, false, genericFn.str())
		var expectedReturnType Type = VoidType{}
		var params = []Type{}

		// Validate Arity of Generic Call
		expectedGenericArity := len(genericFn.Generics)
		recievedGenericArity := len(e.Generics)

		if expectedGenericArity != recievedGenericArity {
			panic(fmt.Sprintf("Expected %d generic args but recieved %d instead. You must suply the explit generic types in a generic fn call.", expectedGenericArity, recievedGenericArity))
		}

		// Since we know the arity is correct, assign each generic type parameter with the ones recieved
		for index, genericName := range genericFn.Generics {
			genericType := typecheck_type(e.Generics[index], env)
			fnEnv.DefinedTypes[genericName] = genericType
		}

		// Validate and Install Parameters
		for _, param := range genericFn.Parameters {
			paramName := param.Name
			paramType := typecheck_type(param.Type, fnEnv)

			fnEnv.Symbols[paramName] = SymbolInfo{
				Type:            paramType,
				IsConstant:      true,
				AssignmentCount: 1,
			}

			params = append(params, paramType)
		}

		// Return Type
		if genericFn.ReturnType != nil {
			expectedReturnType = typecheck_type(genericFn.ReturnType, fnEnv)
		}

		// Evaluate Body
		for _, stmt := range genericFn.Body {
			typecheck_stmt(stmt, fnEnv)
		}

		// Validate Returns Match
		for _, foundReturn := range fnEnv.FoundReturnTypes {
			if !typesSame(foundReturn, expectedReturnType) {
				panic(fmt.Sprintf("In generic function call %s, found %s as a return type, however expected to find %s", genericFn.str(), foundReturn.str(), expectedReturnType.str()))
			}
		}

		// If we expect a return type but recieve none
		if len(fnEnv.FoundReturnTypes) == 0 && !typesSame(expectedReturnType, VoidType{}) {
			panic(fmt.Sprintf("In generic function call %s , expected to find return type of %s but found none.", genericFn.str(), expectedReturnType.str()))
		}

		fnInstanceType = FnType{
			Variadic:   false,
			ParamTypes: params,
			ReturnType: expectedReturnType,
		}
	}

	// Create Arg Types
	for _, recievedArg := range e.Arguments {
		recievedArgs = append(recievedArgs, typecheck_expr(recievedArg, env))
	}

	expectedArity := len(fnInstanceType.ParamTypes)
	recievedArity := len(recievedArgs)

	// TODO: Variadic functions ???
	if expectedArity != recievedArity {
		panic(fmt.Sprintf("Expected arity of call expression %s, is %d but recieved %d instead.", fnInstanceType.str(), expectedArity, recievedArity))
	}

	for argIndx, expectedArg := range fnInstanceType.ParamTypes {
		recievedArg := recievedArgs[argIndx]
		if !typesSame(expectedArg, recievedArg) {
			panic(fmt.Sprintf("In function call %s: Arg at position %d expected to be a type %s but recieved %s instead.", fnInstanceType.str(), argIndx, expectedArg.str(), recievedArg.str()))
		}
	}

	return fnInstanceType.ReturnType
}

func tc_assignment_expr(e ast.AssignmentExpr, env *SymbolTable) Type {
	assigne := typecheck_expr(e.Assigne, env)
	value := typecheck_expr(e.AssignedValue, env)

	// Make sure we are assigning valid things.
	switch lhs := e.Assigne.(type) {
	case ast.SymbolExpr:
		varname := lhs.Value
		// make sure symbol exists and is not constant
		varInfo, varEnv := env.findNearestSymbolEnv(varname)

		// Make sure variable exists
		if varInfo == nil || varEnv == nil {
			panic(fmt.Sprintf("Cannot perform assignment on %s as it does not exist.", varname))
		}

		// Avoid Constant Assignment
		if varInfo.IsConstant {
			panic(fmt.Sprintf("Cannot perform assignment on %s as it declared as constant.", varname))
		}

		// Make sure types match
		if !typesSame(varInfo.Type, assigne) {
			panic(fmt.Sprintf("Invalid assignment of %s with %s = %s\n", varname, varInfo.Type.str(), value.str()))
		}

		// Update assignment and access count
		varInfo.AccessedCount-- // When performing the lookup it adds an access. Remove it.
		varInfo.AssignmentCount++

		varEnv.Symbols[varname] = *varInfo

		return value

	case ast.MemberExpr:
		memberExpr := typecheck_expr(lhs.Member, env)

		switch member := memberExpr.(type) {
		case StructType:
			propertyName := lhs.Property
			propertyValue := member.getPropertyByName(propertyName)

			// Make sure types match
			if !typesSame(propertyValue, value) {
				panic(fmt.Sprintf("Mismatched types. Invalid assignment of %s.%s with %s. Expected type is %s.\n", member.StructName, propertyName, value.str(), propertyValue.str()))
			}

			return value
		}
	}

	litter.Dump(e)
	panic(fmt.Sprintf("Invalid assignment expression. Cannot assign %s to %s\n", assigne.str(), value.str()))
}
