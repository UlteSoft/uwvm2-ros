# Actual production compile AND link gates for the selected original ROS2 SDK.
# No try_run, fake generated struct, version-label enable, or caller boolean.
# Root must later invoke this only in its owned bounded compiler lane and retain
# the actual CMake compiler/link source/log/full MD and selected SDK identities.
include_guard(GLOBAL)
include(CheckCXXSourceCompiles)
function(uwvm_ros2_probe_native_rcl_service output_directory)
  if(NOT IS_ABSOLUTE "${output_directory}")
    message(FATAL_ERROR "Native RCL generated feature directory must be absolute")
  endif()
  foreach(_target rcl::rcl rclcpp::rclcpp rmw::rmw std_srvs::std_srvs__rosidl_generator_cpp)
    if(NOT TARGET "${_target}")
      message(FATAL_ERROR "Actual selected SDK target missing: ${_target}")
    endif()
  endforeach()
  # Function scope preserves its caller's required-check variables. Remove both
  # ordinary and cache results: a previous SDK/externally passed -D=1 is not a
  # fresh actual compile/link observation for this selected source closure.
  set(CMAKE_REQUIRED_LIBRARIES rcl::rcl rclcpp::rclcpp rmw::rmw std_srvs::std_srvs__rosidl_generator_cpp)
  unset(UWVM_NATIVE_RCL_SERVICE_API)
  unset(UWVM_NATIVE_RCL_SERVICE_API CACHE)
  check_cxx_source_compiles([=[
#include <rcl/client.h>
#include <rcl/service.h>
#include <rcl/node.h>
#include <rclcpp/node_interfaces/node_base_interface.hpp>
#include <rmw/rmw.h>
#include <rmw/time.h>
#include <std_srvs/srv/empty.hpp>
#include <type_traits>
#define HOLD(name) decltype(&name) volatile keep_##name=&name
HOLD(rcl_get_zero_initialized_client);HOLD(rcl_get_zero_initialized_service);
HOLD(rcl_client_get_default_options);HOLD(rcl_service_get_default_options);
HOLD(rcl_client_init);HOLD(rcl_service_init);HOLD(rcl_client_fini);HOLD(rcl_service_fini);
HOLD(rcl_client_get_rmw_handle);HOLD(rcl_node_is_valid);
HOLD(rcl_client_get_service_name);HOLD(rcl_service_get_service_name);
HOLD(rcl_send_request);HOLD(rcl_send_response);
HOLD(rcl_take_response_with_info);HOLD(rcl_take_request_with_info);
HOLD(rcl_client_request_publisher_get_actual_qos);
HOLD(rcl_client_response_subscription_get_actual_qos);
HOLD(rcl_service_request_subscription_get_actual_qos);
HOLD(rcl_service_response_publisher_get_actual_qos);
HOLD(rmw_get_gid_for_client);HOLD(rmw_time_equal);HOLD(rmw_time_normalize);
#undef HOLD
static_assert(sizeof(rmw_request_id_t::writer_guid)==16);
static_assert(sizeof(rmw_gid_t::data)==16);
static_assert(sizeof(rmw_service_info_t::source_timestamp)==8&&sizeof(rmw_service_info_t::received_timestamp)==8);
static_assert(std::is_signed_v<decltype(rmw_service_info_t::source_timestamp)>);
static_assert(std::is_signed_v<decltype(rmw_service_info_t::received_timestamp)>);
static_assert(std::is_signed_v<decltype(rmw_request_id_t::sequence_number)>);
static_assert(sizeof(std_srvs::srv::Empty::Event)>0);
using shared_node=decltype(std::declval<rclcpp::node_interfaces::NodeBaseInterface&>().get_shared_rcl_node_handle());
static_assert(std::is_same_v<shared_node,std::shared_ptr<rcl_node_t>>);
int main(){rmw_qos_profile_t p{};p.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;
 p.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;p.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
 p.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;p.depth=1;p.deadline={0,0};
 p.lifespan={0,0};p.liveliness_lease_duration={0,0};p.avoid_ros_namespace_conventions=false;
 return keep_rcl_get_zero_initialized_client&&keep_rcl_get_zero_initialized_service&&
 keep_rcl_client_get_default_options&&keep_rcl_service_get_default_options&&
 keep_rcl_client_init&&keep_rcl_service_init&&keep_rcl_client_fini&&keep_rcl_service_fini&&
 keep_rcl_client_get_rmw_handle&&keep_rcl_node_is_valid&&
 keep_rcl_client_get_service_name&&keep_rcl_service_get_service_name&&
 keep_rcl_send_request&&keep_rcl_send_response&&keep_rcl_take_response_with_info&&keep_rcl_take_request_with_info&&
 keep_rcl_client_request_publisher_get_actual_qos&&keep_rcl_client_response_subscription_get_actual_qos&&
 keep_rcl_service_request_subscription_get_actual_qos&&keep_rcl_service_response_publisher_get_actual_qos&&
 keep_rmw_get_gid_for_client&&keep_rmw_time_equal&&keep_rmw_time_normalize?0:1;}
]=] UWVM_NATIVE_RCL_SERVICE_API)
  unset(UWVM_NATIVE_RCL_AVAILABILITY_API)
  unset(UWVM_NATIVE_RCL_AVAILABILITY_API CACHE)
  check_cxx_source_compiles([=[
#include <rcl/graph.h>
decltype(&rcl_service_server_is_available) volatile query=&rcl_service_server_is_available;
int main(){return query?0:1;}
]=] UWVM_NATIVE_RCL_AVAILABILITY_API)
  # Genuine original Node/Event/context/graph predicate functions must COMPILE
  # AND LINK in this actual selected SDK. Volatile member/function pointers
  # retain all symbols; no distribution label or test-only header enables this.
  unset(UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API)
  unset(UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API CACHE)
  # Only this NEW optional check overrides the toolchain shortcut. The old
  # two probes above retain their complete original bodies and semantics.
  # Function scope restores the caller's target type and required variables.
  set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
  check_cxx_source_compiles([=[
#include <rclcpp/node.hpp>
#include <rclcpp/event.hpp>
#include <rclcpp/node_interfaces/node_base_interface.hpp>
#include <rclcpp/utilities.hpp>
#include <rcl/graph.h>
#include <poll.h>
#include <type_traits>
#include <chrono>
#include <atomic>
#include <csignal>
#ifndef __linux__
#error Native borrowed session/pidfd cancellation requires actual Linux
#endif
#ifndef POLLRDHUP
#error Actual socket half-close observation unavailable
#endif
static_assert(std::atomic<std::sig_atomic_t>::is_always_lock_free,
 "Actual signal-handler stores must be always-lock-free on selected Linux");
static std::atomic<std::sig_atomic_t> stop_state{0};
static void stop_handler(int)noexcept{stop_state.store(1,std::memory_order_relaxed);}
auto volatile graph_event=&rclcpp::Node::get_graph_event;
auto volatile graph_wait=&rclcpp::Node::wait_for_graph_change;
auto volatile event_clear=&rclcpp::Event::check_and_clear;
decltype(&rcl_service_server_is_available) volatile native_ready=&rcl_service_server_is_available;
decltype(&::poll) volatile kernel_poll=&::poll;
static_assert(std::is_same_v<decltype(std::declval<rclcpp::Node&>().get_graph_event()),rclcpp::Event::SharedPtr>);
static_assert(std::is_same_v<decltype(std::declval<rclcpp::Node&>().wait_for_graph_change(
 std::declval<rclcpp::Event::SharedPtr>(),std::chrono::nanoseconds{1})),void>);
bool original_context_ok(rclcpp::Node& node){return rclcpp::ok(node.get_node_base_interface()->get_context());}
auto volatile context_ok=&original_context_ok;
int main(){pollfd actual{0,POLLRDHUP,0};stop_handler(0);if(stop_state.load(std::memory_order_relaxed)!=1)return 1;return graph_event&&graph_wait&&event_clear&&native_ready&&kernel_poll&&context_ok&&actual.events?0:1;}
]=] UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API)
  configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/native_rcl_service_features.h.in"
    "${output_directory}/native_rcl_service_features.h" @ONLY)
  set(UWVM_NATIVE_RCL_SERVICE_FEATURE_INCLUDE_DIR "${output_directory}" PARENT_SCOPE)
endfunction()
