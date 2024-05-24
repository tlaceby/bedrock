#include "lookup.h"
#include "parser.h"

using namespace parser;
using namespace ast;
using namespace lexer;

shared_ptr<ast::Stmt> parser::parse_stmt(Parser &p) {
  auto it = stmt_lu.find(p.current_tk_kind());

  if (it != stmt_lu.end()) {
    return stmt_lu.at(p.current_tk_kind())(p);
  }

  return parse_expr_stmt(p);
}

shared_ptr<ExprStmt> parser::parse_expr_stmt(Parser &p) {
  auto expr_stmt = make_shared<ExprStmt>();
  expr_stmt->expr = parse_expr(p, BindingPower::DEFAULT_BP);
  p.expect(lexer::SEMICOLON);
  return expr_stmt;
}

shared_ptr<ast::BlockStmt> parser::parse_block_stmt(Parser &p) {
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

shared_ptr<ast::StructStmt> parser::parse_struct_stmt(Parser &p) {
  auto stmt = make_shared<StructStmt>();
  p.expect(STRUCT);
  stmt->name = p.expect(IDENTIFIER).value;

  // Handle generics
  if (p.current_tk_kind() == OPEN_GENERIC) {
    stmt->generics = parse_generics_list(p);
  }

  p.expect(OPEN_CURLY);
  while (p.has_tokens() && p.current_tk_kind() != CLOSE_CURLY) {
    bool pub = false;
    bool is_static = false;
    string name;
    shared_ptr<Type> type;

    if (p.current_tk_kind() == PUB) {
      pub = true;
      p.advance();

      if (p.current_tk_kind() == STATIC) {
        auto err = Err(ErrKind::InvalidStructDeclaration);
        err.message("Static methods/properties are already public.");
        err.hint("Remove the pub specifier for static methods/properties.");
        err.location(p.current_tk().pos);
        p.report(err);
      }
    }

    if (p.current_tk_kind() == STATIC) {
      is_static = true;
      p.advance();
    }

    // Handle Property Parsing
    if (p.current_tk_kind() == IDENTIFIER) {
      name = p.expect(IDENTIFIER).value;

      // Check for duplicate name
      if (stmt->public_status.find(name) != stmt->public_status.end()) {
        auto err = Err(ErrKind::InvalidStructDeclaration)
                       .message("Duplicate field inside struct declaration " + red(name))
                       .location(p.current_tk().pos);

        p.report(err);
      }

      p.expect(COLON);
      type = parse_type(p, DEFAULT_BP);
      stmt->public_status[name] = is_static || pub;
      stmt->properties.push_back(ast::PropertyKey(pub, is_static, name, type));
      p.expect(SEMICOLON);
      continue;
    }

    auto err = Err(ErrKind::InvalidStructDeclaration);
    err.message("Unknown token inside struct declaration.");
    err.location(p.current_tk().pos);
    p.report(err);
  }

  p.expect(CLOSE_CURLY);
  return stmt;
}

shared_ptr<ast::VarDeclStmt> parser::parse_var_decl_stmt(Parser &p) {
  auto stmt = make_shared<VarDeclStmt>();
  stmt->constant = p.advance_as(CONST);
  stmt->varname = p.expect(IDENTIFIER).value;

  // Explicit Type
  if (p.current_tk_kind() == COLON) {
    p.advance();
    stmt->type = parse_type(p, DEFAULT_BP);
  }

  // Handle explicit Value
  if (p.current_tk_kind() != SEMICOLON) {
    p.expect(ASSIGNMENT);
    stmt->value = parse_expr(p, LOGICAL_BP);
  }

  // Make constant declaration has a value
  if (stmt->constant && !stmt->value) {
    auto err = Err(ErrKind::InvalidVariableDeclaration);
    err.message("Must provide value when declaring a constant.");
    err.hint("Please add a value to the constant declaration.");
    err.hint("If you dont want to provide a default value, use let instead.");
    err.location(p.current_tk().pos);
    p.report(err);
  }

  // Make sure either type or value is supplied.
  if (!stmt->constant && (!stmt->value && !stmt->type)) {
    auto err = Err(ErrKind::InvalidVariableDeclaration);
    err.message("Must specify a type or infer value for variable declaration.");
    err.hint("ex: `let " + blue(stmt->varname) + ": " + green("Type") + ";`");
    err.hint("ex: `let " + blue(stmt->varname) + " = " + green("Expr") + ";`");
    err.location(p.current_tk().pos);
    p.report(err);
  }

  p.expect(SEMICOLON);
  return stmt;
}

shared_ptr<ast::FnDeclStmt> parser::parse_fn_decl_stmt(Parser &p) {
  auto stmt = make_shared<FnDeclStmt>();
  p.expect(FN);
  stmt->name = p.expect(IDENTIFIER).value;
  auto [params, _] = parse_fn_params(p);
  stmt->params = params;

  if (p.current_tk_kind() == ARROW) {
    p.advance();
    stmt->return_type = parse_type(p, DEFAULT_BP);
  }

  stmt->body = parse_block_stmt(p);
  return stmt;
}

shared_ptr<ast::ImplStmt> parser::parse_impl_stmt(Parser &p) {
  auto stmt = make_shared<ImplStmt>();
  TODO("parse_impl not done");
  UNUSED(p);
  return stmt;
}

shared_ptr<ast::ReturnStmt> parser::parse_return_stmt(Parser &p) {
  auto stmt = make_shared<ReturnStmt>();
  p.expect(lexer::RETURN);
  stmt->rhs = parse_expr_stmt(p)->expr;
  return stmt;
}

shared_ptr<ast::DeferStmt> parser::parse_defer_stmt(Parser &p) {
  auto stmt = make_shared<DeferStmt>();
  p.expect(DEFER);

  // Handle defer block
  if (p.advance_as(OPEN_CURLY)) {
    while (p.has_tokens() && p.current_tk_kind() != CLOSE_CURLY) {
      stmt->actions.push_back(parse_expr_stmt(p)->expr);
    }

    p.expect(CLOSE_CURLY);
    return stmt;
  }

  // Handle defer expr
  stmt->actions.push_back(parse_expr_stmt(p)->expr);
  return stmt;
}

vector<string> parser::parse_generics_list(Parser &p) {
  auto generics = vector<string>();
  p.expect(OPEN_GENERIC);

  while (p.has_tokens() && p.current_tk_kind() != CLOSE_GENERIC) {
    generics.push_back(p.expect(IDENTIFIER).value);

    if (p.current_tk_kind() != CLOSE_GENERIC) {
      p.expect(COMMA);
    }
  }

  p.expect(CLOSE_GENERIC);
  return generics;
}
