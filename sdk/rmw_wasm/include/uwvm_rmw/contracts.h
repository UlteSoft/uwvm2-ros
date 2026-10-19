/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2026 UWVM contributors.
 *
 * Versioned seam between the REAL upstream RMW ABI implementation and the
 * independently implemented WASI platform / wait-set providers. Include genuine
 * rmw/rosidl/rcutils headers from one fixed, complete cross-built SDK. Do not put
 * sdk/compat/rclc/include (the independent source facade) on this include path.
 *
 * The graph guard below is a real Guest-local notification object. Its local
 * existence confers no native graph/discovery permission. A provider must keep
 * its real state, trigger/wake semantics and teardown, not return a success
 * placeholder. Native graph projections still need separate explicit aliases.
 * No provider is installed by default. B's rmw_create_node MUST fail if these
 * mandatory hooks are missing; a graph guard cannot be silently omitted.
 */
#ifndef UWVM_RMW_CONTRACTS_H
#define UWVM_RMW_CONTRACTS_H
#include <stddef.h>
#include <stdint.h>
#include <rmw/rmw.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_CONTRACT_VERSION 1u
#define UWVM_RMW_MAX_CONTEXTS 8u
#define UWVM_RMW_MAX_NODES 32u
#define UWVM_RMW_MAX_OWNERS 256u
#define UWVM_RMW_MAX_NAME_BYTES 255u
#define UWVM_RMW_MAX_CANONICAL_BYTES 4194304u
/* C owns this provider's implementation, not this versioned declaration. The
 * caller retains each context/guard until destroy returns normally. create
 * returns NULL + RMW error on failure, with no unreported partial guard. A
 * failed destroy must still retire its local object once and preserve its own
 * diagnostic; it is never automatically replayed. notify is local-only, cannot
 * invoke application callbacks and cannot open native resources. */
typedef struct uwvm_rmw_graph_guard_provider {
  uint32_t version;
  void * state;
  rmw_guard_condition_t * (*create)(rmw_context_t * context, void * state);
  rmw_ret_t (*destroy)(rmw_guard_condition_t * guard, void * state);
  rmw_ret_t (*notify)(rmw_guard_condition_t const * guard, void * state);
} uwvm_rmw_graph_guard_provider;
/* A owns platform/ and cmake/, C owns guard/wait/subscription/service/metadata
 * sources. B owns bridge.h/internal.h/codec.h and context/node/publisher/codec
 * sources. A must link rmw_implementation with official static selection and
 * exactly one real generated typesupport, not fake dlopen/native stubs.
 *
 * C must use genuine bounded readiness/wait/clock operations. A stdio credit or
 * number of polls is not elapsed nanoseconds. Existing event metadata does not
 * contain DDS writer GUID / timestamps; C must distinguish real RMW-local IDs
 * from unavailable native fields and extend the protocol where required.
 *
 * Internal context access is via internal.h admission helpers, never by casting
 * arbitrary foreign rmw_context_t::impl / node::data and trusting its contents.
 * Every provider must honor the shared context dispatch/close lease. C cannot
 * mutate B's owner arrays directly; corresponding reservation/admission APIs
 * will be declared in internal.h before C begins substantive implementation.
 */
#ifdef __cplusplus
}
#endif
#endif
