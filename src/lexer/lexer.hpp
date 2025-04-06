#pragma once

#include <functional>
#include <regex>

#include "token.hpp"
#include "token.hpp"
#include "../errors/lex_error.hpp"

using std::function;
using std::regex;

namespace lexer {
    struct Lexer;

    typedef function<void(Lexer &, const regex &)> regex_handler;

    struct regex_pattern {
    regex re;
    regex_handler handler;
    };

    pair<vector<Token>, vector<errors::LexicalError>> tokenize(string file_path, bool from_import);

    struct Lexer {
        size_t pos;
        size_t line;
        vector<Token> tokens;
        shared_ptr<ModuleFileRef> file;
        vector<errors::LexicalError> errs;
        vector<regex_pattern> patterns;

        Lexer(shared_ptr<ModuleFileRef> mod);
        void advance_n(size_t n);
        void push(Token token);
        bool at_eof();
        string remainder();
    };

    regex_handler default_handler(TokenKind kind, const string &value);
    void string_handler(Lexer &lex, const regex &re);
    void number_handler(Lexer &lex, const regex &re);
    void symbol_handler(Lexer &lex, const regex &re);
    void skip_handler(Lexer &lex, const regex &re);
    void comment_handler(Lexer &lex, const regex &re);

    std::optional<shared_ptr<ModuleFileRef>>  file_ref_from_relative(string relative_path);
    std::optional<shared_ptr<ModuleFileRef>>  file_ref_from_import(string in_code_name);
}