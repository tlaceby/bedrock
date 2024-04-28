#include "lookup.h"
#include "parser.h"

using namespace parser;
using namespace ast;
using namespace lexer;

std::unordered_map<lexer::TokenKind, type_nud_handler> parser::type_nud_lu{};
std::unordered_map<lexer::TokenKind, type_led_handler> parser::type_led_lu{};
std::unordered_map<lexer::TokenKind, BindingPower> parser::bp_lu{};
std::unordered_map<lexer::TokenKind, nud_handler> parser::nud_lu{};
std::unordered_map<lexer::TokenKind, BindingPower> parser::type_bp_lu{};
std::unordered_map<lexer::TokenKind, led_handler> parser::led_lu{};
std::unordered_map<lexer::TokenKind, stmt_handler> parser::stmt_lu{};

void parser::def_type_nud(TokenKind k, type_nud_handler h) {
  type_nud_lu[k] = h;  //
}

void parser::def_type_led(TokenKind k, type_led_handler h, BindingPower bp) {
  type_bp_lu[k] = bp;
  type_led_lu[k] = h;
}

void parser::def_stmt(TokenKind kind, stmt_handler handler) {
  stmt_lu[kind] = handler;  //
}

void parser::def_led(TokenKind kind, led_handler handler, BindingPower bp) {
  led_lu[kind] = handler;
  bp_lu[kind] = bp;
}

void parser::def_nud(TokenKind kind, nud_handler handler) {
  nud_lu[kind] = handler;  //
}

inline bool parser::PRATT_PARSER_SETUP = false;
void parser::setup_pratt_parser() {
  if (PRATT_PARSER_SETUP) return;
  PRATT_PARSER_SETUP = true;
  setup_type_pratt_parser();

  // Stmts
  def_stmt(lexer::OPEN_CURLY, parse_block_stmt);

  // NUD HANDLERS
  def_nud(lexer::OPEN_PAREN, parse_grouping_expr);
  def_nud(lexer::MINUS, parse_prefix_expr);
  def_nud(lexer::NOT, parse_prefix_expr);
  def_nud(lexer::NUMBER, parse_primary_expr);
  def_nud(lexer::STRING, parse_primary_expr);
  def_nud(lexer::IDENTIFIER, parse_primary_expr);

  // LED HANLDERS
  def_led(lexer::PLUS, parse_binary_expr, ADDITIVE_BP);
  def_led(lexer::MINUS, parse_binary_expr, ADDITIVE_BP);

  def_led(lexer::STAR, parse_binary_expr, MULTIPLICATIVE_BP);
  def_led(lexer::SLASH, parse_binary_expr, MULTIPLICATIVE_BP);
  def_led(lexer::PERCENT, parse_binary_expr, MULTIPLICATIVE_BP);
}

void parser::setup_type_pratt_parser() {
  //
}