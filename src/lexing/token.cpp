#include "token.h"

using namespace lexer;

string lexer::TokenPos::error_str() {
  string s = "(" + this->file->file_path + ")[";

  s += to_string(this->line) + ":";
  s += to_string(this->start) + ":";
  s += to_string(this->end) + "]";

  return s;
}

string lexer::TokenPos::get_snippet(bool showUnderline, size_t linePaddings) {
  UNUSED(showUnderline);
  UNUSED(linePaddings);

  TODO("lexer::TokenPos::get_snippet() not yet implimented");
}