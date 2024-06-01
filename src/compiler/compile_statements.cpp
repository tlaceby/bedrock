#include "../ast/ast_type.h"
#include "compiler.h"

using namespace ast;
using namespace runtime;
using namespace compiler;

void Compiler::compile_expr_stmt(ExprStmt *stmt, shared_ptr<analysis::Scope> env) {
  compile_expr(stmt->expr, env);
}

void Compiler::compile_module(ModuleStmt *stmt) {
  auto label_start = ip();
  depth++;
  module_name = stmt->name;

  for (const auto &s : stmt->body) {
    compile_stmt(s, stmt->scope);
  }

  // Setup Main function and call
  if (stmt->is_entry) {
    emit(ip());   // Push return address onto the stack
    emit(op_jmp); // Jump to the __main__ fn
    emit(this->globals_lu.at("main"));
    emit(op_hlt); // End program execution
  }

  depth--;
}

void Compiler::compile_block_stmt(BlockStmt *stmt, shared_ptr<analysis::Scope> env) {
  depth++;

  // TODO: Push Framepointer and allocate stack space for each datatype
  for (const auto &s : stmt->body) {
    compile_stmt(s, stmt->scope);
  }

  // TODO: Cleanup Local Stack Addresses
  depth--;
}

void Compiler::compile_fn_decl_stmt(FnDeclStmt *stmt, shared_ptr<analysis::Scope> env) {
  auto fnEnv = stmt->body.get()->scope;
  auto fnBody = stmt->body.get()->body;
  auto numLocals = fnEnv->symbols.size();

  debugBytecode();

  // Set Label address for use in other functions and recusrive calls
  globals_lu[getGlobalName(stmt->name)] = ip();

  // Setup Space for Locals
  emit(op_incsp);
  emit(numLocals);

  for (const auto &stmt : fnBody) {
    compile_stmt(stmt, fnEnv);
  }

  // Remove space for Locals
  emit(op_decsp);
  emit(numLocals);
}

void Compiler::compile_var_decl_stmt(VarDeclStmt *stmt, shared_ptr<analysis::Scope> env) {
  compile_expr(stmt->value, env);

  // Create Global Variable
  if (depth < 2) {
    size_t globalAddr = ip();
    globals_lu[getGlobalName(stmt->varname)] = globalAddr;
    emit(op_storeg);
    emit(globalAddr);
    return;
  }

  // Creating a Local Variable
}
