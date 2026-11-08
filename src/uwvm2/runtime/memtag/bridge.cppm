module;
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <type_traits>
#include <utility>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.runtime.memtag.bridge;
import uwvm2.object.memory.linear;
import uwvm2.extension.memtag.operations;
import uwvm2.extension.memtag.instruction;
import uwvm2.extension.memtag.shadow;
import uwvm2.extension.memtag.storage;
import uwvm2.runtime.memtag.random;
import uwvm2.runtime.memtag.native_pointer;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "bridge.h"
