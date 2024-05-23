#include "analsyis.h"

using namespace analysis;
using namespace ast;

shared_ptr<analysis::Type> analysis::tc_expr(shared_ptr<ast::Expr> expr, shared_ptr<analysis::Scope> env) {
  switch (expr->kind) {
  case NUMBER_EXPR:
    return MK_NUM();
  case STRING_EXPR:
    return MK_STR();
  case PREFIX_EXPR:
    return tc_prefix_expr(static_cast<PrefixExpr *>(expr.get()), env);
  case ASSIGN_EXPR:
    return tc_assignment_expr(static_cast<AssignmentExpr *>(expr.get()), env);
    // case BINARY_EXPR:
    //   return tc_binary_expr(static_cast<BinaryExpr*>(expr.get()), env);

  default:
    expr->debug(0);
    std::cout << "^^^^^ typechecking for node Unimplimented ^^^^^^\n";
    TODO("Unimplimented Typechecking for expr");
    break;
  }
}

shared_ptr<analysis::Type> analysis::tc_binary_expr(ast::BinaryExpr *expr, shared_ptr<analysis::Scope> env) {
  //
}

shared_ptr<analysis::Type> analysis::tc_prefix_expr(ast::PrefixExpr *expr, shared_ptr<analysis::Scope> env) {
  auto rhs = tc_expr(expr->right, env);
  auto opKind = expr->operation.kind;

  switch (opKind) {
  case lexer::AMPERSAND:
    return MK_PTR(rhs);
  case lexer::PLUS:
  case lexer::MINUS: {
    if (rhs->kind == NUMBER) {
      return rhs;
    }
  }
  case lexer::NOT: {
    return MK_BOOL();
  }
  }

  // If type is not a number invalid operation
  std::cout << "Invalid prefix operation " << expr->operation.value;
  std::cout << " with type " << rhs->str() << "\n";
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_assignment_expr(ast::AssignmentExpr *expr, shared_ptr<analysis::Scope> env) {
  auto rhs = tc_expr(expr->value, env);

  // varname = Expr | Variable Assignment
  if (expr->assigne->kind == SYMBOL_EXPR) {
    string varname = static_cast<SymbolExpr *>(expr->assigne.get())->symbol;

    // Make sure variable exists
    if (!env->symbolExists(varname)) {
      std::cout << "Invalid assignment operation. ";
      std::cout << "Variable " << magenta(varname);
      std::cout << " does not exist.\n";
      exit(1);
    }

    auto expectedType = env->resolveSymbol(varname);
    if (types_match(expectedType, rhs)) {
      return rhs;
    }

    std::cout << "Invalid assignment operation. Variable " << magenta(varname);
    std::cout << " expected type: " << expectedType->str() << " but";
    std::cout << " recieved " << rhs->str() << " instead\n";
    exit(1);
  }

  // *varname = Expr | Pointer Deference Assignment
  if (expr->assigne->kind == PREFIX_EXPR) {
    auto prefix = static_cast<PrefixExpr *>(expr->assigne.get());
    bool isPtrOperation = prefix->operation.kind == lexer::STAR;

    // *Varname = Expr
    if (isPtrOperation && prefix->right->kind == SYMBOL_EXPR) {
      string varname = static_cast<SymbolExpr *>(prefix->right.get())->symbol;

      // Make sure variable exists
      if (!env->symbolExists(varname)) {
        std::cout << "Invalid assignment operation. ";
        std::cout << "Variable " << magenta(varname);
        std::cout << " does not exist.\n";
        exit(1);
      }

      // Make sure variable holds a pointer to T
      auto ptrType = env->resolveSymbol(varname);
      if (ptrType->kind != POINTER) {
        std::cout << "Invalid pointer assignment operation. Variable ";
        std::cout << varname << " does not hold a pointer\n";
        exit(1);
      }

      auto ptrUnderlying = static_cast<PointerType *>(ptrType.get())->underlying;
      // Verify the rhs type matches that of what varname is pointing too
      if (types_match(ptrUnderlying, rhs)) {
        return rhs;
      }

      std::cout << "Invalid pointer assignment operation. ";
      std::cout << "Variable " << magenta(varname) << " points to ";
      std::cout << ptrUnderlying->str() << " but";
      std::cout << " recieved " << rhs->str() << " instead\n";
      exit(1);
    }

    std::cout << "Invalid assignment operation. *Varname = Expr expected\n";
    std::cout << "Invalid prefix on lhs\n";
    exit(1);
  }
}
