module;
#include <array>
#include <initializer_list>
#include <utility>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <optional>
#include <string_view>
export module uwvm2.uwvm.wasm.storage:module_policy;
import fast_io;
import uwvm2.uwvm.runtime.runtime_mode;
import uwvm2.parser.wasm.standard.wasm1.features;
import uwvm2.parser.wasm_custom.customs;
import uwvm2.extension.memtag.execution_policy;
import uwvm2.utils.container;
import uwvm2.parser.wasm.standard.wasm1p1.features;
import uwvm2.uwvm.wasm.feature;
import uwvm2.uwvm.wasm.type;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "module_policy.h"
