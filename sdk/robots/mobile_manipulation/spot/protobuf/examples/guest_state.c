/* SPDX-License-Identifier: Apache-2.0
 * argv: STATE_ALIAS; explicit FD0: token(u64LE),nonce(u64LE),budget_ns(u64LE).
 * No credentials/target/lease/motion defaults. All storage precedes open. */
#include <wasi/api.h>
#include <roswasm/ros2.h>
#include "../../sdk.h"
#include "robot_state.h"
#include <stdalign.h>
#include <string.h>
static alignas(max_align_t) unsigned char bank0[1048576],bank1[1048576];
static uwvm_spot_proto_document state;
static mm_spot_buffers buffers;
static mm_spot_State_response response;
static struct {uint32_t primary,transport,provider,unknown,decode,cleanup;} report;
static int exact(unsigned char *p,size_t n) {
    while(n) {
        __wasi_iovec_t iov={p,n};__wasi_size_t got=0;
        if(__wasi_fd_read(0,&iov,1,&got)||!got||got>n)return 0;
        p+=got;n-=got;
    }
    return 1;
}
int main(int argc,char **argv) {
    unsigned char input[24];mm_spot_State_request request={0};
    roswasmc_context context={0};roswasmc_capability source;uint32_t code;
    if(argc!=2||!argv[1]||!argv[1][0]||strlen(argv[1])>255||!exact(input,sizeof input))return 22;
    request.token=uwvm_ros_load_u64(input);request.nonce=uwvm_ros_load_u64(input+8);
    request.budget_ns=uwvm_ros_load_u64(input+16);
    if(!request.token||!request.nonce||!request.budget_ns||request.budget_ns>5000000000ull)return 22;
    code=uwvm_spot_proto_init(&state,bank0,bank1,sizeof bank0);if(code)return (int)code;
    context.backend=roswasmc_ros2_backend();roswasmc_capability_init(&source,&context);
    code=roswasmc_open(&source,argv[1],strlen(argv[1]));
    report.primary=code;
    if(!code) {
        roswasmc_result result=mm_spot_State_call(&source,&request,&response,&buffers);
        report.transport=result.error;
        if(result.error)report.primary=result.error;
        else {
            report.provider=response.error;report.unknown=response.unknown;
            report.primary=response.error?response.error:(response.unknown?115:0);
            if(!report.primary) {
                report.decode=uwvm_spot_robot_state_decode(&state,response.native_proto.data,response.native_proto.size);
                report.primary=report.decode;
                /* Full generated value remains live for genuine debugger reads:
                 * state.value -> bosdyn_api_RobotState with every original field.
                 * An EStopState field is telemetry, not physical EStop authority. */
            }
        }
    }
    if(source.handle)report.cleanup=roswasmc_close(&source);
    if(!report.primary)report.primary=report.cleanup;
    return report.primary?(int)(report.primary<126?report.primary:125):0;
}
