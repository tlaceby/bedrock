#pragma once

#include "../bedrock.h"
#include "token.h"

namespace lexer {

pair<vector<Token>, vector<errors::Err>> tokenize(string file_path);

struct Lexer {
  size_t pos;
  size_t line;
  vector<Token> tokens;
  shared_ptr<SourceFile> file;
  vector<errors::Err> errs;

  Lexer(string file_path);
};
};  // namespace lexer