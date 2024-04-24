#include "ast_stmt.h"

using namespace ast;
using utils::space;

// ModuleStmt
string ModuleStmt::debug(size_t depth) {
    string output = space(depth) + "Module." + name + "\n";

    for (const auto& stmt : body) {
        output += stmt->debug(depth + 1);
    }

    return output;
}

// ProgramStmt
string ProgramStmt::debug(size_t depth) {
    string output = "Program\n";

    for (const auto& mod : modules) {
        output += mod->debug(depth + 1);
    }

    return output;
}