// Licensed under APL-2.0 (see LICENSE).
module;
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <span>
#include <utility>
#include <vector>
export module uwvm2.uwvm.debugger.checkpoint_replay_memory;
import fast_io;
import fast_io_crypto;
import uwvm2.uwvm.debugger.checkpoint_replay;
import uwvm2.uwvm.debugger.checkpoint_state;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "checkpoint_replay_memory.h"
