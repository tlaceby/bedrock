package analysis

import (
	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/ast"
)

type Type interface {
	str() string // string representation of Abstract Type
}

type SymbolTable struct {
	IsGlobal     bool // whether this table exists in the global scope
	IsFunction   bool // whether this table is the body of a function
	Parent       *SymbolTable
	DefinedTypes map[string]Type
	Symbols      map[string]Type
}

func CreateSymbolTable(parent *SymbolTable, fn bool) *SymbolTable {
	isGlobal := true

	if parent != nil {
		isGlobal = false
	}

	return &SymbolTable{
		Parent:       parent,
		IsGlobal:     isGlobal,
		IsFunction:   fn,
		Symbols:      map[string]Type{},
		DefinedTypes: map[string]Type{},
	}
}

func typecheck_expr(expr ast.Expr, env *SymbolTable) Type {
	switch e := expr.(type) {
	case ast.NumberExpr:
		return NumType{}
	case ast.StringExpr:
		return StrType{}
	case ast.SymbolExpr:
		return tc_symbol_expr(e, env)
	case ast.BinaryExpr:
		return tc_binary_expr(e, env)
	// case ast.AssignmentExpr:
	// 	// Handle AssignmentExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.PrefixExpr:
	// 	// Handle PrefixExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.MemberExpr:
	// 	// Handle MemberExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.StaticMemberExpr:
	// 	// Handle StaticMemberExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.CallExpr:
	// 	// Handle CallExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.ComputedExpr:
	// 	// Handle ComputedExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.RangeExpr:
	// 	// Handle RangeExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.FunctionExpr:
	// 	// Handle FunctionExpr
	// 	// ...
	// 	return VoidType{}
	// case ast.ArrayLiteral:
	// 	// Handle ArrayLiteral
	// 	// ...
	// 	return VoidType{}
	// case ast.StructInstantiationExpr:
	// 	// Handle StructInstantiationExpr
	// 	// ...
	// 	return VoidType{}
	default:
		litter.Dump(expr)
		panic("^^^^^^ Unknown ast.Expr encountered! ^^^^^^\n")
	}
}

func typecheck_stmt(stmt ast.Stmt, env *SymbolTable) Type {
	switch s := stmt.(type) {
	case ast.BlockStmt:
		return tc_block_stmt(s, env)
	// case ast.VarDeclarationStmt:
	// 	// Handle VarDeclarationStmt
	// 	// ...
	case ast.ExpressionStmt:
		return typecheck_expr(s.Expression, env)
	// case ast.FunctionDeclarationStmt:
	// 	// Handle FunctionDeclarationStmt
	// 	// ...
	// case ast.IfStmt:
	// 	// Handle IfStmt
	// 	// ...
	// case ast.ImportStmt:
	// 	// Handle ImportStmt
	// 	// ...
	// case ast.ForeachStmt:
	// 	// Handle ForeachStmt
	// 	// ...
	// case ast.StructDeclarationStmt:
	// 	// Handle StructDeclarationStmt
	// 	// ...
	// case ast.TraitStmt:
	// 	// Handle TraitStmt
	// 	// ...
	// case ast.ReturnStmt:
	// 	// Handle ReturnStmt
	// 	// ...
	default:
		litter.Dump(stmt)
		panic("^^^^^^ Unknown ast.Stmt encountered! ^^^^^^\n")
	}
}

func Typecheck(blockStmt ast.BlockStmt) Type {
	t := typecheck_stmt(blockStmt, nil)
	println("\nFinal Type: " + t.str())
	return t
}
