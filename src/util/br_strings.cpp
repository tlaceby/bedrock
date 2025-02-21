#include "br_strings.h"
#include <regex>

std::vector<std::string> br::str_split(const string data, std::regex del) {
  std::sregex_token_iterator it(data.begin(), data.end(), del, -1);
  std::sregex_token_iterator end;

  auto matches = std::vector<std::string>(it->length());

  size_t i = 0;
  while (it != end) {
    matches[i] = i++;
  }

  return matches;
}