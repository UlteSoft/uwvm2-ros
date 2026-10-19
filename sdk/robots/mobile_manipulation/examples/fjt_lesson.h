/** Real C11 caller: complete telemetry -> explicit Action owners -> full Goal.
 * Operator supplies actual complete goal and real per-import credit function.
 * Credit bounds work, not elapsed time/readiness; caller owns clock grants.
 */
#ifndef UWVM_MOBILE_FJT_LESSON_H
#define UWVM_MOBILE_FJT_LESSON_H
#include "../fjt_goal.h"
typedef struct mm_fjt_report {
    uint32_t primary,cleanup[5];uint64_t native_goal_id;uint8_t terminal_status;
    roswasmc_bounded_2_sensor_msgs_msg_JointState telemetry;
    mm_fjt_feedback feedback;mm_fjt_result result;
} mm_fjt_report;
typedef int (*mm_credit_fn)(void*);
/** aliases order telemetry/send/feedback/result/cancel, all independently bound.
 * Entire report/storage exists before opens; close errors never overwrite primary.
 * This is actual SDK code, not a callback table standing in for native ROS.
 */
static inline uint32_t mm_fjt_lesson(roswasmc_context const* context,char const* const aliases[5],
    mm_fjt_goal const* goal,mm_credit_fn credit,void* state,mm_fjt_report* report) {
    roswasmc_capability owners[5];mm_fjt_client client={0};unsigned char wire[32768];
    uint32_t i,e=0,index=0,loops=0;roswasmc_result r;int c,accepted=0;
    if(!context||!context->backend||!aliases||!goal||!credit||!report)return 22;
    for(i=0;i<5;++i){unsigned j;report->cleanup[i]=0;roswasmc_capability_init(&owners[i],context);
        if(!aliases[i]||!*aliases[i]||strlen(aliases[i])>255)return 22;
        for(j=0;j<i;++j)if(!strcmp(aliases[i],aliases[j]))return 22;}
    report->primary=0;report->native_goal_id=0;
    e=roswasmc_open(&owners[0],aliases[0],strlen(aliases[0]));if(e)goto cleanup;
    e=roswasmc_subscribe(&owners[0]);if(e)goto cleanup;
    if(credit(state)!='T'){e=22;goto cleanup;}
    r=roswasmc_poll(&owners[0],wire,sizeof wire);e=r.error;if(e)goto cleanup;
    if(!r.has_length){e=22;goto cleanup;}
    e=roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(wire,(size_t)r.length,&report->telemetry);if(e)goto cleanup;
    for(i=1;i<5;++i){e=roswasmc_open(&owners[i],aliases[i],strlen(aliases[i]));if(e)goto cleanup;}
    e=mm_fjt_init(&client,&owners[1],&owners[2],&owners[3],&owners[4]);if(e)goto cleanup;
    if(credit(state)!='S'){e=22;goto cleanup;}
    e=mm_fjt_send(&client,goal,&index);if(e)goto cleanup;
    report->native_goal_id=client.history[index].id;
    while(loops++<64){
        c=credit(state);
        if(c=='F')e=mm_fjt_feedback_once(&client,index,&report->feedback);
        else if(c=='R') {
            e=mm_fjt_result_once(&client,index,&report->terminal_status,&report->result);
            if(!e)goto cleanup;
        } else if(c=='C')e=mm_fjt_cancel(&client,index,&accepted);
        else {e=22;goto cleanup;}
        if(e&&e!=11)goto cleanup;
    }
    e=90; /* finite work exhausted, unknown; this is not an elapsed deadline */
cleanup:
    report->primary=e;(void)mm_fjt_retire_local(&client);
    for(i=5;i>0;--i)if(owners[i-1].handle)report->cleanup[i-1]=roswasmc_close(&owners[i-1]);
    return e;
}
#endif
