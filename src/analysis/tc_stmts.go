package analysis

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
)

func tc_block_stmt(s ast.BlockStmt, env *SymbolTable) Type {
	var last Type
	env = CreateSymbolTable(env, false, "block")
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

	functionBodyEnv := CreateSymbolTable(env, true, functionName)

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
