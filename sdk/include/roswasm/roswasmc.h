/** @file
 * @brief C11 capability, resource, and polling foundation for ROS ABI v1.
 *
 * Include ros1.h or ros2.h to obtain an explicit backend. The context is only
 * an import dispatch table: it creates no native node, thread, socket, endpoint,
 * permission, environment inheritance, or hidden retry. Every resource opens a
 * host-granted alias. Handles must not be copied between owners or instances.
 * This header requires only stdint.h/stddef.h and supports freestanding guests.
 */
#ifndef ROSWASMC_H
#define ROSWASMC_H
#include "../../common/abi.h"
#ifdef __cplusplus
extern "C" {
#endif
/** @brief Canonical request operations. Missing backend entries return error 95. */
typedef enum roswasmc_operation {
    ROSWASMC_SERVICE_CALL, /**< Service call; requires its own alias. */
    ROSWASMC_SERVICE_RESPOND, /**< Service respond; requires its own alias. */
    ROSWASMC_ACTION_SEND_GOAL, /**< Action send goal; requires its own alias. */
    ROSWASMC_ACTION_CANCEL_OWN_GOAL, /**< Action cancel own goal; requires its own alias. */
    ROSWASMC_ACTION_CANCEL_ANY_GOAL, /**< Action cancel any goal; requires its own alias. */
    ROSWASMC_ACTION_RECEIVE_FEEDBACK, /**< Action receive feedback; requires its own alias. */
    ROSWASMC_ACTION_RECEIVE_RESULT, /**< Action receive result; requires its own alias. */
    ROSWASMC_ACTION_ADVERTISE, /**< Action advertise; requires its own alias. */
    ROSWASMC_ACTION_RESPOND, /**< Action respond; requires its own alias. */
    ROSWASMC_TF_LOOKUP, /**< Tf lookup; requires its own alias. */
    ROSWASMC_TF_BROADCAST, /**< Tf broadcast; requires its own alias. */
    ROSWASMC_PARAMETER_GET, /**< Parameter get; requires its own alias. */
    ROSWASMC_PARAMETER_SET, /**< Parameter set; requires its own alias. */
    ROSWASMC_PARAMETER_DELETE, /**< Parameter delete; requires its own alias. */
    ROSWASMC_PARAMETER_EXISTS, /**< Parameter exists; requires its own alias. */
    ROSWASMC_PARAMETER_LIST, /**< Parameter list; requires its own alias. */
    ROSWASMC_PARAMETER_SEARCH, /**< Parameter search; requires its own alias. */
    ROSWASMC_PARAMETER_DESCRIBE, /**< Parameter describe; requires its own alias. */
    ROSWASMC_PARAMETER_DECLARE, /**< Parameter declare; requires its own alias. */
    ROSWASMC_PARAMETER_UNDECLARE, /**< Parameter undeclare; requires its own alias. */
    ROSWASMC_GRAPH_INSPECT_TOPIC, /**< Graph inspect topic; requires its own alias. */
    ROSWASMC_GRAPH_INSPECT_NODE, /**< Graph inspect node; requires its own alias. */
    ROSWASMC_GRAPH_INSPECT_SERVICE, /**< Graph inspect service; requires its own alias. */
    ROSWASMC_GRAPH_INSPECT_ACTION, /**< Graph inspect action; requires its own alias. */
    ROSWASMC_GRAPH_LIST_TOPICS, /**< Graph list topics; requires its own alias. */
    ROSWASMC_GRAPH_LIST_NODES, /**< Graph list nodes; requires its own alias. */
    ROSWASMC_GRAPH_LIST_SERVICES, /**< Graph list services; requires its own alias. */
    ROSWASMC_LIFECYCLE_GET_STATE, /**< Lifecycle get state; requires its own alias. */
    ROSWASMC_LIFECYCLE_CHANGE_STATE, /**< Lifecycle change state; requires its own alias. */
    ROSWASMC_TIME_READ_ROS, /**< Time read ros; requires its own alias. */
    ROSWASMC_TIME_READ_WALL, /**< Time read wall; requires its own alias. */
    ROSWASMC_TIME_READ_SYSTEM, /**< Time read system; requires its own alias. */
    ROSWASMC_TIME_READ_STEADY, /**< Time read steady; requires its own alias. */
    ROSWASMC_OPERATION_COUNT
} roswasmc_operation;
/** @brief Raw request/response import signature; all offsets/lengths are i64. */
typedef uint32_t (*roswasmc_request_import)(uint64_t,uint32_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);
/** @brief Immutable dispatch table. NULL operations are unsupported, never emulated. */
typedef struct roswasmc_backend {
    uint32_t version;
    uint32_t (*abi_version)(void);
    uint32_t (*open)(uint32_t,uint64_t,uint64_t,uint64_t);
    uint32_t (*close)(uint64_t);
    uint32_t (*revoke)(uint64_t);
    uint32_t (*publish)(uint64_t,uint32_t,uint64_t,uint64_t);
    uint32_t (*subscribe)(uint64_t);
    uint32_t (*poll)(uint64_t,uint32_t,uint64_t,uint64_t,uint64_t);
    uint32_t (*advertise_service)(uint64_t);
    uint32_t (*subscribe_parameter_events)(uint64_t);
    uint32_t (*log)(uint64_t,uint32_t,uint64_t,uint64_t);
    roswasmc_request_import request[ROSWASMC_OPERATION_COUNT];
} roswasmc_backend;
/** @brief Guest-local context. Its backend must outlive every dependent owner. */
typedef struct roswasmc_context { roswasmc_backend const* backend; } roswasmc_context;
/** @brief Unique logical owner. Initialize before use; copying does not duplicate ownership. */
typedef struct roswasmc_capability { roswasmc_context const* context; uint64_t handle; } roswasmc_capability;
/** @brief Result with an explicit output-length validity bit.
 * Error 90 alone does not mean length is valid. An event or RPC may already have
 * been consumed when output capacity was insufficient: never replay implicitly.
 */
typedef struct roswasmc_result { uint32_t error; uint64_t length; int has_length; } roswasmc_result;
/** @brief Create an empty owner without opening authority. */
static inline void roswasmc_capability_init(roswasmc_capability* cap,roswasmc_context const* context) {
    cap->context=context;cap->handle=0;
}
/** @brief Inspect local ownership without calling an import or gaining rights. */
static inline int roswasmc_is_open(roswasmc_capability const* cap) {
    return cap&&cap->context&&cap->context->backend&&cap->handle;
}
/** @brief Validate a request selector before a factory opens an alias.
 * Return22 for a malformed selector/context and95 for a missing backend entry.
 * Support is a dispatch-table property; success grants no native permission.
 */
static inline uint32_t roswasmc_check_operation(roswasmc_context const* context,roswasmc_operation op) {
    if(!context||!context->backend||(unsigned)op>=ROSWASMC_OPERATION_COUNT) return UWVM_ROS_INVALID;
    return context->backend->request[op]?UWVM_ROS_SUCCESS:UWVM_ROS_UNSUPPORTED;
}
/** @brief Open exactly one nonempty, explicitly granted alias.
 * @param alias Non-NUL-terminated bytes; names do not choose a ROS endpoint.
 * @param length Alias byte count. The host validates syntax and length (1..255).
 * @return Numeric ABI errno. A failed open preserves the previous empty owner.
 */
static inline uint32_t roswasmc_open(roswasmc_capability* cap,char const* alias,size_t length) {
    unsigned char slot[8]={0,0,0,0,0,0,0,0};uint32_t error;
    if(!cap||!cap->context||!cap->context->backend||!alias||!length||length>255||cap->handle) return UWVM_ROS_INVALID;
    if(!cap->context->backend->open) return UWVM_ROS_UNSUPPORTED;
    error=cap->context->backend->open(0,uwvm_ros_address(alias),length,uwvm_ros_address(slot));
    if(!error) {
        uint64_t handle=uwvm_ros_load_u64(slot);if(!handle) return UWVM_ROS_INVALID;
        cap->handle=handle;
    }
    return error;
}
/** @brief Relinquish ownership after one close attempt.
 * The host may retire a handle before native cleanup returns 107/110. Therefore
 * this owner is cleared even on an import error; callers must not retry close
 * assuming an error preserved a live resource. No permission is restored.
 */
static inline uint32_t roswasmc_close(roswasmc_capability* cap) {
    uint32_t error;if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!cap->context->backend->close) {cap->handle=0;return UWVM_ROS_UNSUPPORTED;}
    error=cap->context->backend->close(cap->handle);cap->handle=0;return error;
}
/** @brief Revoke the owned alias, then relinquish this local owner.
 * Revocation retires existing handles. Reopening a revoked alias returns 125.
 */
static inline uint32_t roswasmc_revoke(roswasmc_capability* cap) {
    uint32_t error;if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!cap->context->backend->revoke) {cap->handle=0;return UWVM_ROS_UNSUPPORTED;}
    error=cap->context->backend->revoke(cap->handle);cap->handle=0;return error;
}
/** @brief Send canonical bytes through a topic.publish alias; never encode native structs directly. */
static inline uint32_t roswasmc_publish(roswasmc_capability const* cap,void const* bytes,size_t length) {
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!bytes&&length) return UWVM_ROS_INVALID;
    if(!cap->context->backend->publish) return UWVM_ROS_UNSUPPORTED;
    return cap->context->backend->publish(cap->handle,0,uwvm_ros_address(bytes),length);
}
/** @brief Activate a topic.subscribe alias. This does not grant publication. */
static inline uint32_t roswasmc_subscribe(roswasmc_capability const* cap) {
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!cap->context->backend->subscribe) return UWVM_ROS_UNSUPPORTED;
    return cap->context->backend->subscribe(cap->handle);
}
/** @brief Poll one event into a caller-owned bounded buffer. Empty queues return 11.
 * Callback invocation, if desired, occurs later on the calling guest thread.
 */
static inline roswasmc_result roswasmc_poll(roswasmc_capability const* cap,void* output,size_t capacity) {
    unsigned char slot[8]={255,255,255,255,255,255,255,255};roswasmc_result result={UWVM_ROS_BAD_HANDLE,0,0};uint64_t length;
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return result;
    if(!output&&capacity) {result.error=UWVM_ROS_INVALID;return result;}
    if(!cap->context->backend->poll) {result.error=UWVM_ROS_UNSUPPORTED;return result;}
    result.error=cap->context->backend->poll(cap->handle,0,uwvm_ros_address(output),capacity,uwvm_ros_address(slot));
    length=uwvm_ros_load_u64(slot);
    if(!result.error) {
        if(length==UINT64_MAX||length>capacity) {result.error=UWVM_ROS_INVALID;return result;}
        result.length=length;result.has_length=1;
    } else if(result.error==UWVM_ROS_TOO_LARGE&&length!=UINT64_MAX) {result.length=length;result.has_length=1;}
    return result;
}
/** @brief Perform exactly one canonical request operation, without retry.
 * @param op One named operation; authority still comes exclusively from cap.
 * @param input Request bytes; NULL is allowed only when input_length is zero.
 * @param output Caller-owned response storage; NULL is allowed when capacity is zero.
 * @return Error, length, and has_length. Do not access output after an error.
 */
static inline roswasmc_result roswasmc_request(roswasmc_capability const* cap,roswasmc_operation op,
    void const* input,size_t input_length,void* output,size_t capacity) {
    unsigned char slot[8]={255,255,255,255,255,255,255,255};roswasmc_result result={UWVM_ROS_BAD_HANDLE,0,0};uint64_t length;roswasmc_request_import fn;
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return result;
    if((unsigned)op>=ROSWASMC_OPERATION_COUNT||(!input&&input_length)||(!output&&capacity)) {result.error=UWVM_ROS_INVALID;return result;}
    fn=cap->context->backend->request[op];if(!fn) {result.error=UWVM_ROS_UNSUPPORTED;return result;}
    result.error=fn(cap->handle,0,uwvm_ros_address(input),input_length,uwvm_ros_address(output),capacity,uwvm_ros_address(slot));
    length=uwvm_ros_load_u64(slot);
    if(!result.error) {
        if(length==UINT64_MAX||length>capacity) {result.error=UWVM_ROS_INVALID;return result;}
        result.length=length;result.has_length=1;
    } else if(result.error==UWVM_ROS_TOO_LARGE&&length!=UINT64_MAX) {result.length=length;result.has_length=1;}
    return result;
}
/** @brief Activate a service.advertise queue. Replies need a separate service.respond owner. */
static inline uint32_t roswasmc_service_advertise(roswasmc_capability const* cap) {
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!cap->context->backend->advertise_service) return UWVM_ROS_UNSUPPORTED;
    return cap->context->backend->advertise_service(cap->handle);
}
/** @brief Activate a separately granted ROS2 parameter-event queue; ROS1 returns 95. */
static inline uint32_t roswasmc_parameter_subscribe_events(roswasmc_capability const* cap) {
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!cap->context->backend->subscribe_parameter_events) return UWVM_ROS_UNSUPPORTED;
    return cap->context->backend->subscribe_parameter_events(cap->handle);
}
/** @brief Write a canonical u32-length UTF-8 String through a log.write alias. */
static inline uint32_t roswasmc_log(roswasmc_capability const* cap,char const* bytes,size_t length) {
    if(!cap||!cap->context||!cap->context->backend||!cap->handle) return UWVM_ROS_BAD_HANDLE;
    if(!bytes&&length) return UWVM_ROS_INVALID;
    if(!cap->context->backend->log) return UWVM_ROS_UNSUPPORTED;
    return cap->context->backend->log(cap->handle,0,uwvm_ros_address(bytes),length);
}
/** @brief Guest-side callback. The borrowed bytes expire when the callback returns. */
typedef void (*roswasmc_event_callback)(void* user,unsigned char const* bytes,size_t length);
/** @brief One caller-owned executor registration; capability and storage must outlive spin_some. */
typedef struct roswasmc_poll_task {
    roswasmc_capability const* capability;unsigned char* buffer;size_t capacity;
    roswasmc_event_callback callback;void* user;
} roswasmc_poll_task;
/** @brief Poll registrations once, bounded by maximum_events; run callbacks on the guest caller.
 * Error 11 is normal idle work. Other errors stop this pass and remain visible.
 * @param delivered Receives the number of callbacks completed, including before a later error.
 * There is no host thread, busy spin loop, sleep, blocking wait, or authority expansion.
 */
static inline uint32_t roswasmc_spin_some(roswasmc_poll_task const* tasks,size_t count,size_t maximum_events,size_t* delivered) {
    size_t i,n=0;if(!delivered||(!tasks&&count)) return UWVM_ROS_INVALID;*delivered=0;
    for(i=0;i<count&&n<maximum_events;++i) {
        roswasmc_result result;if(!tasks[i].callback) return UWVM_ROS_INVALID;
        result=roswasmc_poll(tasks[i].capability,tasks[i].buffer,tasks[i].capacity);
        if(result.error==UWVM_ROS_AGAIN) continue;
        if(result.error) return result.error;
        if(!result.has_length||result.length>tasks[i].capacity) return UWVM_ROS_INVALID;
        tasks[i].callback(tasks[i].user,tasks[i].buffer,(size_t)result.length);*delivered=++n;
    }
    return UWVM_ROS_SUCCESS;
}
/** @brief Service call convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_service_call(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_SERVICE_CALL,input,input_length,output,capacity);
}
/** @brief Service respond convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_service_respond(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_SERVICE_RESPOND,input,input_length,output,capacity);
}
/** @brief Action send goal convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_send_goal(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_SEND_GOAL,input,input_length,output,capacity);
}
/** @brief Action cancel own goal convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_cancel_own_goal(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_CANCEL_OWN_GOAL,input,input_length,output,capacity);
}
/** @brief Action cancel any goal convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_cancel_any_goal(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_CANCEL_ANY_GOAL,input,input_length,output,capacity);
}
/** @brief Action receive feedback convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_receive_feedback(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_RECEIVE_FEEDBACK,input,input_length,output,capacity);
}
/** @brief Action receive result convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_receive_result(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_RECEIVE_RESULT,input,input_length,output,capacity);
}
/** @brief Action advertise convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_advertise(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_ADVERTISE,input,input_length,output,capacity);
}
/** @brief Action respond convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_action_respond(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_ACTION_RESPOND,input,input_length,output,capacity);
}
/** @brief Tf lookup convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_tf_lookup(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_TF_LOOKUP,input,input_length,output,capacity);
}
/** @brief Tf broadcast convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_tf_broadcast(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_TF_BROADCAST,input,input_length,output,capacity);
}
/** @brief Parameter get convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_get(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_GET,input,input_length,output,capacity);
}
/** @brief Parameter set convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_set(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_SET,input,input_length,output,capacity);
}
/** @brief Parameter delete convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_delete(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_DELETE,input,input_length,output,capacity);
}
/** @brief Parameter exists convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_exists(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_EXISTS,input,input_length,output,capacity);
}
/** @brief Parameter list convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_list(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_LIST,input,input_length,output,capacity);
}
/** @brief Parameter search convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_search(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_SEARCH,input,input_length,output,capacity);
}
/** @brief Parameter describe convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_describe(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_DESCRIBE,input,input_length,output,capacity);
}
/** @brief Parameter declare convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_declare(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_DECLARE,input,input_length,output,capacity);
}
/** @brief Parameter undeclare convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_parameter_undeclare(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_PARAMETER_UNDECLARE,input,input_length,output,capacity);
}
/** @brief Graph inspect topic convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_inspect_topic(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_INSPECT_TOPIC,input,input_length,output,capacity);
}
/** @brief Graph inspect node convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_inspect_node(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_INSPECT_NODE,input,input_length,output,capacity);
}
/** @brief Graph inspect service convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_inspect_service(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_INSPECT_SERVICE,input,input_length,output,capacity);
}
/** @brief Graph inspect action convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_inspect_action(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_INSPECT_ACTION,input,input_length,output,capacity);
}
/** @brief Graph list topics convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_list_topics(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_LIST_TOPICS,input,input_length,output,capacity);
}
/** @brief Graph list nodes convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_list_nodes(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_LIST_NODES,input,input_length,output,capacity);
}
/** @brief Graph list services convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_graph_list_services(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_GRAPH_LIST_SERVICES,input,input_length,output,capacity);
}
/** @brief Lifecycle get state convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_lifecycle_get_state(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_LIFECYCLE_GET_STATE,input,input_length,output,capacity);
}
/** @brief Lifecycle change state convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_lifecycle_change_state(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_LIFECYCLE_CHANGE_STATE,input,input_length,output,capacity);
}
/** @brief Time read ros convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_time_read_ros(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_TIME_READ_ROS,input,input_length,output,capacity);
}
/** @brief Time read wall convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_time_read_wall(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_TIME_READ_WALL,input,input_length,output,capacity);
}
/** @brief Time read system convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_time_read_system(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_TIME_READ_SYSTEM,input,input_length,output,capacity);
}
/** @brief Time read steady convenience entry; requires a separately granted matching alias.
 * The caller provides canonical bytes and bounded response storage. No replay occurs.
 */
static inline roswasmc_result roswasmc_time_read_steady(roswasmc_capability const* cap,void const* input,size_t input_length,void* output,size_t capacity) {
    return roswasmc_request(cap,ROSWASMC_TIME_READ_STEADY,input,input_length,output,capacity);
}
#ifdef __cplusplus
}
#endif
#endif
