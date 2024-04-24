#pragma once

#include "ast.h"

namespace ast {

    struct ModuleStmt : public Stmt {
        bool is_entry;
        string name;
        vector<shared_ptr<Stmt>> body;

        ModuleStmt() { kind = MODULE_STMT; }
        std::string debug(size_t depth) = 0;
    };

    struct ProgramStmt : public Stmt {
        shared_ptr<ModuleStmt> entry;
        vector<shared_ptr<ModuleStmt>> modules;

        ProgramStmt() { kind = PROGRAM_STMT; }
        std::string debug(size_t depth) = 0;
    };


};