/* Real WASI command: raw|state ALIAS. No native robot socket or driver import. */
#include <string.h>
#include <wasi/api.h>
#include "state.h"
#ifndef UWVM_TURTLEBOT3_EXAMPLE_POLL
#define UWVM_TURTLEBOT3_EXAMPLE_POLL(c,v,s) uwvm_turtlebot3_poll_state(c,v,s)
#endif
static int uwvm_turtlebot3_entry(int argc,char** argv) {
 roswasmc_context context={0};roswasmc_capability cap={0};uwvm_turtlebot3_storage scratch={0};
 uint32_t primary=0,cleanup=0;size_t used=0;uwvm_turtlebot3_state value={0};
 if(argc!=3||!argv[1]||!argv[2]||!argv[2][0]||strlen(argv[2])>255||
    (strcmp(argv[1],"raw")&&strcmp(argv[1],"state")))return 22;
 context.backend=UWVM_TURTLEBOT3_BACKEND();roswasmc_capability_init(&cap,&context);
 primary=roswasmc_open(&cap,argv[2],strlen(argv[2]));
 if(!primary&&!strcmp(argv[1],"state")){
  primary=roswasmc_subscribe(&cap);
  if(!primary)primary=UWVM_TURTLEBOT3_EXAMPLE_POLL(&cap,&value,&scratch);
  if(!primary)primary=UWVM_TURTLEBOT3_STATE_ENCODE(&value,scratch.bytes,sizeof scratch.bytes,&used);
  if(!primary){__wasi_ciovec_t v={scratch.bytes,used};__wasi_size_t n=0;
   if(__wasi_fd_write(1,&v,1,&n)||n!=used)primary=5;}
 }
 if(cap.handle)cleanup=roswasmc_close(&cap);
 /* Preserve the primary operation separately; cleanup is not physical stop. */
 uint32_t status=primary?primary:cleanup;return status?(int)(status<126?status:125):0;
}
