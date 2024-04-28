#include "lexer.h"

#include "../util/utils.h"

using namespace lexer;

lexer::Lexer::Lexer(string file_path) {
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

  errs.push_back(Err(ErrKind::InvalidFilePath)
                     .message("Attempted but failed to read file: " +
                              bold_white(file_path))
                     .hint("Verify a bedrock file exists at this location."));
};

pair<vector<Token>, vector<errors::Err>> lexer::tokenize(string file_path) {
  Lexer lex{file_path};
  // Means loading the file produces errors.
  if (lex.errs.size() != 0) {
    return make_pair(lex.tokens, lex.errs);
  }

  // TODO: Perform tokenization

  return make_pair(lex.tokens, lex.errs);
}
