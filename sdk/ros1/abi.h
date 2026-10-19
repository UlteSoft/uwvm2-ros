/** @file
 * @brief Complete ROS 1 ABI v1 imports for C11 and C++ guests.
 * Imports resolve from `uwvm_ros1_v1`; no native ROS library is linked.
 * Every operation needs its own host-granted alias. All offsets and lengths are
 * i64, memory indices are i32, and handles/length slots are eight-byte LE values.
 * Error 90 may leave output length unwritten; do not infer a required size from
 * that error alone. Do not automatically retry a consumed event or RPC.
 */
#ifndef UWVM_ROS1_ABI_H
#define UWVM_ROS1_ABI_H
#include "../common/abi.h"
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__wasm__)
# define UWVM_ROS1_IMPORT(name) __attribute__((import_module("uwvm_ros1_v1"),import_name(name)))
#else
# define UWVM_ROS1_IMPORT(name)
#endif
/** @brief Return ABI version 1 (a value, not an errno).
 */
UWVM_ROS1_IMPORT("abi_version") uint32_t uwvm_ros1_abi_version(void);
/** @brief Open one explicitly granted alias; write its owned handle only on success.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param alias_offset Offset of nonempty alias bytes (not NUL terminated).
 * @param alias_length Alias byte count, at most 255.
 * @param handle_out_offset Writable eight-byte little-endian handle slot offset.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("capability_open") uint32_t uwvm_ros1_capability_open(uint32_t memory, uint64_t alias_offset, uint64_t alias_length, uint64_t handle_out_offset);
/** @brief Retire this owned handle before native cleanup; a transport error can still mean retired.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("capability_close") uint32_t uwvm_ros1_capability_close(uint64_t handle);
/** @brief Revoke this alias for this instance and retire its existing handles.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("capability_revoke") uint32_t uwvm_ros1_capability_revoke(uint64_t handle);
/** @brief Publish one complete canonical message through a topic.publish alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param payload_offset Complete canonical message offset.
 * @param payload_length Canonical message byte count.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("topic_publish") uint32_t uwvm_ros1_topic_publish(uint64_t handle, uint32_t memory, uint64_t payload_offset, uint64_t payload_length);
/** @brief Activate a topic.subscribe queue; read it with event_poll.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("topic_subscribe") uint32_t uwvm_ros1_topic_subscribe(uint64_t handle);
/** @brief Copy one queued event; error 11 means currently empty. No native guest callback occurs.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param payload_out_offset Writable output buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("event_poll") uint32_t uwvm_ros1_event_poll(uint64_t handle, uint32_t memory, uint64_t payload_out_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Bind separately granted event.observe and source owners; correlation is nonzero only for an owned outgoing Action goal. No aliases or rights are created.
 * All i64 addresses reference the selected guest memory; only service_submit
 * writes its request slot on success. Close/revoke metadata needs its own mask.
 */
UWVM_ROS1_IMPORT("event_watch") uint32_t uwvm_ros1_event_watch(uint64_t observer_handle, uint64_t source_handle, uint64_t correlation);
/** @brief Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.
 * All i64 addresses reference the selected guest memory; only service_submit
 * writes its request slot on success. Close/revoke metadata needs its own mask.
 */
UWVM_ROS1_IMPORT("event_poll_v1") uint32_t uwvm_ros1_event_poll_v1(uint64_t handle, uint32_t memory, uint64_t output_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.
 * All i64 addresses reference the selected guest memory; only service_submit
 * writes its request slot on success. Close/revoke metadata needs its own mask.
 */
UWVM_ROS1_IMPORT("event_ready") uint32_t uwvm_ros1_event_ready(uint64_t observer_handle, uint32_t memory, uint64_t output_offset);
/** @brief Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.
 * All i64 addresses reference the selected guest memory; only service_submit
 * writes its request slot on success. Close/revoke metadata needs its own mask.
 */
UWVM_ROS1_IMPORT("service_submit") uint32_t uwvm_ros1_service_submit(uint64_t source_handle, uint64_t observer_handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t request_id_out_offset);
/** @brief Perform service call using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("service_call") uint32_t uwvm_ros1_service_call(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Activate a service.advertise queue; events carry an owned request ID.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("service_advertise") uint32_t uwvm_ros1_service_advertise(uint64_t handle);
/** @brief Perform service respond using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("service_respond") uint32_t uwvm_ros1_service_respond(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action send goal using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_send_goal") uint32_t uwvm_ros1_action_send_goal(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action cancel own goal using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_cancel_own_goal") uint32_t uwvm_ros1_action_cancel_own_goal(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action cancel any goal using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_cancel_any_goal") uint32_t uwvm_ros1_action_cancel_any_goal(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action receive feedback using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_receive_feedback") uint32_t uwvm_ros1_action_receive_feedback(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action receive result using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_receive_result") uint32_t uwvm_ros1_action_receive_result(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action advertise using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_advertise") uint32_t uwvm_ros1_action_advertise(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform action respond using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("action_respond") uint32_t uwvm_ros1_action_respond(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform tf lookup using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("tf_lookup") uint32_t uwvm_ros1_tf_lookup(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform tf broadcast using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("tf_broadcast") uint32_t uwvm_ros1_tf_broadcast(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform parameter get using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("parameter_get") uint32_t uwvm_ros1_parameter_get(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform parameter set using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("parameter_set") uint32_t uwvm_ros1_parameter_set(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform parameter delete using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("parameter_delete") uint32_t uwvm_ros1_parameter_delete(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform parameter exists using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("parameter_exists") uint32_t uwvm_ros1_parameter_exists(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform parameter list using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("parameter_list") uint32_t uwvm_ros1_parameter_list(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform parameter search using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("parameter_search") uint32_t uwvm_ros1_parameter_search(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform graph inspect topic using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("graph_inspect_topic") uint32_t uwvm_ros1_graph_inspect_topic(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform graph inspect node using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("graph_inspect_node") uint32_t uwvm_ros1_graph_inspect_node(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform graph inspect service using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("graph_inspect_service") uint32_t uwvm_ros1_graph_inspect_service(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform graph list topics using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("graph_list_topics") uint32_t uwvm_ros1_graph_list_topics(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform graph list nodes using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("graph_list_nodes") uint32_t uwvm_ros1_graph_list_nodes(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform graph list services using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("graph_list_services") uint32_t uwvm_ros1_graph_list_services(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform time read ros using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("time_read_ros") uint32_t uwvm_ros1_time_read_ros(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Perform time read wall using its own explicitly granted alias.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @param response_offset Writable response buffer offset.
 * @param capacity Explicit output byte capacity, subject to independent host quotas.
 * @param length_out_offset Writable eight-byte LE length slot; read only if actually written.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("time_read_wall") uint32_t uwvm_ros1_time_read_wall(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length, uint64_t response_offset, uint64_t capacity, uint64_t length_out_offset);
/** @brief Write UTF-8 bytes using the host-selected logger and severity.
 * @param handle Opaque handle owned by this instance and ROS backend.
 * @param memory Guest memory index; convenience wrappers use memory zero.
 * @param request_offset Canonical request offset; empty requests may use zero.
 * @param request_length Canonical request byte count.
 * @return Numeric ABI errno; zero is success. Unwritten output slots retain their previous bytes.
 */
UWVM_ROS1_IMPORT("log_write") uint32_t uwvm_ros1_log_write(uint64_t handle, uint32_t memory, uint64_t request_offset, uint64_t request_length);
#undef UWVM_ROS1_IMPORT
#ifdef __cplusplus
}
#endif
/** @brief Memory-zero open helper; caller buffers remain valid throughout the synchronous call. */
static inline uint32_t uwvm_ros1_open(char const* alias, uint64_t length, uwvm_ros_handle* handle) {
    unsigned char encoded[8]={255,255,255,255,255,255,255,255}; uint32_t e=uwvm_ros1_capability_open(0,uwvm_ros_address(alias),length,uwvm_ros_address(encoded));
    if(!e) *handle=uwvm_ros_load_u64(encoded); return e;
}
/** @brief Memory-zero publish helper; caller buffers remain valid throughout the synchronous call. */
static inline uint32_t uwvm_ros1_publish(uwvm_ros_handle handle, void const* bytes, uint64_t length) {
    return uwvm_ros1_topic_publish(handle,0,uwvm_ros_address(bytes),length);
}
/** @brief Memory-zero poll helper; caller buffers remain valid throughout the synchronous call. */
static inline uint32_t uwvm_ros1_poll(uwvm_ros_handle handle, void* bytes, uint64_t capacity, uint64_t* length) {
    unsigned char encoded[8]={255,255,255,255,255,255,255,255}; uint32_t e=uwvm_ros1_event_poll(handle,0,uwvm_ros_address(bytes),capacity,uwvm_ros_address(encoded));
    if(!e || (e==UWVM_ROS_TOO_LARGE && uwvm_ros_load_u64(encoded)!=UINT64_MAX)) *length=uwvm_ros_load_u64(encoded); return e;
}
#endif
