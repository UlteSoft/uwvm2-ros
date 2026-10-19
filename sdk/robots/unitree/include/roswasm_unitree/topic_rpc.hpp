/** @file Noncopyable local Topic-RPC correlation owner; no transport/thread.
 * Destruction drops local state only. Close the separate SDK topic owners with
 * their checked cleanup APIs; neither operation cancels a remote Sport request.
 */
#pragma once
#include "topic_rpc.h"
namespace roswasm_unitree {
class TopicRequests {
    uwvm_unitree_rpc_tracker tracker_{};
public:
    TopicRequests() noexcept {uwvm_unitree_rpc_init(&tracker_);}
    TopicRequests(TopicRequests const&)=delete;
    TopicRequests& operator=(TopicRequests const&)=delete;
    TopicRequests(TopicRequests&&)=delete;
    TopicRequests& operator=(TopicRequests&&)=delete;
    ~TopicRequests(){close();}
    uint32_t begin(int64_t id,int64_t api,uint64_t now,uint64_t deadline) noexcept {return uwvm_unitree_rpc_begin(&tracker_,id,api,now,deadline);}
    /** Register the next never-reused local ID. The output changes only on0.
     * This neither publishes nor coordinates IDs with another Guest instance.
     */
    uint32_t begin_next(int64_t api,uint64_t now,uint64_t deadline,int64_t& id) noexcept {return uwvm_unitree_rpc_begin_next(&tracker_,api,now,deadline,&id);}
    uint32_t accept(int64_t id,int64_t api,uint64_t now) noexcept {return uwvm_unitree_rpc_accept(&tracker_,id,api,now);}
    uint32_t forget(int64_t id,int64_t api) noexcept {return uwvm_unitree_rpc_forget(&tracker_,id,api);}
    uint32_t expire(int64_t id,int64_t api,uint64_t now) noexcept {return uwvm_unitree_rpc_expire(&tracker_,id,api,now);}
    void close() noexcept {uwvm_unitree_rpc_close(&tracker_);}
};
}
