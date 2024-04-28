#pragma once

#include "../bedrock.h"

namespace lexer {
enum TokenKind {
  END_FILE,
  IDENT,
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

  LET,
  CONST,
  FN,
  IF,
  ELSE,
  MOD,
  STRUCT,
  TRAIT,
};

struct Token {
  TokenPos pos;
  TokenKind kind;
  string value;
};
}  // namespace lexer