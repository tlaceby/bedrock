#pragma once

#include "includes.h"

// Forward Declarations
namespace lexer {
struct SourceFile {
  string file_path;
  string contents;
};

struct SourcePos {
  shared_ptr<SourceFile> file;
  size_t line;
  size_t start;
  size_t end;

  /// @brief Returns a string representation used for error reporting.
  /// Format: (/path/to/file/filename.br)[line:start:end]
  string error_str();
  /// @brief Returns a snippet of the section of code which is around an section
  /// in the code.
  /// @param showUnderline Whether to show a ^^^^ underneath the error.
  /// @param linePaddings number of lines to include both above and underneath
  /// the error.
  string get_snippet(bool showUnderline, size_t linePaddings);
};
};  // namespace lexer

/// @brief Logic management for handling errors in the lexer | parser |
/// typechecker | codegen
namespace errors {

enum ErrKind {
  InvalidFilePath,           // lexing
  UnexepectedEOF,            // lexing | parsing
  MultipleFloatingPoints,    // lexing
  MissingStringTermination,  // lexing
  UnexpectedToken,           // lexing | parsing
  Fatal,                     // lexing | parsing | analysis | codegen
  ExpectedPrimaryExpr,       // parsing
};

/// @brief Returns a string representation of the ErrKind enum which is passed
/// in.
string error_kind(ErrKind);

/// @brief Represents all Compiletime Errors which can occur. Parser, lexer,
/// typechecking and codegen errors are all represented.
struct Err {
 public:
  string str();
  void display();

  /// @brief Crates an Error type. Represents a error during comptime.
  /// @param ErrorKind Type of error which is being created.
  Err(ErrKind);

  /// @brief Used to update the location of the Error. Returns the instance
  /// which it mutates.
  Err& location(std::shared_ptr<lexer::SourcePos>);
  /// @brief Sets the message for this Error. Returns the mutated instance.
  Err& message(std::string);
  /// @brief Used to update the hints[]. Appends the hint to hints. Returns the
  /// mutated instance.
  Err& hint(std::string);

 private:
  ErrKind kind;
  shared_ptr<lexer::SourcePos> loc;
  string msg;
  vector<string> hints;
};

Err make_error(ErrKind);
};  // namespace errors

using errors::Err;
using errors::ErrKind;