/** @file
 * @brief Allocation-free C Node-style construction and typed resource helpers.
 *
 * A node is a guest-local context, not native node identity or permission. It
 * must outlive the owners created from it. Every helper opens only the alias
 * explicitly provided by the application; host policy chooses all endpoints.
 * Ownership is explicit: close each initialized owner once, including on exit.
 */
#ifndef ROSWASMC_NODE_H
#define ROSWASMC_NODE_H
#include "codec.h"
#include "session.h"
/** @brief Stable context and a borrowed diagnostic label; moving a live node invalidates its owners. */
typedef struct roswasmc_node {roswasmc_context context;roswasmc_string label;} roswasmc_node;
/** @brief Initialize a local node with an explicitly selected immutable backend; no imports are called. */
static inline void roswasmc_node_init(roswasmc_node* node,roswasmc_backend const* backend,roswasmc_string label) {
    node->context.backend=backend;node->label=label;
}
/** @brief Initialize a fresh/retired owner and open one exact alias.
 * Never pass an already-live owner: replacing its context would lose ownership.
 * Close the result after successful use. The stable node must outlive the owner.
 */
static inline uint32_t roswasmc_node_open(roswasmc_node const* node,roswasmc_capability* cap,roswasmc_string alias) {
    if(!node||!cap) return UWVM_ROS_INVALID;roswasmc_capability_init(cap,&node->context);return roswasmc_open(cap,alias.data,alias.size);
}
/** @brief Create one publication owner. This opens no subscription or arbitrary topic. */
static inline uint32_t roswasmc_node_create_publisher(roswasmc_node const* node,roswasmc_capability* cap,roswasmc_string alias) {
    return roswasmc_node_open(node,cap,alias);
}
/** @brief Transactionally open/activate a queue with independent cleanup status.
 * A failed activation immediately retires its acquired owner. The setup code is
 * returned unchanged; optional cleanup reports a distinct close failure. No
 * activation or close is retried after timeout/transport failure.
 */
static inline uint32_t roswasmc_node_create_subscription_checked(roswasmc_node const* node,
    roswasmc_capability* cap,roswasmc_string alias,roswasmc_cleanup_report* cleanup) {
    roswasmc_cleanup_report report=roswasmc_cleanup_init(0);uint32_t error;
    if(node&&cap) roswasmc_capability_init(cap,&node->context);
    if(!node||!cap||!node->context.backend) error=UWVM_ROS_INVALID;
    else if(!node->context.backend->subscribe||!node->context.backend->poll) error=UWVM_ROS_UNSUPPORTED;
    else {error=roswasmc_node_open(node,cap,alias);if(!error) {error=roswasmc_subscribe(cap);if(error) roswasmc_cleanup_owner(cap,&report);}}
    report.primary_error=error;if(cleanup) *cleanup=report;return error;
}
/** @brief Open/activate a subscription, immediately rolling back activation failure.
 * This convenience returns the primary error. Use the checked overload above
 * when the separate rollback-close result must be inspected by the application.
 */
static inline uint32_t roswasmc_node_create_subscription(roswasmc_node const* node,roswasmc_capability* cap,roswasmc_string alias) {
    return roswasmc_node_create_subscription_checked(node,cap,alias,0);
}
/** @brief Transactionally create a ROS2 parameter-event queue; ROS1 opens nothing.
 * On error, cleanup retains any separate attempted-close failure. A registered
 * import does not grant an event capability; scalar parameter grants do not
 * satisfy this factory. It never acquires a clock or arbitrary native callback.
 */
static inline uint32_t roswasmc_node_create_parameter_events_checked(roswasmc_node const* node,
    roswasmc_capability* cap,roswasmc_string alias,roswasmc_cleanup_report* cleanup) {
    roswasmc_cleanup_report report=roswasmc_cleanup_init(0);uint32_t error;
    if(node&&cap) roswasmc_capability_init(cap,&node->context);
    if(!node||!cap||!node->context.backend) error=UWVM_ROS_INVALID;
    else if(!node->context.backend->subscribe_parameter_events||!node->context.backend->poll) error=UWVM_ROS_UNSUPPORTED;
    else {error=roswasmc_node_open(node,cap,alias);if(!error) {error=roswasmc_parameter_subscribe_events(cap);if(error) roswasmc_cleanup_owner(cap,&report);}}
    report.primary_error=error;if(cleanup) *cleanup=report;return error;
}
/** @brief Create a ROS2 event queue with primary-error convenience semantics. */
static inline uint32_t roswasmc_node_create_parameter_events(roswasmc_node const* node,roswasmc_capability* cap,roswasmc_string alias) {
    return roswasmc_node_create_parameter_events_checked(node,cap,alias,0);
}
/** @brief Call Trigger once; the decoded message borrows caller response storage. */
static inline uint32_t roswasmc_trigger_call(roswasmc_capability const* client,void* response_bytes,size_t capacity,roswasmc_service_response* response) {
    roswasmc_result result=roswasmc_service_call(client,0,0,response_bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_service_response(response_bytes,(size_t)result.length,response);
}
/** @brief Call SetBool once. Repeating after timeout could duplicate a native side effect. */
static inline uint32_t roswasmc_set_bool_call(roswasmc_capability const* client,int value,void* response_bytes,size_t capacity,roswasmc_service_response* response) {
    unsigned char input[1];size_t length;roswasmc_result result;uint32_t error=roswasmc_encode_bool(value,input,1,&length);if(error) return error;
    result=roswasmc_service_call(client,input,length,response_bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_service_response(response_bytes,(size_t)result.length,response);
}
/** @brief Service request event: owned ID followed by canonical Trigger/SetBool request bytes. */
typedef struct roswasmc_service_request {uint64_t id;roswasmc_bytes payload;} roswasmc_service_request;
/** @brief Decode the owned service envelope; payload-specific decoder validates the chosen type. */
static inline uint32_t roswasmc_decode_service_request(void const* bytes,size_t length,roswasmc_service_request* value) {
    roswasmc_service_request next;if(!value||!bytes||length<8) return UWVM_ROS_INVALID;
    next.id=uwvm_ros_load_u64((unsigned char const*)bytes);if(!next.id) return UWVM_ROS_INVALID;
    next.payload.data=(unsigned char const*)bytes+8;next.payload.size=length-8;*value=next;return 0;
}
/** @brief Read one typed parameter through a matching get alias; result views borrow caller storage. */
static inline uint32_t roswasmc_parameter_read(roswasmc_capability const* cap,roswasmc_parameter_tag tag,void* bytes,size_t capacity,roswasmc_parameter_value* value) {
    roswasmc_result result=roswasmc_parameter_get(cap,0,0,bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_parameter(tag,bytes,(size_t)result.length,value);
}
/** @brief Set/declare a typed value once, with caller-owned canonical scratch storage.
 * @param operation Must be PARAMETER_SET or PARAMETER_DECLARE; each needs its own alias.
 * Empty response storage is sufficient only if this operation's successful response is empty.
 */
static inline uint32_t roswasmc_parameter_write(roswasmc_capability const* cap,roswasmc_operation operation,roswasmc_parameter_value const* value,void* scratch,size_t capacity,void* response,size_t response_capacity) {
    size_t length;uint32_t error;roswasmc_result result;if(operation!=ROSWASMC_PARAMETER_SET&&operation!=ROSWASMC_PARAMETER_DECLARE) return UWVM_ROS_INVALID;
    error=roswasmc_encode_parameter(value,scratch,capacity,&length);if(error) return error;
    result=roswasmc_request(cap,operation,scratch,length,response,response_capacity);return result.error;
}
/** @brief Inspect/list one host-filtered graph projection; response is a borrowed canonical String. */
static inline uint32_t roswasmc_graph_read(roswasmc_capability const* cap,roswasmc_operation operation,void* bytes,size_t capacity,roswasmc_string* text) {
    roswasmc_result result;if(operation<ROSWASMC_GRAPH_INSPECT_TOPIC||operation>ROSWASMC_GRAPH_LIST_SERVICES) return UWVM_ROS_INVALID;
    result=roswasmc_request(cap,operation,0,0,bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_string(bytes,(size_t)result.length,text);
}
/** @brief Lookup only the policy-granted frame pair; time zero means latest. */
static inline uint32_t roswasmc_tf_read(roswasmc_capability const* cap,uint64_t nanoseconds,void* bytes,size_t capacity,roswasmc_transform* value) {
    unsigned char request[8];roswasmc_writer out=roswasmc_writer_init(request,8);roswasmc_result result;
    roswasmc_write_integer(&out,nanoseconds,8);result=roswasmc_tf_lookup(cap,request,8,bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_transform(bytes,(size_t)result.length,value);
}
/** @brief Broadcast one complete Transform through a separate alias; output is canonically empty. */
static inline uint32_t roswasmc_tf_write(roswasmc_capability const* cap,roswasmc_transform const* value,void* scratch,size_t capacity) {
    size_t length;uint32_t error=roswasmc_encode_transform(value,scratch,capacity,&length);roswasmc_result result;if(error) return error;
    result=roswasmc_tf_broadcast(cap,scratch,length,0,0);return result.error;
}
/** @brief Read ROS2 Lifecycle GetState; label borrows caller response storage. */
static inline uint32_t roswasmc_lifecycle_read(roswasmc_capability const* cap,void* bytes,size_t capacity,roswasmc_lifecycle_state* state) {
    roswasmc_result result=roswasmc_lifecycle_get_state(cap,0,0,bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_lifecycle_state(bytes,(size_t)result.length,state);
}
/** @brief Request the one host-granted lifecycle transition; guest never chooses a transition ID. */
static inline uint32_t roswasmc_lifecycle_change(roswasmc_capability const* cap,int* accepted) {
    unsigned char response[1];roswasmc_result result=roswasmc_lifecycle_change_state(cap,0,0,response,1);if(result.error) return result.error;
    if(!result.has_length||result.length!=1) return UWVM_ROS_INVALID;return roswasmc_decode_bool(response,1,accepted);
}
/** @brief Cancel a goal with an explicitly chosen own/any cancellation alias. */
static inline uint32_t roswasmc_action_cancel(roswasmc_capability const* cap,roswasmc_operation operation,uint64_t goal,int* accepted) {
    unsigned char request[8],response[1];roswasmc_writer out=roswasmc_writer_init(request,8);roswasmc_result result;
    if(operation!=ROSWASMC_ACTION_CANCEL_OWN_GOAL&&operation!=ROSWASMC_ACTION_CANCEL_ANY_GOAL) return UWVM_ROS_INVALID;
    roswasmc_write_integer(&out,goal,8);result=roswasmc_request(cap,operation,request,8,response,1);if(result.error) return result.error;
    if(!result.has_length||result.length!=1) return UWVM_ROS_INVALID;return roswasmc_decode_bool(response,1,accepted);
}
/** @brief Cancel all goals using a distinct action.cancel-any-goal alias.
 * The encoded goal ID is zero only for this independently granted operation.
 * A cancel-own owner fails the host operation check and cannot gain any authority.
 */
static inline uint32_t roswasmc_action_cancel_all(roswasmc_capability const* cap,int* accepted) {
    return roswasmc_action_cancel(cap,ROSWASMC_ACTION_CANCEL_ANY_GOAL,0,accepted);
}
/** @brief Receive feedback/result for one owned goal, without polling an unrelated goal. */
static inline uint32_t roswasmc_action_receive(roswasmc_capability const* cap,roswasmc_operation operation,uint64_t goal,void* bytes,size_t capacity,roswasmc_action_sequence* sequence) {
    unsigned char request[8];roswasmc_writer out=roswasmc_writer_init(request,8);roswasmc_result result;uint32_t error;roswasmc_action_sequence next;
    if(operation!=ROSWASMC_ACTION_RECEIVE_FEEDBACK&&operation!=ROSWASMC_ACTION_RECEIVE_RESULT) return UWVM_ROS_INVALID;
    roswasmc_write_integer(&out,goal,8);result=roswasmc_request(cap,operation,request,8,bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity||!sequence) return UWVM_ROS_INVALID;
    error=roswasmc_decode_action_sequence(bytes,(size_t)result.length,operation==ROSWASMC_ACTION_RECEIVE_RESULT,&next);
    if(error||next.goal!=goal||(operation==ROSWASMC_ACTION_RECEIVE_RESULT&&next.kind<2)) return UWVM_ROS_INVALID;*sequence=next;return 0;
}
/** @brief Action server goal event: nonzero owned ID followed by signed Fibonacci order bits. */
typedef struct roswasmc_fibonacci_goal {uint64_t id;int32_t order;} roswasmc_fibonacci_goal;
/** @brief Decode one complete twelve-byte advertised goal event. */
static inline uint32_t roswasmc_decode_fibonacci_goal(void const* bytes,size_t length,roswasmc_fibonacci_goal* goal) {
    roswasmc_reader in=roswasmc_reader_init(bytes,length);roswasmc_fibonacci_goal next;uint64_t order;uint32_t bits;
    if(!goal||!roswasmc_read_integer(&in,&next.id,8)||!next.id||!roswasmc_read_integer(&in,&order,4)||!roswasmc_reader_done(&in)) return UWVM_ROS_INVALID;
    bits=(uint32_t)order;roswasmc_copy_object(&next.order,&bits,4);*goal=next;return 0;
}
/** @brief Activate an Action advertisement; receive goal events through event_poll. */
static inline uint32_t roswasmc_action_start(roswasmc_capability const* cap) {return roswasmc_action_advertise(cap,0,0,0,0).error;}
/** @brief Send one feedback1 or terminalresult2..4 through a separate respond alias. */
static inline uint32_t roswasmc_action_reply(roswasmc_capability const* cap,roswasmc_action_sequence const* value,void* scratch,size_t capacity) {
    size_t length;uint32_t error=roswasmc_encode_action_sequence(value,1,scratch,capacity,&length);roswasmc_result result;if(error) return error;
    result=roswasmc_action_respond(cap,scratch,length,0,0);return result.error;
}
#include "resources.h"
#include "executor.h"
#endif
