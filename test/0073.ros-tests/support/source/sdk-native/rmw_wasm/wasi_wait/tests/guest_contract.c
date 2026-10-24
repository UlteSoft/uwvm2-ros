/* SPDX-License-Identifier: Apache-2.0
 * Actual Preview1 import contract, NOT executed. Run each mode in a fresh owned
 * Guest with separately explicit import and monotonic permission selections.
 * Full original-RMW/A-clock measurement is in original_rmw_composite.c. */
#include <uwvm_rmw/wasi_wait.h>
#include "../../../../../../../../sdk/origin/rmw_wasm/wasi_wait/src/internal.h"
#include <stdlib.h>
#include <string.h>
static void need(int ok){if(!ok)abort();}
int main(int argc,char**argv){
#if !UWVM_RMW_WASI_WAIT_P1_API
 (void)argc;(void)argv;return 95;
#else
 if(argc!=2)return 22;int denied=!strcmp(argv[1],"--explicit-clock-denied");
 if(!denied&&strcmp(argv[1],"--explicit-clock-granted"))return 22;
 rcutils_allocator_t allocator=rcutils_get_default_allocator();uwvm_rmw_wasi_wait*state=NULL;
 need(uwvm_rmw_wasi_wait_create(1000000000,10000000,1000,&allocator,&state)==0);
 uwvm_rmw_c_wait_provider provider={0};need(uwvm_rmw_wasi_wait_callbacks(state,&provider)==0);
 uint32_t code=provider.wait_slice(provider.state,2000000);uwvm_rmw_wasi_wait_diagnostics d;
 need(uwvm_rmw_wasi_wait_observe(state,&d)==0&&d.attempts==1&&d.last_userdata&&d.last_requested_ns==2000000);
 if(denied){need(code==13&&d.last_call_wasi_errno==__WASI_ERRNO_NOTCAPABLE&&!d.successful_events&&!d.last_event_valid);}
 else{
  need(code==0&&d.last_call_wasi_errno==0&&d.last_event_count==1&&d.event_observed&&d.last_event_valid&&d.successful_events==1);
  need(d.last_event_wasi_errno==0&&d.last_event_type==__WASI_EVENTTYPE_CLOCK&&d.last_event_userdata==d.last_userdata);
  uint64_t old=d.last_userdata;need(provider.wait_slice(provider.state,1000000)==0);
  need(uwvm_rmw_wasi_wait_observe(state,&d)==0&&d.last_userdata>old&&d.successful_events==2);
 }
 return 0;
#endif
}
