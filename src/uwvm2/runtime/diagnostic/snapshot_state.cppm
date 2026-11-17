module;
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>
#include <fast_io.h>
#include <fast_io_crypto.h>
export module uwvm2.runtime.diagnostic.snapshot_state;
import uwvm2.runtime.diagnostic.snapshot_archive;
import uwvm2.runtime.diagnostic.snapshot_wasip1;
import uwvm2.runtime.diagnostic.snapshot_ros;
import uwvm2.uwvm.debugger.checkpoint_codec;
import uwvm2.uwvm.debugger.checkpoint_execution_replay;
import uwvm2.runtime.checkpoint.materialization;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "snapshot_state.h"
#undef UWVM_MODULE_EXPORT
