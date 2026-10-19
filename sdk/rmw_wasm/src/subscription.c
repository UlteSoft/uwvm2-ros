/* SPDX-License-Identifier: Apache-2.0
 * Genuine rmw_subscription_t over selected real generated objects. This module
 * never invokes an application callback or retains a fictitious callback lease.
 * The original upper rcl/rclcpp owner remains responsible for its real lifetime. */
#include "c_internal.h"
typedef struct {
 rmw_subscription_t public_handle;
 uwvm_rmw_resource*resource;roswasmc_capability*owner;uwvm_rmw_codec const*codec;
 unsigned char*canonical;size_t length;int queued,has_info,poisoned;uint32_t error;
 rmw_message_info_t info;uwvm_rmw_c_native_endpoint native;
} local_subscription;
static uint32_t native_retire(void*object){
 local_subscription*s=object;uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(!s->native.native_incarnation)return 0;
 return p->activation.retire(p->activation.state,s->owner,NULL,&s->native);
}
static uint32_t release(void*object){
 local_subscription*s=object;rcutils_allocator_t a=s->resource->context->allocator;
 a.deallocate(s->canonical,a.state);a.deallocate(s,a.state);return 0;
}
rmw_subscription_t*rmw_create_subscription(rmw_node_t const*handle,
 rosidl_message_type_support_t const*support,char const*name,rmw_qos_profile_t const*qos,rmw_subscription_options_t const*options){
 UWVM_RMW_ENTRY(NULL);
 uwvm_rmw_node*n=uwvm_rmw_node_find(handle,0);
 if(!n||!support||!name||!qos||!options){uwvm_rmw_error(n?n->context:NULL,22,"invalid actual RMW subscription arguments");return NULL;}
 uwvm_rmw_context*c=n->context;uwvm_rmw_scope scope;
 if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(!p||p->activation.version!=1||p->metadata.version!=1||!p->metadata.take_topic){uwvm_rmw_error(c,95,"actual native activation/QoS provider is required before subscription effects");goto fail;}
 if(options->rmw_specific_subscription_payload||options->ignore_local_publications||options->content_filter_options||
  options->require_unique_network_flow_endpoints!=RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED){
  uwvm_rmw_error(c,95,"subscription payload/local-source filtering/content filter/unique flow is not implemented");goto fail;
 }
 uwvm_rmw_endpoint_selection const*row=uwvm_rmw_select(n,UWVM_RMW_SUBSCRIPTION,name,qos,support);
 if(!row){uwvm_rmw_error(c,13,"exact selected node/topic/generated type/requested QoS subscription binding absent");goto fail;}
 if(c->resource_count==UWVM_RMW_RESOURCE_LIMIT||c->session.count==UWVM_RMW_MAX_OWNERS){uwvm_rmw_error(c,90,"bounded subscription/owner slots exhausted before effects");goto fail;}
 uwvm_rmw_codec const*codec=c->profile->codecs+row->codec_index;
 uwvm_rmw_c_mark_started();local_subscription*s=c->allocator.zero_allocate(1,sizeof*s,c->allocator.state);
 if(!s){uwvm_rmw_error(c,12,"subscription record allocation failed before native effects");goto fail;}
 s->canonical=c->allocator.allocate(codec->maximum_wire_bytes?codec->maximum_wire_bytes:1u,c->allocator.state);
 if(!s->canonical){c->allocator.deallocate(s,c->allocator.state);uwvm_rmw_error(c,12,"complete canonical queue allocation failed before native effects");goto fail;}
 s->codec=codec;s->public_handle.implementation_identifier=uwvm_rmw_identifier;s->public_handle.data=s;
 s->public_handle.topic_name=row->resolved_name;s->public_handle.options=*options;
 s->public_handle.can_loan_messages=false;s->public_handle.is_cft_enabled=false;
 s->resource=uwvm_rmw_resource_reserve(&scope,n,row,s,release);
 if(!s->resource){c->allocator.deallocate(s->canonical,c->allocator.state);c->allocator.deallocate(s,c->allocator.state);uwvm_rmw_error(c,90,"stable subscription cleanup reservation failed before effects");goto fail;}
 s->resource->native_retire=native_retire;
 uint32_t raw=uwvm_rmw_owner_open(&scope,s->resource,0,&s->owner);
 if(!raw)raw=p->activation.activate(p->activation.state,UWVM_RMW_SUBSCRIPTION,s->owner,NULL,codec,qos,&s->native);
 if(!raw)raw=uwvm_rmw_c_endpoint_valid(&s->native,s->owner,1);
 if(!raw&&!uwvm_rmw_qos_equal(s->native.actual_qos,&row->effective_qos))raw=13;
 if(raw){uwvm_rmw_error(c,raw,"actual subscription activation/native QoS attestation failed");(void)uwvm_rmw_resource_retire(&scope,s->resource);goto fail;}
 if(uwvm_rmw_notify_local_graph(n)!=RMW_RET_OK){uwvm_rmw_error(c,5,"local graph wake failed after actual subscription activation");(void)uwvm_rmw_resource_retire(&scope,s->resource);goto fail;}
 uwvm_rmw_scope_leave(&scope);return &s->public_handle;
 fail:uwvm_rmw_scope_leave(&scope);return NULL;
}
rmw_ret_t rmw_destroy_subscription(rmw_node_t*handle,rmw_subscription_t*subscription){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_node*n=uwvm_rmw_node_find(handle,1);uwvm_rmw_resource*r=uwvm_rmw_resource_find(subscription,UWVM_RMW_SUBSCRIPTION,1);
 if(!n||!r||r->node!=n)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,1);if(code!=RMW_RET_OK)return code;
 uint32_t raw=uwvm_rmw_resource_retire(&scope,r);rmw_ret_t graph=uwvm_rmw_notify_local_graph(n);uwvm_rmw_scope_leave(&scope);
 if(raw||graph!=RMW_RET_OK){RMW_SET_ERROR_MSG("subscription teardown/local notification failed; independent outcomes retained");return RMW_RET_ERROR;}return RMW_RET_OK;
}
uint32_t uwvm_rmw_c_subscription_ready(uwvm_rmw_resource*r,int*ready){
 if(!r||!ready||r->kind!=UWVM_RMW_SUBSCRIPTION||!r->context->leased)return 22;
 local_subscription*s=r->object;if(s->poisoned)return s->error;
 if(s->queued){*ready=1;return 0;}
 uint32_t raw=uwvm_rmw_c_endpoint_inspect(&s->native,s->owner,NULL);if(raw){s->poisoned=1;s->error=raw;return raw;}
 size_t length=SIZE_MAX;rmw_message_info_t metadata={0};
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(p&&p->metadata.version==1&&p->metadata.take_topic){
  raw=p->metadata.take_topic(p->metadata.state,&s->native,s->owner,s->canonical,s->codec->maximum_wire_bytes,&length,&metadata);
  /* Provider success means complete, same-record native metadata was actually
   * acquired, including the full selected GID extent. All-zero bytes remain
   * a valid observed value; they do not prove either presence or absence. */
  if(!raw&&(length==SIZE_MAX||length>s->codec->maximum_wire_bytes))raw=22;
  if(!raw){
   /* This RMW actually traverses Host IPC. Native callback IP flags are separate
    * diagnostics, not proof of intra-process delivery inside this Guest Context. */
   metadata.from_intra_process=false;metadata.publisher_gid.implementation_identifier=uwvm_rmw_identifier;
   s->info=metadata;s->has_info=1;
  }
 }else raw=95;
 if(raw==11){*ready=0;return 0;}
 if(raw){s->poisoned=1;s->error=raw;return raw;}
 s->length=length;s->queued=1;*ready=1;return 0;
}
static rmw_ret_t take(rmw_subscription_t const*handle,void*message,bool*taken,
 rmw_message_info_t*info,rmw_subscription_allocation_t*allocation){
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_SUBSCRIPTION,0);
 if(!r||!message||!taken)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=r->context;local_subscription*s=r->object;
 if(allocation)return uwvm_rmw_error(c,95,"native subscription allocation/loan objects are not supported");
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(info&&(!p||p->metadata.version!=1||!p->metadata.take_topic))
  return uwvm_rmw_error(c,95,"full native topic metadata unavailable; no fabricated publisher/time facts");
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
 int ready=0;uint32_t raw=uwvm_rmw_c_subscription_ready(r,&ready);
 if(raw){uwvm_rmw_scope_leave(&scope);return uwvm_rmw_error(c,raw,"owned subscription observation failed without replay");}
 if(!ready){*taken=false;uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;}
 if(info&&!s->has_info){uwvm_rmw_scope_leave(&scope);return uwvm_rmw_error(c,95,"prefetched record has no same-record native metadata");}
 /* Real generated-object codec promises full transactional decode. No callback
  * receives a borrowed queue slice, native struct memcpy or stale shared view. */
 raw=s->codec->decode(s->canonical,s->length,message);s->queued=0;
 if(raw){s->poisoned=1;s->error=raw;uwvm_rmw_scope_leave(&scope);return uwvm_rmw_error(c,raw,"complete native-object decode failed; consumed record is not replayed");}
 if(info)*info=s->info;*taken=true;uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;
}
rmw_ret_t rmw_take(rmw_subscription_t const*s,void*message,bool*taken,rmw_subscription_allocation_t*a){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return take(s,message,taken,NULL,a);}
rmw_ret_t rmw_take_with_info(rmw_subscription_t const*s,void*message,bool*taken,rmw_message_info_t*info,rmw_subscription_allocation_t*a){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 if(!info)return RMW_RET_INVALID_ARGUMENT;return take(s,message,taken,info,a);
}
rmw_ret_t rmw_subscription_get_actual_qos(rmw_subscription_t const*handle,rmw_qos_profile_t*out){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_SUBSCRIPTION,0);if(!r||!out)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,0);if(code!=RMW_RET_OK)return code;
 local_subscription*s=r->object;uint32_t raw=uwvm_rmw_c_endpoint_inspect(&s->native,s->owner,NULL);
 if(!raw)*out=s->native.actual_qos[0];uwvm_rmw_scope_leave(&scope);
 return raw?uwvm_rmw_error(r->context,raw,"actual subscription QoS inspection failed"):RMW_RET_OK;
}
rmw_ret_t rmw_subscription_count_matched_publishers(rmw_subscription_t const*handle,size_t*out){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_SUBSCRIPTION,0);if(!r||!out)return RMW_RET_INVALID_ARGUMENT;
 return uwvm_rmw_error(r->context,95,"matched native publisher count requires an actual independent graph provider");
}
