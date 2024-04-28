#include "token.h"

using namespace lexer;

string lexer::SourcePos::error_str() {
  string s = "(" + this->file->file_path + ")[";

  s += to_string(this->line) + ":";
  s += to_string(this->start) + ":";
  s += to_string(this->end) + "]";

  return s;
}

string lexer::SourcePos::get_snippet(bool showUnderline, size_t linePaddings) {
  UNUSED(showUnderline);
  UNUSED(linePaddings);

  TODO("lexer::SourcePos::get_snippet() not yet implimented");
}

string lexer::token_tag(TokenKind kind) {
  switch (kind) {
    case END_FILE:
      return "eof";
    case IDENTIFIER:
      return "identifier";
    case NUMBER:
      return "number";
    case STRING:
      return "string";
    case OPEN_PAREN:
      return "open_paren";
    case CLOSE_PAREN:
      return "close_paren";
    case OPEN_CURLY:
      return "open_curly";
    case CLOSE_CURLY:
      return "close_curly";
    case OPEN_BRACKET:
      return "open_bracket";
    case CLOSE_BRACKET:
      return "close_bracket";
    case COLON:
      return "colon";
    case SEMICOLON:
      return "semicolon";
    case COMMA:
      return "comma";
    case EQUALS:
      return "equals";
    case ASSIGNMENT:
      return "assignment";
    case NOT:
      return "not";
    case NOT_EQUALS:
      return "not_equals";
    case LESS:
      return "less";
    case LESS_EQ:
      return "less_eq";
    case GREATER:
      return "greater";
    case GREATER_EQ:
      return "greater_eq";
    case PLUS:
      return "plus";
    case MINUS:
      return "minus";
    case STAR:
      return "star";
    case SLASH:
      return "slash";
    case PERCENT:
      return "percent";
    case DOT:
      return "dot";
    case DOT_DOT:
      return "dot_dot";
    case ARROW:
      return "arrow";
    case COLON_COLON:
      return "colon_colon";
    case QUESTION:
      return "question";
    case PLUS_PLUS:
      return "plus_plus";
    case MINUS_MINUS:
      return "minus_minus";
    case PLUS_EQUALS:
      return "plus_equals";
    case MINUS_EQUALS:
      return "minus_equals";
    case SLASH_EQUALS:
      return "slash_equals";
    case STAR_EQUALS:
      return "star_equals";
    case AMPERSAND:
      return "ampersand";
    case OPEN_GENERIC:
      return "open_generic";
    case CLOSE_GENERIC:
      return "close_generic";
    case LET:
      return "let";
    case CONST:
      return "const";
    case FN:
      return "fn";
    case IF:
      return "if";
    case ELSE:
      return "else";
    case MOD:
      return "mod";
    case STRUCT:
      return "struct";
    case TRAIT:
      return "trait";
    case PUB:
      return "pub";
    case DYN:
      return "dyn";
    case IMPORT:
      return "import";
    case INTERFACE:
      return "interface";
    case RETURN:
      return "return";
    case CONTINUE:
      return "continue";
    case BREAK:
      return "break";
    case STATIC:
      return "static";
    case WHILE:
      return "while";
    case FOR:
      return "for";
    case AS:
      return "as";
    case TYPEOF:
      return "typeof";
    case IN:
      return "in";
    case TYPE:
      return "type";
    case UNSAFE:
      return "unsafe";
    case MATCH:
      return "match";
    case CASE:
      return "case";
    case LT:
      return "lt";
    case LTE:
      return "lte";
    case GT:
      return "gt";
    case GTE:
      return "gte";
    case NE:
      return "ne";
    case EQ:
      return "eq";
    case AND:
      return "and";
    case OR:
      return "or";
    case ASSERT_MACRO:
      return "@assert";
    case ASSERT_OR_MACRO:
      return "@assert_or";
    case NUMBER_MACRO:
      return "@number";
    case BOOL_MACRO:
      return "@bool";
    case STRING_MACRO:
      return "@string";
    default:
      return "unknown_tk " + to_string(kind);
  }
}