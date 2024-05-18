#include "types.h"

using namespace analysis;

std::string analysis::ToPrecisionString(NumberKind kind) {
  switch (kind) {
    case i8:
      return "signed char";
    case i32:
      return "int";
    case i64:
      return "long int";
    case u8:
      return "unsigned char";
    case u32:
      return "unsigned int";
    case u64:
      return "unsigned long int";
    case f32:
      return "float";
    case f64:
      return "double";
    default:
      return "unknown";
  }
}

NumberKind analysis::matchPrecisionFromToken(string numstr) {
  return NumberKind::i32;
}

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

shared_ptr<analysis::NumberType> analysis::MK_NUM(NumberKind k) {
  return make_shared<NumberType>(k);
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