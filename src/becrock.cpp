#include "analysis/analsyis.h"
#include "bedrock.h"
#include "compiler/gen.h"
#include "lexing/lexer.h"
#include "parser/parser.h"
#include "util/colors.h"

inline bool COLORS_ENABLED = true;
inline bool DISPLAY_AST = false;
inline bool DISPLAY_TOKENS = false;
inline bool DISPLAY_TYPEINFO = false;
inline bool DISABLE_BOUND_CHECKING = false;
inline string COMPILED_FILES_PATH = ".builds/";

int display_help() {
  using std::cout;

  cout << white("\n--------------  " + string("Bedrock | v") + BEDROCK_VERSION +
                "  --------------\n\n");

  // HELP
  cout << bold_blue("(help|info)");
  cout << yellow(" `bedrock help` | `bedrock info`\n");
  cout << "  - "
       << white("Displays a list of CLI commands and expected arguments.\n");

  // RUN
  cout << bold_blue("\n(run)");
  cout << yellow(" `bedrock run path/to/file.br`\n");
  cout << "  - " << white("Builds and executes the in debug mode.\n") << "  - ";
  cout << white(
      "[fn] The relative or absolute path to a bedrock file which will "
      "contain "
      "the entry point of the application.\n");

  cout << "\n\n";

  // --no-color
  cout << bold_magenta("[--no-color]") << "\n";
  cout << "  -" << white("Disables colored output to the console/stdout.\n");

  // --no-bounds
  cout << bold_magenta("\n[--no-bounds]") << bold_red("  unsafe\n");
  cout << "  -"
       << white(
              "Disables bound checking at compile time. Often improves "
              "performance at the cost of safety.\n");

  // -----------
  // DEBUG FLAGS
  cout << bold_cyan("\n\n[--ast]\n");
  cout << "  - " << white("Prints the AST to stdout after parsing.\n");

  cout << bold_cyan("\n[--tokens]\n");
  cout << "  - " << white("Prints lexer output to stdout.\n");

  cout << bold_cyan("\n[--types]\n");
  cout << "  - "
       << white(
              "Prints type, scope and variable/symbol usage to stdout after "
              "typechecking and static analysis is done.\n");

  cout << bold_cyan("\n[--debug]\n");
  cout << "  - " << white("Prints ALL debug info to stdout.\n");

  cout << "\n";
  return 0;
}

int bedrock_run(string file_path) {
  auto program = parser::parse(file_path);
  analysis::tc_program(program);
  compiler::compile_program_stmt(program.get());

  system(string(COMPILED_FILES_PATH + "debug").c_str());
  return 0;
}

pair<optional<string>, vector<string>> parse_args(int argc, const char** argv) {
  optional<string> command;
  vector<string> args;

  if (argc < 2) {
    return make_pair(nullptr, args);
  }

  command.emplace(argv[1]);

  for (int i = 2; i < argc; i++) {
    args.push_back(argv[i]);
  }

  return make_pair(command, args);
}

void set_flags(vector<string>& args) {
  for (const auto& arg : args) {
    if (arg == "--no-color" || arg == "--no-colors") {
      COLORS_ENABLED = false;
    }

    if (arg == "--no-bounds") {
      DISABLE_BOUND_CHECKING = true;
    }

    if (arg == "--ast") {
      DISPLAY_AST = true;
    }

    if (arg == "--tokens") {
      DISPLAY_TOKENS = true;
    }

    if (arg == "--types") {
      DISPLAY_TYPEINFO = true;
    }

    if (arg == "--debug") {
      DISPLAY_TYPEINFO = true;
      DISPLAY_AST = true;
      DISPLAY_TOKENS = true;
    }
  }
}

int main(int argc, const char** argv) {
  string file_path;

  if (argc == 1) {
    return display_help();
  }

  auto [commandOpt, args] = parse_args(argc, argv);
  set_flags(args);

  if (commandOpt.has_value()) {
    string command = commandOpt.value();

    if (command == "run") {
      if (args.size() == 0) {
        std::cout << "\nImproper usage of (run) command.";
        return display_help();
      }

      return bedrock_run(args[0]);
    }

    if (command == "help" || command == "info") {
      return display_help();
    }

    std::cout << "Unknown command " << command << "\n";
  }

  return display_help();
}