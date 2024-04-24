#pragma once
#include <optional>

#include "../bedrock.h"

namespace utils {

optional<string> read_file_contents(const string& file_path);

};