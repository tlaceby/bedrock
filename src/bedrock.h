#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#define BEDROCK_VERSION 0.1
#define BEDROCK_EXTENSION ".br"
#define BEDROCK_GITHUB "https://github.com/tlaceby/bedrock/"

using std::make_shared;
using std::optional;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;

/// @brief Supresses Unused message from clang/gcc
#define UNUSED(x) (void)(x)
/// @brief
#define TODO() exit(1)