#include <fstream>

#include "files.h"

using std::istreambuf_iterator, std::ifstream;

optional<string> utils::read_file_contents(const string& file_path) {
  ifstream file(file_path);

  if (!file) {
    return std::nullopt;
  }

  string contents((istreambuf_iterator<char>(file)),
                  istreambuf_iterator<char>());

  if (file.bad()) {
    return std::nullopt;
  }

  return contents;
}

void utils::write_file_contents(const std::string& file_path,
                                const std::string& data) {
  std::ofstream file(file_path, std::ios::out | std::ios::trunc);

  if (!file) {
    throw std::runtime_error("Error opening file: " + file_path);
  }

  file << data;

  if (!file) {
    throw std::runtime_error("Error writing to file: " + file_path);
  }
}

std::string utils::sanitize_file_path(const std::string& file_path) {
  std::string result = file_path;

  // Replace all '.' with '~'
  std::replace(result.begin(), result.end(), '.', '~');

  // Replace all '/' with '-'
  std::replace(result.begin(), result.end(), '/', '_');

  return result;
}