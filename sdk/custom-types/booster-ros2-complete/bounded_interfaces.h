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
static inline roswasmc_result roswasmc_bounded_2_booster_interface_srv_AgentService_call(roswasmc_capability const* cap,roswasmc_bounded_2_booster_interface_srv_AgentService_Request const* request,roswasmc_bounded_2_booster_interface_srv_AgentService_Response* response) {
unsigned char input[4100],output[4100];size_t n=0;roswasmc_result r;roswasmc_bounded_2_booster_interface_srv_AgentService_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_booster_interface_srv_AgentService_Request_encode(request,input,4100,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4100);if(r.error)return r;
if(!r.has_length||r.length>4100||roswasmc_bounded_2_booster_interface_srv_AgentService_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_booster_interface_srv_AgentService_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_booster_interface_srv_AgentService_Request* request) {
unsigned char output[4108];roswasmc_result r;roswasmc_bounded_2_booster_interface_srv_AgentService_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_booster_interface_srv_AgentService_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_booster_interface_srv_AgentService_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_booster_interface_srv_AgentService_Response const* response) {
unsigned char input[4108];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_booster_interface_srv_AgentService_Response_encode(response,input+8,4100,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
/** Call one explicitly authorized custom Service exactly once; no replay. */
static inline roswasmc_result roswasmc_bounded_2_booster_interface_srv_RpcService_call(roswasmc_capability const* cap,roswasmc_bounded_2_booster_interface_srv_RpcService_Request const* request,roswasmc_bounded_2_booster_interface_srv_RpcService_Response* response) {
unsigned char input[4108],output[4108];size_t n=0;roswasmc_result r;roswasmc_bounded_2_booster_interface_srv_RpcService_Response next={0};
if(!response||!uwvm_bounded_cap_destination(cap,response,sizeof *response)||roswasmc_bounded_2_booster_interface_srv_RpcService_Request_encode(request,input,4108,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_service_call(cap,input,n,output,4108);if(r.error)return r;
if(!r.has_length||r.length>4108||roswasmc_bounded_2_booster_interface_srv_RpcService_Response_decode(output,(size_t)r.length,&next)){r.error=UWVM_ROS_INVALID;return r;}*response=next;return r;}
/** Poll one native request; returned nonzero ticket is never a capability handle. */
static inline roswasmc_result roswasmc_bounded_2_booster_interface_srv_RpcService_poll(roswasmc_capability const* cap,uint64_t* ticket,roswasmc_bounded_2_booster_interface_srv_RpcService_Request* request) {
unsigned char output[4116];roswasmc_result r;roswasmc_bounded_2_booster_interface_srv_RpcService_Request next={0};uint64_t id;
if(!ticket||!request||!uwvm_bounded_cap_destination(cap,ticket,sizeof *ticket)||!uwvm_bounded_cap_destination(cap,request,sizeof *request)||uwvm_bounded_overlap(ticket,sizeof *ticket,request,sizeof *request))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
r=roswasmc_poll(cap,output,sizeof output);if(r.error)return r;if(!r.has_length||r.length<8||r.length>sizeof output||(id=uwvm_bounded_get(output,8))==0||roswasmc_bounded_2_booster_interface_srv_RpcService_Request_decode(output+8,(size_t)r.length-8,&next)){r.error=UWVM_ROS_INVALID;return r;}*ticket=id;*request=next;return r;}
/** Respond to one owned live ticket using the independently granted respond alias. */
static inline roswasmc_result roswasmc_bounded_2_booster_interface_srv_RpcService_respond(roswasmc_capability const* cap,uint64_t ticket,roswasmc_bounded_2_booster_interface_srv_RpcService_Response const* response) {
unsigned char input[4116];size_t n=0;roswasmc_result r;if(!ticket||roswasmc_bounded_2_booster_interface_srv_RpcService_Response_encode(response,input+8,4108,&n))return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);uwvm_bounded_put(input,ticket,8);r=roswasmc_service_respond(cap,input,n+8,NULL,0);if(!r.error&&(!r.has_length||r.length))r.error=UWVM_ROS_INVALID;return r;}
#endif
