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
  ASSIGNMENT,
  NOT,
  NOT_EQUALS,
  EQUALS,
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
  RETURN,
  CONTINUE,
  BREAK,
  WHILE,
  FOR,
  TYPEOF,
  IN,
  TYPE,
  MATCH,
  CASE,
  IMPL,
  DEFER,
  USE,
  MUT,
  UNION,
  OVERRIDE,

  AND,
  OR,

};

const unordered_map<string, TokenKind> reserved_lu = {
    {"let", LET},
    {"const", CONST},
    {"pub", PUB},
    {"mod", MOD},
    {"dyn", DYN},
    {"struct", STRUCT},
    {"impl", IMPL},
    {"defer", DEFER},
    {"fn", FN},
    {"if", IF},
    {"else", ELSE},
    {"return", RETURN},
    {"continue", CONTINUE},
    {"break", BREAK},
    {"while", WHILE},
    {"for", FOR},
    {"trait", TRAIT},
    {"typeof", TYPEOF},
    {"in", IN},
    {"type", TYPE},
    {"match", MATCH},
    {"case", CASE},
    {"use", USE},
    {"mut", MUT},
    {"union", UNION},
    {"override", OVERRIDE},

    // Operators as keywords
    {"and", AND},
    {"or", OR},
};

string token_tag(TokenKind kind);

struct Token {
  shared_ptr<lexer::SourcePos> pos;
  TokenKind kind;
  string value;

  // Default constructor
  Token() {
  }

  Token(lexer::SourcePos pos, TokenKind kind, string value)
      : pos(make_shared<SourcePos>(pos)), kind(kind), value(value) {
  }

  Token(TokenKind kind, string value) : kind(kind), value(value) {
    pos->file = nullptr;
    pos->line = 0;
    pos->start = 0;
    pos->end = 0;
  }

  void display() const {
    std::cout << bold_white(token_tag(this->kind)) << " (";

    if (this->kind == IDENTIFIER || this->kind == NUMBER || this->kind == STRING) {
      std::cout << blue(this->value);
    }

    std::cout << ")\n";
  }
};

} // namespace lexer
