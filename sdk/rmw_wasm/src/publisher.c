/* SPDX-License-Identifier: Apache-2.0
 * Real RMW publisher symbols over actual rosidl-generated objects. Creation
 * requires an independently implemented genuine native endpoint provider.
 * Capability open alone and Guest-declared QoS are never native attestation. */
#include "uwvm_rmw/internal.h"
#include <string.h>
#include <rmw/error_handling.h>
typedef struct uwvm_rmw_publisher {
  rmw_publisher_t public_handle; /* First: the admitted public address. */
  uwvm_rmw_resource * resource;
  roswasmc_capability * owner;
  uwvm_rmw_codec const * codec;
  unsigned char * scratch;
  uwvm_rmw_publisher_receipt receipt;
  int using_native;
} uwvm_rmw_publisher;
static int binding_valid(uwvm_rmw_publisher_receipt const * q,roswasmc_capability const * owner) {
  return q&&owner&&q->version==1&&q->backend==2&&q->source==owner->handle&&
    (uint32_t)q->source&&(uint32_t)(q->source>>32)&&q->instance&&
    q->runtime_generation&&q->session&&q->native_incarnation;
}
static int same_binding(uwvm_rmw_publisher_receipt const * a,uwvm_rmw_publisher_receipt const * b) {
  return a->version==b->version&&a->backend==b->backend&&a->source==b->source&&
    a->instance==b->instance&&a->runtime_generation==b->runtime_generation&&
    a->session==b->session&&a->native_incarnation==b->native_incarnation;
}
static int gid_valid(uwvm_rmw_publisher_receipt const * q) {
  /* Presence is the complete associated written receipt and exact extent.
   * Original RMW defines no universal all-zero invalid GID sentinel. */
  return q->native_gid_bytes==RMW_GID_STORAGE_SIZE;
}
static uint32_t retire_native(void * object) {
  uwvm_rmw_publisher * p=(uwvm_rmw_publisher *)object;
  uwvm_rmw_publisher_provider const * api=&p->resource->context->profile->publisher;
  /* B calls this exactly once, while the source is still open. Even a failed
   * retire leaves its real diagnostic and cannot skip the later raw close. */
  p->using_native=1;uint32_t raw=api->retire(api->state,p->owner,&p->receipt);
  p->using_native=0;return raw;
}
static uint32_t release(void * object) {
  uwvm_rmw_publisher * p=(uwvm_rmw_publisher *)object;
  rcutils_allocator_t a=p->resource->context->allocator;
  a.deallocate(p->scratch,a.state);a.deallocate(p,a.state);return 0;
}
rmw_publisher_t * rmw_create_publisher(rmw_node_t const * handle,
  rosidl_message_type_support_t const * support,char const * name,
  rmw_qos_profile_t const * qos,rmw_publisher_options_t const * options) {
 UWVM_RMW_ENTRY(NULL);
  uwvm_rmw_node * n=uwvm_rmw_node_find(handle,0);
  if(!n||!support||!name||!qos||!options){uwvm_rmw_error(n?n->context:NULL,22,"invalid RMW publisher arguments");return NULL;}
  uwvm_rmw_context * c=n->context;uwvm_rmw_scope scope;
  if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
  if(options->rmw_specific_publisher_payload||
    options->require_unique_network_flow_endpoints!=RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED) {
    uwvm_rmw_error(c,95,"publisher payload/unique network endpoint option is not implemented");goto fail;
  }
  uwvm_rmw_publisher_provider const * api=&c->profile->publisher;
  if(api->version!=1||!api->activate||!api->inspect||!api->publish||!api->retire) {
    uwvm_rmw_error(c,95,"real same-incarnation native publisher provider absent before open");goto fail;
  }
  uwvm_rmw_endpoint_selection const * row=uwvm_rmw_select(n,UWVM_RMW_PUBLISHER,name,qos,support);
  if(!row){uwvm_rmw_error(c,13,"exact node/topic/type/QoS publisher binding absent (raw13)");goto fail;}
  if(c->resource_count==UWVM_RMW_RESOURCE_LIMIT||c->session.count==UWVM_RMW_MAX_OWNERS) {
    uwvm_rmw_error(c,90,"bounded publisher/owner capacity exhausted");goto fail;
  }
  uwvm_rmw_codec const * codec=c->profile->codecs+row->codec_index;
  uwvm_rmw_publisher * p=c->allocator.zero_allocate(1,sizeof(*p),c->allocator.state);
  if(!p){uwvm_rmw_error(c,12,"publisher record allocation failed before effects");goto fail;}
  /* An actual Empty canonical body is0 bytes. Keep one private physical byte
   * without projecting a dummy byte into the logical canonical capacity. */
  size_t physical=codec->maximum_wire_bytes?codec->maximum_wire_bytes:1;
  p->scratch=c->allocator.allocate(physical,c->allocator.state);
  if(!p->scratch){c->allocator.deallocate(p,c->allocator.state);uwvm_rmw_error(c,12,"publisher codec scratch allocation failed before effects");goto fail;}
  p->codec=codec;p->public_handle.implementation_identifier=uwvm_rmw_identifier;
  p->public_handle.data=p;p->public_handle.topic_name=row->resolved_name;
  p->public_handle.options=*options;p->public_handle.can_loan_messages=false;
  /* Both once-only callback slots, actual receipt and diagnostics are retained
   * by the fixed resource BEFORE the first source/native activation effect. */
  p->resource=uwvm_rmw_resource_reserve(&scope,n,row,p,release);
  if(!p->resource) {
    c->allocator.deallocate(p->scratch,c->allocator.state);c->allocator.deallocate(p,c->allocator.state);
    uwvm_rmw_error(c,90,"publisher stable cleanup reservation refused before effects");goto fail;
  }
  uint32_t raw=uwvm_rmw_owner_open(&scope,p->resource,0,&p->owner);
  if(raw){uwvm_rmw_error(c,raw,"actual selected publisher capability open failed");(void)uwvm_rmw_resource_retire(&scope,p->resource);goto fail;}
  uwvm_rmw_publisher_receipt observed={0};p->using_native=1;
  raw=api->activate(api->state,p->owner,row,codec,options,&observed);p->using_native=0;
  if(binding_valid(&observed,p->owner)) {
    p->receipt=observed;p->resource->native_retire=retire_native;
  } else if(!raw)raw=22;
  if(!raw&&(!gid_valid(&observed)||!uwvm_rmw_qos_concrete(&observed.actual_qos)||
    !uwvm_rmw_qos_equal(&observed.actual_qos,&row->effective_qos)))raw=22;
  if(raw){uwvm_rmw_error(c,raw,"actual activation/associated QoS failed; native/raw/local cleanup retained");(void)uwvm_rmw_resource_retire(&scope,p->resource);goto fail;}
  if(uwvm_rmw_notify_local_graph(n)!=RMW_RET_OK) {
    uwvm_rmw_error(c,5,"local graph notification failed after genuine native publisher activation");
    (void)uwvm_rmw_resource_retire(&scope,p->resource);goto fail;
  }
  uwvm_rmw_scope_leave(&scope);return &p->public_handle;
fail:uwvm_rmw_scope_leave(&scope);return NULL;
}
rmw_ret_t rmw_destroy_publisher(rmw_node_t * handle,rmw_publisher_t * publisher) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_resource * r=uwvm_rmw_resource_find(publisher,UWVM_RMW_PUBLISHER,1);
  uwvm_rmw_node * n=uwvm_rmw_node_find(handle,1);
  if(!r||!n||r->node!=n){RMW_SET_ERROR_MSG("foreign publisher/node relationship");return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;}
  uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,1);if(code!=RMW_RET_OK)return code;
  uint32_t raw=uwvm_rmw_resource_retire(&scope,r);
  rmw_ret_t graph=uwvm_rmw_notify_local_graph(n);uwvm_rmw_scope_leave(&scope);
  if(raw||graph!=RMW_RET_OK){RMW_SET_ERROR_MSG("publisher native/raw/local cleanup failure retained separately");return RMW_RET_ERROR;}
  return RMW_RET_OK;
}
rmw_ret_t rmw_publish(rmw_publisher_t const * handle,void const * message,rmw_publisher_allocation_t * allocation) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_resource * r=uwvm_rmw_resource_find(handle,UWVM_RMW_PUBLISHER,0);
  if(!r||!message){RMW_SET_ERROR_MSG("invalid owned RMW publisher/message");return RMW_RET_INVALID_ARGUMENT;}
  uwvm_rmw_context * c=r->context;uwvm_rmw_publisher * p=(uwvm_rmw_publisher *)r->object;
  if(allocation)return uwvm_rmw_error(c,95,"native publisher allocation objects are not supported");
  if(p->using_native)return uwvm_rmw_error(c,17,"recursive publisher operation borrows scratch/receipt (raw17)");
  /* Actual RMW function scope only. No imaginary upper-rclcpp callback lease:
   * original upper-layer shared owners remain responsible for their lifetime. */
  uwvm_rmw_scope scope={0};
  if(!c->leased&&uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return RMW_RET_ERROR;
  p->using_native=1;size_t length=0;
  uint32_t raw=p->codec->encode(message,p->scratch,p->codec->maximum_wire_bytes,&length);
  if(!raw&&length>p->codec->maximum_wire_bytes)raw=22;
  if(!raw){uwvm_rmw_publisher_provider const * api=&c->profile->publisher;
    raw=api->publish(api->state,p->owner,&p->receipt,p->scratch,length);}
  p->using_native=0;uwvm_rmw_scope_leave(&scope);
  return raw?uwvm_rmw_error(c,raw,"canonical conversion or same-incarnation native publication failed; no replay"):RMW_RET_OK;
}
static rmw_ret_t inspect_retained(uwvm_rmw_resource * r,uwvm_rmw_publisher_receipt * observed) {
  uwvm_rmw_context * c=r->context;uwvm_rmw_publisher * p=r->object;
  if(p->using_native)return uwvm_rmw_error(c,17,"publisher metadata query is already borrowed");
  uwvm_rmw_scope scope={0};if(!c->leased&&uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return RMW_RET_ERROR;
  uwvm_rmw_publisher_provider const * api=&c->profile->publisher;
  p->using_native=1;uint32_t raw=api->inspect(api->state,p->owner,&p->receipt,observed);p->using_native=0;
  if(!raw&&(!binding_valid(observed,p->owner)||!same_binding(observed,&p->receipt)||
    !gid_valid(observed)||memcmp(observed->native_gid,p->receipt.native_gid,RMW_GID_STORAGE_SIZE)||
    !uwvm_rmw_qos_concrete(&observed->actual_qos)||!uwvm_rmw_qos_equal(&observed->actual_qos,&r->selection->effective_qos)))raw=22;
  uwvm_rmw_scope_leave(&scope);
  return raw?uwvm_rmw_error(c,raw,"actual native QoS/GID/binding query failed; caller output preserved"):RMW_RET_OK;
}
rmw_ret_t rmw_publisher_get_actual_qos(rmw_publisher_t const * handle,rmw_qos_profile_t * output) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_resource * r=uwvm_rmw_resource_find(handle,UWVM_RMW_PUBLISHER,0);
  if(!r||!output)return RMW_RET_INVALID_ARGUMENT;
  uwvm_rmw_publisher_receipt observed={0};rmw_ret_t rc=inspect_retained(r,&observed);
  if(rc==RMW_RET_OK)*output=observed.actual_qos;return rc;
}
rmw_ret_t rmw_get_gid_for_publisher(rmw_publisher_t const * handle,rmw_gid_t * output) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  uwvm_rmw_resource * r=uwvm_rmw_resource_find(handle,UWVM_RMW_PUBLISHER,0);
  if(!r||!output)return RMW_RET_INVALID_ARGUMENT;
  uwvm_rmw_publisher_receipt observed={0};rmw_ret_t rc=inspect_retained(r,&observed);
  if(rc!=RMW_RET_OK)return rc;
  rmw_gid_t next={0};next.implementation_identifier=uwvm_rmw_identifier;
  memcpy(next.data,observed.native_gid,RMW_GID_STORAGE_SIZE);*output=next;return RMW_RET_OK;
}
rmw_ret_t rmw_compare_gids_equal(rmw_gid_t const * a,rmw_gid_t const * b,bool * output) {
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
  if(!a||!b||!output)return RMW_RET_INVALID_ARGUMENT;
  if(a->implementation_identifier!=uwvm_rmw_identifier||b->implementation_identifier!=uwvm_rmw_identifier)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
  *output=!memcmp(a->data,b->data,RMW_GID_STORAGE_SIZE);return RMW_RET_OK;
}
