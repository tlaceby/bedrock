package parser

import (
	"fmt"

	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/helpers"
	"github.com/tlaceby/bedrock/src/lexer"
)

type type_nud_handler func(p *parser) ast.Type
type type_led_handler func(p *parser, left ast.Type, bp binding_power) ast.Type

type type_nud_lookup map[lexer.TokenKind]type_nud_handler
type type_led_lookup map[lexer.TokenKind]type_led_handler
type type_bp_lookup map[lexer.TokenKind]binding_power

var type_bp_lu = type_bp_lookup{}
var type_nud_lu = type_nud_lookup{}
var type_led_lu = type_led_lookup{}

func type_led(kind lexer.TokenKind, bp binding_power, led_fn type_led_handler) {
	type_bp_lu[kind] = bp
	type_led_lu[kind] = led_fn
}

func type_nud(kind lexer.TokenKind, nud_fn type_nud_handler) {
	type_bp_lu[kind] = primary
	type_nud_lu[kind] = nud_fn
}

func createTypeTokenLookups() {
	type_nud(lexer.IDENTIFIER, parse_symbol_type) // T
	type_nud(lexer.OPEN_BRACKET, parse_list_type) // []T
	type_nud(lexer.FN, parse_fn_type)             // fn (...[]Type) -> Type

	type_led(lexer.LESS, primary, parse_generic_type) // StructName<T, ..>
}

func parse_type(p *parser, bp binding_power) ast.Type {
	tokenKind := p.currentTokenKind()
	nud_fn, exists := type_nud_lu[tokenKind]

	if !exists {
		panic(fmt.Sprintf("type: NUD Handler expected for token %s\n", lexer.TokenKindString(tokenKind)))
	}

	left := nud_fn(p)

	for type_bp_lu[p.currentTokenKind()] > bp {
		tokenKind = p.currentTokenKind()
		led_fn, exists := type_led_lu[tokenKind]

		if !exists {
			panic(fmt.Sprintf("type: LED Handler expected for token %s\n", lexer.TokenKindString(tokenKind)))
		}

		left = led_fn(p, left, bp)
	}

	return left
}

func parse_generic_type(p *parser, left ast.Type, bp binding_power) ast.Type {
	p.expect(lexer.LESS)
	generics := make([]ast.Type, 0)

	for p.hasTokens() && p.currentTokenKind() != lexer.GREATER {
		genericType := parse_type(p, defalt_bp)
		generics = append(generics, genericType)

		if p.currentTokenKind() != lexer.GREATER {
			p.expect(lexer.COMMA)
		}
	}

	p.expect(lexer.GREATER)
	return ast.StructType{
		GenericList: generics,
		StructName:  helpers.ExpectType[ast.SymbolType](left).Value,
	}
}

func parse_symbol_type(p *parser) ast.Type {
	return ast.SymbolType{
		Value: p.advance().Value,
	}
}

func parse_list_type(p *parser) ast.Type {
	p.advance()
	p.expect(lexer.CLOSE_BRACKET)
	insideType := parse_type(p, defalt_bp)

	return ast.ListType{
		Underlying: insideType,
	}
}

func parse_fn_type(p *parser) ast.Type {
	var params = []ast.Type{}
	var returns ast.Type

	p.expect(lexer.FN)
	p.expect(lexer.OPEN_PAREN)
	for p.hasTokens() && p.currentTokenKind() != lexer.CLOSE_PAREN {
		params = append(params, parse_type(p, defalt_bp))

		if p.currentTokenKind() != lexer.CLOSE_PAREN {
			p.expect(lexer.COMMA)
		}
	}

	p.expect(lexer.CLOSE_PAREN)
	p.expect(lexer.ARROW)

	returns = parse_type(p, defalt_bp)
	return ast.FnType{
		Parameters: params,
		ReturnType: returns,
	}
}
