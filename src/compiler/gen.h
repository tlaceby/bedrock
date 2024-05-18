#pragma once

#include "../ast/ast.h"
#include "../ast/ast_expr.h"
#include "../ast/ast_stmt.h"
#include "../ast/ast_type.h"
#include "../bedrock.h"

/// @brief Manages all operations regarding compilation of a bedrock program.
namespace compiler {
string compile(shared_ptr<ast::Stmt>, shared_ptr<analysis::Scope>);
string compile(shared_ptr<ast::Expr>, shared_ptr<analysis::Scope>);
string compile(shared_ptr<ast::Type>, shared_ptr<analysis::Scope>);

// Stmt Compiler Handlers
string compile_program_stmt(ast::ProgramStmt*);
string compile_module_stmt(ast::ModuleStmt*);
string compile_block_stmt(ast::BlockStmt*, shared_ptr<analysis::Scope>);
string compile_var_decl_stmt(ast::VarDeclStmt*, shared_ptr<analysis::Scope>);
string compile_defer_stmt(ast::DeferStmt*, shared_ptr<analysis::Scope>);
string compile_fn_decl_stmt(ast::FnDeclStmt*, shared_ptr<analysis::Scope>);

// Expr Compiler Handlers
string compile_binary_expr(ast::BinaryExpr*, shared_ptr<analysis::Scope>);
string compile_assignment_expr(ast::AssignmentExpr*,
                               shared_ptr<analysis::Scope>);
string compile_prefix_expr(ast::PrefixExpr*, shared_ptr<analysis::Scope>);
string compile_string_expr(ast::StringExpr*, shared_ptr<analysis::Scope>);
string compile_number_expr(ast::NumberExpr*, shared_ptr<analysis::Scope>);
string compile_symbol_expr(ast::SymbolExpr*, shared_ptr<analysis::Scope>);

// Type Compiler Handlers
string compile_symbol_type(ast::SymbolType*, shared_ptr<analysis::Scope>);
string compile_pointer_type(ast::PointerType*, shared_ptr<analysis::Scope>);
string compile_fn_type(ast::FnType*, shared_ptr<analysis::Scope>);
};  // namespace compiler