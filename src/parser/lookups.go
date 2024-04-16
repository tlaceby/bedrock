package parser

import (
	"github.com/tlaceby/bedrock/src/ast"
	"github.com/tlaceby/bedrock/src/lexer"
)

type binding_power int

const (
	defalt_bp binding_power = iota
	comma
	assignment
	logical
	relational
	additive
	multiplicative
	unary
	call
	member
	primary
)

type stmt_handler func(p *parser) ast.Stmt
type nud_handler func(p *parser) ast.Expr
type led_handler func(p *parser, left ast.Expr, bp binding_power) ast.Expr

type stmt_lookup map[lexer.TokenKind]stmt_handler
type nud_lookup map[lexer.TokenKind]nud_handler
type led_lookup map[lexer.TokenKind]led_handler
type bp_lookup map[lexer.TokenKind]binding_power

var bp_lu = bp_lookup{}
var nud_lu = nud_lookup{}
var led_lu = led_lookup{}
var stmt_lu = stmt_lookup{}

func led(kind lexer.TokenKind, bp binding_power, led_fn led_handler) {
	bp_lu[kind] = bp
	led_lu[kind] = led_fn
}

func nud(kind lexer.TokenKind, nud_fn nud_handler) {
	nud_lu[kind] = nud_fn
}

func stmt(kind lexer.TokenKind, stmt_fn stmt_handler) {
	stmt_lu[kind] = stmt_fn
}

func createTokenLookups() {
	// Assignment
	led(lexer.ASSIGNMENT, assignment, parse_assignment_expr)
	led(lexer.PLUS_EQUALS, assignment, parse_assignment_expr)
	led(lexer.MINUS_EQUALS, assignment, parse_assignment_expr)
	led(lexer.SLASH_EQUALS, assignment, parse_assignment_expr)
	led(lexer.STAR_EQUALS, assignment, parse_assignment_expr)

	// Logical
	led(lexer.AND, logical, parse_binary_expr)
	led(lexer.OR, logical, parse_binary_expr)
	led(lexer.DOT_DOT, logical, parse_range_expr)

	// Relational
	led(lexer.LESS, relational, parse_binary_expr)
	led(lexer.LESS_EQUALS, relational, parse_binary_expr)
	led(lexer.GREATER, relational, parse_binary_expr)
	led(lexer.GREATER_EQUALS, relational, parse_binary_expr)
	led(lexer.EQUALS, relational, parse_binary_expr)
	led(lexer.NOT_EQUALS, relational, parse_binary_expr)

	// Additive & Multiplicitave
	led(lexer.PLUS, additive, parse_binary_expr)
	led(lexer.DASH, additive, parse_binary_expr)
	led(lexer.SLASH, multiplicative, parse_binary_expr)
	led(lexer.STAR, multiplicative, parse_binary_expr)
	led(lexer.PERCENT, multiplicative, parse_binary_expr)

	// Literals & Symbols
	nud(lexer.NUMBER, parse_primary_expr)
	nud(lexer.STRING, parse_primary_expr)
	nud(lexer.IDENTIFIER, parse_primary_expr)

	// Unary/Prefix
	nud(lexer.TYPEOF, parse_prefix_expr)
	nud(lexer.DASH, parse_prefix_expr)
	nud(lexer.NOT, parse_prefix_expr)
	nud(lexer.OPEN_BRACKET, parse_array_literal_expr)

	// Member / Computed / Call
	led(lexer.DOT, member, parse_member_expr)
	led(lexer.COLON_COLON, member, parse_static_member_expr)
	led(lexer.OPEN_BRACKET, member, parse_member_expr)
	led(lexer.OPEN_PAREN, call, parse_call_expr)
	led(lexer.OPEN_CURLY, call, parse_struct_instantiation)
	led(lexer.OPEN_GENERIC, call, parse_generic_list_instantiation)

	// Grouping Expr
	nud(lexer.OPEN_PAREN, parse_grouping_expr)
	nud(lexer.FN, parse_fn_expr)

	// Macros
	nud(lexer.ASSERT_MACRO, parse_assert_macro)
	nud(lexer.STRING_MACRO, parse_string_macro)

	stmt(lexer.OPEN_CURLY, parse_block_stmt)
	stmt(lexer.LET, parse_var_decl_stmt)
	stmt(lexer.CONST, parse_var_decl_stmt)
	stmt(lexer.FN, parse_fn_declaration)
	stmt(lexer.IF, parse_if_stmt)
	stmt(lexer.IMPORT, parse_import_stmt)
	stmt(lexer.FOR, parse_for_stmt)
	stmt(lexer.WHILE, parse_while_stmt)
	stmt(lexer.STRUCT, parse_struct_declaration_stmt)
	stmt(lexer.RETURN, parse_return_stmt)
	stmt(lexer.TRAIT, parse_trait_stmt)
	stmt(lexer.MATCH, parse_match_stmt)
	stmt(lexer.UNSAFE, parse_unsafe_stmt)
	stmt(lexer.TYPEDEF, parse_typedef_stmt)
}
