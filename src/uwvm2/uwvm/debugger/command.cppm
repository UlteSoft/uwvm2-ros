module;
#include <uwvm2/utils/container/standard.h>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <string_view>
#include <string>
#include <utility>
export module uwvm2.uwvm.debugger:command;
import fast_io;
import uwvm2.uwvm.debugger.source_pretty_registry;
import :console_help_catalog;
import uwvm2.utils.control;
import :wasm_events;
import uwvm2.uwvm.debugger.wasm_state;
import uwvm2.uwvm.debugger.wasm_mutation;
import uwvm2.uwvm.debugger.wasm_path;
import uwvm2.uwvm.debugger.wasip1_state;
import uwvm2.uwvm.debugger.ros_state;
import uwvm2.uwvm.debugger.scene_state;
import uwvm2.uwvm.debugger.wasithread_state;
import uwvm2.uwvm.debugger.host_trace;
#define UWVM_MODULE
#define UWVM_MODULE_EXPORT export
#include "command.h"
