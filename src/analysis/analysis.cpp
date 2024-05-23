#include "../ast/ast_stmt.h"
#include "analsyis.h"

using namespace analysis;
using namespace ast;

shared_ptr<Scope> Scope::global = make_shared<Scope>();
unordered_map<string, shared_ptr<Scope>> Scope::modules;

void analysis::Scope::debugAllScopes() {
  if (!DISPLAY_TYPEINFO) return;
  Scope::global->debugScope();

  for (const auto& [_, scope] : Scope::modules) {
    scope->debugScope();
  }

  std::cout << "\n";
}

void analysis::Scope::debugScope() {
  size_t PADDING = 10;
  if (!DISPLAY_TYPEINFO) return;
  using std::cout;
  using utils::space;
  int structCount = 0;

  if (is_global) {
    cout << "\n----  " << bold_magenta("Globals") << "  ----\n";
  } else
    cout << "\n---- " << bold_magenta(name) << "  ----\n";

  if (types.size() > 0) {
    // Count struct occurences
    for (const auto& [name, type] : types) {
      if (type->kind == STRUCT) {
        structCount += 1;
      }
    }

    // Display Non Struct Properties
    if (types.size() != structCount) {
      cout << yellow("types") << ":\n";
      for (const auto& [name, type] : types) {
        if (type->kind == STRUCT) {
          continue;
        }

        cout << "  " << white(name) << ":"
             << space((uint)(PADDING - name.size()));
        cout << type->str() << "\n";
      }

      std::cout << "\n";
    }
  }

  if (structCount > 0) {
    for (const auto& [_, type] : types) {
      if (type->kind == STRUCT) {
        cout << AS_STRUCT(type)->debugScopeStr();
      }
    }

    std::cout << "\n";
  }

  if (symbols.size() > 0) {
    cout << yellow("symbols") << ":\n";
    for (const auto& [name, type] : symbols) {
      cout << "  " << white(name) << ":"
           << space((uint)(PADDING - name.size()));
      cout << type->str() << "\n";
    }

    std::cout << "\n";
  }

  cout << "\n\n";  // end
}

// Scope Functions
void analysis::Scope::defineSymbol(string name, shared_ptr<analysis::Type> type,
                                   bool constant) {
  symbols[name] = type;
  constants[name] = constant;
}

void analysis::Scope::defineSymbol(string name,
                                   shared_ptr<analysis::Type> type) {
  symbols[name] = type;
  constants[name] = false;
}

void analysis::Scope::defineType(string name, shared_ptr<analysis::Type> type) {
  types[name] = type;
}

bool analysis::Scope::symbolExists(string name) {
  return symbols.find(name) != symbols.end();
}

bool analysis::Scope::typeExists(string name) {
  return types.find(name) != types.end();
}

shared_ptr<analysis::Type> analysis::Scope::resolveSymbol(string name) {
  if (symbolExists(name)) {
    return symbols[name];
  }

  if (parent) {
    return parent->resolveSymbol(name);
  }

  if (global->symbols.find(name) != global->symbols.end()) {
    return global->symbols[name];
  }

  return nullptr;
}

shared_ptr<analysis::Type> analysis::Scope::resolveType(string name) {
  if (typeExists(name)) {
    return types[name];
  }

  if (parent) {
    return parent->resolveType(name);
  }

  if (global->types.find(name) != global->types.end()) {
    return global->types[name];
  }

  return nullptr;
}

Scope* analysis::Scope::get_module() {
  if (parent == nullptr && is_module) {
    return this;
  }

  return parent->get_module();
}