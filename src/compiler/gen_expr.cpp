#include "../analysis/types.h"
#include "../ast/ast_expr.h"
#include "gen.h"

using namespace compiler;
using namespace analysis;
using namespace ast;

string compiler::compile(shared_ptr<Expr> expr, shared_ptr<Scope> scope) {
  switch (expr->kind) {
    case NUMBER_EXPR:
      return compile_number_expr(static_cast<NumberExpr*>(expr.get()), scope);
    default:
      expr->debug(1);
      TODO("^^^^^^^^ Unimplimented COMPILE PATH: ^^^^^^^");
  }

  return "UNKNOWN";
}

string compiler::compile_symbol_expr(SymbolExpr* symbol,
                                     shared_ptr<Scope> scope) {
  return symbol->symbol;
}

string compiler::compile_number_expr(NumberExpr* number,
                                     shared_ptr<Scope> scope) {
  return number->value;
}

string compiler::compile_string_expr(StringExpr* str, shared_ptr<Scope> scope) {
  return "\"" + str->value + "\"\\0";
}

string compiler::compile_prefix_expr(PrefixExpr* prefix,
                                     shared_ptr<Scope> scope) {
  UNUSED(prefix);
  return "";
}

string compiler::compile_assignment_expr(AssignmentExpr* expr,
                                         shared_ptr<Scope> scope) {
  UNUSED(expr);
  return "";
}
