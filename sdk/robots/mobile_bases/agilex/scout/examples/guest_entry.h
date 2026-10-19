/* SPDX-License-Identifier: Apache-2.0
 * Shared real WASI entry for the C and C++ teaching applications.
 * argv: OPERATION ALIAS. No native topic, CAN device, hostname or credential is
 * selected. Status/odometry/remote (and ROS1 bms) write one complete canonical
 * value to stdout only after a whole successful decode. Idle 11 ends this run.
 * Command stdin layouts are documented in README.md; they are ordinary input,
 * never a trusted Runtime policy or implicit command/stop authorization.
 */
#include <string.h>
#include <wasi/api.h>
#include "scout.h"
static int uwvm_scout_example_read(unsigned char* p,size_t size) {
    while(size) {__wasi_iovec_t part={p,size};__wasi_size_t n=0;
        if(__wasi_fd_read(0,&part,1,&n)||!n||n>size)return 0;p+=n;size-=n;}
    return 1;
}
static uint32_t uwvm_scout_example_write(unsigned char const* p,size_t size) {
    while(size) {__wasi_ciovec_t part={p,size};__wasi_size_t n=0;
        if(__wasi_fd_write(1,&part,1,&n)||!n||n>size)return 5;p+=n;size-=n;}
    return 0;
}
#ifndef UWVM_SCOUT_EXAMPLE_STATUS
#define UWVM_SCOUT_EXAMPLE_STATUS(cap,value,storage) uwvm_scout_poll_status(cap,value,storage)
#endif
static int uwvm_scout_example_main(int argc,char** argv) {
    roswasmc_context context={0};roswasmc_capability cap={0};uwvm_scout_storage scratch={0};
    uint32_t primary=0,cleanup=0;size_t used=0;char const* operation;
    if(argc!=3||!argv[1]||!argv[2]||!argv[2][0]||strlen(argv[2])>255)return 22;
    operation=argv[1];
    if(strcmp(operation,"raw")&&strcmp(operation,"status")&&strcmp(operation,"odom")&&
       strcmp(operation,"remote")&&strcmp(operation,"motion")&&strcmp(operation,"light")&&strcmp(operation,"stop")
#if UWVM_SCOUT_ROS_VERSION == 1
       &&strcmp(operation,"bms")
#endif
       )return 22;
    context.backend=UWVM_SCOUT_BACKEND();roswasmc_capability_init(&cap,&context);
    primary=roswasmc_open(&cap,argv[2],strlen(argv[2]));
    if(!primary) {
        if(!strcmp(operation,"raw")) { /* A successful open alone publishes no command. */ }
        else if(!strcmp(operation,"motion")) {
            unsigned char input[52];uint64_t model=0;roswasmc_twist value={{0,0,0},{0,0,0}};
            uwvm_scout_motion_limits limits={0};double forward=0,lateral=0,yaw=0;
            if(!uwvm_scout_example_read(input,sizeof input))primary=22;
            else {roswasmc_reader reader=roswasmc_reader_init(input,sizeof input);
                if(!roswasmc_read_integer(&reader,&model,4)||!roswasmc_read_float64(&reader,&limits.forward)||
                   !roswasmc_read_float64(&reader,&limits.lateral)||!roswasmc_read_float64(&reader,&limits.yaw)||
                   !roswasmc_read_float64(&reader,&forward)||!roswasmc_read_float64(&reader,&lateral)||
                   !roswasmc_read_float64(&reader,&yaw)||!roswasmc_reader_done(&reader))primary=22;
                else {limits.model=(uint32_t)model;primary=uwvm_scout_make_motion(&limits,forward,lateral,yaw,&value);
                    if(!primary)primary=roswasmc_publish_twist(&cap,&value);}}
        } else if(!strcmp(operation,"light")) {
            unsigned char input[5];uwvm_scout_light value={0};
            if(!uwvm_scout_example_read(input,sizeof input))primary=22;
            else {primary=uwvm_scout_make_light(input[0],input[1],input[2],input[3],input[4],&value);
                if(!primary)primary=uwvm_scout_publish_light(&cap,&value,&scratch);}
        } else if(!strcmp(operation,"stop"))primary=uwvm_scout_stop(&cap);
        else {
            primary=roswasmc_subscribe(&cap);
            if(!primary&&!strcmp(operation,"status")) {
                uwvm_scout_status value={0};primary=UWVM_SCOUT_EXAMPLE_STATUS(&cap,&value,&scratch);
                if(!primary)primary=UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutStatus),_encode)(&value,scratch.wire,sizeof scratch.wire,&used);
            } else if(!primary&&!strcmp(operation,"odom")) {
                uwvm_scout_odometry value={0};primary=uwvm_scout_poll_odometry(&cap,&value,&scratch);
                if(!primary)primary=UWVM_SCOUT_JOIN(UWVM_SCOUT_ODOMETRY,_encode)(&value,scratch.wire,sizeof scratch.wire,&used);
            } else if(!primary&&!strcmp(operation,"remote")) {
                uwvm_scout_remote value={0};primary=uwvm_scout_poll_remote(&cap,&value,&scratch);
                if(!primary)primary=UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(UWVM_SCOUT_REMOTE),_encode)(&value,scratch.wire,sizeof scratch.wire,&used);
            }
#if UWVM_SCOUT_ROS_VERSION == 1
            else if(!primary&&!strcmp(operation,"bms")) {
                uwvm_scout_bms value={0};primary=uwvm_scout_poll_bms(&cap,&value,&scratch);
                if(!primary)primary=UWVM_SCOUT_JOIN(UWVM_SCOUT_RECORD(ScoutBmsStatus),_encode)(&value,scratch.wire,sizeof scratch.wire,&used);
            }
#endif
            if(!primary)primary=uwvm_scout_example_write(scratch.wire,used);
        }
    }
    /* Closing this owner does not stop physical motion. All primary errors
     * remain primary; cleanup is still attempted exactly once after an open. */
    if(cap.handle)cleanup=roswasmc_close(&cap);
    if(!primary)primary=cleanup;
    return primary?(int)(primary<126?primary:125):0;
}
