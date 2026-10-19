/**
 * @file relay_protocol.h
 * @brief Closed binary relay protocol and exact selected ROS ABI v1 dispatch metadata.
 *
 * This header contains no policy, credentials, native ROS implementation or caller
 * identity. Only the real imports of the selected Wasm command perform authorization.
 * Integer fields on the pipe are little endian; serialize them explicitly rather
 * than writing a C struct. The command owns memory0 and exclusively reserved scratch.
 * All indices/types below come from sdk/abi/abi-v1.json. Regenerate only together
 * with the JS metadata and verify the actual compiled import signatures separately.
 */
#ifndef UWVM_JS_RELAY_PROTOCOL_H
#define UWVM_JS_RELAY_PROTOCOL_H
#include <stdint.h>

#define UWVM_JS_RELAY_MAGIC UINT32_C(0x524a5755) /* Bytes U W J R. */
#define UWVM_JS_RELAY_VERSION 1u
#define UWVM_JS_RELAY_HEADER_BYTES 24u
#define UWVM_JS_RELAY_GUARD_BYTES 16u
#define UWVM_JS_RELAY_MAX_ARGUMENTS 7u
#define UWVM_JS_RELAY_MAX_INTERRUPTS 16u
#define UWVM_JS_RELAY_NO_FUNCTION 65535u
#define UWVM_JS_RELAY_HELLO_BYTES 80u

/** Closed message kinds. FATAL never carries a Runtime status or success. */
enum uwvm_js_relay_kind { UWVM_JS_HELLO=1, UWVM_JS_CALL=2, UWVM_JS_RESULT=3, UWVM_JS_SHUTDOWN=4, UWVM_JS_FATAL=5 };
/** Original ABI forms; they determine exact argument/transfer/slot semantics. */
enum uwvm_js_relay_form { UWVM_JS_VERSION, UWVM_JS_OPEN, UWVM_JS_HANDLE, UWVM_JS_WRITE, UWVM_JS_POLL, UWVM_JS_RPC, UWVM_JS_WATCH, UWVM_JS_READY, UWVM_JS_SUBMIT };
/** Fixed public transport failures; no dynamic payload/name/path is printed. */
enum uwvm_js_relay_failure { UWVM_JS_BAD_HEADER=1, UWVM_JS_BAD_BODY, UWVM_JS_INPUT_IO, UWVM_JS_OUTPUT_IO, UWVM_JS_GUARD, UWVM_JS_ABI, UWVM_JS_SEQUENCE };

#if ROSWASM_BRIDGE_ROS_VERSION == 1
#define UWVM_JS_RELAY_FUNCTION_COUNT 38u
/** Typed-table compatibility SHA256, not authority or native identity. */
static unsigned char const uwvm_js_relay_table_digest[32] = {0x83, 0xe5, 0x5c, 0x0a, 0xef, 0xe3, 0x0a, 0xa6, 0x62, 0x54, 0x47, 0xe9, 0x5f, 0x88, 0xa2, 0xff, 0xad, 0x60, 0x1a, 0x68, 0x69, 0x24, 0x19, 0x60, 0x90, 0xdf, 0x4d, 0xf3, 0x8a, 0x56, 0x17, 0xdd};
/** X(index,name,form,arity,i64mask), exact authoritative schema order. */
#define UWVM_JS_RELAY_ABI_ROWS(X) \
    X(0, abi_version, VERSION, 0, 0u) \
    X(1, capability_open, OPEN, 4, 14u) \
    X(2, capability_close, HANDLE, 1, 1u) \
    X(3, capability_revoke, HANDLE, 1, 1u) \
    X(4, topic_publish, WRITE, 4, 13u) \
    X(5, topic_subscribe, HANDLE, 1, 1u) \
    X(6, event_poll, POLL, 5, 29u) \
    X(7, event_watch, WATCH, 3, 7u) \
    X(8, event_poll_v1, POLL, 5, 29u) \
    X(9, event_ready, READY, 3, 5u) \
    X(10, service_submit, SUBMIT, 6, 59u) \
    X(11, service_call, RPC, 7, 125u) \
    X(12, service_advertise, HANDLE, 1, 1u) \
    X(13, service_respond, RPC, 7, 125u) \
    X(14, action_send_goal, RPC, 7, 125u) \
    X(15, action_cancel_own_goal, RPC, 7, 125u) \
    X(16, action_cancel_any_goal, RPC, 7, 125u) \
    X(17, action_receive_feedback, RPC, 7, 125u) \
    X(18, action_receive_result, RPC, 7, 125u) \
    X(19, action_advertise, RPC, 7, 125u) \
    X(20, action_respond, RPC, 7, 125u) \
    X(21, tf_lookup, RPC, 7, 125u) \
    X(22, tf_broadcast, RPC, 7, 125u) \
    X(23, parameter_get, RPC, 7, 125u) \
    X(24, parameter_set, RPC, 7, 125u) \
    X(25, parameter_delete, RPC, 7, 125u) \
    X(26, parameter_exists, RPC, 7, 125u) \
    X(27, parameter_list, RPC, 7, 125u) \
    X(28, parameter_search, RPC, 7, 125u) \
    X(29, graph_inspect_topic, RPC, 7, 125u) \
    X(30, graph_inspect_node, RPC, 7, 125u) \
    X(31, graph_inspect_service, RPC, 7, 125u) \
    X(32, graph_list_topics, RPC, 7, 125u) \
    X(33, graph_list_nodes, RPC, 7, 125u) \
    X(34, graph_list_services, RPC, 7, 125u) \
    X(35, time_read_ros, RPC, 7, 125u) \
    X(36, time_read_wall, RPC, 7, 125u) \
    X(37, log_write, WRITE, 4, 13u)
#elif ROSWASM_BRIDGE_ROS_VERSION == 2
#define UWVM_JS_RELAY_FUNCTION_COUNT 43u
/** Typed-table compatibility SHA256, not authority or native identity. */
static unsigned char const uwvm_js_relay_table_digest[32] = {0xf1, 0xf2, 0xef, 0x80, 0xfe, 0xe1, 0xa4, 0x0b, 0x3d, 0xe7, 0xda, 0xa5, 0x84, 0xd4, 0x97, 0x43, 0x1e, 0xf0, 0x43, 0x9f, 0x3b, 0xdd, 0x35, 0x53, 0xa2, 0xc3, 0x84, 0xc6, 0x91, 0x39, 0xec, 0x68};
/** X(index,name,form,arity,i64mask), exact authoritative schema order. */
#define UWVM_JS_RELAY_ABI_ROWS(X) \
    X(0, abi_version, VERSION, 0, 0u) \
    X(1, capability_open, OPEN, 4, 14u) \
    X(2, capability_close, HANDLE, 1, 1u) \
    X(3, capability_revoke, HANDLE, 1, 1u) \
    X(4, topic_publish, WRITE, 4, 13u) \
    X(5, topic_subscribe, HANDLE, 1, 1u) \
    X(6, event_poll, POLL, 5, 29u) \
    X(7, event_watch, WATCH, 3, 7u) \
    X(8, event_poll_v1, POLL, 5, 29u) \
    X(9, event_ready, READY, 3, 5u) \
    X(10, service_submit, SUBMIT, 6, 59u) \
    X(11, service_call, RPC, 7, 125u) \
    X(12, service_advertise, HANDLE, 1, 1u) \
    X(13, service_respond, RPC, 7, 125u) \
    X(14, action_send_goal, RPC, 7, 125u) \
    X(15, action_cancel_own_goal, RPC, 7, 125u) \
    X(16, action_cancel_any_goal, RPC, 7, 125u) \
    X(17, action_receive_feedback, RPC, 7, 125u) \
    X(18, action_receive_result, RPC, 7, 125u) \
    X(19, action_advertise, RPC, 7, 125u) \
    X(20, action_respond, RPC, 7, 125u) \
    X(21, tf_lookup, RPC, 7, 125u) \
    X(22, tf_broadcast, RPC, 7, 125u) \
    X(23, parameter_get, RPC, 7, 125u) \
    X(24, parameter_set, RPC, 7, 125u) \
    X(25, parameter_list, RPC, 7, 125u) \
    X(26, parameter_describe, RPC, 7, 125u) \
    X(27, parameter_declare, RPC, 7, 125u) \
    X(28, parameter_undeclare, RPC, 7, 125u) \
    X(29, graph_inspect_topic, RPC, 7, 125u) \
    X(30, graph_inspect_node, RPC, 7, 125u) \
    X(31, graph_inspect_service, RPC, 7, 125u) \
    X(32, graph_inspect_action, RPC, 7, 125u) \
    X(33, parameter_subscribe_events, HANDLE, 1, 1u) \
    X(34, graph_list_topics, RPC, 7, 125u) \
    X(35, graph_list_nodes, RPC, 7, 125u) \
    X(36, graph_list_services, RPC, 7, 125u) \
    X(37, lifecycle_get_state, RPC, 7, 125u) \
    X(38, lifecycle_change_state, RPC, 7, 125u) \
    X(39, time_read_ros, RPC, 7, 125u) \
    X(40, time_read_system, RPC, 7, 125u) \
    X(41, time_read_steady, RPC, 7, 125u) \
    X(42, log_write, WRITE, 4, 13u)
#else
#error "Select exactly ROSWASM_BRIDGE_ROS_VERSION=1 or2 for the command relay."
#endif

/** Inspect only metadata for this selected table; unsupported indices never dispatch. */
struct uwvm_js_relay_entry { unsigned char form, arity, i64_mask; };
#define UWVM_JS_METADATA(index,name,form,arity,mask) { UWVM_JS_##form, arity, mask },
static struct uwvm_js_relay_entry const uwvm_js_relay_entries[UWVM_JS_RELAY_FUNCTION_COUNT] = { UWVM_JS_RELAY_ABI_ROWS(UWVM_JS_METADATA) };
#undef UWVM_JS_METADATA
#endif /* UWVM_JS_RELAY_PROTOCOL_H */
