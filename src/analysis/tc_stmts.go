package analysis

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
)

func tc_block_stmt(s ast.BlockStmt, env *SymbolTable) Type {
	var last Type
	env = CreateSymbolTable(env, false)
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
