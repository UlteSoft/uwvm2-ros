module;
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <type_traits>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
export module uwvm2.runtime.compiler.uwvm_int.optable:memtag_bulk;
import uwvm2.uwvm.runtime.storage;
import uwvm2.runtime.wasm_threads;
import uwvm2.runtime.memtag.bridge;
import uwvm2.extension.memtag.shadow;
import uwvm2.utils.container;
import :define;
import :memory;
import :memtag;
import fast_io;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "memtag_bulk.h"
