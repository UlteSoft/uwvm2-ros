/* SPDX-License-Identifier: Apache-2.0
 * Optional concrete Guest C transport for B's genuinely owned ROS2 publisher.
 * Include original rmw/rcutils headers from the same complete cross-built SDK;
 * never put the independent sdk/compat/rclc substitute profile on this path.
 */
#ifndef UWVM_RMW_PUBLISHER_TRANSPORT_H
#define UWVM_RMW_PUBLISHER_TRANSPORT_H
#include <uwvm_rmw/bridge.h>
#include <rcutils/allocator.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_PUBLISHER_TRANSPORT_MAX_SLOTS (UWVM_RMW_MAX_CONTEXTS*UWVM_RMW_MAX_OWNERS)
#define UWVM_RMW_PUBLISHER_TRANSPORT_RECEIPT_BYTES 256u
typedef struct uwvm_rmw_publisher_transport uwvm_rmw_publisher_transport;
/* One fixed source slot is claimed BEFORE its first raw activation. Slots and
 * source identities are never reused during this state object's lifetime.
 * Raw version/activation/inspection errors are retained separately from native
 * retirement. A timeout may have an effect; neither this object nor B retries.
 * original_wire is the complete successful raw activation reply, even if later
 * malformed or incompatible with the Guest GID extent. It is not fabricated
 * native metadata. last_wire is the last successful raw inspection reply.
 */
typedef struct uwvm_rmw_publisher_transport_diagnostic {
  uint64_t source;
  uint32_t abi_version,activation_code,activation_validation,first_primary;
  uint32_t inspect_code,inspect_validation,publish_code,retire_code;
  uint64_t inspections,publications;
  uint8_t activation_attempted,original_wire_present,associated;
  uint8_t gid_compatible,retire_attempted,last_wire_present;
  uint8_t original_wire[UWVM_RMW_PUBLISHER_TRANSPORT_RECEIPT_BYTES];
  uint8_t last_wire[UWVM_RMW_PUBLISHER_TRANSPORT_RECEIPT_BYTES];
} uwvm_rmw_publisher_transport_diagnostic;
/* Allocate ALL slot/diagnostic storage before any profile or context can open a
 * source. 1..MAX_SLOTS; no later allocation or dynamic container is used by the
 * callbacks. Output pointers stay unchanged on failure. The allocator and state
 * must remain alive and unmoved until the entire borrowing profile/options/
 * contexts have been finalized. This creates no alias, endpoint or permission.
 */
uint32_t uwvm_rmw_publisher_transport_create(size_t capacity,
  rcutils_allocator_t const * allocator,uwvm_rmw_publisher_transport ** output);
/* Borrow this complete vtable into an explicit profile before init options.
 * No default provider or process-global install occurs. No raw import is called.
 */
uint32_t uwvm_rmw_publisher_transport_callbacks(uwvm_rmw_publisher_transport *,
  uwvm_rmw_publisher_provider * output);
/* Index is the permanent admission order, not a native handle. Output/count are
 * ordinary caller-owned DATA. No native query/cleanup is triggered by reading.
 */
uint32_t uwvm_rmw_publisher_transport_count(uwvm_rmw_publisher_transport *,size_t * output);
uint32_t uwvm_rmw_publisher_transport_diagnostics(uwvm_rmw_publisher_transport *,
  size_t index,uwvm_rmw_publisher_transport_diagnostic * output);
/* LOCAL memory retirement only, after uninstalling the borrowing profile and
 * finalizing every B options/context. Save both B raw-owner-close diagnostics
 * and these raw-native diagnostics first. A known associated incarnation that
 * has never received its once-only retire attempt refuses17. A failed native
 * retire is not retried here and does not prove Host drain; B must still attempt
 * raw source close, and real instance/session drain handles an unknown effect.
 * These external lifetime preconditions are the caller's ordinary C ownership
 * contract, not a claim that this function can inspect all live RMW contexts.
 * On success *state becomes NULL; on refusal it is preserved. Calling callbacks
 * after freeing this state, or destroying concurrently with a borrower, is invalid.
 */
uint32_t uwvm_rmw_publisher_transport_destroy(uwvm_rmw_publisher_transport ** state);
#ifdef __cplusplus
}
#endif
#endif
