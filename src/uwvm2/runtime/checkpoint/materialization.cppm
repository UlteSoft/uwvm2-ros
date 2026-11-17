/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
module;
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <span>
#include <utility>
#include <vector>
#include <atomic>
#include <mutex>
export module uwvm2.runtime.checkpoint.materialization;
import fast_io;
export import uwvm2.runtime.checkpoint.feature_policy_schema;
import uwvm2.parser.wasm.standard.wasm3.type;
import uwvm2.object.global;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "materialization.h"
