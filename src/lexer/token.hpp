#pragma once

#include "../includes.hpp"
#include <cstddef>

// Used to keep track of relevant file information which is used for debugging, logging, error reporting, module tracing, and compilation.
struct ModuleFileRef {
    string filepath; // the path given to lexer. eg: use std.io `std.io` would be the filepath
    string absolute_path; // the absolute path of the file. // In the avove example, it would be @std/io.br OR @std.io/io.br and this will need to be determined before lexing time
    string folder; // what folder the file is in
    string data; // raw data for the file // TODO: Look into deleteing this after each lexer operation (or not if it will be more memory effecient ...)
    bool from_import; // whether this lexing operation is coming as the result of following the `use` keyword.
};

namespace lexer {
    enum TokenKind {
        END_FILE,
        IDENTIFIER,
        NUMBER,
        CHAR, // 'c' // just like a CString
        STRING, // *char but with a length

        // RESERVED KEYWORDS
        USE,
        EXPORT,
        AS,
        PUB,

        TRAIT,
        IMPL,
        STRUCT,
        SELF,

        FN,
        RETURN,
        ENUM,
        MATCH,
        EXTERN,
        FOR,
        IF,
        IN,
        DYN,

        // Grouping
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
        RCURLY,
        LCURLY,
        DOT,
        DOTDOT,

        COLON,
        SEMICOLON,
        COLON_COLON,

        PLUS,
        PLUS_EQUALS,
        MINUS,
        MINUS_EQUALS,
        STAR,
        STAR_EQUALS,
        SLASH,
        MODULUS,

        NOT,
        NOT_EQUALS,
        LESS_THAN,
        LESS_THAN_EQ,
        GREATER_THAN,
        GRETER_THAN_EQ,
        ASSIGNMENT,
        EQUALS,
    };

    const unordered_map<string, TokenKind> reserved_lu = {

    };

    struct Token {
        TokenKind kind;
        string value;
        size_t line;
        size_t offset;
        size_t length;
        shared_ptr<ModuleFileRef> file;
    };
}