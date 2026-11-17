// Independent I1T policy; importing this partition grants no authority.
module;
#include <array>
#include <memory>
#include <string_view>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/utils/control/wasithread_diagnostics.h>
#include <uwvm2/utils/control/wasithread_system.h>
export module uwvm2.uwvm.cmdline.params:wasithread;
import fast_io;
import uwvm2.utils.container;
import uwvm2.utils.cmdline;
import uwvm2.uwvm.io;
#ifndef UWVM_MODULE
#define UWVM_MODULE
#endif
#undef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
export {
#include "../callback/wasithread.h"
#include "wasithread.h"
}
