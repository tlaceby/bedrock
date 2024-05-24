#include "utils.h"

#include <regex>

size_t utils::countMatchInRegex(std::string s, std::regex re) {
  auto words_begin = std::sregex_iterator(s.begin(), s.end(), re);
  auto words_end = std::sregex_iterator();
  return std::distance(words_begin, words_end);
}