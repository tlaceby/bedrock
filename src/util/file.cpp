#include "files.h"

#include <fstream>

using std::istreambuf_iterator, std::ifstream;

optional<string> utils::read_file_contents(const string& file_path) {
  ifstream file(file_path);

  if (!file) {
    return std::nullopt;
  }

  string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

  if (file.bad()) {
    return std::nullopt;
  }

  return contents;
}