module;
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>
#include <utility>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
export module uwvm2.runtime.compiler.uwvm_int.optable:memtag_threads;
import uwvm2.validation.standard.wasm3.atomic_immediate;
import uwvm2.runtime.memtag.wait;
import uwvm2.runtime.memtag.bridge;
import uwvm2.runtime.wasm_threads;
import uwvm2.runtime.compiler.shared.wasm_threads;
import uwvm2.extension.memtag.shadow;
import :define;
import :storage;
import :memory;
import :memtag;
import :threads;
import fast_io;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "memtag_threads.h"
