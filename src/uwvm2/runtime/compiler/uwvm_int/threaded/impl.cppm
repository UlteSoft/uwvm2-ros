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
#if defined(__linux__)
#include <pthread.h>
#include <sched.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#endif
#include <uwvm2/runtime/lib/uwvm_runtime_wasm_fp_environment.h>
#include <uwvm2/runtime/compiler/shared/strict_float.h>

export module uwvm2.runtime.compiler.uwvm_int.threaded;
import fast_io;
import uwvm2.utils.container;
import uwvm2.uwvm.runtime.storage;
import uwvm2.uwvm.runtime.initializer;
import uwvm2.runtime.wasm_threads;
import uwvm2.validation.error;
import uwvm2.validation.standard.wasm3.reference_policy;
import uwvm2.validation.standard.wasm3.exception_immediate;
import uwvm2.object.memory.linear;
import uwvm2.parser.wasm.standard.wasm3.type;
import uwvm2.parser.wasm.standard.wasm1p1.features;
import uwvm2.object.global;
import uwvm2.runtime.compiler.shared.wasm_threads;
#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
export { extern "C++" {
#include "contract.h"
#include "program.h"
#include "engine.h"
} }
