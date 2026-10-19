/* SPDX-License-Identifier: Apache-2.0 */
#include <uwvm_rmw/internal.h>
#include <uwvm_rmw/threaded.h>
#if !UWVM_RMW_ENABLE_THREADED_PROFILE
#error "Threaded diagnostics source requires its explicit real profile"
#endif
rmw_ret_t uwvm_rmw_get_threaded_diagnostics(rmw_context_t const*h,uwvm_rmw_threaded_diagnostics*out){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);uwvm_rmw_context*c=uwvm_rmw_context_find(h,1);
 if(!c||!out)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_threaded_diagnostics next={0};next.version=1;next.current_thread_last_entry_refusal=uwvm_rmw_dispatch_last_refusal();
 uwvm_rmw_registry_lock();next.asynchronous_pins=c->asynchronous_pins;uwvm_rmw_registry_unlock();
 next.active_waits=c->active_waits;next.notification_epoch=__atomic_load_n(&c->wake_epoch,__ATOMIC_ACQUIRE);
 next.logical_shutdown=uwvm_rmw_shutdown_requested(c);next.shutdown_cleanup_complete=c->shutdown_cleanup_complete;
 for(size_t i=0;i<c->resource_count;++i)if(!c->resources[i].live&&c->resources[i].release){next.local_release_pending[i]=1;++next.retired_local_pending;}
 *out=next;return RMW_RET_OK;
}
