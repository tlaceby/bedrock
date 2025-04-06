#include "lexer.hpp"
#include "../utils/colors.hpp"
#include "../utils/fs.hpp"
#include "token.hpp"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <optional>
#include <regex>
#include <string>

using std::regex;
using std::smatch;
using namespace lexer;
using namespace errors;

lexer::Lexer::Lexer(std::shared_ptr<ModuleFileRef> mod) {
  pos = 0;
  line = 1;
  tokens = std::vector<Token>();
  file = mod;

  using lexer::TokenKind;

  patterns = {
      {regex(R"(\s+)"), skip_handler},
      {regex(R"(\/\/.*)"), comment_handler},
      {regex(R"("[^"]*")"), string_handler},
      {regex(R"([0-9]+(\.[0-9]+)?)"), number_handler},
      {regex(R"([a-zA-Z_@][a-zA-Z0-9_]*)"), symbol_handler},
      {regex(R"(\[)"), default_handler(LBRACKET, "[")},
      {regex(R"(\])"), default_handler(RBRACKET, "]")},
      {regex(R"(\{)"), default_handler(LCURLY, "{")},
      {regex(R"(\})"), default_handler(RCURLY, "}")},
      {regex(R"(\()"), default_handler(LPAREN, "(")},
      {regex(R"(\))"), default_handler(RPAREN, ")")},
      {regex(R"(\=\=)"), default_handler(EQUALS, "==")},
      {regex(R"(\!\=)"), default_handler(NOT_EQUALS, "!=")},
      {regex(R"(!)"), default_handler(NOT, "!")},
      {regex(R"(=)"), default_handler(ASSIGNMENT, "=")},
      {regex(R"(->)"), default_handler(ARROW, "->")},
      {regex(R"(<)"), default_handler(LESS_THAN, "<")},
      {regex(R"(>)"), default_handler(GREATER_THAN, ">")},
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

std::optional<shared_ptr<ModuleFileRef>>
lexer::file_ref_from_relative(string relative_path) {
  auto ref = std::make_shared<ModuleFileRef>();
  auto opt = utils::fs::read_file(relative_path);

  if (!opt.has_value()) {
    string message = bold_white("Unable to load bedrock file. ") +
                     "The file: " + relative_path +
                     " could not be loaded. Please ensure the path is correct "
                     "and has the proper read permissions";
    LexicalError(LexicalErrorKind::InvalidFilePath, message).add_location(relative_path)->display();
    return std::nullopt;
  }

  ref->data = opt.value();
  ref->from_import = false;
  return std::make_optional<shared_ptr<ModuleFileRef>>(ref);
}

std::optional<shared_ptr<ModuleFileRef>>
lexer::file_ref_from_import(string in_code_name) {
  auto ref = std::make_shared<ModuleFileRef>();
  TODO("Unimplimented");
  ref->from_import = true;
  ref->import_name = in_code_name;
  return ref;
}

pair<vector<Token>, vector<errors::LexicalError>>
lexer::tokenize(string file_path, bool from_import) {
  // construct a lexer from the filepath. This will require looking up the path,
  // checking if it is a bedrock file. Also checking if there are other known
  // bedrock files If there are, return the ModRef for that. If there are not,
  // then create a new one with validation.
  auto opt = from_import ? file_ref_from_import(file_path)
                         : file_ref_from_relative(file_path);
  if (!opt.has_value()) {
    exit(1);
    // It's safe to exit as the errors have already been displayed
  }

  Lexer lex{opt.value()};

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
      string message =
          bold_white("Unrecognized token located while lexing file.") +
          "The character: `" + lex.file->data[lex.pos] + "` is not reconized.\n";
      lex.errs.push_back(
          *LexicalError(LexicalErrorKind::UnxepextedToken, message)
               .add_location(lex.file->absolute_path, lex.line, lex.pos));
    }
  }

  lex.push(Token{TokenKind::END_FILE, "EOF", lex.line, lex.pos, lex.file});
  return make_pair(lex.tokens, lex.errs);
}

regex_handler lexer::default_handler(TokenKind kind, const string &value) {
  return [kind, value](Lexer &lex, const regex &) {
    lex.push(Token{kind, value, lex.line, lex.pos, lex.file});
    lex.advance_n(value.length());
  };
}

void lexer::string_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();

  if (regex_search(remainder, match, re)) {
    string literal = match.str().substr(1, match.str().length() - 2);
    lex.push(Token{TokenKind::STRING, literal, lex.line, lex.pos, lex.file});
    lex.advance_n(match.str().length());
  }
}

void lexer::number_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();

  if (regex_search(remainder, match, re)) {
    lex.push(
        Token{TokenKind::NUMBER, match.str(), lex.line, lex.pos, lex.file});
    lex.advance_n(match.str().length());
  }
}

void lexer::symbol_handler(Lexer &lex, const regex &re) {
  smatch match;
  string remainder = lex.remainder();

  if (regex_search(remainder, match, re)) {
    string symbol = match.str();
    auto it = reserved_lu.find(symbol);

    if (it != reserved_lu.end()) {
      lex.push({it->second, symbol, lex.line, lex.pos, lex.file});
    } else {
      lex.push({TokenKind::IDENTIFIER, symbol, lex.line, lex.pos, lex.file});
    }

    lex.advance_n(symbol.length());
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

void Lexer::advance_n(size_t n) { pos += n; }

void Lexer::push(Token token) { tokens.push_back(token); }

bool Lexer::at_eof() { return pos >= file->data.length(); }

string Lexer::remainder() { return file->data.substr(pos); }
