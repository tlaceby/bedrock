package analysis

import (
	"fmt"

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
