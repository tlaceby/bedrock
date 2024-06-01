#include "../ast/ast_type.h"
#include "compiler.h"

using namespace ast;
using namespace runtime;
using namespace compiler;

void Compiler::compile_expr_stmt(ExprStmt *stmt, shared_ptr<analysis::Scope> env) {
  compile_expr(stmt->expr, env);
}

void Compiler::compile_module(ModuleStmt *stmt) {
  depth++;
  module_name = stmt->name;

  for (const auto &s : stmt->body) {
    compile_stmt(s, stmt->scope);
  }

  // Setup Main function and call
  if (stmt->is_entry) {
    emit(op_call); // Jump to the main();
    emit(chunks_lu[getGlobalName("main")]);
  }

  depth--;
}

void Compiler::compile_block_stmt(BlockStmt *stmt, shared_ptr<analysis::Scope> env) {
  scope_enter(stmt->scope);

  for (const auto &s : stmt->body) {
    compile_stmt(s, stmt->scope);
  }

  scope_exit(stmt->scope);
}

void Compiler::compile_fn_decl_stmt(FnDeclStmt *stmt, shared_ptr<analysis::Scope> env) {
  auto fnEnv = stmt->body.get()->scope;
  auto fnBody = stmt->body.get()->body;

  chunks_lu[getGlobalName(stmt->name)] = ip();
  // Set Label address for use in other functions and recusrive calls
  scope_enter(fnEnv);

  for (const auto &s : fnBody) {
    compile_stmt(s, fnEnv);
  }

  scope_exit(fnEnv);
}

void Compiler::compile_var_decl_stmt(VarDeclStmt *stmt, shared_ptr<analysis::Scope> env) {
  string varname = stmt->varname;
  compile_expr(stmt->value, env);

  // Global Variable
  if (depth < 2) {
    size_t globalAddr = globals_lu.size();
    globals_lu[getGlobalName(varname)] = globalAddr;
    emit(op_storeg);
    emit(globalAddr);
    return;
  }

  // Variable Declaration
  size_t offset = env->local_offsets[varname];
  emit(op_storel);
  emit(offset);
}

void Compiler::compile_return_stmt(ReturnStmt *stmt, shared_ptr<analysis::Scope> env) {
  u_int16_t returnValues = 0;
  if (stmt->rhs) {
    compile_expr(stmt->rhs, env);
    returnValues = 1;
  }

  emit(op_ret);
  emit(returnValues);
}