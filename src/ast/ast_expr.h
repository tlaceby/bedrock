#pragma once

#include "ast.h"

namespace ast {
struct NumberExpr : public Expr {
  double value;

  NumberExpr() { kind = NUMBER_EXPR; }
  string debug(size_t depth) = 0;
};

struct StringExpr : public Expr {
  string value;

  StringExpr() { kind = STRING_EXPR; }
  string debug(size_t depth) = 0;
};

struct SymbolExpr : public Expr {
  string symbol;

  SymbolExpr() { kind = SYMBOL_EXPR; }
  string debug(size_t depth) = 0;
};

};  // namespace ast