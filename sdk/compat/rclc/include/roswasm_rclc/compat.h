/* Retained attribution for adapted declarations and QoS enum comments:
 * Copyright 2014-2017 Open Source Robotics Foundation, Inc.
 *   ros2/rmw rmw/include/rmw/types.h
 * Copyright 2015 Open Source Robotics Foundation, Inc.
 *   ros2/rmw rmw/include/rmw/qos_profiles.h
 * Copyright 2014-2018 Open Source Robotics Foundation, Inc.
 *   ros2/rmw rmw/include/rmw/ret_types.h
 *   rmw commit 8cb94a87cf3f4049abe2d563af893517bf336930.
 * Copyright 2015 Open Source Robotics Foundation, Inc.
 *   ros2/rcutils include/rcutils/allocator.h
 * Copyright 2014 Open Source Robotics Foundation, Inc.
 *   ros2/rcutils include/rcutils/error_handling.h
 *   rcutils commit 26413034bab69688475f230ea7723e534ec3f8cd.
 * Copyright 2015 Open Source Robotics Foundation, Inc.
 *   ros2/rosidl rosidl_runtime_c/include/rosidl_runtime_c/string.h
 *   rosidl commit 85fa592b698b0f665e3120f48fac0d35e2f7d8a4.
 * Copyright 2014 Open Source Robotics Foundation, Inc.
 *   ros2/rcl rcl/include/rcl/types.h
 *   rcl commit 18d96c9c2b98e616324ae9a2c7701dd0b0886ff0.
 * Copyright (c) 2019, 2020 - for the respective rclc copyright owners,
 * see upstream/NOTICE and https://github.com/ros2/rclc.
 * Copyright 2014 Open Source Robotics Foundation, Inc.
 *   ros2/rclc rclc/include/rclc/types.h and rclc/include/rclc/executor.h
 *   rclc commit 45ef3de567588652c7f05017a91ea29921dd6862.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Changed source: the retained public spellings and comments are adapted to
 * this finite Guest-only source profile, with private ownership fields and
 * selected APIs. The unsupported native deprecation annotation dependency is
 * omitted while its original enumerator and value2 are retained. This file
 * does not declare native binary ABI compatibility or implement native RMW.
 */
/** @file
 * Opt-in C SOURCE adapter for two pinned complete Jazzy rclc examples.
 *
 * This is not the native rcl/rmw binary ABI, a DDS waitset or full rclc.
 * Only put this include directory before native ROS headers for an explicitly
 * selected Guest build. Never mix its objects/structs with installed rcl libs.
 * The public source spellings/signatures, rcl_ret_t numeric results, allocator
 * callbacks and String lifecycle are based on the retained official sources.
 * Layouts with uwvm fields are private Guest storage, not native ABI promises.
 *
 * The selection is immutable trusted build input; node/namespace/topic/type/QoS
 * must all match exactly before opening its explicit alias. No discovery,
 * remapping, ambient ROS grant, Host debug/update/policy request or wait thread
 * is added. A separate selected clock alias is needed by timer operations.
 *
 * All context/resource/executor addresses must remain stable until fini. Copying
 * them never duplicates ownership. One context is single-threaded; callbacks
 * must return normally (no longjmp/terminal trap through leases). Whole-context
 * mutation/close/open during dispatch is refused BEFORE mutation; publishing
 * through an already retained publisher is allowed in a timer callback.
 * Fixed ownership slots and cleanup statuses exist before opens. A close is
 * attempted once even on107/110; its separate result remains inspectable.
 * Unsupported API names are absent or explicitly return UNSUPPORTED, never OK.
 */
#ifndef UWVM_ROSWASM_RCLC_COMPAT_H
#define UWVM_ROSWASM_RCLC_COMPAT_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <roswasm/node.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RCLC_MAX_OWNERS 256u
#define UWVM_RCLC_MAX_HANDLES 256u
#define UWVM_RCLC_MAX_WIRE 65540u
#define UWVM_RCLC_MAX_CONTEXTS 16u
typedef int32_t rmw_ret_t;
typedef rmw_ret_t rcl_ret_t;
#define RCL_RET_OK 0
#define RCL_RET_ERROR 1
#define RCL_RET_TIMEOUT 2
#define RCL_RET_BAD_ALLOC 10
#define RCL_RET_INVALID_ARGUMENT 11
#define RCL_RET_UNSUPPORTED 3
#define RCL_RET_ALREADY_INIT 100
#define RCL_RET_NOT_INIT 101
#define RCL_RET_MISMATCHED_RMW_ID 102
#define RCL_RET_TOPIC_NAME_INVALID 103
#define RCL_RET_SERVICE_NAME_INVALID 104
#define RCL_RET_UNKNOWN_SUBSTITUTION 105
#define RCL_RET_ALREADY_SHUTDOWN 106
#define RCL_RET_NOT_FOUND 107
#define RCL_RET_NODE_INVALID 200
#define RCL_RET_NODE_INVALID_NAME 201
#define RCL_RET_NODE_INVALID_NAMESPACE 202
#define RCL_RET_NODE_NAME_NON_EXISTENT 203
#define RCL_RET_PUBLISHER_INVALID 300
#define RCL_RET_SUBSCRIPTION_INVALID 400
#define RCL_RET_SUBSCRIPTION_TAKE_FAILED 401
#define RCL_RET_CLIENT_INVALID 500
#define RCL_RET_CLIENT_TAKE_FAILED 501
#define RCL_RET_SERVICE_INVALID 600
#define RCL_RET_SERVICE_TAKE_FAILED 601
#define RCL_RET_TIMER_INVALID 800
#define RCL_RET_TIMER_CANCELED 801
#define RCL_RET_WAIT_SET_INVALID 900
#define RCL_RET_WAIT_SET_EMPTY 901
#define RCL_RET_WAIT_SET_FULL 902
#define RCL_RET_INVALID_REMAP_RULE 1001
#define RCL_RET_WRONG_LEXEME 1002
#define RCL_RET_INVALID_ROS_ARGS 1003
#define RCL_RET_INVALID_PARAM_RULE 1010
#define RCL_RET_INVALID_LOG_LEVEL_RULE 1020
#define RCL_RET_EVENT_INVALID 2000
#define RCL_RET_EVENT_TAKE_FAILED 2001
#define RCL_RET_LIFECYCLE_STATE_REGISTERED 3000
#define RCL_RET_LIFECYCLE_STATE_NOT_REGISTERED 3001
#define RCL_MS_TO_NS(ms) ((ms) * 1000000LL)
#define RCL_US_TO_NS(us) ((us) * 1000LL)
#define RCL_S_TO_NS(s) ((s) * 1000000000LL)
#define RCLC_UNUSED(x) (void)(x)
/* Trivially copyable real rcutils allocator callback/state shape. The allocator
 * and its state must stay valid through the last allocation's deallocation. */
typedef struct rcutils_allocator_s {
 void*(*allocate)(size_t,void*);void(*deallocate)(void*,void*);
 void*(*reallocate)(void*,size_t,void*);void*(*zero_allocate)(size_t,size_t,void*);void*state;
} rcutils_allocator_t;
typedef rcutils_allocator_t rcl_allocator_t;
rcutils_allocator_t rcutils_get_default_allocator(void);
rcutils_allocator_t rcutils_get_zero_initialized_allocator(void);
bool rcutils_allocator_is_valid(rcutils_allocator_t const*);
void* rcutils_reallocf(void*,size_t,rcutils_allocator_t*);
#define rcl_get_default_allocator rcutils_get_default_allocator
#define rcl_reallocf rcutils_reallocf
#define RCUTILS_ERROR_MESSAGE_MAX_LENGTH 1024
#define RCUTILS_ERROR_STATE_MESSAGE_MAX_LENGTH 768
#define RCUTILS_ERROR_STATE_FILE_MAX_LENGTH 229
typedef struct rcutils_error_string_s {char str[1024];} rcutils_error_string_t;
typedef struct rcutils_error_state_s {char message[768];char file[229];uint64_t line_number;}rcutils_error_state_t;
typedef rcutils_error_string_t rcl_error_string_t;
void rcutils_set_error_state(char const*,char const*,size_t);
bool rcutils_error_is_set(void);
rcutils_error_state_t const*rcutils_get_error_state(void);
rcutils_error_string_t rcutils_get_error_string(void);
void rcutils_reset_error(void);
#define rcl_get_error_state rcutils_get_error_state
#define rcl_get_error_string rcutils_get_error_string
#define rcl_reset_error rcutils_reset_error
#define rcl_error_is_set rcutils_error_is_set
#define RCL_SET_ERROR_MSG(s) rcutils_set_error_state((s),__FILE__,__LINE__)
#define RCUTILS_SET_ERROR_MSG(s) RCL_SET_ERROR_MSG(s)
/* Real native String field shape and owned malloc/realloc lifecycle. The wire
 * bound is applied by the selected endpoint, not by projecting this structure. */
typedef struct rosidl_runtime_c__String {char*data;size_t size;size_t capacity;}rosidl_runtime_c__String;
bool rosidl_runtime_c__String__init(rosidl_runtime_c__String*);
void rosidl_runtime_c__String__fini(rosidl_runtime_c__String*);
bool rosidl_runtime_c__String__assignn(rosidl_runtime_c__String*,char const*,size_t);
bool rosidl_runtime_c__String__assign(rosidl_runtime_c__String*,char const*);
bool rosidl_runtime_c__String__copy(rosidl_runtime_c__String const*,rosidl_runtime_c__String*);
bool rosidl_runtime_c__String__are_equal(rosidl_runtime_c__String const*,rosidl_runtime_c__String const*);
typedef struct std_msgs__msg__String {rosidl_runtime_c__String data;}std_msgs__msg__String;
bool std_msgs__msg__String__init(std_msgs__msg__String*);
void std_msgs__msg__String__fini(std_msgs__msg__String*);
std_msgs__msg__String*std_msgs__msg__String__create(void);
void std_msgs__msg__String__destroy(std_msgs__msg__String*);
bool std_msgs__msg__String__copy(std_msgs__msg__String const*,std_msgs__msg__String*);
bool std_msgs__msg__String__are_equal(std_msgs__msg__String const*,std_msgs__msg__String const*);
/* A finite Guest type token only: its NULL native metadata functions do not
 * assert an installed native typesupport, baked RIHS or native ABI identity. */
typedef struct rosidl_type_hash_s rosidl_type_hash_t;
typedef struct rosidl_runtime_c__type_description__TypeDescription rosidl_runtime_c__type_description__TypeDescription;
typedef struct rosidl_runtime_c__type_description__TypeSource__Sequence rosidl_runtime_c__type_description__TypeSource__Sequence;
typedef struct rosidl_message_type_support_t rosidl_message_type_support_t;
typedef rosidl_message_type_support_t const*(*rosidl_message_typesupport_handle_function)(rosidl_message_type_support_t const*,char const*);
struct rosidl_message_type_support_t {
 char const*typesupport_identifier;void const*data;rosidl_message_typesupport_handle_function func;
 rosidl_type_hash_t const*(*get_type_hash_func)(rosidl_message_type_support_t const*);
 rosidl_runtime_c__type_description__TypeDescription const*(*get_type_description_func)(rosidl_message_type_support_t const*);
 rosidl_runtime_c__type_description__TypeSource__Sequence const*(*get_type_description_sources_func)(rosidl_message_type_support_t const*);
};
rosidl_message_type_support_t const*rosidl_typesupport_c__get_message_type_support_handle__std_msgs__msg__String(void);
#define ROSIDL_GET_MSG_TYPE_SUPPORT(pkg,sub,name) rosidl_typesupport_c__get_message_type_support_handle__##pkg##__##sub##__##name()
typedef enum rmw_qos_history_policy_e
{
  /// Implementation default for history policy
  RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT,

  /// Only store up to a maximum number of samples, dropping oldest once max is exceeded
  RMW_QOS_POLICY_HISTORY_KEEP_LAST,

  /// Store all samples, subject to resource limits
  RMW_QOS_POLICY_HISTORY_KEEP_ALL,

  /// History policy has not yet been set
  RMW_QOS_POLICY_HISTORY_UNKNOWN
} rmw_qos_history_policy_t;
typedef enum rmw_qos_reliability_policy_e
{
  /// Implementation specific default
  RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT,

  /// Guarantee that samples are delivered, may retry multiple times.
  RMW_QOS_POLICY_RELIABILITY_RELIABLE,

  /// Attempt to deliver samples, but some may be lost if the network is not robust
  RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,

  /// Reliability policy has not yet been set
  RMW_QOS_POLICY_RELIABILITY_UNKNOWN,

  /// Will match the majority of endpoints and use a reliable policy if possible
  /**
   * A policy will be chosen at the time of creating a subscription or publisher.
   * A reliable policy will by chosen if it matches with all discovered endpoints,
   * otherwise a best effort policy will be chosen.
   *
   * The QoS policy reported by functions like `rmw_subscription_get_actual_qos` or
   * `rmw_publisher_get_actual_qos` may be best available, reliable, or best effort.
   *
   * Services and clients are not supported and default to the reliability value in
   * `rmw_qos_profile_services_default`.
   *
   * The middleware is not expected to update the policy after creating a subscription or
   * publisher, even if the chosen policy is incompatible with newly discovered endpoints.
   * Therefore, this policy should be used with care since non-deterministic behavior
   * can occur due to races with discovery.
   */
  RMW_QOS_POLICY_RELIABILITY_BEST_AVAILABLE
} rmw_qos_reliability_policy_t;
typedef enum rmw_qos_durability_policy_e
{
  /// Impplementation specific default
  RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT,

  /// The rmw publisher is responsible for persisting samples for “late-joining” subscribers
  RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL,

  /// Samples are not persistent
  RMW_QOS_POLICY_DURABILITY_VOLATILE,

  /// Durability policy has not yet been set
  RMW_QOS_POLICY_DURABILITY_UNKNOWN,

  /// Will match the majority of endpoints and use a transient local policy if possible
  /**
   * A policy will be chosen at the time of creating a subscription or publisher.
   * A transient local policy will by chosen if it matches with all discovered endpoints,
   * otherwise a volatile policy will be chosen.
   *
   * In the case that a volatile policy is chosen for a subscription, any messages sent before
   * the subscription was created by transient local publishers will not be received.
   *
   * The QoS policy reported by functions like `rmw_subscription_get_actual_qos` or
   * `rmw_publisher_get_actual_qos` may be best available, transient local, or volatile.
   *
   * Services and clients are not supported and default to the durability value in
   * `rmw_qos_profile_services_default`.
   *
   * The middleware is not expected to update the policy after creating a subscription or
   * publisher, even if the chosen policy is incompatible with newly discovered endpoints.
   * Therefore, this policy should be used with care since non-deterministic behavior
   * can occur due to races with discovery.
   */
  RMW_QOS_POLICY_DURABILITY_BEST_AVAILABLE
} rmw_qos_durability_policy_t;
typedef enum rmw_qos_liveliness_policy_e
{
  /// Implementation specific default
  RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT = 0,

  /// The signal that establishes a Topic is alive comes from the ROS rmw layer.
  RMW_QOS_POLICY_LIVELINESS_AUTOMATIC = 1,

  /// Explicitly asserting node liveliness is required in this case.
  /// This option is deprecated, use RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC if your application
  /// requires to assert liveliness manually.
  RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE = 2,

  /// The signal that establishes a Topic is alive is at the Topic level. Only publishing a message
  /// on the Topic or an explicit signal from the application to assert liveliness on the Topic
  /// will mark the Topic as being alive.
  // Using `3` for backwards compatibility.
  RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC = 3,

  /// Liveliness policy has not yet been set
  RMW_QOS_POLICY_LIVELINESS_UNKNOWN = 4,

  /// Will match the majority of endpoints and use a manual by topic policy if possible
  /**
   * A policy will be chosen at the time of creating a subscription or publisher.
   * A manual by topic policy will by chosen if it matches with all discovered endpoints,
   * otherwise an automatic policy will be chosen.
   *
   * The QoS policy reported by functions like `rmw_subscription_get_actual_qos` or
   * `rmw_publisher_get_actual_qos` may be best available, automatic, or manual by topic.
   *
   * Services and clients are not supported and default to the liveliness value in
   * `rmw_qos_profile_services_default`.
   *
   * The middleware is not expected to update the policy after creating a subscription or
   * publisher, even if the chosen policy is incompatible with newly discovered endpoints.
   * Therefore, this policy should be used with care since non-deterministic behavior
   * can occur due to races with discovery.
   */
  RMW_QOS_POLICY_LIVELINESS_BEST_AVAILABLE = 5
} rmw_qos_liveliness_policy_t;
typedef struct rmw_time_s {uint64_t sec;uint64_t nsec;}rmw_time_t;
typedef struct rmw_qos_profile_s {
 enum rmw_qos_history_policy_e history;size_t depth;
 enum rmw_qos_reliability_policy_e reliability;enum rmw_qos_durability_policy_e durability;
 struct rmw_time_s deadline,lifespan;enum rmw_qos_liveliness_policy_e liveliness;
 struct rmw_time_s liveliness_lease_duration;bool avoid_ros_namespace_conventions;
}rmw_qos_profile_t;
extern rmw_qos_profile_t const rmw_qos_profile_default,rmw_qos_profile_sensor_data;
typedef enum uwvm_rclc_role {UWVM_RCLC_PUBLISH,UWVM_RCLC_SUBSCRIBE}uwvm_rclc_role;
typedef struct uwvm_rclc_topic_binding {
 char const*node;char const*namespace_;char const*topic;char const*type;
 uwvm_rclc_role role;char const*alias;rmw_qos_profile_t qos;size_t maximum_wire_bytes;
}uwvm_rclc_topic_binding;
typedef struct uwvm_rclc_node_binding {char const*node;char const*namespace_;}uwvm_rclc_node_binding;
/** step is an explicit bounded pacing/shutdown source, NOT a DDS waitset.
 * 0 permits one pass,125 requests local shutdown, every other code is failure.
 * It may block only within an externally enforced finite deadline. It cannot
 * add authority. timeout_ns is the caller's request, not a fabricated wait.
 * Actual future native integration must qualify the source and Host policy. */
struct uwvm_rclc_service_profile;
typedef struct uwvm_rclc_selection {
 roswasmc_backend const*backend;uwvm_rclc_topic_binding const*topics;size_t topic_count;
 uwvm_rclc_node_binding const*nodes;size_t node_count;char const*steady_clock_alias;
 uint32_t(*step)(void*,uint64_t);void*step_context;size_t maximum_spin_steps;
 struct uwvm_rclc_service_profile const*services; /* Optional trailing profile; NULL preserves topics. */
}uwvm_rclc_selection;
uwvm_rclc_selection const*uwvm_rclc_selected_configuration(void);
/** Last operation error is separate from every acquisition-index cleanup result.
 * No printf/error-string allocation participates in the cleanup loop. */
typedef struct uwvm_rclc_diagnostics {
 rcl_ret_t primary;uint32_t raw_primary;roswasmc_cleanup_report cleanup;
 uint32_t close_codes[UWVM_RCLC_MAX_OWNERS];size_t close_attempts[UWVM_RCLC_MAX_OWNERS];
}uwvm_rclc_diagnostics;
typedef struct rcl_context_s {
 uint64_t uwvm_generation;bool uwvm_initialized;bool uwvm_shutdown;size_t uwvm_dispatch;
 uwvm_rclc_selection const*uwvm_selection;rcl_allocator_t uwvm_allocator;
 roswasmc_local_context uwvm_lifecycle;roswasmc_session uwvm_session;
 roswasmc_capability uwvm_owners[UWVM_RCLC_MAX_OWNERS];
 roswasmc_capability*uwvm_clock;uwvm_rclc_diagnostics uwvm_diagnostics;
 uint32_t uwvm_sleep_error; /* Latched failure from the official void sleep API. */
}rcl_context_t;
typedef int64_t rcl_time_point_value_t;
typedef int64_t rcl_duration_value_t;
typedef enum rcl_clock_type_e {RCL_CLOCK_UNINITIALIZED=0,RCL_ROS_TIME=1,RCL_SYSTEM_TIME=2,RCL_STEADY_TIME=3}rcl_clock_type_t;
typedef struct rcl_clock_s {rcl_clock_type_t type;rcl_context_t*uwvm_context;uint64_t uwvm_generation;}rcl_clock_t;
typedef struct rcl_node_s {rcl_context_t*context;uint64_t uwvm_generation;char const*uwvm_name;char const*uwvm_namespace;size_t uwvm_children;bool uwvm_initialized;}rcl_node_t;
typedef struct rcl_publisher_s {rcl_node_t const*uwvm_node;roswasmc_capability*uwvm_owner;uint64_t uwvm_generation;uwvm_rclc_topic_binding const*uwvm_binding;}rcl_publisher_t;
typedef struct rcl_subscription_s {rcl_node_t*uwvm_node;roswasmc_capability*uwvm_owner;uint64_t uwvm_generation;uwvm_rclc_topic_binding const*uwvm_binding;}rcl_subscription_t;
typedef struct rcl_timer_s rcl_timer_t;
typedef void(*rcl_timer_callback_t)(rcl_timer_t*,int64_t);
struct rcl_timer_s {rcl_context_t*uwvm_context;uint64_t uwvm_generation;rcl_timer_callback_t uwvm_callback;uint64_t uwvm_period,uwvm_due,uwvm_last;bool uwvm_initialized,uwvm_canceled;};
typedef struct rclc_support_t {rcl_context_t context;rcl_allocator_t*allocator;rcl_clock_t clock;}rclc_support_t;
rcl_context_t rcl_get_zero_initialized_context(void);
rcl_node_t rcl_get_zero_initialized_node(void);
rcl_publisher_t rcl_get_zero_initialized_publisher(void);
rcl_subscription_t rcl_get_zero_initialized_subscription(void);
rcl_timer_t rcl_get_zero_initialized_timer(void);
bool rcl_context_is_valid(rcl_context_t const*);
rcl_ret_t rcl_shutdown(rcl_context_t*);
rcl_ret_t rcl_context_fini(rcl_context_t*);
rcl_ret_t rclc_support_init(rclc_support_t*,int,char const*const*,rcl_allocator_t*);
rcl_ret_t rclc_support_fini(rclc_support_t*);
rcl_ret_t rclc_node_init_default(rcl_node_t*,char const*,char const*,rclc_support_t*);
rcl_ret_t rcl_node_fini(rcl_node_t*);
rcl_ret_t rclc_publisher_init(rcl_publisher_t*,rcl_node_t const*,rosidl_message_type_support_t const*,char const*,rmw_qos_profile_t const*);
rcl_ret_t rclc_publisher_init_default(rcl_publisher_t*,rcl_node_t const*,rosidl_message_type_support_t const*,char const*);
rcl_ret_t rclc_publisher_init_best_effort(rcl_publisher_t*,rcl_node_t const*,rosidl_message_type_support_t const*,char const*);
rcl_ret_t rclc_subscription_init(rcl_subscription_t*,rcl_node_t*,rosidl_message_type_support_t const*,char const*,rmw_qos_profile_t const*);
rcl_ret_t rclc_subscription_init_default(rcl_subscription_t*,rcl_node_t*,rosidl_message_type_support_t const*,char const*);
rcl_ret_t rclc_subscription_init_best_effort(rcl_subscription_t*,rcl_node_t*,rosidl_message_type_support_t const*,char const*);
rcl_ret_t rcl_publisher_fini(rcl_publisher_t*,rcl_node_t*);
rcl_ret_t rcl_subscription_fini(rcl_subscription_t*,rcl_node_t*);
typedef struct rmw_publisher_allocation_s rmw_publisher_allocation_t;
rcl_ret_t rcl_publish(rcl_publisher_t const*,void const*,rmw_publisher_allocation_t*);
rcl_ret_t rclc_timer_init_default2(rcl_timer_t*,rclc_support_t*,uint64_t,rcl_timer_callback_t,bool);
rcl_ret_t rclc_timer_init_default(rcl_timer_t*,rclc_support_t*,uint64_t,rcl_timer_callback_t);
rcl_ret_t rcl_timer_fini(rcl_timer_t*);
rcl_ret_t rcl_timer_cancel(rcl_timer_t*);
rcl_ret_t rcl_timer_reset(rcl_timer_t*);
rcl_ret_t rcl_timer_is_canceled(rcl_timer_t const*,bool*);
rcl_ret_t rcl_timer_get_period(rcl_timer_t const*,int64_t*);
typedef enum rclc_executor_handle_invocation_t {ON_NEW_DATA=0,ALWAYS=1}rclc_executor_handle_invocation_t;
typedef void(*rclc_subscription_callback_t)(void const*);
typedef void(*rclc_subscription_callback_with_context_t)(void const*,void*);
typedef struct uwvm_rclc_executor_handle {
 int kind;rcl_subscription_t*subscription;rcl_timer_t*timer;std_msgs__msg__String*message;
 rclc_subscription_callback_t callback;rclc_subscription_callback_with_context_t with_context;
 void*user;rclc_executor_handle_invocation_t invocation;unsigned char*wire;size_t wire_capacity;
 bool ready;
 /* Additive service handles borrow stable registered owners and messages. */
 void*uwvm_service;void*uwvm_client;void*uwvm_request;void*uwvm_response;
 void(*uwvm_service_callback)(void const*,void*);
 void(*uwvm_service_context_callback)(void const*,void*,void*);
 void(*uwvm_client_callback)(void const*);
}rclc_executor_handle_t;
typedef struct rclc_executor_t_s {
 rcl_context_t*context;uint64_t uwvm_generation;rcl_allocator_t uwvm_allocator;
 rclc_executor_handle_t*handles;size_t max_handles,index;uint64_t timeout_ns;
 size_t uwvm_steps;bool uwvm_initialized,uwvm_spinning;
}rclc_executor_t;
rclc_executor_t rclc_executor_get_zero_initialized_executor(void);
rcl_ret_t rclc_executor_init(rclc_executor_t*,rcl_context_t*,size_t,rcl_allocator_t const*);
rcl_ret_t rclc_executor_fini(rclc_executor_t*);
rcl_ret_t rclc_executor_add_subscription(rclc_executor_t*,rcl_subscription_t*,void*,rclc_subscription_callback_t,rclc_executor_handle_invocation_t);
rcl_ret_t rclc_executor_add_subscription_with_context(rclc_executor_t*,rcl_subscription_t*,void*,rclc_subscription_callback_with_context_t,void*,rclc_executor_handle_invocation_t);
rcl_ret_t rclc_executor_add_timer(rclc_executor_t*,rcl_timer_t*);
rcl_ret_t rclc_executor_set_timeout(rclc_executor_t*,uint64_t);
rcl_ret_t rclc_executor_prepare(rclc_executor_t*);
rcl_ret_t rclc_executor_spin_some(rclc_executor_t*,uint64_t);
rcl_ret_t rclc_executor_spin(rclc_executor_t*);
/** Explicit selected teardown for a still-live context after an early original
 * main return. Never call with an expired stack context; terminal Guest traps
 * require genuine Host instance drain. It attempts all retained owners once. */
rcl_ret_t uwvm_rclc_close_context(rcl_context_t*,rcl_ret_t,uint32_t);
uwvm_rclc_diagnostics const*uwvm_rclc_context_diagnostics(rcl_context_t const*);
#include "services.h"
#ifdef __cplusplus
}
#endif
#endif
