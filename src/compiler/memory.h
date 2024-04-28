#pragma once

#include "../analysis/scope.h"
#include "../analysis/types.h"
#include "../bedrock.h"

namespace compiler {
/// @brief Given a type will determine the size in bytes required for storage.
/// will return number of padded bytes required to store/manage this type in the
/// vm.
size_t get_alloc_size(shared_ptr<analysis::Type>);

/// @brief Most types are require no runtime introspection and can be stored on
/// the stack. However some types will require runtime introspection and this
/// function will return whether the size can be determined at comptime.
bool is_fixed_size(shared_ptr<analysis::Scope>, std::string);
}  // namespace compiler