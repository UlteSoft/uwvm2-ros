/** @file
 * @brief Concrete freestanding Wasm guest bridge for the JavaScript SDK.
 *
 * This module exports ABI-shaped forwarding functions and reserved scratch
 * storage. It does not implement ROS imports or native networking. An external
 * embedding must provide authenticated uwvm-compatible ROS imports and shared
 * exported memory; a generic permissive JS import object is not that policy.
 * Select exactly one backend using ROSWASM_BRIDGE_ROS_VERSION=1 or2.
 * Compile with --export-memory and --no-entry: JavaScript calls its exports.
 * There is no _start, file, socket, clock, thread or implicit capability open.
 */
#include <stdint.h>
#include <stddef.h>

#ifndef ROSWASM_BRIDGE_ROS_VERSION
#define ROSWASM_BRIDGE_ROS_VERSION 2
#endif
#if ROSWASM_BRIDGE_ROS_VERSION == 1
#include "../ros1/abi.h"
#define ROSWASM_CALL(name) uwvm_ros1_##name
#elif ROSWASM_BRIDGE_ROS_VERSION == 2
#include "../ros2/abi.h"
#define ROSWASM_CALL(name) uwvm_ros2_##name
#else
#error "ROSWASM_BRIDGE_ROS_VERSION must be 1 or 2"
#endif

#if defined(__wasm__)
#define ROSWASM_EXPORT(name) __attribute__((export_name(name)))
#else
#define ROSWASM_EXPORT(name)
#endif

#ifndef ROSWASM_BRIDGE_INPUT_CAPACITY
#define ROSWASM_BRIDGE_INPUT_CAPACITY 4096u
#endif
#ifndef ROSWASM_BRIDGE_OUTPUT_CAPACITY
#define ROSWASM_BRIDGE_OUTPUT_CAPACITY 4096u
#endif
_Static_assert(ROSWASM_BRIDGE_INPUT_CAPACITY > 0 && ROSWASM_BRIDGE_INPUT_CAPACITY <= 4193786u,"bounded input scratch required");
_Static_assert(ROSWASM_BRIDGE_OUTPUT_CAPACITY > 0 && ROSWASM_BRIDGE_OUTPUT_CAPACITY <= 4194304u,"bounded output scratch required");
static unsigned char roswasm_scratch[ROSWASM_BRIDGE_INPUT_CAPACITY + 8u + ROSWASM_BRIDGE_OUTPUT_CAPACITY];

/** @brief Return the memory-zero offset of the exclusively reserved scratch.
 * The JS Session lays out input, an eight-byte slot, then output in this region.
 * Only one serialized Session may own it; no Guest allocator uses this array.
 */
ROSWASM_EXPORT("roswasm_scratch_offset") uint64_t roswasm_scratch_offset(void) { return (uint64_t)(uintptr_t)roswasm_scratch; }
/** @brief Return the compiled, bounded input capacity; this grants no authority. */
ROSWASM_EXPORT("roswasm_input_capacity") uint32_t roswasm_input_capacity(void) { return ROSWASM_BRIDGE_INPUT_CAPACITY; }
/** @brief Return the compiled, bounded response capacity; never grow/replay implicitly. */
ROSWASM_EXPORT("roswasm_output_capacity") uint32_t roswasm_output_capacity(void) { return ROSWASM_BRIDGE_OUTPUT_CAPACITY; }

/** @brief Return ABI version 1. This does not open a capability or authorize a resource.
 * @return Version value1.
 */
ROSWASM_EXPORT("abi_version") uint32_t roswasm_forward_abi_version(void) {
    return ROSWASM_CALL(abi_version)();
}

/** @brief Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param alias_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param alias_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param handle_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("capability_open") uint32_t roswasm_forward_capability_open(uint32_t memory, uint64_t alias_offset, uint64_t alias_length, uint64_t handle_out_offset) {
    return ROSWASM_CALL(capability_open)(memory, alias_offset, alias_length, handle_out_offset);
}

/** @brief Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("capability_close") uint32_t roswasm_forward_capability_close(uint64_t handle) {
    return ROSWASM_CALL(capability_close)(handle);
}

/** @brief Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("capability_revoke") uint32_t roswasm_forward_capability_revoke(uint64_t handle) {
    return ROSWASM_CALL(capability_revoke)(handle);
}

/** @brief Publish one canonical generated message through a separately granted publisher alias.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param payload_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param payload_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("topic_publish") uint32_t roswasm_forward_topic_publish(uint64_t handle, uint32_t memory, uint64_t payload_offset, uint64_t payload_length) {
    return ROSWASM_CALL(topic_publish)(handle, memory, payload_offset, payload_length);
}

/** @brief Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("topic_subscribe") uint32_t roswasm_forward_topic_subscribe(uint64_t handle) {
    return ROSWASM_CALL(topic_subscribe)(handle);
}

/** @brief Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param output_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("event_poll") uint32_t roswasm_forward_event_poll(uint64_t handle, uint32_t memory, uint64_t output_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(event_poll)(handle, memory, output_offset, capacity, length_out_offset);
}

/** @brief Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("service_call") uint32_t roswasm_forward_service_call(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(service_call)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Activate the explicitly granted service server. Poll owned request IDs through event_poll.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("service_advertise") uint32_t roswasm_forward_service_advertise(uint64_t handle) {
    return ROSWASM_CALL(service_advertise)(handle);
}

/** @brief Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("service_respond") uint32_t roswasm_forward_service_respond(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(service_respond)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Send a canonical Fibonacci i32 order and receive an opaque u64 Goal ID. Order and byte limits remain controlled by the explicit grant.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_send_goal") uint32_t roswasm_forward_action_send_goal(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_send_goal)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_cancel_own_goal") uint32_t roswasm_forward_action_cancel_own_goal(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_cancel_own_goal)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_cancel_any_goal") uint32_t roswasm_forward_action_cancel_any_goal(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_cancel_any_goal)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Receive feedback for an owned Goal ID. Decode the ID/count/int32 envelope; an empty queue returns 11.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_receive_feedback") uint32_t roswasm_forward_action_receive_feedback(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_receive_feedback)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Receive a terminal result for an owned Goal ID. Decode ID/outcome/count/int32; terminal consumption retires ownership.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_receive_result") uint32_t roswasm_forward_action_receive_result(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_receive_result)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Activate the explicitly granted Action server with an empty request. Poll incoming ID/order events; responses require another grant.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_advertise") uint32_t roswasm_forward_action_advertise(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_advertise)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Send owned Goal feedback or terminal result using ID/kind/count/int32. Advertiser and responder must have the same explicit ROS2 QoS contract.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("action_respond") uint32_t roswasm_forward_action_respond(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(action_respond)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("tf_lookup") uint32_t roswasm_forward_tf_lookup(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(tf_lookup)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("tf_broadcast") uint32_t roswasm_forward_tf_broadcast(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(tf_broadcast)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_get") uint32_t roswasm_forward_parameter_get(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_get)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_set") uint32_t roswasm_forward_parameter_set(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_set)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

#if ROSWASM_BRIDGE_ROS_VERSION == 1
/** @brief Delete the explicitly granted ROS1 parameter using an empty request; no implicit parent-name authority is added.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_delete") uint32_t roswasm_forward_parameter_delete(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_delete)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 1
/** @brief Query existence of the explicitly granted ROS1 parameter; decode one canonical Bool response.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_exists") uint32_t roswasm_forward_parameter_exists(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_exists)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

/** @brief List the separately authorized parameter scope; decode the bounded canonical String result.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_list") uint32_t roswasm_forward_parameter_list(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_list)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

#if ROSWASM_BRIDGE_ROS_VERSION == 1
/** @brief Perform the implemented exact-name ROS1 search. This SDK does not add parent-namespace search support to the Runtime.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_search") uint32_t roswasm_forward_parameter_search(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_search)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

/** @brief Inspect the configured topic filter with an empty request and decode sorted canonical String rows.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_inspect_topic") uint32_t roswasm_forward_graph_inspect_topic(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_inspect_topic)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Inspect the configured node filter with an empty request and decode sorted canonical String rows.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_inspect_node") uint32_t roswasm_forward_graph_inspect_node(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_inspect_node)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Inspect the configured service filter with an empty request and decode sorted canonical String rows.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_inspect_service") uint32_t roswasm_forward_graph_inspect_service(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_inspect_service)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_list_topics") uint32_t roswasm_forward_graph_list_topics(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_list_topics)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief List nodes within the explicit graph filter; this is not unrestricted discovery authority.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_list_nodes") uint32_t roswasm_forward_graph_list_nodes(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_list_nodes)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief List services within the explicit graph filter; decode sorted canonical String rows.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_list_services") uint32_t roswasm_forward_graph_list_services(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_list_services)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

/** @brief Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("time_read_ros") uint32_t roswasm_forward_time_read_ros(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(time_read_ros)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}

#if ROSWASM_BRIDGE_ROS_VERSION == 1
/** @brief Read the independently authorized ROS1 wall clock; another time grant does not imply this one.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("time_read_wall") uint32_t roswasm_forward_time_read_wall(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(time_read_wall)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

/** @brief Write one canonical String to the separately granted ROS logger. This does not open a file or socket.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param payload_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param payload_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("log_write") uint32_t roswasm_forward_log_write(uint64_t handle, uint32_t memory, uint64_t payload_offset, uint64_t payload_length) {
    return ROSWASM_CALL(log_write)(handle, memory, payload_offset, payload_length);
}

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Read the separately authorized ROS2 parameter metadata as a canonical String.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_describe") uint32_t roswasm_forward_parameter_describe(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_describe)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Declare a typed parameter on the explicitly selected local ROS2 Gateway node. Remote declaration is unsupported.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_declare") uint32_t roswasm_forward_parameter_declare(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_declare)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Undeclare a typed parameter on the explicitly selected local ROS2 Gateway node. A missing remote operation is not emulated.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_undeclare") uint32_t roswasm_forward_parameter_undeclare(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(parameter_undeclare)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Inspect the separately configured ROS2 Action filter; ROS1 does not export this function.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("graph_inspect_action") uint32_t roswasm_forward_graph_inspect_action(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(graph_inspect_action)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Activate a separate ROS2 parameter-event projection grant. This takes only one handle, not the seven RPC arguments.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("parameter_subscribe_events") uint32_t roswasm_forward_parameter_subscribe_events(uint64_t handle) {
    return ROSWASM_CALL(parameter_subscribe_events)(handle);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Read the separately selected ROS2 lifecycle node state as u8 state ID followed by canonical String label.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("lifecycle_get_state") uint32_t roswasm_forward_lifecycle_get_state(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(lifecycle_get_state)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Apply the single ROS2 transition fixed by policy using an empty request; the guest cannot override that transition.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("lifecycle_change_state") uint32_t roswasm_forward_lifecycle_change_state(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(lifecycle_change_state)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Read the independently authorized ROS2 system clock as u64 nanoseconds.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("time_read_system") uint32_t roswasm_forward_time_read_system(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(time_read_system)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

#if ROSWASM_BRIDGE_ROS_VERSION == 2
/** @brief Read the independently authorized ROS2 steady clock as u64 nanoseconds.
 * @param handle Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param memory Exact i32 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param request_length Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param response_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param capacity Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @param length_out_offset Exact i64 ABI field; see docs/abi-v1.md for memory/ownership requirements.
 * @return Original Runtime error code; this forwarder performs no retries.
 */
ROSWASM_EXPORT("time_read_steady") uint32_t roswasm_forward_time_read_steady(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset) {
    return ROSWASM_CALL(time_read_steady)(handle, memory, request_offset, request_length, response_offset, capacity, length_out_offset);
}
#endif

/** @brief Bind separately granted event.observe and source owners; correlation is nonzero only for an owned outgoing Action goal. No aliases or rights are created.
 * Arguments retain exact ABI widths; selected guest memory is copied by Host.
 */
ROSWASM_EXPORT("event_watch") uint32_t roswasm_forward_event_watch(uint64_t observer_handle, uint64_t source_handle, uint64_t correlation) {return ROSWASM_CALL(event_watch)(observer_handle,source_handle,correlation);}

/** @brief Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.
 * Arguments retain exact ABI widths; selected guest memory is copied by Host.
 */
ROSWASM_EXPORT("event_poll_v1") uint32_t roswasm_forward_event_poll_v1(uint64_t handle, uint32_t memory, uint64_t output_offset, uint64_t capacity, uint64_t length_out_offset) {return ROSWASM_CALL(event_poll_v1)(handle,memory,output_offset,capacity,length_out_offset);}

/** @brief Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.
 * Arguments retain exact ABI widths; selected guest memory is copied by Host.
 */
ROSWASM_EXPORT("event_ready") uint32_t roswasm_forward_event_ready(uint64_t observer_handle, uint32_t memory, uint64_t output_offset) {return ROSWASM_CALL(event_ready)(observer_handle,memory,output_offset);}

/** @brief Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.
 * Arguments retain exact ABI widths; selected guest memory is copied by Host.
 */
ROSWASM_EXPORT("service_submit") uint32_t roswasm_forward_service_submit(uint64_t source_handle, uint64_t observer_handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t request_id_out_offset) {return ROSWASM_CALL(service_submit)(source_handle,observer_handle,memory,request_offset,request_length,request_id_out_offset);}

/* Keep forwarding macros available through every ABI extension, then avoid
 * leaking these implementation details into a translation unit such as relay.c.
 */
#undef ROSWASM_EXPORT
#undef ROSWASM_CALL
