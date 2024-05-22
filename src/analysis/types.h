#pragma once

#include "../bedrock.h"

namespace analysis {
enum TypeKind {
  NUMBER,
  STRING,
  BOOL,
  POINTER,
  VOID,
  MODULE,
  FN,
  STRUCT,

  GENERIC_FN,
  GENERIC_STRUCT,
};

struct Type {
  TypeKind kind;

  virtual string str() = 0;
};

struct NumberType : public Type {
  virtual ~NumberType() {}
  NumberType() { kind = NUMBER; }

  string str() override { return cyan("Number"); }
};

struct PointerType : public Type {
  std::shared_ptr<analysis::Type> underlying;

  virtual ~PointerType() {}
  PointerType(std::shared_ptr<analysis::Type> u) : underlying(u) {
    kind = POINTER;
  }

  std::string str() override { return yellow("&") + underlying->str(); }
};

struct StringType : public Type {
  virtual ~StringType() {}
  StringType() { kind = STRING; }
  string str() override { return cyan("String"); }
};

struct BoolType : public Type {
  virtual ~BoolType() {}
  BoolType() { kind = BOOL; }
  string str() override { return cyan("Bool"); }
};

struct VoidType : public Type {
  virtual ~VoidType() {}
  VoidType() { kind = VOID; }
  string str() override { return cyan("Void"); }
};

struct ModuleType : public Type {
  string name;

  virtual ~ModuleType() {}
  ModuleType() { kind = MODULE; }
  string str() override { return "module." + name; }
};

struct FnParam {
  string name;
  shared_ptr<Type> type;

  FnParam(string n, shared_ptr<Type> t) {
    name = n;
    type = t;
  }
};

struct FnType : public Type {
  vector<FnParam> params;
  shared_ptr<Type> returns;
  bool variadic;

  virtual ~FnType() {}
  FnType() { kind = FN; }
  FnType(vector<FnParam> params, shared_ptr<Type> returns, bool variadic)
      : params(params), returns(returns), variadic(variadic) {
    kind = FN;
  }

  string str() override {
    string str = "fn (";

    for (const auto& param : params) {
      str += param.type->str() + " ";
    }

    str += ") -> " + returns->str();
    return str;
  }
};

// Type Creation

shared_ptr<analysis::VoidType> MK_VOID();
shared_ptr<analysis::BoolType> MK_BOOL();
shared_ptr<analysis::StringType> MK_STR();
shared_ptr<analysis::PointerType> MK_PTR(shared_ptr<Type>);
shared_ptr<analysis::NumberType> MK_NUM();
shared_ptr<analysis::FnType> MK_FN(vector<FnParam>, shared_ptr<Type>, bool);

bool types_match(shared_ptr<Type> expected, shared_ptr<Type> recieved);
};  // namespace analysis