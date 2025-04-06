#pragma once
#include "../includes.hpp"
#include "../utils//colors.hpp"
#include <cstddef>
#include <sstream> 


namespace errors {
    // The subtype for LexicalError
    enum LexicalErrorKind {
        InvalidFilePath,
        UnknownErrorOccured,
        InvalidNumberFormat,
        InvalidStringFormat,
        InvalidCharFormat,
        UnxepextedToken,
        UnexpectedEOF,
    };

    inline string error_kind_to_string(LexicalErrorKind kind) {
        switch (kind) {
            case InvalidFilePath: return "Invalid File Path";
            case UnknownErrorOccured: return "Unknown Error";
            case InvalidNumberFormat: return "Invalid Number Format";
            case InvalidStringFormat: return "Invalid String Format";
            case InvalidCharFormat: return "Invalid Char Format";
            case UnxepextedToken: return "Unexpected Token";
            case UnexpectedEOF: return "Unexpected End of File";
            default: return "Unknown";
        }
    }

    // Represents an error which occurs during tokenization of bedrock files
    class LexicalError{
    public:
        size_t line;
        size_t file_offset;
        string filepath;
        string message;
        LexicalErrorKind kind;

        LexicalError (LexicalErrorKind k, string msg) {
            kind = k;
            message = msg;
        }

        LexicalError* add_location(string abs_path, size_t line, size_t offset) {
            this->filepath = abs_path;
            this->line = line;
            this->file_offset = offset;
            return this;
        }

        LexicalError* add_location(string abs_path) {
            this->filepath = abs_path;
            return this;
        }
        
        string error_message() const {
            std::stringstream ss;
        
            ss << bold_red("[Lexical Error]");
            
            if (line > 0 && file_offset > 0) {
                ss << "(" << to_string(line) << ":" << to_string(file_offset) << ")";
            }
            
            ss << " " << bold_white(error_kind_to_string(kind)) << "\n";
        
            if (!filepath.empty()) {
                ss << "  → " << bold("File: ") << filepath << "\n";
            }
        
            if (line > 0 || file_offset > 0) {
                ss << "     at " << "line " << yellow(to_string(line))
                   << ", offset " << yellow(to_string(file_offset)) << "\n";
            }
        
            ss << "\n  " << message << "\n";
        
            return ss.str();
        }
        
        void display() const {
            cout << error_message() << endl;
        }
    };

    inline LexicalError lexical_error(LexicalErrorKind k, string msg1) {
        return LexicalError(k, msg1);
    }
}