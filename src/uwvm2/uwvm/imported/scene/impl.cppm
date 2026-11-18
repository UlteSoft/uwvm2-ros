/** @file
 * @brief Named-module owner of the concrete checked scene host import tuple.
 * @details The low-level model and deployment owner have no dependency on the
 * runtime or frontend. Only this module binds checked runtime memory/caller
 * APIs and publishes lifecycle/debug hooks. Old ROS providers remain separate.
 */
module;
// Match the model's global allocator/callback attachment across exporters.
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/function_ref.h>
#include <exception>
#include <algorithm>
#include <array>
#include <cerrno>
#include <charconv>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <new>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#if defined(__linux__)
#include <fcntl.h>
#include <linux/memfd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/runtime/lib/uwvm_runtime_wasm_fp_native_control.h>
// Imports cannot carry macros. Read only the concrete operation-list macro;
// descriptor/type declarations belong to uwvm2.imported.ros.scene.
#define UWVM_SCENE_RUNTIME_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/runtime/contract.h>
#undef UWVM_SCENE_RUNTIME_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_ARTIFACT_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/artifact_contract.h>
#undef UWVM_SCENE_ARTIFACT_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_OUTPUT_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/output_contract.h>
#undef UWVM_SCENE_OUTPUT_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_REVOCATION_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/revocation_contract.h>
#undef UWVM_SCENE_REVOCATION_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_MANAGED_LIFECYCLE_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/managed_lifecycle_contract.h>
#undef UWVM_SCENE_MANAGED_LIFECYCLE_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_BUFFER_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/buffer_contract.h>
#undef UWVM_SCENE_BUFFER_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_MODEL_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/model_contract.h>
#undef UWVM_SCENE_MODEL_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_ROBOT_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/robot_provider_contract.h>
#undef UWVM_SCENE_ROBOT_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_SENSOR_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/sensor_contract.h>
#undef UWVM_SCENE_SENSOR_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_PROPOSAL_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/proposal_contract.h>
#undef UWVM_SCENE_PROPOSAL_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_TRANSFORM_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/transform_contract.h>
#undef UWVM_SCENE_TRANSFORM_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_LOG_DIAGNOSTIC_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/log_diagnostic_contract.h>
#undef UWVM_SCENE_LOG_DIAGNOSTIC_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_RECORDING_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/recording_contract.h>
#undef UWVM_SCENE_RECORDING_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_COMMUNICATION_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/communication_contract.h>
#undef UWVM_SCENE_COMMUNICATION_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_COMMUNICATION_RECEIVE_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/communication_receive_contract.h>
#undef UWVM_SCENE_COMMUNICATION_RECEIVE_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_COMMUNICATION_CREATION_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/communication_creation_control_contract.h>
#undef UWVM_SCENE_COMMUNICATION_CREATION_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_PARAMETER_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/parameter_contract.h>
#define UWVM_SCENE_ACTION_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/communication_action_contract.h>
#undef UWVM_SCENE_ACTION_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_GRAPH_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/communication_graph_contract.h>
#undef UWVM_SCENE_GRAPH_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_EVENT_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/event_contract.h>
#undef UWVM_SCENE_EVENT_CONTRACT_MACROS_ONLY
#undef UWVM_SCENE_PARAMETER_CONTRACT_MACROS_ONLY

#define UWVM_SCENE_DRIVER_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/driver_contract.h>
#undef UWVM_SCENE_DRIVER_CONTRACT_MACROS_ONLY

#define UWVM_SCENE_MODULE_EXCHANGE_MACROS_ONLY
#include <uwvm2/imported/ros/scene/module_exchange.h>
#undef UWVM_SCENE_MODULE_EXCHANGE_MACROS_ONLY
#define UWVM_SCENE_PERMISSION_REPORT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/permission_report.h>
#undef UWVM_SCENE_PERMISSION_REPORT_MACROS_ONLY

export module uwvm2.uwvm.imported.scene;
export import uwvm2.imported.ros.scene;
export import uwvm2.uwvm.imported.scene.storage;
import uwvm2.imported.ros;
#if defined(__linux__)
import fast_io_crypto;
#endif
import uwvm2.runtime;
import uwvm2.utils.container;
import uwvm2.utils.control;
import uwvm2.utils.thread;
#if defined(UWVM_VERSION_DEV)
import uwvm2.utils.debug.host_calls;
#endif
import uwvm2.uwvm.wasm.type;
import uwvm2.uwvm.wasm.storage;
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
import uwvm2.uwvm.imported.wasi.wasip1.storage;
#endif
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
import uwvm2.uwvm.imported.ros.policy_state;
#endif
import uwvm2.uwvm.runtime.builtin_ros_loader;
import uwvm2.uwvm.runtime.runtime_mode;
#define UWVM_MODULE
export {
#include "input.h"
#include "permission_observer.h"
#include "module_observer.h"
#include "vm_observer.h"
#include "host.h"
}
