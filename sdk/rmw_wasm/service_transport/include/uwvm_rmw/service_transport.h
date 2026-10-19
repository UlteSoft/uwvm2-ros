/* SPDX-License-Identifier: Apache-2.0
 * Explicit original RMW C provider for the separate retained-native raw12 ABI.
 * All rmw/rcutils/rosidl types come from the actual selected original SDK.
 * Source-only: numeric declarations are not an installed Host or permission.
 */
#ifndef UWVM_RMW_SERVICE_TRANSPORT_H
#define UWVM_RMW_SERVICE_TRANSPORT_H
#include <uwvm_rmw_c/provider.h>
#include <rcutils/allocator.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_SERVICE_TRANSPORT_MAX_SLOTS (UWVM_RMW_MAX_CONTEXTS*UWVM_RMW_MAX_OWNERS)
#define UWVM_RMW_SERVICE_TRANSPORT_MAX_TRANSACTIONS 65536u
#define UWVM_RMW_SERVICE_TRANSPORT_MAX_HISTORY_BYTES (64u*1024u*1024u)
typedef struct uwvm_rmw_service_transport uwvm_rmw_service_transport;
typedef struct uwvm_rmw_service_transport_diagnostic {
 uint64_t source,peer,submit_attempts,response_attempts,peeks,commits,cancels,last_sequence,last_reservation,last_committed_sequence,last_ticket;
 size_t transaction_slots_used;
 uint32_t abi_version,first_primary,activation_code,activation_validation,inspect_code,inspect_validation,
  submit_code,respond_code,peek_code,peek_validation,commit_code,cancel_code,retire_code,abandon_code,available_code,raw_close_code[2];
 uint8_t kind,activation_attempted,associated,original_wire_present,inspect_wire_present,poisoned,
  commit_attempted,commit_succeeded,cancel_attempted,retire_attempted,abandon_attempted,available_attempted,raw_close_reported[2],record_wire_present;
 /* Last actually copied fixed wire prefix, including refused bytes, is DATA
  * for independent diagnosis. Presence never means validation/consumption. */
 unsigned char original_wire[256],inspect_wire[256],last_record_wire[352];
} uwvm_rmw_service_transport_diagnostic;
/* Every permanent endpoint, nonreused transaction-attempt slot, receipt,
 * private UMRC160+URMI192+body packet and UWRE64+response scratch is allocated
 * BEFORE any capability/native/raw effect. Actual resource admission belongs
 * to the caller. Logical zero stays zero. No install/open/version query here. */
uint32_t uwvm_rmw_service_transport_create(size_t endpoints,size_t transactions_per_endpoint,
 size_t maximum_request,size_t maximum_response,rcutils_allocator_t const*,uwvm_rmw_service_transport**);
/* Service-only immutable vtables. Wrong endpoint kind refuses95 before own
 * raw12 calls. This is NOT a change to original C's owner-open ordering. */
uint32_t uwvm_rmw_service_transport_callbacks(uwvm_rmw_service_transport*,
 uwvm_rmw_c_activation_provider*,uwvm_rmw_c_request_provider*,uwvm_rmw_c_native_metadata_provider*);
/* Explicit COMPOSITE: copy BOTH actual subscription vtables/states before this
 * service state is exported. Activation dispatches by genuine endpoint kind;
 * inspect/retire use expected count1(topic) vs2(service) only for typed routing,
 * then the real delegated provider verifies its own full source association.
 * take_topic forwards to the original subscription state, request/response to
 * this actual service state. Only these three outputs change: caller keeps its
 * independently selected wait provider and B.publisher. No silent replacement,
 * ambient provider load, implicit lifetime extension or default rights. */
uint32_t uwvm_rmw_service_transport_compose_subscription(uwvm_rmw_service_transport*,
 uwvm_rmw_c_activation_provider const*subscription_activation,
 uwvm_rmw_c_native_metadata_provider const*subscription_metadata,
 uwvm_rmw_c_activation_provider*,uwvm_rmw_c_request_provider*,uwvm_rmw_c_native_metadata_provider*);
/* This independently checks a genuinely owned client using a DISTINCT actual
 * service.available capability in the SAME context. It is a bootstrap/helper
 * API; public RMW wiring is a separate explicit optional C availability vtable.
 * Zero wait is one native observation; positive real waits may return95. */
uint32_t uwvm_rmw_service_transport_available(uwvm_rmw_service_transport*,
 uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*call,
 roswasmc_capability const*availability,uint64_t wait_ns,bool*available);
/* Export the actual existing service state as the OPTIONAL immediate public
 * RMW availability provider. Also valid for this same state's explicitly
 * composed subscription/service callbacks; it never replaces their state.
 * Export makes process lifetime sticky, like the other C14 callback accessors. */
uint32_t uwvm_rmw_service_transport_availability_callbacks(uwvm_rmw_service_transport*,
 uwvm_rmw_c_availability_provider*);
/* One explicit local abandonment only; no native cancellation is claimed.
 * It cannot make an old ticket/retry safe. This is not an upstream RMW API. */
uint32_t uwvm_rmw_service_transport_abandon(uwvm_rmw_service_transport*,
 uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*call,uint64_t ticket);
uint32_t uwvm_rmw_service_transport_count(uwvm_rmw_service_transport*,size_t*);
uint32_t uwvm_rmw_service_transport_diagnostics(uwvm_rmw_service_transport*,size_t,
 uwvm_rmw_service_transport_diagnostic*);
/* DATA attachment only: save genuine B diagnostics BEFORE context_fini. This
 * performs no close and cannot prove one; retain the independent B report. */
uint32_t uwvm_rmw_service_transport_record_raw_close(uwvm_rmw_service_transport*,size_t,unsigned owner_index,uint32_t code);
/* C install has no uninstall. Once callbacks are exported all source/profile/
 * state/allocator objects live until process retirement; this refuses17.
 * Dispose is allowed only before export and before all endpoint attempts. */
uint32_t uwvm_rmw_service_transport_dispose_unpublished(uwvm_rmw_service_transport**);
#ifdef __cplusplus
}
#endif
#endif
