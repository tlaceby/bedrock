#pragma once

#include "../bedrock.h"
#include "../util/utils.h"

namespace ast {
enum NodeKind {
  // Statements
  PROGRAM_STMT,  // List of all modules,
  MODULE_STMT,
  BLOCK_STMT,
  VAR_DECL_STMT,
  EXPR_STMT,

  // Expressions
  NUMBER_EXPR,
  STRING_EXPR,
  SYMBOL_EXPR,
  BINARY_EXPR,
  PREFIX_EXPR,
  ASSIGN_EXPR,

  // Types
  SYMBOL_TYPE,
  SLICE_TYPE,
};

struct Expr {
  NodeKind kind;
  virtual std::string debug(size_t depth) = 0;
};

struct Stmt {
  NodeKind kind;
  virtual std::string debug(size_t depth) = 0;
};

struct Type {
  NodeKind kind;
  virtual std::string debug(size_t depth) = 0;
};
};  // namespace ast