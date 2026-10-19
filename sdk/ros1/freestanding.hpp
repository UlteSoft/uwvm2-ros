/* C++11 freestanding guest SDK; requires only the stable C ABI header. */
#pragma once
#include "abi.h"
namespace uwvm_ros1 {
/** @brief Move-only alias ownership; no operation or endpoint is granted implicitly. */
class freestanding_capability {
    uwvm_ros_handle handle_{};
    template<uint32_t (*Import)(uint64_t,uint32_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t)>
    uint32_t request(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) {
        unsigned char encoded[8]{255,255,255,255,255,255,255,255};
        auto error=Import(handle_,0,uwvm_ros_address(input),input_length,uwvm_ros_address(output),capacity,uwvm_ros_address(encoded));
        if(!error||(error==UWVM_ROS_TOO_LARGE&&uwvm_ros_load_u64(encoded)!=UINT64_MAX)) length=uwvm_ros_load_u64(encoded);
        return error;
    }
public:
    /** @brief Create an empty owner without opening an alias.
     */
    freestanding_capability()=default;
    /** @brief Copy ownership is disabled.
     */
    freestanding_capability(freestanding_capability const&)=delete;
    /** @brief Assignment ownership contract; copy is disabled and move retires the previous owner once. */
    freestanding_capability& operator=(freestanding_capability const&)=delete;
    /** @brief Transfer the owned handle and leave the source empty.
     */
    freestanding_capability(freestanding_capability&& other) noexcept : handle_{other.handle_} { other.handle_=0; }
    /** @brief Assignment ownership contract; copy is disabled and move retires the previous owner once. */
    freestanding_capability& operator=(freestanding_capability&& other) noexcept {
        if(this!=&other) { if(handle_) uwvm_ros1_capability_close(handle_);handle_=other.handle_;other.handle_=0; } return *this;
    }
    /** @brief Attempt one synchronous close on destruction; cleanup errors are not thrown or retried. */
    ~freestanding_capability() { if(handle_) uwvm_ros1_capability_close(handle_); }
    /** @brief Open exactly one nonempty host-granted alias; repeated open on a live owner returns22.
     */
    uint32_t open(char const* alias,uint64_t length) { if(handle_) return UWVM_ROS_INVALID;return uwvm_ros1_open(alias,length,&handle_); }
    /** @brief Publish already canonical message bytes through a topic.publish alias; no native struct layout is accepted.
     */
    uint32_t publish(void const* bytes,uint64_t length) { return uwvm_ros1_publish(handle_,bytes,length); }
    /** @brief Activate a topic.subscribe queue; polling later invokes no native guest callback.
     */
    uint32_t subscribe() { return uwvm_ros1_topic_subscribe(handle_); }
    /** @brief Poll once into explicit bounded storage;11 means idle and90 may report a written required size.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t poll(void* bytes,uint64_t capacity,uint64_t& length) { return uwvm_ros1_poll(handle_,bytes,capacity,&length); }
    /** @brief Activate service.advertise; queued events contain an owned request ID and canonical request.
     */
    uint32_t advertise_service() { return uwvm_ros1_service_advertise(handle_); }
    /** @brief Write an already encoded u32-length canonical String at the host-selected severity (not bare UTF-8).
     */
    uint32_t log(void const* bytes,uint64_t length) { return uwvm_ros1_log_write(handle_,0,uwvm_ros_address(bytes),length); }
    /** @brief Perform service call once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t service_call(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_service_call>(input,input_length,output,capacity,length); }
    /** @brief Perform service respond once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t service_respond(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_service_respond>(input,input_length,output,capacity,length); }
    /** @brief Perform action send goal once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_send_goal(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_send_goal>(input,input_length,output,capacity,length); }
    /** @brief Perform action cancel own goal once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_cancel_own_goal(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_cancel_own_goal>(input,input_length,output,capacity,length); }
    /** @brief Perform action cancel any goal once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_cancel_any_goal(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_cancel_any_goal>(input,input_length,output,capacity,length); }
    /** @brief Perform action receive feedback once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_receive_feedback(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_receive_feedback>(input,input_length,output,capacity,length); }
    /** @brief Perform action receive result once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_receive_result(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_receive_result>(input,input_length,output,capacity,length); }
    /** @brief Perform action advertise once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_advertise(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_advertise>(input,input_length,output,capacity,length); }
    /** @brief Perform action respond once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_respond(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_action_respond>(input,input_length,output,capacity,length); }
    /** @brief Perform tf lookup once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t tf_lookup(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_tf_lookup>(input,input_length,output,capacity,length); }
    /** @brief Perform tf broadcast once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t tf_broadcast(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_tf_broadcast>(input,input_length,output,capacity,length); }
    /** @brief Perform parameter get once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_get(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_parameter_get>(input,input_length,output,capacity,length); }
    /** @brief Perform parameter set once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_set(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_parameter_set>(input,input_length,output,capacity,length); }
    /** @brief Perform parameter delete once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_delete(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_parameter_delete>(input,input_length,output,capacity,length); }
    /** @brief Perform parameter exists once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_exists(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_parameter_exists>(input,input_length,output,capacity,length); }
    /** @brief Perform parameter list once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_list(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_parameter_list>(input,input_length,output,capacity,length); }
    /** @brief Perform parameter search once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_search(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_parameter_search>(input,input_length,output,capacity,length); }
    /** @brief Perform graph inspect topic once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_inspect_topic(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_graph_inspect_topic>(input,input_length,output,capacity,length); }
    /** @brief Perform graph inspect node once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_inspect_node(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_graph_inspect_node>(input,input_length,output,capacity,length); }
    /** @brief Perform graph inspect service once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_inspect_service(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_graph_inspect_service>(input,input_length,output,capacity,length); }
    /** @brief Perform graph list topics once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_list_topics(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_graph_list_topics>(input,input_length,output,capacity,length); }
    /** @brief Perform graph list nodes once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_list_nodes(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_graph_list_nodes>(input,input_length,output,capacity,length); }
    /** @brief Perform graph list services once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_list_services(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_graph_list_services>(input,input_length,output,capacity,length); }
    /** @brief Perform time read ros once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t time_read_ros(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_time_read_ros>(input,input_length,output,capacity,length); }
    /** @brief Perform time read wall once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t time_read_wall(void const* input,uint64_t input_length,void* output,uint64_t capacity,uint64_t& length) { return request<uwvm_ros1_time_read_wall>(input,input_length,output,capacity,length); }
    /** @brief Attempt close; the legacy owner clears only on0, although the host may retire it before107/110.
     */
    uint32_t close() { if(!handle_) return UWVM_ROS_BAD_HANDLE;auto error=uwvm_ros1_capability_close(handle_);if(!error) handle_=0;return error; }
    /** @brief Revoke the alias and retire its host handles; the legacy cached handle does not imply later validity.
     */
    uint32_t revoke() { return uwvm_ros1_capability_revoke(handle_); }
    /** @brief Borrow the opaque instance-owned handle; it is not a pointer and must not be transferred.
     */
    uwvm_ros_handle handle() const noexcept { return handle_; }
};
}
