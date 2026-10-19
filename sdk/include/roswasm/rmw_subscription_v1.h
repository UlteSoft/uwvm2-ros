/** @file
 * @brief Explicit optional imports for one retained ROS2 native Subscription.
 *
 * Open a genuinely granted topic.subscribe source through the normal ROS2
 * backend first. These seven numeric functions grant nothing, choose no ROS
 * topic/type/QoS, and expose no Host update/debug/management interface. Missing
 * Host support is a real import/link failure, never a successful weak fallback.
 * Default ROS rights remain empty. Normal original business and publisher
 * namespaces remain unchanged; this separate namespace is selected explicitly.
 *
 * Activation/inspection return exactly UMRE-v1 256 bytes describing the same
 * retained Subscription later used by peek/commit/cancel/retire. All full64
 * source/instance/generation/session/native-incarnation values and complete
 * actual QoS must match the original admitted receipt. A Subscription's receipt
 * has no publisher GID: only a real received record supplies that native fact.
 *
 * Peek writes one complete UMRS96 + same-record URMI192 + canonical body into
 * the preflighted caller range. Capacity includes ALL headers and body; it must
 * be288..the published IPC ceiling. The internal UMRS header carries exact
 * record_bytes, hence there is no separate output-length pointer/write. Validate
 * all record/body bounds and native fields before using any byte. expected_gid
 * is the selected original RMW publisher GID width,1..32, not a guessed identity.
 * Capacity90, missing metadata95 and idle11 preserve outputs and do not consume.
 * Every nonzero error preserves output bytes; no error is returned after their
 * successful final Host copy. There is exactly one outstanding borrow per source.
 *
 * A successful peek reserves its real record. Pass BOTH the accepted sequence
 * and independent positive reservation nonce to commit/cancel. Cancel retains
 * the record, and its next peek receives a new nonce. Old tokens never act on a
 * later borrow of the same sequence. Commit consumes that one reserved record.
 * Successive explicit application requests are distinct operations. Never
 * automatically retry an uncertain commit/ACK, invent a cancellation rollback,
 * or publish a prepared RMW result after a failed commit. Lost/late ACK means
 * ambiguous consumption; preserve it and retire the source/session independently.
 *
 * Offsets/capacities remain64-bit for Wasm32/Wasm64; memory is an actual index.
 * Pointer-based C providers explicitly select memory0. Logical zero-byte body
 * stays zero. No native struct, CDR, callback pointer or native address crosses
 * this interface. The complete received metadata/body belongs to one record.
 *
 * Retire attempts this native incarnation once, then the caller still closes
 * the raw source capability even on failure. Close/revoke/session/instance drain
 * independently retires unknown native effects. Old full handles and native
 * incarnations are never automatically reopened or replayed.
 */
#ifndef UWVM_ROSWASM_RMW_SUBSCRIPTION_V1_H
#define UWVM_ROSWASM_RMW_SUBSCRIPTION_V1_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__wasm__)
#define UWVM_ROS_RMW_SUB_IMPORT(name) __attribute__((__import_module__("uwvm_ros2_rmw_sub_v1"),__import_name__(name)))
#else
#define UWVM_ROS_RMW_SUB_IMPORT(name)
#endif
enum {UWVM_ROS_RMW_SUB_ENDPOINT_BYTES=256,UWVM_ROS_RMW_SUB_CONTROL_BYTES=96,
  UWVM_ROS_RMW_SUB_METADATA_BYTES=192,UWVM_ROS_RMW_SUB_MINIMUM_DELIVERY_BYTES=288};
/** Discover only this optional ABI's version. Does not open or grant a source. */
UWVM_ROS_RMW_SUB_IMPORT("abi_version")
uint32_t uwvm_ros2_rmw_sub_v1_abi_version(void);
/** Eagerly create one actual Subscription, without a legacy listener or probe. */
UWVM_ROS_RMW_SUB_IMPORT("subscription_activate")
uint32_t uwvm_ros2_rmw_sub_v1_subscription_activate(uint64_t source,uint32_t memory,uint64_t receipt);
/** Query the exact retained original object without refreshing its identity. */
UWVM_ROS_RMW_SUB_IMPORT("subscription_inspect")
uint32_t uwvm_ros2_rmw_sub_v1_subscription_inspect(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t receipt);
/** Borrow and copy exactly one complete associated record; no implicit commit. */
UWVM_ROS_RMW_SUB_IMPORT("subscription_peek")
uint32_t uwvm_ros2_rmw_sub_v1_subscription_peek(uint64_t source,uint64_t incarnation,
  uint32_t expected_gid,uint32_t memory,uint64_t packet,uint64_t capacity);
/** Consume the exact current borrow. Failure may be ambiguous; never replay. */
UWVM_ROS_RMW_SUB_IMPORT("subscription_commit")
uint32_t uwvm_ros2_rmw_sub_v1_subscription_commit(uint64_t source,uint64_t incarnation,uint64_t sequence,uint64_t reservation);
/** Release the exact borrow while preserving its accepted record and sequence. */
UWVM_ROS_RMW_SUB_IMPORT("subscription_cancel")
uint32_t uwvm_ros2_rmw_sub_v1_subscription_cancel(uint64_t source,uint64_t incarnation,uint64_t sequence,uint64_t reservation);
/** Tombstone and once-retire the retained Subscription; raw source stays open. */
UWVM_ROS_RMW_SUB_IMPORT("subscription_retire")
uint32_t uwvm_ros2_rmw_sub_v1_subscription_retire(uint64_t source,uint64_t incarnation);
#undef UWVM_ROS_RMW_SUB_IMPORT
#ifdef __cplusplus
}
#endif
#endif
