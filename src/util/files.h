#pragma once
#include "../bedrock.h"
#include <optional>

namespace utils {

  optional<string> read_file_contents(const string& file_path);

};