package ast

type ASTStructType struct {
	GenericList []ASTType
	StructName  string
}

func (t ASTStructType) _type() {}

type ASTSymbolType struct {
	Value string
}

func (t ASTSymbolType) _type() {}

type ASTListType struct {
	Underlying ASTType
}

func (t ASTListType) _type() {}

type ASTFnType struct {
	Parameters []ASTType
	ReturnType ASTType
}

func (t ASTFnType) _type() {}
