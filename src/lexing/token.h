#pragma once

#include <unordered_map>

#include "../bedrock.h"
#include "../util/utils.h"

namespace lexer {

enum TokenKind {
  END_FILE,
  IDENTIFIER,
  NUMBER,
  STRING,
  OPEN_PAREN,
  CLOSE_PAREN,
  OPEN_CURLY,
  CLOSE_CURLY,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  COLON,
  SEMICOLON,
  COMMA,
  EQUALS,
  ASSIGNMENT,
  NOT,
  NOT_EQUALS,
  LESS,
  LESS_EQ,
  GREATER,
  GREATER_EQ,
  PLUS,
  MINUS,
  STAR,
  SLASH,
  PERCENT,

  DOT,
  DOT_DOT,
  ARROW,
  COLON_COLON,
  QUESTION,
  PLUS_PLUS,
  MINUS_MINUS,
  PLUS_EQUALS,
  MINUS_EQUALS,
  SLASH_EQUALS,
  STAR_EQUALS,
  AMPERSAND,
  OPEN_GENERIC,
  CLOSE_GENERIC,

  LET,
  CONST,
  FN,
  IF,
  ELSE,
  MOD,
  STRUCT,
  TRAIT,
  PUB,
  DYN,
  IMPORT,
  INTERFACE,
  RETURN,
  CONTINUE,
  BREAK,
  STATIC,
  WHILE,
  FOR,
  AS,
  TYPEOF,
  IN,
  TYPE,
  UNSAFE,
  MATCH,
  CASE,
  IMPL,
  DEFER,
  // Operators as keywords
  LT,
  LTE,
  GT,
  GTE,
  NE,
  EQ,
  AND,
  OR,
  // Reserved Macros
  ASSERT_MACRO,
  ASSERT_OR_MACRO,
  NUMBER_MACRO,
  BOOL_MACRO,
  STRING_MACRO,
};

const unordered_map<string, TokenKind> reserved_lu = {
    {"let", LET},
    {"const", CONST},
    {"pub", PUB},
    {"mod", MOD},
    {"dyn", DYN},
    {"struct", STRUCT},
    {"import", IMPORT},
    {"impl", IMPL},
    {"defer", DEFER},
    {"as", AS},
    {"interface", INTERFACE},
    {"fn", FN},
    {"if", IF},
    {"else", ELSE},
    {"return", RETURN},
    {"continue", CONTINUE},
    {"break", BREAK},
    {"static", STATIC},
    {"while", WHILE},
    {"for", FOR},
    {"as", AS},
    {"trait", TRAIT},
    {"typeof", TYPEOF},
    {"in", IN},
    {"type", TYPE},
    {"unsafe", UNSAFE},
    {"match", MATCH},
    {"case", CASE},

    // Operators as keywords
    {"lt", LT},
    {"lte", LTE},
    {"gt", GT},
    {"gte", GTE},
    {"ne", NE},
    {"not", NOT},
    {"eq", EQ},
    {"and", AND},
    {"or", OR},

    // Reserved Macros
    {"@assert", ASSERT_MACRO},
    {"@assert_or", ASSERT_OR_MACRO},
    {"@string", STRING_MACRO},
    {"@number", NUMBER_MACRO},
    {"@bool", BOOL_MACRO},
};

string token_tag(TokenKind kind);

struct Token {
  shared_ptr<lexer::SourcePos> pos;
  TokenKind kind;
  string value;

  // Default constructor
  Token() {}

  Token(lexer::SourcePos pos, TokenKind kind, string value)
      : pos(make_shared<SourcePos>(pos)), kind(kind), value(value) {}

  Token(TokenKind kind, string value) : kind(kind), value(value) {
    pos->file = nullptr;
    pos->line = 0;
    pos->start = 0;
    pos->end = 0;
  }

  void display() const {
    std::cout << bold_white(token_tag(this->kind)) << " (";

    if (this->kind == IDENTIFIER || this->kind == NUMBER ||
        this->kind == STRING) {
      std::cout << blue(this->value);
    }

    std::cout << ")\n";
  }
};

}  // namespace lexer