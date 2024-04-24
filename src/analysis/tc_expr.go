package analysis

import (
	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/ast"
)

func (tc *Typechecker) expr(node ast.Expr, scope *ast.Scope) ast.Type {
	switch expr := node.(type) {
	case ast.StringExpr:
		return String()
	case ast.SymbolExpr:
		return tc.symbol_expr(expr, scope)
	case ast.NumberExpr:
		return Number()
	case ast.BinaryExpr:
		return tc.binary_expr(expr, scope)
	}

	litter.Dump(node)
	panic("Unknown type inside typecheck_expr()")
}

func (tc *Typechecker) binary_expr(node ast.BinaryExpr, scope *ast.Scope) ast.Type {
	return nil
}

func (tc *Typechecker) symbol_expr(node ast.SymbolExpr, scope *ast.Scope) ast.Type {
	return nil
}
