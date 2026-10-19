// SPDX-License-Identifier: Apache-2.0
// Real module owner; no default execution, WASI, ROS or management grant.
module;
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <mutex>
#include <span>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/utils/ansies/uwvm_color_push_macro.h>
#include <uwvm2/runtime/lib/uwvm_runtime_threaded_profile_feature.h>

export module uwvm2.uwvm.cmdline.params:ros;
import fast_io;
import uwvm2.utils.container;
import uwvm2.utils.cmdline;
import uwvm2.uwvm.io;
import uwvm2.imported.ros;
import uwvm2.uwvm.imported.ros.policy_state;

#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
export {
#include "../callback/ros.h"
#include "ros1.h"
#include "ros2.h"
}
