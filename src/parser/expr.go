package parser

import (
	"fmt"
	"strconv"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/lexer"
)

func parse_expr(p *parser, bp binding_power) ast.Expr {
	tokenKind := p.currentTokenKind()
	nud_fn, exists := nud_lu[tokenKind]

	if !exists {
		panic(fmt.Sprintf("NUD Handler expected for token %s\n", p.currentToken().String()))
	}

	left := nud_fn(p)

	for bp_lu[p.currentTokenKind()] > bp {
		tokenKind = p.currentTokenKind()
		led_fn, exists := led_lu[tokenKind]

		if !exists {
			panic(fmt.Sprintf("LED Handler expected for token %s\n", p.currentToken().String()))
		}

		left = led_fn(p, left, bp_lu[p.currentTokenKind()])
	}

	return left
}

func parse_prefix_expr(p *parser) ast.Expr {
	operatorToken := p.advance()
	expr := parse_expr(p, unary)

	return ast.PrefixExpr{
		Operator: operatorToken,
		Right:    expr,
	}
}

func parse_assignment_expr(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	p.advance()
	rhs := parse_expr(p, bp)

	return ast.AssignmentExpr{
		Assigne:       left,
		AssignedValue: rhs,
	}
}

func parse_range_expr(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	p.advance()

	return ast.RangeExpr{
		Lower: left,
		Upper: parse_expr(p, bp),
	}
}

func parse_binary_expr(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	operatorToken := p.advance()
	right := parse_expr(p, bp)

	return ast.BinaryExpr{
		Left:     left,
		Operator: operatorToken,
		Right:    right,
	}
}

func parse_primary_expr(p *parser) ast.Expr {
	switch p.currentTokenKind() {
	case lexer.NUMBER:
		number, _ := strconv.ParseFloat(p.advance().Value, 64)
		return ast.NumberExpr{
			Value: number,
		}
	case lexer.STRING:
		return ast.StringExpr{
			Value: p.advance().Value,
		}
	case lexer.IDENTIFIER:
		return ast.SymbolExpr{
			Value: p.advance().Value,
		}
	default:
		panic(fmt.Sprintf("Cannot create primary_expr from %s\n", lexer.TokenKindString(p.currentTokenKind())))
	}
}

func parse_member_expr(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	isComputed := p.advance().Kind == lexer.OPEN_BRACKET

	if isComputed {
		rhs := parse_expr(p, bp)
		p.expect(lexer.CLOSE_BRACKET)
		return ast.ComputedExpr{
			Member:   left,
			Property: rhs,
		}
	}

	return ast.MemberExpr{
		Member:   left,
		Property: p.expect(lexer.IDENTIFIER).Value,
	}
}

func parse_static_member_expr(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	p.expect(lexer.COLON_COLON)

	memberName := ast.ExpectExpr[ast.SymbolExpr](left).Value

	return ast.StaticMemberExpr{
		StructName: memberName,
		MethodName: p.expect(lexer.IDENTIFIER).Value,
	}
}

func parse_array_literal_expr(p *parser) ast.Expr {
	p.expect(lexer.OPEN_BRACKET)
	arrayContents := make([]ast.Expr, 0)

	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_BRACKET {
		arrayContents = append(arrayContents, parse_expr(p, logical))

		if !p.currentToken().IsOneOfMany(lexer.EOF, lexer.CLOSE_BRACKET) {
			p.expect(lexer.COMMA)
		}
	}

	p.expect(lexer.CLOSE_BRACKET)

	return ast.ArrayLiteral{
		Contents: arrayContents,
	}
}

func parse_grouping_expr(p *parser) ast.Expr {
	p.expect(lexer.OPEN_PAREN)
	expr := parse_expr(p, defalt_bp)
	p.expect(lexer.OPEN_PAREN)
	return expr
}

func parse_call_expr(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	p.expect(lexer.OPEN_PAREN)
	arguments := make([]ast.Expr, 0)

	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_PAREN {
		arguments = append(arguments, parse_expr(p, assignment))

		if p.currentTokenKind() != lexer.CLOSE_PAREN {
			p.expect(lexer.COMMA)
		}
	}

	p.expect(lexer.CLOSE_PAREN)
	return ast.CallExpr{
		Method:    left,
		Arguments: arguments,
	}
}

func parse_fn_expr(p *parser) ast.Expr {
	p.expect(lexer.FN)
	functionParams, returnType, functionBody := parse_fn_params_and_body(p)

	return ast.FunctionExpr{
		Parameters: functionParams,
		ReturnType: returnType,
		Body:       functionBody,
	}
}

func parse_struct_instantiation(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	p.expect(lexer.OPEN_CURLY)
	structName := ast.ExpectExpr[ast.SymbolExpr](left).Value
	objects := make([]ast.ObjectField, 0)

	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_CURLY {
		key := p.expect(lexer.IDENTIFIER).Value
		var explicitValue ast.Expr

		// this is the last value as we require a ending comma to terminate the struct literal
		if p.currentTokenKind() == lexer.COMMA {
			objects = append(objects, ast.ObjectField{
				PropertyName:  key,
				PropertyValue: explicitValue,
			})

			p.advance() // eat the comma
			continue
		}

		p.expect(lexer.COLON)
		explicitValue = parse_expr(p, assignment)
		objects = append(objects, ast.ObjectField{
			PropertyName:  key,
			PropertyValue: explicitValue,
		})

		p.expect(lexer.COMMA)
	}

	p.expect(lexer.CLOSE_CURLY)

	return ast.StructInstantiationExpr{
		StructName: structName,
		Objects:    objects,
	}
}

func parse_generic_list_instantiation(p *parser, left ast.Expr, bp binding_power) ast.Expr {
	var genericLists = []ast.Type{}
	p.expect(lexer.LESS)

	for p.hasTokens() && p.currentTokenKind() != lexer.GREATER {
		genericLists = append(genericLists, parse_type(p, defalt_bp))

		if p.currentTokenKind() != lexer.GREATER {
			p.expect(lexer.COMMA)
		}
	}

	p.expect(lexer.GREATER)

	// Handle call expression -> method <T, A, ...> ()
	if p.currentTokenKind() == lexer.OPEN_PAREN {
		callExprNode := ast.ExpectExpr[ast.CallExpr](parse_call_expr(p, left, call))
		callExprNode.Generics = genericLists

		return callExprNode
	}

	// Handle struct instantiation -> Struct <T> {}
	if p.currentTokenKind() == lexer.OPEN_CURLY {
		structNode := ast.ExpectExpr[ast.StructInstantiationExpr](parse_struct_instantiation(p, left, call))
		structNode.Generics = genericLists

		return structNode
	}

	// Handle Static Instance w/ Generics -> Struct <T>::staticMethod();
	if p.currentTokenKind() == lexer.COLON_COLON {
		staticInstantiation := ast.ExpectExpr[ast.StaticMemberExpr](parse_static_member_expr(p, left, call))
		staticInstantiation.StructGenerics = genericLists

		return staticInstantiation
	}

	panic("Unknown symbol after generics list inside parse_generic_list_instantiation()")
}
