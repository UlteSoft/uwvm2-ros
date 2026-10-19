/** @file
 * @brief C Node factories and typed operations for every registered ABI resource.
 *
 * This layer organizes existing capability/codec calls. Resource names, message
 * types, QoS, rate/byte/inflight limits and synchronous deadlines belong to host
 * CLI grants. Passing a selector never changes those grants. Every RPC executes
 * once, including errors90/107/110; an admitted native effect may survive an
 * error, so callers must not blindly replay writes or grow-and-replay polls.
 *
 * Factory outputs are fresh/retired caller-owned capability objects. The node
 * and selected backend must outlive them. Use checked factories to inspect both
 * the primary setup error and independent rollback errors.
 */
#ifndef ROSWASMC_RESOURCES_H
#define ROSWASMC_RESOURCES_H
#include "node.h"
#include "action_bundle.h"
/** @brief Construct an explicitly named four-role Action group over a stable
 * Node, using caller-owned bundle/goal storage. Optional any-cancel is separate
 * authority. The node/backend must outlive every bundle operation and cleanup.
 */
static inline uint32_t roswasmc_node_create_action_bundle(roswasmc_node const* node,
    roswasmc_action_bundle* bundle,roswasmc_action_aliases const* aliases,uint64_t* goals,size_t capacity) {
    return roswasmc_action_bundle_init(node?&node->context:0,bundle,aliases,goals,capacity);
}
/** @brief Open one named registered request role; support is checked before open.
 * The caller still needs a matching exact alias. A missing backend entry returns
 * unsupported95 with no open attempt, never a simulated functional success.
 */
static inline uint32_t roswasmc_node_open_for(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    uint32_t error;if(!node||!cap) return UWVM_ROS_INVALID;
    roswasmc_capability_init(cap,&node->context);
    error=roswasmc_check_operation(&node->context,operation);return error?error:roswasmc_node_open(node,cap,alias);
}
/** @brief Open exactly one service.call alias; bounded call storage stays caller-owned. */
static inline uint32_t roswasmc_node_create_client(roswasmc_node const* node,roswasmc_capability* cap,roswasmc_string alias) {
    return roswasmc_node_open_for(node,cap,alias,ROSWASMC_SERVICE_CALL);
}
/** @brief Close both server owners in reverse order while retaining primary failure.
 * Optional statuses[0]/[1] correspond to advertise/respond respectively. Every
 * live owner is attempted once even when the first close returns107/110.
 */
static inline roswasmc_cleanup_report roswasmc_server_close(roswasmc_capability* advertise,
    roswasmc_capability* respond,uint32_t primary_error,uint32_t* statuses) {
    roswasmc_cleanup_report result=roswasmc_cleanup_init(primary_error);
    uint32_t response_code=roswasmc_cleanup_owner(respond,&result);
    uint32_t advertise_code=roswasmc_cleanup_owner(advertise,&result);
    if(statuses) {statuses[0]=advertise_code;statuses[1]=response_code;}return result;
}
/** @brief Compare two alias byte strings without assuming NUL termination. */
static inline int roswasmc_alias_equal(roswasmc_string left,roswasmc_string right) {
    size_t index;if(left.size!=right.size||!left.data||!right.data) return 0;
    for(index=0;index<left.size;++index) if(left.data[index]!=right.data[index]) return 0;return 1;
}
/** @brief Acquire advertise/respond independently before activating a service.
 * Both outputs must be distinct fresh objects and both aliases distinct. If
 * either open/activation fails, every acquired owner closes immediately and
 * report preserves the primary code plus independent first cleanup code/count.
 * No advertiser is activated before response authority has been acquired.
 */
static inline uint32_t roswasmc_node_create_service_checked(roswasmc_node const* node,
    roswasmc_capability* advertise,roswasmc_capability* respond,roswasmc_string advertise_alias,
    roswasmc_string respond_alias,roswasmc_cleanup_report* cleanup) {
    uint32_t error;roswasmc_cleanup_report report=roswasmc_cleanup_init(0);
    if(node&&advertise&&respond&&advertise!=respond) {roswasmc_capability_init(advertise,&node->context);roswasmc_capability_init(respond,&node->context);}
    if(!node||!advertise||!respond||advertise==respond||!node->context.backend||
        !advertise_alias.data||!respond_alias.data||!advertise_alias.size||!respond_alias.size||
        advertise_alias.size>255||respond_alias.size>255||roswasmc_alias_equal(advertise_alias,respond_alias)) error=UWVM_ROS_INVALID;
    else if(!node->context.backend->advertise_service||!node->context.backend->poll) error=UWVM_ROS_UNSUPPORTED;
    else if((error=roswasmc_check_operation(&node->context,ROSWASMC_SERVICE_RESPOND))==0) {
        roswasmc_capability_init(advertise,&node->context);roswasmc_capability_init(respond,&node->context);
        error=roswasmc_open(advertise,advertise_alias.data,advertise_alias.size);
        if(!error) error=roswasmc_open(respond,respond_alias.data,respond_alias.size);
        if(!error) error=roswasmc_service_advertise(advertise);
        if(error) report=roswasmc_server_close(advertise,respond,error,0);
    }
    report.primary_error=error;if(cleanup) *cleanup=report;return error;
}
/** @brief Open one exact action role. No other cancellation/read role is implied. */
static inline uint32_t roswasmc_node_create_action_client(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    if(operation<ROSWASMC_ACTION_SEND_GOAL||operation>ROSWASMC_ACTION_RECEIVE_RESULT) return UWVM_ROS_INVALID;
    return roswasmc_node_open_for(node,cap,alias,operation);
}
/** @brief Acquire Action advertise/respond independently before activating goals.
 * The empty advertisement request needs registered action.advertise and poll.
 * This does not grant send-goal, cancellation, clocks or automatic responses.
 */
static inline uint32_t roswasmc_node_create_action_server_checked(roswasmc_node const* node,
    roswasmc_capability* advertise,roswasmc_capability* respond,roswasmc_string advertise_alias,
    roswasmc_string respond_alias,roswasmc_cleanup_report* cleanup) {
    uint32_t error;roswasmc_cleanup_report report=roswasmc_cleanup_init(0);
    if(node&&advertise&&respond&&advertise!=respond) {roswasmc_capability_init(advertise,&node->context);roswasmc_capability_init(respond,&node->context);}
    if(!node||!advertise||!respond||advertise==respond||!node->context.backend||
        !advertise_alias.data||!respond_alias.data||!advertise_alias.size||!respond_alias.size||
        advertise_alias.size>255||respond_alias.size>255||roswasmc_alias_equal(advertise_alias,respond_alias)) error=UWVM_ROS_INVALID;
    else if(!node->context.backend->poll) error=UWVM_ROS_UNSUPPORTED;
    else if((error=roswasmc_check_operation(&node->context,ROSWASMC_ACTION_ADVERTISE))==0&&
        (error=roswasmc_check_operation(&node->context,ROSWASMC_ACTION_RESPOND))==0) {
        roswasmc_capability_init(advertise,&node->context);roswasmc_capability_init(respond,&node->context);
        error=roswasmc_open(advertise,advertise_alias.data,advertise_alias.size);
        if(!error) error=roswasmc_open(respond,respond_alias.data,respond_alias.size);
        if(!error) error=roswasmc_action_start(advertise);
        if(error) report=roswasmc_server_close(advertise,respond,error,0);
    }
    report.primary_error=error;if(cleanup) *cleanup=report;return error;
}
/** @brief Open one scalar/array/metadata parameter role. Events use their dedicated factory. */
static inline uint32_t roswasmc_node_create_parameters(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    if(operation<ROSWASMC_PARAMETER_GET||operation>ROSWASMC_PARAMETER_UNDECLARE) return UWVM_ROS_INVALID;
    return roswasmc_node_open_for(node,cap,alias,operation);
}
/** @brief Open one filtered graph inspect/list role; no unrestricted discovery is implied. */
static inline uint32_t roswasmc_node_create_graph(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    if(operation<ROSWASMC_GRAPH_INSPECT_TOPIC||operation>ROSWASMC_GRAPH_LIST_SERVICES) return UWVM_ROS_INVALID;
    return roswasmc_node_open_for(node,cap,alias,operation);
}
/** @brief Open one exact TF lookup/broadcast role; frames remain CLI-selected. */
static inline uint32_t roswasmc_node_create_tf(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    if(operation!=ROSWASMC_TF_LOOKUP&&operation!=ROSWASMC_TF_BROADCAST) return UWVM_ROS_INVALID;
    return roswasmc_node_open_for(node,cap,alias,operation);
}
/** @brief Open one ROS/platform clock role. This does not grant a WASI clock. */
static inline uint32_t roswasmc_node_create_clock(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    if(operation<ROSWASMC_TIME_READ_ROS||operation>ROSWASMC_TIME_READ_STEADY) return UWVM_ROS_INVALID;
    return roswasmc_node_open_for(node,cap,alias,operation);
}
/** @brief Open one explicit log.write severity/logger alias. */
static inline uint32_t roswasmc_node_create_logger(roswasmc_node const* node,roswasmc_capability* cap,roswasmc_string alias) {
    if(!node||!node->context.backend) return UWVM_ROS_INVALID;
    if(!node->context.backend->log) return UWVM_ROS_UNSUPPORTED;return roswasmc_node_open(node,cap,alias);
}
/** @brief Open one ROS2 get/change lifecycle role. ROS1 dispatch is unsupported95. */
static inline uint32_t roswasmc_node_create_lifecycle(roswasmc_node const* node,roswasmc_capability* cap,
    roswasmc_string alias,roswasmc_operation operation) {
    if(operation!=ROSWASMC_LIFECYCLE_GET_STATE&&operation!=ROSWASMC_LIFECYCLE_CHANGE_STATE) return UWVM_ROS_INVALID;
    return roswasmc_node_open_for(node,cap,alias,operation);
}
/** @brief Encode UTF-8 String into bounded scratch, then write one granted log record. */
static inline uint32_t roswasmc_logger_write(roswasmc_capability const* cap,roswasmc_string text,void* scratch,size_t capacity) {
    size_t length;uint32_t error=roswasmc_encode_string(text,scratch,capacity,&length);
    return error?error:roswasmc_log(cap,(char const*)scratch,length);
}
/** @brief Read exists metadata as one exact Bool under its own ROS1 grant. */
static inline uint32_t roswasmc_parameter_exists_value(roswasmc_capability const* cap,int* exists) {
    unsigned char bytes[1];roswasmc_result result;if(!exists) return UWVM_ROS_INVALID;
    result=roswasmc_parameter_exists(cap,0,0,bytes,1);if(result.error) return result.error;
    if(!result.has_length||result.length!=1) return UWVM_ROS_INVALID;return roswasmc_decode_bool(bytes,1,exists);
}
/** @brief Read list/search/describe as a complete canonical String view.
 * The view borrows response storage. Exact ROS1 search and explicitly granted
 * parent-search semantics remain runtime policy; the guest sends no arbitrary
 * namespace/name or privilege escalation request.
 */
static inline uint32_t roswasmc_parameter_metadata(roswasmc_capability const* cap,roswasmc_operation operation,
    void* bytes,size_t capacity,roswasmc_string* text) {
    roswasmc_result result;if(operation!=ROSWASMC_PARAMETER_LIST&&operation!=ROSWASMC_PARAMETER_SEARCH&&operation!=ROSWASMC_PARAMETER_DESCRIBE) return UWVM_ROS_INVALID;
    result=roswasmc_request(cap,operation,0,0,bytes,capacity);if(result.error) return result.error;
    if(!result.has_length||result.length>capacity) return UWVM_ROS_INVALID;return roswasmc_decode_string(bytes,(size_t)result.length,text);
}
#endif
