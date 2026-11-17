module;
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <atomic>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <span>
#include <utility>
#include <vector>
export module uwvm2.uwvm.debugger.checkpoint_replay;
import uwvm2.uwvm.debugger.checkpoint_state;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "checkpoint_replay.h"
