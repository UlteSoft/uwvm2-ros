module;
#include <array>
#include <atomic>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <limits>
#include <type_traits>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
export module uwvm2.runtime.compiler.uwvm_int.optable:memtag;
import fast_io;
import uwvm2.utils.container;
import uwvm2.object.memory.linear;
import uwvm2.extension.memtag.instruction;
import uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.operations;
import uwvm2.runtime.memtag;
import :define;
import :storage;
import :memory;
import :memory64;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "memtag.h"
