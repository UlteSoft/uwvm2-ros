module;
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <uwvm2/utils/allocator/resource_limit.h>
#include <uwvm2/utils/container/standard.h>

export module uwvm2.uwvm.wasm.storage:input_policy;
import fast_io;
import uwvm2.utils.control;
import uwvm2.utils.utf;
import uwvm2.utils.container;
import uwvm2.uwvm.wasm.feature;
import uwvm2.uwvm.wasm.type;
import :module_policy;

#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "input_policy.h"
