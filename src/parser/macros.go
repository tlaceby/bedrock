package parser

import (
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/lexer"
)

func parse_assert_macro(p *parser) ast.Expr {
	p.expect(lexer.ASSERT_MACRO)
	p.expect(lexer.OPEN_PAREN)
	assertType := parse_type(p, defalt_bp)
	p.expect(lexer.COMMA)
	expr := parse_expr(p, logical)
	p.expect(lexer.CLOSE_PAREN)

	return ast.AssertExpr{
		TypeAssertion: assertType,
		Expr:          expr,
	}
}

func parse_string_macro(p *parser) ast.Expr {
	p.expect(lexer.STRING_MACRO)
	p.expect(lexer.OPEN_PAREN)
	expr := parse_expr(p, logical)
	p.expect(lexer.CLOSE_PAREN)

	return ast.CastStringExpr{
		Expr: expr,
	}
}
