/* SPDX-License-Identifier: Apache-2.0
 * Real raw7 transport. No allocation after bootstrap, ordinary event_poll,
 * fabricated native fields, legacy listener fallback, or automatic ACK replay.
 * UMRE/UMRS/URMI numbers are the independently reviewed exact wire vocabulary.
 */
#include <uwvm_rmw/subscription_transport.h>
#include <roswasm/rmw_subscription_v1.h>
#include <rmw/time.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#if defined(__has_include)
#if __has_include("uwvm_rmw_subscription_features.h")
#include "uwvm_rmw_subscription_features.h"
#endif
#endif
#ifndef UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API
#define UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API 0
#endif
#define STATE_MAGIC UINT32_C(0x54535755)
#define IPC_MAX UINT64_C(4194304)
#define PACKET_OVERHEAD 288u
typedef struct subscription_slot {
 roswasmc_capability const*owner;roswasmc_context const*context;uwvm_rmw_codec const*codec;
 size_t canonical_bound;uwvm_rmw_c_native_endpoint admitted;
 uwvm_rmw_subscription_transport_diagnostic facts;
} subscription_slot;
struct uwvm_rmw_subscription_transport {
 uint32_t magic;atomic_bool busy;int exported;rcutils_allocator_t allocator;
 size_t capacity,count,maximum_body,packet_capacity;
 unsigned char*packet;unsigned char receipt[256];subscription_slot slots[];
};
static uint32_t enter(uwvm_rmw_subscription_transport*s){
 if(!s||s->magic!=STATE_MAGIC)return 22;
 return atomic_exchange_explicit(&s->busy,true,memory_order_acquire)?17:0;
}
static void leave(uwvm_rmw_subscription_transport*s){atomic_store_explicit(&s->busy,false,memory_order_release);}
static uint64_t le(unsigned char const*p,size_t n){uint64_t v=0;for(size_t i=0;i<n;++i)v|=(uint64_t)p[i]<<(8*i);return v;}
static int zero(unsigned char const*p,size_t n){for(size_t i=0;i<n;++i)if(p[i])return 0;return 1;}
static int full_handle(uint64_t n){return (uint32_t)n&&(uint32_t)(n>>32);}
static int source_valid(roswasmc_capability const*c){return roswasmc_is_open(c)&&c->context->backend->version==2&&full_handle(c->handle);}
static void primary(subscription_slot*p,uint32_t n){if(n&&!p->facts.first_primary)p->facts.first_primary=n;}
static int same_binding(uwvm_rmw_c_native_endpoint const*a,uwvm_rmw_c_native_endpoint const*b){
 return a->version==1&&b->version==1&&a->backend==2&&b->backend==2&&a->source==b->source&&a->instance==b->instance&&
 a->runtime_generation==b->runtime_generation&&a->session==b->session&&a->native_incarnation==b->native_incarnation;
}
static int same_receipt(uwvm_rmw_c_native_endpoint const*a,uwvm_rmw_c_native_endpoint const*b){
 return same_binding(a,b)&&a->qos_count==1&&b->qos_count==1&&!a->native_gid_bytes&&!b->native_gid_bytes&&
 zero(a->native_gid,32)&&zero(b->native_gid,32)&&uwvm_rmw_qos_equal(a->actual_qos,b->actual_qos);
}
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
static uint32_t receipt_decode(unsigned char const*b,uint32_t op,uint64_t source,uwvm_rmw_c_native_endpoint*out){
 if(le(b,4)!=UINT32_C(0x45524d55)||le(b+4,2)!=1||b[6]!=op||b[7]!=2||le(b+8,4)!=256||b[12]!=2||b[13]!=1||
 b[14]||b[15]||!zero(b+56,8)||!zero(b+144,112))return 22;
 uwvm_rmw_c_native_endpoint x={0};x.version=1;x.backend=2;x.source=le(b+16,8);x.instance=le(b+24,8);
 x.runtime_generation=le(b+32,8);x.session=le(b+40,8);x.native_incarnation=le(b+48,8);x.qos_count=1;
 if(x.source!=source||!full_handle(x.source)||!x.instance||!x.runtime_generation||!x.session||!x.native_incarnation)return 22;
 uint32_t raw=qos_decode(b+64,x.actual_qos);if(raw)return raw;*out=x;return 0;
}
static subscription_slot*find(uwvm_rmw_subscription_transport*s,roswasmc_capability const*source,uwvm_rmw_c_native_endpoint const*expected){
 if(!source_valid(source)||!expected)return NULL;
 for(size_t i=0;i<s->count;++i){subscription_slot*p=s->slots+i;
 if(p->facts.associated&&p->owner==source&&p->context==source->context&&p->facts.source==source->handle&&same_receipt(expected,&p->admitted))return p;}
 return NULL;
}
static uint32_t inspect_slot(uwvm_rmw_subscription_transport*s,subscription_slot*p,roswasmc_capability const*source,uwvm_rmw_c_native_endpoint*out){
 if(p->facts.retire_attempted)return 125;if(p->facts.poisoned)return p->facts.first_primary?p->facts.first_primary:125;
 if(p->facts.activation_validation)return p->facts.activation_validation;
 memset(s->receipt,0xa5,256);uint32_t raw=uwvm_ros2_rmw_sub_v1_subscription_inspect(source->handle,p->admitted.native_incarnation,0,(uint64_t)(uintptr_t)s->receipt);
 p->facts.inspect_code=raw;p->facts.inspect_validation=0;if(raw){primary(p,raw);return raw;}
 memcpy(p->facts.inspect_wire,s->receipt,256);p->facts.inspect_wire_present=1;uwvm_rmw_c_native_endpoint next={0};
 raw=receipt_decode(s->receipt,2,source->handle,&next);
 if(!raw&&(!same_receipt(&next,&p->admitted)||memcmp(s->receipt,p->facts.original_wire,6)||memcmp(s->receipt+7,p->facts.original_wire+7,249)))raw=13;
 p->facts.inspect_validation=raw;primary(p,raw);if(!raw)*out=next;return raw;
}
static uint32_t retire_slot(subscription_slot*p,roswasmc_capability const*source){
 if(p->facts.retire_attempted)return p->facts.retire_code;p->facts.retire_attempted=1;
 uint32_t raw=uwvm_ros2_rmw_sub_v1_subscription_retire(source->handle,p->admitted.native_incarnation);p->facts.retire_code=raw;return raw;
}
static uint32_t activate_cb(void*state,enum uwvm_rmw_endpoint_kind kind,roswasmc_capability const*source,roswasmc_capability const*aux,
 uwvm_rmw_codec const*codec,rmw_qos_profile_t const*requested,uwvm_rmw_c_native_endpoint*out){
 if(kind!=UWVM_RMW_SUBSCRIPTION)return 95;
 uwvm_rmw_subscription_transport*s=state;uint32_t raw=enter(s);if(raw)return raw;
 if(aux||!source_valid(source)||!codec||!requested||!out||codec->version!=1||codec->kind!=UWVM_RMW_MESSAGE_CODEC||
 !codec->generated_support||!codec->validate||!codec->encode||!codec->decode||codec->request||codec->response||codec->maximum_wire_bytes>s->maximum_body){leave(s);return 22;}
 raw=codec->validate(codec,codec->generated_support);if(raw){leave(s);return raw;}
 for(size_t i=0;i<s->count;++i)if(s->slots[i].facts.source==source->handle){leave(s);return 13;}
 if(s->count==s->capacity){leave(s);return 90;}
 subscription_slot*p=s->slots+s->count++;p->owner=source;p->context=source->context;p->codec=codec;p->canonical_bound=codec->maximum_wire_bytes;p->facts.source=source->handle;
 p->facts.abi_version=uwvm_ros2_rmw_sub_v1_abi_version();if(p->facts.abi_version!=1){primary(p,95);leave(s);return 95;}
 memset(s->receipt,0xa5,256);p->facts.activation_attempted=1;
 raw=uwvm_ros2_rmw_sub_v1_subscription_activate(source->handle,0,(uint64_t)(uintptr_t)s->receipt);p->facts.activation_code=raw;
 if(raw){primary(p,raw);leave(s);return raw;}
 memcpy(p->facts.original_wire,s->receipt,256);p->facts.original_wire_present=1;uwvm_rmw_c_native_endpoint next={0};
 raw=receipt_decode(s->receipt,1,source->handle,&next);p->facts.activation_validation=raw;
 if(raw){primary(p,raw);leave(s);return raw;}
 p->admitted=next;p->facts.associated=1;
 // Complete actual effective QoS is supplied to C, which compares its separately
 // selected effective row. Do not resolve SYSTEM_DEFAULT using this declaration.
 *out=next;leave(s);return 0;
}
static uint32_t inspect_cb(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,
 uwvm_rmw_c_native_endpoint const*expected,uwvm_rmw_c_native_endpoint*out){
 uwvm_rmw_subscription_transport*s=state;uint32_t raw=enter(s);if(raw)return raw;
 if(aux||!out){leave(s);return 22;}subscription_slot*p=find(s,source,expected);if(!p){leave(s);return 13;}
 uwvm_rmw_c_native_endpoint next={0};raw=inspect_slot(s,p,source,&next);if(!raw)*out=next;leave(s);return raw;
}
static uint32_t retire_cb(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_c_native_endpoint const*expected){
 uwvm_rmw_subscription_transport*s=state;uint32_t raw=enter(s);if(raw)return raw;
 if(aux){leave(s);return 22;}subscription_slot*p=find(s,source,expected);if(!p){leave(s);return 13;}
 raw=retire_slot(p,source);leave(s);return raw;
}
/* A valid returned UMRS header establishes the ONLY token this attempt may
 * cancel. A malformed or unassociated header never supplies trusted token data.
 * The complete private packet is validated before any result is published. */
static int bound(unsigned char const*b,uwvm_rmw_c_native_endpoint const*e){
 return b[7]==2&&le(b+16,8)==e->source&&le(b+24,8)==e->instance&&le(b+32,8)==e->runtime_generation&&le(b+40,8)==e->session&&le(b+48,8)==e->native_incarnation;
}
static uint32_t envelope(unsigned char const*b,size_t capacity,size_t maximum,uwvm_rmw_c_native_endpoint const*e,
 uint64_t*sequence,uint64_t*reservation,size_t*body){
 if(le(b,4)!=UINT32_C(0x53524d55)||le(b+4,2)!=1||b[6]!=1||le(b+8,4)!=96||b[12]!=RMW_GID_STORAGE_SIZE||
 !zero(b+13,3)||!zero(b+88,8)||!bound(b,e)||le(b+72,8)!=capacity||!le(b+56,8)||!le(b+64,8))return 22;
 uint64_t n=le(b+80,8);if(n<192||n>capacity-96||n-192>maximum)return 90;
 *sequence=le(b+56,8);*reservation=le(b+64,8);*body=(size_t)n-192;return 0;
}
#if UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API
static uint32_t native_information(unsigned char const*b,size_t n,uwvm_rmw_c_native_endpoint const*e,uint64_t sequence,rmw_message_info_t*out){
 uint64_t flags=le(b+12,4);if(le(b,4)!=UINT32_C(0x494d5255)||le(b+4,2)!=1||b[6]!=1||le(b+8,4)!=192||
 (flags&~UINT64_C(31))||(flags&7)!=7||!bound(b,e)||le(b+56,8)!=sequence||le(b+64,8)||le(b+104,8)||le(b+112,8)!=n||
 b[121]||b[122]||b[123]||le(b+124,4)||!zero(b+160,32))return 22;
 if(b[120]!=RMW_GID_STORAGE_SIZE)return 95;if(!zero(b+128+b[120],32-b[120]))return 22;
 uint64_t publication=le(b+88,8),reception=le(b+96,8);
 if(((flags&8)&&publication==UINT64_MAX)||(!(flags&8)&&publication!=UINT64_MAX)||
 ((flags&16)&&reception==UINT64_MAX)||(!(flags&16)&&reception!=UINT64_MAX))return 22;
 rmw_message_info_t info={0};uint64_t source_time=le(b+72,8),received_time=le(b+80,8);
 // Preserve every bit of the actual signed native timestamps. Zero is data.
 memcpy(&info.source_timestamp,&source_time,8);memcpy(&info.received_timestamp,&received_time,8);
 info.publication_sequence_number=(flags&8)?publication:RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED;
 info.reception_sequence_number=(flags&16)?reception:RMW_MESSAGE_INFO_SEQUENCE_NUMBER_UNSUPPORTED;
 memcpy(info.publisher_gid.data,b+128,RMW_GID_STORAGE_SIZE);info.publisher_gid.implementation_identifier=uwvm_rmw_identifier;
 // This real Guest receive traversed Host IPC, not Guest intra-process delivery.
 info.from_intra_process=false;*out=info;return 0;
}
#endif
static int overlap(void const*a,size_t an,void const*b,size_t bn){
 if(!an||!bn)return 0;uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;
 if(x>UINTPTR_MAX-an||y>UINTPTR_MAX-bn)return 1;return x<y+bn&&y<x+an;
}
static uint32_t terminal(roswasmc_capability const*source,subscription_slot*p,uint32_t reason){
 primary(p,reason);p->facts.poisoned=1;if(!p->facts.retire_attempted)(void)retire_slot(p,source);return reason;
}
static uint32_t cancel_borrow(roswasmc_capability const*source,subscription_slot*p,uint64_t sequence,uint64_t reservation,uint32_t reason){
 primary(p,reason);p->facts.cancel_attempted=1;++p->facts.cancels;
 uint32_t code=uwvm_ros2_rmw_sub_v1_subscription_cancel(source->handle,p->admitted.native_incarnation,sequence,reservation);p->facts.cancel_code=code;
 if(code)(void)terminal(source,p,reason);return reason;
}
static uint32_t take_cb(void*state,uwvm_rmw_c_native_endpoint const*expected,roswasmc_capability const*source,
 void*canonical,size_t capacity,size_t*length,rmw_message_info_t*information){
#if !UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API
 (void)state;(void)expected;(void)source;(void)canonical;(void)capacity;(void)length;(void)information;return 95;
#else
 if(RMW_GID_STORAGE_SIZE==0||RMW_GID_STORAGE_SIZE>32)return 95; // before borrowing
 uwvm_rmw_subscription_transport*s=state;uint32_t raw=enter(s);if(raw)return raw;
 subscription_slot*p=find(s,source,expected);if(!p){leave(s);return 13;}
 if(!length||!information||(!canonical&&capacity)||capacity<p->canonical_bound){leave(s);return capacity<p->canonical_bound?90:22;}
 size_t state_bytes=sizeof*s+s->capacity*sizeof(subscription_slot);
 if(overlap(canonical,capacity,length,sizeof*length)||overlap(canonical,capacity,information,sizeof*information)||overlap(length,sizeof*length,information,sizeof*information)||
 overlap(canonical,capacity,s,state_bytes)||overlap(length,sizeof*length,s,state_bytes)||overlap(information,sizeof*information,s,state_bytes)||
 overlap(canonical,capacity,s->packet,s->packet_capacity)||overlap(length,sizeof*length,s->packet,s->packet_capacity)||overlap(information,sizeof*information,s->packet,s->packet_capacity)){leave(s);return 22;}
 if(p->facts.poisoned||p->facts.retire_attempted){raw=p->facts.first_primary?p->facts.first_primary:125;leave(s);return raw;}
 if(p->facts.peeks==UINT64_MAX||p->facts.commits==UINT64_MAX||p->facts.cancels==UINT64_MAX){leave(s);return 90;}
 uwvm_rmw_c_native_endpoint inspected={0};raw=inspect_slot(s,p,source,&inspected);if(raw){leave(s);return raw;}
 size_t total=PACKET_OVERHEAD+p->canonical_bound;memset(s->packet,0xa5,total);++p->facts.peeks;
 raw=uwvm_ros2_rmw_sub_v1_subscription_peek(source->handle,p->admitted.native_incarnation,RMW_GID_STORAGE_SIZE,0,(uint64_t)(uintptr_t)s->packet,total);
 p->facts.peek_code=raw;p->facts.peek_validation=0;
 if(raw){if(raw!=11)primary(p,raw);if(raw!=11&&raw!=90&&raw!=95)(void)terminal(source,p,raw);leave(s);return raw;}
 uint64_t sequence=0,reservation=0;size_t n=0;
 raw=envelope(s->packet,total,p->canonical_bound,expected,&sequence,&reservation,&n);
 if(raw){p->facts.peek_validation=raw;(void)terminal(source,p,raw);leave(s);return raw;}
 if(sequence<=p->facts.last_committed_sequence||reservation<=p->facts.last_reservation){p->facts.peek_validation=13;(void)terminal(source,p,13);leave(s);return 13;}
 p->facts.last_sequence=sequence;p->facts.last_reservation=reservation;
 rmw_message_info_t info={0};raw=native_information(s->packet+96,n,expected,sequence,&info);p->facts.peek_validation=raw;
 if(raw){raw=cancel_borrow(source,p,sequence,reservation,raw);leave(s);return raw;}
 // All finite source/header/metadata/body bounds and output shapes are now
 // prepared. Do not expose any canonical/length/info result before actual ACK.
 p->facts.commit_attempted=1;p->facts.commit_succeeded=0;++p->facts.commits;
 raw=uwvm_ros2_rmw_sub_v1_subscription_commit(source->handle,p->admitted.native_incarnation,sequence,reservation);p->facts.commit_code=raw;
 if(raw){(void)terminal(source,p,raw);leave(s);return raw;} // NO retry/cancel-as-rollback
 p->facts.commit_succeeded=1;p->facts.last_committed_sequence=sequence;p->facts.last_committed_reservation=reservation;
 if(n)memcpy(canonical,s->packet+PACKET_OVERHEAD,n);*length=n;*information=info;leave(s);return 0;
#endif
}
uint32_t uwvm_rmw_subscription_transport_create(size_t slots,size_t maximum,rcutils_allocator_t const*a,uwvm_rmw_subscription_transport**out){
 if(!out||!a||!rcutils_allocator_is_valid(a)||!slots||slots>UWVM_RMW_SUBSCRIPTION_TRANSPORT_MAX_SLOTS||maximum>UWVM_RMW_MAX_CANONICAL_BYTES||
 maximum>IPC_MAX-PACKET_OVERHEAD||slots>(SIZE_MAX-sizeof(uwvm_rmw_subscription_transport))/sizeof(subscription_slot))return 22;
 size_t bytes=sizeof(uwvm_rmw_subscription_transport)+slots*sizeof(subscription_slot);
 uwvm_rmw_subscription_transport*s=a->zero_allocate(1,bytes,a->state);if(!s)return 12;
 s->packet=a->allocate(PACKET_OVERHEAD+maximum,a->state);if(!s->packet){a->deallocate(s,a->state);return 12;}
 s->allocator=*a;s->capacity=slots;s->maximum_body=maximum;s->packet_capacity=PACKET_OVERHEAD+maximum;s->magic=STATE_MAGIC;atomic_init(&s->busy,false);*out=s;return 0;
}
uint32_t uwvm_rmw_subscription_transport_callbacks(uwvm_rmw_subscription_transport*s,uwvm_rmw_c_activation_provider*a,uwvm_rmw_c_native_metadata_provider*m){
#if !UWVM_RMW_SUBSCRIPTION_MESSAGE_INFO_API
 (void)s;(void)a;(void)m;return 95;
#else
 if(RMW_GID_STORAGE_SIZE==0||RMW_GID_STORAGE_SIZE>32)return 95;
 uint32_t raw=enter(s);if(raw)return raw;if(!a||!m){leave(s);return 22;}
 uwvm_rmw_c_activation_provider activation={1,s,activate_cb,inspect_cb,retire_cb};uwvm_rmw_c_native_metadata_provider metadata={1,s,take_cb,NULL,NULL};
 *a=activation;*m=metadata;s->exported=1;leave(s);return 0;
#endif
}
uint32_t uwvm_rmw_subscription_transport_count(uwvm_rmw_subscription_transport*s,size_t*out){uint32_t raw=enter(s);if(raw)return raw;if(!out){leave(s);return 22;}*out=s->count;leave(s);return 0;}
uint32_t uwvm_rmw_subscription_transport_diagnostics(uwvm_rmw_subscription_transport*s,size_t i,uwvm_rmw_subscription_transport_diagnostic*out){
 uint32_t raw=enter(s);if(raw)return raw;if(!out||i>=s->count){leave(s);return 22;}*out=s->slots[i].facts;leave(s);return 0;
}
uint32_t uwvm_rmw_subscription_transport_record_raw_close(uwvm_rmw_subscription_transport*s,size_t i,uint32_t code){
 uint32_t raw=enter(s);if(raw)return raw;if(i>=s->count||s->slots[i].facts.raw_close_reported){leave(s);return 22;}
 s->slots[i].facts.raw_close_reported=1;s->slots[i].facts.raw_close_code=code;leave(s);return 0;
}
uint32_t uwvm_rmw_subscription_transport_dispose_unpublished(uwvm_rmw_subscription_transport**state){
 if(!state||!*state)return 22;uwvm_rmw_subscription_transport*s=*state;uint32_t raw=enter(s);if(raw)return raw;
 if(s->exported||s->count){leave(s);return 17;}rcutils_allocator_t a=s->allocator;void*packet=s->packet;s->magic=0;*state=NULL;a.deallocate(packet,a.state);a.deallocate(s,a.state);return 0;
}
