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
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest const* request,roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse* response) {
unsigned char input[1],output[52];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,52);if(r.error)return r;
if(!r.has_length||r.length>52||roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse const* response) {
unsigned char input[60];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_encode(response,input+8,52,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest const* request,roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest const* request,roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest const* request,roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest const* request,roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse* response) {
unsigned char input[53],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_encode(request,input,53,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest* request) {
unsigned char output[61];roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest const* request,roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mybuddy_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mecharm_communication_GetAnglesRequest const* request,roswasmc_bounded_1_mecharm_communication_GetAnglesResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mecharm_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mecharm_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mecharm_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mecharm_communication_GetAnglesResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mecharm_communication_GetCoordsRequest const* request,roswasmc_bounded_1_mecharm_communication_GetCoordsResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mecharm_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mecharm_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mecharm_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mecharm_communication_GetCoordsResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mecharm_communication_GripperStatusRequest const* request,roswasmc_bounded_1_mecharm_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mecharm_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mecharm_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mecharm_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mecharm_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mecharm_communication_PumpStatusRequest const* request,roswasmc_bounded_1_mecharm_communication_PumpStatusResponse* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mecharm_communication_PumpStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mecharm_communication_PumpStatusRequest* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_1_mecharm_communication_PumpStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mecharm_communication_PumpStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mecharm_communication_SetAnglesRequest const* request,roswasmc_bounded_1_mecharm_communication_SetAnglesResponse* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mecharm_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mecharm_communication_SetAnglesRequest* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_1_mecharm_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mecharm_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mecharm_communication_SetCoordsRequest const* request,roswasmc_bounded_1_mecharm_communication_SetCoordsResponse* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mecharm_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mecharm_communication_SetCoordsRequest* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_1_mecharm_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mecharm_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mecharm_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_myarm_communication_GetAnglesRequest const* request,roswasmc_bounded_1_myarm_communication_GetAnglesResponse* response) {
unsigned char input[1],output[28];size_t n=0;roswasmc_result r;roswasmc_bounded_1_myarm_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_myarm_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,28);if(r.error)return r;
if(!r.has_length||r.length>28||roswasmc_bounded_1_myarm_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_myarm_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_myarm_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_myarm_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_myarm_communication_GetAnglesResponse const* response) {
unsigned char input[36];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_myarm_communication_GetAnglesResponse_encode(response,input+8,28,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_myarm_communication_GetCoordsRequest const* request,roswasmc_bounded_1_myarm_communication_GetCoordsResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_myarm_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_myarm_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_myarm_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_myarm_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_myarm_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_myarm_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_myarm_communication_GetCoordsResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_myarm_communication_GetCoordsResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_myarm_communication_GripperStatusRequest const* request,roswasmc_bounded_1_myarm_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_myarm_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_myarm_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_myarm_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_myarm_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_myarm_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_myarm_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_myarm_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_myarm_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_myarm_communication_PumpStatusRequest const* request,roswasmc_bounded_1_myarm_communication_PumpStatusResponse* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_myarm_communication_PumpStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_myarm_communication_PumpStatusRequest_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_myarm_communication_PumpStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_myarm_communication_PumpStatusRequest* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_1_myarm_communication_PumpStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_myarm_communication_PumpStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_myarm_communication_PumpStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_myarm_communication_PumpStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_myarm_communication_SetAnglesRequest const* request,roswasmc_bounded_1_myarm_communication_SetAnglesResponse* response) {
unsigned char input[29],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_myarm_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_myarm_communication_SetAnglesRequest_encode(request,input,29,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_myarm_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_myarm_communication_SetAnglesRequest* request) {
unsigned char output[37];roswasmc_result r;roswasmc_bounded_1_myarm_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_myarm_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_myarm_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_myarm_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_myarm_communication_SetCoordsRequest const* request,roswasmc_bounded_1_myarm_communication_SetCoordsResponse* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_myarm_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_myarm_communication_SetCoordsRequest_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_myarm_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_myarm_communication_SetCoordsRequest* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_1_myarm_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_myarm_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_myarm_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_myarm_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_myarm_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest const* request,roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest const* request,roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest const* request,roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest const* request,roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest const* request,roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest const* request,roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_320_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_GetAnglesRequest const* request,roswasmc_bounded_1_mycobot_communication_GetAnglesResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_GetAnglesResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_GetCoordsRequest const* request,roswasmc_bounded_1_mycobot_communication_GetCoordsResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_GetCoordsResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_GripperStatusRequest const* request,roswasmc_bounded_1_mycobot_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_PumpStatusRequest const* request,roswasmc_bounded_1_mycobot_communication_PumpStatusResponse* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_PumpStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_PumpStatusRequest* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_PumpStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_PumpStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_SetAnglesRequest const* request,roswasmc_bounded_1_mycobot_communication_SetAnglesResponse* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_SetAnglesRequest* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_SetCoordsRequest const* request,roswasmc_bounded_1_mycobot_communication_SetCoordsResponse* response) {
unsigned char input[26],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_encode(request,input,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_SetCoordsRequest* request) {
unsigned char output[34];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetEndType_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest const* request,roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetEndType_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetEndType_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetFreshMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest const* request,roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetFreshMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetFreshMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetToolReference_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest const* request,roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse* response) {
unsigned char input[24],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_encode(request,input,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetToolReference_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest* request) {
unsigned char output[32];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetToolReference_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetVisionMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest const* request,roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetVisionMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_communication_SetVisionMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse* response) {
unsigned char input[1],output[24];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,24);if(r.error)return r;
if(!r.has_length||r.length>24||roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse const* response) {
unsigned char input[32];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_encode(response,input+8,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValue_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse* response) {
unsigned char input[1],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValue_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValue_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse* response) {
unsigned char input[25],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_encode(request,input,25,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest* request) {
unsigned char output[33];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetFreshMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest const* request,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetFreshMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mycobot_pro450_communication_SetFreshMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest const* request,roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest const* request,roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest const* request,roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_PumpStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest const* request,roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse* response) {
unsigned char input[3],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_encode(request,input,3,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_PumpStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest* request) {
unsigned char output[11];roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_PumpStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest const* request,roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse* response) {
unsigned char input[17],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_encode(request,input,17,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest* request) {
unsigned char output[25];roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest const* request,roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse* response) {
unsigned char input[18],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_encode(request,input,18,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest* request) {
unsigned char output[26];roswasmc_result r;roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_mypalletizer_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest const* request,roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest const* request,roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse* response) {
unsigned char input[1],output[16];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16);if(r.error)return r;
if(!r.has_length||r.length>16||roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse const* response) {
unsigned char input[24];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_encode(response,input+8,16,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetGripperValue_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest const* request,roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse* response) {
unsigned char input[1],output[4];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_encode(request,input,0,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4);if(r.error)return r;
if(!r.has_length||r.length>4||roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetGripperValue_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest* request) {
unsigned char output[8];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GetGripperValue_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse const* response) {
unsigned char input[12];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_encode(response,input+8,4,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GripperStatus_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest const* request,roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GripperStatus_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_GripperStatus_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetAngles_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest const* request,roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse* response) {
unsigned char input[20],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_encode(request,input,20,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetAngles_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest* request) {
unsigned char output[28];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetAngles_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetCoords_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest const* request,roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse* response) {
unsigned char input[20],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_encode(request,input,20,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetCoords_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest* request) {
unsigned char output[28];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetCoords_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetFreshMode_call(roswasmc_capability const* cap,roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest const* request,roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse* response) {
unsigned char input[1],output[1];size_t n=0;roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_encode(request,input,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,1);if(r.error)return r;
if(!r.has_length||r.length>1||roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetFreshMode_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest* request) {
unsigned char output[9];roswasmc_result r;roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_1_ultraarm_communication_SetFreshMode_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse const* response) {
unsigned char input[9];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_encode(response,input+8,1,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
