#pragma once

#include "ast.h"

namespace ast {
struct SymbolType : public Expr {
  string symbol;

  virtual ~SymbolType() {}
  SymbolType() { kind = SYMBOL_TYPE; }
  std::string debug(size_t depth);
};

struct SliceType : public Expr {
  shared_ptr<Type> type;

  virtual ~SliceType() {}
  SliceType() { kind = SLICE_TYPE; }
  std::string debug(size_t depth);
};

};  // namespace ast