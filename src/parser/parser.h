#pragma once

#include <functional>

#include "../ast/ast.h"
#include "../ast/ast_expr.h"
#include "../ast/ast_macro.h"
#include "../ast/ast_stmt.h"
#include "../ast/ast_type.h"
#include "../bedrock.h"
#include "../lexing/lexer.h"
#include "binding_power.h"

namespace parser {

// Parser Management
struct ParserManager {
  vector<errors::Err> errors;
  unordered_map<string, shared_ptr<ast::ModuleStmt>> modules;

  ParserManager() {
  }
};

struct Parser {
  static shared_ptr<ParserManager> manager;

  shared_ptr<lexer::SourceFile> file;
  vector<lexer::Token> tokens;
  size_t pos;

  bool has_tokens();
  lexer::Token peak();
  lexer::Token current_tk();
  lexer::TokenKind current_tk_kind();
  lexer::Token expect();
  lexer::Token expect(lexer::TokenKind);
  lexer::Token advance();
  bool advance_as(lexer::TokenKind);

  void report(Err);
  optional<shared_ptr<ast::ModuleStmt>> get_module(string);
  shared_ptr<ast::ModuleStmt> add_module(string, shared_ptr<ast::ModuleStmt>);

  Parser(vector<lexer::Token> &tokens) : tokens(tokens) {
  }
};

// Public Methods
shared_ptr<ast::ProgramStmt> parse(string file_path);
shared_ptr<ast::ProgramStmt> parse(vector<lexer::Token> &tokens);
shared_ptr<ast::ModuleStmt> parse_module(Parser &);

// Stmt Parsing -----------
// ------------------------
shared_ptr<ast::Stmt> parse_stmt(Parser &);
shared_ptr<ast::BlockStmt> parse_block_stmt(Parser &);
shared_ptr<ast::ExprStmt> parse_expr_stmt(Parser &);
shared_ptr<ast::StructStmt> parse_struct_stmt(Parser &);
shared_ptr<ast::VarDeclStmt> parse_var_decl_stmt(Parser &);
shared_ptr<ast::FnDeclStmt> parse_fn_decl_stmt(Parser &);
shared_ptr<ast::ImplStmt> parse_impl_stmt(Parser &);
shared_ptr<ast::DeferStmt> parse_defer_stmt(Parser &);
shared_ptr<ast::ReturnStmt> parse_return_stmt(Parser &p);

// Expression Parsing -----
// ------------------------
shared_ptr<ast::Expr> parse_expr(Parser &, BindingPower);
shared_ptr<ast::Expr> parse_primary_expr(Parser &);
shared_ptr<ast::BinaryExpr> parse_binary_expr(Parser &, shared_ptr<ast::Expr>, BindingPower);
shared_ptr<ast::AssignmentExpr> parse_assignment_expr(Parser &, shared_ptr<ast::Expr>, BindingPower);
shared_ptr<ast::CallExpr> parse_call_expr(Parser &, shared_ptr<ast::Expr>, BindingPower);
shared_ptr<ast::Expr> parse_grouping_expr(Parser &p);
shared_ptr<ast::PrefixExpr> parse_prefix_expr(Parser &);

// Type Parsing -----------
// ------------------------
shared_ptr<ast::Type> parse_type(Parser &, BindingPower);
shared_ptr<ast::SymbolType> parse_symbol_type(Parser &);
shared_ptr<ast::PointerType> parse_pointer_type(Parser &);
shared_ptr<ast::FnType> parse_fn_type(Parser &);

// Macro & Trait Parsing -----
// ------------------------
shared_ptr<ast::LogMacro> parse_log_macro(Parser &);
shared_ptr<ast::NumMacro> parse_num_macro(Parser &);
shared_ptr<ast::StrMacro> parse_str_macro(Parser &);
shared_ptr<ast::FmtMacro> parse_fmt_macro(Parser &);

// Shared Parsing Methods
pair<vector<ast::PropertyKey>, bool> parse_fn_params(Parser &);
vector<string> parse_generics_list(Parser &p);
vector<shared_ptr<ast::Type>> parse_generic_type_list(Parser &p);

}; // namespace parser