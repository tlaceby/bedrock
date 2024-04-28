#include "ast_expr.h"

using namespace ast;
using std::to_string;
using utils::space;

//  NumberExpr
string NumberExpr::debug(size_t depth) {
  string str = space(depth);
  str += blue("Number") + "(" + yellow(to_string(this->value)) + ")\n";

  return str;
}

//  StringExpr
string StringExpr::debug(size_t depth) {
  string str = space(depth);
  str += bold_blue("String") + "(\"" + yellow(this->value) + "\")\n";

  return str;
}

//  SymbolExpr
string SymbolExpr::debug(size_t depth) {
  string str = space(depth);
  str += bold_blue("Symbol") + "(" + this->symbol + ")\n";

  return str;
}

//  BinaryExpr
string BinaryExpr::debug(size_t depth) {
  string str = space(depth);
  str += bold_blue("Binary ") + this->operation.value + "\n";

  str += space(depth + 1);
  str += magenta("Left") + ":\n" + this->left->debug(depth + 2);
  str += space(depth + 1);
  str += magenta("Right") + ":\n" + this->right->debug(depth + 2);

  return str;
}

//  PrefixExpr
string PrefixExpr::debug(size_t depth) {
  string str = space(depth + 1);
  str += bold_blue("Prefix ") + this->operation.value + "\n";
  str += space(depth + 2);
  str += magenta("Right") + ":\n" + this->right->debug(depth + 3);

  return str;
}