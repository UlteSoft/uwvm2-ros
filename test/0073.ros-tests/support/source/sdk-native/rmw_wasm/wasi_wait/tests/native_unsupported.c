/* SPDX-License-Identifier: Apache-2.0
 * Actual host build only: no fake WASI struct or syscall may be linked. */
#include <uwvm_rmw/wasi_wait.h>
#include <string.h>
#include <stdlib.h>
static void need(int ok){if(!ok)abort();}
int main(void){
 uwvm_rmw_wasi_wait*state=NULL;
 need(uwvm_rmw_wasi_wait_create(1000,100,10,NULL,&state)==95&&!state);
 uwvm_rmw_c_wait_provider p;memset(&p,0x5a,sizeof p);uwvm_rmw_c_wait_provider old=p;
 need(uwvm_rmw_wasi_wait_callbacks(NULL,&p)==95&&!memcmp(&p,&old,sizeof p));
 uwvm_rmw_wasi_wait_diagnostics d;memset(&d,0x5a,sizeof d);uwvm_rmw_wasi_wait_diagnostics before=d;
 need(uwvm_rmw_wasi_wait_observe(NULL,&d)==95&&!memcmp(&d,&before,sizeof d));return 0;
}
