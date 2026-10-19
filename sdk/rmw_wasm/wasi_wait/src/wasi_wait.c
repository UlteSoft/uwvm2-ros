/* SPDX-License-Identifier: Apache-2.0
 * Real one-clock Preview1 wait, no elapsed-time synthesis or syscall injection.
 * A process-global nonblocking entry flag bounds this one-buffer provider to
 * one call at a time; contenders return17, never wait on another Guest thread.
 */
#include <uwvm_rmw/wasi_wait.h>
#include "internal.h"
#include <stdatomic.h>
#include <string.h>
#include <limits.h>
#if UWVM_RMW_WASI_WAIT_P1_API
struct uwvm_rmw_wasi_wait {
 rcutils_allocator_t allocator;uint64_t maximum_wait_ns,maximum_slice_ns;size_t maximum_iterations;
 unsigned exported;uwvm_rmw_wasi_wait_diagnostics facts;
 __wasi_subscription_t subscription;__wasi_event_t event;__wasi_size_t count;
};
static atomic_flag busy=ATOMIC_FLAG_INIT;
static uwvm_rmw_wasi_wait*states[UWVM_RMW_WASI_WAIT_MAX_STATES];
/* Nonreusing process-wide association, including across unpublished disposal.
 * Synchronous poll does not deliver an asynchronous callback after return. */
static uint64_t next_userdata=1;static unsigned exhausted;
static int enter(void){return !atomic_flag_test_and_set_explicit(&busy,memory_order_acquire);}
static void leave(void){atomic_flag_clear_explicit(&busy,memory_order_release);}
static uwvm_rmw_wasi_wait*find(void const*address){
 for(size_t i=0;i<UWVM_RMW_WASI_WAIT_MAX_STATES;++i)if(states[i]==address)return states[i];return NULL;
}
static void bump(uint64_t*n){if(*n!=UINT64_MAX)++*n;}
static uint32_t record(uwvm_rmw_wasi_wait*s,uint32_t code){
 s->facts.last_error=code;if(code&&!s->facts.first_error)s->facts.first_error=code;return code;
}
static uint32_t wait_slice(void*address,uint64_t maximum_ns){
 if(!enter())return 17;uwvm_rmw_wasi_wait*s=find(address);uint32_t code;
 if(!s){leave();return 22;}
 if(!s->exported){leave();return 13;}
 if(!maximum_ns||maximum_ns>s->maximum_slice_ns){code=record(s,22);leave();return code;}
 if(exhausted){s->facts.userdata_exhausted=1;code=record(s,90);leave();return code;}
 __wasi_userdata_t token=next_userdata;
 if(next_userdata==UINT64_MAX)exhausted=1;else ++next_userdata;
 memset(&s->subscription,0,sizeof s->subscription);memset(&s->event,0,sizeof s->event);
 s->count=(__wasi_size_t)-1;s->event.type=(__wasi_eventtype_t)-1;s->event.error=__WASI_ERRNO_IO;
 s->subscription.userdata=token;s->subscription.u.tag=__WASI_EVENTTYPE_CLOCK;
 s->subscription.u.u.clock.id=__WASI_CLOCKID_MONOTONIC;
 s->subscription.u.u.clock.timeout=(__wasi_timestamp_t)maximum_ns;
 s->subscription.u.u.clock.precision=0;s->subscription.u.u.clock.flags=0; /* relative nanoseconds */
 s->facts.last_userdata=token;s->facts.last_requested_ns=maximum_ns;s->facts.last_event_valid=0;s->facts.event_observed=0;bump(&s->facts.attempts);
 /* Exactly ONE genuine imported poll. No retry/fallback/callback, heap
  * allocation, clock-epoch conversion, fd subscription or stdio wait credit.
  * Trap/cancel must retire the whole Guest group; buffers stay process-live
  * until every actual Host import is physically joined. No reset/unexport API. */
 __wasi_errno_t actual=__wasi_poll_oneoff(&s->subscription,&s->event,1,&s->count);
 s->facts.last_call_wasi_errno=(uint32_t)actual;s->facts.last_event_count=(uint64_t)s->count;
 s->facts.event_observed=(unsigned)(s->count==1);
 s->facts.last_event_wasi_errno=(uint32_t)s->event.error;s->facts.last_event_type=(uint32_t)s->event.type;
 s->facts.last_event_userdata=(uint64_t)s->event.userdata;
 code=uwvm_rmw_wasi_wait_validate(actual,s->count,&s->event,token);
 if(!code){s->facts.last_event_valid=1;bump(&s->facts.successful_events);}
 record(s,code);leave();return code;
}
#endif
uint32_t uwvm_rmw_wasi_wait_create(uint64_t maximum_wait_ns,uint64_t maximum_slice_ns,
 size_t maximum_iterations,rcutils_allocator_t const*allocator,uwvm_rmw_wasi_wait**output){
 if(!output)return 22;
#if !UWVM_RMW_WASI_WAIT_P1_API
 (void)maximum_wait_ns;(void)maximum_slice_ns;(void)maximum_iterations;(void)allocator;return 95;
#else
 if(!allocator||!rcutils_allocator_is_valid(allocator)||!maximum_wait_ns||maximum_wait_ns>UWVM_RMW_WASI_WAIT_MAXIMUM_NS||
 !maximum_slice_ns||maximum_slice_ns>maximum_wait_ns||maximum_slice_ns>UWVM_RMW_WASI_WAIT_SLICE_NS||
 !maximum_iterations||maximum_iterations>1048576)return 22;
 if(!enter())return 17;size_t slot=0;while(slot<UWVM_RMW_WASI_WAIT_MAX_STATES&&states[slot])++slot;
 if(slot==UWVM_RMW_WASI_WAIT_MAX_STATES){leave();return 90;}
 uwvm_rmw_wasi_wait*s=allocator->zero_allocate(1,sizeof*s,allocator->state);
 if(!s){leave();return 12;}s->allocator=*allocator;s->maximum_wait_ns=maximum_wait_ns;
 s->maximum_slice_ns=maximum_slice_ns;s->maximum_iterations=maximum_iterations;states[slot]=s;*output=s;leave();return 0;
#endif
}
uint32_t uwvm_rmw_wasi_wait_callbacks(uwvm_rmw_wasi_wait*address,uwvm_rmw_c_wait_provider*output){
 if(!output)return 22;
#if !UWVM_RMW_WASI_WAIT_P1_API
 (void)address;return 95;
#else
 if(!enter())return 17;uwvm_rmw_wasi_wait*s=find(address);if(!s){leave();return 22;}
 /* Sticky BEFORE exposing the borrowed callback/state, even on a later failed
  * C profile install. Current original C has no uninstall protocol. */
 s->exported=1;s->facts.exported=1;
 uwvm_rmw_c_wait_provider next={1,s,wait_slice,s->maximum_wait_ns,s->maximum_slice_ns,s->maximum_iterations};
 *output=next;leave();return 0;
#endif
}
uint32_t uwvm_rmw_wasi_wait_observe(uwvm_rmw_wasi_wait*address,uwvm_rmw_wasi_wait_diagnostics*output){
 if(!output)return 22;
#if !UWVM_RMW_WASI_WAIT_P1_API
 (void)address;return 95;
#else
 if(!enter())return 17;uwvm_rmw_wasi_wait*s=find(address);if(!s){leave();return 22;}
 *output=s->facts;leave();return 0;
#endif
}
uint32_t uwvm_rmw_wasi_wait_dispose_unpublished(uwvm_rmw_wasi_wait**output){
 if(!output)return 22;
#if !UWVM_RMW_WASI_WAIT_P1_API
 return 95;
#else
 if(!enter())return 17;uwvm_rmw_wasi_wait*s=find(*output);if(!s){leave();return 22;}
 if(s->exported){leave();return 17;}rcutils_allocator_t allocator=s->allocator;
 for(size_t i=0;i<UWVM_RMW_WASI_WAIT_MAX_STATES;++i)if(states[i]==s){states[i]=NULL;break;}
 allocator.deallocate(s,allocator.state);*output=NULL;leave();return 0;
#endif
}
