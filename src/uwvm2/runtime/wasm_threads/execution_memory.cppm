module;
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <span>
export module uwvm2.runtime.wasm_threads:execution_memory;
import uwvm2.extension.memtag;
#define UWVM_MODULE
export { extern "C++" {
#include "execution_memory.h"
} }
