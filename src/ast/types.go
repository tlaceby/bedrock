package ast

type SymbolType struct {
	Value string
}

func (t SymbolType) _type() {}

type ListType struct {
	Underlying Type
}

func (t ListType) _type() {}

type FnType struct {
	Parameters []Type
	ReturnType Type
}

func (t FnType) _type() {}
