#include <stdlib.h>

#include <iostream>

#include "../analysis/types.h"
#include "../ast/ast_stmt.h"
#include "gen.h"
#include "stdio.h"

using namespace compiler;
using namespace ast;
using namespace analysis;

string compiler::compile(shared_ptr<Stmt> stmt, shared_ptr<Scope> scope) {
  switch (stmt->kind) {
    case PROGRAM_STMT:
      return compile_program_stmt(static_cast<ProgramStmt*>(stmt.get()));
    case FN_DECL_STMT:
      return compile_fn_decl_stmt(static_cast<FnDeclStmt*>(stmt.get()), scope);
    case VAR_DECL_STMT:
      return compile_var_decl_stmt(static_cast<VarDeclStmt*>(stmt.get()),
                                   scope);
    default:
      std::cout << stmt->debug(0);
      TODO("^^^^^^^^ Unimplimented COMPILE PATH: ^^^^^^^");
  }

  return "UNKNOWN";
}

string compiler::compile_program_stmt(ProgramStmt* stmt) {
  string buildDir = COMPILED_FILES_PATH;
  string buildCommand = "gcc " + buildDir + "main.c -o " + buildDir + "debug";
  string entry = "#include<stdio.h>\n#include<stdlib.h>\n\n";
  size_t modi = 0;

  // Make sure folder exists
  string dbgPath = string(COMPILED_FILES_PATH + ".headers/");
  system(string(string("rm -rf ") + COMPILED_FILES_PATH).c_str());
  system(string(string("mkdir ") + COMPILED_FILES_PATH).c_str());
  system(string(string("mkdir ") + dbgPath).c_str());

  for (const auto& mod : stmt->modules) {
    mod->name = to_string(modi);
    string headerName = "_mod" + mod->name;
    string headerPath = ".headers/" + headerName + ".h";

    compile_module_stmt(mod.get());
    entry += "#include \"" + headerPath + "\"\n";
  }

  entry += "\nint main (const int argc, char** args) ";
  entry += "{ __br__main__();";
  entry += "printf(\"Hello world\\n\"); return 0;}\n";

  utils::write_file_contents(buildDir + "main.c", entry);
  system(buildCommand.c_str());
  return "";
}

string compiler::compile_module_stmt(ModuleStmt* stmt) {
  string headerName = "_mod" + stmt->name;
  string h = "#ifndef " + headerName + "\n" + "#define " + headerName + "\n\n";
  stmt->scope->is_entry = stmt->is_entry;

  for (const auto& s : stmt->body) {
    switch (s->kind) {
      case FN_DECL_STMT:
      case STRUCT_STMT:
      case IMPL_STMT:
      case VAR_DECL_STMT:
        h += compile(s, stmt->scope);
        break;
      default:
        s->debug(0);
        printf(
            "^^^^^^^ Cannot perform this action in the root of a module. "
            "^^^^^^^\n");
    }
  }

  h += "\n#endif//finish include header";

  // write header to builds folder
  string headerPath = COMPILED_FILES_PATH + ".headers/" + headerName + ".h";
  utils::write_file_contents(headerPath, h);
  return h;
}

string compiler::compile_block_stmt(BlockStmt* stmt, shared_ptr<Scope> env) {
  string out = "\n{ ";

  for (const auto& s : stmt->body) {
    out += compile(s, stmt->scope);
  }

  out += "}\n";
  return out;
}

string compiler::compile_fn_decl_stmt(FnDeclStmt* stmt, shared_ptr<Scope> env) {
  string out = "";
  auto fnType =
      static_cast<analysis::FnType*>(env->resolveSymbol(stmt->name).get());

  string fnName = stmt->name;

  // Make sure main is not being created outside the main module.
  // rename main to be called from the correct main
  if (fnName == "main") {
    fnName = "__br__main__";

    if (!((env->get_module())->is_entry)) {
      printf(
          "Current module cannot have a main function as this is not the main "
          "entry point of the application.\n");
      exit(1);
    }
  }

  // Generate Signature
  out += fnType->returns->str() + " ";
  out += fnName + " (";

  for (const auto& param : fnType->params) {
    out += param.name + " " + param.type->str() + ", ";
  }

  out += ") ";
  out += compile_block_stmt(stmt->body.get(), stmt->body->scope);
  return out;
}

string compiler::compile_var_decl_stmt(VarDeclStmt* stmt,
                                       shared_ptr<Scope> env) {
  string out = stmt->constant ? "const " : "";
  out += env->symbols[stmt->varname]->str();
  out += " " + stmt->varname;

  if (stmt->value) {
    out += " = " + compile(stmt->value, env);
  }

  out += ";\n";
  return out;
}

string compiler::compile_defer_stmt(DeferStmt* stmt, shared_ptr<Scope> env) {
  UNUSED(stmt);
  return "";
  //
}
