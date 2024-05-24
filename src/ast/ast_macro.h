#pragma once

#include "../lexing/token.h"
#include "ast.h"

namespace ast {
struct StrMacro : public Expr {
  shared_ptr<Expr> expr;

  virtual ~StrMacro() {
  }
  StrMacro() {
    kind = STR_MACRO;
  }

  string debug(size_t depth);
};

struct LogMacro : public Expr {
  shared_ptr<Expr> expr;

  virtual ~LogMacro() {
  }
  LogMacro() {
    kind = LOG_MACRO;
  }

  string debug(size_t depth);
};

struct FmtMacro : public Expr {
  string formatString;
  vector<shared_ptr<Expr>> args;

  virtual ~FmtMacro() {
  }
  FmtMacro() {
    kind = FMT_MACRO;
  }

  string debug(size_t depth);
};

struct NumMacro : public Expr {
  shared_ptr<Expr> expr;

  virtual ~NumMacro() {
  }
  NumMacro() {
    kind = NUM_MACRO;
  }

  string debug(size_t depth);
};

}; // namespace ast