package analysis

import (
	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/ast"
)

func (tc *Typechecker) stmt(node ast.Stmt, scope *ast.Scope) ast.Type {
	switch stmt := node.(type) {
	case ast.ModuleStmt:
		return tc.Typecheck(stmt)
	case ast.BlockStmt:
		return tc.block_stmt(stmt, scope)
	case ast.VarDeclarationStmt:
		return tc.var_decl_stmt(stmt, scope)
	case ast.TraitStmt:
		return tc.trait_stmt(stmt, scope)
	}

	litter.Dump(node)
	panic("Unknown type inside typecheck_stmt()")
}

func (tc *Typechecker) block_stmt(node ast.BlockStmt, scope *ast.Scope) ast.Type {
	return nil
}

func (tc *Typechecker) var_decl_stmt(node ast.VarDeclarationStmt, scope *ast.Scope) ast.Type {
	return nil
}

func (tc *Typechecker) trait_stmt(node ast.TraitStmt, scope *ast.Scope) ast.Type {
	return nil
}
