/* SPDX-License-Identifier: Apache-2.0
 * Source-selected raw12 provider. No allocation after bootstrap, no old81
 * business fallback, fake request ID, ambient clock or automatic operation retry.
 */
#include <uwvm_rmw/service_transport.h>
#include <roswasm/rmw_service_v1.h>
#include <rmw/time.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#if defined(__has_include)
#if __has_include("uwvm_rmw_service_features.h")
#include "uwvm_rmw_service_features.h"
#endif
#endif
#ifndef UWVM_RMW_SERVICE_INFO_API
#define UWVM_RMW_SERVICE_INFO_API 0
#endif
#define STATE_MAGIC UINT32_C(0x56535755)
#define IPC_MAX 4194304u
#define OVERHEAD 352u
#define INPUT_MAX (IPC_MAX-518u-160u)
enum transaction_state {TRANSACTION_RESERVED=1,TRANSACTION_ADMITTED=2,TRANSACTION_DELIVERED=3,TRANSACTION_DONE=4,TRANSACTION_ABANDONED=5};
typedef struct transaction {uint64_t ticket;uint8_t state;rmw_service_info_t native_info;} transaction;
typedef struct service_slot {
 roswasmc_capability const*owner,*peer;roswasmc_context const*context;uwvm_rmw_codec const*codec;
 size_t request_bound,response_bound,count;transaction*transactions;
 uwvm_rmw_c_native_endpoint admitted;uwvm_rmw_service_transport_diagnostic facts;
} service_slot;
struct uwvm_rmw_service_transport {
 uint32_t magic;atomic_bool busy;int exported,composed;rcutils_allocator_t allocator;
 size_t capacity,count,transaction_capacity,maximum_request,maximum_response,packet_capacity,event_capacity;
 unsigned char*packet,*event;unsigned char receipt[256],ticket[8],available;
 uwvm_rmw_c_activation_provider subscription_activation;
 uwvm_rmw_c_native_metadata_provider subscription_metadata;
 service_slot slots[];
};
static uint32_t enter(uwvm_rmw_service_transport*s){if(!s||s->magic!=STATE_MAGIC)return 22;return atomic_exchange_explicit(&s->busy,true,memory_order_acquire)?17:0;}
static void leave(uwvm_rmw_service_transport*s){atomic_store_explicit(&s->busy,false,memory_order_release);}
static uint64_t le(unsigned char const*p,size_t n){uint64_t v=0;for(size_t i=0;i<n;++i)v|=(uint64_t)p[i]<<(8*i);return v;}
static int zero(unsigned char const*p,size_t n){for(size_t i=0;i<n;++i)if(p[i])return 0;return 1;}
static int full_handle(uint64_t n){return (uint32_t)n&&(uint32_t)(n>>32);}
static int source_valid(roswasmc_capability const*c){return roswasmc_is_open(c)&&c->context->backend->version==2&&full_handle(c->handle);}
static uint32_t qos_decode(unsigned char const*p,rmw_qos_profile_t*out){
 uint64_t h=le(p,4),r=le(p+4,4),d=le(p+8,4),l=le(p+12,4),depth=le(p+16,8);
 if(h<1||h>2||r<1||r>2||d<1||d>2||l<1||l>2||(h==1&&!depth)||depth>SIZE_MAX||p[72]>1||!zero(p+73,7)||
 le(p+32,8)>=1000000000ULL||le(p+48,8)>=1000000000ULL||le(p+64,8)>=1000000000ULL)return 22;
 rmw_qos_profile_t q={0};q.history=h==1?RMW_QOS_POLICY_HISTORY_KEEP_LAST:RMW_QOS_POLICY_HISTORY_KEEP_ALL;
 q.reliability=r==1?RMW_QOS_POLICY_RELIABILITY_RELIABLE:RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
 q.durability=d==1?RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL:RMW_QOS_POLICY_DURABILITY_VOLATILE;
 q.liveliness=l==1?RMW_QOS_POLICY_LIVELINESS_AUTOMATIC:RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;q.depth=(size_t)depth;
 q.deadline=(rmw_time_t){le(p+24,8),le(p+32,8)};q.lifespan=(rmw_time_t){le(p+40,8),le(p+48,8)};
 q.liveliness_lease_duration=(rmw_time_t){le(p+56,8),le(p+64,8)};q.avoid_ros_namespace_conventions=p[72]!=0;*out=q;return 0;
}

static void put(unsigned char*p,uint64_t value,size_t width){for(size_t i=0;i<width;++i)p[i]=(unsigned char)(value>>(8*i));}
static void primary(service_slot*p,uint32_t n){if(n&&!p->facts.first_primary)p->facts.first_primary=n;}
static int same_binding(uwvm_rmw_c_native_endpoint const*a,uwvm_rmw_c_native_endpoint const*b){
 return a->version==1&&b->version==1&&a->backend==2&&b->backend==2&&a->source==b->source&&a->instance==b->instance&&a->runtime_generation==b->runtime_generation&&a->session==b->session&&a->native_incarnation==b->native_incarnation;
}
static int same_receipt(uwvm_rmw_c_native_endpoint const*a,uwvm_rmw_c_native_endpoint const*b){
 return same_binding(a,b)&&a->qos_count==2&&b->qos_count==2&&a->native_gid_bytes==b->native_gid_bytes&&a->native_gid_bytes<=32&&
 !memcmp(a->native_gid,b->native_gid,32)&&uwvm_rmw_qos_equal(a->actual_qos,b->actual_qos)&&uwvm_rmw_qos_equal(a->actual_qos+1,b->actual_qos+1);
}
static uint32_t receipt_decode(unsigned char const*b,uint32_t op,uint32_t kind,uint64_t source,uwvm_rmw_c_native_endpoint*out){
 if(le(b,4)!=UINT32_C(0x45524d55)||le(b+4,2)!=1||b[6]!=op||b[7]!=2||le(b+8,4)!=256||b[12]!=kind||b[13]!=2||b[14]>32||b[15]||!zero(b+56,8))return 22;
 if((kind==4&&b[14])||!zero(b+224+b[14],32-b[14]))return 22;
 uwvm_rmw_c_native_endpoint e={0};e.version=1;e.backend=2;e.source=le(b+16,8);e.instance=le(b+24,8);e.runtime_generation=le(b+32,8);e.session=le(b+40,8);e.native_incarnation=le(b+48,8);e.qos_count=2;e.native_gid_bytes=b[14];
 if(e.source!=source||!full_handle(e.source)||!e.instance||!e.runtime_generation||!e.session||!e.native_incarnation)return 22;
 uint32_t raw=qos_decode(b+64,e.actual_qos);if(!raw)raw=qos_decode(b+144,e.actual_qos+1);if(raw)return raw;
 memcpy(e.native_gid,b+224,32);*out=e;return 0;
}
static service_slot*find(uwvm_rmw_service_transport*s,roswasmc_capability const*source,uwvm_rmw_c_native_endpoint const*expected){
 if(!source_valid(source)||!expected)return NULL;
 for(size_t i=0;i<s->count;++i){service_slot*p=s->slots+i;if(p->facts.associated&&p->owner==source&&p->context==source->context&&p->facts.source==source->handle&&same_receipt(expected,&p->admitted))return p;}return NULL;
}
static int pair(service_slot const*p,roswasmc_capability const*other){return source_valid(other)&&p->peer==other&&p->context==other->context&&p->facts.peer==other->handle&&other->handle!=p->facts.source;}
static uint32_t retire_slot(service_slot*p){
 if(p->facts.retire_attempted)return p->facts.retire_code;p->facts.retire_attempted=1;
 p->facts.retire_code=uwvm_ros2_rmw_srv_v1_endpoint_retire(p->facts.source,p->admitted.native_incarnation);return p->facts.retire_code;
}
static uint32_t terminal(service_slot*p,uint32_t reason){primary(p,reason);p->facts.poisoned=1;if(!p->facts.retire_attempted)(void)retire_slot(p);return reason;}
static uint32_t inspect_slot(uwvm_rmw_service_transport*s,service_slot*p,uwvm_rmw_c_native_endpoint*out){
 if(p->facts.retire_attempted)return 125;if(p->facts.poisoned)return p->facts.first_primary?p->facts.first_primary:125;
 memset(s->receipt,0xa5,256);uint32_t raw=uwvm_ros2_rmw_srv_v1_endpoint_inspect(p->facts.source,p->admitted.native_incarnation,0,(uint64_t)(uintptr_t)s->receipt);
 p->facts.inspect_code=raw;p->facts.inspect_validation=0;if(raw){primary(p,raw);return raw;}
 memcpy(p->facts.inspect_wire,s->receipt,256);p->facts.inspect_wire_present=1;uwvm_rmw_c_native_endpoint next={0};
 raw=receipt_decode(s->receipt,2,p->facts.kind,p->facts.source,&next);
 if(!raw&&(!same_receipt(&next,&p->admitted)||memcmp(s->receipt,p->facts.original_wire,6)||memcmp(s->receipt+7,p->facts.original_wire+7,249)))raw=13;
 p->facts.inspect_validation=raw;primary(p,raw);if(!raw)*out=next;return raw;
}
static transaction*ticket(service_slot*p,uint64_t value){if(!value)return NULL;for(size_t i=0;i<p->count;++i)if(p->transactions[i].ticket==value)return p->transactions+i;return NULL;}
static uint32_t activate_cb(void*state,enum uwvm_rmw_endpoint_kind kind,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_codec const*codec,rmw_qos_profile_t const*requested,uwvm_rmw_c_native_endpoint*out){
 uwvm_rmw_service_transport*s=state;
 if(kind==UWVM_RMW_SUBSCRIPTION&&s&&s->magic==STATE_MAGIC&&s->composed)return s->subscription_activation.activate(s->subscription_activation.state,kind,source,aux,codec,requested,out);
 if(kind!=UWVM_RMW_CLIENT&&kind!=UWVM_RMW_SERVICE)return 95;
 uint32_t raw=enter(s);if(raw)return raw;
 if(!source_valid(source)||!source_valid(aux)||source->context!=aux->context||source->handle==aux->handle||!codec||!requested||!out||codec->version!=1||codec->kind!=UWVM_RMW_SERVICE_CODEC||!codec->generated_support||!codec->validate||!codec->request||!codec->response||!codec->request->validate||!codec->response->validate||!codec->request->encode||!codec->request->decode||!codec->response->encode||!codec->response->decode||codec->request->maximum_wire_bytes>s->maximum_request||codec->response->maximum_wire_bytes>s->maximum_response){leave(s);return 22;}
 raw=codec->validate(codec,codec->generated_support);if(!raw)raw=codec->request->validate(codec->request,codec->request->generated_support);if(!raw)raw=codec->response->validate(codec->response,codec->response->generated_support);if(raw){leave(s);return raw;}
 for(size_t i=0;i<s->count;++i)if(s->slots[i].facts.source==source->handle||s->slots[i].facts.peer==aux->handle){leave(s);return 13;}
 if(s->count==s->capacity){leave(s);return 90;}
 service_slot*p=s->slots+s->count++;p->owner=source;p->peer=aux;p->context=source->context;p->codec=codec;p->request_bound=codec->request->maximum_wire_bytes;p->response_bound=codec->response->maximum_wire_bytes;
 p->facts.source=source->handle;p->facts.peer=aux->handle;p->facts.kind=kind==UWVM_RMW_CLIENT?3:4;
 p->facts.abi_version=uwvm_ros2_rmw_srv_v1_abi_version();if(p->facts.abi_version!=1){primary(p,95);leave(s);return 95;}
 memset(s->receipt,0xa5,256);p->facts.activation_attempted=1;
 raw=kind==UWVM_RMW_CLIENT?uwvm_ros2_rmw_srv_v1_client_activate(source->handle,aux->handle,0,(uint64_t)(uintptr_t)s->receipt):uwvm_ros2_rmw_srv_v1_service_activate(source->handle,aux->handle,0,(uint64_t)(uintptr_t)s->receipt);
 p->facts.activation_code=raw;if(raw){primary(p,raw);leave(s);return raw;}
 memcpy(p->facts.original_wire,s->receipt,256);p->facts.original_wire_present=1;uwvm_rmw_c_native_endpoint next={0};raw=receipt_decode(s->receipt,1,p->facts.kind,source->handle,&next);p->facts.activation_validation=raw;
 if(raw){primary(p,raw);leave(s);return raw;}p->admitted=next;p->facts.associated=1;*out=next;leave(s);return 0;
}
static uint32_t inspect_cb(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_c_native_endpoint const*expected,uwvm_rmw_c_native_endpoint*out){
 uwvm_rmw_service_transport*s=state;
 if(s&&s->magic==STATE_MAGIC&&s->composed&&expected&&expected->qos_count==1)return s->subscription_activation.inspect(s->subscription_activation.state,source,aux,expected,out);
 uint32_t raw=enter(s);if(raw)return raw;if(!out){leave(s);return 22;}service_slot*p=find(s,source,expected);if(!p||!pair(p,aux)){leave(s);return 13;}uwvm_rmw_c_native_endpoint next={0};raw=inspect_slot(s,p,&next);if(!raw)*out=next;leave(s);return raw;
}
static uint32_t retire_cb(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_c_native_endpoint const*expected){
 uwvm_rmw_service_transport*s=state;
 if(s&&s->magic==STATE_MAGIC&&s->composed&&expected&&expected->qos_count==1)return s->subscription_activation.retire(s->subscription_activation.state,source,aux,expected);
 uint32_t raw=enter(s);if(raw)return raw;service_slot*p=find(s,source,expected);if(!p||!pair(p,aux)){leave(s);return 13;}raw=retire_slot(p);leave(s);return raw;
}
static uint32_t submit_cb(void*state,uwvm_rmw_c_native_endpoint const*expected,roswasmc_capability const*source,roswasmc_capability const*observer,void const*bytes,size_t n,uint64_t*out){
 uwvm_rmw_service_transport*s=state;uint32_t raw=enter(s);if(raw)return raw;service_slot*p=find(s,source,expected);
 if(!p||!pair(p,observer)||p->facts.kind!=3){leave(s);return 13;}if(!out||(!bytes&&n)||n>p->request_bound){leave(s);return 22;}
 if(p->facts.poisoned||p->facts.retire_attempted){raw=p->facts.first_primary?p->facts.first_primary:125;leave(s);return raw;}
 if(p->count==s->transaction_capacity||p->facts.submit_attempts==UINT64_MAX){leave(s);return 90;}
 uwvm_rmw_c_native_endpoint inspected={0};raw=inspect_slot(s,p,&inspected);if(raw){leave(s);return raw;}
 transaction*t=p->transactions+p->count++;t->state=TRANSACTION_RESERVED;p->facts.transaction_slots_used=p->count;++p->facts.submit_attempts;
 memset(s->ticket,0,8);raw=uwvm_ros2_rmw_srv_v1_client_submit(source->handle,p->admitted.native_incarnation,0,(uint64_t)(uintptr_t)bytes,n,(uint64_t)(uintptr_t)s->ticket);p->facts.submit_code=raw;
 if(raw){if(raw!=11)(void)terminal(p,raw);leave(s);return raw;}
 uint64_t value=le(s->ticket,8);if(!value||ticket(p,value)){raw=terminal(p,22);leave(s);return raw;}
 t->ticket=value;t->state=TRANSACTION_ADMITTED;p->facts.last_ticket=value;*out=value;leave(s);return 0;
}
static uint32_t respond_cb(void*state,uwvm_rmw_c_native_endpoint const*expected,roswasmc_capability const*source,roswasmc_capability const*responder,uint64_t value,void const*bytes,size_t n){
 uwvm_rmw_service_transport*s=state;uint32_t raw=enter(s);if(raw)return raw;service_slot*p=find(s,source,expected);
 if(!p||!pair(p,responder)||p->facts.kind!=4){leave(s);return 13;}if((!bytes&&n)||n>p->response_bound){leave(s);return 22;}
 transaction*t=ticket(p,value);if(!t||t->state!=TRANSACTION_DELIVERED){leave(s);return 9;}
 if(p->facts.poisoned||p->facts.retire_attempted){raw=p->facts.first_primary?p->facts.first_primary:125;leave(s);return raw;}
 if(p->facts.response_attempts==UINT64_MAX){leave(s);return 90;}
 uwvm_rmw_c_native_endpoint inspected={0};raw=inspect_slot(s,p,&inspected);if(raw){leave(s);return raw;}
 t->state=TRANSACTION_DONE;++p->facts.response_attempts;raw=uwvm_ros2_rmw_srv_v1_service_respond(source->handle,p->admitted.native_incarnation,responder->handle,value,0,(uint64_t)(uintptr_t)bytes,n);p->facts.respond_code=raw;
 if(raw)(void)terminal(p,raw);leave(s);return raw;
}
static int overlap(void const*a,size_t an,void const*b,size_t bn){if(!an||!bn)return 0;uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;if(x>UINTPTR_MAX-an||y>UINTPTR_MAX-bn)return 1;return x<y+bn&&y<x+an;}
static int bound(unsigned char const*b,uwvm_rmw_c_native_endpoint const*e){return b[7]==2&&le(b+16,8)==e->source&&le(b+24,8)==e->instance&&le(b+32,8)==e->runtime_generation&&le(b+40,8)==e->session&&le(b+48,8)==e->native_incarnation;}
typedef struct delivered {uint64_t sequence,reservation,ticket;size_t body;unsigned kind,flags;uint32_t error;rmw_service_info_t info;int present;} delivered;
static uint32_t envelope(unsigned char const*b,size_t capacity,size_t maximum,service_slot const*p,delivered*out){
 if(le(b,4)!=UINT32_C(0x43524d55)||le(b+4,2)!=1||b[6]!=5||le(b+8,4)!=160||b[12]!=p->facts.kind||b[13]!=RMW_GID_STORAGE_SIZE||!zero(b+14,2)||!zero(b+128,32)||!bound(b,&p->admitted)||le(b+56,8)!=p->facts.peer||!le(b+72,8)||!le(b+80,8)||le(b+88,8)||le(b+96,8)!=capacity||le(b+112,8)||le(b+120,8))return 22;
 uint64_t n=le(b+104,8);if(n<192||n>capacity-160||n-192>maximum)return 90;
 delivered d={0};d.sequence=le(b+72,8);d.reservation=le(b+80,8);d.ticket=le(b+64,8);d.body=(size_t)n-192;*out=d;return 0;
}
#if UWVM_RMW_SERVICE_INFO_API
static uint32_t information(unsigned char const*b,service_slot const*p,delivered*d){
 uint64_t flags=le(b+12,4);unsigned kind=b[6],control=kind==6||kind==7;
 if(le(b,4)!=UINT32_C(0x494d5255)||le(b+4,2)!=1||le(b+8,4)!=192||!bound(b,&p->admitted)||le(b+56,8)!=d->sequence||le(b+64,8)!=d->ticket||le(b+112,8)!=d->body||b[123]||!zero(b+176,16))return 22;
 d->kind=kind;d->flags=b[122];d->error=(uint32_t)le(b+124,4);
 if(control){if(flags||d->body||b[120]||b[121]||!zero(b+128,48)||le(b+72,8)||le(b+80,8)||le(b+88,8)!=UINT64_MAX||le(b+96,8)!=UINT64_MAX||le(b+104,8)||(d->flags&~3u)||(kind==6?!d->error:!(d->flags&1u)))return 22;d->present=0;return 0;}
 if(kind!=(p->facts.kind==3?3u:2u)||flags!=35||!d->ticket||d->error||d->flags||b[120]||b[121]!=RMW_GID_STORAGE_SIZE||!zero(b+128,32)||le(b+88,8)!=UINT64_MAX||le(b+96,8)!=UINT64_MAX)return b[121]!=RMW_GID_STORAGE_SIZE?95:22;
 if(!zero(b+160+RMW_GID_STORAGE_SIZE,16-RMW_GID_STORAGE_SIZE))return 22;
 rmw_service_info_t info={0};uint64_t a=le(b+72,8),c=le(b+80,8),seq=le(b+104,8);memcpy(&info.source_timestamp,&a,8);memcpy(&info.received_timestamp,&c,8);memcpy(&info.request_id.sequence_number,&seq,8);memcpy(info.request_id.writer_guid,b+160,RMW_GID_STORAGE_SIZE);d->info=info;d->present=1;return 0;
}
#endif
static uint32_t cancel_borrow(service_slot*p,delivered const*d,uint32_t reason){primary(p,reason);p->facts.cancel_attempted=1;++p->facts.cancels;p->facts.cancel_code=uwvm_ros2_rmw_srv_v1_endpoint_cancel(p->facts.source,p->admitted.native_incarnation,d->sequence,d->reservation);if(p->facts.cancel_code)(void)terminal(p,reason);return reason;}
static uint32_t commit_borrow(service_slot*p,delivered const*d){p->facts.commit_attempted=1;p->facts.commit_succeeded=0;++p->facts.commits;uint32_t raw=uwvm_ros2_rmw_srv_v1_endpoint_commit(p->facts.source,p->admitted.native_incarnation,d->sequence,d->reservation);p->facts.commit_code=raw;if(raw)return terminal(p,raw);p->facts.commit_succeeded=1;p->facts.last_committed_sequence=d->sequence;return 0;}
/* All destinations are private C-owned objects; validate disjoint caller
 * outputs and this provider's complete state/scratch before borrowing. */
static int unsafe_output(uwvm_rmw_service_transport*s,void*p,size_t n){size_t state_bytes=sizeof*s+s->capacity*sizeof(service_slot);size_t history=s->capacity*s->transaction_capacity*sizeof(transaction);return overlap(p,n,s,state_bytes)||overlap(p,n,s->packet,s->packet_capacity)||overlap(p,n,s->event,s->event_capacity)||overlap(p,n,s->slots[0].transactions,history);}
static uint32_t take_common(uwvm_rmw_service_transport*s,uwvm_rmw_c_native_endpoint const*expected,roswasmc_capability const*source,roswasmc_capability const*observer,void*canonical,size_t capacity,size_t*length,uint64_t*request_ticket,rmw_service_info_t*info,unsigned*present,int response){
#if !UWVM_RMW_SERVICE_INFO_API
 (void)s;(void)expected;(void)source;(void)observer;(void)canonical;(void)capacity;(void)length;(void)request_ticket;(void)info;(void)present;(void)response;return 95;
#else
 if(RMW_GID_STORAGE_SIZE!=16)return 95;
 uint32_t raw=enter(s);if(raw)return raw;service_slot*p=find(s,source,expected);
 if(!p||p->facts.kind!=(response?3:4)||(response&&!pair(p,observer))){leave(s);return 13;}
 size_t bound_bytes=response?p->response_bound:p->request_bound,needed=response?64u+bound_bytes:bound_bytes;
 if(!length||!info||(!canonical&&capacity)||capacity<needed||(!response&&!request_ticket)||(response&&!present)){leave(s);return capacity<needed?90:22;}
 void*outputs[5]={canonical,length,info,response?(void*)present:(void*)request_ticket,NULL};size_t lengths[5]={capacity,sizeof*length,sizeof*info,response?sizeof*present:sizeof*request_ticket,0};
 for(unsigned i=0;i<4;++i){if(unsafe_output(s,outputs[i],lengths[i])){leave(s);return 22;}for(unsigned j=0;j<i;++j)if(overlap(outputs[i],lengths[i],outputs[j],lengths[j])){leave(s);return 22;}}
 if(p->facts.poisoned||p->facts.retire_attempted){raw=p->facts.first_primary?p->facts.first_primary:125;leave(s);return raw;}
 if((!response&&p->count==s->transaction_capacity)||p->facts.peeks==UINT64_MAX||p->facts.commits==UINT64_MAX||p->facts.cancels==UINT64_MAX){leave(s);return 90;}
 uwvm_rmw_c_native_endpoint inspected={0};raw=inspect_slot(s,p,&inspected);if(raw){leave(s);return raw;}
 size_t total=OVERHEAD+bound_bytes;memset(s->packet,0xa5,total);++p->facts.peeks;
 raw=uwvm_ros2_rmw_srv_v1_endpoint_peek(source->handle,p->admitted.native_incarnation,RMW_GID_STORAGE_SIZE,0,(uint64_t)(uintptr_t)s->packet,total);p->facts.peek_code=raw;p->facts.peek_validation=0;
 if(raw){if(raw!=11)primary(p,raw);if(raw!=11&&raw!=90&&raw!=95)(void)terminal(p,raw);leave(s);return raw;}
 memcpy(p->facts.last_record_wire,s->packet,OVERHEAD);p->facts.record_wire_present=1;
 delivered d={0};raw=envelope(s->packet,total,bound_bytes,p,&d);if(raw){p->facts.peek_validation=raw;(void)terminal(p,raw);leave(s);return raw;}
 if(d.sequence<=p->facts.last_committed_sequence||(response&&(p->facts.last_committed_sequence==UINT64_MAX||d.sequence!=p->facts.last_committed_sequence+1))||d.reservation<=p->facts.last_reservation){p->facts.peek_validation=13;raw=terminal(p,13);leave(s);return raw;}
 p->facts.last_sequence=d.sequence;p->facts.last_reservation=d.reservation;raw=information(s->packet+160,p,&d);p->facts.peek_validation=raw;
 if(raw){raw=cancel_borrow(p,&d,raw);leave(s);return raw;}
 transaction*t=ticket(p,d.ticket);
 if(d.present){if(!response){for(size_t i=0;i<p->count;++i)if((p->transactions[i].native_info.request_id.sequence_number==d.info.request_id.sequence_number&&!memcmp(p->transactions[i].native_info.request_id.writer_guid,d.info.request_id.writer_guid,16))){raw=cancel_borrow(p,&d,13);(void)terminal(p,raw);leave(s);return raw;}}if(response?(!t||t->state!=TRANSACTION_ADMITTED):(t!=NULL)){raw=cancel_borrow(p,&d,13);(void)terminal(p,raw);leave(s);return raw;}}
 if(response){memset(s->event,0,64);put(s->event,UINT32_C(0x45525755),4);put(s->event+4,1,2);put(s->event+6,d.kind,2);s->event[8]=2;s->event[9]=(unsigned char)d.flags;put(s->event+12,d.error,4);put(s->event+16,p->admitted.source,8);put(s->event+24,d.ticket,8);put(s->event+32,d.sequence,8);put(s->event+40,p->admitted.instance,8);put(s->event+48,p->admitted.session,8);put(s->event+56,d.body,8);if(d.body)memcpy(s->event+64,s->packet+OVERHEAD,d.body);}
 raw=commit_borrow(p,&d);if(raw){leave(s);return raw;}
 if(!response&&!d.present){raw=terminal(p,d.error?d.error:125);leave(s);return raw;}
 if(d.present){if(response)t->state=TRANSACTION_DONE;else{t=p->transactions+p->count++;t->ticket=d.ticket;t->state=TRANSACTION_DELIVERED;t->native_info=d.info;p->facts.transaction_slots_used=p->count;}p->facts.last_ticket=d.ticket;}
 if(response){memcpy(canonical,s->event,64+d.body);*length=64+d.body;*present=d.present?1u:0u;if(d.present)*info=d.info;}
 else{if(d.body)memcpy(canonical,s->packet+OVERHEAD,d.body);*length=d.body;*request_ticket=d.ticket;*info=d.info;}
 leave(s);return 0;
#endif
}
static uint32_t take_request_cb(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,void*bytes,size_t n,size_t*length,uint64_t*t,rmw_service_info_t*info){return take_common(state,e,source,NULL,bytes,n,length,t,info,NULL,0);}
static uint32_t take_response_cb(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*observer,roswasmc_capability const*source,void*bytes,size_t n,size_t*length,rmw_service_info_t*info,unsigned*present){return take_common(state,e,source,observer,bytes,n,length,NULL,info,present,1);}
static uint32_t take_topic_cb(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,void*bytes,size_t n,size_t*length,rmw_message_info_t*info){uwvm_rmw_service_transport*s=state;if(!s||s->magic!=STATE_MAGIC||!s->composed)return 95;return s->subscription_metadata.take_topic(s->subscription_metadata.state,e,source,bytes,n,length,info);}
uint32_t uwvm_rmw_service_transport_create(size_t slots,size_t transactions,size_t request,size_t response,rcutils_allocator_t const*a,uwvm_rmw_service_transport**out){
 if(!out||!a||!rcutils_allocator_is_valid(a)||!slots||slots>UWVM_RMW_SERVICE_TRANSPORT_MAX_SLOTS||!transactions||transactions>UWVM_RMW_SERVICE_TRANSPORT_MAX_TRANSACTIONS||request>INPUT_MAX||response>INPUT_MAX||request>UWVM_RMW_MAX_CANONICAL_BYTES||response>UWVM_RMW_MAX_CANONICAL_BYTES||slots>(SIZE_MAX-sizeof(uwvm_rmw_service_transport))/sizeof(service_slot)||transactions>SIZE_MAX/slots/sizeof(transaction)||slots*transactions*sizeof(transaction)>UWVM_RMW_SERVICE_TRANSPORT_MAX_HISTORY_BYTES)return 22;
 size_t bytes=sizeof(uwvm_rmw_service_transport)+slots*sizeof(service_slot),history=slots*transactions*sizeof(transaction),maximum=request>response?request:response;
 uwvm_rmw_service_transport*s=a->zero_allocate(1,bytes,a->state);if(!s)return 12;
 transaction*t=a->zero_allocate(1,history,a->state);s->packet=a->allocate(OVERHEAD+maximum,a->state);s->event=a->allocate(64u+response,a->state);
 if(!t||!s->packet||!s->event){if(t)a->deallocate(t,a->state);if(s->packet)a->deallocate(s->packet,a->state);if(s->event)a->deallocate(s->event,a->state);a->deallocate(s,a->state);return 12;}
 s->allocator=*a;s->capacity=slots;s->transaction_capacity=transactions;s->maximum_request=request;s->maximum_response=response;s->packet_capacity=OVERHEAD+maximum;s->event_capacity=64u+response;
 for(size_t i=0;i<slots;++i)s->slots[i].transactions=t+i*transactions;s->magic=STATE_MAGIC;atomic_init(&s->busy,false);*out=s;return 0;
}
static uint32_t export_callbacks(uwvm_rmw_service_transport*s,uwvm_rmw_c_activation_provider*a,uwvm_rmw_c_request_provider*q,uwvm_rmw_c_native_metadata_provider*m){
 if(!a||!q||!m)return 22;uwvm_rmw_c_activation_provider activation={1,s,activate_cb,inspect_cb,retire_cb};uwvm_rmw_c_request_provider requests={1,s,submit_cb,respond_cb};uwvm_rmw_c_native_metadata_provider metadata={1,s,take_topic_cb,take_request_cb,take_response_cb};*a=activation;*q=requests;*m=metadata;s->exported=1;return 0;
}
uint32_t uwvm_rmw_service_transport_callbacks(uwvm_rmw_service_transport*s,uwvm_rmw_c_activation_provider*a,uwvm_rmw_c_request_provider*q,uwvm_rmw_c_native_metadata_provider*m){
#if !UWVM_RMW_SERVICE_INFO_API
 (void)s;(void)a;(void)q;(void)m;return 95;
#else
 if(RMW_GID_STORAGE_SIZE!=16)return 95;uint32_t raw=enter(s);if(raw)return raw;raw=export_callbacks(s,a,q,m);leave(s);return raw;
#endif
}
uint32_t uwvm_rmw_service_transport_compose_subscription(uwvm_rmw_service_transport*s,uwvm_rmw_c_activation_provider const*a,uwvm_rmw_c_native_metadata_provider const*m,uwvm_rmw_c_activation_provider*outa,uwvm_rmw_c_request_provider*outq,uwvm_rmw_c_native_metadata_provider*outm){
#if !UWVM_RMW_SERVICE_INFO_API
 (void)s;(void)a;(void)m;(void)outa;(void)outq;(void)outm;return 95;
#else
 if(RMW_GID_STORAGE_SIZE!=16)return 95;uint32_t raw=enter(s);if(raw)return raw;
 if(s->exported||s->composed||s->count){leave(s);return 17;}
 if(!a||!m||a->version!=1||m->version!=1||!a->activate||!a->inspect||!a->retire||!m->take_topic||!outa||!outq||!outm){leave(s);return 22;}
 s->subscription_activation=*a;s->subscription_metadata=*m;s->composed=1;raw=export_callbacks(s,outa,outq,outm);leave(s);return raw;
#endif
}
uint32_t uwvm_rmw_service_transport_available(uwvm_rmw_service_transport*s,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*call,roswasmc_capability const*availability,uint64_t wait,bool*out){
 uint32_t raw=enter(s);if(raw)return raw;service_slot*p=find(s,call,e);
 if(!p||p->facts.kind!=3||!source_valid(availability)||availability->context!=call->context||availability->handle==call->handle||availability->handle==p->facts.peer){leave(s);return 13;}
 if(!out||wait>UINT64_C(60000000000)){leave(s);return 22;}uwvm_rmw_c_native_endpoint observed={0};raw=inspect_slot(s,p,&observed);if(raw){leave(s);return raw;}
 s->available=0xa5;p->facts.available_attempted=1;raw=uwvm_ros2_rmw_srv_v1_client_available(call->handle,p->admitted.native_incarnation,availability->handle,wait,0,(uint64_t)(uintptr_t)&s->available);p->facts.available_code=raw;
 if(!raw&&s->available>1)raw=22;if(!raw)*out=s->available!=0;primary(p,raw);leave(s);return raw;
}
static uint32_t availability_query_cb(void*state,uwvm_rmw_c_native_endpoint const*e,
 roswasmc_capability const*call,roswasmc_capability const*availability,unsigned*out){
 if(!out)return 22;bool actual=false;
 uint32_t raw=uwvm_rmw_service_transport_available(state,e,call,availability,0,&actual);
 if(!raw)*out=actual?1u:0u;return raw;
}
uint32_t uwvm_rmw_service_transport_availability_callbacks(uwvm_rmw_service_transport*s,
 uwvm_rmw_c_availability_provider*out){
#if !UWVM_RMW_SERVICE_INFO_API
 (void)s;(void)out;return 95;
#else
 if(RMW_GID_STORAGE_SIZE!=16)return 95;uint32_t raw=enter(s);if(raw)return raw;
 if(!out){leave(s);return 22;}
 uwvm_rmw_c_availability_provider next={1,s,availability_query_cb};
 s->exported=1;*out=next;leave(s);return 0;
#endif
}
uint32_t uwvm_rmw_service_transport_abandon(uwvm_rmw_service_transport*s,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*call,uint64_t value){
 uint32_t raw=enter(s);if(raw)return raw;service_slot*p=find(s,call,e);if(!p||p->facts.kind!=3){leave(s);return 13;}transaction*t=ticket(p,value);
 if(!t||t->state!=TRANSACTION_ADMITTED){leave(s);return 9;}uwvm_rmw_c_native_endpoint next={0};raw=inspect_slot(s,p,&next);if(raw){leave(s);return raw;}
 t->state=TRANSACTION_ABANDONED;p->facts.abandon_attempted=1;raw=uwvm_ros2_rmw_srv_v1_client_abandon(call->handle,p->admitted.native_incarnation,value);p->facts.abandon_code=raw;if(raw)(void)terminal(p,raw);leave(s);return raw;
}
uint32_t uwvm_rmw_service_transport_count(uwvm_rmw_service_transport*s,size_t*out){uint32_t raw=enter(s);if(raw)return raw;if(!out){leave(s);return 22;}*out=s->count;leave(s);return 0;}
uint32_t uwvm_rmw_service_transport_diagnostics(uwvm_rmw_service_transport*s,size_t i,uwvm_rmw_service_transport_diagnostic*out){uint32_t raw=enter(s);if(raw)return raw;if(!out||i>=s->count){leave(s);return 22;}*out=s->slots[i].facts;leave(s);return 0;}
uint32_t uwvm_rmw_service_transport_record_raw_close(uwvm_rmw_service_transport*s,size_t i,unsigned owner,uint32_t code){uint32_t raw=enter(s);if(raw)return raw;if(i>=s->count||owner>1||s->slots[i].facts.raw_close_reported[owner]){leave(s);return 22;}s->slots[i].facts.raw_close_reported[owner]=1;s->slots[i].facts.raw_close_code[owner]=code;leave(s);return 0;}
uint32_t uwvm_rmw_service_transport_dispose_unpublished(uwvm_rmw_service_transport**state){if(!state||!*state)return 22;uwvm_rmw_service_transport*s=*state;uint32_t raw=enter(s);if(raw)return raw;if(s->exported||s->count){leave(s);return 17;}rcutils_allocator_t a=s->allocator;void*history=s->slots[0].transactions,*packet=s->packet,*event=s->event;s->magic=0;*state=NULL;a.deallocate(history,a.state);a.deallocate(packet,a.state);a.deallocate(event,a.state);a.deallocate(s,a.state);return 0;}
