#include "bedrock.h"

using namespace errors;
#include "util/colors.h"

std::string errors::error_kind(ErrKind kind) {
  switch (kind) {
    case InvalidFilePath:
      return "InvalidFilePath";
    case UnexepectedEOF:
      return "UnexpectedEOF";
    case MultipleFloatingPoints:
      return "MultipleFloatingPoints";
    case MissingStringTermination:
      return "MissingStringTermination";
    case UnexpectedToken:
      return "UnexpectedToken";
    case Fatal:
      return "Fatal";
    default:
      return "UnknownParserError";
  }
}

Err::Err(ErrKind _kind) { kind = _kind; }
Err& Err::location(std::shared_ptr<lexer::TokenPos> _loc) {
  loc = _loc;
  return *this;
}

Err& Err::message(std::string _msg) {
  msg = _msg;
  return *this;
}

Err& Err::hint(std::string hint) {
  hints.push_back(hint);
  return *this;
}

std::string Err::str() {
  // TODO: Enable colored output
  std::string err;

  err += bold_red("Error") + "::" + error_kind(this->kind) + "\n";
  err += bold_magenta(" .Message") + ": " + this->msg + "\n";

  if (this->hints.size() > 0) {
    err += bold_green(" .Suggestions") + ":\n";
    for (const string& hint : this->hints) {
      err += "   - " + hint + "\n";
    }
  }

  // TODO: Impliment Location Reporting
  // TODO: Impliment code snippet view
  if (this->loc) {
    err += " .Location: " + this->loc->error_str();
  }

  err += "\n";
  return err;
}

void Err::display() { std::cout << this->str(); }