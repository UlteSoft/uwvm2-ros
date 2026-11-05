/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
module;
#include <uwvm2/utils/macro/push_macros.h>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>
#include <span>
#include <string>
#include <string_view>
#include <utility>
export module uwvm2.uwvm.debugger:native_decoder_cache;
import :native_target_metadata;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "native_decoder_cache.h"
