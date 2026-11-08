module;
#include <uwvm2/utils/macro/push_macros.h>
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <utility>
#if defined(UWVM_SUPPORT_MMAP) && !(defined(_WIN32) || defined(__CYGWIN__))
# include <sys/mman.h>
#endif
export module uwvm2.extension.memtag.storage;
import fast_io;
import uwvm2.utils.mutex;
import uwvm2.object.memory.platform_page;
import uwvm2.extension.memtag.instruction;
import uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.hardware_mte;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "storage.h"
