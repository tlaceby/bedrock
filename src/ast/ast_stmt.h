#pragma once

#include "../analysis/scope.h"
#include "../lexing/token.h"
#include "ast.h"

namespace ast {
struct ModuleStmt : public Stmt {
  bool is_entry;
  shared_ptr<analysis::Scope> scope;
  string name;
  vector<shared_ptr<Stmt>> body;

  virtual ~ModuleStmt() {}
  ModuleStmt() { kind = MODULE_STMT; }
  std::string debug(size_t depth);
};

struct ProgramStmt : public Stmt {
  shared_ptr<ModuleStmt> entry;
  vector<shared_ptr<ModuleStmt>> modules;

  virtual ~ProgramStmt() {}
  ProgramStmt() { kind = PROGRAM_STMT; }
  std::string debug(size_t depth);
};

struct BlockStmt : public Stmt {
  shared_ptr<analysis::Scope> scope;
  vector<shared_ptr<Stmt>> body;

  virtual ~BlockStmt() {}
  BlockStmt() { kind = BLOCK_STMT; }
  std::string debug(size_t depth);
};

struct ExprStmt : public Stmt {
  shared_ptr<Expr> expr;

  virtual ~ExprStmt() {}
  ExprStmt() { kind = EXPR_STMT; }
  std::string debug(size_t depth);
};

struct VarDeclStmt : public Stmt {
  bool constant;
  string varname;
  shared_ptr<Type> type;
  shared_ptr<Expr> value;

  virtual ~VarDeclStmt() {}
  VarDeclStmt() { kind = VAR_DECL_STMT; }
  std::string debug(size_t depth);
};

struct FnDeclStmt : public Stmt {
  bool variadic;  // whether the function has variable arity -> ...name: []T
  string name;
  vector<PropertyKey> params;
  shared_ptr<Type> return_type;
  shared_ptr<BlockStmt> body;

  virtual ~FnDeclStmt() {}
  FnDeclStmt() { kind = FN_DECL_STMT; }
  std::string debug(size_t depth);
};

// Forward declare FnType
struct FnType;

struct StructStmt : public Stmt {
  string name;
  vector<PropertyKey> properties;
  unordered_map<string, shared_ptr<FnType>> instance_methods;
  unordered_map<string, shared_ptr<FnType>> static_methods;

  virtual ~StructStmt() {}
  StructStmt() { kind = STRUCT_STMT; }
  std::string debug(size_t depth);
};

struct ImplStmt : public Stmt {
  shared_ptr<Type> type;

  virtual ~ImplStmt() {}
  ImplStmt() { kind = IMPL_STMT; }
  std::string debug(size_t depth);
};

struct DeferStmt : public Stmt {
  vector<shared_ptr<Expr>> actions;

  virtual ~DeferStmt() {}
  DeferStmt() { kind = DEFER_STMT; }
  std::string debug(size_t depth);
};

};  // namespace ast