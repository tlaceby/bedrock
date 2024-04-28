#include "lookup.h"
#include "parser.h"

using namespace parser;
using namespace ast;
using namespace lexer;

shared_ptr<ast::Stmt> parser::parse_stmt(Parser& p) {
  auto it = stmt_lu.find(p.current_tk_kind());

  if (it != stmt_lu.end()) {
    return stmt_lu.at(p.current_tk_kind())(p);
  }

  return parse_expr_stmt(p);
}

shared_ptr<ExprStmt> parser::parse_expr_stmt(Parser& p) {
  auto expr_stmt = make_shared<ExprStmt>();
  expr_stmt->expr = parse_expr(p, BindingPower::DEFAULT_BP);
  p.expect(lexer::SEMICOLON);
  return expr_stmt;
}

shared_ptr<ast::BlockStmt> parser::parse_block_stmt(Parser& p) {
  auto block = make_shared<BlockStmt>();

  p.expect(lexer::OPEN_CURLY);

  while (p.has_tokens() && p.current_tk_kind() != lexer::CLOSE_CURLY) {
    try {
      block->body.push_back(parse_stmt(p));
    } catch (Err err) {
      err.display();
    }
  }

  p.expect(lexer::CLOSE_CURLY);
  return block;
}