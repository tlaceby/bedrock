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
  //
}

void Compiler::compile_binary_expr(BinaryExpr *expr, shared_ptr<analysis::Scope> env) {
  //
}
