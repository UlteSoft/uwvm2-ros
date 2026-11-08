module;
#include <atomic>
#include <array>
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.imported.wasi.wasip1.memory:checked_access;
import uwvm2.runtime.memtag.host_fault;
import uwvm2.runtime.memtag.native_pointer;
import uwvm2.extension.memtag.instruction;
import uwvm2.extension.memtag.shadow;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "checked_access.h"
