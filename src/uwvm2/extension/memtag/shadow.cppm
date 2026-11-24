module;
#include <atomic>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <type_traits>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.hardware_mte;
import uwvm2.extension.memtag.software_x86;
import uwvm2.utils.mutex;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "shadow.h"
