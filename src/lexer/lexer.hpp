#pragma once

#include <functional>
#include <regex>

#include "token.hpp"
#include "token.hpp"
#include "../errors/errors.hpp"

using std::function;
using std::regex;

namespace lexer {

    struct Lexer;

    typedef function<void(Lexer &, const regex &)> regex_handler;

    struct regex_pattern {
    regex re;
    regex_handler handler;
    };

    pair<vector<Token>, vector<errors::Error>> tokenize(string file_path);

    struct Lexer {
        size_t pos;
        size_t line;
        vector<Token> tokens;
        shared_ptr<ModuleFileRef> file;
        vector<errors::Error> errs;
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
}