#include "lexer.h"

#include "../util/utils.h"

using namespace lexer;

struct Lexer {
  size_t pos;
  size_t line;
  vector<Token> tokens;
  shared_ptr<SourceFile> file;

  Lexer(string file_path) {
    pos = 0;
    line = 1;
    tokens = std::vector<Token>();

    file = make_shared<SourceFile>();
    file->file_path = file_path;

    auto contents = utils::read_file_contents(file_path);

    if (contents.has_value()) {
      file->contents = contents.value();
      return;
    }

    std::cout << "Could not read file at " << file_path << "\n";
    exit(1);
  }
};

vector<Token> lexer::tokenize(string file_path) {
  Lexer lex{file_path};

  return lex.tokens;
}
