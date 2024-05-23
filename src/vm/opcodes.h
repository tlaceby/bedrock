#pragma once

namespace vm {

enum Opcode {
  OP_HALT, // Expects Number on stack.
  OP_LOG,  // (String | Number | Bool | Overloaded Type) on the stack.
  OP_STR,  // Expects (Number | Bool) on stack. Pushes String to stack.
  OP_NUM,  // Expects String on stack. Pushes Number to stack.
  OP_BOOL, // Expects any type on stack. Pushes Bool to stack.
};

}; // namespace vm