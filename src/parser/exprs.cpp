#include "lookup.h"
#include "parser.h"

using namespace parser;
using namespace ast;
using namespace lexer;

Err parser::bad_lu_handler_err(string hname, Token tk) {
  auto err = Err(ErrKind::UnexpectedToken);
  err.message("Cannot parse expression at current token " +
              bold_red(token_tag(tk.kind)));
  err.hint("This token is not supported in the current context.");
  err.hint("Could be a parser::" + hname + "::error");
  err.location(tk.pos);

  return err;
}

led_handler parser::get_led(Parser& p) {
  auto tk = p.current_tk();
  auto it = led_lu.find(tk.kind);

  if (it != led_lu.end()) {
    return led_lu.at(tk.kind);
  }

  auto err = bad_lu_handler_err("led()", tk);
  p.expect();  // advance past char since it has no null
  p.report(err);
  return nullptr;
}

nud_handler parser::get_nud(Parser& p) {
  auto tk = p.current_tk();
  auto it = nud_lu.find(tk.kind);

  if (it != nud_lu.end()) {
    return nud_lu.at(tk.kind);
  }

  auto err = bad_lu_handler_err("nud()", tk);
  p.expect();  // advance past char since it has no null
  p.report(err);
  return nullptr;
}

shared_ptr<ast::Expr> parser::parse_expr(Parser& p, BindingPower bp) {
  shared_ptr<Expr> left;
  nud_handler nud = get_nud(p);

  left = nud(p);

  while (bp_lu[p.current_tk_kind()] > bp) {
    led_handler led = get_led(p);
    left = led(p, left, bp_lu[p.current_tk_kind()]);
  }

  return left;
}

shared_ptr<ast::Expr> parser::parse_primary_expr(Parser& p) {
  auto tk = p.current_tk();
  switch (tk.kind) {
    case IDENTIFIER: {
      auto expr = make_shared<SymbolExpr>();
      expr->symbol = p.expect().value;
      return expr;
    }

    case NUMBER: {
      auto expr = make_shared<NumberExpr>();
      expr->value = std::stod(p.expect().value);
      return expr;
    }

    case STRING: {
      auto expr = make_shared<StringExpr>();
      expr->value = p.expect().value;
      return expr;
    }

    default:
      auto err = Err(ErrKind::ExpectedPrimaryExpr);
      err.message("Could not parse primary-expression.");
      err.hint("The expected token should be that of [NUMBER|STRING|IDENT]");
      err.hint("This is likely an issue with the compiler.");
      err.location(tk.pos);

      p.report(err);
      return nullptr;
  }
}

shared_ptr<ast::BinaryExpr> parser::parse_binary_expr(Parser& p,
                                                      shared_ptr<Expr> left,
                                                      BindingPower bp) {
  auto binary_expr = make_shared<BinaryExpr>();
  binary_expr->operation = p.expect();
  binary_expr->left = left;
  binary_expr->right = parse_expr(p, bp);
  return binary_expr;
}

shared_ptr<ast::PrefixExpr> parser::parse_prefix_expr(Parser& p) {
  auto expr = make_shared<PrefixExpr>();
  expr->operation = p.expect();
  expr->right = parse_expr(p, BindingPower::UNARY_BP);

  return expr;
}

shared_ptr<ast::Expr> parser::parse_grouping_expr(Parser& p) {
  p.expect(lexer::OPEN_PAREN);
  auto expr = parse_expr(p, DEFAULT_BP);
  p.expect(lexer::CLOSE_PAREN);
  return expr;
}
