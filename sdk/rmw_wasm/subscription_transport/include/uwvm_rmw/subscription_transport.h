/* SPDX-License-Identifier: Apache-2.0
 * Source-selected Guest C adapter for the real optional subscription raw7 ABI.
 * This is not a native rmw header, permission, legacy listener, wait driver or
 * concurrency implementation. Use genuine original rmw/rcutils/rosidl headers.
 * The caller must retain B's actual function-scope/context/source lease. No
 * upper rclcpp callback policy or cross-callback lease is fabricated here.
 */
#ifndef UWVM_RMW_SUBSCRIPTION_TRANSPORT_H
#define UWVM_RMW_SUBSCRIPTION_TRANSPORT_H
#include <uwvm_rmw_c/provider.h>
#include <rcutils/allocator.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_SUBSCRIPTION_TRANSPORT_MAX_SLOTS (UWVM_RMW_MAX_CONTEXTS*UWVM_RMW_MAX_OWNERS)
typedef struct uwvm_rmw_subscription_transport uwvm_rmw_subscription_transport;
typedef struct uwvm_rmw_subscription_transport_diagnostic {
 uint64_t source,peeks,commits,cancels,last_sequence,last_reservation,last_committed_sequence,last_committed_reservation;
 uint32_t abi_version,first_primary,activation_code,activation_validation,inspect_code,inspect_validation,
  peek_code,peek_validation,commit_code,cancel_code,retire_code,raw_close_code;
 uint8_t activation_attempted,associated,original_wire_present,inspect_wire_present,
  commit_attempted,commit_succeeded,cancel_attempted,retire_attempted,poisoned,raw_close_reported;
 unsigned char original_wire[256],inspect_wire[256];
} uwvm_rmw_subscription_transport_diagnostic;
/* Allocate every permanent source/diagnostic slot and a serialized private
 * UMRE256 and UMRS96+URMI192+maximum_body scratch BEFORE effects. Logical0
 * stays0; a private allocation is never published as a one-byte message.
 * Allocator/state must outlive all profiles/options/contexts/objects borrowing
 * callbacks. No capability, raw version query or provider install occurs here.
 */
uint32_t uwvm_rmw_subscription_transport_create(size_t slots,size_t maximum_body,
 rcutils_allocator_t const*,uwvm_rmw_subscription_transport**);
/* Explicit subscription-only vtables; other kinds return95 before this
 * provider makes any import. Existing C factories can open owners BEFORE their
 * activation callback: do not claim this changes that upstream core behavior.
 * Compose by kind with other real providers if services are desired; never
 * overwrite a service provider and describe it as complete compatibility.
 * This copies callbacks only. It does NOT install the C process singleton.
 */
uint32_t uwvm_rmw_subscription_transport_callbacks(uwvm_rmw_subscription_transport*,
 uwvm_rmw_c_activation_provider*,uwvm_rmw_c_native_metadata_provider*);
uint32_t uwvm_rmw_subscription_transport_count(uwvm_rmw_subscription_transport*,size_t*);
uint32_t uwvm_rmw_subscription_transport_diagnostics(uwvm_rmw_subscription_transport*,size_t,
 uwvm_rmw_subscription_transport_diagnostic*);
/* Optional joint-report attachment, DATA ONLY: caller first obtains the genuine
 * B owner-close diagnostic before context_fini, identifies this permanent source
 * slot, and attaches its exact attempted return value. This performs NO close
 * and is not independent evidence of cleanup, native success or authority.
 * Preserve the original B diagnostics alongside this convenience copy. */
uint32_t uwvm_rmw_subscription_transport_record_raw_close(uwvm_rmw_subscription_transport*,size_t,uint32_t);
/* Dispose only a never-exported bootstrap that was not installed. Once vtables
 * are exported, C's immutable process singleton may retain them for future
 * contexts and has NO uninstall API. State therefore stays process-lifetime;
 * this function refuses17 rather than freeing a still-borrowable callback.
 * Actual raw owners/native endpoints must still retire normally through B/C.
 */
uint32_t uwvm_rmw_subscription_transport_dispose_unpublished(uwvm_rmw_subscription_transport**);
#ifdef __cplusplus
}
#endif
#endif
