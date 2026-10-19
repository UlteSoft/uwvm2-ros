/* Real WASI entry: raw|io ALIAS. No implicit subscription, driver or socket. */
#include <string.h>
#include <wasi/api.h>
#include "io.h"
#ifndef UWVM_UR_EXAMPLE_POLL
#define UWVM_UR_EXAMPLE_POLL(c,v,s) uwvm_ur_poll_io(c,v,s)
#endif
static int uwvm_ur_entry(int argc,char** argv) {
 roswasmc_context context={0};roswasmc_capability cap={0};uwvm_ur_storage scratch={0};
 uint32_t primary=0,cleanup=0;size_t used=0;uwvm_ur_io_state value={0};
 if(argc!=3||!argv[1]||!argv[2]||!argv[2][0]||strlen(argv[2])>255||
    (strcmp(argv[1],"raw")&&strcmp(argv[1],"io")))return 22;
 context.backend=UWVM_UR_BACKEND();roswasmc_capability_init(&cap,&context);
 primary=roswasmc_open(&cap,argv[2],strlen(argv[2]));
 if(!primary&&!strcmp(argv[1],"io")){
  primary=roswasmc_subscribe(&cap);
  if(!primary)primary=UWVM_UR_EXAMPLE_POLL(&cap,&value,&scratch);
  if(!primary)primary=UWVM_UR_IO_ENCODE(&value,scratch.bytes,sizeof scratch.bytes,&used);
  if(!primary){__wasi_ciovec_t v={scratch.bytes,used};__wasi_size_t n=0;
   if(__wasi_fd_write(1,&v,1,&n)||n!=used)primary=5;}
 }
 if(cap.handle)cleanup=roswasmc_close(&cap);
 /* Aggregate exit status is distinct from the primary/cleanup observations. */
 uint32_t status=primary?primary:cleanup;return status?(int)(status<126?status:125):0;
}
