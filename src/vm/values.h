#pragma once
#include "../bedrock.h"

namespace runtime {
enum ValueKind {
  Number,
  Boolean,
  Heap,
};

enum HeapKind {
  String,
  Slice,
  Struct,
};

struct HeapVal;
struct Val {
  ValueKind kind;

  union data {
    double number;
    bool boolean;
    HeapVal *heap;
  } data;

  Val(double d) {
    kind = Number;
    data.number = d;
  }

  Val(bool b) {
    kind = Boolean;
    data.boolean = b;
  }
};

struct StructVal {
  size_t id;
  Val *fields;
  size_t count;
};

struct SliceVal {
  Val *data;
  size_t capacity;
};

struct StringVal {
  char *data;
  size_t length;
};

struct HeapVal {
  HeapKind kind;

  union data {
    StringVal string;
    StructVal structval;
    SliceVal slice;
  };
};

void logRuntimeValue(Val val);
}; // namespace runtime
