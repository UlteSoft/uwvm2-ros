/* SPDX-License-Identifier: Apache-2.0
 * Two independent read-only ROS1 aliases in one Wasm process. Both SDKs use the
 * SAME regenerated complete domain. Never concatenate generated record headers.
 * Status output: four little-endian uint32 values, Clearpath primary/cleanup
 * followed by Elephant primary/cleanup. No host address is accepted as an alias.
 */
#ifndef UWVM_CLEARPATH_ELEPHANT_ENTRY_H
#define UWVM_CLEARPATH_ELEPHANT_ENTRY_H
#include <string.h>
#include <wasi/api.h>
#include "sdk/origin/robots/mobile_bases/clearpath/platform/include/state.h"
#include "sdk/origin/robots/arms/elephant/mycobot/include/state.h"
#if UWVM_CLEARPATH_SELECTION!=1 || UWVM_ELEPHANT_SELECTION!=14
#error "Select the exact complete Noetic platform and Mycobot source domains"
#endif
#ifndef UWVM_PAIR_CLEARPATH_POLL
#define UWVM_PAIR_CLEARPATH_POLL(c,v,s) uwvm_clearpath_poll_state(c,v,s)
#endif
#ifndef UWVM_PAIR_ELEPHANT_POLL
#define UWVM_PAIR_ELEPHANT_POLL(c,v,s) uwvm_elephant_poll_state(c,v,s)
#endif
/** One bounded attempt per vendor; idle11/storage90 are visible outcomes.
 * No loop, retry, motion, update or debugger control is performed. A failed
 * open does not suppress the other alias's independent operation or cleanup.
 */
static int uwvm_clearpath_elephant_entry(int argc,char** argv){
 roswasmc_context a={0},b={0};roswasmc_capability ca={0},cb={0};
 uwvm_clearpath_storage sa={0};uwvm_elephant_storage sb={0};
 uwvm_clearpath_state va={0};uwvm_elephant_state vb={0};
 uint32_t result[4]={0,0,0,0};unsigned char bytes[16];
 if(argc!=4||!argv[1]||!argv[2]||!argv[3]||
    (strcmp(argv[1],"raw")&&strcmp(argv[1],"state"))||
    !argv[2][0]||!argv[3][0]||strlen(argv[2])>255||strlen(argv[3])>255||
    !strcmp(argv[2],argv[3]))return 22;
 a.backend=UWVM_CLEARPATH_BACKEND();b.backend=UWVM_ELEPHANT_BACKEND();
 roswasmc_capability_init(&ca,&a);roswasmc_capability_init(&cb,&b);
 result[0]=roswasmc_open(&ca,argv[2],strlen(argv[2]));
 result[2]=roswasmc_open(&cb,argv[3],strlen(argv[3]));
 if(!strcmp(argv[1],"state")){
  if(!result[0]){result[0]=roswasmc_subscribe(&ca);
   if(!result[0])result[0]=UWVM_PAIR_CLEARPATH_POLL(&ca,&va,&sa);}
  if(!result[2]){result[2]=roswasmc_subscribe(&cb);
   if(!result[2])result[2]=UWVM_PAIR_ELEPHANT_POLL(&cb,&vb,&sb);}
 }
 if(ca.handle)result[1]=roswasmc_close(&ca);
 if(cb.handle)result[3]=roswasmc_close(&cb);
 /* Keep all primary/cleanup statuses. Close is not a physical emergency stop. */
 for(size_t i=0;i<4;++i)for(size_t j=0;j<4;++j)
  bytes[4*i+j]=(unsigned char)(result[i]>>(8*j));
 __wasi_ciovec_t out={bytes,sizeof bytes};__wasi_size_t written=0;
 if(__wasi_fd_write(1,&out,1,&written)||written!=sizeof bytes)return 5;
 uint32_t status=result[0]?result[0]:result[2]?result[2]:result[1]?result[1]:result[3];
 return status?(int)(status<126?status:125):0;
}
#endif
