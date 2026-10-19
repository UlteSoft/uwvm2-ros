/* C++ guest wrapper for the stable ROS 1 C ABI. */
#pragma once
#include "abi.h"
#include <span>
#include <string_view>
#include <utility>
namespace uwvm_ros1 {
/** @brief Move-only alias ownership; no operation or endpoint is granted implicitly. */
class capability {
    uwvm_ros_handle handle_{};
    template<auto Import> uint32_t request(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) {
        unsigned char encoded[8]{255,255,255,255,255,255,255,255};
        auto error=Import(handle_,0,uwvm_ros_address(input.data()),input.size(),uwvm_ros_address(output.data()),output.size(),uwvm_ros_address(encoded));
        if(!error||(error==UWVM_ROS_TOO_LARGE&&uwvm_ros_load_u64(encoded)!=UINT64_MAX)) length=uwvm_ros_load_u64(encoded);return error;
    }
public:
    /** @brief Create an empty owner without opening an alias.
     */
    capability()=default;
    /** @brief Copy ownership is disabled.
     */
    capability(capability const&)=delete;
    capability& operator=(capability const&)=delete;
    /** @brief Transfer the owned handle and leave the source empty.
     */
    capability(capability&& other) noexcept : handle_{std::exchange(other.handle_,0)} {}
    /** @brief Attempt one synchronous close on destruction; cleanup errors are not thrown or retried. */
    ~capability() { if(handle_) { uwvm_ros1_capability_close(handle_); } }
    /** @brief Open exactly one nonempty host-granted alias; repeated open on a live owner returns22.
     */
    uint32_t open(std::string_view alias) {
        if(handle_) { return UWVM_ROS_INVALID; } return uwvm_ros1_open(alias.data(),alias.size(),&handle_);
    }
    /** @brief Publish already canonical message bytes through a topic.publish alias; no native struct layout is accepted.
     */
    uint32_t publish(std::span<unsigned char const> bytes) { return uwvm_ros1_publish(handle_,bytes.data(),bytes.size()); }
    /** @brief Activate a topic.subscribe queue; polling later invokes no native guest callback.
     */
    uint32_t subscribe() { return uwvm_ros1_topic_subscribe(handle_); }
    /** @brief Poll once into explicit bounded storage;11 means idle and90 may report a written required size.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t poll(std::span<unsigned char> bytes,uint64_t& length) { return uwvm_ros1_poll(handle_,bytes.data(),bytes.size(),&length); }
    /** @brief Perform service call once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t service_call(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_service_call>(input,output,length); }
    /** @brief Perform service respond once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t service_respond(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_service_respond>(input,output,length); }
    /** @brief Perform action send goal once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_send_goal(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_send_goal>(input,output,length); }
    /** @brief Perform action cancel own goal once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_cancel_own_goal(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_cancel_own_goal>(input,output,length); }
    /** @brief Perform action cancel any goal once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_cancel_any_goal(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_cancel_any_goal>(input,output,length); }
    /** @brief Perform action receive feedback once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_receive_feedback(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_receive_feedback>(input,output,length); }
    /** @brief Perform action receive result once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_receive_result(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_receive_result>(input,output,length); }
    /** @brief Perform action advertise once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_advertise(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_advertise>(input,output,length); }
    /** @brief Perform action respond once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t action_respond(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_action_respond>(input,output,length); }
    /** @brief Perform tf lookup once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t tf_lookup(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_tf_lookup>(input,output,length); }
    /** @brief Perform tf broadcast once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t tf_broadcast(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_tf_broadcast>(input,output,length); }
    /** @brief Perform parameter get once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_get(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_parameter_get>(input,output,length); }
    /** @brief Perform parameter set once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_set(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_parameter_set>(input,output,length); }
    /** @brief Perform parameter list once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_list(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_parameter_list>(input,output,length); }
    /** @brief Perform graph list topics once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_list_topics(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_graph_list_topics>(input,output,length); }
    /** @brief Perform graph list nodes once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_list_nodes(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_graph_list_nodes>(input,output,length); }
    /** @brief Perform graph list services once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_list_services(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_graph_list_services>(input,output,length); }
    /** @brief Perform graph inspect topic once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_inspect_topic(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_graph_inspect_topic>(input,output,length); }
    /** @brief Perform graph inspect node once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_inspect_node(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_graph_inspect_node>(input,output,length); }
    /** @brief Perform graph inspect service once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t graph_inspect_service(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_graph_inspect_service>(input,output,length); }
    /** @brief Perform time read ros once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t time_read_ros(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_time_read_ros>(input,output,length); }
    /** @brief Perform parameter delete once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_delete(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_parameter_delete>(input,output,length); }
    /** @brief Perform parameter exists once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_exists(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_parameter_exists>(input,output,length); }
    /** @brief Perform parameter search once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t parameter_search(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_parameter_search>(input,output,length); }
    /** @brief Perform time read wall once through its independently granted matching alias.
     * Input/output storage remains valid during the synchronous import. On errors,
     * read length only when the slot was written; do not automatically replay an event/RPC.
     */
    uint32_t time_read_wall(std::span<unsigned char const> input,std::span<unsigned char> output,uint64_t& length) { return request<uwvm_ros1_time_read_wall>(input,output,length); }
    /** @brief Activate service.advertise; queued events contain an owned request ID and canonical request.
     */
    uint32_t advertise_service() { return uwvm_ros1_service_advertise(handle_); }
    /** @brief Write an already encoded u32-length canonical String at the host-selected severity (not bare UTF-8).
     */
    uint32_t log(std::span<unsigned char const> bytes) { return uwvm_ros1_log_write(handle_,0,uwvm_ros_address(bytes.data()),bytes.size()); }
    /** @brief Attempt close; the legacy owner clears only on0, although the host may retire it before107/110.
     */
    uint32_t close() { if(!handle_) return UWVM_ROS_BAD_HANDLE; auto error=uwvm_ros1_capability_close(handle_); if(!error) handle_=0;return error; }
    /** @brief Revoke the alias and retire its host handles; the legacy cached handle does not imply later validity.
     */
    uint32_t revoke() { return uwvm_ros1_capability_revoke(handle_); }
    /** @brief Borrow the opaque instance-owned handle; it is not a pointer and must not be transferred.
     */
    uwvm_ros_handle handle() const noexcept { return handle_; }
};
}
