/** @file
 * @brief Verify the concrete scene wrapper's closed floating-point whitelist.
 * @details LLVM builds replace the complete outer guard only when the audited
 * local wrapper constructs the existing native register-control guard before
 * any callback/identity/allocation work. Allocator or memory callbacks may
 * modify FP controls even on nominally integer-only paths. Interpreter-only
 * and unsupported-native-guard builds retain the complete outer guard.
 * ABI, capability administration, two-phase publication
 * and a fresh callback always keep the complete guard.
 * Compile with UWVM_SCENE_TEST_MODULE to check exported specializations too,
 * using separately compiled modules for each backend configuration.
 */
#include <cstdint>
#include <type_traits>
#include <utility>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#ifdef UWVM_SCENE_TEST_EXPECT_LLVM
# if UWVM_SCENE_TEST_EXPECT_LLVM
#  if !defined(UWVM_RUNTIME_LLVM_JIT)
#   error "Expected the LLVM callback-policy configuration"
#  endif
# elif defined(UWVM_RUNTIME_LLVM_JIT)
#  error "Expected the interpreter-only callback-policy configuration"
# endif
#endif
#ifdef UWVM_SCENE_TEST_MODULE
import uwvm2.uwvm.imported.scene;
#else
#include <uwvm2/uwvm/imported/scene/host.h>
#endif

namespace host=uwvm2::uwvm::imported::scene;
namespace contract=uwvm2::imported::ros::runtime_contract;
namespace artifact=uwvm2::imported::ros::artifact_contract;
namespace output=uwvm2::imported::ros::output_contract;
namespace buffer=uwvm2::imported::ros::buffer_contract;
namespace managed=uwvm2::imported::ros::managed_lifecycle_contract;
namespace models=uwvm2::imported::ros::model_contract;
namespace messages=uwvm2::imported::ros::scene::module_exchange;
namespace robot=uwvm2::imported::ros::scene::robot_contract;
using policy=std::remove_cv_t<decltype(host::wrapper::native<&host::abi,host::wrapper::abi_name>::wasm_fp_control_policy)>;
#if defined(UWVM_RUNTIME_LLVM_JIT)
inline constexpr policy local_policy=host::wrapper::native_control_guard_available?policy::preserves_wasm_control:policy::may_modify;
#else
inline constexpr policy local_policy=policy::may_modify;
#endif
inline constexpr char8_t fresh_name[]=u8"fresh.callback";
template<std::size_t Index> consteval bool robot_keeps_complete_guard()
{
    constexpr auto action=static_cast<robot::operation>(Index);
    if constexpr(Index<10)return host::wrapper::native<&host::invoke_device<action>,fresh_name>::wasm_fp_control_policy==policy::may_modify;
    else if constexpr(Index<14)return host::wrapper::native<&host::invoke_media<action>,fresh_name>::wasm_fp_control_policy==policy::may_modify;
    else if constexpr(Index<18)return host::wrapper::native<&host::invoke_bridge<action>,fresh_name>::wasm_fp_control_policy==policy::may_modify;
    else return false; // Retired component operations have no current import.
}
template<std::size_t... Index> consteval bool all_robot_guards(std::index_sequence<Index...>)
{return (robot_keeps_complete_guard<Index>()&&...);}
static_assert(robot::operations.size()==22);
static_assert(all_robot_guards(std::make_index_sequence<18>{}));
namespace drivers=uwvm2::imported::ros::scene::drivers;
template<std::size_t... Index> consteval bool all_driver_guards(std::index_sequence<Index...>)
#if defined(UWVM_ROS_ATTACHMENT_OPENSSL)
{return ((host::wrapper::native<&host::invoke_driver<static_cast<drivers::operation>(Index)>,fresh_name>::wasm_fp_control_policy==policy::may_modify)&&...);}
#else
{return ((host::wrapper::native<&host::invoke_driver<static_cast<drivers::operation>(Index)>,fresh_name>::wasm_fp_control_policy==local_policy)&&...);}
#endif
static_assert(drivers::names.size()==5);
static_assert(all_driver_guards(std::make_index_sequence<drivers::names.size()>{}));
std::uint32_t fresh_callback() noexcept{return 0;}
template<std::size_t... Index> consteval bool all_local(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke<static_cast<contract::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        (static_cast<contract::operation>(Index)==contract::operation::resource_close?policy::may_modify:local_policy))&&...);
}
static_assert(contract::operations.size()==16);
static_assert(all_local(std::make_index_sequence<contract::operations.size()>{}));
static_assert(host::wrapper::native<&host::abi,host::wrapper::abi_name>::wasm_fp_control_policy==policy::may_modify);
static_assert(host::wrapper::native<&host::open,host::wrapper::open_name>::wasm_fp_control_policy==policy::may_modify);
static_assert(host::wrapper::native<&host::close,host::wrapper::close_name>::wasm_fp_control_policy==policy::may_modify);
static_assert(host::wrapper::native<&host::invoke_revocation,host::wrapper::capability_revoke_name>::wasm_fp_control_policy==policy::may_modify);
static_assert(host::wrapper::native<&host::invoke_artifact<artifact::operation::open>,host::wrapper::artifact_open_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_artifact<artifact::operation::read>,host::wrapper::artifact_read_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_artifact<artifact::operation::describe>,host::wrapper::artifact_describe_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::begin>,host::wrapper::artifact_begin_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::append>,host::wrapper::artifact_append_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::commit>,host::wrapper::artifact_commit_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::abort>,host::wrapper::artifact_abort_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::remove>,host::wrapper::artifact_remove_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::status>,host::wrapper::artifact_status_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::receipt>,host::wrapper::artifact_receipt_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::publish>,host::wrapper::artifact_publish_name>::wasm_fp_control_policy==policy::may_modify);
template<std::size_t... Index> consteval bool all_buffers(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_buffer<static_cast<buffer::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(buffer::operations.size()==5);
static_assert(all_buffers(std::make_index_sequence<buffer::operations.size()>{}));
template<std::size_t... Index> consteval bool all_managed(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_managed<static_cast<managed::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(managed::operations.size()==6);
static_assert(all_managed(std::make_index_sequence<managed::operations.size()>{}));
template<std::size_t... Index> consteval bool all_models(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_model<static_cast<models::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(models::operations.size()==7);
static_assert(all_models(std::make_index_sequence<models::operations.size()>{}));
namespace actions=uwvm2::imported::ros::action_contract;
template<std::size_t... Index> consteval bool all_actions(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_communication<static_cast<actions::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(actions::operations.size()==7);
static_assert(all_actions(std::make_index_sequence<actions::operations.size()>{}));
namespace recording=uwvm2::imported::ros::recording_contract;
namespace sensor=uwvm2::imported::ros::sensor_contract;
namespace proposal=uwvm2::imported::ros::proposal_contract;
template<std::size_t... Index> consteval bool all_recordings(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_recording<static_cast<recording::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
template<std::size_t... Index> consteval bool all_sensors(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_sensor<static_cast<sensor::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
template<std::size_t... Index> consteval bool all_proposals(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_proposal<static_cast<proposal::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(recording::operations.size()==7&&sensor::operations.size()==3&&proposal::operations.size()==4);
static_assert(all_recordings(std::make_index_sequence<recording::operations.size()>{}));
static_assert(all_sensors(std::make_index_sequence<sensor::operations.size()>{}));
static_assert(all_proposals(std::make_index_sequence<proposal::operations.size()>{}));
namespace communication=uwvm2::imported::ros::communication_contract;
namespace receiving=uwvm2::imported::ros::communication_receive_contract;
template<class Operation,std::size_t... Index> consteval bool all_communication(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_communication<static_cast<Operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(all_communication<communication::operation>(std::make_index_sequence<communication::operations.size()>{}));
static_assert(all_communication<receiving::operation>(std::make_index_sequence<receiving::operations.size()>{}));
namespace graphing=uwvm2::imported::ros::graph_contract;namespace streaming=uwvm2::imported::ros::event_contract;
static_assert(all_communication<graphing::operation>(std::make_index_sequence<graphing::operations.size()>{}));
static_assert(all_communication<streaming::operation>(std::make_index_sequence<streaming::operations.size()>{}));
namespace parameters=uwvm2::imported::ros::parameter_contract;
template<std::size_t... Index> consteval bool all_parameters(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_parameter<static_cast<parameters::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        policy::may_modify)&&...);
}
static_assert(all_parameters(std::make_index_sequence<parameters::operations.size()>{}));
static_assert(host::wrapper::native<&fresh_callback,fresh_name>::wasm_fp_control_policy==policy::may_modify);
template<std::size_t... Index> consteval bool all_messages(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_module_call<static_cast<messages::operation>(Index)>,fresh_name>::wasm_fp_control_policy==
        local_policy)&&...);
}
static_assert(all_messages(std::make_index_sequence<messages::names.size()>{}));
namespace reports=uwvm2::imported::ros::scene::permission_report;
static_assert(host::wrapper::native<&host::invoke_permission_report,fresh_name>::wasm_fp_control_policy==local_policy);
static_assert(host::wrapper::native<&host::invoke_module_permission_report,fresh_name>::wasm_fp_control_policy==local_policy);
template<std::size_t... Index> consteval bool all_observations(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_module_observation<static_cast<reports::module_operation>(Index)>,fresh_name>::wasm_fp_control_policy==local_policy)&&...);
}
static_assert(all_observations(std::make_index_sequence<reports::module_operations.size()>{}));
static_assert(host::wrapper::native<&host::invoke_module_observation<reports::module_operation::describe,true>,fresh_name>::wasm_fp_control_policy==local_policy);
static_assert(!host::wrapper::native<&fresh_callback,fresh_name>::guards_fp_controls);
static_assert(std::is_same_v<host::wrapper::native<&fresh_callback,fresh_name>::fp_control_guard,
    host::wrapper::no_local_fp_control_guard>);
static_assert(host::wrapper::native<&host::invoke_output<output::operation::begin>,host::wrapper::artifact_begin_name>::guards_fp_controls==
    (local_policy==policy::preserves_wasm_control));

namespace transforms=uwvm2::imported::ros::transform_contract;
template<std::size_t... Index> consteval bool all_transform(std::index_sequence<Index...>)
{
    return ((host::wrapper::native<&host::invoke_transform<static_cast<transforms::operation>(Index)>,fresh_name>::wasm_fp_control_policy==policy::may_modify)&&...);
}
static_assert(transforms::operations.size()==9);
static_assert(all_transform(std::make_index_sequence<transforms::operations.size()>{}));
namespace log_diagnostic=uwvm2::imported::ros::log_diagnostic_contract;
static_assert(host::wrapper::native<&host::invoke_log_diagnostic<log_diagnostic::operation::emit>,host::wrapper::log_emit_name>::wasm_fp_control_policy==policy::may_modify);
static_assert(host::wrapper::native<&host::invoke_log_diagnostic<log_diagnostic::operation::read>,host::wrapper::diagnostic_read_name>::wasm_fp_control_policy==policy::may_modify);
