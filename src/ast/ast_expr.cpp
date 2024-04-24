#include "ast_expr.h"

using namespace ast;
using std::to_string;
using utils::space;

//  NumberExpr
string NumberExpr::debug(size_t depth) {
  return space(depth) + "Number (" + to_string(value) + ")\n";
}

//  StringExpr
string StringExpr::debug(size_t depth) {
  return space(depth) + "String (" + value + ")\n";
}

//  SymbolExpr
string SymbolExpr::debug(size_t depth) {
  return space(depth) + "Symbol (" + symbol + ")\n";
}