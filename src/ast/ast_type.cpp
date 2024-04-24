#include "ast_type.h"

using namespace ast;
using utils::space;

//  SymbolType
string SymbolType::debug(size_t depth) {
    return space(depth) + "Symbol (" + symbol + ")\n";
}

//  SliceType
string SliceType::debug(size_t depth) {
    string output = space(depth) + "Slice\n";
    output += space(depth) + "type: ";
    output += type->debug(depth + 1);

    return output;
}