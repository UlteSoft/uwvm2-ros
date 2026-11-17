module;
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/container/standard.h>
#include <limits>
#include <array>
#include <memory>
#include <mutex>
#include <new>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>
export module uwvm2.uwvm.debugger:source_scope_path;
import :source_dwarf_types;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "source_scope_path.h"
