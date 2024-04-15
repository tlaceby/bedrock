package analysis

import (
	"fmt"

	"github.com/fatih/color"
	"github.com/tlaceby/bedrock/src/ast"
)

type TraitType struct {
	TraitName string
	Methods   map[string]FnType
}

func (t TraitType) str() string {
	return fmt.Sprintf("%s {}", color.MagentaString(t.TraitName))
}

type ModuleType struct {
	ModuleName    string
	PublicSymbols map[string]SymbolInfo
	PublicTypes   map[string]Type // TODO: ....
}

func (t ModuleType) str() string {
	return fmt.Sprintf("%s %s", color.RedString("module"), t.ModuleName)
}

type StructType struct {
	StructName    string
	Properties    map[string]Type
	Methods       map[string]FnType
	StaticMethods map[string]FnType
}

func (t StructType) getPropertyByName(propertyName string) Type {
	value, exists := t.Properties[propertyName]
	if exists {
		return value
	}

	return nil
}

func (t StructType) str() string {
	return fmt.Sprintf("%s {}", t.StructName)
}

type GenericStructType struct {
	Generics        []string
	Closure         *SymbolTable
	Name            string
	Properties      []ast.StructProperty
	StaticMethods   []ast.FunctionDeclarationStmt
	InstanceMethods []ast.FunctionDeclarationStmt
}

func (t GenericStructType) str() string {
	return createGenericListStr(t.Name, t.Generics)
}

type ErrorType struct {
	Message string
}

func (t ErrorType) str() string {
	return fmt.Sprintf("%s (%s)", color.RedString("Error"), t.Message)
}

func ErrType(message string) ErrorType {
	return ErrorType{Message: message}
}

type NumType struct{}

func (t NumType) str() string {
	return color.BlueString("Number")
}

type VoidType struct{}

func (t VoidType) str() string {
	return color.RedString("Void")
}

type StrType struct{}

func (t StrType) str() string {
	return color.BlueString("String")
}

type BoolType struct{}

func (t BoolType) str() string {
	return color.RedString("Bool")
}

type ArrayType struct {
	Capacity   uint
	Underlying Type
}

func (t ArrayType) str() string {
	return fmt.Sprintf("[]%s", t.Underlying.str())
}

type FnType struct {
	Variadic   bool
	ParamTypes []Type
	ReturnType Type
}

func (t FnType) str() string {
	var paramsStr string = ""

	for idx, param := range t.ParamTypes {
		paramsStr += param.str()

		if idx < len(t.ParamTypes)-1 {
			paramsStr += ", "
		}
	}

	return fmt.Sprintf("%s (%s) -> %s", color.RedString("fn"), paramsStr, t.ReturnType.str())
}

type GenericFnType struct {
	Body       []ast.Stmt
	Parameters []ast.Parameter
	Generics   []string
	ReturnType ast.Type
	Closure    *SymbolTable
}

func (t GenericFnType) str() string {
	var genericStr string = ""

	genericStr += "<"
	for idx, genericType := range t.Generics {
		genericStr += genericType

		if idx < len(t.Generics)-1 {
			genericStr += ", "
		}
	}

	genericStr += ">"

	return fmt.Sprintf("%s %s", color.RedString("fn"), genericStr)
}

func defineGlobalDefaultTypes(table *SymbolTable) {
	table.DefinedTypes["String"] = StrType{}
	table.DefinedTypes["Number"] = NumType{}
	table.DefinedTypes["Void"] = VoidType{}
	table.DefinedTypes["Bool"] = BoolType{}

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
