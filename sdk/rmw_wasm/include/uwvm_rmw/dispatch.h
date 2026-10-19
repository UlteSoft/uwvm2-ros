/* SPDX-License-Identifier: Apache-2.0
 * Private SOURCE-RECOMPILE contract. No ROS handle, alias or capability opener.
 * Only the implementation can establish TLS ownership by acquiring this mutex.
 * Host instance draining is still required after a terminal VM/allocator trap.
 */
#ifndef UWVM_RMW_DISPATCH_H
#define UWVM_RMW_DISPATCH_H
#include <stdint.h>
#ifndef UWVM_RMW_ENABLE_THREADED_PROFILE
#define UWVM_RMW_ENABLE_THREADED_PROFILE 0
#endif
#ifdef __cplusplus
extern "C" {
#endif
typedef struct uwvm_rmw_dispatch_guard {int owned,platform,borrowed;} uwvm_rmw_dispatch_guard;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
uint32_t uwvm_rmw_dispatch_enter(uwvm_rmw_dispatch_guard*);
uint32_t uwvm_rmw_dispatch_platform_enter(uwvm_rmw_dispatch_guard*);
void uwvm_rmw_dispatch_leave(uwvm_rmw_dispatch_guard*);
void uwvm_rmw_dispatch_suspend(uwvm_rmw_dispatch_guard*);
void uwvm_rmw_dispatch_resume(uwvm_rmw_dispatch_guard*);
int uwvm_rmw_dispatch_owned(void); /* real RMW owner, outside platform callback */
int uwvm_rmw_dispatch_active(void); /* any real owner, for pre-mutation reentry refusal */
uint32_t uwvm_rmw_dispatch_last_refusal(void); /* preallocated TLS diagnostic, no allocator */
void uwvm_rmw_registry_lock(void);
void uwvm_rmw_registry_unlock(void);
/* word is naturally aligned, initialized once before Context exposure, and
 * never reset. UINT32_MAX exhausts the notification domain, refusing further
 * sleeps (90); equality cannot silently wrap back to an earlier observation.
 * physical_wait takes a relative nanosecond interval, or -1 for infinite. It
 * grants no Clock read. Spurious wakeups are legal and require full recheck. */
uint32_t uwvm_rmw_dispatch_notify(uint32_t*word);
uint32_t uwvm_rmw_dispatch_wait(uint32_t*word,uint32_t expected,int64_t relative_ns);
#define UWVM_RMW_ENTRY(failure) \
 uwvm_rmw_dispatch_guard uwvm_dispatch_entry __attribute__((cleanup(uwvm_rmw_dispatch_leave)))={0}; \
 do {if(uwvm_rmw_dispatch_enter(&uwvm_dispatch_entry))return (failure);} while(0)
#else
#define UWVM_RMW_ENTRY(failure) ((void)0)
#endif
#ifdef __cplusplus
}
#endif
#endif
