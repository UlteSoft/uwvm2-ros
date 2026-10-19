#pragma once
/* Native SDK capabilities, never Guest authority or a distribution number.
 * CMake replaces these conservative header-only defaults with actual compile
 * and link probes. Direct handwritten fixtures may include this header without
 * a configured build directory; they must not infer a successful native test.
 * Resolve __has_include only in preprocessing conditionals (C++17 [cpp.cond]).
 * A feature is also used in ordinary C++ expressions, where a replacement list
 * containing __has_include is invalid. Each fallback therefore supplies 0 or 1;
 * header presence retains exactly its prior meaning, not a link/API proof.
 * Sources: official ROS2 Dashing/Iron/Lyrical release notes and rclcpp headers.
 */
#if __has_include("uwvm_ros2_features.h")
#include "uwvm_ros2_features.h"
#endif
#ifndef UWVM_ROS2_HAVE_QOS
#if __has_include(<rclcpp/qos.hpp>)
#define UWVM_ROS2_HAVE_QOS 1
#else
#define UWVM_ROS2_HAVE_QOS 0
#endif
#endif
#ifndef UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
// A complete profile includes actual RMW duration semantics and linked
// utility symbols. Header presence alone cannot establish this contract.
// Configured compile AND link probes enable raw-profile or QoS-class SDKs.
#define UWVM_ROS2_HAVE_COMPLETE_RMW_QOS 0
#endif
#ifndef UWVM_ROS2_HAVE_NODE_OPTIONS
#if __has_include(<rclcpp/node_options.hpp>)
#define UWVM_ROS2_HAVE_NODE_OPTIONS 1
#else
#define UWVM_ROS2_HAVE_NODE_OPTIONS 0
#endif
#endif
#ifndef UWVM_ROS2_HAVE_PARAMETER_DECLARE
#define UWVM_ROS2_HAVE_PARAMETER_DECLARE UWVM_ROS2_HAVE_NODE_OPTIONS
#endif
#ifndef UWVM_ROS2_HAVE_DYNAMIC_TYPING
#define UWVM_ROS2_HAVE_DYNAMIC_TYPING 0
#endif
#ifndef UWVM_ROS2_HAVE_POST_SET
#if __has_include(<rosidl_runtime_c/type_hash.h>)
#define UWVM_ROS2_HAVE_POST_SET 1
#else
#define UWVM_ROS2_HAVE_POST_SET 0
#endif
#endif
#ifndef UWVM_ROS2_HAVE_TYPE_DESCRIPTION
#if __has_include(<rosidl_runtime_c/type_description/type_description__struct.h>)
#define UWVM_ROS2_HAVE_TYPE_DESCRIPTION 1
#else
#define UWVM_ROS2_HAVE_TYPE_DESCRIPTION 0
#endif
#endif
#ifndef UWVM_ROS2_HAVE_DEFERRED_SERVICE
#define UWVM_ROS2_HAVE_DEFERRED_SERVICE UWVM_ROS2_HAVE_TYPE_DESCRIPTION
#endif
#ifndef UWVM_ROS2_HAVE_ACTION
#if __has_include(<rclcpp_action/rclcpp_action.hpp>) && __has_include(<action_tutorials_interfaces/action/fibonacci.hpp>)
#define UWVM_ROS2_HAVE_ACTION 1
#else
#define UWVM_ROS2_HAVE_ACTION 0
#endif
#endif
// A handwritten fixture's existing ACTION presence heuristic is retained only
// as a core-header hint. It cannot enable any concrete custom Action route:
// that requires the configured full typed client/server compile/link probe.
#ifndef UWVM_ROS2_HAVE_ACTION_CORE
#define UWVM_ROS2_HAVE_ACTION_CORE UWVM_ROS2_HAVE_ACTION
#endif
#ifndef UWVM_ROS2_HAVE_CUSTOM_ACTION
#define UWVM_ROS2_HAVE_CUSTOM_ACTION 0
#endif
#ifndef UWVM_ROS2_HAVE_LIFECYCLE
#if __has_include(<lifecycle_msgs/srv/get_state.hpp>)
#define UWVM_ROS2_HAVE_LIFECYCLE 1
#else
#define UWVM_ROS2_HAVE_LIFECYCLE 0
#endif
#endif
#ifndef UWVM_ROS2_HAVE_ACTION_GRAPH
#if __has_include(<rcl_action/graph.h>)
#define UWVM_ROS2_HAVE_ACTION_GRAPH 1
#else
#define UWVM_ROS2_HAVE_ACTION_GRAPH 0
#endif
#endif


#ifndef UWVM_ROS2_HAVE_QUIET_NODE
#define UWVM_ROS2_HAVE_QUIET_NODE UWVM_ROS2_HAVE_NODE_OPTIONS
#endif
#ifndef UWVM_ROS2_HAVE_LEGACY_DEFERRED_SERVICE
#define UWVM_ROS2_HAVE_LEGACY_DEFERRED_SERVICE 0
#endif
#ifndef UWVM_ROS2_HAVE_ENCLAVE_ARGUMENT
#define UWVM_ROS2_HAVE_ENCLAVE_ARGUMENT UWVM_ROS2_HAVE_TYPE_DESCRIPTION
#endif
#ifndef UWVM_ROS2_HAVE_ACTUAL_SERVICE_QOS
#define UWVM_ROS2_HAVE_ACTUAL_SERVICE_QOS 0
#endif
#ifndef UWVM_ROS2_HAVE_TF
#if (__has_include(<tf2/buffer_core.hpp>) || __has_include(<tf2/buffer_core.h>)) && __has_include(<tf2_msgs/msg/tf_message.hpp>)
#define UWVM_ROS2_HAVE_TF 1
#else
#define UWVM_ROS2_HAVE_TF 0
#endif
#endif
