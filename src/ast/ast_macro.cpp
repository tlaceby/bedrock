#include "ast_macro.h"

using namespace ast;
using std::to_string;
using utils::space;

string StrMacro::debug(size_t depth) {
  string str = space(depth);
  str += red("@Str") + "\n";
  str += expr->debug(depth + 1);
  return str;
}

string NumMacro::debug(size_t depth) {
  string str = space(depth);
  str += red("@Num") + "\n";
  str += expr->debug(depth + 1);
  return str;
}

string FmtMacro::debug(size_t depth) {
  string str = space(depth);
  str += red("@Fmt") + "\n";
  str += space(depth + 1) + magenta("FormatString") + ": " + formatString + "\n";
  str += space(depth + 1) + magenta("Args") + ":\n";

  for (const auto &arg : args) {
    str += arg->debug(depth + 2);
  }

  str += "\n";
  return str;
}

string LogMacro::debug(size_t depth) {
  string str = space(depth);
  str += red("@Log") + "\n";
  str += expr->debug(depth + 1);
  return str;
}