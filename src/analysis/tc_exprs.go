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
	calle := typecheck_expr(e.Method, env)
	argTypes := []Type{}

	for _, argType := range e.Arguments {
		argTypes = append(argTypes, typecheck_expr(argType, env))
	}

	if !helpers.TypesMatchT[FnType](calle) && !helpers.TypesMatchT[GenericFnType](calle) {
		litter.Dump(e)
		panic(ErrType(fmt.Sprintf("Invalid call expression %s is not a function which can be called", calle.str())).str())
	}

	// If the function is generic then handle in seperate fn
	if len(e.Generics) > 0 {
		return tc_generic_call_expr(e, helpers.ExpectType[GenericFnType](calle), argTypes, env)
	}

	fn := helpers.ExpectType[FnType](calle)

	expectedArity := len(fn.ParamTypes)
	recievedArity := len(argTypes)

	if expectedArity != recievedArity {
		panic(ErrType(fmt.Sprintf("Invalid call expression for %s. Expected %d arguments but recieved %d instead", fn.str(), expectedArity, recievedArity)).str())
	}

	invalidArgErrors := []ErrorType{}
	// Validate each parameter against the supplied arguments
	for paramIndex, expectedType := range fn.ParamTypes {
		recievedType := argTypes[paramIndex]

		if !helpers.TypesMatch(recievedType, expectedType) {
			err := fmt.Sprintf("Expected %s but recieved %s instead call expression index %d", expectedType.str(), recievedType.str(), paramIndex)
			invalidArgErrors = append(invalidArgErrors, ErrType(err))
		}
	}

	if len(invalidArgErrors) > 0 {
		for _, err := range invalidArgErrors {
			println(err.str())
		}

		panic(fmt.Sprintf("Call expression %s failed typechecking due to type mismatches.", fn.str()))
	}

	return fn.ReturnType
}

func tc_generic_call_expr(e ast.CallExpr, fn GenericFnType, args []Type, env *SymbolTable) Type {
	var paramTypes []Type
	genericNames := fn.Generics
	node := fn.FnNode

	if len(genericNames) != len(e.Generics) {
		panic("Arity of generics at defintion and call site dont match")
	}

	fnEnv := CreateSymbolTable(env, true, false, false, fmt.Sprintf("generic.%s", node.Name))

	for indx, gt := range e.Generics {
		genericType := typecheck_type(gt, env)

		// Define T as whatever type is was found to be
		fnEnv.DefinedTypes[genericNames[indx]] = genericType
	}

	// Install parameters now that all generics should be matched.
	for _, param := range node.Parameters {
		paramType := typecheck_type(param.Type, fnEnv)
		paramTypes = append(paramTypes, paramType)

		fnEnv.Symbols[param.Name] = SymbolInfo{
			Type:            paramType,
			IsConstant:      true,
			AssignmentCount: 1,
		}
	}

	// Validate arity of call expr
	// TODO: Make actual TypeError
	if len(paramTypes) != len(args) {
		panic(fmt.Sprintf("Mistach arity in call expression. Expected %d but recieved %d instead", len(paramTypes), len(args)))
	}

	for indx, recievedArg := range args {
		if !helpers.TypesMatch(recievedArg, paramTypes[indx]) {
			panic(fmt.Sprintf("Types dont match. Expected %s but recieved %s instead in call_expr for %s", paramTypes[indx].str(), recievedArg.str(), node.Name))
		}
	}

	// Validate return type
	var expectedReturn Type = VoidType{}
	if node.ReturnType != nil {
		expectedReturn = typecheck_type(node.ReturnType, fnEnv)
	}

	expectsVoid := helpers.TypesMatchT[VoidType](expectedReturn)

	// evaluate the body and replace the generic typenames with their coresponding types
	for _, stmt := range node.Body {
		typecheck_stmt(stmt, fnEnv)
	}

	numReturns := len(fnEnv.FoundReturnTypes)

	if expectsVoid && numReturns == 0 {
		return VoidType{}
	}

	if !expectsVoid && numReturns == 0 {
		return ErrType(fmt.Sprintf("Function %s expected a return type of %s but never returns anything.", node.Name, expectedReturn.str()))
	}

	// Make sure each time we may return we return the correct type
	for _, foundReturnType := range fnEnv.FoundReturnTypes {
		if !helpers.TypesMatch(expectedReturn, foundReturnType) {
			return ErrType(fmt.Sprintf("Function %s expected return type: %s but recieved %s instead.", node.Name, expectedReturn.str(), foundReturnType.str()))
		}
	}

	return expectedReturn
}
