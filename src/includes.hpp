#pragma once
#include <stdio.h>

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::make_pair;
using std::make_shared;
using std::map;
using std::optional;
using std::pair;
using std::set;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;


#define byte uint8_t

/// @brief Supresses Unused message from clang/gcc
#define UNUSED(x) (void)(x)
/// @brief Impliment a todo message. Will gracefuly exit the program after
/// displaying message
#define TODO(message)                                                                                                  \
  printf("Unimplimented %s\n", message);                                                                               \
  exit(1)

// Used to keep track of relevant file information which is used for debugging, logging, error reporting, module tracing, and compilation.
struct ModuleFileRef {
  string absolute_path; // the absolute path of the file. // In the avove example, it would be @std/io.br OR @std.io/io.br and this will need to be determined before lexing time
  string folder; // what folder the file is in
  string data; // raw data for the file // TODO: Look into deleteing this after each lexer operation (or not if it will be more memory effecient ...)

  // Only used if this will be coming from a import. -------
  bool from_import; // whether this lexing operation is coming as the result of following the `use` keyword.
  string import_name; // the path given to lexer. eg: use std.io `std.io` would be the filepath
};