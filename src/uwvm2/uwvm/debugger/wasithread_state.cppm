module;
#include <uwvm2/utils/control/wasithread.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <array>
export module uwvm2.uwvm.debugger.wasithread_state;
import fast_io;
#define UWVM_MODULE
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT export
#include "wasithread_state.h"
#undef UWVM_MODULE_EXPORT
