/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_RMW_WASI_WAIT_H
#define UWVM_RMW_WASI_WAIT_H
#include <uwvm_rmw_c/provider.h>
#include <rcutils/allocator.h>
#ifdef __cplusplus
extern "C" {
#endif
#define UWVM_RMW_WASI_WAIT_VERSION 1u
#define UWVM_RMW_WASI_WAIT_MAX_STATES 16u
#define UWVM_RMW_WASI_WAIT_MAXIMUM_NS UINT64_C(60000000000)
#define UWVM_RMW_WASI_WAIT_SLICE_NS UINT64_C(100000000)
typedef struct uwvm_rmw_wasi_wait uwvm_rmw_wasi_wait;
typedef struct uwvm_rmw_wasi_wait_diagnostics {
 uint64_t attempts,successful_events,last_userdata,last_event_userdata,last_requested_ns,last_event_count;
 uint32_t first_error,last_error,last_call_wasi_errno,last_event_wasi_errno,last_event_type;
 unsigned exported,event_observed,last_event_valid,userdata_exhausted;
} uwvm_rmw_wasi_wait_diagnostics;
/* Allocate all one-subscription/event/count storage BEFORE export/import.
 * Bounds are local resource limits, not grants. Neither this nor callbacks()
 * calls WASI/ROS, probes rights, changes aliases or grants clock/poll authority.
 * The actual allocator/code/state must remain valid for process lifetime.
 * Native-only/non-Preview1 builds return95 without substitute WASI structures.
 * All failures preserve the caller's output pointer/provider/diagnostics.
 */
uint32_t uwvm_rmw_wasi_wait_create(uint64_t maximum_wait_ns,uint64_t maximum_slice_ns,
 size_t maximum_iterations,rcutils_allocator_t const*,uwvm_rmw_wasi_wait**);
/* Sticky export BEFORE returning a callback. Original C has no uninstall;
 * exported state cannot be freed even if later bootstrap installation fails.
 * Profile and every copied callback must remain immutable/alive until the last
 * original context/object/import is physically retired. Concurrent entry17.
 */
uint32_t uwvm_rmw_wasi_wait_callbacks(uwvm_rmw_wasi_wait*,uwvm_rmw_c_wait_provider*);
uint32_t uwvm_rmw_wasi_wait_observe(uwvm_rmw_wasi_wait*,uwvm_rmw_wasi_wait_diagnostics*);
uint32_t uwvm_rmw_wasi_wait_dispose_unpublished(uwvm_rmw_wasi_wait**);
#ifdef __cplusplus
}
#endif
#endif
