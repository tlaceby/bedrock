#include "analsyis.h"

using namespace analysis;
using namespace ast;

shared_ptr<analysis::Type> analysis::tc_expr(shared_ptr<ast::Expr> expr, shared_ptr<analysis::Scope> env) {
  switch (expr->kind) {
  case NUMBER_EXPR:
    return MK_NUM();
  case STRING_EXPR:
    return MK_STR();
  case SYMBOL_EXPR:
    return tc_symbol_expr(static_cast<SymbolExpr *>(expr.get()), env);
  case PREFIX_EXPR:
    return tc_prefix_expr(static_cast<PrefixExpr *>(expr.get()), env);
  case ASSIGN_EXPR:
    return tc_assignment_expr(static_cast<AssignmentExpr *>(expr.get()), env);
  case BINARY_EXPR:
    return tc_binary_expr(static_cast<BinaryExpr *>(expr.get()), env);
  case CALL_EXPR:
    return tc_call_expr(static_cast<CallExpr *>(expr.get()), env);

  // Macros
  case LOG_MACRO:
    return tc_log_macro(static_cast<LogMacro *>(expr.get()), env);
  case FMT_MACRO:
    return tc_fmt_macro(static_cast<FmtMacro *>(expr.get()), env);
  case STR_MACRO:
    return tc_str_macro(static_cast<StrMacro *>(expr.get()), env);
  case NUM_MACRO:
    return tc_num_macro(static_cast<NumMacro *>(expr.get()), env);
  default:
    expr->debug(0);
    std::cout << "^^^^^ typechecking for node Unimplimented ^^^^^^\n";
    std::cout << "ASTKind: " << expr->kind << "\n";
    TODO("Unimplimented Typechecking for expr");
    break;
  }
}

shared_ptr<analysis::Type> analysis::tc_binary_expr(ast::BinaryExpr *expr, shared_ptr<analysis::Scope> env) {
  auto l = tc_expr(expr->left, env);
  auto r = tc_expr(expr->right, env);
  auto op = expr->operation.kind;

  if (!types_match(l, r)) {
    std::cout << "Invalid binary operation of " << l->str() << " " << expr->operation.value << " " << r->str() << "\n";
    exit(1);
  }

  // Numeric only operations - * / % < <= > >=
  if (op == lexer::MINUS || op == lexer::STAR || op == lexer::SLASH || op == lexer::PERCENT || op == lexer::LESS ||
      op == lexer::LESS_EQ || op == lexer::GREATER || op == lexer::GREATER_EQ) {
    if (l->kind == NUMBER) {
      return l;
    }
  }

  // Handle Logical Operations && ||
  if (op == lexer::AND || op == lexer::OR) {
    if (l->kind == BOOL) {
      return l;
    }
  }

  // TODO: Overloading and Type == Type may not be valid
  if (op == lexer::EQUALS || op == lexer::NOT_EQUALS) {
    if (l->kind == NUMBER || l->kind == STRING || l->kind == BOOL) {
      return MK_BOOL();
    }
  }

  // TODO: Handle overloads
  // Handle String + String and Number + Number
  if (op == lexer::PLUS) {
    if (l->kind == STRING || l->kind == NUMBER) {
      return l;
    }
  }

  std::cout << "Invalid binary operation of " << l->str() << " " << expr->operation.value << " " << r->str() << "\n";
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_call_expr(ast::CallExpr *expr, shared_ptr<analysis::Scope> env) {
  auto calle = tc_expr(expr->calle, env);
  FnType *fn = AS_FN(calle);

  if (calle->kind != analysis::FN) {
    std::cout << "Invalid call-expression on type " << calle->str() << "\n";
    exit(1);
  }

  // Check expected vs recieved arity
  if (fn->variadic) {
    std::cout << "Unhandled call expr check variadic function\n";
    exit(1);
  }

  if (fn->params.size() != expr->args.size()) {
    std::cout << "Function call " << fn->str() << " expected " << fn->params.size() << " arguments but recieved ";
    std::cout << expr->args.size() << " arguments instead\n";
    exit(1);
  }

  auto fnEnv = make_shared<Scope>();
  fnEnv->is_function = true;
  fnEnv->parent = env;
  fn->body->scope = fnEnv;

  // TODO: Handle variadic instantiation
  // Install Paremeters into body of function
  for (size_t i = 0; i < fn->params.size(); i++) {
    auto param = fn->params[i];
    auto argType = tc_expr(expr->args[i], fnEnv);

    if (!types_match(param.type, argType)) {
      std::cout << "Param at position " << i << " expected to be " << param.type->str();
      std::cout << " but recieved " << argType->str() << " instead\n";
      exit(1);
    }

    fnEnv->defineSymbol(param.name, param.type);
  }

  tc_block_stmt(fn->body.get());

  // Validate Return Type
  bool foundInvalidReturnType = false;
  for (const auto &foundReturn : fnEnv->found_return_types) {
    if (!types_match(fn->returns, foundReturn)) {
      std::cout << "Mismatch return type inside function declaration. Expected a return of ";
      std::cout << fn->returns->str() << " but recieved " << foundReturn->str() << " instead\n";

      foundInvalidReturnType = true;
    }
  }

  if (foundInvalidReturnType) {
    exit(1);
  }

  return fn->returns;
}

shared_ptr<analysis::Type> analysis::tc_symbol_expr(ast::SymbolExpr *expr, shared_ptr<analysis::Scope> env) {
  auto symbolType = env->resolveSymbol(expr->symbol);
  if (symbolType) {
    return symbolType;
  }

  std::cout << red("ReferenceError ") << cyan(expr->symbol) << " does not exist in scope\n";
  exit(1);
}

shared_ptr<analysis::Type> analysis::tc_prefix_expr(ast::PrefixExpr *expr, shared_ptr<analysis::Scope> env) {
  auto rhs = tc_expr(expr->right, env); // &Number
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
  case lexer::STAR: {
    if (rhs->kind == POINTER) {
      return AS_PTR(rhs)->underlying;
    }
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
