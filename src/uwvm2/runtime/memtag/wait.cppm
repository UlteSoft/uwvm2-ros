module;
#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.runtime.memtag.wait;
import uwvm2.runtime.memtag.bridge;
import uwvm2.runtime.wasm_threads;
import uwvm2.runtime.compiler.shared.wasm_threads;
import uwvm2.extension.memtag.shadow;
import uwvm2.utils.thread;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "wait.h"
