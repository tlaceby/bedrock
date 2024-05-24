#pragma once

#include "../bedrock.h"

namespace ast {
struct BlockStmt;
};

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
  virtual ~NumberType() {
  }
  NumberType() {
    kind = NUMBER;
  }

  string str() override {
    return cyan("Number");
  }
};

struct PointerType : public Type {
  std::shared_ptr<analysis::Type> underlying;

  virtual ~PointerType() {
  }
  PointerType(std::shared_ptr<analysis::Type> u) : underlying(u) {
    kind = POINTER;
  }

  std::string str() override {
    return yellow("&") + underlying->str();
  }
};

struct StringType : public Type {
  virtual ~StringType() {
  }
  StringType() {
    kind = STRING;
  }
  string str() override {
    return cyan("String");
  }
};

struct BoolType : public Type {
  virtual ~BoolType() {
  }
  BoolType() {
    kind = BOOL;
  }
  string str() override {
    return cyan("Bool");
  }
};

struct VoidType : public Type {
  virtual ~VoidType() {
  }
  VoidType() {
    kind = VOID;
  }
  string str() override {
    return cyan("Void");
  }
};

struct ModuleType : public Type {
  string name;

  virtual ~ModuleType() {
  }
  ModuleType() {
    kind = MODULE;
  }
  string str() override {
    return "module." + name;
  }
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
  shared_ptr<ast::BlockStmt> body;

  virtual ~FnType() {
  }
  FnType() {
    kind = FN;
  }
  FnType(vector<FnParam> params, shared_ptr<Type> returns, bool variadic, shared_ptr<ast::BlockStmt> body)
      : params(params), returns(returns), variadic(variadic), body(body) {
    kind = FN;
  }

  string str() override {
    string str = "fn (";

    for (const auto &param : params) {
      str += param.type->str() + " ";
    }

    str += ") -> " + returns->str();
    return str;
  }
};

struct StructType : public Type {
  string name;
  set<string> publicMembers; // whether a member is public or private
  unordered_map<string, shared_ptr<Type>> staticProperties;
  unordered_map<string, shared_ptr<FnType>> staticMethods;
  unordered_map<string, shared_ptr<Type>> properties;
  unordered_map<string, shared_ptr<FnType>> methods;

  virtual ~StructType() {
  }
  StructType(string name) : name(name) {
    kind = STRUCT;
  }

  bool hasField(string propName) {
    return hasProperty(propName, false) || hasProperty(propName, true) || hasMethod(propName, false) ||
           hasMethod(propName, true);
  }

  bool hasProperty(string propName, bool isStatic) {
    if (isStatic) {
      return staticProperties.find(propName) != staticProperties.end();
    }

    return properties.find(propName) != properties.end();
  }

  bool hasMethod(string propName, bool isStatic) {
    if (isStatic) {
      return staticMethods.find(propName) != staticMethods.end();
    }

    return methods.find(propName) != methods.end();
  }

  shared_ptr<Type> getProperty(string propName, bool isStatic) {
    if (isStatic) {
      return staticProperties[propName];
    }

    return properties[propName];
  }

  shared_ptr<FnType> getMethod(string propName, bool isStatic) {
    if (isStatic) {
      return staticMethods[propName];
    }

    return methods[propName];
  }

  string str() override {
    return yellow("Struct") + "." + name + "\n";
  }

  // Output when printing the debug for a scope
  string debugScopeStr() {
    using utils::space;
    size_t PADDING = 10;
    string out;

    out += magenta("Struct") + "." + blue(name) + ":\n";

    // Properties
    if (this->staticProperties.size() > 0 || this->properties.size() > 0) {
      out += yellow("  properties") + ":\n";
      for (const auto &[name, type] : this->staticProperties) {
        out += "    " + red(name) + ":";
        out += space((uint)(PADDING - name.size()));
        out += type->str() += "\n";
      }

      for (const auto &[name, type] : this->properties) {
        out += "    " + white(name) + ":" + space((uint)(PADDING - name.size()));
        out += type->str() += "\n";
      }
    }

    // Methods
    if (this->staticMethods.size() > 0 || this->methods.size() > 0) {
      out += yellow("  methods") + ":\n";
      for (const auto &[name, type] : this->staticMethods) {
        out += "    " + red(name) + ":";
        out += space((uint)(PADDING - name.size()));
        out += type->str() += "\n";
      }

      for (const auto &[name, type] : this->methods) {
        out += "    " + white(name) + ":" + space((uint)(PADDING - name.size()));
        out += type->str() += "\n";
      }
    }

    return out;
  }
};
// Type Creation

shared_ptr<analysis::VoidType> MK_VOID();
shared_ptr<analysis::BoolType> MK_BOOL();
shared_ptr<analysis::StringType> MK_STR();
shared_ptr<analysis::PointerType> MK_PTR(shared_ptr<Type>);
shared_ptr<analysis::NumberType> MK_NUM();
shared_ptr<analysis::StructType> MK_STRUCT(string);
shared_ptr<analysis::FnType> MK_FN(vector<FnParam>, shared_ptr<Type>, bool, shared_ptr<ast::BlockStmt>);

analysis::VoidType *AS_VOID(shared_ptr<Type>);
analysis::BoolType *AS_BOOL(shared_ptr<Type>);
analysis::StringType *AS_STR(shared_ptr<Type>);
analysis::PointerType *AS_PTR(shared_ptr<Type>);
analysis::NumberType *AS_NUM(shared_ptr<Type>);
analysis::StructType *AS_STRUCT(shared_ptr<Type>);
analysis::FnType *AS_FN(shared_ptr<Type>);

bool types_match(shared_ptr<Type> expected, shared_ptr<Type> recieved);
}; // namespace analysis