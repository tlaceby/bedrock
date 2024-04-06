package analysis

import (
	"fmt"
)

type ModuleType struct {
	ModuleName    string
	PublicSymbols map[string]SymbolInfo
	PublicTypes   map[string]Type // TODO: ....
}

func (t ModuleType) str() string {
	return fmt.Sprintf("module %s", t.ModuleName)
}

type StructType struct {
	StructName    string
	Generics      []string
	Properties    map[string]Type
	Methods       map[string]FnType
	StaticMethods map[string]FnType
	PublicMembers []string // Names of all public struct properties/methods. // TODO:
}

func (t StructType) str() string {
	return fmt.Sprintf("%s {}", t.StructName)
}

type ErrorType struct {
	Message string
}

func (t ErrorType) str() string {
	return fmt.Sprintf("Error (%s)", t.Message)
}

func ErrType(message string) ErrorType {
	return ErrorType{Message: message}
}

type NumType struct{}

func (t NumType) str() string {
	return "number"
}

type VoidType struct{}

func (t VoidType) str() string {
	return "void"
}

type StrType struct{}

func (t StrType) str() string {
	return "string"
}

type BoolType struct{}

func (t BoolType) str() string {
	return "bool"
}

type ArrayType struct {
	Underlying Type
}

func (t ArrayType) str() string {
	return fmt.Sprintf("[]%s", t.Underlying.str())
}

type FnType struct {
	ReturnType Type
	ParamTypes []Type
}

func (t FnType) str() string {
	var paramsStr string = ""
	numParams := len(t.ParamTypes)

	for idx, param := range t.ParamTypes {
		paramsStr += param.str()

		if idx < numParams-1 {
			paramsStr += ", "
		}
	}

	return fmt.Sprintf("fn (%s) -> %s", paramsStr, t.ReturnType.str())
}

func CastableToBool(t Type) bool {
	switch t.(type) {
	case BoolType:
	case NumType:
	case StrType:
	case ArrayType:
		return true
	default:
		return false
	}

	return false
}

func defineGlobalDefaultTypes(table *SymbolTable) {
	table.DefinedTypes["string"] = StrType{}
	table.DefinedTypes["number"] = NumType{}
	table.DefinedTypes["void"] = VoidType{}
	table.DefinedTypes["bool"] = BoolType{}

	table.Symbols["true"] = SymbolInfo{
		Type:            BoolType{},
		IsConstant:      true,
		AssignmentCount: 1,
		AccessedCount:   1,
	}

	table.Symbols["false"] = SymbolInfo{
		Type:            BoolType{},
		IsConstant:      true,
		AssignmentCount: 1,
		AccessedCount:   1,
	}
}
