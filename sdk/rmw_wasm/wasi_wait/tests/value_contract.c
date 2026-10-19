/* SPDX-License-Identifier: Apache-2.0
 *18 authored DATA/lifecycle groups using ORIGINAL WASI types and actual
 * production validator. No imported syscall is replaced or executed here. */
#include <uwvm_rmw/wasi_wait.h>
#include "../src/internal.h"
#include <stdlib.h>
#include <string.h>
static unsigned groups;
static void need(int ok){if(!ok)abort();}
int main(void){
#if !UWVM_RMW_WASI_WAIT_P1_API
 return 95;
#else
 __wasi_userdata_t token=UINT64_C(0x100000001);__wasi_event_t e={0};e.userdata=token;e.type=__WASI_EVENTTYPE_CLOCK;
 need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==0);++groups;
 need(uwvm_rmw_wasi_wait_validate(__WASI_ERRNO_ACCES,1,&e,token)==13);++groups;
 need(uwvm_rmw_wasi_wait_validate(0,0,&e,token)==22);++groups;
 need(uwvm_rmw_wasi_wait_validate(0,2,&e,token)==22);++groups;
 need(uwvm_rmw_wasi_wait_validate(0,1,NULL,token)==22);++groups;
 e.userdata=1;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==22);e.userdata=token;++groups;
 need(uwvm_rmw_wasi_wait_validate(0,1,&e,0)==22);++groups;
 e.type=__WASI_EVENTTYPE_FD_READ;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==22);e.type=__WASI_EVENTTYPE_CLOCK;++groups;
 e.error=__WASI_ERRNO_NOMEM;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==12);e.error=0;++groups;
 need(uwvm_rmw_wasi_wait_validate(__WASI_ERRNO_NOSYS,1,&e,token)==95);++groups;
 e.error=__WASI_ERRNO_INTR;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==4);e.error=0;++groups;
 e.error=__WASI_ERRNO_CANCELED;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==125);e.error=0;++groups;
 e.error=(__wasi_errno_t)UINT16_MAX;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==5);e.error=0;++groups;
 e.error=__WASI_ERRNO_TIMEDOUT;need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==110);e.error=0;++groups;
 e.fd_readwrite.nbytes=UINT64_MAX;e.fd_readwrite.flags=(__wasi_eventrwflags_t)UINT16_MAX;
 need(uwvm_rmw_wasi_wait_validate(0,1,&e,token)==0);++groups; /* Original CLOCK ignores this field. */
 need(uwvm_rmw_wasi_wait_validate((__wasi_errno_t)UINT16_MAX,0,NULL,0)==5);++groups;
 rcutils_allocator_t allocator=rcutils_get_default_allocator();uwvm_rmw_wasi_wait*state=NULL;
 need(uwvm_rmw_wasi_wait_create(1000000000,10000000,1000,&allocator,&state)==0&&state);
 uwvm_rmw_c_wait_provider p={0};need(uwvm_rmw_wasi_wait_callbacks(state,&p)==0&&p.version==1&&p.state==state&&p.wait_slice);
 uwvm_rmw_wasi_wait*same=state;need(uwvm_rmw_wasi_wait_dispose_unpublished(&state)==17&&state==same);
 uwvm_rmw_wasi_wait_diagnostics d;need(uwvm_rmw_wasi_wait_observe(state,&d)==0&&d.exported&&d.attempts==0);
 need(p.wait_slice(p.state,0)==22);need(uwvm_rmw_wasi_wait_observe(state,&d)==0&&d.attempts==0);++groups;
 uwvm_rmw_wasi_wait*unchanged=NULL;
 need(uwvm_rmw_wasi_wait_create(0,1,1,&allocator,&unchanged)==22&&!unchanged);
 need(uwvm_rmw_wasi_wait_create(100,101,1,&allocator,&unchanged)==22&&!unchanged);
 need(uwvm_rmw_wasi_wait_create(100,100,0,&allocator,&unchanged)==22&&!unchanged);
 need(uwvm_rmw_wasi_wait_create(100,100,1,&allocator,&unchanged)==0&&unchanged);
 need(uwvm_rmw_wasi_wait_dispose_unpublished(&unchanged)==0&&!unchanged);++groups;
 need(groups==18);return 0;
#endif
}
