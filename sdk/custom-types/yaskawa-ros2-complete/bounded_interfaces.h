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
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_call(roswasmc_capability const* cap,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request const* request,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response* response) {
unsigned char input[4164],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_encode(request,input,4164,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request* request) {
unsigned char output[4172];roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_call(roswasmc_capability const* cap,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request const* request,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response* response) {
unsigned char input[1],output[32768];size_t n=0;roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,32768);if(r.error)return r;
if(!r.has_length||r.length>32768||roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* response) {
unsigned char input[32776];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_encode(response,input+8,32768,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_call(roswasmc_capability const* cap,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request const* request,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_call(roswasmc_capability const* cap,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request const* request,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_StartMotion_call(roswasmc_capability const* cap,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request const* request,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_StartMotion_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_StartMotion_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_StopMotion_call(roswasmc_capability const* cap,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request const* request,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_StopMotion_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_industrial_msgs_srv_StopMotion_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response* response) {
unsigned char input[1],output[32768];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,32768);if(r.error)return r;
if(!r.has_length||r.length>32768||roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* response) {
unsigned char input[32776];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_encode(response,input+8,32768,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response* response) {
unsigned char input[1],output[8780];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8780);if(r.error)return r;
if(!r.has_length||r.length>8780||roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response const* response) {
unsigned char input[8788];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_encode(response,input+8,8780,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response* response) {
unsigned char input[1228],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_encode(request,input,1228,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request* request) {
unsigned char output[1236];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response* response) {
unsigned char input[4],output[522];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_encode(request,input,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,522);if(r.error)return r;
if(!r.has_length||r.length>522||roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request* request) {
unsigned char output[12];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response const* response) {
unsigned char input[530];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_encode(response,input+8,522,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response* response) {
unsigned char input[4],output[523];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_encode(request,input,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,523);if(r.error)return r;
if(!r.has_length||r.length>523||roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request* request) {
unsigned char output[12];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response const* response) {
unsigned char input[531];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_encode(response,input+8,523,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response* response) {
unsigned char input[4],output[525];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_encode(request,input,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,525);if(r.error)return r;
if(!r.has_length||r.length>525||roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request* request) {
unsigned char output[12];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response const* response) {
unsigned char input[533];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_encode(response,input+8,525,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response* response) {
unsigned char input[8],output[521];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_encode(request,input,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,521);if(r.error)return r;
if(!r.has_length||r.length>521||roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request* request) {
unsigned char output[16];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response const* response) {
unsigned char input[529];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_encode(response,input+8,521,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response* response) {
unsigned char input[5],output[521];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_encode(request,input,5,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,521);if(r.error)return r;
if(!r.has_length||r.length>521||roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request* request) {
unsigned char output[13];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response const* response) {
unsigned char input[529];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_encode(response,input+8,521,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response* response) {
unsigned char input[6],output[521];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_encode(request,input,6,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,521);if(r.error)return r;
if(!r.has_length||r.length>521||roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request* request) {
unsigned char output[14];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response const* response) {
unsigned char input[529];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_encode(response,input+8,521,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request const* request,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response* response) {
unsigned char input[8],output[521];size_t n=0;roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_encode(request,input,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,521);if(r.error)return r;
if(!r.has_length||r.length>521||roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request* request) {
unsigned char output[16];roswasmc_result r;roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response const* response) {
unsigned char input[529];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_encode(response,input+8,521,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
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
