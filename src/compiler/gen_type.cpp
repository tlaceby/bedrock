#include "../analysis/types.h"
#include "../ast/ast_type.h"
#include "gen.h"

using namespace compiler;
using namespace ast;
using analysis::Scope;

string compiler::compile(shared_ptr<Type> type, shared_ptr<Scope> scope) {
  switch (type->kind) {
    case SYMBOL_TYPE:
      return compile_symbol_type(static_cast<SymbolType*>(type.get()), scope);
    default:
      type->debug(1);
      TODO("^^^^^^^^ Unimplimented COMPILE PATH: ^^^^^^^");
  }

  return "UNKNOWN";
}

string compiler::compile_symbol_type(SymbolType* symbol,
                                     shared_ptr<Scope> scope) {
  return symbol->symbol;
  //
}

string compiler::compile_pointer_type(PointerType* pt,
                                      shared_ptr<Scope> scope) {
  return compile(pt->type, scope) + "*";
  //
}

string compiler::compile_fn_type(FnType* fn, shared_ptr<Scope> scope) {
  TODO("COMPILE_FN_TYPE");
  UNUSED(fn);
  return "";
  //
}
