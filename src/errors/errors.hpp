#pragma once
#include "../includes.hpp"

namespace errors {
    enum ErrorKind {
        LexicalError,
        ParsingError,
        TypecheckingError,
        CodeGenerationError,
        RuntimeError,
    };

    class Error {
    public:
        ErrorKind kind;
        string message;
        string secondaryMessage;
        vector<string> hints;

        virtual void display() = 0;
        virtual string error_message() = 0;
        inline string kind_string() {
            switch (kind) {
                case LexicalError: return "LexicalError";
                case ParsingError: return "ParsingError";
                case TypecheckingError: return "TypecheckingError";
                case CodeGenerationError: return "CodeGenerationError";
                default: return "RuntimeError";
            }
        }
    };
}