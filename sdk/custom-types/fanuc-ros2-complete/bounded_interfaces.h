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
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response* response) {
unsigned char input[8],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_encode(request,input,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request* request) {
unsigned char output[16];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response* response) {
unsigned char input[70],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_encode(request,input,70,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request* request) {
unsigned char output[78];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response* response) {
unsigned char input[70],output[5];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_encode(request,input,70,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,5);if(r.error)return r;
if(!r.has_length||r.length>5||roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request* request) {
unsigned char output[78];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response const* response) {
unsigned char input[13];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_encode(response,input+8,5,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response* response) {
unsigned char input[1],output[54];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,54);if(r.error)return r;
if(!r.has_length||r.length>54||roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response const* response) {
unsigned char input[62];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_encode(response,input+8,54,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response* response) {
unsigned char input[70],output[6];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_encode(request,input,70,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,6);if(r.error)return r;
if(!r.has_length||r.length>6||roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request* request) {
unsigned char output[78];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response const* response) {
unsigned char input[14];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_encode(response,input+8,6,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response* response) {
unsigned char input[2],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response* response) {
unsigned char input[2],output[32];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,32);if(r.error)return r;
if(!r.has_length||r.length>32||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response const* response) {
unsigned char input[40];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_encode(response,input+8,32,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response* response) {
unsigned char input[1],output[6];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,6);if(r.error)return r;
if(!r.has_length||r.length>6||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response const* response) {
unsigned char input[14];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_encode(response,input+8,6,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response* response) {
unsigned char input[2],output[32];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,32);if(r.error)return r;
if(!r.has_length||r.length>32||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response const* response) {
unsigned char input[40];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_encode(response,input+8,32,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response* response) {
unsigned char input[2],output[601];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,601);if(r.error)return r;
if(!r.has_length||r.length>601||roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response const* response) {
unsigned char input[609];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_encode(response,input+8,601,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response* response) {
unsigned char input[1],output[13];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,13);if(r.error)return r;
if(!r.has_length||r.length>13||roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response const* response) {
unsigned char input[21];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_encode(response,input+8,13,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response* response) {
unsigned char input[2],output[30];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,30);if(r.error)return r;
if(!r.has_length||r.length>30||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response const* response) {
unsigned char input[38];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_encode(response,input+8,30,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response* response) {
unsigned char input[1],output[7];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,7);if(r.error)return r;
if(!r.has_length||r.length>7||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response const* response) {
unsigned char input[15];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_encode(response,input+8,7,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response* response) {
unsigned char input[2],output[30];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,30);if(r.error)return r;
if(!r.has_length||r.length>30||roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response const* response) {
unsigned char input[38];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_encode(response,input+8,30,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_ReadError_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response* response) {
unsigned char input[1],output[2585];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,2585);if(r.error)return r;
if(!r.has_length||r.length>2585||roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_ReadError_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_ReadError_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response const* response) {
unsigned char input[2593];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_encode(response,input+8,2585,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_Reset_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response* response) {
unsigned char input[1],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_Reset_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_Reset_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response* response) {
unsigned char input[74],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_encode(request,input,74,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request* request) {
unsigned char output[82];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response* response) {
unsigned char input[71],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_encode(request,input,71,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request* request) {
unsigned char output[79];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response* response) {
unsigned char input[1],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response* response) {
unsigned char input[72],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_encode(request,input,72,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request* request) {
unsigned char output[80];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response* response) {
unsigned char input[6],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_encode(request,input,6,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request* request) {
unsigned char output[14];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response* response) {
unsigned char input[30],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_encode(request,input,30,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request* request) {
unsigned char output[38];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response* response) {
unsigned char input[2],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_encode(request,input,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request* request) {
unsigned char output[10];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response* response) {
unsigned char input[31],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_encode(request,input,31,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request* request) {
unsigned char output[39];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response* response) {
unsigned char input[599],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_encode(request,input,599,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request* request) {
unsigned char output[607];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response* response) {
unsigned char input[26],output[5];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,5);if(r.error)return r;
if(!r.has_length||r.length>5||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response const* response) {
unsigned char input[13];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_encode(response,input+8,5,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response* response) {
unsigned char input[3],output[5];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,5);if(r.error)return r;
if(!r.has_length||r.length>5||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response const* response) {
unsigned char input[13];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_encode(response,input+8,5,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response* response) {
unsigned char input[26],output[5];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,5);if(r.error)return r;
if(!r.has_length||r.length>5||roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response const* response) {
unsigned char input[13];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_encode(response,input+8,5,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_call(roswasmc_capability const* cap,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request const* request,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response* response) {
unsigned char input[4],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_encode(request,input,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request* request) {
unsigned char output[12];roswasmc_result r;roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddCall_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response* response) {
unsigned char input[9992],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_encode(request,input,9992,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddCall_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request* request) {
unsigned char output[10000];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddCall_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response* response) {
unsigned char input[4014],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_encode(request,input,4014,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request* request) {
unsigned char output[4022];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response* response) {
unsigned char input[1],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response* response) {
unsigned char input[1],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response* response) {
unsigned char input[1],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response* response) {
unsigned char input[518],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_encode(request,input,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request* request) {
unsigned char output[526];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response* response) {
unsigned char input[4],output[8];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_encode(request,input,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,8);if(r.error)return r;
if(!r.has_length||r.length>8||roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request* request) {
unsigned char output[12];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response const* response) {
unsigned char input[16];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_encode(response,input+8,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_CallCommand_call(roswasmc_capability const* cap,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request const* request,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response* response) {
unsigned char input[1104],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_encode(request,input,1104,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_CallCommand_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request* request) {
unsigned char output[1112];roswasmc_result r;roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_rmi_msgs_srv_CallCommand_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
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
