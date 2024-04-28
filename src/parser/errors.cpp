#include "parser.h"

using namespace parser;

std::string error_kind(ParserError kind) {
  switch (kind) {
    case Fatal:
      return "Fatal";
    case UnexpectedToken:
      return "UnexpectedToken";
    default:
      return "UnknownParserError";
  }
}

std::string parser::Error::str() { return ""; }

void parser::Error::display(bool verbose) {
  std::cout << str() << (verbose ? "" : "");
}