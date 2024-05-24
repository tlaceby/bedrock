#include "types.h"
#include "../ast/ast_stmt.h"

using namespace analysis;

static shared_ptr<VoidType> SHARED_VOID = make_shared<VoidType>();
static shared_ptr<StringType> SHARED_STRING = make_shared<StringType>();
static shared_ptr<NumberType> SHARED_NUMBER = make_shared<NumberType>();
static shared_ptr<BoolType> SHARED_BOOLEAN = make_shared<BoolType>();

shared_ptr<analysis::VoidType> analysis::MK_VOID() {
  return SHARED_VOID;
}

shared_ptr<analysis::BoolType> analysis::MK_BOOL() {
  return SHARED_BOOLEAN;
}

shared_ptr<analysis::StringType> analysis::MK_STR() {
  return SHARED_STRING;
}

shared_ptr<analysis::NumberType> analysis::MK_NUM() {
  return SHARED_NUMBER;
}

shared_ptr<analysis::PointerType> analysis::MK_PTR(shared_ptr<Type> u) {
  return make_shared<PointerType>(u);
}

shared_ptr<analysis::StructType> analysis::MK_STRUCT(string name) {
  return make_shared<StructType>(name);
}

shared_ptr<analysis::FnType> analysis::MK_FN(vector<FnParam> params, shared_ptr<Type> returns, bool variadic,
                                             shared_ptr<ast::BlockStmt> body) {
  return make_shared<FnType>(params, returns, variadic, body);
}

bool analysis::types_match(shared_ptr<Type> expected, shared_ptr<Type> recieved) {
  if (expected->kind != recieved->kind) {
    return false;
  }

  return expected->str() == recieved->str(); // TODO: Make lose comparison
}

analysis::VoidType *analysis::AS_VOID(shared_ptr<Type> t) {
  return static_cast<VoidType *>(t.get());
}

analysis::BoolType *analysis::AS_BOOL(shared_ptr<Type> t) {
  return static_cast<BoolType *>(t.get());
}

analysis::StringType *analysis::AS_STR(shared_ptr<Type> t) {
  return static_cast<StringType *>(t.get());
}

analysis::PointerType *analysis::AS_PTR(shared_ptr<Type> t) {
  return static_cast<PointerType *>(t.get());
}

analysis::NumberType *analysis::AS_NUM(shared_ptr<Type> t) {
  return static_cast<NumberType *>(t.get());
}

analysis::StructType *analysis::AS_STRUCT(shared_ptr<Type> t) {
  return static_cast<StructType *>(t.get());
}

analysis::FnType *analysis::AS_FN(shared_ptr<Type> t) {
  return static_cast<FnType *>(t.get());
}
