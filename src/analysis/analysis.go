package analysis

import (
	"fmt"

	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
)

var numTables int = 0

type Type interface {
	str() string // string representation of Abstract Type
}

type SymbolInfo struct {
	Type            Type // Underlying type of symbol
	IsConstant      bool // Whether the symbol is defined as a constant or fn/module etc...
	AssignmentCount int  // Number of times this symbol is assigned/reassigned
	AccessedCount   int  // Number of times symbol is accessed/referenced
}

type SymbolTable struct {
	TableName string // the chained name of the current environment
	TableID   int    // ID assigned to table

	IsGlobal       bool // whether this table exists in the global scope
	IsModule       bool // Represents the module which this table exists in
	IsFunction     bool // whether this table is the body of a function
	IsStaticMethod bool // whether we are inside a static methods scope. Prevents access to instance methods/properties from inside

	Parent       *SymbolTable          // Reference to parent environment
	DefinedTypes map[string]Type       // All Alias/ Defined Types / Traits
	Symbols      map[string]SymbolInfo // All Variables/Functions/Modlues

	FoundReturnTypes []Type // Inside functions keep track of found return types
}

func CreateSymbolTable(parent *SymbolTable, fn bool, isModule bool, isStruct bool, givenTableName string) *SymbolTable {
	isGlobal := true

	if parent != nil {
		isGlobal = false
	}

	numTables += 1
	table := &SymbolTable{
		TableID:      numTables,
		Parent:       parent,
		IsGlobal:     isGlobal,
		IsModule:     isModule,
		IsFunction:   fn,
		Symbols:      map[string]SymbolInfo{},
		DefinedTypes: map[string]Type{},
	}

	// Generate Table Name
	if table.Parent != nil {
		table.TableName = fmt.Sprintf("%s.%s", table.Parent.TableName, givenTableName)
	} else {
		table.TableName = givenTableName
	}

	if table.IsGlobal {
		defineGlobalDefaultTypes(table)
		table.TableName = "global"
	}

	return table
}

func typecheck_expr(expr ast.Expr, env *SymbolTable) Type {
	switch e := expr.(type) {
	case ast.NumberExpr:
		return NumType{}
	case ast.StringExpr:
		return StrType{}
	case ast.SymbolExpr:
		return tc_symbol_expr(e, env)
	case ast.BinaryExpr:
		return tc_binary_expr(e, env)
	case ast.MemberExpr:
		return tc_member_expr(e, env)
	case ast.StaticMemberExpr:
		return tc_static_member_expr(e, env)
	case ast.CallExpr:
		return tc_call_expr(e, env)
	case ast.StructInstantiationExpr:
		return tc_struct_instantation_expr(e, env)
	default:
		litter.Dump(expr)
		panic("^^^^^^ Unknown ast.Expr encountered! ^^^^^^\n")
	}
}

func typecheck_type(_type ast.Type, env *SymbolTable) Type {
	switch t := _type.(type) {
	case ast.SymbolType:
		return tc_symbol_type(t, env)
	case ast.ListType:
		return tc_list_type(t, env)
	case ast.FnType:
		return tc_fn_type(t, env)
	default:
		litter.Dump(t)
		panic("^^^^^^ Unknown ast.Type encountered! ^^^^^^\n")
	}
}

func typecheck_stmt(stmt ast.Stmt, env *SymbolTable) Type {
	switch s := stmt.(type) {
	case ast.ModuleStmt:
		return tc_module_stmt(s, env)
	case ast.BlockStmt:
		return tc_block_stmt(s, env)
	case ast.VarDeclarationStmt:
		return tc_var_declaration_stmt(s, env)
	case ast.ExpressionStmt:
		return typecheck_expr(s.Expression, env)
	case ast.FunctionDeclarationStmt:
		return tc_fn_declaration_stmt(s, env)
	case ast.ReturnStmt:
		return tc_return_stmt(s, env)
	case ast.StructDeclarationStmt:
		return tc_struct_declaration_stmt(s, env)
	default:
		litter.Dump(stmt)
		panic("^^^^^^ Unknown ast.Stmt encountered! ^^^^^^\n")
	}
}

func Typecheck(module ast.ModuleStmt, globalEnv *SymbolTable) Type {
	t := typecheck_stmt(module, globalEnv)
	return t
}

// ------------------------
// Helpers for symbol table
// ------------------------

func (table *SymbolTable) findNearestFunctionEnv() *SymbolTable {
	if table.IsFunction {
		return table
	}

	if table.Parent != nil {
		return table.Parent.findNearestFunctionEnv()
	}

	return nil
}

func (table *SymbolTable) findNearestSymbolEnv(symbolName string) (*SymbolInfo, *SymbolTable) {
	t, existsInCurrentTable := table.Symbols[symbolName]

	if existsInCurrentTable {
		return &t, table
	}

	if table.Parent != nil {
		return table.Parent.findNearestSymbolEnv(symbolName)
	}

	return nil, nil
}

func (table *SymbolTable) findNearestTypeEnv(typeName string) (Type, *SymbolTable) {
	t, existsInCurrentTable := table.DefinedTypes[typeName]

	if existsInCurrentTable {
		return t, table
	}

	if table.Parent != nil {
		return table.Parent.findNearestTypeEnv(typeName)
	}

	return ErrType(fmt.Sprintf("Type %s does not exist in the current env", typeName)), nil
}

func (table *SymbolTable) debugTable(printParent bool) {

	if printParent && table.Parent != nil {
		table.Parent.debugTable(printParent)
	}

	println(fmt.Sprintf("\n------------   %s   ------------ \n", table.TableName))
	println("types:")
	for typename, typevalue := range table.DefinedTypes {
		println(fmt.Sprintf(" %s  -> %s", typename, typevalue.str()))
	}

	println("\nstructs:")
	for typename, typevalue := range table.DefinedTypes {
		if helpers.TypesMatchT[StructType](typevalue) {
			println(fmt.Sprintf(" %s: ", typename))

			structVal := helpers.ExpectType[StructType](typevalue)

			if len(structVal.Properties) > 0 {
				for propertyName, propertyType := range structVal.Properties {
					println(fmt.Sprintf("   %s : %s", propertyName, propertyType.str()))
				}
			}

			if len(structVal.Methods) > 0 {
				for propertyName, methodType := range structVal.Methods {
					println(fmt.Sprintf("   %s : %s", propertyName, methodType.str()))
				}
			}

			if len(structVal.StaticMethods) > 0 {
				for propertyName, staticMethod := range structVal.StaticMethods {
					println(fmt.Sprintf("   %s : static %s", propertyName, staticMethod.str()))
				}
			}
		}
	}

	println("\nsymbols:")
	for symbol, symbolInfo := range table.Symbols {
		println(fmt.Sprintf(" %s: ", symbol))
		println(fmt.Sprintf("  type     : %s", symbolInfo.Type.str()))

		if symbolInfo.IsConstant {
			println("  const    : true")
		} else {
			println("  const    : false")
		}

		println(fmt.Sprintf("  accessed : %d", symbolInfo.AccessedCount))
		println(fmt.Sprintf("  assigned : %d\n", symbolInfo.AssignmentCount))
	}
}
