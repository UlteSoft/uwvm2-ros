// PRIVATE observation-only adapter; no production import or native admission.
module;
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <cstddef>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <utility>
#include <vector>

export module uwvm2.runtime.compiler.shared.wasm_exception_private_leaf_effect;

#ifndef UWVM_MODULE
# define UWVM_MODULE
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT export
#endif
#include "wasm_exception_private_leaf_effect.h"
