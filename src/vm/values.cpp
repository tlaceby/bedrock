#include "values.h"
using namespace runtime;

void runtime::logRuntimeValue(Val val) {
  switch (val.kind) {
  case Number:
    std::cout << val.data.number;
    break;
  case Boolean:
    std::cout << val.data.boolean ? "true" : "false";
  }
}