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

	return ErrType(fmt.Sprintf("Symbol %s does not exist in the current env", e.Value))
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

	return err
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

	if !helpers.TypesMatchT[FnType](calle) {
		litter.Dump(e)
		panic(ErrType(fmt.Sprintf("Invalid call expression %s is not a function which can be called", calle.str())).str())
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
