#pragma once

#include "ast.h"

namespace ast {

    struct SymbolType : public Expr {
        string symbol;

        SymbolType() { kind = SYMBOL_TYPE; }
        std::string debug(size_t depth) = 0;
    };

    struct SliceType : public Expr {
        shared_ptr<Type> type;

        SliceType() { kind = SLICE_TYPE; }
        std::string debug(size_t depth) = 0;
    };

};