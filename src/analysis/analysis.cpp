#include "../ast/ast_stmt.h"
#include "analsyis.h"

using namespace analysis;
using namespace ast;

shared_ptr<Scope> Scope::global = make_shared<Scope>();
unordered_map<string, shared_ptr<Scope>> Scope::modules;

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