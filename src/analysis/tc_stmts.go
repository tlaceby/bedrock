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

	panic(e.str())
}

func tc_fn_declaration_stmt(s ast.FunctionDeclarationStmt, env *SymbolTable) Type {
	var functionName = s.Name
	var body = s.Body
	var generics = s.Generics
	var isGeneric = len(generics) > 0

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

	if isGeneric {
		genericFn := GenericFnType{
			Body:       body,
			Generics:   generics,
			Parameters: s.Parameters,
			ReturnType: s.ReturnType,
			Closure:    env,
		}

		env.Symbols[functionName] = SymbolInfo{
			Type:            genericFn,
			IsConstant:      true,
			AssignmentCount: 1,
		}

		return genericFn
	}

	var fnBodyEnv = CreateSymbolTable(env, true, false, false, functionName)
	var parameters = []Type{}
	var returns Type = VoidType{}

	if s.ReturnType != nil {
		returns = typecheck_type(s.ReturnType, env)
	}

	// Install Parameters
	for _, param := range s.Parameters {
		paramName := param.Name
		paramType := typecheck_type(param.Type, env)

		fnBodyEnv.Symbols[paramName] = SymbolInfo{
			Type:            paramType,
			IsConstant:      true,
			AssignmentCount: 1,
		}

		parameters = append(parameters, paramType)
	}

	var fnType = FnType{
		ParamTypes: parameters,
		ReturnType: returns,
		Variadic:   false,
	}

	env.Symbols[functionName] = SymbolInfo{
		Type:            fnType,
		AssignmentCount: 1,
		IsConstant:      true,
	}

	// Verify Function Body
	for _, stmt := range s.Body {
		typecheck_stmt(stmt, fnBodyEnv)
	}

	// Validate Return Types
	for _, foundReturn := range fnBodyEnv.FoundReturnTypes {
		if !typesSame(foundReturn, returns) {
			panic(fmt.Sprintf("In function declaration for %s, Found %s as a return type, however expected to find %s", functionName, foundReturn.str(), returns.str()))
		}
	}

	// If function expects a return but none is provided.
	if len(fnBodyEnv.FoundReturnTypes) == 0 && !typesSame(returns, VoidType{}) {
		panic(fmt.Sprintf("In function declaration for %s, Expected to find return type of %s but found none.", functionName, returns.str()))
	}

	fnBodyEnv.debugTable(false)
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

	moduleEnv.debugTable(false)

	return ModuleType{
		ModuleName: s.ModuleName,
	}
}

func tc_struct_declaration_stmt(s ast.StructDeclarationStmt, env *SymbolTable) Type {
	var structName = s.Name
	var isGeneric = len(s.Generics) > 0

	// Make sure struct is only defined inside global/module scope
	if !env.IsGlobal && !env.IsModule {
		panic(fmt.Sprintf("Cannot define struct %s in current scope. Structs can only be defined inside a module.", structName))
	}

	// Make sure struct does not already exist in current module/global scopes
	_, foundEnv := env.findNearestTypeEnv(structName)
	if foundEnv != nil {
		panic(fmt.Sprintf("Cannot define struct as a type with this name (%s) already exists", structName))
	}

	if isGeneric {
		genericType := GenericStructType{
			Name:            s.Name,
			Properties:      s.Properties,
			InstanceMethods: s.InstanceMethods,
			StaticMethods:   s.StaticMethods,
			Closure:         env,
			Generics:        s.Generics,
		}

		env.DefinedTypes[structName] = genericType
		return genericType
	}

	var structEnv = CreateSymbolTable(env, false, false, true, structName)
	var structType = validate_struct_body(structEnv, structName, s.Properties, s.StaticMethods, s.InstanceMethods)

	env.DefinedTypes[structName] = structType
	return structType
}

func validate_struct_body(structEnv *SymbolTable, structName string, Properties []ast.StructProperty, StaticMethods []ast.FunctionDeclarationStmt, InstanceMethods []ast.FunctionDeclarationStmt) StructType {
	var staticMethods = map[string]FnType{}
	var methods = map[string]FnType{}
	var properties = map[string]Type{}
	var structType = StructType{
		StructName:    structName,
		StaticMethods: staticMethods,
		Properties:    properties,
		Methods:       methods,
	}

	structEnv.DefinedTypes[structName] = structType
	structEnv.DefinedTypes["Self"] = structType

	// Generate Property Types
	for _, prop := range Properties {
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
	// This will just generate the type definitions for each of the static methods.
	for _, staticNode := range StaticMethods {
		var returnType Type = VoidType{}

		if staticNode.ReturnType != nil {
			returnType = typecheck_type(staticNode.ReturnType, structEnv)
		}

		params := []Type{}

		for _, param := range staticNode.Parameters {
			params = append(params, typecheck_type(param.Type, structEnv))
		}

		staticMethods[staticNode.Name] = FnType{
			ReturnType: returnType,
			ParamTypes: params,
		}
	}

	// Generate Instance Method Types (NO BODY EVAL)
	// This will just generate the type definitions for each of the instance methods.
	for _, instanceNode := range InstanceMethods {
		var returnType Type = VoidType{}

		if instanceNode.ReturnType != nil {
			returnType = typecheck_type(instanceNode.ReturnType, structEnv)
		}

		params := []Type{}

		for _, param := range instanceNode.Parameters {
			paramType := typecheck_type(param.Type, structEnv)
			params = append(params, paramType)
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
		}
	}

	// Generate Instance Method Types (NO BODY EVAL)
	// This will just generate the type definitions for each of the instance methods.
	for _, instanceNode := range InstanceMethods {
		var returnType Type = VoidType{}

		if instanceNode.ReturnType != nil {
			returnType = typecheck_type(instanceNode.ReturnType, structEnv)
		}

		params := []Type{}

		for _, param := range instanceNode.Parameters {
			params = append(params, typecheck_type(param.Type, structEnv))
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
		}
	}

	// Eval Body for static/instance methods
	// Now that all of the methods/instace/static/member properties are known we can re run through each
	// method/static method and make sure they return the proper types and dont access things they are not supposed to.

	for _, instanceMethod := range InstanceMethods {
		methodName := instanceMethod.Name
		methodEnv := CreateSymbolTable(structEnv, true, false, false, methodName+"()")

		methodEnv.Symbols["self"] = SymbolInfo{
			IsConstant:      true,
			AssignmentCount: 1,
			Type:            structType,
		}

		// foreach parameter populate the simulated env
		for _, param := range instanceMethod.Parameters {
			paramName := param.Name
			paramType := typecheck_type(param.Type, structEnv)

			methodEnv.Symbols[paramName] = SymbolInfo{
				Type:            paramType,
				IsConstant:      true,
				AssignmentCount: 1,
			}
		}

		for _, stmt := range instanceMethod.Body {
			typecheck_stmt(stmt, methodEnv)
		}

		// Validate return matches what is needs to for each instance method
		var expectedReturnType Type = VoidType{}
		if instanceMethod.ReturnType != nil {
			expectedReturnType = typecheck_type(instanceMethod.ReturnType, methodEnv)
		}

		for _, recievedReturnType := range methodEnv.FoundReturnTypes {
			if !helpers.TypesMatch(expectedReturnType, recievedReturnType) {
				err := fmt.Sprintf("Mismatch return types for method %s on struct %s. Expected to return %s but recieved %s instead.", instanceMethod.Name, structName, expectedReturnType.str(), recievedReturnType.str())
				panic(ErrType(err).str())
			}
		}

		methodEnv.debugTable(false)
	}

	// Eval static methods to verify the integrigity of the body.
	for _, staticMethod := range StaticMethods {
		methodName := staticMethod.Name
		methodEnv := CreateSymbolTable(structEnv, true, false, false, methodName+"()")

		// this will prevent us from accessing non static methods when inside static methods
		// Check the tc_member_expr() for examples
		methodEnv.IsStaticMethod = true

		// Before evaluating this environment make sure to remove all instance methods so they cannot be called.
		// This prevents static methods from calling or knowing about properties or instance methods
		for instanceMethodName := range structType.Methods {
			delete(methodEnv.Symbols, instanceMethodName)
		}

		for instanceProperty := range structType.Properties {
			delete(methodEnv.Symbols, instanceProperty)
		}

		// foreach parameter populate the simulated env
		for _, param := range staticMethod.Parameters {
			paramName := param.Name
			paramType := typecheck_type(param.Type, methodEnv)

			methodEnv.Symbols[paramName] = SymbolInfo{
				Type:            paramType,
				IsConstant:      true,
				AssignmentCount: 1,
			}

		}

		for _, stmt := range staticMethod.Body {
			typecheck_stmt(stmt, methodEnv)
		}

		// Validate return matches what is needs to for static method
		var expectedReturnType Type = VoidType{}
		if staticMethod.ReturnType != nil {
			expectedReturnType = typecheck_type(staticMethod.ReturnType, methodEnv)
		}

		for _, recievedReturnType := range methodEnv.FoundReturnTypes {
			if !helpers.TypesMatch(expectedReturnType, recievedReturnType) {
				err := fmt.Sprintf("Mismatch return types for method %s on struct %s. Expected to return %s but recieved %s instead.", staticMethod.Name, structName, expectedReturnType.str(), recievedReturnType.str())
				panic(ErrType(err))
			}
		}

		methodEnv.debugTable(false)
	}

	return structType
}

func tc_while_stmt(s ast.WhileStmt, env *SymbolTable) Type {
	var condition = typecheck_expr(s.Condition, env)
	var whileEnv = CreateSymbolTable(env, false, false, false, "while-loop")
	whileEnv.IsLoop = true

	if !typesSame(condition, BoolType{}) {
		panic(fmt.Sprintf("Expected bool in condition for while loop. Recieved %s instead.", condition.str()))
	}

	for _, stmt := range s.Body {
		typecheck_stmt(stmt, whileEnv)
	}

	whileEnv.debugTable(false)
	return VoidType{}
}
