package ast

type StructType struct {
	GenericList []Type
	StructName  string
}

func (t StructType) _type() {}

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
