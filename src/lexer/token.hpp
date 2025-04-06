#pragma once

#include "../includes.hpp"
#include <cstddef>

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
        REF,

        FN,
        RETURN,
        ENUM,
        MATCH,
        EXTERN,
        FOR,
        IF,
        ELSE,
        IN,
        DYN,
        BREAK,
        CONTINUE,

        // Grouping
        LPAREN,
        RPAREN,
        LBRACKET,
        RBRACKET,
        RCURLY,
        LCURLY,
        DOT,
        DOT_DOT,

        COLON,
        SEMICOLON,
        COLON_COLON,
        ARROW,
        QUESTION,
        AT,
        COMMA,

        PLUS,
        PLUS_PLUS,
        PLUS_EQUALS,
        MINUS,
        MINUS_MINUS,
        MINUS_EQUALS,
        STAR,
        STAR_EQUALS,
        SLASH,
        SLASH_EQUALS,
        PERCENT,
        AMPERSAND,

        NOT,
        NOT_EQUALS,
        LESS_THAN,
        LESS_THAN_EQ,
        GREATER_THAN,
        GREATER_THAN_EQ,
        ASSIGNMENT,
        EQUALS,
    };

    const unordered_map<string, TokenKind> reserved_lu = {
        {"use", USE},
        {"export", EXPORT},
        {"as", AS},
        {"pub", PUB},

        {"trait", TRAIT},
        {"impl", IMPL},
        {"struct", STRUCT},
        {"self", SELF},
        {"ref", REF},

        {"fn", FN},
        {"return", RETURN},
        {"enum", ENUM},
        {"match", MATCH},
        {"extern", EXTERN},
        {"for", FOR},
        {"if", IF},
        {"else", ELSE},
        {"in", IN},
        {"dyn", DYN},
        {"break", BREAK},
        {"continue", CONTINUE},
    };

    struct Token {
        TokenKind kind;
        string value;
        size_t line;
        size_t offset;
        shared_ptr<ModuleFileRef> file;
        
        void display() {
            cout << "[" << kind_str() << "]";
            if (of_type(IDENTIFIER, STRING, NUMBER, CHAR)) {
                cout << ": " + value;
            }

            cout << "\n";
        }

        string kind_str() {
            switch (kind) {
                case END_FILE: return "eof";
                case IDENTIFIER: return "identifier";
                case NUMBER: return "number";
                case CHAR: return "char";
                case STRING: return "string";
        
                case USE: return "use";
                case EXPORT: return "export";
                case AS: return "as";
                case PUB: return "pub";
        
                case TRAIT: return "trait";
                case IMPL: return "impl";
                case STRUCT: return "struct";
                case SELF: return "self";
                case REF: return "ref";
        
                case FN: return "fn";
                case RETURN: return "return";
                case ENUM: return "enum";
                case MATCH: return "match";
                case EXTERN: return "extern";
                case FOR: return "for";
                case IF: return "if";
                case ELSE: return "else";
                case IN: return "in";
                case DYN: return "dyn";
                case BREAK: return "break";
                case CONTINUE: return "continue";
        
                case LPAREN: return "lparen";
                case RPAREN: return "rparen";
                case LBRACKET: return "lbracket";
                case RBRACKET: return "rbracket";
                case RCURLY: return "rcurly";
                case LCURLY: return "lcurly";
                case DOT: return "dot";
                case DOT_DOT: return "dot_dot";
        
                case COLON: return "colon";
                case SEMICOLON: return "semicolon";
                case COLON_COLON: return "colon_colon";
                case ARROW: return "arrow";
                case QUESTION: return "question";
                case AT: return "at";
                case COMMA: return "comma";
        
                case PLUS: return "plus";
                case PLUS_PLUS: return "plus_plus";
                case PLUS_EQUALS: return "plus_equals";
                case MINUS: return "minus";
                case MINUS_MINUS: return "minus_minus";
                case MINUS_EQUALS: return "minus_equals";
                case STAR: return "star";
                case STAR_EQUALS: return "star_equals";
                case SLASH: return "slash";
                case SLASH_EQUALS: return "slash_equals";
                case PERCENT: return "percent";
                case AMPERSAND: return "ampersand";
        
                case NOT: return "not";
                case NOT_EQUALS: return "not_equals";
                case LESS_THAN: return "less_than";
                case LESS_THAN_EQ: return "less_than_eq";
                case GREATER_THAN: return "greater_than";
                case GREATER_THAN_EQ: return "greater_than_eq";
                case ASSIGNMENT: return "assignment";
                case EQUALS: return "equals";
        
                default: return "unknown";
            }
        }
        

        template<typename... TokenKind>
        bool of_type(TokenKind... kinds) {
            return ((kinds == kind) || ...);
        }
    };
}