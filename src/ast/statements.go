package ast

type ModuleStmt struct {
	Scope      *Scope
	FilePath   string
	ModuleName string
	Body       []Stmt
}

func (s ModuleStmt) stmt() {}

type BlockStmt struct {
	Scope *Scope
	Body  []Stmt
}

func (b BlockStmt) stmt() {}

type VarDeclarationStmt struct {
	Identifier    string
	Constant      bool
	AssignedValue Expr
	ExplicitType  ASTType
}

func (n VarDeclarationStmt) stmt() {}

type ExpressionStmt struct {
	Expression Expr
}

func (n ExpressionStmt) stmt() {}

type Parameter struct {
	Name string
	Type ASTType
}

type FunctionDeclarationStmt struct {
	Scope      *Scope
	Parameters []Parameter
	Generics   []string
	Name       string
	Body       []Stmt
	ReturnType ASTType
}

func (n FunctionDeclarationStmt) stmt() {}

type IfStmt struct {
	Condition  Expr
	Consequent Stmt
	Alternate  Stmt
}

func (n IfStmt) stmt() {}

type ImportStmt struct {
	Name string
	From string
}

func (n ImportStmt) stmt() {}

// for (let/const varname, index in expr)
type ForStmt struct {
	IndexName string
	VarName   string
	IsConst   bool
	Iterable  Expr
}

func (n ForStmt) stmt() {}

type WhileStmt struct {
	Scope     *Scope
	Condition Expr
	Body      []Stmt
}

func (n WhileStmt) stmt() {}

type StructProperty struct {
	Name string
	Type ASTType
}

type StructMethod struct {
	Name string
	Args []Expr
}

type StructDeclarationStmt struct {
	Name            string
	Generics        []string
	Properties      []StructProperty
	StaticMethods   []FunctionDeclarationStmt
	InstanceMethods []FunctionDeclarationStmt
}

func (n StructDeclarationStmt) stmt() {}

type TraitMethod struct {
	MethodName string
	MethodType ASTFnType
}

type TraitStmt struct {
	Name    string
	Methods []TraitMethod
}

func (n TraitStmt) stmt() {}

type ReturnStmt struct {
	ReturnValue Expr
}

func (n ReturnStmt) stmt() {}

type MatchCase struct {
	Scope      *Scope
	Type       ASTType
	Block      []Stmt
	IsElseCase bool
}

type MatchStmt struct {
	Identifier string
	Cases      []MatchCase
}

func (n MatchStmt) stmt() {}

type UnsafeStmt struct {
	Scope *Scope
	Body  []Stmt
}

func (n UnsafeStmt) stmt() {}

type TypedefStmt struct {
	Typename string
	Type     ASTType
}

func (n TypedefStmt) stmt() {}
