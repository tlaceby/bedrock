#include "ast_type.h"

using namespace ast;
using utils::space;

//  SymbolType
string SymbolType::debug(size_t depth) {
  return space(depth) + bold_yellow("Symbol") + "(" + symbol + ")\n";
}

//  Pointer Type
string PointerType::debug(size_t depth) {
  string output = space(depth) + bold_yellow("Pointer") + ":\n";
  output += space(depth + 1) + blue("Underlying") + ":\n";
  output += type->debug(depth + 2);

  return output;
}

//  FnType
string FnType::debug(size_t depth) {
  string output = space(depth) + bold_yellow("Fn") + "\n";

  return output;
}