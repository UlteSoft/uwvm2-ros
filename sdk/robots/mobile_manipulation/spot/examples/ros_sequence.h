/** Concrete complete typed Spot ROS client. No Native SDK networking in Guest.
 * All eight owners are explicitly OPENED here and independently reverse-closed.
 * The report distinguishes transport/provider/unknown and every cleanup code.
 * Caller allocates this entire report + buffers before calling. No output is
 * interpreted after a failed transport. A Stop response is not physical EStop.
 */
#ifndef UWVM_SPOT_ROS_SEQUENCE_H
#define UWVM_SPOT_ROS_SEQUENCE_H
#include "../sdk.h"
#include <string.h>
#include <math.h>
typedef struct mm_spot_sequence_report {
    uint32_t transport[8],provider[8],cleanup[8],primary;
    unsigned char unknown[8],attempted[8];
    mm_spot_State_response telemetry;mm_spot_Lease_response lease,retained,returned;
    mm_spot_Command_response stood,stopped;
    mm_spot_Velocity_response velocity;mm_spot_Feedback_response feedback;
} mm_spot_sequence_report;
/** aliases: state, acquire, retain, stand, velocity, feedback, stop, return.
 * Each operation has its own genuine service.call alias/type/resource/grant.
 * Token and nonce are anti-replay values in the shared Native profile, not
 * Runtime identity. MAX is valid if the entire eight-value range fits.
 */
static inline uint32_t mm_spot_sequence(roswasmc_context const* context,char const* const aliases[8],
    uint64_t first_token,uint64_t first_nonce,double x,double y,double yaw,uint64_t duration_ns,
    mm_spot_sequence_report* report,mm_spot_buffers* buffers) {
    roswasmc_capability owners[8];unsigned i,j;uint32_t e=0;roswasmc_result r;
    mm_spot_State_request state={0};mm_spot_Lease_request lease={0};mm_spot_Command_request command={0};
    mm_spot_Velocity_request velocity={0};mm_spot_Feedback_request feedback={0};
    int acquired=0,command_attempted=0;
    if(!context||!aliases||!report||!buffers||!first_token||!first_nonce||
       first_token>UINT64_MAX-7||first_nonce>UINT64_MAX-7||
       !isfinite(x)||!isfinite(y)||!isfinite(yaw)||fabs(x)>0.3||fabs(y)>0.3||fabs(yaw)>0.5||
       !duration_ns||duration_ns>1000000000ull||
       uwvm_bounded_overlap(report,sizeof *report,buffers,sizeof *buffers))return 22;
    memset(report,0,sizeof *report);
    for(i=0;i<8;++i){roswasmc_capability_init(&owners[i],context);
        if(!aliases[i]||!*aliases[i]||strlen(aliases[i])>255)return 22;
        for(j=0;j<i;++j)if(!strcmp(aliases[i],aliases[j]))return 22;}
    for(i=0;i<8;++i){e=roswasmc_open(&owners[i],aliases[i],strlen(aliases[i]));if(e)goto cleanup;}
#define MM_CAPTURE(I,RESPONSE) do{report->transport[I]=r.error;report->attempted[I]=1; \
    if(!r.error){report->provider[I]=(RESPONSE).error;report->unknown[I]=(unsigned char)(RESPONSE).unknown;} \
    else report->unknown[I]=1;}while(0)
    state.token=first_token;state.nonce=first_nonce;state.budget_ns=2000000000ull;
    r=mm_spot_State_call(&owners[0],&state,&report->telemetry,buffers);MM_CAPTURE(0,report->telemetry);
    e=r.error?r.error:report->telemetry.error;if(e||report->telemetry.unknown){if(!e)e=115;goto cleanup;}
    lease.token=first_token+1;lease.nonce=first_nonce+1;lease.budget_ns=2000000000ull;
    r=mm_spot_Lease_call(&owners[1],&lease,&report->lease,buffers);MM_CAPTURE(1,report->lease);
    e=r.error?r.error:report->lease.error;if(e||report->lease.unknown){if(!e)e=115;goto cleanup;}acquired=1;
    lease.token=first_token+2;lease.nonce=first_nonce+2;
    r=mm_spot_Lease_call(&owners[2],&lease,&report->retained,buffers);MM_CAPTURE(2,report->retained);
    e=r.error?r.error:report->retained.error;if(e||report->retained.unknown){if(!e)e=115;goto native_cleanup;}
    command.token=first_token+3;command.nonce=first_nonce+3;command.budget_ns=2000000000ull;command_attempted=1;
    r=mm_spot_Command_call(&owners[3],&command,&report->stood,buffers);MM_CAPTURE(3,report->stood);
    e=r.error?r.error:report->stood.error;if(e||report->stood.unknown){if(!e)e=115;goto native_cleanup;}
    velocity.token=first_token+4;velocity.nonce=first_nonce+4;velocity.budget_ns=2000000000ull;
    velocity.x_mps=x;velocity.y_mps=y;velocity.yaw_rps=yaw;velocity.duration_ns=duration_ns;
    r=mm_spot_Velocity_call(&owners[4],&velocity,&report->velocity,buffers);MM_CAPTURE(4,report->velocity);
    e=r.error?r.error:report->velocity.error;if(e||report->velocity.unknown){if(!e)e=115;goto native_cleanup;}
    feedback.token=first_token+5;feedback.nonce=first_nonce+5;feedback.budget_ns=2000000000ull;
    feedback.command_token=first_token+4;
    r=mm_spot_Feedback_call(&owners[5],&feedback,&report->feedback,buffers);MM_CAPTURE(5,report->feedback);
    e=r.error?r.error:report->feedback.error;if(!e&&report->feedback.unknown)e=115;
native_cleanup:
    /* Independent explicitly selected Stop/Return calls, not inverse rollback,
       cancellation of an uncertain previous call or proof of physical state. */
    if(acquired&&command_attempted){command.token=first_token+6;command.nonce=first_nonce+6;
        r=mm_spot_Command_call(&owners[6],&command,&report->stopped,buffers);MM_CAPTURE(6,report->stopped);}
    if(acquired){lease.token=first_token+7;lease.nonce=first_nonce+7;
        r=mm_spot_Lease_call(&owners[7],&lease,&report->returned,buffers);MM_CAPTURE(7,report->returned);}
#undef MM_CAPTURE
cleanup:
    report->primary=e;
    for(i=8;i>0;--i)if(owners[i-1].handle)report->cleanup[i-1]=roswasmc_close(&owners[i-1]);
    return e;
}
#endif
