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
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_call(roswasmc_capability const* cap,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request const* request,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response* response) {
unsigned char input[24],output[26];size_t n=0;roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_encode(request,input,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,26);if(r.error)return r;
if(!r.has_length||r.length>26||roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request* request) {
unsigned char output[32];roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response const* response) {
unsigned char input[34];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_uwvm_spot_bridge_srv_Command_Response_encode(response,input+8,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_call(roswasmc_capability const* cap,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request const* request,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response* response) {
unsigned char input[32],output[16429];size_t n=0;roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_encode(request,input,32,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,16429);if(r.error)return r;
if(!r.has_length||r.length>16429||roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request* request) {
unsigned char output[40];roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response const* response) {
unsigned char input[16437];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_uwvm_spot_bridge_srv_Feedback_Response_encode(response,input+8,16429,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_call(roswasmc_capability const* cap,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request const* request,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response* response) {
unsigned char input[24],output[22];size_t n=0;roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_encode(request,input,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,22);if(r.error)return r;
if(!r.has_length||r.length>22||roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request* request) {
unsigned char output[32];roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response const* response) {
unsigned char input[30];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_uwvm_spot_bridge_srv_Lease_Response_encode(response,input+8,22,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_State_call(roswasmc_capability const* cap,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request const* request,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response* response) {
unsigned char input[24],output[21433];size_t n=0;roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_encode(request,input,24,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,21433);if(r.error)return r;
if(!r.has_length||r.length>21433||roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_State_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request* request) {
unsigned char output[32];roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_State_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response const* response) {
unsigned char input[21441];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_uwvm_spot_bridge_srv_State_Response_encode(response,input+8,21433,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_call(roswasmc_capability const* cap,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request const* request,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response* response) {
unsigned char input[56],output[26];size_t n=0;roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_encode(request,input,56,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,26);if(r.error)return r;
if(!r.has_length||r.length>26||roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request* request) {
unsigned char output[64];roswasmc_result r;roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response const* response) {
unsigned char input[34];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_uwvm_spot_bridge_srv_Velocity_Response_encode(response,input+8,26,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
