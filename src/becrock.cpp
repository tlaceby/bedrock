#include "bedrock.h"
#include "lexing/lexer.h"

int display_help () {
  using std::cout;

  cout << "\n---------- Bedrock Help ----------\n";

  // Help Option
  cout << "(help|info) `bedrock help` | `bedrock info`\n";
  cout << "  - Displays a list of CLI commands and expected arguments.\n";

  cout << "\n(run) `bedrock run [fn]`\n";
  cout << "  - Builds and executes the in debug mode.\n";
  cout << "  - [fn] The relative or absolute path to a bedrock file which will contain the entry point of the application.\n";

  cout << "\n";

  return 0;
}

int bedrock_run (string file_path) {
  auto tokens = lexer::tokenize(file_path);

  std:: cout << "tokens: " << tokens.size() << "\n";
  return 0;
}

int main (int argc, const char** args) {
  string file_path, command;

  if (argc == 1) {
    return display_help();
  }

  command = std::string(args[1]);

  if (command == "run") {
    if (argc != 3) {
      std::cout << "\nImproper usage of (run) command.";
      return display_help();
    }

    return bedrock_run(std::string(args[2]));
  }

  if (command == "help" || command == "info") {
    return display_help();
  }

  std::cout << "Unknown command " << command << "\n";
  return display_help();
}