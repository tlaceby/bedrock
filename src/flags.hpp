#pragma once

#include <string>

/// @brief Whether not to include colored output when printing or logging to
/// console. This also includes whether the output by native functions will
/// display color. Defaults to true
extern bool COLORS_ENABLED;

/// @brief Whether or not to print the AST to stdout after parsing. Defaults to
/// false.
extern bool DISPLAY_AST;

/// @brief Whether or not tokens will be printed to stdout after lexing is
/// finished. Defaults to false.
extern bool DISPLAY_TOKENS;

/// @brief Whether or not to display a typeinfo & scope information after static
/// analysis is completed.
extern bool DISPLAY_TYPEINFO;

/// @brief Instructs compiler to disable bound checking on slices & strings.
extern bool DISABLE_BOUND_CHECKING;

/// @brief Path to compiled c/header files.
extern std::string COMPILED_FILES_PATH;

/// @brief Path to compiled bedrock @std/ library
extern std::string STANDARD_LIBRARY_PATH;


#define BEDROCK_VERSION "0.1"
#define BEDROCK_EXTENSION ".br"
#define BEDROCK_GITHUB "https://github.com/tlaceby/bedrock/"

#define byte uint8_t

/// @brief Supresses Unused message from clang/gcc
#define UNUSED(x) (void)(x)
/// @brief Impliment a todo message. Will gracefuly exit the program after
/// displaying message
#define TODO(message)                                                                                                  \
  printf("Unimplimented %s\n", message);                                                                               \
  exit(1)