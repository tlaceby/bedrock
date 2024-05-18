#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast.h"

using std::shared_ptr;
using std::string;
using std::vector;

namespace ast {

struct SymbolType : public Type {
  string symbol;

  virtual ~SymbolType() {}
  SymbolType() { kind = SYMBOL_TYPE; }
  SymbolType(string symbol) : symbol(symbol) { kind = SYMBOL_TYPE; }
  virtual std::string debug(size_t depth);
};

struct PointerType : public Type {
  shared_ptr<Type> type;

  virtual ~PointerType() {}
  PointerType() { kind = POINTER_TYPE; }
  virtual std::string debug(size_t depth);
};

struct FnType : public Type {
  bool variadic;  // whether the function has variable arity -> ...name: []T
  vector<shared_ptr<Type>> generics;
  vector<PropertyKey> params;
  shared_ptr<Type> returns;

  virtual ~FnType() {}
  FnType() {
    kind = FN_TYPE;
    returns = make_shared<SymbolType>("void");  // set default return
  }
  FnType(vector<shared_ptr<Type>> generics, vector<PropertyKey> params,
         shared_ptr<Type> returns)
      : generics(std::move(generics)),
        params(std::move(params)),
        returns(std::move(returns)) {
    kind = FN_TYPE;
  }

  virtual std::string debug(size_t depth);
};

};  // namespace ast
