#pragma once
#include <optional>

#include "../bedrock.h"

namespace utils {

optional<string> read_file_contents(const string& file_path);
void write_file_contents(const string& file_path, const string& data);
std::string sanitize_file_path(const std::string& file_path);
};