#pragma once

namespace parser {
enum BindingPower {
  DEFAULT_BP,
  ASSIGNMENT_BP,
  LOGICAL_BP,
  RELATIONAL_BP,
  ADDITIVE_BP,
  MULTIPLICATIVE_BP,
  UNARY_BP,
  CALL_BP,
  MEMBER_BP,
  PRIMARY_BP,
};
}; // namespace parser