#pragma once
#include "errors.hpp"

namespace errors {
    // The subtype for LexicalError
    enum LexicalErrorKind {
        UnknownErrorOccured,
        InvalidNumberFormat,
        InvalidStringFormat,
        InvalidCharFormat,
        UnexpectedEOF,
    };

    // Represents an error which occurs during tokenization of bedrock files
    class LexicalError : Error {
        int line;
        int file_offset;
        string filepath;
        LexicalErrorKind subkind;
        LexicalError (LexicalErrorKind k, string fp, string msg1) {
            kind = ErrorKind::LexicalError;
            subkind = k;
            filepath = fp;
            message = msg1;
        }

        LexicalError* add_location(int line, int file_offset) {
            this->line = line;
            this->file_offset = file_offset;
            return this;
        }

        LexicalError* add_msg(string msg2) {
            this->secondaryMessage = msg2;
            return this;
        }

        void display() {
            cout << error_message() << endl;
        }

        string error_message() { return "lexical error occured"; }
    };
}