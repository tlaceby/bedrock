#pragma once

#include "../lexing/token.h"
#include "ast.h"

namespace ast {
struct NumberExpr : public Expr {
  string value;

  virtual ~NumberExpr() {
  }
  NumberExpr() {
    kind = NUMBER_EXPR;
  }
  string debug(size_t depth);
};

struct StringExpr : public Expr {
  string value;

  virtual ~StringExpr() {
  }
  StringExpr() {
    kind = STRING_EXPR;
  }
  string debug(size_t depth);
};

struct SymbolExpr : public Expr {
  string symbol;

  virtual ~SymbolExpr() {
  }
  SymbolExpr() {
    kind = SYMBOL_EXPR;
  }
  string debug(size_t depth);
};

// Complex Binary/Unary

struct BinaryExpr : public Expr {
  shared_ptr<Expr> left;
  shared_ptr<Expr> right;
  lexer::Token operation;

  virtual ~BinaryExpr() {
  }
  BinaryExpr() {
    kind = BINARY_EXPR;
  }
  string debug(size_t depth);
};

struct PrefixExpr : public Expr {
  shared_ptr<Expr> right;
  lexer::Token operation;

  virtual ~PrefixExpr() {
  }
  PrefixExpr() {
    kind = PREFIX_EXPR;
  }

  string debug(size_t depth);
};

struct AssignmentExpr : public Expr {
  shared_ptr<Expr> assigne;
  shared_ptr<Expr> value;

  virtual ~AssignmentExpr() {
  }
  AssignmentExpr() {
    kind = ASSIGN_EXPR;
  }
  string debug(size_t depth);
};

struct CallExpr : public Expr {
  shared_ptr<Expr> calle;
  vector<shared_ptr<Expr>> args;

  virtual ~CallExpr() {
  }
  CallExpr() {
    kind = CALL_EXPR;
  }
  string debug(size_t depth);
};

}; // namespace ast