/** @file
 * @brief Compile/import-inspection implementation for the C and C++ ABI probes.
 *
 * This file is NOT a runnable ROS tutorial. The exported selector deliberately
 * passes zero raw offsets, lengths and handles. Those values are placeholders,
 * not valid buffers, aliases, resources or trusted grants. Never invoke the
 * selector as a ROS operation. Each wrapper's main returns without calling it.
 *
 * A variable externally supplied index keeps every selected raw import reachable
 * from an explicitly exported, non-inlined function after optimization and
 * dead stripping. The matrix separately requires the COMPLETE ABI-v1 import
 * inventory and exact signatures, then runs a full Wasm validator. Compilation
 * proves neither Guest startup nor native ROS delivery, permissions or success.
 *
 * Use the public raw ABI declarations so the real C/C++ compiler checks every
 * argument's width. Include exactly one backend; obtaining these declarations
 * grants no authority. Keep this single implementation shared by both language
 * wrappers so neither can silently lose a backend function.
 */
#ifndef ROSWASM_EXAMPLE_ABI_COVERAGE_IMPL_H
#define ROSWASM_EXAMPLE_ABI_COVERAGE_IMPL_H
#include <stdint.h>
#if ROSWASM_EXAMPLE_ROS_VERSION == 1
#include <roswasm/abi/ros1.h>
#elif ROSWASM_EXAMPLE_ROS_VERSION == 2
#include <roswasm/abi/ros2.h>
#else
#error "Select exactly ROSWASM_EXAMPLE_ROS_VERSION=1 or 2 for ABI coverage"
#endif
#if !defined(__wasm__)
#error "This is a compile/import-inspection Wasm fixture, not a native ROS executable"
#endif
#ifdef __cplusplus
extern "C" {
#endif
/** Retain all functions of one backend. Do not execute any valid selector case.
 * Invalid indexes return unsupported status 95 without entering a ROS import. The
 * explicit linker export is also required by the maintained matrix recipe.
 */
__attribute__((export_name("roswasm_abi_coverage"), used, noinline))
uint32_t roswasm_abi_coverage(uint32_t index) {
#if ROSWASM_EXAMPLE_ROS_VERSION == 1
    switch(index) {
    case 0: return uwvm_ros1_abi_version();
    case 1: return uwvm_ros1_capability_open(0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 2: return uwvm_ros1_capability_close(UINT64_C(0));
    case 3: return uwvm_ros1_capability_revoke(UINT64_C(0));
    case 4: return uwvm_ros1_topic_publish(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0));
    case 5: return uwvm_ros1_topic_subscribe(UINT64_C(0));
    case 6: return uwvm_ros1_event_poll(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 7: return uwvm_ros1_service_call(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 8: return uwvm_ros1_service_advertise(UINT64_C(0));
    case 9: return uwvm_ros1_service_respond(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 10: return uwvm_ros1_action_send_goal(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 11: return uwvm_ros1_action_cancel_own_goal(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 12: return uwvm_ros1_action_cancel_any_goal(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 13: return uwvm_ros1_action_receive_feedback(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 14: return uwvm_ros1_action_receive_result(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 15: return uwvm_ros1_action_advertise(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 16: return uwvm_ros1_action_respond(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 17: return uwvm_ros1_tf_lookup(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 18: return uwvm_ros1_tf_broadcast(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 19: return uwvm_ros1_parameter_get(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 20: return uwvm_ros1_parameter_set(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 21: return uwvm_ros1_parameter_delete(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 22: return uwvm_ros1_parameter_exists(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 23: return uwvm_ros1_parameter_list(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 24: return uwvm_ros1_parameter_search(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 25: return uwvm_ros1_graph_inspect_topic(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 26: return uwvm_ros1_graph_inspect_node(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 27: return uwvm_ros1_graph_inspect_service(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 28: return uwvm_ros1_graph_list_topics(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 29: return uwvm_ros1_graph_list_nodes(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 30: return uwvm_ros1_graph_list_services(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 31: return uwvm_ros1_time_read_ros(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 32: return uwvm_ros1_time_read_wall(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 33: return uwvm_ros1_log_write(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0));
    case 34: return uwvm_ros1_event_watch(UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 35: return uwvm_ros1_event_poll_v1(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 36: return uwvm_ros1_event_ready(UINT64_C(0),0u,UINT64_C(0));
    case 37: return uwvm_ros1_service_submit(UINT64_C(0),UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    default: return 95u;
    }
#else
    switch(index) {
    case 0: return uwvm_ros2_abi_version();
    case 1: return uwvm_ros2_capability_open(0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 2: return uwvm_ros2_capability_close(UINT64_C(0));
    case 3: return uwvm_ros2_capability_revoke(UINT64_C(0));
    case 4: return uwvm_ros2_topic_publish(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0));
    case 5: return uwvm_ros2_topic_subscribe(UINT64_C(0));
    case 6: return uwvm_ros2_event_poll(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 7: return uwvm_ros2_service_call(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 8: return uwvm_ros2_service_advertise(UINT64_C(0));
    case 9: return uwvm_ros2_service_respond(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 10: return uwvm_ros2_action_send_goal(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 11: return uwvm_ros2_action_cancel_own_goal(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 12: return uwvm_ros2_action_cancel_any_goal(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 13: return uwvm_ros2_action_receive_feedback(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 14: return uwvm_ros2_action_receive_result(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 15: return uwvm_ros2_action_advertise(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 16: return uwvm_ros2_action_respond(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 17: return uwvm_ros2_tf_lookup(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 18: return uwvm_ros2_tf_broadcast(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 19: return uwvm_ros2_parameter_get(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 20: return uwvm_ros2_parameter_set(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 21: return uwvm_ros2_parameter_list(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 22: return uwvm_ros2_parameter_describe(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 23: return uwvm_ros2_parameter_declare(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 24: return uwvm_ros2_parameter_undeclare(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 25: return uwvm_ros2_graph_inspect_topic(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 26: return uwvm_ros2_graph_inspect_node(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 27: return uwvm_ros2_graph_inspect_service(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 28: return uwvm_ros2_graph_inspect_action(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 29: return uwvm_ros2_parameter_subscribe_events(UINT64_C(0));
    case 30: return uwvm_ros2_graph_list_topics(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 31: return uwvm_ros2_graph_list_nodes(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 32: return uwvm_ros2_graph_list_services(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 33: return uwvm_ros2_lifecycle_get_state(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 34: return uwvm_ros2_lifecycle_change_state(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 35: return uwvm_ros2_time_read_ros(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 36: return uwvm_ros2_time_read_system(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 37: return uwvm_ros2_time_read_steady(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 38: return uwvm_ros2_log_write(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0));
    case 39: return uwvm_ros2_event_watch(UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 40: return uwvm_ros2_event_poll_v1(UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    case 41: return uwvm_ros2_event_ready(UINT64_C(0),0u,UINT64_C(0));
    case 42: return uwvm_ros2_service_submit(UINT64_C(0),UINT64_C(0),0u,UINT64_C(0),UINT64_C(0),UINT64_C(0));
    default: return 95u;
    }
#endif
}
#ifdef __cplusplus
}
#endif
#endif
