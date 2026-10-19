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
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request const* request,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response* response) {
unsigned char input[68],output[1285];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_encode(request,input,68,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1285);if(r.error)return r;
if(!r.has_length||r.length>1285||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request* request) {
unsigned char output[76];roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response const* response) {
unsigned char input[1293];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_encode(response,input+8,1285,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request const* request,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response* response) {
unsigned char input[835],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_encode(request,input,835,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request* request) {
unsigned char output[843];roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request const* request,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response* response) {
unsigned char input[584],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_encode(request,input,584,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request* request) {
unsigned char output[592];roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request const* request,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response* response) {
unsigned char input[73],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_encode(request,input,73,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request* request) {
unsigned char output[81];roswasmc_result r;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response* response) {
unsigned char input[516],output[4618];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_encode(request,input,516,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4618);if(r.error)return r;
if(!r.has_length||r.length>4618||roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request* request) {
unsigned char output[524];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response const* response) {
unsigned char input[4626];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_encode(response,input+8,4618,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response* response) {
unsigned char input[516],output[1034];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_encode(request,input,516,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1034);if(r.error)return r;
if(!r.has_length||r.length>1034||roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request* request) {
unsigned char output[524];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response const* response) {
unsigned char input[1042];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_encode(response,input+8,1034,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response* response) {
unsigned char input[204],output[519];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,519);if(r.error)return r;
if(!r.has_length||r.length>519||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response const* response) {
unsigned char input[527];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_encode(response,input+8,519,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response* response) {
unsigned char input[204],output[526];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,526);if(r.error)return r;
if(!r.has_length||r.length>526||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response const* response) {
unsigned char input[534];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_encode(response,input+8,526,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response* response) {
unsigned char input[204],output[522];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,522);if(r.error)return r;
if(!r.has_length||r.length>522||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response const* response) {
unsigned char input[530];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_encode(response,input+8,522,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response* response) {
unsigned char input[204],output[1034];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1034);if(r.error)return r;
if(!r.has_length||r.length>1034||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response const* response) {
unsigned char input[1042];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_encode(response,input+8,1034,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response* response) {
unsigned char input[204],output[1034];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_encode(request,input,204,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1034);if(r.error)return r;
if(!r.has_length||r.length>1034||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request* request) {
unsigned char output[212];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response const* response) {
unsigned char input[1042];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_encode(response,input+8,1034,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response* response) {
unsigned char input[1],output[4618];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4618);if(r.error)return r;
if(!r.has_length||r.length>4618||roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response const* response) {
unsigned char input[4626];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_encode(response,input+8,4618,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response* response) {
unsigned char input[1],output[519];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,519);if(r.error)return r;
if(!r.has_length||r.length>519||roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response const* response) {
unsigned char input[527];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_encode(response,input+8,519,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response* response) {
unsigned char input[4616],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_encode(request,input,4616,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request* request) {
unsigned char output[4624];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response* response) {
unsigned char input[1032],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_encode(request,input,1032,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request* request) {
unsigned char output[1040];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response* response) {
unsigned char input[205],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_encode(request,input,205,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request* request) {
unsigned char output[213];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response* response) {
unsigned char input[212],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_encode(request,input,212,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request* request) {
unsigned char output[220];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response* response) {
unsigned char input[208],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_encode(request,input,208,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request* request) {
unsigned char output[216];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response* response) {
unsigned char input[720],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_encode(request,input,720,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request* request) {
unsigned char output[728];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response* response) {
unsigned char input[720],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_encode(request,input,720,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request* request) {
unsigned char output[728];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_call(roswasmc_capability const* cap,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request const* request,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response* response) {
unsigned char input[1],output[518];size_t n=0;roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,518);if(r.error)return r;
if(!r.has_length||r.length>518||roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response const* response) {
unsigned char input[526];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_encode(response,input+8,518,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
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
