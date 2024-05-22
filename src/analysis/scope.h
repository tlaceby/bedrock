#pragma once
#include "../bedrock.h"

// Forward Declaration
namespace ast {
struct Expr;
struct Stmt;
struct Type;
};  // namespace ast

/// @brief Contains contents for static analysis and typechecking.
namespace analysis {
struct Type;  // forward declare type.

struct Scope {
  shared_ptr<Scope> parent;
  string name;

  bool is_global;
  bool is_entry;  // if in a module and the module has the main fn
  bool is_module;
  bool is_function;
  bool is_loop;
  bool is_unsafe;
  bool is_static;
  bool is_impl;

  // Meta info
  unordered_map<string, bool> constants;
  unordered_map<string, bool> exported;

  unordered_map<string, shared_ptr<Type>> types;
  unordered_map<string, shared_ptr<Type>> symbols;
  vector<shared_ptr<Type>> found_return_types;

  static unordered_map<string, shared_ptr<Scope>> modules;
  static shared_ptr<Scope> global;
  static void debugAllScopes();
  void debugScope();

  void defineSymbol(string name, shared_ptr<analysis::Type> type,
                    bool constant);
  void defineSymbol(string name, shared_ptr<analysis::Type> type);

  void defineType(string name, shared_ptr<analysis::Type> type);
  bool symbolExists(string name);
  bool typeExists(string name);

  shared_ptr<Type> resolveSymbol(string name);
  shared_ptr<Type> resolveType(string name);

  Scope* get_module();
};
};  // namespace analysis