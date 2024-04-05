package analysis

import "github.com/tlaceby/bedrock/src/ast"

func tc_block_stmt(s ast.BlockStmt, env *SymbolTable) Type {
	var last Type
	env = CreateSymbolTable(env, false)
	for _, stmt := range s.Body {
		last = typecheck_stmt(stmt, env)
	}

	return last
}
