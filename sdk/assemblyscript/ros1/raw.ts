/** Stable ABI v1 raw imports for ROS 1.
 * @module uwvm_ros1_v1
 * These declarations emit real Wasm imports; importing them grants no authority.
 * memory is i32. Every handle, offset, capacity and length is u64/i64, including wasm32.
 * Output handle/length slots need eight writable little-endian bytes.
 * Imports synchronously copy bytes; callers keep the referenced memory alive until return.
 * Error codes are returned as u32; abi_version returns version 1 rather than an error.
 * This raw layer permits explicit memory indices. Managed wrappers use memory 0.
 */

/** Return ABI version 1. Call before opening a capability.
 * @returns ABI version 1.
 */
@external("uwvm_ros1_v1", "abi_version")
export declare function abi_version(): u32;

/** Open a CLI-provisioned UTF-8 alias; on success write the opaque handle to an eight-byte slot.
 * @param memory Unsigned memory index.
 * @param alias_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param alias_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param handle_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "capability_open")
export declare function capability_open(memory: u32, alias_offset: u64, alias_length: u64, handle_out_offset: u64): u32;

/** Close this guest-owned handle. Never close a borrowed or forged handle.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "capability_close")
export declare function capability_close(handle: u64): u32;

/** Request revocation using existing authority; this operation cannot grant rights.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "capability_revoke")
export declare function capability_revoke(handle: u64): u32;

/** Copy a canonical message through a pre-opened topic.publish capability.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param payload_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param payload_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "topic_publish")
export declare function topic_publish(handle: u64, memory: u32, payload_offset: u64, payload_length: u64): u32;

/** Start subscription using a pre-opened topic.subscribe capability.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "topic_subscribe")
export declare function topic_subscribe(handle: u64): u32;

/** Poll one copied event without native callbacks. Code 11 means no event/rate limit; code 90 may write required length only when the UINT64_MAX slot sentinel is overwritten. Never automatically replay a poll.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param output_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "event_poll")
export declare function event_poll(handle: u64, memory: u32, output_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform service call. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "service_call")
export declare function service_call(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Advertise using a pre-opened service.advertise capability.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "service_advertise")
export declare function service_advertise(handle: u64): u32;

/** Perform service respond. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "service_respond")
export declare function service_respond(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action send goal. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_send_goal")
export declare function action_send_goal(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action cancel own goal. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_cancel_own_goal")
export declare function action_cancel_own_goal(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action cancel any goal. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_cancel_any_goal")
export declare function action_cancel_any_goal(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action receive feedback. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_receive_feedback")
export declare function action_receive_feedback(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action receive result. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_receive_result")
export declare function action_receive_result(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action advertise. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_advertise")
export declare function action_advertise(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform action respond. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "action_respond")
export declare function action_respond(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform tf lookup. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "tf_lookup")
export declare function tf_lookup(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform tf broadcast. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "tf_broadcast")
export declare function tf_broadcast(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform parameter get. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "parameter_get")
export declare function parameter_get(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform parameter set. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "parameter_set")
export declare function parameter_set(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform parameter delete. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "parameter_delete")
export declare function parameter_delete(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform parameter exists. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "parameter_exists")
export declare function parameter_exists(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform parameter list. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "parameter_list")
export declare function parameter_list(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform parameter search. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "parameter_search")
export declare function parameter_search(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform graph inspect topic. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "graph_inspect_topic")
export declare function graph_inspect_topic(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform graph inspect node. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "graph_inspect_node")
export declare function graph_inspect_node(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform graph inspect service. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "graph_inspect_service")
export declare function graph_inspect_service(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform graph list topics. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "graph_list_topics")
export declare function graph_list_topics(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform graph list nodes. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "graph_list_nodes")
export declare function graph_list_nodes(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform graph list services. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "graph_list_services")
export declare function graph_list_services(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform time read ros. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "time_read_ros")
export declare function time_read_ros(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Perform time read wall. Request/response schemas are defined in the SDK wire-format documentation; the capability fixes resource, type and limits.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param request_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param request_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @param response_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param capacity Unsigned 64-bit ABI value; do not narrow to i32.
 * @param length_out_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "time_read_wall")
export declare function time_read_wall(handle: u64, memory: u32, request_offset: u64, request_length: u64, response_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Copy a canonical length-prefixed UTF-8 log String through an explicit log.write capability.
 * @param handle Unsigned 64-bit ABI value; do not narrow to i32.
 * @param memory Unsigned memory index.
 * @param payload_offset Unsigned 64-bit ABI value; do not narrow to i32.
 * @param payload_length Unsigned 64-bit ABI value; do not narrow to i32.
 * @returns Runtime error code; zero is success. No automatic retry or authority escalation.
 */
@external("uwvm_ros1_v1", "log_write")
export declare function log_write(handle: u64, memory: u32, payload_offset: u64, payload_length: u64): u32;

/** Bind separately granted event.observe and source owners; correlation is nonzero only for an owned outgoing Action goal. No aliases or rights are created.
 * Explicit memory indices and64-bit addresses; no native callback enters Wasm.
 */
@external("uwvm_ros1_v1", "event_watch")
export declare function event_watch(observer_handle: u64, source_handle: u64, correlation: u64): u32;

/** Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.
 * Explicit memory indices and64-bit addresses; no native callback enters Wasm.
 */
@external("uwvm_ros1_v1", "event_poll_v1")
export declare function event_poll_v1(handle: u64, memory: u32, output_offset: u64, capacity: u64, length_out_offset: u64): u32;

/** Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.
 * Explicit memory indices and64-bit addresses; no native callback enters Wasm.
 */
@external("uwvm_ros1_v1", "event_ready")
export declare function event_ready(observer_handle: u64, memory: u32, output_offset: u64): u32;

/** Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.
 * Explicit memory indices and64-bit addresses; no native callback enters Wasm.
 */
@external("uwvm_ros1_v1", "service_submit")
export declare function service_submit(source_handle: u64, observer_handle: u64, memory: u32, request_offset: u64, request_length: u64, request_id_out_offset: u64): u32;

