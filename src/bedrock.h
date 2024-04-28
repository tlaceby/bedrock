#pragma once
#include "includes.h"

// bedrock's error handling locic for parsing, lexing and typechecking
#include "errors.h"
#include "flags.h"

#define BEDROCK_VERSION "0.1"
#define BEDROCK_EXTENSION ".br"
#define BEDROCK_GITHUB "https://github.com/tlaceby/bedrock/"

#define byte uint8_t

/// @brief Supresses Unused message from clang/gcc
#define UNUSED(x) (void)(x)
/// @brief Impliment a todo message. Will gracefuly exit the program after
/// displaying message
#define TODO(message)                    \
  printf("Unimplimented %s\n", message); \
  exit(1)