module;
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>
#include <uwvm2/runtime/checkpoint/host_memory_phase.h>
#include <uwvm2/utils/macro/push_macros.h>
export module uwvm2.imported.wasi.wasip1.func:checkpoint_io;
import fast_io;
import uwvm2.imported.wasi.wasip1.abi;
import uwvm2.imported.wasi.wasip1.memory;
#ifndef UWVM_MODULE
# define UWVM_MODULE
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT export
#endif
#include "checkpoint_io.h"
#include <uwvm2/utils/macro/pop_macros.h>
