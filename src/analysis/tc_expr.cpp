#include "analsyis.h"

using namespace analysis;
using namespace ast;

shared_ptr<analysis::Type> analysis::tc(shared_ptr<ast::Expr> expr,
                                        shared_ptr<analysis::Scope> env) {
  switch (expr->kind) {
    case NUMBER_EXPR: {
      auto precision =
          matchPrecisionFromToken(static_cast<NumberExpr*>(expr.get())->value);
      return MK_NUM(precision);
    }
    case STRING_EXPR:
      return MK_STR();

    default:
      expr->debug(0);
      std::cout << "^^^^^ typechecking for node Unimplimented ^^^^^^";
      TODO("Unimplimented Typechecking for stmt");
      break;
  }
}

shared_ptr<analysis::Type> analysis::tc_binary_expr(
    ast::BinaryExpr* expr, shared_ptr<analysis::Scope> env) {
  //
}

shared_ptr<analysis::Type> analysis::tc_prefix_expr(
    ast::PrefixExpr* expr, shared_ptr<analysis::Scope> env) {
  //
}

shared_ptr<analysis::Type> analysis::tc_assignment_expr(
    ast::AssignmentExpr* expr, shared_ptr<analysis::Scope> env) {
  //
}
