module;
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>
export module uwvm2.uwvm.debugger:wasm_events;
import fast_io;
import uwvm2.extension.memtag.instruction;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "wasm_events.h"
