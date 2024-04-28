#pragma once

#include "../ast/ast.h"
#include "../ast/ast_expr.h"
#include "../ast/ast_stmt.h"
#include "../ast/ast_type.h"
#include "../bedrock.h"
#include "../lexing/lexer.h"

namespace parser {

// Parser Management
struct ParserManager {
  vector<errors::Err> errors;
  unordered_map<string, shared_ptr<ast::ModuleStmt>> modules;
};

struct Parser {
  static shared_ptr<ParserManager> manager;

  shared_ptr<lexer::SourceFile> file;
  vector<lexer::Token> tokens;
  size_t pos;
};

// Public Methods

// shared_ptr<ast::ProgramStmt> parse();
// shared_ptr<ast::ModuleStmt> parse_module(shared_ptr<ParserManager>);

};  // namespace parser