#include "parser.h"

#include "lookup.h"

using namespace parser;

shared_ptr<ParserManager> Parser::manager = nullptr;

shared_ptr<ast::ProgramStmt> parser::parse(string file_path) {
  // TODO: Generalize DISPLAY_TOKENS, DISPLAY_AST CHECK
  auto [tokens, errors] = lexer::tokenize(file_path);

  if (errors.size() > 0) {
    for (auto& err : errors) {
      err.display();
    }

    return nullptr;
  }

  if (DISPLAY_TOKENS) {
    std::cout << "\nTokens: " << to_string(tokens.size()) << "\n";
    for (const auto& token : tokens) {
      token.display();
    }
    std::cout << std::endl;
  }

  return parser::parse(tokens);
}

shared_ptr<ast::ProgramStmt> parser::parse(vector<lexer::Token>& tokens) {
  Parser parser{tokens};
  parser.pos = 0;
  parser.file = tokens.at(0).pos->file;

  setup_pratt_parser();

  if (parser.manager == nullptr) {
    parser.manager = make_shared<ParserManager>();
  }

  auto program = make_shared<ast::ProgramStmt>();
  auto entry_module = parse_module(parser);
  entry_module->is_entry = true;

  if (Parser::manager->errors.size() > 0) {
    exit(1);
  }

  program->entry = entry_module;
  program->modules.push_back(entry_module);

  if (DISPLAY_AST) {
    std::cout << "\n----------   AST   ----------\n\n";
    std::cout << program->debug(0);
    std::cout << "\n\n";
  }

  return program;
}

shared_ptr<ast::ModuleStmt> parser::parse_module(Parser& parser) {
  auto mod = make_shared<ast::ModuleStmt>();
  mod->name = parser.file->file_path;

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

// ---------------------
// Parser struct methods
// ---------------------

bool Parser::has_tokens() {
  return this->pos < this->tokens.size() &&
         this->current_tk_kind() != lexer::END_FILE;
}

lexer::Token Parser::peak() { return this->tokens.at(this->pos + 1); }

lexer::Token Parser::current_tk() { return this->tokens.at(this->pos); }

lexer::TokenKind Parser::current_tk_kind() { return this->current_tk().kind; }

lexer::Token Parser::expect() { return this->expect(this->current_tk_kind()); }

lexer::Token Parser::expect(lexer::TokenKind expected) {
  auto tk = this->current_tk();

  this->pos++;
  if (tk.kind != expected) {
    auto err = Err(errors::UnexpectedToken);
    err.message("Expected to find " + bold_white(lexer::token_tag(expected)) +
                " but recieved " + bold_white(lexer::token_tag(tk.kind)) +
                " instead.");
    err.location(tk.pos);
    this->report(err);
  }

  return tk;
}

void Parser::report(Err err) {
  this->manager->errors.push_back(err);
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

shared_ptr<ast::ModuleStmt> Parser::add_module(
    string mod_name, shared_ptr<ast::ModuleStmt> mod) {
  this->manager->modules.insert_or_assign(mod_name, mod);
  return mod;
}
