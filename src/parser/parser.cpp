#include "parser.h"

#include "../util/br_strings.h"
#include "lookup.h"

using namespace parser;

shared_ptr<ParserManager> Parser::manager = nullptr;

shared_ptr<ast::ProgramStmt> parser::parse_program(string entry_point) {
  auto [tokens, errors] = lexer::tokenize(entry_point);
  if (errors.size() > 0) {
    for (auto &err : errors) {
      err.display();
    }

    return nullptr;
  }

  if (DISPLAY_TOKENS) {
    std::cout << "\nTokens: " << to_string(tokens.size()) << "\n";
    for (const auto &token : tokens) {
      token.display();
    }

    std::cout << std::endl;
  }

  setup_pratt_parser();

  Parser parser{tokens};
  parser.pos = 0;
  parser.file = tokens.at(0).pos->file;

  parser.manager = make_shared<ParserManager>();
  auto program = make_shared<ast::ProgramStmt>();
  auto mod = parse_file(parser);
  mod->is_entry = true;

  if (mod->name != "main") {
    // TODO: Replace with proper error
    std::cout << "Entry module name is not main!\n";
    exit(1);
  }

  if (Parser::manager->errors.size() > 0) {
    exit(1);
  }

  program->entry = mod;
  program->modules.push_back(mod);

  if (DISPLAY_AST) {
    std::cout << "\n----------   AST   ----------\n\n";
    std::cout << program->debug(0);
    std::cout << "\n\n";
  }

  return program;
  setup_pratt_parser();
}

shared_ptr<ast::ModuleStmt> parser::parse_file(Parser &parser) {
  auto mod = make_shared<ast::ModuleStmt>();
  mod->path = parser.file->file_path;

  parser.expect(lexer::MOD);
  string modname = parser.expect(lexer::IDENTIFIER).value;
  parser.expect(lexer::SEMICOLON);

  // Verify module name matches the parent folder name.
  verify_file_inside_module_folder(parser.file->file_path, "main");

  // Create module if it does not already exist

  // Get weak reference to module if it does already exist

  while (parser.has_tokens()) {
    try {
      auto stmt = parse_stmt(parser);
      mod->body.push_back(stmt);
    } catch (Err err) {
      err.display();
    }
  }

  return mod;
}

bool parser::verify_file_inside_module_folder(string file_path, string mod) {
  auto res = br::str_split(file_path, std::regex("/"));
  
  for (auto a : res) {
    std::cout << a << std::endl;
  }

  return true;
}

// ---------------------
// Parser struct methods
// ---------------------

bool Parser::has_tokens() {
  return this->pos < this->tokens.size() && this->current_tk_kind() != lexer::END_FILE;
}

lexer::Token Parser::peak() {
  return this->tokens.at(this->pos + 1);
}

lexer::Token Parser::current_tk() {
  return this->tokens.at(this->pos);
}

lexer::TokenKind Parser::current_tk_kind() {
  return this->current_tk().kind;
}

lexer::Token Parser::expect() {
  return this->expect(this->current_tk_kind());
}

bool Parser::advance_as(lexer::TokenKind expectedKind) {
  auto kind = current_tk_kind();
  advance();
  return kind == expectedKind;
}

lexer::Token Parser::advance() {
  return this->expect(this->current_tk_kind());
}

lexer::Token Parser::expect(lexer::TokenKind expected) {
  auto tk = this->current_tk();

  this->pos++;
  if (tk.kind != expected) {
    auto err = Err(errors::UnexpectedToken);
    err.message("Expected to find " + bold_white(lexer::token_tag(expected)) + " but recieved " +
                bold_white(lexer::token_tag(tk.kind)) + " instead.");
    err.location(tk.pos);
    this->report(err);
  }

  return tk;
}

void Parser::report(Err err) {
  this->manager->errors.push_back(err);
  err.display();

  // TODO: Actualy propigate errors and catch multiple
  exit(1);
  throw err;
}

optional<shared_ptr<ast::ModuleStmt>> Parser::get_module(string mod_name) {
  auto opt = optional<shared_ptr<ast::ModuleStmt>>{nullptr};
  auto it = this->manager->modules.find(mod_name);

  if (it != this->manager->modules.end()) {
    opt.emplace(this->manager->modules.at(mod_name));
  }

  return opt;
}

shared_ptr<ast::ModuleStmt> Parser::add_module(string mod_name, string folder_path, shared_ptr<ast::ModuleStmt> mod) {
  this->manager->modules.insert_or_assign(mod_name, mod);
  this->manager->modules[mod_name]->path = folder_path;
  return mod;
}

shared_ptr<ast::ModuleStmt> Parser::add_module(string mod_name, shared_ptr<ast::ModuleStmt> mod) {
  this->manager->modules.insert_or_assign(mod_name, mod);
  return mod;
}