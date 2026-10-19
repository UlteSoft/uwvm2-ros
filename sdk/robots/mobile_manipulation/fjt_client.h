/** Complete typed C11 FJT over the actual existing ROS ABI.
 * Requires actual `bounded_records.h` from the selected complete vendor IR.
 * No native layout casts, hand-written substitute message types or grant flag.
 * All four capability owners are BORROWED and must outlive the caller/operation.
 * Caller serializes this object and owner retirement; no callback is invoked.
 */
#ifndef UWVM_MOBILE_FJT_CLIENT_H
#define UWVM_MOBILE_FJT_CLIENT_H
#include <roswasm/roswasmc.h>
#include "bounded_records.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal mm_fjt_goal;
typedef roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback mm_fjt_feedback;
typedef roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result mm_fjt_result;
typedef struct mm_fjt_entry { uint64_t id; uint32_t state,error; } mm_fjt_entry;
typedef struct mm_fjt_client {
    roswasmc_capability const* roles[4]; /* send, feedback, result, cancel */
    mm_fjt_entry history[32]; uint32_t used,closed,busy;
    unsigned char wire[32768+9];
} mm_fjt_client;
#if defined(__cplusplus)
static_assert(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_MAX_WIRE_BYTES<=32768);
static_assert(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_MAX_WIRE_BYTES<=32768);
static_assert(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MAX_WIRE_BYTES<=32768);
#else
_Static_assert(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_MAX_WIRE_BYTES<=32768,"bounded complete goal required");
_Static_assert(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_MAX_WIRE_BYTES<=32768,"bounded complete feedback required");
_Static_assert(ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MAX_WIRE_BYTES<=32768,"bounded complete result required");
#endif
static inline void mm_fjt_put_u64(unsigned char* output,uint64_t value) {
    unsigned i;for(i=0;i<8;++i)output[i]=(unsigned char)(value>>(8*i));
}
static inline uint32_t mm_fjt_init(mm_fjt_client* output,roswasmc_capability const* send,
    roswasmc_capability const* feedback,roswasmc_capability const* result,roswasmc_capability const* cancel) {
    mm_fjt_client next={0};unsigned i,j;
    next.roles[0]=send;next.roles[1]=feedback;next.roles[2]=result;next.roles[3]=cancel;
    if(!output||!send||!send->context)return 22;
    if(!send->context->backend||send->context->backend->version!=2)return 95;
    if(output->roles[0]||output->used||output->closed||output->busy)return 17;
    for(i=0;i<4;++i) {
        if(!roswasmc_is_open(next.roles[i])||next.roles[i]->context!=send->context)return 9;
        for(j=0;j<i;++j)if(next.roles[j]->handle==next.roles[i]->handle)return 22;
    }
    *output=next;return 0;
}
static inline uint32_t mm_fjt_enter(mm_fjt_client* client) {
    unsigned i;if(!client)return 22;if(client->closed)return 125;if(client->busy)return 17;
    for(i=0;i<4;++i)if(!roswasmc_is_open(client->roles[i]))return 9;
    client->busy=1;return 0;
}
/** Submit the ENTIRE original Goal. out_index is committed only on success.
 * Row state 1=unknown,2=accepted,3=cancel requested,4=terminal. Never reuse rows.
 */
static inline uint32_t mm_fjt_send(mm_fjt_client* client,mm_fjt_goal const* goal,uint32_t* out_index) {
    uint32_t e,i;size_t n=0;roswasmc_result r;mm_fjt_entry* row;
    if(!out_index||!client||uwvm_bounded_overlap(out_index,sizeof *out_index,client,sizeof *client))return 22;e=mm_fjt_enter(client);if(e)return e;
    if(client->used==32){client->busy=0;return 90;}
    e=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(goal,client->wire,32768,&n);
    if(e){client->busy=0;return e;}
    row=&client->history[client->used++];row->state=1;
    {unsigned char output[8]={0};
      r=roswasmc_action_send_goal(client->roles[0],client->wire,n,output,sizeof output);
      e=r.error;if(!e&&(!r.has_length||r.length!=8))e=22;
      if(!e){uint64_t id=uwvm_ros_load_u64(output);if(!id)e=22;
        for(i=0;i+1<client->used;++i)if(client->history[i].id==id)e=22;
        if(!e){row->id=id;row->state=2;*out_index=client->used-1;}}
    }
    row->error=e;client->busy=0;return e;
}
static inline uint32_t mm_fjt_live(mm_fjt_client* client,uint32_t index) {
    if(!client)return 22;
    if(index>=client->used||!client->history[index].id)return 9;
    return client->history[index].state==2||client->history[index].state==3?0:125;
}
/** Cancel only the exact owned goal. Acceptance is NOT physical completion. */
static inline uint32_t mm_fjt_cancel(mm_fjt_client* client,uint32_t index,int* accepted) {
    uint32_t e;unsigned char input[8],output[1]={0};roswasmc_result r;
    if(!accepted||!client||uwvm_bounded_overlap(accepted,sizeof *accepted,client,sizeof *client))return 22;e=mm_fjt_enter(client);if(e)return e;
    e=mm_fjt_live(client,index);if(e){client->busy=0;return e;}
    mm_fjt_put_u64(input,client->history[index].id);
    r=roswasmc_action_cancel_own_goal(client->roles[3],input,8,output,1);e=r.error;
    if(!e&&(!r.has_length||r.length!=1||output[0]>1))e=22;
    if(e){client->history[index].state=1;client->history[index].error=e;}
    else{*accepted=(int)output[0];if(*accepted)client->history[index].state=3;}
    client->busy=0;return e;
}
static inline uint32_t mm_fjt_feedback_once(mm_fjt_client* client,uint32_t index,mm_fjt_feedback* output) {
    uint32_t e;unsigned char input[8];roswasmc_result r;
    if(!output||!client||uwvm_bounded_overlap(output,sizeof *output,client,sizeof *client))return 22;e=mm_fjt_enter(client);if(e)return e;
    e=mm_fjt_live(client,index);if(e){client->busy=0;return e;}
    mm_fjt_put_u64(input,client->history[index].id);
    r=roswasmc_action_receive_feedback(client->roles[1],input,8,client->wire,sizeof client->wire);e=r.error;
    if(!e&&(!r.has_length||r.length<8||uwvm_ros_load_u64(client->wire)!=client->history[index].id))e=22;
    if(!e)e=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_decode(client->wire+8,(size_t)r.length-8,output);
    if(e!=0&&e!=11){client->history[index].state=1;client->history[index].error=e;}
    client->busy=0;return e;
}
static inline uint32_t mm_fjt_result_once(mm_fjt_client* client,uint32_t index,uint8_t* status,mm_fjt_result* output) {
    uint32_t e;unsigned char input[8];roswasmc_result r;
    if(!output||!status||!client||uwvm_bounded_overlap(output,sizeof *output,client,sizeof *client)||
       uwvm_bounded_overlap(status,sizeof *status,client,sizeof *client)||uwvm_bounded_overlap(status,sizeof *status,output,sizeof *output))return 22;e=mm_fjt_enter(client);if(e)return e;
    e=mm_fjt_live(client,index);if(e){client->busy=0;return e;}
    mm_fjt_put_u64(input,client->history[index].id);
    r=roswasmc_action_receive_result(client->roles[2],input,8,client->wire,sizeof client->wire);e=r.error;
    if(!e&&(!r.has_length||r.length<9||uwvm_ros_load_u64(client->wire)!=client->history[index].id||client->wire[8]<2||client->wire[8]>4))e=22;
    if(!e)e=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_decode(client->wire+9,(size_t)r.length-9,output);
    if(!e){*status=client->wire[8];client->history[index].state=4;}
    else if(e!=11){client->history[index].state=1;client->history[index].error=e;}
    client->busy=0;return e;
}
/** Local retirement only; the actual owner must close all four raw handles. */
static inline uint32_t mm_fjt_retire_local(mm_fjt_client* client) {
    if(!client)return 22;if(client->busy)return 17;client->closed=1;return 0;
}
#ifdef __cplusplus
}
#endif
#endif
