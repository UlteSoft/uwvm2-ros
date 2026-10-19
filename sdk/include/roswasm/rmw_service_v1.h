/** @file
 * @brief Optional numeric imports for retained native ROS2 clients/services.
 *
 * Default rights remain empty. Explicitly open service.call plus its selected
 * event.observe observer for a client, or service.advertise plus a distinct
 * service.respond owner for a server. Activation binds those actual current
 * handles to one retained native incarnation; aliases and full type/QoS remain
 * Host policy choices. Numeric arguments grant no authority. There is no weak
 * fallback and no update/debug/policy/management import in this namespace.
 *
 * Activation/inspection copy exactly UMRE256, containing both actual native QoS
 * profiles and any actual native client GID. A client GID is NOT a request writer
 * GUID. Submission returns one full-u64 Gateway ticket in little-endian bytes;
 * the ticket is not a native signed sequence or bearer permission. A successful
 * send is submission only, never an already completed service response.
 *
 * Peek copies one UMRC160 + same-take URMI192 + entire canonical body. Its
 * capacity includes all headers (minimum352, maximum published IPC ceiling).
 * UMRC record_bytes is internal; no second output-length slot is written.
 * Validate every association and exact extent before using any byte. Full
 * actual writer GUID, signed native sequence and source/receive timestamps
 * belong to the SAME take as the body. Real zero-valued native bytes are data.
 * Resource-error/closed controls preserve actual error/flags/correlation with
 * no invented service metadata. Logical zero-body Empty remains zero.
 *
 * Peek borrows; commit consumes exactly the positive accepted sequence plus
 * independent per-attempt nonce. Cancel preserves its record, and a later borrow
 * uses a fresh nonce even at the same sequence. Never retry ambiguous sends,
 * responses or commit/cancel ACKs. Late/lost ACK requires source/session
 * retirement; no rollback, delivery, cancellation or successful completion may
 * be invented. client_abandon is local abandonment and late-response disposal,
 * not a claim that DDS/native request cancellation exists.
 *
 * Availability needs a separately opened service.available capability, exact
 * current client/type/QoS pairing and its own explicit finite bound. It queries
 * THIS retained client; it never constructs a probe client or uses graph lists.
 * Output is one byte0/1. wait_ns0 performs one observation; finite waiting must
 * remain within actual independently admitted Host/native deadlines.
 *
 * Every nonzero error preserves output bytes. Offsets and extents are64-bit;
 * memory is the actual memory index. Pointer-based C providers select memory0.
 * Retire each native incarnation once, then independently close all raw owners,
 * retaining primary/commit/cancel/retire/close outcomes. Guest code cannot select
 * a different Runtime principal, replay an old incarnation or reset its quota.
 */
#ifndef UWVM_ROSWASM_RMW_SERVICE_V1_H
#define UWVM_ROSWASM_RMW_SERVICE_V1_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__wasm__)
#define UWVM_ROS_RMW_SERVICE_IMPORT(name) __attribute__((__import_module__("uwvm_ros2_rmw_srv_v1"),__import_name__(name)))
#else
#define UWVM_ROS_RMW_SERVICE_IMPORT(name)
#endif
enum {UWVM_ROS_RMW_SERVICE_ENDPOINT_BYTES=256,UWVM_ROS_RMW_SERVICE_CONTROL_BYTES=160,
  UWVM_ROS_RMW_SERVICE_METADATA_BYTES=192,UWVM_ROS_RMW_SERVICE_MINIMUM_DELIVERY_BYTES=352};
/** Discover this optional ABI version, without opening an endpoint. */
UWVM_ROS_RMW_SERVICE_IMPORT("abi_version")
uint32_t uwvm_ros2_rmw_srv_v1_abi_version(void);
/** Create one real client using independently granted call/observer owners. */
UWVM_ROS_RMW_SERVICE_IMPORT("client_activate")
uint32_t uwvm_ros2_rmw_srv_v1_client_activate(uint64_t source,uint64_t observer,uint32_t memory,uint64_t receipt);
/** Create one real service using independently granted advertise/respond owners. */
UWVM_ROS_RMW_SERVICE_IMPORT("service_activate")
uint32_t uwvm_ros2_rmw_srv_v1_service_activate(uint64_t advertise,uint64_t responder,uint32_t memory,uint64_t receipt);
/** Query the same retained client/service; output is the exact original UMRE256. */
UWVM_ROS_RMW_SERVICE_IMPORT("endpoint_inspect")
uint32_t uwvm_ros2_rmw_srv_v1_endpoint_inspect(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t receipt);
/** Submit one complete request through the retained client; one eight-byte ticket. */
UWVM_ROS_RMW_SERVICE_IMPORT("client_submit")
uint32_t uwvm_ros2_rmw_srv_v1_client_submit(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t input,uint64_t length,uint64_t ticket);
/** Abandon only the local retained request; no native cancellation is claimed. */
UWVM_ROS_RMW_SERVICE_IMPORT("client_abandon")
uint32_t uwvm_ros2_rmw_srv_v1_client_abandon(uint64_t source,uint64_t incarnation,uint64_t ticket);
/** One actual server response using the independently granted responder owner. */
UWVM_ROS_RMW_SERVICE_IMPORT("service_respond")
uint32_t uwvm_ros2_rmw_srv_v1_service_respond(uint64_t advertise,uint64_t incarnation,uint64_t responder,uint64_t ticket,uint32_t memory,uint64_t input,uint64_t length);
/** Borrow a complete service record; expected_writer is the original native extent. */
UWVM_ROS_RMW_SERVICE_IMPORT("endpoint_peek")
uint32_t uwvm_ros2_rmw_srv_v1_endpoint_peek(uint64_t source,uint64_t incarnation,uint32_t expected_writer,uint32_t memory,uint64_t packet,uint64_t capacity);
/** Consume exactly the current sequence+nonce; unknown outcome is never replayed. */
UWVM_ROS_RMW_SERVICE_IMPORT("endpoint_commit")
uint32_t uwvm_ros2_rmw_srv_v1_endpoint_commit(uint64_t source,uint64_t incarnation,uint64_t sequence,uint64_t reservation);
/** Cancel only the current borrow, preserving its native record and stream sequence. */
UWVM_ROS_RMW_SERVICE_IMPORT("endpoint_cancel")
uint32_t uwvm_ros2_rmw_srv_v1_endpoint_cancel(uint64_t source,uint64_t incarnation,uint64_t sequence,uint64_t reservation);
/** Query this retained client through its distinct explicit availability owner. */
UWVM_ROS_RMW_SERVICE_IMPORT("client_available")
uint32_t uwvm_ros2_rmw_srv_v1_client_available(uint64_t source,uint64_t incarnation,uint64_t availability,uint64_t wait_ns,uint32_t memory,uint64_t available);
/** Once-retire the retained native object; paired raw owners still need close. */
UWVM_ROS_RMW_SERVICE_IMPORT("endpoint_retire")
uint32_t uwvm_ros2_rmw_srv_v1_endpoint_retire(uint64_t source,uint64_t incarnation);
#undef UWVM_ROS_RMW_SERVICE_IMPORT
#ifdef __cplusplus
}
#endif
#endif
