#pragma once
#include <functional>

#include "../ast/ast.h"
#include "../ast/ast_expr.h"
#include "../ast/ast_stmt.h"
#include "../ast/ast_type.h"
#include "../bedrock.h"
#include "../lexing/lexer.h"
#include "binding_power.h"

namespace parser {
extern bool PRATT_PARSER_SETUP;
struct Parser;

// Lookup Information ----------
// -----------------------------
using ast::Expr;
using ast::Stmt;
using ast::Type;
using std::function;

// Stmts & Expressions
typedef function<shared_ptr<Stmt>(Parser &)> stmt_handler;
typedef function<shared_ptr<Expr>(Parser &)> nud_handler;
typedef function<shared_ptr<Expr>(Parser &, shared_ptr<Expr>, BindingPower)> led_handler;

// Types
typedef function<shared_ptr<Type>(Parser &)> type_nud_handler;
typedef function<shared_ptr<Type>(Parser &, shared_ptr<Type>, BindingPower)> type_led_handler;

extern unordered_map<lexer::TokenKind, BindingPower> bp_lu;
extern unordered_map<lexer::TokenKind, BindingPower> type_bp_lu;

extern unordered_map<lexer::TokenKind, stmt_handler> stmt_lu;
extern unordered_map<lexer::TokenKind, nud_handler> nud_lu;
extern unordered_map<lexer::TokenKind, led_handler> led_lu;
extern unordered_map<lexer::TokenKind, type_nud_handler> type_nud_lu;
extern unordered_map<lexer::TokenKind, type_led_handler> type_led_lu;

void setup_pratt_parser();
void setup_type_pratt_parser();

void def_type_nud(lexer::TokenKind, type_nud_handler);
void def_type_led(lexer::TokenKind, type_led_handler, BindingPower);
void def_stmt(lexer::TokenKind, stmt_handler);
void def_led(lexer::TokenKind, led_handler, BindingPower);
void def_nud(lexer::TokenKind, nud_handler);

Err bad_lu_handler_err(string, lexer::Token);

type_nud_handler get_type_nud(Parser &p);
type_led_handler get_type_led(Parser &p);
led_handler get_led(Parser &p);
nud_handler get_nud(Parser &p);
}; // namespace parser