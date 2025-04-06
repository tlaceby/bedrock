#pragma once

#include <exception>
#include <optional>
#include <string>
#include <fstream>

namespace utils {
    namespace fs {
        inline std::optional<std::string> read_file(std::string filepath) {
            try {
                auto size = std::filesystem::file_size(filepath);
                std::string content(size, '\0');
                std::ifstream in(filepath);
                in.read(&content[0], size);

                return std::make_optional(content);
            } catch (std::exception e) {
                return std::nullopt;
            }
            // TODO: Add actual error handling lol
        }
    }
}