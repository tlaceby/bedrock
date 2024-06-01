#include "ast_stmt.h"

#include "ast_type.h"

using namespace ast;
using utils::space;

// ModuleStmt
string ModuleStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("Module") + "(" + name + ")\n";

  for (const auto &stmt : body) {
    output += stmt->debug(depth + 2) + "\n";
  }

  return output;
}

// ProgramStmt
string ProgramStmt::debug(size_t depth) {
  string output = bold_white("Program\n");

  for (const auto &mod : modules) {
    output += mod->debug(depth) + "\n";
  }

  return output;
}

// BlockStmt
string BlockStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("Block\n");

  for (const auto &stmt : body) {
    output += stmt->debug(depth + 2) + "\n";
  }

  return output;
}

// StructStmt

string displayProperty(size_t depth, ast::PropertyKey prop) {
  string out = space(depth) + bold_green(prop.name) + ": \n";

  out += space(depth + 1) + blue("Public") + ": ";
  out += string((prop.is_pub || prop.is_static) ? "true" : "false") + "\n";

  out += space(depth + 1);
  out += blue("Static") + ": " + string(prop.is_static ? "true" : "false");

  out += "\n" + prop.type->debug(depth + 1);
  return out;
}

string StructStmt::debug(size_t depth) {
  string out = space(depth) + bold_magenta("Struct");
  out += "." + this->name;

  if (this->generics.size() > 0) {
    out += " <";

    for (size_t i = 0; i < this->generics.size(); i++) {
      out += this->generics.at(i);

      if (i < this->generics.size() - 1) {
        out += ", ";
      }
    }

    out += ">";
  }

  out += "\n" + space(depth + 1) + blue("Pub") + ": ";
  out += yellow(string(this->pub ? "true" : "false")) + "\n";

  if (this->properties.size() > 0) {
    out += space(depth + 1) + blue("Properties") + ": \n";

    for (const auto &property : properties) {
      out += displayProperty(depth + 2, property);
    }

    out += "\n";
  }

  return out;
}

// FnDeclStmt
string FnDeclStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("FnDecl") + "\n";
  output += space(depth + 1) + blue("Name") + ": " + this->name + "\n";
  output += space(depth + 1) + blue("Variadic") + ": ";
  output += string(this->variadic ? "true" : "false") + "\n";

  if (return_type) {
    output += space(depth + 1) + blue("Returns") + ": \n";
    output += return_type->debug(depth + 2);
  }

  output += body->debug(depth + 1);
  return output;
}

// VarDeclStmt
string VarDeclStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("VarDecl") + "\n";
  output += space(depth + 1) + blue("Varname") + ": " + this->varname + "\n";
  output += space(depth + 1) + blue("Constant") + ": ";
  output += string(this->constant ? "true" : "false") + "\n";

  if (type) {
    output += space(depth + 1) + blue("Type") + ":\n";
    output += type->debug(depth + 2);
  }

  if (value) {
    output += space(depth + 1) + blue("Value") + ":\n";
    output += value->debug(depth + 2);
  }

  return output;
}

// DeferStmt
string DeferStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("Defer") + "\n";

  for (const auto &action : actions) {
    output += action->debug(depth + 1) + "\n";
  }

  return output;
}

// ImplStmt
string ImplStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("Impl") + type->debug(0) + "\n";
  return output;
}

// ExprStmt
string ExprStmt::debug(size_t depth) {
  return expr->debug(depth);
}

string ReturnStmt::debug(size_t depth) {
  string output = space(depth) + bold_magenta("Return") + "\n";

  if (rhs) {
    output += rhs->debug(depth + 1) + "\n";
  }

  return output;
}