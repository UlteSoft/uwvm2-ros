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
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest const* request,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse* response) {
unsigned char input[68],output[1285];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_encode(request,input,68,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1285);if(r.error)return r;
if(!r.has_length||r.length>1285||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest* request) {
unsigned char output[76];roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettings_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse const* response) {
unsigned char input[1293];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_encode(response,input+8,1285,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest const* request,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse* response) {
unsigned char input[835],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_encode(request,input,835,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest* request) {
unsigned char output[843];roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettings_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest const* request,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse* response) {
unsigned char input[584],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_encode(request,input,584,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest* request) {
unsigned char output[592];roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutine_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest const* request,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse* response) {
unsigned char input[73],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_encode(request,input,73,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest* request) {
unsigned char output[81];roswasmc_result r;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommand_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetFileContents_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse* response) {
unsigned char input[516],output[4618];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_encode(request,input,516,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4618);if(r.error)return r;
if(!r.has_length||r.length>4618||roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetFileContents_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest* request) {
unsigned char output[524];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetFileContents_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse const* response) {
unsigned char input[4626];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_encode(response,input+8,4618,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse* response) {
unsigned char input[516],output[1034];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_encode(request,input,516,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1034);if(r.error)return r;
if(!r.has_length||r.length>1034||roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest* request) {
unsigned char output[524];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetIOSignal_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse const* response) {
unsigned char input[1042];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_encode(response,input+8,1034,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse* response) {
unsigned char input[204],output[519];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,519);if(r.error)return r;
if(!r.has_length||r.length>519||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse const* response) {
unsigned char input[527];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_encode(response,input+8,519,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse* response) {
unsigned char input[204],output[526];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,526);if(r.error)return r;
if(!r.has_length||r.length>526||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse const* response) {
unsigned char input[534];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_encode(response,input+8,526,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse* response) {
unsigned char input[204],output[522];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,522);if(r.error)return r;
if(!r.has_length||r.length>522||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse const* response) {
unsigned char input[530];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_encode(response,input+8,522,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse* response) {
unsigned char input[204],output[1034];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1034);if(r.error)return r;
if(!r.has_length||r.length>1034||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDString_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse const* response) {
unsigned char input[1042];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_encode(response,input+8,1034,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse* response) {
unsigned char input[204],output[1034];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1034);if(r.error)return r;
if(!r.has_length||r.length>1034||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbol_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse const* response) {
unsigned char input[1042];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_encode(response,input+8,1034,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse* response) {
unsigned char input[1],output[4618];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4618);if(r.error)return r;
if(!r.has_length||r.length>4618||roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescription_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse const* response) {
unsigned char input[4626];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_encode(response,input+8,4618,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest const* request,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse* response) {
unsigned char input[1],output[519];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,519);if(r.error)return r;
if(!r.has_length||r.length>519||roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatio_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse const* response) {
unsigned char input[527];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_encode(response,input+8,519,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetFileContents_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse* response) {
unsigned char input[4616],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_encode(request,input,4616,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetFileContents_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest* request) {
unsigned char output[4624];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetFileContents_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse* response) {
unsigned char input[1032],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_encode(request,input,1032,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest* request) {
unsigned char output[1040];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetIOSignal_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse* response) {
unsigned char input[205],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_encode(request,input,205,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest* request) {
unsigned char output[213];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse* response) {
unsigned char input[212],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_encode(request,input,212,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest* request) {
unsigned char output[220];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse* response) {
unsigned char input[208],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_encode(request,input,208,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest* request) {
unsigned char output[216];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse* response) {
unsigned char input[720],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_encode(request,input,720,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest* request) {
unsigned char output[728];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDString_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse* response) {
unsigned char input[720],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_encode(request,input,720,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest* request) {
unsigned char output[728];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbol_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest const* request,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatio_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_call(roswasmc_capability const* cap,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest const* request,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Send one complete custom Goal using an explicitly initialized owned role bundle. */
static inline roswasmc_result roswasmc_bounded_1_control_msgs_FollowJointTrajectory_send(roswasmc_action_bundle* bundle,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal const* goal,uint64_t* id) {
unsigned char input[6392],output[8];size_t n=0;uint64_t next;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,id,sizeof *id))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(bundle->count==bundle->capacity)return roswasmc_action_bundle_failure(UWVM_ROS_TOO_LARGE);
if(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_encode(goal,input,6392,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_action_send_goal(&bundle->cap[0],input,n,output,sizeof output);if(r.error)return r;
if(!r.has_length||r.length!=8||(next=uwvm_bounded_get(output,8))==0||roswasmc_action_bundle_owns(bundle,next)){r.error=UWVM_ROS_INVALID;return r;}bundle->goals[bundle->count++]=next;*id=next;return r;}
/** Read custom feedback only for a locally owned exact Goal ID; idle leaves output untouched. */
static inline roswasmc_result roswasmc_bounded_1_control_msgs_FollowJointTrajectory_feedback(roswasmc_action_bundle const* bundle,uint64_t id,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback* feedback) {
unsigned char input[8],output[2132];roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback next={0};roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,feedback,sizeof *feedback))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);
r=roswasmc_action_receive_feedback(&bundle->cap[2],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*feedback=next;return r;}
/** A validated terminal result removes exactly one local Goal; cancel acknowledgement does not. */
static inline roswasmc_result roswasmc_bounded_1_control_msgs_FollowJointTrajectory_result(roswasmc_action_bundle* bundle,uint64_t id,uint8_t* kind,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult* result) {
unsigned char input[8],output[4113];roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult next={0};size_t i;roswasmc_result r;uint32_t error=roswasmc_action_bundle_check(bundle);if(error)return roswasmc_action_bundle_failure(error);
if(!roswasmc_action_bundle_destination(bundle,kind,sizeof *kind)||!roswasmc_action_bundle_destination(bundle,result,sizeof *result)||uwvm_bounded_overlap(kind,sizeof *kind,result,sizeof *result))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);if(!roswasmc_action_bundle_owns(bundle,id))return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);uwvm_bounded_put(input,id,8);
r=roswasmc_action_receive_result(&bundle->cap[3],input,8,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<9||r.length>sizeof output||uwvm_bounded_get(output,8)!=id||output[8]<2||output[8]>4||roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_decode(output+9,(size_t)r.length-9,&next)){r.error=UWVM_ROS_INVALID;return r;}for(i=0;i<bundle->count;++i)if(bundle->goals[i]==id){--bundle->count;bundle->goals[i]=bundle->goals[bundle->count];bundle->goals[bundle->count]=0;break;}*kind=output[8];*result=next;return r;}
/** Poll one incoming custom Goal under an independently activated advertise alias. */
static inline roswasmc_result roswasmc_bounded_1_control_msgs_FollowJointTrajectory_poll(roswasmc_capability const* cap,uint64_t* id,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal* goal) {
unsigned char output[6400];roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal next={0};uint64_t selected;roswasmc_result r;if(!id||!goal||!uwvm_bounded_cap_destination(cap,id,sizeof *id)||!uwvm_bounded_cap_destination(cap,goal,sizeof *goal)||uwvm_bounded_overlap(id,sizeof *id,goal,sizeof *goal))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(selected=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*id=selected;*goal=next;return r;}
/** Send one typed custom feedback; host owns ticket, deadline and one-shot terminal state. */
static inline roswasmc_result roswasmc_bounded_1_control_msgs_FollowJointTrajectory_respond_feedback(roswasmc_capability const* cap,uint64_t id,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback const* value) {unsigned char input[2133];size_t n=0;roswasmc_result r;if(!id||roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_encode(value,input+9,2124,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]=1;r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Send one typed custom result; host owns ticket, deadline and one-shot terminal state. */
static inline roswasmc_result roswasmc_bounded_1_control_msgs_FollowJointTrajectory_respond_result(roswasmc_capability const* cap,uint64_t id,uint8_t kind,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult const* value) {unsigned char input[4113];size_t n=0;roswasmc_result r;if(!id||kind<2||kind>4||roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_encode(value,input+9,4104,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,id,8);input[8]=kind;r=roswasmc_action_respond(cap,input,n+9,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
