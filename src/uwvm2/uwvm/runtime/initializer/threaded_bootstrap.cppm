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

export module uwvm2.uwvm.runtime.initializer:threaded_bootstrap;
import uwvm2.runtime.wasm_threads;
import :threaded_child_world;
import fast_io;
import uwvm2.runtime.gc.entry_admission;
import uwvm2.runtime.gc.instance_phase;
import uwvm2.utils.container;
import uwvm2.utils.debug;
import uwvm2.parser.wasm.base;
import uwvm2.parser.wasm.concepts;
import uwvm2.parser.wasm.standard.wasm1.type;
import uwvm2.parser.wasm.standard.wasm1.const_expr;
import uwvm2.parser.wasm.standard.wasm1.opcode;
import uwvm2.parser.wasm.standard.wasm1.features;
import uwvm2.parser.wasm.standard.wasm1p1.type;
import uwvm2.parser.wasm.standard.wasm1p1.features;
import uwvm2.parser.wasm.standard.wasm2.features;
import uwvm2.parser.wasm.standard.wasm3.type;
import uwvm2.validation.standard.wasm3.constant_expression;
import uwvm2.validation.standard.wasm3.reference_policy;
import uwvm2.validation.standard.wasm3.value_immediate;
import uwvm2.validation.standard.wasm3.declaration_policy;
import uwvm2.parser.wasm.binfmt.binfmt_ver1;
import uwvm2.object;
import uwvm2.uwvm.io;
import uwvm2.uwvm.utils.ansies;
import uwvm2.uwvm.imported.wasi.wasip1.storage;
import uwvm2.uwvm.wasm;
import uwvm2.uwvm.runtime.storage;
#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
export { extern "C++" {
#include "threaded_bootstrap.h"
#include "threaded_main_active.h"
} }
