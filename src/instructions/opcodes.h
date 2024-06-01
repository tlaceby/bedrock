#pragma once
#include "../bedrock.h"

/// ------------------------------------
/// ------------------------------------
/// fp: Frame Pointer
///   - Points to the current frame/code block we are operating in. Will represent the first address of a scope/block.
/// sp: Stack Pointer
///   - Points to stack offset which represents the current stack top.
/// ip: Instruction Pointer
///   - Points to the instruction which is currently being executed inside the .code section
/// ------------------------------------
// 0 - 30 Common VM Operations w/ Memory
/// ------------------------------------

/// @brief Halts program execution and exit's gracefuly.
const uint8_t op_hlt = 0;
/// @brief Pops `n` elements off the stack eg: `pop 2` will pop 2 Values off the stack.
const uint8_t op_pop = 1;
/// @brief Pushes a Value from the constant pool onto the stack at a .data address offset eg: `push 0x08` Means push the
/// value at the data address of 0x08 onto the stack.
const uint8_t op_push = 2;

/// @brief Incriment SP by `n` positions. Used to setup local variables for blocks/fns
const uint8_t op_incsp = 3;
/// @brief Decrement SP by `n` positions. Used to cleanup local variables for blocks/fns
const uint8_t op_decsp = 4;

/// @brief Pushes a value in .globals onto the stack.
const uint8_t op_loadg = 5;
/// @brief Pops value off the stack and stores at .global offset.
const uint8_t op_storeg = 6;

/// @brief Pushes a value at bp address offset onto the stack.
const uint8_t op_loadl = 7;
/// @brief Pops a value off the stack and stores at local offset from BasePointer.
const uint8_t op_storel = 8;

/// @brief Accepts a chunk_address to jump to. Will setup stack with proper return address and fp.
const uint8_t op_call = 9;
/// @brief Cleans up and ensures a return value is on stack if required.
const uint8_t op_ret = 10;
/// ------------------------------------
// 30 - 45 Binary & Unary Operations
/// ------------------------------------

/// @brief Expects 2 strings on the stack. Pops both and pushes the combined string.
const uint8_t op_concat = 30;
/// @brief Expects 2 numbers on the stack. Pops both and pushes the sum.
const uint8_t op_add = 31;
/// @brief Expects 2 numbers on the stack. Pops both and pushes the result of the subtraction.
const uint8_t op_sub = 32;
/// @brief Expects 2 numbers on the stack. Pops both and pushes the product.
const uint8_t op_mul = 33;
/// @brief Expects 2 numbers on the stack. Pops both and pushes the division result.
const uint8_t op_div = 34;
/// @brief Expects 2 numbers on the stack. Pops both and pushes the remainder
const uint8_t op_mod = 35;

/// @brief Pops number off stack and pushes -Number back.
const uint8_t op_inverse = 36;

/// @brief Expects a bool on the stack and places it's inverse back onto the stack.
const uint8_t op_not = 37;

/// -------------------------------------
// 46 - 60 Comparison & Logical Operators
/// -------------------------------------

/// @brief Expects 2 bools on the stack. Pops both and pushes a bool which represents whether they are equal.
const uint8_t op_cmp = 46;
/// @brief Expects 2 numbers on the stack. Pops both and pushes a bool which represents whether they are equal.
const uint8_t op_numcmp = 47;
/// @brief Expects 2 strings on the stack. Pops both and pushes a bool which represents whether they are equal.
const uint8_t op_strcmp = 48;

/// @brief Expects 2 numbers on the stack. Pops both and pushes a bool which represents whether the left is less than
/// the right
const uint8_t op_lt = 49;
/// @brief Expects 2 numbers on the stack. Pops both and pushes a bool which represents whether the left is less than
/// or equal to the right
const uint8_t op_lteq = 50;

/// @brief Expects 2 numbers on the stack. Pops both and pushes a bool which represents whether the left is greater than
/// the right
const uint8_t op_gt = 51;
/// @brief Expects 2 numbers on the stack. Pops both and pushes a bool which represents whether the left is greater than
/// or equal to the right
const uint8_t op_gteq = 52;

/// @brief Expects 2 bools on the stack. Pops both and pushes a bool which represents whether they are both true.
const uint8_t op_and = 53;
/// @brief Expects 2 bools on the stack. Pops both and pushes a bool which represents whether atleast one is true.
const uint8_t op_or = 54;

/// @brief Jumps to a instruction address
const uint8_t op_jmp = 55;
/// @brief Jumps to a instruction address if boolean on stack is true. Pops value off stack.
const uint8_t op_cjmp = 56;

/// -----------------
// 150 - 255 Macros
/// -----------------

/// @brief expects a string on the stack and will log it to stdout.
const uint8_t op_log = 150;
/// @brief Pops a number off the stack and pushes a string representation of it.
const uint8_t op_numstr = 151;
/// @brief Pops a string off the stack. Attempts to push a number representation of it onto the stack.
const uint8_t op_strnum = 152;