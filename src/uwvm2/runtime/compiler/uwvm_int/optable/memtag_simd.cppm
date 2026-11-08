module;
#include <array>
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
export module uwvm2.runtime.compiler.uwvm_int.optable:memtag_simd;
import uwvm2.utils.container;
import uwvm2.runtime.memtag;
import uwvm2.runtime.compiler.shared.wasm1p1_simd;
import :define;
import :memory;
import :memory64_simd;
import :memtag;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "memtag_simd.h"
