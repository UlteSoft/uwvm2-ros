/* SPDX-License-Identifier: Apache-2.0
 * Cohort C extends genuine upstream RMW, not sdk/compat/rclc. All ROS types
 * below must come from the same original cross-built SDK as cohorts A and B.
 * Explicit source-selected providers confer no Host authority. This header
 * installs neither a wait driver nor metadata by default. */
#ifndef UWVM_RMW_C_PROVIDER_H
#define UWVM_RMW_C_PROVIDER_H
#include <uwvm_rmw/internal.h>
#include <roswasm/events.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_C_PROVIDER_VERSION 1u
#define UWVM_RMW_C_MAX_CONDITIONS (2u*UWVM_RMW_MAX_OWNERS)
#define UWVM_RMW_C_MAX_PENDING 64u
/* max_ns is a real upper-duration request to the caller's independently owned
 * finite wait implementation. A success may be an early wake. The RMW loop
 * re-reads actual A steady time and does not call steps nanoseconds. A driver
 * must not invoke application callbacks, mutate endpoint ownership or start
 * actors. It must have a genuine independently enforced blocking deadline.
 * No provider means positive-duration wait95. Zero-duration readiness does
 * not need a clock/wait grant and still observes the actual current sources. */
typedef struct uwvm_rmw_c_wait_provider {
 uint32_t version;void*state;
 uint32_t(*wait_slice)(void*state,uint64_t maximum_ns);
 uint64_t maximum_wait_ns,maximum_slice_ns;size_t maximum_iterations;
} uwvm_rmw_c_wait_provider;
/* These are adapter callbacks for an ACTUAL independently qualified atomic
 * native-metadata transport. They are NOT a metadata generator or a permission
 * to make up GUIDs/timestamps. Each callback must acquire canonical body and
 * full metadata from the SAME actual consumed record/source under its granted
 * incarnation, and validate the complete native protocol before writing outputs.
 * Success0 explicitly attests complete actually acquired same-record metadata,
 * including the full selected GID/writer extent; zero bytes are data, never an
 * inferred absence marker. A response separately requires info_present==1.
 * Idle11 and every error preserve length/metadata, do not consume on95, and do
 * not cause replay/fallback. Root must source-peer and qualify a real provider
 * before installing it. Current81 imports do not provide these missing fields.
 * Calling ordinary poll then guessing/graph-querying metadata is invalid.
 */
/* Exact source/native-object receipt from the new independently qualified D
 * path. instance/runtime_generation/session are Runtime facts, not Guest-local
 * counters. QoS[0] is topic or request; QoS[1] is service response. These values
 * are actual getters, never copied from requested QoS as discovery evidence. */
typedef struct uwvm_rmw_c_native_endpoint {
 uint32_t version,backend;uint64_t source,instance,runtime_generation,session,native_incarnation;
 size_t qos_count;rmw_qos_profile_t actual_qos[2];
 /* Optional ACTUAL native client GID, full extent preserved. A selected Guest
  * with a different RMW_GID_STORAGE_SIZE cannot truncate or invent a mapping.
  * No GID still permits local request correlation; public global GID stays95. */
 size_t native_gid_bytes;unsigned char native_gid[32];
} uwvm_rmw_c_native_endpoint;
typedef struct uwvm_rmw_c_native_metadata_provider {
 uint32_t version;void*state;
 uint32_t(*take_topic)(void*,uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*,void*,size_t,size_t*,rmw_message_info_t*);
 uint32_t(*take_request)(void*,uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*,void*,size_t,size_t*,uint64_t*,rmw_service_info_t*);
 /* Complete current UWRE header64+canonical payload plus SAME-record native
  * service facts, reconstructed ONLY from the SAME owned native URMI record.
  * Full binding, native incarnation and queue sequence must match the admitted
  * endpoint; the original native receipt remains separately owned by transport.
  * On RESOURCE_ERROR/CLOSED info_present is0, with the complete
  * original error/flags/correlation and empty body preserved. Response info
  * must be present; the C consumer still validates the full source/sequence/
  * incarnation via its real SDK event binding before using either output. */
 uint32_t(*take_response)(void*,uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*observer,
  roswasmc_capability const*source,void*uwre,size_t,size_t*,rmw_service_info_t*,unsigned*info_present);
} uwvm_rmw_c_native_metadata_provider;
/* Endpoint existence and complete effective QoS must be actual native facts,
 * not the declaration in B selection. The provider activates/attests this fresh
 * selected source pair and native type/incarnation under independently granted
 * versioned protocol, without sending any business request. Missing provider
 * makes public endpoint construction95 before its first open in cohort C. */
typedef struct uwvm_rmw_c_activation_provider {
 uint32_t version;void*state;
 uint32_t(*activate)(void*,enum uwvm_rmw_endpoint_kind,
  roswasmc_capability const*source,roswasmc_capability const*auxiliary,
  uwvm_rmw_codec const*,rmw_qos_profile_t const*requested,uwvm_rmw_c_native_endpoint*actual);
 uint32_t(*inspect)(void*,roswasmc_capability const*source,roswasmc_capability const*auxiliary,
  uwvm_rmw_c_native_endpoint const*expected,uwvm_rmw_c_native_endpoint*actual);
 uint32_t(*retire)(void*,roswasmc_capability const*source,roswasmc_capability const*auxiliary,
  uwvm_rmw_c_native_endpoint const*expected);
} uwvm_rmw_c_activation_provider;
/* These business operations use the SAME new native endpoint receipt. They
 * never call legacy service_submit/respond on a different client/server. All
 * storage and pending slots must already be reserved by C. submit returns only
 * admission and a full-u64 Gateway ticket, not completion/native sequence.
 * Every nonzero post-admission ambiguity is terminal; neither callback replays.
 * respond has one attempt and returns success only after actual empty reply
 * extent validation in its real D transport. No callback invokes app code. */
typedef struct uwvm_rmw_c_request_provider {
 uint32_t version;void*state;
 uint32_t(*submit)(void*,uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*source,
  roswasmc_capability const*observer,void const*,size_t,uint64_t*ticket);
 uint32_t(*respond)(void*,uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*advertise,
  roswasmc_capability const*respond,uint64_t ticket,void const*,size_t);
} uwvm_rmw_c_request_provider;
/* Optional immediate query against the SAME original owned client/native
 * receipt. Source+query are distinct live owners in the same Context. No graph,
 * probe client, hidden clock/wait or application callback. query success MUST
 * write exactly0/1 to unsigned output; every error leaves it unchanged. */
typedef struct uwvm_rmw_c_availability_provider {
 uint32_t version;void*state;
 uint32_t(*query)(void*,uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*source,
  roswasmc_capability const*availability,unsigned*available);
} uwvm_rmw_c_availability_provider;
typedef struct uwvm_rmw_c_profile {
 uint32_t version;
 uwvm_rmw_c_activation_provider activation;
 uwvm_rmw_c_request_provider requests;
 uwvm_rmw_c_wait_provider wait;
 uwvm_rmw_c_native_metadata_provider metadata;
 uwvm_rmw_c_availability_provider availability; /* trailing optional; zero =>95 */
} uwvm_rmw_c_profile;
/* Local diagnostics do not grant authority or substitute a native receipt.
 * actual_native_info is the SAME-record native writer/sequence/time; the local
 * sequence exposed by this RMW client is stored separately, never conflated. */
typedef struct uwvm_rmw_c_service_diagnostics {
 uint32_t terminal_error,event_flags;uint64_t gateway_ticket;int64_t local_sequence;
 bool actual_native_info_present,ambiguous_admission;rmw_service_info_t actual_native_info;
} uwvm_rmw_c_service_diagnostics;
rmw_ret_t uwvm_rmw_c_client_diagnostics(rmw_client_t const*,uwvm_rmw_c_service_diagnostics*);
rmw_ret_t uwvm_rmw_c_server_diagnostics(rmw_service_t const*,uwvm_rmw_c_service_diagnostics*);
/* Explicit one-time process bootstrap, before any C guard/wait/endpoint. The
 * complete profile must remain immutable and actual provider state must live through the last
 * context/object. No ambient driver/clock/metadata provider is synthesized. */
rmw_ret_t uwvm_rmw_c_install_profile(uwvm_rmw_c_profile const*);
/* output indices Request,Response,complete Service. Expected identities are
 * mandatory independent actual exports; this function does not invent them. */
uint32_t uwvm_rmw_c_add_two_ints_codecs_init(uwvm_rmw_codec output[3],uint8_t const expected[3][32]);
uwvm_rmw_graph_guard_provider uwvm_rmw_c_graph_provider(void);
#ifdef __cplusplus
}
#endif
#endif
