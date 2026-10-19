/* SPDX-License-Identifier: Apache-2.0
 * Version1 B/C private bridge contract. No application may cast or mutate these
 * records. Real handles are admitted by registered address before dereference.
 * The default-OFF original model remains single-threaded. The explicit ON
 * source-recompiled profile adds actual process dispatch/pins/physical wait;
 * original upstream callback lifetime remains above the RMW call boundary.
 */
#ifndef UWVM_RMW_INTERNAL_H
#define UWVM_RMW_INTERNAL_H
#include "bridge.h"
#ifndef UWVM_RMW_ENABLE_THREADED_PROFILE
#define UWVM_RMW_ENABLE_THREADED_PROFILE 0
#endif
#if UWVM_RMW_ENABLE_THREADED_PROFILE
#include "dispatch.h"
#else
#define UWVM_RMW_ENTRY(failure) ((void)0)
#endif
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_RESOURCE_LIMIT UWVM_RMW_MAX_OWNERS
typedef struct rmw_context_impl_s uwvm_rmw_context;
typedef struct uwvm_rmw_node uwvm_rmw_node;
typedef struct uwvm_rmw_resource uwvm_rmw_resource;
typedef struct uwvm_rmw_scope {uwvm_rmw_context * context;int active;} uwvm_rmw_scope;
/* Local release is invoked once AFTER all recorded owners were attempted. It
 * must release every local allocation/complete every pending local waiter even
 * after one callback failure, and return the first raw local failure. It must
 * NOT close/replay native owners. Deferred ON release waits for the last real
 * borrower, then runs under a new actual business-owned scope. That function
 * scope remains active throughout;
 * callbacks cannot create/retire resources or recursively wait. A C++ adapter
 * must catch user exceptions before returning across this C callback boundary.
 * VM/allocator terminal traps require genuine Host instance drain. */
typedef uint32_t (*uwvm_rmw_release_function)(void * object);
struct uwvm_rmw_resource {
  uwvm_rmw_context * context;
  uwvm_rmw_node * node;
  uwvm_rmw_endpoint_selection const * selection;
  void * object; /* Object begins with the actual public rmw handle. */
  uwvm_rmw_release_function release;
  /* Optional real native endpoint retire, always BEFORE raw owner close.
   * The fixed slot and attempted flag exist before activation effects. A failed
   * callback is retained and never retried; ordinary raw/local cleanup continues.
   */
  uwvm_rmw_release_function native_retire;
  uint32_t native_retire_code;
  int native_retire_attempted;
  size_t owner_indices[3],owner_count; /* third slot only explicit client query */
  enum uwvm_rmw_endpoint_kind kind;
  uint32_t local_release_code;
  int live;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  size_t wait_pins; /* short-registry protected; local release is deferred */
#endif
};
struct uwvm_rmw_node {
  rmw_node_t public_handle;
  uwvm_rmw_context * context;
  rmw_guard_condition_t * graph;
  size_t selection_index,resource_count;
  int live;
};
struct rmw_context_impl_s {
  rmw_context_t * public_handle;
  uwvm_rmw_profile const * profile;
  rcutils_allocator_t allocator;
  roswasmc_session session;
  roswasmc_capability owners[UWVM_RMW_MAX_OWNERS];
  uwvm_rmw_resource resources[UWVM_RMW_RESOURCE_LIMIT];
  uwvm_rmw_node nodes[UWVM_RMW_MAX_NODES];
  uwvm_rmw_diagnostics diagnostics;
  size_t resource_count,node_count,live_nodes,provider_children;
  uint64_t local_instance,next_local_gid;
  int shutdown,leased;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
  size_t asynchronous_pins,active_waits;
  uint32_t wake_epoch; /* atomic builtins; finite non-reused notification domain */
  int shutdown_cleanup_complete;
#endif
};
extern char const uwvm_rmw_identifier[];
#if UWVM_RMW_ENABLE_THREADED_PROFILE
static inline int uwvm_rmw_shutdown_requested(uwvm_rmw_context const*c){return __atomic_load_n(&c->shutdown,__ATOMIC_ACQUIRE);}
uwvm_rmw_context*uwvm_rmw_context_pin(rmw_context_t const*);
void uwvm_rmw_context_unpin(uwvm_rmw_context*);
uint32_t uwvm_rmw_resource_pin(uwvm_rmw_resource*);
void uwvm_rmw_resource_unpin(uwvm_rmw_resource*);
void uwvm_rmw_deferred_drain(uwvm_rmw_context*);
uint32_t uwvm_rmw_context_wake(uwvm_rmw_context*);
void uwvm_rmw_c_shutdown_guards(uwvm_rmw_context*); /* registry already held */
void uwvm_rmw_c_guard_drain(uwvm_rmw_context*);
void uwvm_rmw_c_wait_drain(uwvm_rmw_context*);
int uwvm_rmw_c_graph_provider_matches(uwvm_rmw_graph_guard_provider const*);
#else
static inline int uwvm_rmw_shutdown_requested(uwvm_rmw_context const*c){return c->shutdown;}
#endif
uwvm_rmw_context * uwvm_rmw_context_find(rmw_context_t const *,int allow_shutdown);
uwvm_rmw_node * uwvm_rmw_node_find(rmw_node_t const *,int allow_shutdown);
uwvm_rmw_resource * uwvm_rmw_resource_find(void const *,enum uwvm_rmw_endpoint_kind,int allow_shutdown);
rmw_ret_t uwvm_rmw_scope_enter(uwvm_rmw_context *,uwvm_rmw_scope *,int allow_shutdown);
void uwvm_rmw_scope_leave(uwvm_rmw_scope *);
rmw_ret_t uwvm_rmw_error(uwvm_rmw_context *,uint32_t raw,char const * literal_message);
uwvm_rmw_endpoint_selection const * uwvm_rmw_select(uwvm_rmw_node *,
  enum uwvm_rmw_endpoint_kind,char const * resolved_name,rmw_qos_profile_t const *,void const * actual_support);
int uwvm_rmw_qos_equal(rmw_qos_profile_t const *,rmw_qos_profile_t const *);
int uwvm_rmw_qos_concrete(rmw_qos_profile_t const *);
uwvm_rmw_resource * uwvm_rmw_resource_reserve(uwvm_rmw_scope *,uwvm_rmw_node *,
  uwvm_rmw_endpoint_selection const *,void * object,uwvm_rmw_release_function);
uint32_t uwvm_rmw_owner_open(uwvm_rmw_scope *,uwvm_rmw_resource *,size_t alias_index,roswasmc_capability **);
/* Private, not a public alias opener. Requires this admitted CLIENT resource,
 * both original open owners in this same leased context/session, and its exact
 * selected trailing query alias. Fixed ownership is retained before return. */
uint32_t uwvm_rmw_owner_open_availability(uwvm_rmw_scope *,uwvm_rmw_resource *,roswasmc_capability **);
uint32_t uwvm_rmw_resource_retire(uwvm_rmw_scope *,uwvm_rmw_resource *);
/* C-owned guard/wait objects must retain their real context until retirement.
 * Called only with an actually admitted ALREADY leased context, e.g. a private
 * graph provider under B's scope. Retain refuses shutdown; release permits it. No recursive public
 * guard creation or fabricated upper-callback lease is introduced. C must keep
 * its actual object registry and balance this count before freeing that object.
 * B context_fini refuses while any such object remains; no dangling ctx is freed.
 */
uint32_t uwvm_rmw_context_child_retain(uwvm_rmw_context *);
uint32_t uwvm_rmw_context_child_release(uwvm_rmw_context *);
rmw_ret_t uwvm_rmw_notify_local_graph(uwvm_rmw_node *);
/* C must poll/validate while holding a scope, using exactly the admitted
 * observer/source owners; it cannot manufacture readiness from a graph list.
 * Owner/resource slots never move or get reused during a context's lifetime.
 * C's exported rmw_* functions must validate public handle addresses through
 * these helpers. They must not reinterpret unadmitted foreign ::data pointers.
 * A/C may include this header, but B exclusively owns its definitions. */
#ifdef __cplusplus
}
#endif
#endif
