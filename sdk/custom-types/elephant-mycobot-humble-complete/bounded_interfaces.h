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
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request const* request,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request const* request,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request const* request,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request const* request,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request const* request,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request const* request,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request const* request,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request const* request,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request const* request,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request const* request,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request const* request,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request const* request,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request const* request,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request const* request,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request const* request,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request const* request,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request const* request,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request const* request,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request const* request,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request const* request,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request const* request,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request const* request,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request const* request,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response* response) {
unsigned char input[17],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_encode(request,input,17,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request* request) {
unsigned char output[25];roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request const* request,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response* response) {
unsigned char input[17],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_encode(request,input,17,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request* request) {
unsigned char output[25];roswasmc_result r;roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Request const* request,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Response* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Response const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetAngles_Response_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Request const* request,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Response* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Response const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetCoords_Response_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_call(roswasmc_capability const* cap,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Request const* request,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Request_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Request* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GetGripperValue_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Request const* request,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Response* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Request_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Request* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_GripperStatus_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Request const* request,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Response* response) {
unsigned char input[20],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Request_encode(request,input,20,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Request* request) {
unsigned char output[28];roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetAngles_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Request const* request,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Response* response) {
unsigned char input[20],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Request_encode(request,input,20,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Request* request) {
unsigned char output[28];roswasmc_result r;roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Response const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_ultraarm_p1_interfaces_srv_SetCoords_Response_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
