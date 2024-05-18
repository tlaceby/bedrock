#pragma once

#include "ast.h"

namespace ast {
struct SymbolType : public Type {
  string symbol;

  virtual ~SymbolType() {}
  SymbolType() { kind = SYMBOL_TYPE; }
  std::string debug(size_t depth);
};

struct PointerType : public Type {
  shared_ptr<Type> type;

  virtual ~PointerType() {}
  PointerType() { kind = POINTER_TYPE; }
  std::string debug(size_t depth);
};

struct FnType : public Type {
  bool variadic;  // whether the function has variable arity -> ...name: []T
  vector<PropertyKey> params;
  shared_ptr<Type> return_type;

  virtual ~FnType() {}
  FnType() { kind = FN_TYPE; }
  std::string debug(size_t depth);
};

};  // namespace ast