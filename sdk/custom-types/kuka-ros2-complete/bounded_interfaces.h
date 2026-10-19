/** @file Complete typed custom Service/Action operations over existing C capabilities.
 * Generated canonical components are distinct from Runtime ticket/outcome envelopes.
 * No alias is opened here. For Action clients initialize one existing action_bundle
 * with explicit send/cancel-own/feedback/result aliases (optional cancel-any), and
 * keep its context and goal table stable. Cancel and checked close/revoke use that
 * bundle unchanged. Idle11 is not success; retain owners for caller-driven polling.
 * All outputs commit only after exact framing, matching IDs and component validation. */
#ifndef UWVM_BOUNDED_INTERFACES_H
#define UWVM_BOUNDED_INTERFACES_H
#include "bounded_records.h"
#include <roswasm/action_bundle.h>
/** Caller destinations cannot overwrite the stable owner/context/backend. Live C object storage remains caller responsibility. */
static inline int uwvm_bounded_cap_destination(roswasmc_capability const* cap,void const* out,size_t size){roswasmc_context const* context=cap?cap->context:NULL;return cap&&roswasmc_action_bundle_range(out,size)&&!roswasmc_action_bundle_overlap(out,size,cap,sizeof *cap)&&(!context||(!roswasmc_action_bundle_overlap(out,size,context,sizeof *context)&&(!context->backend||!roswasmc_action_bundle_overlap(out,size,context->backend,sizeof *context->backend))));}
/** Send one complete custom Goal using an explicitly initialized owned role bundle. */
static inline roswasmc_result roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_send(roswasmc_action_bundle* bundle,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal const* goal,uint64_t* id) {
unsigned char input[20920],output[8];size_t n=0;uint64_t next;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,id,sizeof *id))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(bundle->count==bundle->capacity)return roswasmc_action_bundle_failure(UWVM_ROS_TOO_LARGE);
if(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(goal,input,20920,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_action_send_goal(&bundle->cap[0],input,n,output,sizeof output);if(r.error)return r;
if(!r.has_length||r.length!=8||(next=uwvm_bounded_get(output,8))==0||roswasmc_action_bundle_owns(bundle,next)){r.error=UWVM_ROS_INVALID;return r;}bundle->goals[bundle->count++]=next;*id=next;return r;}
/** Read custom feedback only for a locally owned exact Goal ID; idle leaves output untouched. */
static inline roswasmc_result roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_feedback(roswasmc_action_bundle const* bundle,uint64_t id,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback* feedback) {
unsigned char input[8],output[4136];roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback next={0};roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,feedback,sizeof *feedback))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);
r=roswasmc_action_receive_feedback(&bundle->cap[2],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*feedback=next;return r;}
/** A validated terminal result removes exactly one local Goal; cancel acknowledgement does not. */
static inline roswasmc_result roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_result(roswasmc_action_bundle* bundle,uint64_t id,uint8_t* kind,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result* result) {
unsigned char input[8],output[4113];roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result next={0};size_t i;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,kind,sizeof *kind)||!roswasmc_action_bundle_destination(bundle,result,sizeof *result)||uwvm_bounded_overlap(kind,sizeof *kind,result,sizeof *result))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);
r=roswasmc_action_receive_result(&bundle->cap[3],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<9||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||output[8]<2||output[8]>4||roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_decode(output+9,(size_t)r.length-9,&next)){r.error=UWVM_ROS_INVALID;return r;}for(i=0;i<bundle->count;++i)if(bundle->goals[i]==id){--bundle->count;bundle->goals[i]=bundle->goals[bundle->count];bundle->goals[bundle->count]=0;break;}*kind=output[8];*result=next;return r;}
/** Poll one incoming custom Goal under an independently activated advertise alias. */
static inline roswasmc_result roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_poll(roswasmc_capability const* cap,uint64_t* id,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal* goal) {
unsigned char output[20928];roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal next={0};uint64_t selected;roswasmc_result r;if(!id||!goal||!uwvm_bounded_cap_destination(cap,id,sizeof *id)||!uwvm_bounded_cap_destination(cap,goal,sizeof *goal)||uwvm_bounded_overlap(id,sizeof *id,goal,sizeof *goal))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(selected=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*id=selected;*goal=next;return r;}
/** Send one typed custom feedback; host owns ticket, deadline and one-shot terminal state. */
static inline roswasmc_result roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_respond_feedback(roswasmc_capability const* cap,uint64_t id,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback const* value) {unsigned char input[4137];size_t n=0;roswasmc_result r;if(!id||roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_encode(value,input+9,4128,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]=1;r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Send one typed custom result; host owns ticket, deadline and one-shot terminal state. */
static inline roswasmc_result roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_respond_result(roswasmc_capability const* cap,uint64_t id,uint8_t kind,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result const* value) {unsigned char input[4113];size_t n=0;roswasmc_result r;if(!id||kind<2||kind>4||roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_encode(value,input+9,4104,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]=kind;r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_call(roswasmc_capability const* cap,roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request const* request,roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response* response) {
unsigned char input[517],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request_encode(request,input,517,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request* request) {
unsigned char output[525];roswasmc_result r;roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_lifecycle_msgs_srv_ChangeState_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_lifecycle_msgs_srv_GetState_call(roswasmc_capability const* cap,roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request const* request,roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response* response) {
unsigned char input[1],output[517];size_t n=0;roswasmc_result r;roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,517);if(r.error)return r;
if(!r.has_length||r.length>517||roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_lifecycle_msgs_srv_GetState_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_lifecycle_msgs_srv_GetState_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response const* response) {
unsigned char input[525];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_lifecycle_msgs_srv_GetState_Response_encode(response,input+8,517,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_std_srvs_srv_Trigger_call(roswasmc_capability const* cap,roswasmc_bounded_2_std_srvs_srv_Trigger_Request const* request,roswasmc_bounded_2_std_srvs_srv_Trigger_Response* response) {
unsigned char input[1],output[517];size_t n=0;roswasmc_result r;roswasmc_bounded_2_std_srvs_srv_Trigger_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_std_srvs_srv_Trigger_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,517);if(r.error)return r;
if(!r.has_length||r.length>517||roswasmc_bounded_2_std_srvs_srv_Trigger_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_std_srvs_srv_Trigger_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_std_srvs_srv_Trigger_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_std_srvs_srv_Trigger_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_std_srvs_srv_Trigger_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_std_srvs_srv_Trigger_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_std_srvs_srv_Trigger_Response const* response) {
unsigned char input[525];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_std_srvs_srv_Trigger_Response_encode(response,input+8,517,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
