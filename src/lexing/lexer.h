#pragma once

#include <functional>
#include <regex>

#include "../bedrock.h"
#include "token.h"

using std::function;
using std::regex;

namespace lexer {

struct Lexer;

typedef function<void(Lexer&, const regex&)> regex_handler;

struct regex_pattern {
  regex re;
  regex_handler handler;
};

pair<vector<Token>, vector<errors::Err>> tokenize(string file_path);

struct Lexer {
  size_t pos;
  size_t line;
  vector<Token> tokens;
  shared_ptr<SourceFile> file;
  vector<errors::Err> errs;
  vector<regex_pattern> patterns;

  Lexer(string file_path);
  void advance_n(size_t n);
  void push(Token token);
  bool at_eof();
  string remainder();
};

regex_handler default_handler(TokenKind kind, const string& value);
void string_handler(Lexer& lex, const regex& re);
void number_handler(Lexer& lex, const regex& re);
void symbol_handler(Lexer& lex, const regex& re);
void skip_handler(Lexer& lex, const regex& re);
void comment_handler(Lexer& lex, const regex& re);
}  // namespace lexer