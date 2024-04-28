#include "ast_stmt.h"

using namespace ast;
using utils::space;

// ModuleStmt
string ModuleStmt::debug(size_t depth) {
  string output = space(depth) + magenta("Module") + "(" + name + ")\n";

  for (const auto& stmt : body) {
    output += stmt->debug(depth + 2);
  }

  return output;
}

// ProgramStmt
string ProgramStmt::debug(size_t depth) {
  string output = bold_white("Program\n");

  for (const auto& mod : modules) {
    output += mod->debug(depth);
  }

  return output;
}

// BlockStmt
string BlockStmt::debug(size_t depth) {
  string output = magenta("Block\n");

  for (const auto& stmt : body) {
    output += stmt->debug(depth + 2);
  }

  return output;
}

// ExprStmt
string ExprStmt::debug(size_t depth) { return expr->debug(depth); }