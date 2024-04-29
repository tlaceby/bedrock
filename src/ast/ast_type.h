#pragma once

#include "ast.h"

namespace ast {
struct SymbolType : public Type {
  string symbol;

  virtual ~SymbolType() {}
  SymbolType() { kind = SYMBOL_TYPE; }
  std::string debug(size_t depth);
};

struct SliceType : public Type {
  shared_ptr<Type> type;

  virtual ~SliceType() {}
  SliceType() { kind = SLICE_TYPE; }
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