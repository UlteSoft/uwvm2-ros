// SPDX-License-Identifier: Apache-2.0
module;
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
export module uwvm2.uwvm.runtime.builtin_ros_loader;
import fast_io;
import fast_io_crypto;
import uwvm2.uwvm.wasm.type;
import uwvm2.uwvm.wasm.storage;
#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT export
#include "builtin_ros_loader.h"
