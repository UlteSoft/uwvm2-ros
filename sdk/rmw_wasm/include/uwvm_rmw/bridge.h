/* SPDX-License-Identifier: Apache-2.0
 * This is a REAL RMW implementation seam, not a replacement rcl/rmw header.
 * Every public ROS type below comes from one original cross-built SDK. The
 * selected profile must remain immutable and alive through options/context
 * finalization. Selection confers no Host rights: all raw opens still undergo
 * actual Runtime policy/type/QoS checks. Missing selection is denied by default.
 */
#ifndef UWVM_RMW_BRIDGE_H
#define UWVM_RMW_BRIDGE_H
#include "contracts.h"
#include "codec.h"
#include <roswasm/session.h>
#ifdef __cplusplus
extern "C" {
#endif
enum uwvm_rmw_endpoint_kind { UWVM_RMW_PUBLISHER=1, UWVM_RMW_SUBSCRIPTION=2,
  UWVM_RMW_CLIENT=3, UWVM_RMW_SERVICE=4 };
typedef struct uwvm_rmw_node_selection {
  char const * name;
  char const * namespace_;
} uwvm_rmw_node_selection;
typedef struct uwvm_rmw_endpoint_selection {
  size_t node_index,codec_index;
  enum uwvm_rmw_endpoint_kind kind;
  char const * resolved_name;
  /* Full requested and effective QoS are distinct. The latter must come from
   * the actual selected deployment/native resource, not a claim that a request
   * was negotiated by this Guest. No arbitrary QoS resolution/discovery occurs. */
  rmw_qos_profile_t requested_qos,effective_qos;
  char const * aliases[2];
  size_t alias_count;
  /* Optional explicit CLIENT-only service.available grant. This is distinct
   * from original call/observe aliases[2]; NULL grants nothing and retains the
   * original two-owner client. Source-recompile B/C/providers/app together. */
  char const * availability_alias;
} uwvm_rmw_endpoint_selection;
/* Required real publisher transport, independent of the original81 ABI.
 * A source open alone does not create a native publisher. Every callback below
 * must use the SAME session-owned native incarnation, not a temporary probe or
 * the old lazy roswasmc_publish path. An actual reviewed provider must perform
 * complete current native type/codec/QoS/policy admission and bounded Host RPC.
 * No implementation is supplied here and no provider is selected by default.
 * These C values are a provider API, NOT a memcpy wire or an authentication token.
 */
typedef struct uwvm_rmw_publisher_receipt {
  uint32_t version,backend;
  uint64_t source,instance,runtime_generation,session,native_incarnation;
  rmw_qos_profile_t actual_qos; /* Genuine query of this retained native object. */
  /* Genuine domain-global native publisher GID from this same retained object.
   * Exact actual RMW storage extent is required: never truncate/pad a different
   * native extent or substitute process-local counters. Original81 has no such
   * attestation; a real optional provider is required before creation succeeds.
   */
  size_t native_gid_bytes;
  uint8_t native_gid[RMW_GID_STORAGE_SIZE];
} uwvm_rmw_publisher_receipt;
typedef struct uwvm_rmw_publisher_provider {
  uint32_t version;
  void * state;
  /* Caller allocated/zeroed receipt and all cleanup slots before any open.
   * On ordinary failure the provider must retire partial local state itself;
   * any unknown native effect remains owned by this exact source capability.
   * A complete associated receipt returned even with an error is retained for
   * one explicit retire attempt. Raw source close/Host drain remains mandatory.
   * Actual native QoS is queried, never copied from expected_qos/profile rows.
   */
  uint32_t (*activate)(void *,roswasmc_capability const *,
    uwvm_rmw_endpoint_selection const *,uwvm_rmw_codec const *,
    rmw_publisher_options_t const *,uwvm_rmw_publisher_receipt *);
  /* Inspect must return the same complete binding/incarnation and its actual
   * current QoS. These callbacks never invoke user code or retain Guest bytes.
   * Failed operations do not automatically reopen, replay, resize or retry.
   */
  uint32_t (*inspect)(void *,roswasmc_capability const *,
    uwvm_rmw_publisher_receipt const *,uwvm_rmw_publisher_receipt *);
  uint32_t (*publish)(void *,roswasmc_capability const *,
    uwvm_rmw_publisher_receipt const *,void const *,size_t);
  uint32_t (*retire)(void *,roswasmc_capability const *,
    uwvm_rmw_publisher_receipt const *);
} uwvm_rmw_publisher_provider;
typedef struct uwvm_rmw_profile {
  uint32_t version;
  size_t domain_id;
  roswasmc_backend const * backend; /* Actual immutable ROS2 ABI dispatch. */
  uwvm_rmw_node_selection const * nodes;
  size_t node_count;
  uwvm_rmw_codec const * codecs;
  size_t codec_count;
  uwvm_rmw_endpoint_selection const * endpoints;
  size_t endpoint_count;
  uwvm_rmw_graph_guard_provider graph;
  uwvm_rmw_publisher_provider publisher; /* NULL callbacks =>95 before opens. */
} uwvm_rmw_profile;
/* Explicit bootstrap only, before any init options/context exists. NULL selects
 * no profile and opens nothing. The function validates the complete finite
 * selection and all generated codec graphs before publishing the pointer. */
rmw_ret_t uwvm_rmw_install_profile(uwvm_rmw_profile const * profile);
typedef struct uwvm_rmw_diagnostics {
  uint32_t first_raw_primary;
  uint32_t owner_close_codes[UWVM_RMW_MAX_OWNERS];
  uint8_t owner_close_attempted[UWVM_RMW_MAX_OWNERS];
  uint32_t graph_close_codes[UWVM_RMW_MAX_NODES];
  uint8_t graph_close_attempted[UWVM_RMW_MAX_NODES];
  size_t owner_acquisitions,owner_close_attempts,owner_close_failures;
  uint32_t local_release_codes[UWVM_RMW_MAX_OWNERS];
  size_t local_retirement_failures;
  uint32_t native_retire_codes[UWVM_RMW_MAX_OWNERS];
  uint8_t native_retire_attempted[UWVM_RMW_MAX_OWNERS];
  size_t native_retire_attempts,native_retire_failures;
} uwvm_rmw_diagnostics;
/* Read-only copy; no allocation/import/graph query. Save it BEFORE context_fini.
 * The first raw setup/business failure never gets replaced by a later cleanup
 * failure. Per-owner and per-guard cleanup outcomes remain independently visible. */
rmw_ret_t uwvm_rmw_get_diagnostics(rmw_context_t const *,uwvm_rmw_diagnostics *);
#ifdef __cplusplus
}
#endif
#endif
