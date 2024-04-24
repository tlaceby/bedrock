package ast

import (
	"fmt"

	"github.com/fatih/color"
)

type Type interface {
	str() string
}

type TraitType struct {
	TraitName string
	Methods   map[string]FnType
}

func (t TraitType) str() string {
	return fmt.Sprintf("%s {}", color.MagentaString(t.TraitName))
}

type ModuleType struct {
	ModuleName    string
	PublicSymbols map[string]Symbol
	PublicTypes   map[string]Type
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
	Closure         *Scope
	Name            string
	Properties      []StructProperty
	StaticMethods   []FunctionDeclarationStmt
	InstanceMethods []FunctionDeclarationStmt
}

func (t GenericStructType) str() string {
	return createGenericListStr(t.Name, t.Generics)
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
	Body       []Stmt
	Parameters []Parameter
	Generics   []string
	ReturnType Type
	Closure    *Scope
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

func defineGlobalDefaultTypes(table *Scope) {
	table.DefinedTypes["String"] = StrType{}
	table.DefinedTypes["Number"] = NumType{}
	table.DefinedTypes["Void"] = VoidType{}
	table.DefinedTypes["Bool"] = BoolType{}

	table.Symbols["true"] = Symbol{
		Type:            BoolType{},
		IsConstant:      true,
		AssignmentCount: 1,
		AccessedCount:   1,
	}

	table.Symbols["false"] = Symbol{
		Type:            BoolType{},
		IsConstant:      true,
		AssignmentCount: 1,
		AccessedCount:   1,
	}

	// Create default builtin global methods
	table.Symbols["println"] = Symbol{
		IsConstant: true, AssignmentCount: 1,
		AccessedCount: 1, Type: FnType{
			Variadic:   false,
			ReturnType: VoidType{},
			ParamTypes: []Type{
				StrType{},
			},
		},
	}

	table.Symbols["print"] = Symbol{
		IsConstant: true, AssignmentCount: 1,
		AccessedCount: 1, Type: FnType{
			Variadic:   false,
			ReturnType: VoidType{},
			ParamTypes: []Type{
				StrType{},
			},
		},
	}
}

func createGenericListStr(structName string, generics []string) string {
	str := color.MagentaString(structName) + "<"

	for indx, name := range generics {
		str += name
		if indx != len(generics)-1 {
			str += ", "
		}
	}

	str += ">"
	return str
}

func typesSame(t Type, expected Type) bool {
	if t.str() == expected.str() {
		return true
	}

	switch other := expected.(type) {
	case TraitType:
		return implements(t, other)
	}

	return false
}

func CastableToBool(t Type) bool {
	switch t.(type) {
	case BoolType:
		return true
	case NumType:
		return true
	case StrType:
		return true
	case ArrayType:
		return true
	default:
		return false
	}
}

func CastableToString(t Type) bool {
	// For now everything can be cast to a string functions, structs and traits included.
	switch t.(type) {
	case BoolType:
		return true
	case NumType:
		return true
	case StrType:
		return true
	case ArrayType:
		return true
	default:
		return true
	}
}

func IsAFnType(t Type) bool {
	switch t.(type) {
	case FnType:
		return true
	case GenericFnType:
		return true
	}

	return false
}

func IsAStructType(t Type) bool {
	switch t.(type) {
	case StructType:
		return true
	case GenericStructType:
		return true
	}

	return false
}

func IsNormalFnType(t Type) bool {
	switch t.(type) {
	case FnType:
		return true
	}

	return false
}

func IsNormalStructType(t Type) bool {
	switch t.(type) {
	case StructType:
		return true
	}

	return false
}

func IsGenericFnType(t Type) bool {
	switch t.(type) {
	case GenericFnType:
		return true
	}

	return false
}

func IsGenericStructType(t Type) bool {
	switch t.(type) {
	case GenericStructType:
		return true
	}

	return false
}

func implements(t Type, trait TraitType) bool {
	switch v := t.(type) {
	case StructType:
		for methodName, fnType := range trait.Methods {
			method, exists := v.Methods[methodName]
			if !exists || !isFnTypesEqual(method, fnType) {
				return false
			}
		}
		return true
	// Add cases for other types that can implement traits
	default:
		return false
	}
}

func isFnTypesEqual(fn1, fn2 FnType) bool {
	if fn1.Variadic != fn2.Variadic || len(fn1.ParamTypes) != len(fn2.ParamTypes) {
		return false
	}

	for i := range fn1.ParamTypes {
		if !isTypesEqual(fn1.ParamTypes[i], fn2.ParamTypes[i]) {
			return false
		}
	}

	return isTypesEqual(fn1.ReturnType, fn2.ReturnType)
}

func isTypesEqual(t1, t2 Type) bool {
	// Implement type equality check based on your type system
	// You can compare the string representation of types for simplicity
	return t1.str() == t2.str()
}
