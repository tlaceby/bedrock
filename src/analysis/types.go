package analysis

import (
	"fmt"
)

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
	table.DefinedTypes["bool"] = BoolType{}
}
