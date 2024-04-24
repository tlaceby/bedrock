package ast

import (
	"github.com/tlaceby/bedrock/src/lexer"
)

// --------------------
// Literal Expressions
// --------------------

type NumberExpr struct {
	Value float64
}

func (n NumberExpr) expr() {}

type StringExpr struct {
	Value string
}

func (n StringExpr) expr() {}

type SymbolExpr struct {
	Value string
}

func (n SymbolExpr) expr() {}

// --------------------
// Complex Expressions
// --------------------

type BinaryExpr struct {
	Left     Expr
	Operator lexer.Token
	Right    Expr
}

func (n BinaryExpr) expr() {}

type AssignmentExpr struct {
	Assigne       Expr
	AssignedValue Expr
}

func (n AssignmentExpr) expr() {}

type PrefixExpr struct {
	Operator lexer.Token
	Right    Expr
}

func (n PrefixExpr) expr() {}

type MemberExpr struct {
	Member   Expr
	Property string
}

func (n MemberExpr) expr() {}

type StaticMemberExpr struct {
	StructName     string
	MethodName     string
	StructGenerics []ASTType
}

func (n StaticMemberExpr) expr() {}

type CallExpr struct {
	Method    Expr
	Arguments []Expr
	Generics  []ASTType
}

func (n CallExpr) expr() {}

type ComputedExpr struct {
	Member   Expr
	Property Expr
}

func (n ComputedExpr) expr() {}

type RangeExpr struct {
	Lower Expr
	Upper Expr
}

func (n RangeExpr) expr() {}

type FunctionExpr struct {
	Parameters []Parameter
	Body       []Stmt
	ReturnType ASTType
}

func (n FunctionExpr) expr() {}

type ArrayLiteral struct {
	UnderlyingType ASTType
	Capacity       int // -1 represents none a inferable size
	Contents       []Expr
}

func (n ArrayLiteral) expr() {}

type ObjectField struct {
	PropertyName  string
	PropertyValue Expr // Whether the value was passed or shorthand was used is determine if nil
}

type StructInstantiationExpr struct {
	StructName string
	Generics   []ASTType
	Objects    []ObjectField
}

func (n StructInstantiationExpr) expr() {}

// Macros

// @assert(Type, Expr)
type AssertExpr struct {
	TypeAssertion ASTType
	Expr          Expr
}

func (n AssertExpr) expr() {}

// @string(Expr)
type CastStringExpr struct {
	Expr Expr
}

func (n CastStringExpr) expr() {}
