#pragma once
#include "../bedrock.h"

// Forward Declaration
namespace ast {
    struct Expr;
    struct Stmt;
    struct Type;
};

namespace analysis {
    struct Type; // forward declare type here.

    enum MemoryKind {
        GLOBAL,
        HEAP,
        LOCAL,
    };

    struct Symbol {
        shared_ptr<Type> type;
        size_t assigned;
        size_t accessed;
        MemoryKind mem_location;
    };


    struct Scope {
        shared_ptr<Scope> parent;
        string name;

        bool is_global;
        bool is_module;
        bool is_function;
        bool is_loop;
        bool is_unsafe;
        bool is_static;

        unordered_map<string, shared_ptr<Type>> types;
        unordered_map<string, shared_ptr<Symbol>> symbols;
        vector<shared_ptr<Type>> found_return_types;
    };
};