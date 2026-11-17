module;
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <cstdint>
#include <vector>
export module uwvm2.uwvm.debugger.checkpoint_execution_replay;
import uwvm2.uwvm.debugger.checkpoint_replay_memory;
import uwvm2.uwvm.debugger.checkpoint_replay;
import uwvm2.uwvm.debugger.checkpoint_state;
import uwvm2.runtime.diagnostic.snapshot_wasip1;
import uwvm2.runtime.diagnostic.snapshot_ros;
import uwvm2.runtime.diagnostic.snapshot_archive;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "checkpoint_execution_replay.h"
#undef UWVM_MODULE_EXPORT
#undef UWVM_MODULE
