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
};  // namespace ast