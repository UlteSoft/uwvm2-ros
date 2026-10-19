/** @file Local, bounded correlation for unitree_api Request/Response TOPICS.
 * This is not a ROS Service/Action and issues no call, cancel, retry or clock
 * import.  Caller-supplied monotonic ticks drive finite expiry explicitly.
 * The caller separately owns topic.publish/topic.subscribe aliases and complete
 * generated codecs.  A matched response identity is correlation, not security
 * authentication.  Session/Gateway authority remains the only ROS permission.
 */
#ifndef UWVM_ROBOTS_UNITREE_TOPIC_RPC_H
#define UWVM_ROBOTS_UNITREE_TOPIC_RPC_H
#include <stdint.h>
#include <stddef.h>
#define UWVM_UNITREE_RPC_CAPACITY 16u
typedef struct uwvm_unitree_rpc_slot {
    int64_t id,api_id;
    uint64_t deadline;
    uint8_t live;
} uwvm_unitree_rpc_slot;
typedef struct uwvm_unitree_rpc_tracker {
    uwvm_unitree_rpc_slot slots[UWVM_UNITREE_RPC_CAPACITY];
    /* Highest successfully registered ID, including retired requests. Keeping
     * this watermark avoids an unbounded history or a late-response ABA match. */
    int64_t last_id;
    uint8_t closed;
} uwvm_unitree_rpc_tracker;
/** Initialize fresh local storage; this owns no endpoint and is not a reset.
 * Never reinitialize a tracker while its response channel can deliver replies
 * from an earlier lifetime. Across trackers/Guest instances the operator must
 * provide distinct ID ranges or retire and drain the old response channel.
 * All access is caller-serialized; this helper creates no thread or lock.
 */
static inline void uwvm_unitree_rpc_init(uwvm_unitree_rpc_tracker* tracker) {
    size_t i;
    if(!tracker)return;
    for(i=0;i<UWVM_UNITREE_RPC_CAPACITY;++i){
        tracker->slots[i].id=0;
        tracker->slots[i].api_id=0;
        tracker->slots[i].deadline=0;
        tracker->slots[i].live=0;
    }
    tracker->last_id=0;
    tracker->closed=0;
}
/** Register BEFORE exactly-one publication. Positive signed64 IDs must strictly
 * increase over this tracker's entire lifetime, regardless of API or retirement.
 * A reused/out-of-order ID returns17; invalid arguments22, closed9, full11.
 * Every refusal leaves all state unchanged. Full11 does not reserve the ID.
 * An accepted ID remains burned after completion, expiry or local abandonment.
 * A publication error may mean delivery is UNKNOWN; retain the request until
 * explicit local retirement and never infer that the robot did not act. This
 * helper cannot authenticate a response or cancel/retry a remote operation.
 */
static inline uint32_t uwvm_unitree_rpc_begin(uwvm_unitree_rpc_tracker* tracker,int64_t id,int64_t api_id,uint64_t now,uint64_t deadline) {
    size_t i,free_slot=UWVM_UNITREE_RPC_CAPACITY;
    if(!tracker||id<=0||api_id<=0||deadline<=now)return 22;
    if(tracker->closed)return 9;
    if(id<=tracker->last_id)return 17;
    for(i=0;i<UWVM_UNITREE_RPC_CAPACITY;++i){
        if(!tracker->slots[i].live){free_slot=i;break;}
    }
    if(free_slot==UWVM_UNITREE_RPC_CAPACITY)return 11;
    tracker->slots[free_slot].id=id;tracker->slots[free_slot].api_id=api_id;
    tracker->slots[free_slot].deadline=deadline;tracker->slots[free_slot].live=1;
    tracker->last_id=id;
    return 0;
}
/** Reserve the next local ID without a clock/random import or signed overflow.
 * Returns75 at INT64_MAX. On any error *id and the tracker stay unchanged.
 * Allocation starts at1; independent trackers still require disjoint lifetimes
 * or externally assigned ID ranges as documented above. This does not publish.
 */
static inline uint32_t uwvm_unitree_rpc_begin_next(uwvm_unitree_rpc_tracker* tracker,int64_t api_id,uint64_t now,uint64_t deadline,int64_t* id) {
    int64_t candidate;
    uint32_t error;
    if(!tracker||!id||api_id<=0||deadline<=now)return 22;
    if(tracker->closed)return 9;
    if(tracker->last_id==INT64_MAX)return 75;
    candidate=tracker->last_id+1;
    error=uwvm_unitree_rpc_begin(tracker,candidate,api_id,now,deadline);
    if(!error)*id=candidate;
    return error;
}
/** Find exact own request and API together. Wrong/missing identity returns9;
 * neither a response nor timeout may consume another pending request.
 */
static inline uwvm_unitree_rpc_slot* uwvm_unitree_rpc_find(uwvm_unitree_rpc_tracker* tracker,int64_t id,int64_t api_id) {
    size_t i;if(!tracker||tracker->closed||id<=0||api_id<=0)return NULL;
    for(i=0;i<UWVM_UNITREE_RPC_CAPACITY;++i){
        if(tracker->slots[i].live&&tracker->slots[i].id==id&&tracker->slots[i].api_id==api_id)return &tracker->slots[i];
    }
    return NULL;
}
/** Call only AFTER the complete response codec/semantic checks succeed. An
 * expired response returns110 and retains the slot until explicit forget/expiry.
 * Returning0 consumes exactly one slot; a duplicate response subsequently gets9.
 */
static inline uint32_t uwvm_unitree_rpc_accept(uwvm_unitree_rpc_tracker* tracker,int64_t id,int64_t api_id,uint64_t now) {
    uwvm_unitree_rpc_slot* slot=uwvm_unitree_rpc_find(tracker,id,api_id);
    if(!slot)return 9;
    if(now>=slot->deadline)return 110;
    slot->live=0;
    return 0;
}
/** Explicitly retire local ownership, including a delivery-unknown request.
 * The ID stays burned. This does not prove non-delivery or stop a remote robot.
 */
static inline uint32_t uwvm_unitree_rpc_forget(uwvm_unitree_rpc_tracker* tracker,int64_t id,int64_t api_id) {
    uwvm_unitree_rpc_slot* slot=uwvm_unitree_rpc_find(tracker,id,api_id);if(!slot)return 9;slot->live=0;return 0;
}
/** Finite caller-clock expiry. Pending11 leaves ownership intact; elapsed110
 * retires only this local ID. No automatic retransmit or remote cancel exists.
 */
static inline uint32_t uwvm_unitree_rpc_expire(uwvm_unitree_rpc_tracker* tracker,int64_t id,int64_t api_id,uint64_t now) {
    uwvm_unitree_rpc_slot* slot=uwvm_unitree_rpc_find(tracker,id,api_id);if(!slot)return 9;if(now<slot->deadline)return 11;slot->live=0;return 110;
}
/** Close local correlation. Caller still checks separate endpoint cleanup. */
static inline void uwvm_unitree_rpc_close(uwvm_unitree_rpc_tracker* tracker) {
    size_t i;if(!tracker)return;for(i=0;i<UWVM_UNITREE_RPC_CAPACITY;++i)tracker->slots[i].live=0;tracker->closed=1;
}
#endif
