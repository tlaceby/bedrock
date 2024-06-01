#include "lexer.h"

#include <regex>

#include "../util/utils.h"

using std::regex;
using std::smatch;
using namespace lexer;

lexer::Lexer::Lexer(string file_path) {
  pos = 0;
  line = 1;
  tokens = std::vector<Token>();
  file = make_shared<SourceFile>();
  file->file_path = file_path;
  auto contents = utils::read_file_contents(file_path);

  if (!contents.has_value()) {
    errs.push_back(Err(ErrKind::InvalidFilePath)
                       .message("Attempted but failed to read file: " + bold_white(file_path))
                       .hint("Verify a bedrock file exists at this location."));

    return;
  }

  file->contents = contents.value();

  using lexer::TokenKind;
  // TODO: Cleanup regex pattern using macro?
  patterns = {
      {regex(R"(\s+)"), skip_handler},
      {regex(R"(\/\/.*)"), comment_handler},
      {regex(R"("[^"]*")"), string_handler},
      {regex(R"([0-9]+(\.[0-9]+)?)"), number_handler},
      {regex(R"([a-zA-Z_@][a-zA-Z0-9_]*)"), symbol_handler},
      {regex(R"(\[)"), default_handler(OPEN_BRACKET, "[")},
      {regex(R"(\])"), default_handler(CLOSE_BRACKET, "]")},
      {regex(R"(\{)"), default_handler(OPEN_CURLY, "{")},
      {regex(R"(\})"), default_handler(CLOSE_CURLY, "}")},
      {regex(R"(\()"), default_handler(OPEN_PAREN, "(")},
      {regex(R"(\))"), default_handler(CLOSE_PAREN, ")")},
      {regex(R"(\=\=)"), default_handler(EQUALS, "==")},
      {regex(R"(\!\=)"), default_handler(NOT_EQUALS, "!=")},
      {regex(R"(!)"), default_handler(NOT, "!")},
      {regex(R"(=)"), default_handler(ASSIGNMENT, "=")},
      {regex(R"(->)"), default_handler(ARROW, "->")},
      {regex(R"(<)"), default_handler(OPEN_GENERIC, "<")},
      {regex(R"(>)"), default_handler(CLOSE_GENERIC, ">")},
      {regex(R"(\.\.)"), default_handler(DOT_DOT, "..")},
      {regex(R"(\.)"), default_handler(DOT, ".")},
      {regex(R"(;)"), default_handler(SEMICOLON, ";")},
      {regex(R"(::)"), default_handler(COLON_COLON, "::")},
      {regex(R"(:)"), default_handler(COLON, ":")},
      {regex(R"(\?)"), default_handler(QUESTION, "?")},
      {regex(R"(,)"), default_handler(COMMA, ",")},
      {regex(R"(\+\+)"), default_handler(PLUS_PLUS, "++")},
      {regex(R"(--)"), default_handler(MINUS_MINUS, "--")},
      {regex(R"(\+=)"), default_handler(PLUS_EQUALS, "+=")},
      {regex(R"(-=)"), default_handler(MINUS_EQUALS, "-=")},
      {regex(R"(/=)"), default_handler(SLASH_EQUALS, "/=")},
      {regex(R"(\*=)"), default_handler(STAR_EQUALS, "*=")},
      {regex(R"(\+)"), default_handler(PLUS, "+")},
      {regex(R"(-)"), default_handler(MINUS, "-")},
      {regex(R"(/)"), default_handler(SLASH, "/")},
      {regex(R"(\*)"), default_handler(STAR, "*")},
      {regex(R"(%)"), default_handler(PERCENT, "%")},
      {regex(R"(&)"), default_handler(AMPERSAND, "&")},
  };
};

pair<vector<Token>, vector<errors::Err>> lexer::tokenize(string file_path) {
  Lexer lex{file_path};

  // Means loading the file produces errors.
  if (lex.errs.size() != 0) {
    return make_pair(lex.tokens, lex.errs);
  }

  while (!lex.at_eof() && lex.errs.size() == 0) {
    bool matched = false;

    for (const auto &pattern : lex.patterns) {
      smatch match;
      string remainder = lex.remainder();
      if (regex_search(remainder, match, pattern.re) && match.position() == 0) {
        pattern.handler(lex, pattern.re);
        matched = true;
        break; // Exit the loop after the first match
      }
    }

    if (!matched) {
      SourcePos pos{lex.file, lex.line, lex.pos, lex.pos};
      lex.errs.push_back(
          Err(ErrKind::UnexpectedToken)
              .message(bold_white("Unrecognized token near " + pos.error_str()) + "  " + lex.remainder()));
    }
  }

  SourcePos pos{lex.file, lex.line, lex.pos, lex.pos};
  lex.push(Token{pos, TokenKind::END_FILE, "EOF"});

  return make_pair(lex.tokens, lex.errs);
}

regex_handler lexer::default_handler(TokenKind kind, const string &value) {
  return [kind, value](Lexer &lex, const regex &) {
    SourcePos pos{lex.file, lex.line, lex.pos, 0};
    lex.advance_n(value.length());
    pos.end = lex.pos;
    lex.push(Token{pos, kind, value});
  };
}

void lexer::string_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();

  if (regex_search(remainder, match, re)) {
    string literal = match.str().substr(1, match.str().length() - 2);
    SourcePos pos{lex.file, lex.line, lex.pos, 0};
    lex.advance_n(match.str().length());
    pos.end = lex.pos;
    lex.push(Token{pos, TokenKind::STRING, literal});
  }
}

void lexer::number_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();

  if (regex_search(remainder, match, re)) {
    SourcePos pos{lex.file, lex.line, lex.pos, 0};
    lex.advance_n(match.str().length());
    pos.end = lex.pos;
    lex.push(Token{pos, TokenKind::NUMBER, match.str()});
  }
}

void lexer::symbol_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();

  if (regex_search(remainder, match, re)) {
    string symbol = match.str();
    SourcePos pos{lex.file, lex.line, lex.pos, 0};

    lex.advance_n(symbol.length());
    pos.end = lex.pos;

    auto it = reserved_lu.find(symbol);
    if (it != reserved_lu.end()) {
      lex.push(Token{pos, it->second, symbol});
    } else {
      lex.push(Token{pos, TokenKind::IDENTIFIER, symbol});
    }
  }
}

void lexer::skip_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();
  if (regex_search(remainder, match, re)) {
    string matched = match.str();
    size_t new_lines = count(matched.begin(), matched.end(), '\n');
    lex.advance_n(match.length());
    lex.line += new_lines;
  }
}

void lexer::comment_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();
  if (regex_search(remainder, match, re)) {
    lex.advance_n(match.length());
    lex.line++;
  }
}

void Lexer::advance_n(size_t n) {
  pos += n;
}
void Lexer::push(Token token) {
  tokens.push_back(token);
}
bool Lexer::at_eof() {
  return pos >= file->contents.length();
}
string Lexer::remainder() {
  return file->contents.substr(pos);
}