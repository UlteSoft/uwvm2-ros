// SPDX-License-Identifier: Apache-2.0
// Sole named-module owner; import issues no authority or native effects.
module;
#include <cstdint>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>

export module uwvm2.uwvm.imported.wasi.threads;
import uwvm2.utils.container;
import uwvm2.uwvm.wasm.type;

#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT export
export {
#include "local_imported.h"
}
