module;
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <span>
#include <utility>
#include <vector>
export module uwvm2.uwvm.debugger.checkpoint_state;
import fast_io;
export import uwvm2.runtime.checkpoint.feature_policy_schema;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
import uwvm2.extension.memtag.tag_image;
import uwvm2.extension.adm.types;
import uwvm2.runtime.checkpoint.memory_layout;
#include "checkpoint_state.h"
