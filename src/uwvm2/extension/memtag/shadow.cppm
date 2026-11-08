module;
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.hardware_mte;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "shadow.h"
