#pragma once

#include "./colors.h"
#include "files.h"
#include "fmt.h"

#include <regex>

namespace utils {
size_t countMatchInRegex(std::string s, std::regex re);
};