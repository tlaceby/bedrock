package analysis

import (
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
	"github.com/tlaceby/bedrock/src/lexer"
)

func tc_symbol_expr(e ast.SymbolExpr, env *SymbolTable) Type {
	symbolType, exists := env.Symbols[e.Value]

	if exists {
		return symbolType
	}

	panic("Symbol Not Defined: " + e.Value)
}

func tc_binary_expr(e ast.BinaryExpr, env *SymbolTable) Type {
	leftType := typecheck_expr(e.Left, env)
	rightType := typecheck_expr(e.Right, env)

	// Both operands must be numbers
	if e.Operator.IsOneOfMany(lexer.DASH, lexer.SLASH, lexer.STAR, lexer.PERCENT) {
		if helpers.TypesMatchT[NumType](leftType, rightType) {
			return NumType{}
		}
	}

	// Check String Concatenation vs Addition
	if e.Operator.Kind == lexer.PLUS {
		if helpers.TypesMatchT[NumType](leftType) {
			if helpers.TypesMatchT[NumType](rightType) { // number + number
				return NumType{}
			}
		} else if helpers.TypesMatchT[StrType](leftType) { // string + string
			if helpers.TypesMatchT[StrType](rightType) {
				return StrType{}
			}
		}
	}

	err := ErrType("Invalid use of operator " + e.Operator.Value + " with " + leftType.str() + " and " + rightType.str())
	return err
}
