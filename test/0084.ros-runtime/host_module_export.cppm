/** @file
 * @brief Isolate new scene host exports from existing infrastructure module blockers.
 * @details The real scene model/configuration modules own all new API types.
 * Existing runtime/type/loader infrastructure is included in the global module
 * fragment with old ROS feature inclusion temporarily suppressed, avoiding a
 * second textual scene configuration. The actual host.h is then exported.
 * This fixture verifies new exported definitions and specializations only;
 * it does not qualify the production module graph or runtime implementation.
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
#include <atomic>
#include <cerrno>
#include <charconv>
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
#define UWVM_SCENE_TRANSFORM_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/transform_contract.h>
#undef UWVM_SCENE_TRANSFORM_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_LOG_DIAGNOSTIC_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/log_diagnostic_contract.h>
#undef UWVM_SCENE_LOG_DIAGNOSTIC_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_RECORDING_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/recording_contract.h>
#undef UWVM_SCENE_RECORDING_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_SENSOR_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/sensor_contract.h>
#undef UWVM_SCENE_SENSOR_CONTRACT_MACROS_ONLY
#define UWVM_SCENE_PROPOSAL_CONTRACT_MACROS_ONLY
#include <uwvm2/imported/ros/scene/proposal_contract.h>
#undef UWVM_SCENE_PROPOSAL_CONTRACT_MACROS_ONLY
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
#pragma push_macro("UWVM_IMPORT_ROS1")
#pragma push_macro("UWVM_IMPORT_ROS2")
#pragma push_macro("UWVM_IMPORT_ROSV2")
#undef UWVM_IMPORT_ROS1
#undef UWVM_IMPORT_ROS2
#undef UWVM_IMPORT_ROSV2
#include <uwvm2/imported/ros/common/synchronous_wait.h>
#include <uwvm2/runtime/lib/uwvm_runtime.h>
#include <uwvm2/uwvm/wasm/type/impl.h>
#include <uwvm2/uwvm/runtime/storage/builtin_ros_loader.h>
#include <uwvm2/utils/control/debug_permissions.h>
#include <uwvm2/utils/control/host_interfaces.h>
#include <uwvm2/uwvm/wasm/storage/all_module.h>
#include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
#include <uwvm2/uwvm/imported/wasi/wasip1/storage/env.h>
#endif
#pragma pop_macro("UWVM_IMPORT_ROSV2")
#pragma pop_macro("UWVM_IMPORT_ROS2")
#pragma pop_macro("UWVM_IMPORT_ROS1")
export module uwvm2.uwvm.imported.scene;
export import uwvm2.imported.ros.scene;
export import uwvm2.uwvm.imported.scene.storage;
#if defined(__linux__)
import fast_io_crypto;
#endif
#define UWVM_MODULE
export {
#include "uwvm2/uwvm/imported/scene/input.h"
#include "uwvm2/uwvm/imported/scene/permission_observer.h"
#include "uwvm2/uwvm/imported/scene/module_observer.h"
#include "uwvm2/uwvm/imported/scene/vm_observer.h"
#include "uwvm2/uwvm/imported/scene/host.h"
}
