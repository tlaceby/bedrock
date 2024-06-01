#pragma once
#include "../ast/ast.h"
#include "../ast/ast_expr.h"
#include "../ast/ast_macro.h"
#include "../ast/ast_stmt.h"
#include "../bedrock.h"
#include "../instructions/opcodes.h"
#include "../vm/values.h"

// When writing the data section:
// First 4 bytes represent the kind of data being written
// 0: number encoded as a double
// 1: boolean encoded as a boolean
// 2: string with the bytes until the null terminator

namespace compiler {

struct Compiler {
  size_t depth;                             // 0 means global, 1 means module, 2+ means inside a local-scope.
  unordered_map<string, size_t> globals_lu; // Contains the offset for the globals pool for a given variable name.
  unordered_map<string, size_t> chunks_lu;  // Contains a mapping from fn_name to chunk_address in the ip

  vector<runtime::Val> data;
  vector<u_int16_t> code;
  string module_name; // Name of current module

  void compile(shared_ptr<ast::ProgramStmt> program, string outpath);

  void compile_expr(shared_ptr<ast::Expr>, shared_ptr<analysis::Scope>);
  void compile_stmt(shared_ptr<ast::Stmt>, shared_ptr<analysis::Scope>);

  // Statements
  void compile_module(ast::ModuleStmt *);
  void compile_fn_decl_stmt(ast::FnDeclStmt *, shared_ptr<analysis::Scope>);
  void compile_block_stmt(ast::BlockStmt *, shared_ptr<analysis::Scope>);
  void compile_var_decl_stmt(ast::VarDeclStmt *, shared_ptr<analysis::Scope>);
  void compile_expr_stmt(ast::ExprStmt *, shared_ptr<analysis::Scope>);
  void compile_return_stmt(ast::ReturnStmt *, shared_ptr<analysis::Scope>);

  // Expressions
  void compile_number_expr(ast::NumberExpr *, shared_ptr<analysis::Scope>);
  void compile_string_expr(ast::StringExpr *, shared_ptr<analysis::Scope>);
  void compile_symbol_expr(ast::SymbolExpr *, shared_ptr<analysis::Scope>);
  void compile_binary_expr(ast::BinaryExpr *, shared_ptr<analysis::Scope>);

  void scope_enter(shared_ptr<analysis::Scope>);
  void scope_exit(shared_ptr<analysis::Scope>);

  // Macros
  void compile_log_macro(ast::LogMacro *, shared_ptr<analysis::Scope>);

  size_t ip() {
    return code.size();
  }

  size_t getConstantAddr(double d);
  size_t getConstantAddr(string s);
  size_t getConstantAddr(bool b);

  void debugBytecode();
  void emit(u_int16_t byte) {
    code.push_back(byte);
  }

  // Prepends ModuleName along with variable/fn name
  string getGlobalName(string name) {
    return "(" + module_name + ") " + name;
  }

  string getGlobalFromAddr(size_t addr) {
    for (const auto &[varname, address] : globals_lu) {
      if (addr == address) {
        return varname;
      }
    }

    printf("Global at address: %lu not found in getGlobalFromAddr()\n", addr);
    exit(1);
  }

  string getChunkFromAddr(size_t addr) {
    for (const auto &[varname, address] : chunks_lu) {
      if (addr == address) {
        return varname;
      }
    }

    printf("Chunk at address: %lu not found in getChunkFromAddr()\n", addr);
    exit(1);
  }
};

}; // namespace compiler