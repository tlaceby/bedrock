#include "ast_type.h"

#include "ast.h"

using namespace ast;
using utils::space;

//  SymbolType
string ast::SymbolType::debug(size_t depth) {
  return space(depth) + bold_yellow("Symbol") + "(" + symbol + ")\n";
}

//  Pointer Type
string ast::PointerType::debug(size_t depth) {
  string output = space(depth) + bold_yellow("Pointer") + ":\n";
  output += space(depth + 1) + blue("Underlying") + ":\n";
  output += type->debug(depth + 2);

  return output;
}

string ast::FnType::debug(size_t depth) {
  string out = space(depth) + bold_yellow("Fn") + ":\n";

  if (generics.size() > 0) {
    out += space(depth + 1) + blue("Generics") + ":\n";

    for (const auto& generic : generics) {
      out += generic->debug(depth + 2);
    }

    out += "\n";
  }

  if (params.size() > 0) {
    out += space(depth + 1) + blue("Variadic") + ": ";
    out += yellow(string(variadic ? "true" : "false")) + "\n";
    out += space(depth + 1) + blue("Parameters") + ":\n";

    for (const auto& param : params) {
      out += param.type->debug(depth + 2);
    }

    out += "\n";
  }

  out += space(depth + 1) + blue("Returns") + ":\n";
  out += returns->debug(depth + 2) + "\n";
  return out;
}
