#include "types.h"

using namespace analysis;

shared_ptr<analysis::VoidType> analysis::MK_VOID() {
  return make_shared<VoidType>();
}

shared_ptr<analysis::BoolType> analysis::MK_BOOL() {
  return make_shared<BoolType>();
}

shared_ptr<analysis::StringType> analysis::MK_STR() {
  return make_shared<StringType>();
}

shared_ptr<analysis::PointerType> analysis::MK_PTR(shared_ptr<Type> u) {
  return make_shared<PointerType>(u);
}

shared_ptr<analysis::NumberType> analysis::MK_NUM() {
  return make_shared<NumberType>();
}

shared_ptr<analysis::FnType> analysis::MK_FN(vector<FnParam> params,
                                             shared_ptr<Type> returns,
                                             bool variadic) {
  return make_shared<FnType>(params, returns, variadic);
}

bool analysis::types_match(shared_ptr<Type> expected,
                           shared_ptr<Type> recieved) {
  if (expected->kind != recieved->kind) {
    return false;
  }

  return expected->str() == recieved->str();  // TODO: Make lose comparison
}