package parser

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
	"github.com/tlaceby/bedrock/src/lexer"
)

func parse_stmt(p *parser) ast.Stmt {
	stmt_fn, exists := stmt_lu[p.currentTokenKind()]

	if exists {
		return stmt_fn(p)
	}

	return parse_expression_stmt(p)
}

func parse_expression_stmt(p *parser) ast.ExpressionStmt {
	expression := parse_expr(p, defalt_bp)
	p.expect(lexer.SEMI_COLON)

	return ast.ExpressionStmt{
		Expression: expression,
	}
}

func parse_block_stmt(p *parser) ast.Stmt {
	p.expect(lexer.OPEN_CURLY)
	body := []ast.Stmt{}

	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_CURLY {
		body = append(body, parse_stmt(p))
	}

	p.expect(lexer.CLOSE_CURLY)
	return ast.BlockStmt{
		Body: body,
	}
}

func parse_var_decl_stmt(p *parser) ast.Stmt {
	var explicitType ast.Type
	startToken := p.advance().Kind
	isConstant := startToken == lexer.CONST
	symbolName := p.expectError(lexer.IDENTIFIER,
		fmt.Sprintf("Following %s expected variable name however instead recieved %s instead\n",
			lexer.TokenKindString(startToken), lexer.TokenKindString(p.currentTokenKind())))

	if p.currentTokenKind() == lexer.COLON {
		p.expect(lexer.COLON)
		explicitType = parse_type(p, defalt_bp)
	}

	var assignmentValue ast.Expr
	if p.currentTokenKind() != lexer.SEMI_COLON {
		p.expect(lexer.ASSIGNMENT)
		assignmentValue = parse_expr(p, assignment)
	} else if explicitType == nil {
		panic("Missing explicit type for variable declaration.")
	}

	p.expect(lexer.SEMI_COLON)

	if isConstant && assignmentValue == nil {
		panic("Cannot define constant variable without providing default value.")
	}

	return ast.VarDeclarationStmt{
		Constant:      isConstant,
		Identifier:    symbolName.Value,
		AssignedValue: assignmentValue,
		ExplicitType:  explicitType,
	}
}

func parse_fn_params_and_body(p *parser) ([]ast.Parameter, ast.Type, []ast.Stmt) {
	functionParams := make([]ast.Parameter, 0)

	p.expect(lexer.OPEN_PAREN)
	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_PAREN {
		paramName := p.expect(lexer.IDENTIFIER).Value
		p.expect(lexer.COLON)
		paramType := parse_type(p, defalt_bp)

		functionParams = append(functionParams, ast.Parameter{
			Name: paramName,
			Type: paramType,
		})

		if !p.currentToken().IsOneOfMany(lexer.CLOSE_PAREN, lexer.EOF) {
			p.expect(lexer.COMMA)
		}
	}

	p.expect(lexer.CLOSE_PAREN)
	var returnType ast.Type

	if p.currentTokenKind() == lexer.ARROW {
		p.advance()
		returnType = parse_type(p, defalt_bp)
	}

	functionBody := ast.ExpectStmt[ast.BlockStmt](parse_block_stmt(p)).Body

	return functionParams, returnType, functionBody
}

func parse_fn_declaration(p *parser) ast.Stmt {
	p.expect(lexer.FN)
	functionName := p.expect(lexer.IDENTIFIER).Value
	var generics = []string{}

	if p.currentTokenKind() == lexer.OPEN_GENERIC {
		generics = parse_generic_declaration(p)
	}

	functionParams, returnType, functionBody := parse_fn_params_and_body(p)

	return ast.FunctionDeclarationStmt{
		Parameters: functionParams,
		Generics:   generics,
		ReturnType: returnType,
		Body:       functionBody,
		Name:       functionName,
	}
}

func parse_if_stmt(p *parser) ast.Stmt {
	p.advance()
	condition := parse_expr(p, assignment)
	consequent := parse_block_stmt(p)

	var alternate ast.Stmt
	if p.currentTokenKind() == lexer.ELSE {
		p.advance()

		if p.currentTokenKind() == lexer.IF {
			alternate = parse_if_stmt(p)
		} else {
			alternate = parse_block_stmt(p)
		}
	}

	return ast.IfStmt{
		Condition:  condition,
		Consequent: consequent,
		Alternate:  alternate,
	}
}

func parse_import_stmt(p *parser) ast.Stmt {
	p.advance()
	var importFrom string
	importName := p.expect(lexer.IDENTIFIER).Value

	if p.currentTokenKind() == lexer.FROM {
		p.advance()
		importFrom = p.expect(lexer.STRING).Value
	} else {
		importFrom = importName
	}

	p.expect(lexer.SEMI_COLON)
	return ast.ImportStmt{
		Name: importName,
		From: importFrom,
	}
}

func parse_for_stmt(p *parser) ast.Stmt {
	panic("For stmt syntax not implimented")
}

func parse_while_stmt(p *parser) ast.Stmt {
	p.expect(lexer.WHILE)
	p.expect(lexer.OPEN_PAREN)
	var cond = parse_expr(p, assignment)
	p.expect(lexer.CLOSE_PAREN)

	var body = helpers.ExpectType[ast.BlockStmt](parse_block_stmt(p)).Body
	return ast.WhileStmt{
		Condition: cond,
		Body:      body,
	}
}

func parse_generic_declaration(p *parser) []string {
	Generics := make([]string, 0)
	p.expect(lexer.OPEN_GENERIC)

	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_GENERIC {
		Generics = append(Generics, p.expect(lexer.IDENTIFIER).Value)

		if p.currentTokenKind() == lexer.COMMA {
			p.advance()
		}
	}

	p.expect(lexer.CLOSE_GENERIC)

	return Generics
}

func parse_struct_declaration_stmt(p *parser) ast.Stmt {
	p.expect(lexer.STRUCT)
	Name := p.expect(lexer.IDENTIFIER).Value
	Generics := make([]string, 0)
	Properties := make([]ast.StructProperty, 0)
	InstanceMethods := make([]ast.FunctionDeclarationStmt, 0)
	StaticMethods := make([]ast.FunctionDeclarationStmt, 0)

	if p.currentTokenKind() == lexer.OPEN_GENERIC {
		Generics = parse_generic_declaration(p)
	}

	p.expect(lexer.OPEN_CURLY)
	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_CURLY {

		// Check for identifier
		if p.currentTokenKind() == lexer.IDENTIFIER {
			PropertyName := p.expect(lexer.IDENTIFIER).Value
			p.expect(lexer.COLON)
			Type := parse_type(p, defalt_bp)
			p.expect(lexer.SEMI_COLON)

			Properties = append(Properties, ast.StructProperty{Name: PropertyName, Type: Type})
			continue
		}

		// Parse static methods
		if p.currentTokenKind() == lexer.STATIC {
			p.expect(lexer.STATIC)
			method := parse_fn_declaration(p)
			StaticMethods = append(StaticMethods, ast.ExpectStmt[ast.FunctionDeclarationStmt](method))
			continue
		}

		// Parse instance methods
		method := parse_fn_declaration(p)
		InstanceMethods = append(InstanceMethods, ast.ExpectStmt[ast.FunctionDeclarationStmt](method))
	}

	p.expect(lexer.CLOSE_CURLY)
	return ast.StructDeclarationStmt{
		Name: Name, InstanceMethods: InstanceMethods, StaticMethods: StaticMethods,
		Generics: Generics, Properties: Properties,
	}
}

func parse_return_stmt(p *parser) ast.Stmt {
	p.expect(lexer.RETURN)

	if p.currentTokenKind() == lexer.SEMI_COLON {
		p.expect(lexer.SEMI_COLON)
		return ast.ReturnStmt{} // returns void
	}

	return ast.ReturnStmt{
		ReturnValue: parse_expression_stmt(p).Expression,
	}
}

func parse_trait_stmt(p *parser) ast.Stmt {
	p.expect(lexer.TRAIT)
	var methods = []ast.TraitMethod{}
	traitName := p.expectError(lexer.IDENTIFIER, "Expected to find trait-name following `trait` keyword.").Value

	p.expect(lexer.OPEN_CURLY)

	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_CURLY {
		p.expect(lexer.FN)
		methodName := p.expect(lexer.IDENTIFIER).Value
		p.pos-- // set a new tmp token for the fn keywors
		p.tokens[p.pos] = lexer.Token{Kind: lexer.FN}
		methodType := ast.ExpectType[ast.FnType](parse_type(p, defalt_bp))

		methods = append(methods, ast.TraitMethod{
			MethodName: methodName,
			MethodType: methodType,
		})

		p.expect(lexer.SEMI_COLON)
	}

	p.expect(lexer.CLOSE_CURLY)
	return ast.TraitStmt{
		Methods: methods,
		Name:    traitName,
	}
}

func parse_match_stmt(p *parser) ast.Stmt {
	var identifierName string
	var cases = map[string]ast.MatchCase{}

	panic("Unimplimented Match Stmt parser")
	return ast.MatchStmt{
		Identifier: identifierName,
		Cases:      cases,
	}
}

func parse_unsafe_stmt(p *parser) ast.Stmt {
	panic("Unsafe impl parser")
	var body = []ast.Stmt{}
	return ast.UnsafeStmt{Body: body}
}

func parse_typedef_stmt(p *parser) ast.Stmt {
	var typeName string
	var typeType ast.Type

	panic("typedef unimplimented")

	return ast.TypedefStmt{
		Typename: typeName,
		Type:     typeType,
	}
}
