package analysis

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
)

func CreateTypechecker(defaultGlobalScope *ast.Scope) Typechecker {
	if defaultGlobalScope == nil {
		return Typechecker{Global: &ast.Scope{
			IsGlobal:  true,
			ScopeName: "global",
		}}
	}

	return Typechecker{
		Global: defaultGlobalScope,
	}
}

type TypeError struct {
	Kind    string
	Message string
}

type Typechecker struct {
	Errors  []TypeError
	Global  *ast.Scope
	Modules map[string]*ast.ModuleType
}

func (checker *Typechecker) Typecheck(node ast.ModuleStmt) ast.Type {
	module, exists := checker.Modules[node.ModuleName]

	if exists {
		fmt.Printf("module %s already typechecked\n", node.ModuleName)
		return module // todo get the already generated module and return it
	}

	module = &ast.ModuleType{
		ModuleName: node.ModuleName,
	}

	var scope = ast.CreateScope(checker.Global, node.ModuleName).SetIsModule()
	checker.Modules[node.ModuleName] = module

	for _, stmt := range node.Body {
		checker.stmt(stmt, scope)
	}

	return module
}
