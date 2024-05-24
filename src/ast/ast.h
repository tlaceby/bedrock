#pragma once

#include "../bedrock.h"
#include "../util/utils.h"

namespace ast {
enum NodeKind {
  // Statements
  PROGRAM_STMT,
  MODULE_STMT,
  BLOCK_STMT,
  VAR_DECL_STMT,
  FN_DECL_STMT,
  // IMPORT_STMT,
  STRUCT_STMT,
  // UNSAFE_STMT,
  // WHILE_STMT,
  IMPL_STMT,
  DEFER_STMT,
  // TYPE_STMT,
  // IF_STMT,
  // FOR_STMT,
  EXPR_STMT,
  // MATCH_STMT,

  // Expressions
  NUMBER_EXPR,
  STRING_EXPR,
  SYMBOL_EXPR,
  BINARY_EXPR,
  PREFIX_EXPR,
  ASSIGN_EXPR,
  // SLICE_EXPR,
  // INSTANTIATION_EXPR,
  // CALL_EXPR,
  // FN_EXPR,
  // COMPUTED_EXPR,

  // Types
  SYMBOL_TYPE,
  POINTER_TYPE,
  FN_TYPE,

  // Macros
  LOG_MACRO,
  FMT_MACRO,
  STR_MACRO,
  NUM_MACRO,
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

struct PropertyKey {
  bool is_pub;    // used for structs only.
  bool is_static; // used for structs only.
  bool variadic;  // Only used on function declarations and fn_types
  string name;
  shared_ptr<Type> type;

  PropertyKey() {
  }
  PropertyKey(string name, shared_ptr<Type> type) : name(name), type(type) {
  }
  PropertyKey(bool pub, bool s, string name, shared_ptr<Type> type)
      : is_pub(pub), is_static(s), name(name), type(type) {
  }
};

}; // namespace ast