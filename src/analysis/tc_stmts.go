package analysis

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
)

func tc_block_stmt(s ast.BlockStmt, env *SymbolTable) Type {
	var last Type
	env = CreateSymbolTable(env, false, false, false, "block")
	for _, stmt := range s.Body {
		last = typecheck_stmt(stmt, env)
	}

	env.debugTable(false)
	return last
}

func tc_var_declaration_stmt(s ast.VarDeclarationStmt, env *SymbolTable) Type {
	// Check that variable does not already exist in current scope
	e := ErrType(fmt.Sprintf("A variable with the name %s already exists in the current scope. Did you mean to reassign instead?", s.Identifier))
	_, exists := env.Symbols[s.Identifier]

	if !exists {
		var expectedType Type
		var rhsValue Type

		// If a given explicit type is there then need to check it
		if s.ExplicitType != nil {
			expectedType = typecheck_type(s.ExplicitType, env)

			// If nothing was defined but a type was given mark it.
			if s.AssignedValue == nil {
				env.Symbols[s.Identifier] = SymbolInfo{
					Type:            expectedType,
					IsConstant:      s.Constant, // should be false
					AssignmentCount: 0,
					AccessedCount:   0,
				}

				return expectedType
			}
		}

		rhsValue = typecheck_expr(s.AssignedValue, env)

		// This means implicit type from rhs no explicit given
		if expectedType == nil {
			env.Symbols[s.Identifier] = SymbolInfo{
				Type:            rhsValue,
				IsConstant:      s.Constant,
				AssignmentCount: 1,
				AccessedCount:   0,
			}

			return rhsValue
		}

		// Both types match so were good.
		if helpers.TypesMatch(expectedType, rhsValue) {
			env.Symbols[s.Identifier] = SymbolInfo{
				Type:            rhsValue,
				IsConstant:      s.Constant,
				AssignmentCount: 1,
				AccessedCount:   0,
			}

			return expectedType
		}

		e = ErrType(fmt.Sprintf("Mismatched types inside variable declaration of %s. Expected %s but recieved %s instead.", s.Identifier, expectedType.str(), rhsValue.str()))
	}

	return e
}

func tc_fn_declaration_stmt(s ast.FunctionDeclarationStmt, env *SymbolTable) Type {
	var functionName = s.Name
	var parameters = []Type{}
	var returns Type

	// Make sure inside global/module scope when definining functions
	// TODO: Create Module AST node which will represent the file.
	// All Modules have the same global env.
	// A env which is a module or global are allowed for function declarations

	if !env.IsGlobal && !env.IsModule {
		panic(fmt.Sprintf("Cannot define function %s outside module scope.", functionName))
	}

	_, alreadyExists := env.Symbols[functionName]
	if alreadyExists {
		return ErrType(fmt.Sprintf("Cannot define function %s as a symbol with the same name already exists.", functionName))
	}

	// Validate return type (given or explicit meaning void)
	if s.ReturnType == nil {
		returns = VoidType{}
	} else {
		returns = typecheck_type(s.ReturnType, env)
	}

	functionBodyEnv := CreateSymbolTable(env, true, false, false, functionName)

	// Validate each parameter and see what type it is.
	for _, param := range s.Parameters {
		paramType := typecheck_type(param.Type, functionBodyEnv)

		if helpers.TypesMatchT[ErrorType](paramType) {
			err := helpers.ExpectType[ErrorType](paramType).str()
			return ErrType(fmt.Sprintf("Unknown parameter (%s) type found inside function declaration (fn %s) : %s", param.Name, functionName, err))
		}

		// Make sure we dont re-declare variable in parameter list
		_, paramExists := functionBodyEnv.Symbols[param.Name]
		if paramExists {
			return ErrType(fmt.Sprintf("Duplicate parameter (%s) inside fn declaration of %s", param.Name, functionName))
		}

		functionBodyEnv.Symbols[param.Name] = SymbolInfo{
			IsConstant:      true,
			Type:            paramType,
			AccessedCount:   0,
			AssignmentCount: 1,
		}

		parameters = append(parameters, paramType)
	}

	fnType := FnType{
		ParamTypes: parameters,
		ReturnType: returns,
	}

	// Define the function inside the current global scope.
	env.Symbols[functionName] = SymbolInfo{
		IsConstant:      true,
		AssignmentCount: 1,
		AccessedCount:   0,
		Type:            fnType,
	}

	// Validate the body of the function
	for _, stmt := range s.Body {
		typecheck_stmt(stmt, functionBodyEnv)
	}

	// Make sure void function has no returns
	expectsVoid := helpers.TypesMatchT[VoidType](returns)
	numReturns := len(functionBodyEnv.FoundReturnTypes)

	if expectsVoid && numReturns == 0 {
		return fnType
	}

	if !expectsVoid && numReturns == 0 {
		return ErrType(fmt.Sprintf("Function %s expected a return type of %s but never returns anything.", functionName, returns.str()))
	}

	// Make sure each time we may return we return the correct type
	for _, foundReturnType := range functionBodyEnv.FoundReturnTypes {
		if !helpers.TypesMatch(returns, foundReturnType) {
			return ErrType(fmt.Sprintf("Function %s expected return type: %s but recieved %s instead.", functionName, returns.str(), foundReturnType.str()))
		}
	}

	return fnType
}

func tc_return_stmt(s ast.ReturnStmt, env *SymbolTable) Type {
	var returnType Type = VoidType{}
	nearestFnScope := env.findNearestFunctionEnv()

	if s.ReturnValue != nil {
		returnType = typecheck_expr(s.ReturnValue, env)
	}

	if nearestFnScope == nil {
		return ErrType("Cannot use return statement outside a function")
	}

	nearestFnScope.FoundReturnTypes = append(nearestFnScope.FoundReturnTypes, returnType)
	return returnType
}

func tc_module_stmt(s ast.ModuleStmt, env *SymbolTable) Type {
	moduleEnv := CreateSymbolTable(env, false, true, false, s.ModuleName)

	for _, stmt := range s.Body {
		typecheck_stmt(stmt, moduleEnv)
	}

	moduleEnv.debugTable(true)

	return ModuleType{
		ModuleName: s.ModuleName,
	}
}

func tc_struct_declaration_stmt(s ast.StructDeclarationStmt, env *SymbolTable) Type {
	var structName = s.Name
	var generics = s.Generics
	var staticMethods = map[string]FnType{}
	var methods = map[string]FnType{}
	var properties = map[string]Type{}
	var publicMembers = []string{}

	// Make sure struct is only defined inside global/module scope
	if !env.IsGlobal && !env.IsModule {
		panic(fmt.Sprintf("Cannot define struct %s in current scope. Structs can only be defined inside a module.", structName))
	}

	// Make sure struct does not already exist in current module/global scopes
	_, foundEnv := env.findNearestTypeEnv(structName)
	if foundEnv != nil {
		panic(fmt.Sprintf("Cannot define struct as a type with this name (%s) already exists", structName))
	}

	structEnv := CreateSymbolTable(env, false, false, true, structName)

	structType := StructType{
		StructName:    structName,
		Generics:      generics,
		StaticMethods: staticMethods,
		Properties:    properties,
		Methods:       methods,
		PublicMembers: publicMembers,
	}

	env.DefinedTypes[structName] = structType

	// Generate Property Types
	for _, prop := range s.Properties {
		foundType := typecheck_type(prop.Type, structEnv)

		structEnv.Symbols[prop.Name] = SymbolInfo{
			Type:            foundType,
			IsConstant:      false,
			AssignmentCount: 0,
			AccessedCount:   0,
		}

		properties[prop.Name] = foundType
	}

	// Generate Static Method Types (NO BODY EVAL)
	for _, staticNode := range s.StaticMethods {
		returnType := typecheck_type(staticNode.ReturnType, env)
		params := []Type{}

		for _, param := range staticNode.Parameters {
			params = append(params, typecheck_type(param.Type, env))
		}

		staticMethods[staticNode.Name] = FnType{
			ReturnType: returnType,
			ParamTypes: params,
		}
	}

	// Generate Instance Method Types (NO BODY EVAL)
	for _, instanceNode := range s.InstanceMethods {
		returnType := typecheck_type(instanceNode.ReturnType, env)
		params := []Type{}

		for _, param := range instanceNode.Parameters {
			params = append(params, typecheck_type(param.Type, env))
		}

		fnType := FnType{
			ReturnType: returnType,
			ParamTypes: params,
		}

		methods[instanceNode.Name] = fnType

		structEnv.Symbols[instanceNode.Name] = SymbolInfo{
			Type:            fnType,
			IsConstant:      true,
			AssignmentCount: 1,
			AccessedCount:   0,
		}
	}

	// Eval Body for static/instance methods

	// When evaluating the body make sure self is defined
	structEnv.Symbols["self"] = SymbolInfo{
		IsConstant:      true,
		AssignmentCount: 1,
		AccessedCount:   0,
		Type:            structType,
	}

	for _, instanceMethod := range s.InstanceMethods {
		methodName := fmt.Sprintf("%s.%s", structName, instanceMethod.Name)
		methodEnv := CreateSymbolTable(structEnv, true, false, false, methodName)

		for _, stmt := range instanceMethod.Body {
			typecheck_stmt(stmt, methodEnv)
		}

		methodEnv.debugTable(false)
	}

	for _, staticMethod := range s.StaticMethods {
		methodName := fmt.Sprintf("%s::%s", structName, staticMethod.Name)
		methodEnv := CreateSymbolTable(structEnv, true, false, false, methodName)

		for _, stmt := range staticMethod.Body {
			typecheck_stmt(stmt, methodEnv)
		}

		methodEnv.debugTable(false)
	}

	return structType
}
