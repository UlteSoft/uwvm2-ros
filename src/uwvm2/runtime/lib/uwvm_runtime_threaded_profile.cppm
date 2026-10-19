// SPDX-License-Identifier: Apache-2.0
// Canonical named-module owner. No policy, grant, worker or Source is created by import.
module;
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cerrno>
#include <chrono>
#include <climits>
#include <cmath>
#include <condition_variable>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <span>
#include <stop_token>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>

export module uwvm2.runtime:threaded_profile;
export import uwvm2.runtime.wasm_threads;
import uwvm2.uwvm.runtime.storage;
import uwvm2.utils.container;
#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
export { extern "C++" {
#include "uwvm_runtime_threaded_profile_api.h"
} }
