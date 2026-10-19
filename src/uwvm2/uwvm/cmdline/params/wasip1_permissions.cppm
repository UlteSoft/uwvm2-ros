// WASI explicit authority partition. SPDX-License-Identifier: Apache-2.0
module;
#include <memory>
#include <uwvm2/utils/macro/push_macros.h>
#ifndef UWVM_DISABLE_LOCAL_IMPORTED_WASIP1
# include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
#endif
export module uwvm2.uwvm.cmdline.params:wasip1_permissions;
import uwvm2.utils.container;
import uwvm2.utils.cmdline;
import uwvm2.uwvm.imported.wasi.wasip1.storage;
#ifndef UWVM_MODULE
# define UWVM_MODULE
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT export
#endif
#include "wasip1_permissions.h"
