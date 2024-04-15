package analysis

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
)

func tc_cast_string_expr(m ast.CastStringExpr, env *SymbolTable) Type {
	expr := typecheck_expr(m.Expr, env)

	if !CastableToString(expr) {
		panic(fmt.Sprintf("Cannot cast type to a string @string(%s)", expr.str()))
	}

	return StrType{}
}

func tc_assert_expr(m ast.AssertExpr, env *SymbolTable) Type {
	var assertType = typecheck_type(m.TypeAssertion, env)
	typecheck_expr(m.Expr, env)

	return assertType
}
