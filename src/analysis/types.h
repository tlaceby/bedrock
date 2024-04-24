#pragma once

#include "../bedrock.h"

namespace analysis {
enum TypeKind {
  NUMBER,
  STRING,
  BOOL,
  MODULE,
};

struct Type {
  TypeKind kind;

  virtual string str() = 0;
};

struct NumberType : public Type {
  NumberType() { kind = NUMBER; }
  string str() override { return "Number"; }
};

struct StringType : public Type {
  StringType() { kind = STRING; }
  string str() override { return "String"; }
};

struct BoolType : public Type {
  BoolType() { kind = BOOL; }
  string str() override { return "Bool"; }
};

struct ModuleType : public Type {
  string name;

  ModuleType() { kind = MODULE; }
  string str() override { return "module." + name; }
};
};  // namespace analysis