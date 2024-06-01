#pragma once
#include "../bedrock.h"
#include "values.h"

namespace runtime {
const size_t DEFAULT_STACK_SIZE = 16384;

enum OS {
  Win,
  Darwin,
  Other,
};

class vm {

private:
  Val *globals;
  Val *data;
  Val *stack;

  Val *sp, fp;
  u_int16_t *ip;

  size_t STACK_SIZE;      // maximum stack size
  size_t ALLOCATED_BYTES; // Number of bytes currently allocated on heap
};
}; // namespace runtime