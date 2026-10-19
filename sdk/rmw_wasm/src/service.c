/* SPDX-License-Identifier: Apache-2.0
 * Genuine upstream RMW service objects. Native metadata is accepted only from
 * the same independently qualified D record as its complete canonical body.
 * Local client IDs are this RMW's IDs; native request IDs are preserved in a
 * separate diagnostic and never represented as that local writer/sequence.
 * No application callback runs here or keeps a fictitious cross-call lease. */
#include "c_internal.h"
#include <limits.h>
_Static_assert(RMW_GID_STORAGE_SIZE==16,"selected original RMW writer GUID extent required");
enum pending_state {EMPTY=0,RESERVED=1,ADMITTED=2,QUEUED=3,TAKEN=4,ATTEMPTED=5};
typedef struct {
 enum pending_state state;uint64_t ticket;int64_t local_sequence;
 rmw_service_info_t native_info;
} pending;
typedef struct {
 rmw_client_t public_handle;
 uwvm_rmw_resource*resource;roswasmc_capability*source,*observer,*availability;
 uwvm_rmw_codec const*codec;uwvm_rmw_c_native_endpoint native;
 unsigned char*request,*event;size_t event_capacity,event_length;size_t queued_slot;
 roswasmc_event_binding binding;unsigned char writer[16];int64_t next_sequence;
 pending slots[UWVM_RMW_C_MAX_PENDING];int queued,poisoned;uint32_t error;
 uwvm_rmw_c_service_diagnostics diagnostics;
} local_client;
typedef struct {
 rmw_service_t public_handle;
 uwvm_rmw_resource*resource;roswasmc_capability*advertise,*respond;
 uwvm_rmw_codec const*codec;uwvm_rmw_c_native_endpoint native;
 unsigned char*request,*response;size_t request_length,queued_slot;
 pending slots[UWVM_RMW_C_MAX_PENDING];int queued,poisoned;uint32_t error;
 uwvm_rmw_c_service_diagnostics diagnostics;
} local_service;
static void store64(unsigned char*p,uint64_t value){for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(value>>(8*i));}
static int request_equal(rmw_request_id_t const*a,rmw_request_id_t const*b){
 return a->sequence_number==b->sequence_number&&!memcmp(a->writer_guid,b->writer_guid,16);
}
static size_t free_slot(pending const*slots){for(size_t i=0;i<UWVM_RMW_C_MAX_PENDING;++i)if(slots[i].state==EMPTY)return i;return UWVM_RMW_C_MAX_PENDING;}
static size_t find_ticket(pending const*slots,uint64_t ticket){
 for(size_t i=0;i<UWVM_RMW_C_MAX_PENDING;++i)if(slots[i].state!=EMPTY&&slots[i].ticket==ticket)return i;
 return UWVM_RMW_C_MAX_PENDING;
}
static uint32_t client_native_retire(void*object){
 local_client*s=object;uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 return s->native.native_incarnation?p->activation.retire(p->activation.state,s->source,s->observer,&s->native):0;
}
static uint32_t service_native_retire(void*object){
 local_service*s=object;uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 return s->native.native_incarnation?p->activation.retire(p->activation.state,s->advertise,s->respond,&s->native):0;
}
static uint32_t release_client(void*object){
 local_client*s=object;rcutils_allocator_t a=s->resource->context->allocator;
 a.deallocate(s->request,a.state);a.deallocate(s->event,a.state);a.deallocate(s,a.state);return 0;
}
static uint32_t release_service(void*object){
 local_service*s=object;rcutils_allocator_t a=s->resource->context->allocator;
 a.deallocate(s->request,a.state);a.deallocate(s->response,a.state);a.deallocate(s,a.state);return 0;
}
static int service_codec(uwvm_rmw_codec const*c){
 return c&&c->kind==UWVM_RMW_SERVICE_CODEC&&c->request&&c->response&&
  c->request->encode&&c->request->decode&&c->response->encode&&c->response->decode&&
  c->request->maximum_wire_bytes<=UWVM_RMW_MAX_CANONICAL_BYTES-192u&&
  c->response->maximum_wire_bytes<=UWVM_RMW_MAX_CANONICAL_BYTES-192u;
}
static uint32_t attested_pair(uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,
 uwvm_rmw_endpoint_selection const*row){
 uint32_t raw=uwvm_rmw_c_endpoint_valid(e,source,2);if(raw)return raw;
 return !uwvm_rmw_qos_equal(e->actual_qos,&row->effective_qos)||
  !uwvm_rmw_qos_equal(e->actual_qos+1,&row->effective_qos)?13:0;
}
rmw_client_t*rmw_create_client(rmw_node_t const*handle,rosidl_service_type_support_t const*support,
 char const*name,rmw_qos_profile_t const*qos){
 UWVM_RMW_ENTRY(NULL);
 uwvm_rmw_node*n=uwvm_rmw_node_find(handle,0);if(!n||!support||!name||!qos){uwvm_rmw_error(n?n->context:NULL,22,"invalid actual service client arguments");return NULL;}
 uwvm_rmw_context*c=n->context;uwvm_rmw_scope scope;if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(!p||p->activation.version!=1||p->requests.version!=1||!p->requests.submit||p->metadata.version!=1||!p->metadata.take_response){
  uwvm_rmw_error(c,95,"actual same-native-endpoint activation/submit/response metadata providers required before opens");goto fail;
 }
 uwvm_rmw_endpoint_selection const*row=uwvm_rmw_select(n,UWVM_RMW_CLIENT,name,qos,support);
 if(!row||row->alias_count!=2){uwvm_rmw_error(c,13,"exact selected service.call/event.observe pair absent");goto fail;}
 if(row->availability_alias&&(p->availability.version!=1||!p->availability.query)){
  uwvm_rmw_error(c,95,"explicit same-client availability provider required before any owner open");goto fail;
 }
 size_t owners_needed=row->availability_alias?3u:2u;
 uwvm_rmw_codec const*codec=c->profile->codecs+row->codec_index;
 if(!service_codec(codec)){uwvm_rmw_error(c,95,"complete selected service Request/Response object codecs required");goto fail;}
 if(c->resource_count==UWVM_RMW_RESOURCE_LIMIT||c->session.count>UWVM_RMW_MAX_OWNERS-owners_needed||!c->next_local_gid||c->next_local_gid==UINT64_MAX){uwvm_rmw_error(c,90,"client resource/owner/local writer identity bound exhausted before effects");goto fail;}
 uwvm_rmw_c_mark_started();local_client*s=c->allocator.zero_allocate(1,sizeof*s,c->allocator.state);
 if(!s){uwvm_rmw_error(c,12,"client record allocation failed before effects");goto fail;}
 s->event_capacity=64u+codec->response->maximum_wire_bytes;
 s->request=c->allocator.allocate(codec->request->maximum_wire_bytes?codec->request->maximum_wire_bytes:1u,c->allocator.state);
 s->event=c->allocator.allocate(s->event_capacity,c->allocator.state);
 if(!s->request||!s->event){if(s->request)c->allocator.deallocate(s->request,c->allocator.state);if(s->event)c->allocator.deallocate(s->event,c->allocator.state);c->allocator.deallocate(s,c->allocator.state);uwvm_rmw_error(c,12,"client complete owned buffers allocation failed before effects");goto fail;}
 s->codec=codec;s->next_sequence=1;s->public_handle.implementation_identifier=uwvm_rmw_identifier;s->public_handle.data=s;s->public_handle.service_name=row->resolved_name;
 /* A real local writer identity namespace. It is NOT a DDS/native GUID. The
  * context instance and monotonic counter are reserved before first effect. */
 store64(s->writer,c->local_instance);store64(s->writer+8,c->next_local_gid++);
 s->resource=uwvm_rmw_resource_reserve(&scope,n,row,s,release_client);
 if(!s->resource){c->allocator.deallocate(s->request,c->allocator.state);c->allocator.deallocate(s->event,c->allocator.state);c->allocator.deallocate(s,c->allocator.state);uwvm_rmw_error(c,90,"client cleanup reservation failed before effects");goto fail;}
 s->resource->native_retire=client_native_retire;
 uint32_t raw=uwvm_rmw_owner_open(&scope,s->resource,0,&s->source);
 if(!raw)raw=uwvm_rmw_owner_open(&scope,s->resource,1,&s->observer);
 if(!raw&&row->availability_alias)raw=uwvm_rmw_owner_open_availability(&scope,s->resource,&s->availability);
 if(!raw)raw=p->activation.activate(p->activation.state,UWVM_RMW_CLIENT,s->source,s->observer,codec,qos,&s->native);
 if(!raw)raw=attested_pair(&s->native,s->source,row);
 if(!raw&&s->native.native_gid_bytes==RMW_GID_STORAGE_SIZE)memcpy(s->writer,s->native.native_gid,16);
 if(!raw)raw=roswasmc_event_binding_init(&s->binding,2,s->source->handle,0);
 if(raw){uwvm_rmw_error(c,raw,"new native client activation/QoS association failed");(void)uwvm_rmw_resource_retire(&scope,s->resource);goto fail;}
 if(uwvm_rmw_notify_local_graph(n)!=RMW_RET_OK){uwvm_rmw_error(c,5,"local graph wake failed after native client activation");(void)uwvm_rmw_resource_retire(&scope,s->resource);goto fail;}
 uwvm_rmw_scope_leave(&scope);return &s->public_handle;
 fail:uwvm_rmw_scope_leave(&scope);return NULL;
}
rmw_service_t*rmw_create_service(rmw_node_t const*handle,rosidl_service_type_support_t const*support,
 char const*name,rmw_qos_profile_t const*qos){
 UWVM_RMW_ENTRY(NULL);
 uwvm_rmw_node*n=uwvm_rmw_node_find(handle,0);if(!n||!support||!name||!qos){uwvm_rmw_error(n?n->context:NULL,22,"invalid actual service server arguments");return NULL;}
 uwvm_rmw_context*c=n->context;uwvm_rmw_scope scope;if(uwvm_rmw_scope_enter(c,&scope,0)!=RMW_RET_OK)return NULL;
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 if(!p||p->activation.version!=1||p->requests.version!=1||!p->requests.respond||p->metadata.version!=1||!p->metadata.take_request){
  uwvm_rmw_error(c,95,"actual same-native-server activation/respond/request metadata providers required before opens");goto fail;
 }
 uwvm_rmw_endpoint_selection const*row=uwvm_rmw_select(n,UWVM_RMW_SERVICE,name,qos,support);
 if(!row||row->alias_count!=2){uwvm_rmw_error(c,13,"exact selected service.advertise/service.respond pair absent");goto fail;}
 uwvm_rmw_codec const*codec=c->profile->codecs+row->codec_index;
 if(!service_codec(codec)){uwvm_rmw_error(c,95,"complete selected service Request/Response object codecs required");goto fail;}
 if(c->resource_count==UWVM_RMW_RESOURCE_LIMIT||c->session.count>UWVM_RMW_MAX_OWNERS-2u){uwvm_rmw_error(c,90,"server resource/owner bound exhausted before effects");goto fail;}
 uwvm_rmw_c_mark_started();local_service*s=c->allocator.zero_allocate(1,sizeof*s,c->allocator.state);
 if(!s){uwvm_rmw_error(c,12,"server record allocation failed before effects");goto fail;}
 s->request=c->allocator.allocate(codec->request->maximum_wire_bytes?codec->request->maximum_wire_bytes:1u,c->allocator.state);
 s->response=c->allocator.allocate(codec->response->maximum_wire_bytes?codec->response->maximum_wire_bytes:1u,c->allocator.state);
 if(!s->request||!s->response){if(s->request)c->allocator.deallocate(s->request,c->allocator.state);if(s->response)c->allocator.deallocate(s->response,c->allocator.state);c->allocator.deallocate(s,c->allocator.state);uwvm_rmw_error(c,12,"server complete buffers allocation failed before effects");goto fail;}
 s->codec=codec;s->public_handle.implementation_identifier=uwvm_rmw_identifier;s->public_handle.data=s;s->public_handle.service_name=row->resolved_name;
 s->resource=uwvm_rmw_resource_reserve(&scope,n,row,s,release_service);
 if(!s->resource){c->allocator.deallocate(s->request,c->allocator.state);c->allocator.deallocate(s->response,c->allocator.state);c->allocator.deallocate(s,c->allocator.state);uwvm_rmw_error(c,90,"server cleanup reservation failed before effects");goto fail;}
 s->resource->native_retire=service_native_retire;
 uint32_t raw=uwvm_rmw_owner_open(&scope,s->resource,0,&s->advertise);
 if(!raw)raw=uwvm_rmw_owner_open(&scope,s->resource,1,&s->respond);
 if(!raw)raw=p->activation.activate(p->activation.state,UWVM_RMW_SERVICE,s->advertise,s->respond,codec,qos,&s->native);
 if(!raw)raw=attested_pair(&s->native,s->advertise,row);
 if(raw){uwvm_rmw_error(c,raw,"new native server activation/QoS association failed");(void)uwvm_rmw_resource_retire(&scope,s->resource);goto fail;}
 if(uwvm_rmw_notify_local_graph(n)!=RMW_RET_OK){uwvm_rmw_error(c,5,"local graph wake failed after native server activation");(void)uwvm_rmw_resource_retire(&scope,s->resource);goto fail;}
 uwvm_rmw_scope_leave(&scope);return &s->public_handle;
 fail:uwvm_rmw_scope_leave(&scope);return NULL;
}
static rmw_ret_t destroy(rmw_node_t*node,void*object,enum uwvm_rmw_endpoint_kind kind){
 uwvm_rmw_node*n=uwvm_rmw_node_find(node,1);uwvm_rmw_resource*r=uwvm_rmw_resource_find(object,kind,1);
 if(!n||!r||r->node!=n)return RMW_RET_INCORRECT_RMW_IMPLEMENTATION;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,1);if(code!=RMW_RET_OK)return code;
 uint32_t raw=uwvm_rmw_resource_retire(&scope,r);rmw_ret_t graph=uwvm_rmw_notify_local_graph(n);uwvm_rmw_scope_leave(&scope);
 if(raw||graph!=RMW_RET_OK){RMW_SET_ERROR_MSG("service endpoint independent native/local teardown failed");return RMW_RET_ERROR;}return RMW_RET_OK;
}
rmw_ret_t rmw_destroy_client(rmw_node_t*n,rmw_client_t*c){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return destroy(n,c,UWVM_RMW_CLIENT);}
rmw_ret_t rmw_destroy_service(rmw_node_t*n,rmw_service_t*s){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return destroy(n,s,UWVM_RMW_SERVICE);}
rmw_ret_t rmw_send_request(rmw_client_t const*handle,void const*message,int64_t*sequence){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_CLIENT,0);if(!r||!message||!sequence)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=r->context;local_client*s=r->object;uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
 uint32_t raw=s->poisoned?s->error:uwvm_rmw_c_endpoint_inspect(&s->native,s->source,s->observer);
 size_t slot=free_slot(s->slots),length=SIZE_MAX;
 if(!raw&&(slot==UWVM_RMW_C_MAX_PENDING||s->next_sequence<=0||s->next_sequence==INT64_MAX))raw=90;
 if(!raw)raw=s->codec->request->encode(message,s->request,s->codec->request->maximum_wire_bytes,&length);
 if(!raw&&(length==SIZE_MAX||length>s->codec->request->maximum_wire_bytes))raw=22;
 if(!raw){
  pending*q=s->slots+slot;*q=(pending){0};q->state=RESERVED;q->local_sequence=s->next_sequence++;
  uint64_t ticket=0;uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
  raw=p->requests.submit(p->requests.state,&s->native,s->source,s->observer,s->request,length,&ticket);
  if(!raw&&(!ticket||find_ticket(s->slots,ticket)!=UWVM_RMW_C_MAX_PENDING))raw=22;
  if(raw==11){q->state=EMPTY;} /* Explicit no-credit admission, no native dispatch. */
  else if(raw){s->poisoned=1;s->error=raw;s->diagnostics.ambiguous_admission=true;s->diagnostics.terminal_error=raw;q->ticket=ticket;}
  else{q->ticket=ticket;q->state=ADMITTED;*sequence=q->local_sequence;}
 }
 uwvm_rmw_scope_leave(&scope);return raw?uwvm_rmw_error(c,raw,"service submit failed; no automatic replay or invented native sequence"):RMW_RET_OK;
}
uint32_t uwvm_rmw_c_client_ready(uwvm_rmw_resource*r,int*ready){
 if(!r||!ready||r->kind!=UWVM_RMW_CLIENT||!r->context->leased)return 22;
 local_client*s=r->object;if(s->poisoned)return s->error;if(s->queued){*ready=1;return 0;}
 uint32_t raw=uwvm_rmw_c_endpoint_inspect(&s->native,s->source,s->observer);if(raw)goto fail;
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();size_t length=SIZE_MAX;rmw_service_info_t info={0};unsigned present=0;
 raw=p->metadata.take_response(p->metadata.state,&s->native,s->observer,s->source,s->event,s->event_capacity,&length,&info,&present);
 if(raw==11){*ready=0;return 0;}if(raw)goto fail;
 if(length==SIZE_MAX||length>s->event_capacity){raw=22;goto fail;}
 roswasmc_event_header header;raw=roswasmc_event_decode(s->event,length,&header);if(raw)goto fail;
 if(header.instance!=s->native.instance||header.session!=s->native.session){raw=13;goto fail;}
 raw=roswasmc_event_binding_decode(&s->binding,s->event,length,&header);if(raw)goto fail;
 s->diagnostics.gateway_ticket=header.correlation;s->diagnostics.event_flags=header.flags;
 if(header.kind==ROSWASMC_RESOURCE_ERROR||header.kind==ROSWASMC_RESOURCE_CLOSED){
  if(present){raw=22;goto fail;}raw=header.error?header.error:9;goto fail;
 }
 /* Presence is the independently validated same-record protocol field, not
  * a guess from the native writer bytes. A genuine all-zero writer is data. */
 if(header.kind!=ROSWASMC_SERVICE_RESPONSE||present!=1){raw=22;goto fail;}
 size_t index=find_ticket(s->slots,header.correlation);
 if(index==UWVM_RMW_C_MAX_PENDING||s->slots[index].state!=ADMITTED){raw=22;goto fail;}
 s->slots[index].native_info=info;s->diagnostics.actual_native_info=info;s->diagnostics.actual_native_info_present=true;
 s->diagnostics.local_sequence=s->slots[index].local_sequence;s->queued_slot=index;s->event_length=length;s->queued=1;*ready=1;return 0;
 fail:s->poisoned=1;s->error=raw;s->diagnostics.terminal_error=raw;return raw;
}
rmw_ret_t rmw_take_response(rmw_client_t const*handle,rmw_service_info_t*header,void*message,bool*taken){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_CLIENT,0);if(!r||!header||!message||!taken)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=r->context;local_client*s=r->object;uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
 int ready=0;uint32_t raw=uwvm_rmw_c_client_ready(r,&ready);
 if(!raw&&!ready){*taken=false;uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;}
 if(!raw){
  pending*q=s->slots+s->queued_slot;raw=s->codec->response->decode(s->event+64,s->event_length-64,message);
  s->queued=0;if(!raw){rmw_service_info_t result=q->native_info;
   memcpy(result.request_id.writer_guid,s->writer,16);result.request_id.sequence_number=q->local_sequence;
   *header=result;*taken=true;q->state=EMPTY;
  }else{s->poisoned=1;s->error=raw;s->diagnostics.terminal_error=raw;}
 }
 uwvm_rmw_scope_leave(&scope);return raw?uwvm_rmw_error(c,raw,"complete service response take failed without replay"):RMW_RET_OK;
}
uint32_t uwvm_rmw_c_service_ready(uwvm_rmw_resource*r,int*ready){
 if(!r||!ready||r->kind!=UWVM_RMW_SERVICE||!r->context->leased)return 22;
 local_service*s=r->object;if(s->poisoned)return s->error;if(s->queued){*ready=1;return 0;}
 size_t slot=free_slot(s->slots);if(slot==UWVM_RMW_C_MAX_PENDING)return 90;
 uint32_t raw=uwvm_rmw_c_endpoint_inspect(&s->native,s->advertise,s->respond);if(raw)goto fail;
 pending*q=s->slots+slot;*q=(pending){0};q->state=RESERVED;size_t length=SIZE_MAX;uint64_t ticket=0;rmw_service_info_t info={0};
 uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
 raw=p->metadata.take_request(p->metadata.state,&s->native,s->advertise,s->request,s->codec->request->maximum_wire_bytes,&length,&ticket,&info);
 if(raw==11){q->state=EMPTY;*ready=0;return 0;}if(raw)goto fail;
 /* take_request success requires complete same-record native service info.
  * The full native ID still participates in exact duplicate/header matching;
  * zero-valued writer bytes are not an absence sentinel. */
 if(!ticket||length==SIZE_MAX||length>s->codec->request->maximum_wire_bytes){raw=22;goto fail;}
 for(size_t i=0;i<UWVM_RMW_C_MAX_PENDING;++i)if(i!=slot&&s->slots[i].state!=EMPTY&&
  (s->slots[i].ticket==ticket||request_equal(&s->slots[i].native_info.request_id,&info.request_id))){raw=22;goto fail;}
 q->ticket=ticket;q->native_info=info;q->state=QUEUED;s->queued_slot=slot;s->request_length=length;s->queued=1;
 s->diagnostics.gateway_ticket=ticket;s->diagnostics.actual_native_info=info;s->diagnostics.actual_native_info_present=true;*ready=1;return 0;
 fail:s->poisoned=1;s->error=raw;s->diagnostics.terminal_error=raw;return raw;
}
rmw_ret_t rmw_take_request(rmw_service_t const*handle,rmw_service_info_t*header,void*message,bool*taken){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_SERVICE,0);if(!r||!header||!message||!taken)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=r->context;local_service*s=r->object;uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
 int ready=0;uint32_t raw=uwvm_rmw_c_service_ready(r,&ready);
 if(!raw&&!ready){*taken=false;uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;}
 if(!raw){pending*q=s->slots+s->queued_slot;raw=s->codec->request->decode(s->request,s->request_length,message);s->queued=0;
  if(!raw){*header=q->native_info;*taken=true;q->state=TAKEN;}else{s->poisoned=1;s->error=raw;s->diagnostics.terminal_error=raw;}
 }
 uwvm_rmw_scope_leave(&scope);return raw?uwvm_rmw_error(c,raw,"complete native service request decode failed without replay"):RMW_RET_OK;
}
rmw_ret_t rmw_send_response(rmw_service_t const*handle,rmw_request_id_t*header,void*message){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_SERVICE,0);if(!r||!header||!message)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=r->context;local_service*s=r->object;uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
 size_t index=UWVM_RMW_C_MAX_PENDING;
 for(size_t i=0;i<UWVM_RMW_C_MAX_PENDING;++i)if(s->slots[i].state==TAKEN&&request_equal(header,&s->slots[i].native_info.request_id)){index=i;break;}
 uint32_t raw=s->poisoned?s->error:index==UWVM_RMW_C_MAX_PENDING?9:uwvm_rmw_c_endpoint_inspect(&s->native,s->advertise,s->respond);
 size_t length=SIZE_MAX;if(!raw)raw=s->codec->response->encode(message,s->response,s->codec->response->maximum_wire_bytes,&length);
 if(!raw&&(length==SIZE_MAX||length>s->codec->response->maximum_wire_bytes))raw=22;
 if(!raw){pending*q=s->slots+index;q->state=ATTEMPTED;uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();
  raw=p->requests.respond(p->requests.state,&s->native,s->advertise,s->respond,q->ticket,s->response,length);
  /* Mark BEFORE the real request: no success/error can make this ticket replayable. */
  if(!raw)q->state=EMPTY;else{s->poisoned=1;s->error=raw;s->diagnostics.terminal_error=raw;}
 }
 uwvm_rmw_scope_leave(&scope);return raw?uwvm_rmw_error(c,raw,"service response failed; ticket is never automatically replayed"):RMW_RET_OK;
}
static rmw_ret_t actual_qos(void const*handle,enum uwvm_rmw_endpoint_kind kind,size_t index,rmw_qos_profile_t*out){
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,kind,0);if(!r||!out)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,0);if(code!=RMW_RET_OK)return code;
 local_client*c=kind==UWVM_RMW_CLIENT?r->object:NULL;local_service*s=kind==UWVM_RMW_SERVICE?r->object:NULL;
 uwvm_rmw_c_native_endpoint const*e=c?&c->native:&s->native;
 uint32_t raw=uwvm_rmw_c_endpoint_inspect(e,c?c->source:s->advertise,c?c->observer:s->respond);
 if(!raw)*out=e->actual_qos[index];uwvm_rmw_scope_leave(&scope);
 return raw?uwvm_rmw_error(r->context,raw,"actual request/response native QoS inspection failed"):RMW_RET_OK;
}
rmw_ret_t rmw_client_request_publisher_get_actual_qos(rmw_client_t const*c,rmw_qos_profile_t*q){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return actual_qos(c,UWVM_RMW_CLIENT,0,q);}
rmw_ret_t rmw_client_response_subscription_get_actual_qos(rmw_client_t const*c,rmw_qos_profile_t*q){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return actual_qos(c,UWVM_RMW_CLIENT,1,q);}
rmw_ret_t rmw_service_request_subscription_get_actual_qos(rmw_service_t const*s,rmw_qos_profile_t*q){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return actual_qos(s,UWVM_RMW_SERVICE,0,q);}
rmw_ret_t rmw_service_response_publisher_get_actual_qos(rmw_service_t const*s,rmw_qos_profile_t*q){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);return actual_qos(s,UWVM_RMW_SERVICE,1,q);}
rmw_ret_t rmw_service_server_is_available(rmw_node_t const*node,rmw_client_t const*client,bool*available){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_node*n=uwvm_rmw_node_find(node,0);uwvm_rmw_resource*r=uwvm_rmw_resource_find(client,UWVM_RMW_CLIENT,0);
 if(!n||!r||r->node!=n||!available)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_context*c=r->context;uwvm_rmw_scope scope;
 rmw_ret_t code=uwvm_rmw_scope_enter(c,&scope,0);if(code!=RMW_RET_OK)return code;
 local_client*s=r->object;uwvm_rmw_c_profile const*p=uwvm_rmw_c_configuration();uint32_t raw=0;
 if(!r->selection->availability_alias||!s->availability||!p||p->availability.version!=1||!p->availability.query)raw=95;
 else if(!roswasmc_is_open(s->availability)||s->availability->context!=s->source->context||
  s->availability->handle==s->source->handle||s->availability->handle==s->observer->handle)raw=9;
 if(!raw)raw=s->poisoned?s->error:uwvm_rmw_c_endpoint_inspect(&s->native,s->source,s->observer);
 unsigned next=UINT_MAX;
 if(!raw)raw=p->availability.query(p->availability.state,&s->native,s->source,s->availability,&next);
 if(!raw&&next>1)raw=22;
 if(!raw)*available=next!=0; /* One public write after complete true-client query. */
 uwvm_rmw_scope_leave(&scope);
 return raw?uwvm_rmw_error(c,raw,"same-owned-native client availability query failed; no graph/probe/replay"):RMW_RET_OK;
}
rmw_ret_t uwvm_rmw_c_client_diagnostics(rmw_client_t const*handle,uwvm_rmw_c_service_diagnostics*out){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_CLIENT,1);if(!r||!out)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,1);if(code!=RMW_RET_OK)return code;
 *out=((local_client*)r->object)->diagnostics;uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;
}
rmw_ret_t uwvm_rmw_c_server_diagnostics(rmw_service_t const*handle,uwvm_rmw_c_service_diagnostics*out){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_SERVICE,1);if(!r||!out)return RMW_RET_INVALID_ARGUMENT;
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,1);if(code!=RMW_RET_OK)return code;
 *out=((local_service*)r->object)->diagnostics;uwvm_rmw_scope_leave(&scope);return RMW_RET_OK;
}

rmw_ret_t rmw_get_gid_for_client(rmw_client_t const*handle,rmw_gid_t*out){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 uwvm_rmw_resource*r=uwvm_rmw_resource_find(handle,UWVM_RMW_CLIENT,0);if(!r||!out)return RMW_RET_INVALID_ARGUMENT;
 local_client*s=r->object;
 if(s->native.native_gid_bytes!=RMW_GID_STORAGE_SIZE)return uwvm_rmw_error(r->context,95,"complete matching-width native global client GID is unavailable; local correlation is not global identity");
 uwvm_rmw_scope scope;rmw_ret_t code=uwvm_rmw_scope_enter(r->context,&scope,0);if(code!=RMW_RET_OK)return code;
 uint32_t raw=uwvm_rmw_c_endpoint_inspect(&s->native,s->source,s->observer);
 if(!raw){rmw_gid_t next={0};next.implementation_identifier=uwvm_rmw_identifier;memcpy(next.data,s->native.native_gid,RMW_GID_STORAGE_SIZE);*out=next;}
 uwvm_rmw_scope_leave(&scope);return raw?uwvm_rmw_error(r->context,raw,"actual global client GID changed/inspection failed"):RMW_RET_OK;
}
