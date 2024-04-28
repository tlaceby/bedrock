#include "./memory.h"

using namespace compiler;
using analysis::Scope, analysis::Type;

bool compiler::is_fixed_size(shared_ptr<Scope> scope, string varname) {
  UNUSED(scope);
  UNUSED(varname);
  TODO();
}

size_t compiler::get_alloc_size(shared_ptr<Type> type) {
  UNUSED(type);
  TODO();
}