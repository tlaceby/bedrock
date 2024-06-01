#include "../analysis/analsyis.h"
#include "../ast/ast_type.h"
#include "compiler.h"

using namespace ast;
using namespace runtime;
using namespace compiler;

void Compiler::compile_number_expr(NumberExpr *expr, shared_ptr<analysis::Scope> env) {
  emit(op_push);
  emit(getConstantAddr(std::stod(expr->value)));
}

void Compiler::compile_string_expr(StringExpr *expr, shared_ptr<analysis::Scope> env) {
  emit(op_push);
  emit(getConstantAddr(expr->value));
}

void Compiler::compile_symbol_expr(SymbolExpr *expr, shared_ptr<analysis::Scope> env) {
  string varname = expr->symbol;
  analysis::Scope *scope = env->resolveSymbolScope(varname);

  // Global Variable
  if (scope->is_global || scope->is_module) {
    emit(op_loadg);
    emit(globals_lu[getGlobalName(varname)]);
    return;
  }

  // Local Variable
  emit(op_loadl);
  emit(scope->local_offsets[varname]); // TODO: Wont work with multiple scopes
}

void Compiler::compile_binary_expr(BinaryExpr *expr, shared_ptr<analysis::Scope> env) {
  compile_expr(expr->left, env);
  compile_expr(expr->right, env);

  switch (expr->operation.kind) {
  // Numeric Arithmetic
  case lexer::MINUS:
    return emit(op_sub);
  case lexer::STAR:
    return emit(op_mul);
  case lexer::SLASH:
    return emit(op_div);
  case lexer::MOD:
    return emit(op_mod);
  // Boolean Logical
  case lexer::OR:
    return emit(op_or);
  case lexer::AND:
    return emit(op_and);
  // Numeric Conditional & Relational
  case lexer::LESS:
    return emit(op_lt);
  case lexer::LESS_EQ:
    return emit(op_lteq);
  case lexer::GREATER:
    return emit(op_gt);
  case lexer::GREATER_EQ:
    return emit(op_gteq);
  }

  auto op = expr->operation.kind;
  auto leftKind = analysis::tc_expr(expr->left, env);

  // add and concat
  if (op == lexer::PLUS) {
    if (leftKind->kind == analysis::NUMBER) {
      return emit(op_add);
    }

    emit(op_concat);
  }

  TODO("== AND != not implimted at compiler level");
  // == & !=
}
