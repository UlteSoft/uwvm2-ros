module;
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <type_traits>
#include <utility>
export module uwvm2.runtime.memtag.host_access;
import uwvm2.runtime.memtag.bridge;
import uwvm2.extension.memtag.instruction;
import uwvm2.extension.memtag.shadow;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "host_access.h"
