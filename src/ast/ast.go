package ast

import "github.com/tlaceby/bedrock/src/helpers"

type Stmt interface {
	stmt()
}

type Expr interface {
	expr()
}

type ASTType interface {
	_type()
}

func ExpectExpr[T Expr](expr Expr) T {
	return helpers.ExpectType[T](expr)
}

func ExpectStmt[T Stmt](expr Stmt) T {
	return helpers.ExpectType[T](expr)
}

func ExpectType[T ASTType](_type ASTType) T {
	return helpers.ExpectType[T](_type)
}
