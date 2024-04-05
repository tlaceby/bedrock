package analysis

import (
	"github.com/tlaceby/bedrock/src/ast"
)

func tc_symbol_type(t ast.SymbolType, env *SymbolTable) Type {
	_type, _ := env.findNearestTypeEnv(t.Value)
	return _type
}

func tc_list_type(t ast.ListType, env *SymbolTable) Type {
	panic("not IMPLIMENTED")
}

func tc_fn_type(t ast.FnType, env *SymbolTable) Type {
	panic("not IMPLIMENTED")
}
