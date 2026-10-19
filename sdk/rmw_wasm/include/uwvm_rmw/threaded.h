/* SPDX-License-Identifier: Apache-2.0
 * Optional source-recompiled profile diagnostics, not a stable binary ABI.
 * Does not add a Guest import or a capability. Save before Context finalization.
 */
#ifndef UWVM_RMW_THREADED_H
#define UWVM_RMW_THREADED_H
#include "bridge.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct uwvm_rmw_threaded_diagnostics {
 uint32_t version,notification_epoch,current_thread_last_entry_refusal;
 size_t asynchronous_pins,active_waits,retired_local_pending;
 uint8_t local_release_pending[UWVM_RMW_MAX_OWNERS];
 int logical_shutdown,shutdown_cleanup_complete;
} uwvm_rmw_threaded_diagnostics;
/* Requires the actual ON composition. A successful snapshot distinguishes a
 * pending deferred local release from an attempted successful release0; all
 * original per-native/per-raw/per-local status arrays are still independent. */
rmw_ret_t uwvm_rmw_get_threaded_diagnostics(rmw_context_t const*,uwvm_rmw_threaded_diagnostics*);
#ifdef __cplusplus
}
#endif
#endif
