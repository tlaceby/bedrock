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


#define byte uint8_t

/// @brief Supresses Unused message from clang/gcc
#define UNUSED(x) (void)(x)
/// @brief Impliment a todo message. Will gracefuly exit the program after
/// displaying message
#define TODO(message)                                                                                                  \
  printf("Unimplimented %s\n", message);                                                                               \
  exit(1)