/** @file
 * @brief Optional raw imports for a genuinely owned ROS2 native publisher.
 *
 * This is an implementation interface for the original-upstream RMW provider.
 * Application code normally uses the original rcl/rclc/rclcpp API instead.
 * Link this namespace only when the selected Host actually implements it.
 * Original uwvm_ros{1,2}_v1 function lists and numeric signatures are unchanged.
 *
 * No function opens an alias or grants a right. Open the explicitly configured
 * topic.publish alias with the normal source backend first. All calls require
 * the same live full64 source capability, Runtime instance and generation.
 * A Guest cannot set a ROS name, native type, QoS, Host policy, update or debug
 * setting through these calls. Those are fixed by the original explicit grant.
 *
 * Creation is eager and publishes no probe message. Its successful receipt
 * describes the same retained native object later used for inspect/publish/
 * retire, including its genuinely queried effective QoS and complete native GID.
 * A missing original SDK API returns95. It never substitutes requested QoS or a
 * local identifier as evidence. A failed/timed-out call must never be replayed
 * automatically: publication or another native effect may already have happened.
 *
 * Offsets and lengths are64-bit for both Wasm32/Wasm64; memory selects an actual
 * linear memory. Pointer-based C providers in this SDK explicitly select memory0.
 * Nonzero errors leave caller output bytes unchanged. There is no output length
 * slot: activation/inspection success writes exactly256 bytes of UMRE-v1.
 * Validate the entire receipt, all reserved bytes, source binding, incarnation,
 * complete QoS and selected actual GID extent before exposing an RMW object.
 *
 * Retirement releases only this native incarnation, leaves the raw source
 * capability open, and allows one cleanup attempt. Afterwards close that raw
 * capability even if retirement failed. Closing/revoking a source or abandoning
 * its session independently retires associated native owners. Reusing an old
 * full source handle never creates another native object; open a fresh source
 * only when a new application operation is explicitly intended and authorized.
 */
#ifndef UWVM_ROSWASM_RMW_PUBLISHER_V1_H
#define UWVM_ROSWASM_RMW_PUBLISHER_V1_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__wasm__)
#define UWVM_ROS_RMW_IMPORT(name) __attribute__((__import_module__("uwvm_ros2_rmw_v1"),__import_name__(name)))
#else
#define UWVM_ROS_RMW_IMPORT(name)
#endif
enum { UWVM_ROS_RMW_ENDPOINT_RECEIPT_BYTES=256 };
/** Return the raw namespace ABI version. This grants no ROS authority. */
UWVM_ROS_RMW_IMPORT("abi_version")
uint32_t uwvm_ros2_rmw_v1_abi_version(void);
/** Create one real native publisher for source; commit its full UMRE receipt
 * only on success. Do not call the old topic_publish to activate this source. */
UWVM_ROS_RMW_IMPORT("publisher_activate")
uint32_t uwvm_ros2_rmw_v1_publisher_activate(uint64_t source,uint32_t memory,uint64_t receipt);
/** Query the same native object. Missing/changed identity or actual QoS refuses
 * the operation; it does not refresh the admitted snapshot or replace an owner. */
UWVM_ROS_RMW_IMPORT("publisher_inspect")
uint32_t uwvm_ros2_rmw_v1_publisher_inspect(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t receipt);
/** Publish one whole bounded canonical message on the retained native object.
 * Encode every original field with the selected complete canonical codec.
 * Never pass a native generated struct or arbitrary CDR bytes as canonical data. */
UWVM_ROS_RMW_IMPORT("publisher_publish")
uint32_t uwvm_ros2_rmw_v1_publisher_publish(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t canonical,uint64_t bytes);
/** Tombstone this incarnation before one bounded native cleanup attempt.
 * No memory argument or caller-selected management command is accepted. */
UWVM_ROS_RMW_IMPORT("publisher_retire")
uint32_t uwvm_ros2_rmw_v1_publisher_retire(uint64_t source,uint64_t incarnation);
#undef UWVM_ROS_RMW_IMPORT
#ifdef __cplusplus
}
#endif
#endif
