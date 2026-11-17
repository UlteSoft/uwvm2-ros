module;
#include <uwvm2/utils/container/standard.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <set>
#include <span>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
export module uwvm2.runtime.diagnostic.snapshot_ros;
import uwvm2.runtime.diagnostic.snapshot_archive;
import uwvm2.imported.ros;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "snapshot_ros.h"
#undef UWVM_MODULE_EXPORT
