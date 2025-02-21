#pragma once

#include "../includes.h"
#include <regex>

namespace br {
vector<string> str_split(string data, std::regex del_str);
}