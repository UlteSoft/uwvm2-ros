module;
#include <uwvm2/utils/macro/push_macros.h>
#include <atomic>
#include <cstddef>
export module uwvm2.runtime.memtag.native_pointer;
import uwvm2.extension.memtag.hardware_mte;
import uwvm2.extension.memtag.shadow;
import uwvm2.runtime.memtag.hardware_thread;
#if defined(__linux__) && defined(__aarch64__) && defined(UWVM_SUPPORT_MMAP)
import uwvm2.object.memory.signal;
#endif
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "native_pointer.h"
