#include "../ast/ast.h"
#include "../ast/ast_expr.h"
#include "../ast/ast_stmt.h"
#include "../ast/ast_type.h"
#include "../bedrock.h"
#include "scope.h"
#include "types.h"

namespace analysis {

shared_ptr<analysis::Type> tc_stmt(shared_ptr<ast::Stmt>, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_expr(shared_ptr<ast::Expr>, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_type(shared_ptr<ast::Type>, shared_ptr<analysis::Scope>);

// Statements
shared_ptr<analysis::Type> tc_program(shared_ptr<ast::ProgramStmt>);
shared_ptr<analysis::Type> tc_module(shared_ptr<ast::ModuleStmt>);
shared_ptr<analysis::Type> tc_var_decl_stmt(ast::VarDeclStmt *, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_fn_decl_stmt(ast::FnDeclStmt *, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_struct_stmt(ast::StructStmt *, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_block_stmt(ast::BlockStmt *);
shared_ptr<analysis::Type> tc_expr_stmt(ast::ExprStmt *, shared_ptr<analysis::Scope>);

// Expressions
shared_ptr<analysis::Type> tc_symbol_expr(ast::SymbolExpr *, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_binary_expr(ast::BinaryExpr *, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_prefix_expr(ast::PrefixExpr *, shared_ptr<analysis::Scope>);
shared_ptr<analysis::Type> tc_assignment_expr(ast::AssignmentExpr *, shared_ptr<analysis::Scope>);

}; // namespace analysis