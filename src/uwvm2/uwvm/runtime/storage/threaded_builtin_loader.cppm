// SPDX-License-Identifier: Apache-2.0
// Sole named-module owner; import issues no authority or native effects.
module;
#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>

export module uwvm2.uwvm.runtime.threaded_builtin_loader;
import uwvm2.uwvm.wasm.type;
import uwvm2.uwvm.wasm.storage;
import uwvm2.uwvm.imported.wasi.threads;
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
import uwvm2.uwvm.imported.ros;
#endif

#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT export
export {
#include "threaded_builtin_loader.h"
}
