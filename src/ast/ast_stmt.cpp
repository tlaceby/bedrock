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

// StructStmt
string StructStmt::debug(size_t depth) {
  string output = space(depth) + magenta("Struct") + "." + this->name + "\n";
  return output;
}

// FnDeclStmt
string FnDeclStmt::debug(size_t depth) {
  string output = space(depth) + magenta("FnDecl") + "\n";
  output += space(depth + 1) + blue("Name") + ": " + this->name + "\n";
  output += space(depth + 1) + blue("Variadic") + ": ";
  output += string(this->variadic ? "true" : "false") + "\n";

  return output;
}

// VarDeclStmt
string VarDeclStmt::debug(size_t depth) {
  string output = space(depth) + magenta("VarDecl") + "\n";
  output += space(depth + 1) + blue("Varname") + ": " + this->varname + "\n";
  output += space(depth + 1) + blue("Constant") + ": ";
  output += string(this->constant ? "true" : "false") + "\n";

  return output;
}

// ExprStmt
string ExprStmt::debug(size_t depth) { return expr->debug(depth); }