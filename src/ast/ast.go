package ast

import "github.com/tlaceby/bedrock/src/helpers"

type Stmt interface {
	stmt()
}

type Expr interface {
	expr()
}

type Type interface {
	_type()
}

func ExpectExpr[T Expr](expr Expr) T {
	return helpers.ExpectType[T](expr)
}

func ExpectStmt[T Stmt](expr Stmt) T {
	return helpers.ExpectType[T](expr)
}

func ExpectType[T Type](_type Type) T {
	return helpers.ExpectType[T](_type)
}
