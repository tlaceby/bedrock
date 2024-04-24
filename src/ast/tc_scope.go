package ast

type Symbol struct {
	Type            Type // Underlying type of symbol
	IsConstant      bool // Whether the symbol is defined as a constant or fn/module etc...
	ClosedOver      bool // Whether this variable will be used inside a closure and needs heap allocation
	AssignmentCount int  // Number of times this symbol is assigned/reassigned
	AccessedCount   int  // Number of times symbol is accessed/referenced
}

type Scope struct {
	ScopeName string
	Parent    *Scope

	IsGlobal       bool              // whether this table exists in the global scope
	IsModule       bool              // Represents the module which this table exists in
	IsFunction     bool              // whether this table is the body of a function
	IsStaticMethod bool              // whether we are inside a static methods scope. Prevents access to instance methods/properties from inside
	IsLoop         bool              // Whether the current env is a while/for loop
	IsUnsafe       bool              // Inside a unsafe block
	DefinedTypes   map[string]Type   // All Alias/ Defined Types / Traits
	Symbols        map[string]Symbol // All Variables/Functions/Modlues

	FoundReturnTypes []Type // Inside functions keep track of found return types
}

func CreateScope(parent *Scope, name string) *Scope {
	return &Scope{
		ScopeName: name,
		Parent:    parent,
	}
}

func (scope *Scope) SetIsGlobal() *Scope {
	scope.IsGlobal = true
	return scope
}

func (scope *Scope) SetIsModule() *Scope {
	scope.IsModule = true
	return scope
}

func (scope *Scope) SetIsFunction() *Scope {
	scope.IsFunction = true
	return scope
}

func (scope *Scope) SetIsStatic() *Scope {
	scope.IsStaticMethod = true
	return scope
}

func (scope *Scope) SetIsLoop() *Scope {
	scope.IsLoop = true
	return scope
}

func (scope *Scope) SetIsUnsafe() *Scope {
	scope.IsUnsafe = true
	return scope
}
