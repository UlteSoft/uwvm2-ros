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
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse* response) {
unsigned char input[260],output[261];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_encode(request,input,260,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,261);if(r.error)return r;
if(!r.has_length||r.length>261||roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest* request) {
unsigned char output[268];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_AddToLog_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse const* response) {
unsigned char input[269];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_encode(response,input+8,261,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse* response) {
unsigned char input[1],output[521];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,521);if(r.error)return r;
if(!r.has_length||r.length>521||roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgram_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse const* response) {
unsigned char input[529];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_encode(response,input+8,521,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse* response) {
unsigned char input[1],output[781];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,781);if(r.error)return r;
if(!r.has_length||r.length>781||roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetProgramState_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse const* response) {
unsigned char input[789];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_encode(response,input+8,781,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse* response) {
unsigned char input[1],output[262];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,262);if(r.error)return r;
if(!r.has_length||r.length>262||roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetRobotMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse const* response) {
unsigned char input[270];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_encode(response,input+8,262,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse* response) {
unsigned char input[1],output[262];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,262);if(r.error)return r;
if(!r.has_length||r.length>262||roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse const* response) {
unsigned char input[270];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_encode(response,input+8,262,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse* response) {
unsigned char input[1],output[262];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,262);if(r.error)return r;
if(!r.has_length||r.length>262||roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControl_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse const* response) {
unsigned char input[270];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_encode(response,input+8,262,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse* response) {
unsigned char input[1],output[262];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,262);if(r.error)return r;
if(!r.has_length||r.length>262||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunning_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse const* response) {
unsigned char input[270];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_encode(response,input+8,262,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse* response) {
unsigned char input[1],output[522];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,522);if(r.error)return r;
if(!r.has_length||r.length>522||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSaved_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse const* response) {
unsigned char input[530];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_encode(response,input+8,522,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_Load_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse* response) {
unsigned char input[260],output[261];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_encode(request,input,260,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,261);if(r.error)return r;
if(!r.has_length||r.length>261||roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_Load_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest* request) {
unsigned char output[268];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_Load_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse const* response) {
unsigned char input[269];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_encode(response,input+8,261,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_Popup_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse* response) {
unsigned char input[260],output[261];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_encode(request,input,260,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,261);if(r.error)return r;
if(!r.has_length||r.length>261||roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_Popup_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest* request) {
unsigned char output[268];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_Popup_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse const* response) {
unsigned char input[269];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_encode(response,input+8,261,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest const* request,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse* response) {
unsigned char input[260],output[260];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_encode(request,input,260,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,260);if(r.error)return r;
if(!r.has_length||r.length>260||roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest* request) {
unsigned char output[268];roswasmc_result r;roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_RawRequest_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse const* response) {
unsigned char input[268];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_encode(response,input+8,260,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Send one complete custom Goal using an explicitly initialized owned role bundle. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_SetMode_send(roswasmc_action_bundle* bundle,roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal const* goal,uint64_t* id) {
unsigned char input[3],output[8];size_t n=0;uint64_t next;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,id,sizeof *id))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(bundle->count==bundle->capacity)return roswasmc_action_bundle_failure(UWVM_ROS_TOO_LARGE);
if(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_encode(goal,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_action_send_goal(&bundle->cap[0],input,n,output,sizeof output);if(r.error)return r;
if(!r.has_length||r.length!=8||(next=uwvm_bounded_get(output,8))==0||roswasmc_action_bundle_owns(bundle,next)){r.error=UWVM_ROS_INVALID;return r;}bundle->goals[bundle->count++]=next;*id=next;return r;}
/** Read custom feedback only for a locally owned exact Goal ID; idle leaves output untouched. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_SetMode_feedback(roswasmc_action_bundle const* bundle,uint64_t id,roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback* feedback) {
unsigned char input[8],output[10];roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback next={0};roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,feedback,sizeof *feedback))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);
r=roswasmc_action_receive_feedback(&bundle->cap[2],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*feedback=next;return r;}
/** A validated terminal result removes exactly one local Goal; cancel acknowledgement does not. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_SetMode_result(roswasmc_action_bundle* bundle,uint64_t id,uint8_t* kind,roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult* result) {
unsigned char input[8],output[270];roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult next={0};size_t i;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,kind,sizeof *kind)||!roswasmc_action_bundle_destination(bundle,result,sizeof *result)||uwvm_bounded_overlap(kind,sizeof *kind,result,sizeof *result))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);
r=roswasmc_action_receive_result(&bundle->cap[3],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<9||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||output[8]<2||output[8]>4||roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_decode(output+9,(size_t)r.length-9,&next)){r.error=UWVM_ROS_INVALID;return r;}for(i=0;i<bundle->count;++i)if(bundle->goals[i]==id){--bundle->count;bundle->goals[i]=bundle->goals[bundle->count];bundle->goals[bundle->count]=0;break;}*kind=output[8];*result=next;return r;}
/** Poll one incoming custom Goal under an independently activated advertise alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_SetMode_poll(roswasmc_capability const* cap,uint64_t* id,roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal* goal) {
unsigned char output[11];roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal next={0};uint64_t selected;roswasmc_result r;if(!id||!goal||!uwvm_bounded_cap_destination(cap,id,sizeof *id)||!uwvm_bounded_cap_destination(cap,goal,sizeof *goal)||uwvm_bounded_overlap(id,sizeof *id,goal,sizeof *goal))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(selected=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*id=selected;*goal=next;return r;}
/** Send one typed custom feedback; host owns ticket, deadline and one-shot terminal state. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_SetMode_respond_feedback(roswasmc_capability const* cap,uint64_t id,roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback const* value) {unsigned char input[11];size_t n=0;roswasmc_result r;if(!id||roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_encode(value,input+9,2,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]=1;r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Send one typed custom result; host owns ticket, deadline and one-shot terminal state. */
static inline roswasmc_result roswasmc_bounded_1_ur_dashboard_msgs_SetMode_respond_result(roswasmc_capability const* cap,uint64_t id,uint8_t kind,roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult const* value) {unsigned char input[270];size_t n=0;roswasmc_result r;if(!id||kind<2||kind>4||roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_encode(value,input+9,261,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]=kind;r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest const* request,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersion_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetAnalogOutput_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest const* request,roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse* response) {
unsigned char input[6],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_encode(request,input,6,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetAnalogOutput_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest* request) {
unsigned char output[14];roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetAnalogOutput_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetForceMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_msgs_SetForceModeRequest const* request,roswasmc_bounded_1_ur_msgs_SetForceModeResponse* response) {
unsigned char input[335],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetForceModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_msgs_SetForceModeRequest_encode(request,input,335,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ur_msgs_SetForceModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetForceMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_msgs_SetForceModeRequest* request) {
unsigned char output[343];roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetForceModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_msgs_SetForceModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetForceMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_msgs_SetForceModeResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_msgs_SetForceModeResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetIO_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_msgs_SetIORequest const* request,roswasmc_bounded_1_ur_msgs_SetIOResponse* response) {
unsigned char input[6],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetIOResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_msgs_SetIORequest_encode(request,input,6,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ur_msgs_SetIOResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetIO_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_msgs_SetIORequest* request) {
unsigned char output[14];roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetIORequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_msgs_SetIORequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetIO_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_msgs_SetIOResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_msgs_SetIOResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetPayload_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_msgs_SetPayloadRequest const* request,roswasmc_bounded_1_ur_msgs_SetPayloadResponse* response) {
unsigned char input[28],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetPayloadResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_msgs_SetPayloadRequest_encode(request,input,28,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ur_msgs_SetPayloadResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetPayload_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_msgs_SetPayloadRequest* request) {
unsigned char output[36];roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetPayloadRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_msgs_SetPayloadRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetPayload_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_msgs_SetPayloadResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_msgs_SetPayloadResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_call(roswasmc_capability const* cap,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest const* request,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse* response) {
unsigned char input[8],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_encode(request,input,8,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest* request) {
unsigned char output[16];roswasmc_result r;roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ur_msgs_SetSpeedSliderFraction_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
