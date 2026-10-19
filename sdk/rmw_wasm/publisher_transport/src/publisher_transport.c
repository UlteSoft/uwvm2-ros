/* SPDX-License-Identifier: Apache-2.0
 * Complete UMRE-v1 parser plus the five actual optional raw imports. This file
 * never invokes the old publish import, opens/closes an alias, manufactures a
 * native identity or retries a potentially effectful operation. B owns source
 * capabilities; the Host owns actual native schema/type/QoS admission.
 */
#include <uwvm_rmw/publisher_transport.h>
#include <roswasm/rmw_publisher_v1.h>
#include <rmw/time.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
_Static_assert(UWVM_ROS_RMW_ENDPOINT_RECEIPT_BYTES==256,"UMRE-v1 has exactly256 bytes");
_Static_assert(RMW_GID_STORAGE_SIZE>0&&RMW_GID_STORAGE_SIZE<=32,"selected original RMW GID extent must fit the complete UMRE GID field");
#define TRANSPORT_MAGIC UINT32_C(0x54505755)
typedef struct transport_slot {
  roswasmc_capability const * owner; /* Stable B owner; compare only, never free. */
  roswasmc_context const * context;
  size_t canonical_limit;
  uwvm_rmw_publisher_receipt admitted;
  uwvm_rmw_publisher_transport_diagnostic diagnostic;
} transport_slot;
struct uwvm_rmw_publisher_transport {
  uint32_t magic;
  atomic_bool busy;
  rcutils_allocator_t allocator;
  size_t capacity,count;
  transport_slot slots[];
};
/* This lock prevents recursive use of fixed scratch/diagnostics; it does not
 * grant cross-call/upper-callback leases or claim multithread RMW semantics.
 * Bootstrap/destruction require exclusive caller ownership. Raw imports do not
 * invoke application code. B still owns its admitted RMW function-scope lease.
 */
static uint32_t enter(uwvm_rmw_publisher_transport * state) {
  if(!state||state->magic!=TRANSPORT_MAGIC)return 22;
  return atomic_exchange_explicit(&state->busy,true,memory_order_acquire)?17:0;
}
static void leave(uwvm_rmw_publisher_transport * state) {
  atomic_store_explicit(&state->busy,false,memory_order_release);
}
static uint64_t le(uint8_t const * p,size_t n) {
  uint64_t value=0;for(size_t i=0;i<n;++i)value|=(uint64_t)p[i]<<(8*i);return value;
}
static int zeros(uint8_t const * p,size_t n) {
  for(size_t i=0;i<n;++i)if(p[i])return 0;return 1;
}
static int full_source(uint64_t h) {return (uint32_t)h&&(uint32_t)(h>>32);}
static int source_valid(roswasmc_capability const * cap) {
  return cap&&cap->context&&cap->context->backend&&cap->context->backend->version==2&&full_source(cap->handle);
}
/* These are explicit wire enum mappings, never memcpy native enum ordinals. */
static uint32_t decode_qos(uint8_t const * p,rmw_qos_profile_t * output) {
  uint64_t h=le(p,4),r=le(p+4,4),d=le(p+8,4),l=le(p+12,4),depth=le(p+16,8);
  if(h<1||h>2||r<1||r>2||d<1||d>2||l<1||l>2||
    (h==1&&!depth)||depth>SIZE_MAX||p[72]>1||!zeros(p+73,7)||
    le(p+32,8)>=UINT64_C(1000000000)||le(p+48,8)>=UINT64_C(1000000000)||
    le(p+64,8)>=UINT64_C(1000000000))return 22;
  rmw_qos_profile_t q={0};
  q.history=h==1?RMW_QOS_POLICY_HISTORY_KEEP_LAST:RMW_QOS_POLICY_HISTORY_KEEP_ALL;
  q.reliability=r==1?RMW_QOS_POLICY_RELIABILITY_RELIABLE:RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
  q.durability=d==1?RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL:RMW_QOS_POLICY_DURABILITY_VOLATILE;
  q.liveliness=l==1?RMW_QOS_POLICY_LIVELINESS_AUTOMATIC:RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
  q.depth=(size_t)depth;q.deadline.sec=le(p+24,8);q.deadline.nsec=le(p+32,8);
  q.lifespan.sec=le(p+40,8);q.lifespan.nsec=le(p+48,8);
  q.liveliness_lease_duration.sec=le(p+56,8);q.liveliness_lease_duration.nsec=le(p+64,8);
  q.avoid_ros_namespace_conventions=p[72]!=0;*output=q;return 0;
}
static int qos_same(rmw_qos_profile_t const * a,rmw_qos_profile_t const * b) {
  return a->history==b->history&&a->depth==b->depth&&a->reliability==b->reliability&&
    a->durability==b->durability&&a->liveliness==b->liveliness&&
    rmw_time_equal(a->deadline,b->deadline)&&rmw_time_equal(a->lifespan,b->lifespan)&&
    rmw_time_equal(a->liveliness_lease_duration,b->liveliness_lease_duration)&&
    a->avoid_ros_namespace_conventions==b->avoid_ros_namespace_conventions;
}
static int binding_same(uwvm_rmw_publisher_receipt const * a,uwvm_rmw_publisher_receipt const * b) {
  return a->version==b->version&&a->backend==b->backend&&a->source==b->source&&
    a->instance==b->instance&&a->runtime_generation==b->runtime_generation&&
    a->session==b->session&&a->native_incarnation==b->native_incarnation;
}
static int public_same(uwvm_rmw_publisher_receipt const * a,uwvm_rmw_publisher_receipt const * b) {
  return binding_same(a,b)&&qos_same(&a->actual_qos,&b->actual_qos)&&
    a->native_gid_bytes==b->native_gid_bytes&&!memcmp(a->native_gid,b->native_gid,RMW_GID_STORAGE_SIZE);
}
/* Success commits only a fully parsed, associated publisher receipt. A native
 * GID-width mismatch is NOT malformed: preserve the binding and actual extent,
 * zero the incompatible public array, and let activation return95 only after B
 * can retain this receipt for one native-retire attempt. Complete actual bytes
 * remain in the preallocated diagnostic.original_wire, never truncated/padded.
 */
static uint32_t decode(uint8_t const * b,uint8_t operation,uint64_t source,
  uwvm_rmw_publisher_receipt * output) {
  if(le(b,4)!=UINT32_C(0x45524d55)||le(b+4,2)!=1||b[6]!=operation||b[7]!=2||
    le(b+8,4)!=256||b[12]!=1||b[13]!=1||!b[14]||b[14]>32||b[15]||
    !zeros(b+56,8)||!zeros(b+144,80)||!zeros(b+224+b[14],32-b[14]))return 22;
  uwvm_rmw_publisher_receipt next={0};next.version=1;next.backend=2;
  next.source=le(b+16,8);next.instance=le(b+24,8);next.runtime_generation=le(b+32,8);
  next.session=le(b+40,8);next.native_incarnation=le(b+48,8);
  if(!full_source(next.source)||next.source!=source||!next.instance||!next.runtime_generation||
    !next.session||!next.native_incarnation)return 22;
  uint32_t raw=decode_qos(b+64,&next.actual_qos);if(raw)return raw;
  next.native_gid_bytes=b[14];
  if(next.native_gid_bytes==RMW_GID_STORAGE_SIZE)memcpy(next.native_gid,b+224,RMW_GID_STORAGE_SIZE);
  *output=next;return 0;
}
static void primary(transport_slot * slot,uint32_t code) {
  if(code&&!slot->diagnostic.first_primary)slot->diagnostic.first_primary=code;
}
static transport_slot * find(uwvm_rmw_publisher_transport * s,roswasmc_capability const * cap,
  uwvm_rmw_publisher_receipt const * expected) {
  if(!source_valid(cap)||!expected)return NULL;
  for(size_t i=0;i<s->count;++i) {
    transport_slot * p=s->slots+i;
    if(p->diagnostic.associated&&p->owner==cap&&p->context==cap->context&&
      p->diagnostic.source==cap->handle&&public_same(expected,&p->admitted))return p;
  }
  return NULL;
}
/* Internal with state already held: exactly one current native query; whole
 * admitted wire comparison ignores only the command byte(activate1/inspect2).
 * It never refreshes identity/GID/QoS after drift. All native responses are DATA
 * until the full parse and same-original-snapshot association have succeeded.
 */
static uint32_t inspect_slot(transport_slot * p,roswasmc_capability const * source,
  uwvm_rmw_publisher_receipt * output) {
  if(p->diagnostic.retire_attempted)return 125;
  /* A cleanup-only activation never becomes a business endpoint, even when
   * a direct caller still holds its associated receipt. Retire remains allowed. */
  if(p->diagnostic.activation_validation)return p->diagnostic.activation_validation;
  if(p->diagnostic.inspections==UINT64_MAX)return 90;
  uint8_t wire[256];memset(wire,0xa5,sizeof wire);++p->diagnostic.inspections;
  uint32_t raw=uwvm_ros2_rmw_v1_publisher_inspect(source->handle,p->admitted.native_incarnation,0,(uint64_t)(uintptr_t)wire);
  p->diagnostic.inspect_code=raw;p->diagnostic.inspect_validation=0;
  if(raw){primary(p,raw);return raw;}
  memcpy(p->diagnostic.last_wire,wire,256);p->diagnostic.last_wire_present=1;
  uwvm_rmw_publisher_receipt next={0};raw=decode(wire,2,source->handle,&next);
  if(!raw&&(!binding_same(&next,&p->admitted)||memcmp(wire,p->diagnostic.original_wire,6)||
    memcmp(wire+7,p->diagnostic.original_wire+7,249)))raw=13;
  if(!raw&&!p->diagnostic.gid_compatible)raw=95;
  p->diagnostic.inspect_validation=raw;if(raw){primary(p,raw);return raw;}
  *output=next;return 0;
}
static uint32_t activate_cb(void * state,roswasmc_capability const * source,
  uwvm_rmw_endpoint_selection const * row,uwvm_rmw_codec const * codec,
  rmw_publisher_options_t const * options,uwvm_rmw_publisher_receipt * output) {
  uwvm_rmw_publisher_transport * s=state;uint32_t raw=enter(s);if(raw)return raw;
  /* B selects/validates the complete native codec graph before opening. Repeat
   * that actual descriptor callback here; do not infer native type identity
   * from UMRE, which deliberately carries no native schema hash or type name.
   * Unsupported options and malformed arguments are rejected before raw calls.
   */
  if(!source_valid(source)||!row||!codec||!options||!output||row->kind!=UWVM_RMW_PUBLISHER||
    row->alias_count!=1||!row->aliases[0]||!row->aliases[0][0]||codec->version!=1||
    codec->kind!=UWVM_RMW_MESSAGE_CODEC||!codec->validate||!codec->encode||!codec->decode||
    !codec->generated_support||codec->request||codec->response||
    codec->maximum_wire_bytes>UWVM_RMW_MAX_CANONICAL_BYTES) {leave(s);return 22;}
  if(options->rmw_specific_publisher_payload||
    options->require_unique_network_flow_endpoints!=RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED) {leave(s);return 95;}
  raw=codec->validate(codec,codec->generated_support);if(raw){leave(s);return raw;}
  for(size_t i=0;i<s->count;++i)if(s->slots[i].diagnostic.source==source->handle) {leave(s);return 13;}
  if(s->count==s->capacity) {leave(s);return 90;}
  transport_slot * p=s->slots+s->count++;p->owner=source;p->context=source->context;
  p->canonical_limit=codec->maximum_wire_bytes;p->diagnostic.source=source->handle;
  /* Slot is permanent before even a version query. A failed activation cannot
   * be reopened automatically through this provider with the same full handle.
   */
  p->diagnostic.abi_version=uwvm_ros2_rmw_v1_abi_version();
  if(p->diagnostic.abi_version!=1) {primary(p,95);leave(s);return 95;}
  uint8_t wire[256];memset(wire,0xa5,sizeof wire);p->diagnostic.activation_attempted=1;
  raw=uwvm_ros2_rmw_v1_publisher_activate(source->handle,0,(uint64_t)(uintptr_t)wire);
  p->diagnostic.activation_code=raw;
  if(raw) {primary(p,raw);leave(s);return raw;}
  memcpy(p->diagnostic.original_wire,wire,256);p->diagnostic.original_wire_present=1;
  uwvm_rmw_publisher_receipt next={0};raw=decode(wire,1,source->handle,&next);
  p->diagnostic.activation_validation=raw;
  if(raw) {primary(p,raw);leave(s);return raw;}
  p->admitted=next;p->diagnostic.associated=1;
  p->diagnostic.gid_compatible=next.native_gid_bytes==RMW_GID_STORAGE_SIZE;
  /* Return a cleanup-only receipt even on these associated failures. B stores
   * it before handling error and installs its once-only native-retire callback.
   */
  if(!p->diagnostic.gid_compatible)raw=95;
  else if(!qos_same(&next.actual_qos,&row->effective_qos))raw=22;
  p->diagnostic.activation_validation=raw;*output=next;primary(p,raw);leave(s);return raw;
}
static uint32_t inspect_cb(void * state,roswasmc_capability const * source,
  uwvm_rmw_publisher_receipt const * expected,uwvm_rmw_publisher_receipt * output) {
  uwvm_rmw_publisher_transport * s=state;uint32_t raw=enter(s);if(raw)return raw;
  if(!output) {leave(s);return 22;}
  transport_slot * p=find(s,source,expected);if(!p) {leave(s);return 13;}
  uwvm_rmw_publisher_receipt next={0};raw=inspect_slot(p,source,&next);
  if(!raw)*output=next;leave(s);return raw;
}
static uint32_t publish_cb(void * state,roswasmc_capability const * source,
  uwvm_rmw_publisher_receipt const * expected,void const * canonical,size_t bytes) {
  uwvm_rmw_publisher_transport * s=state;uint32_t raw=enter(s);if(raw)return raw;
  transport_slot * p=find(s,source,expected);if(!p) {leave(s);return 13;}
  if((bytes&&!canonical)||bytes>p->canonical_limit) {leave(s);return bytes>p->canonical_limit?90:22;}
  if(p->diagnostic.publications==UINT64_MAX) {leave(s);return 90;}
  uwvm_rmw_publisher_receipt current={0};raw=inspect_slot(p,source,&current);
  if(raw) {leave(s);return raw;}
  ++p->diagnostic.publications;
  /* Logical Empty is exactly0, never the private one-byte scratch allocation.
   * The actual codec owned by B validated/encoded the complete native message.
   */
  raw=uwvm_ros2_rmw_v1_publisher_publish(source->handle,p->admitted.native_incarnation,0,(uint64_t)(uintptr_t)canonical,(uint64_t)bytes);
  p->diagnostic.publish_code=raw;primary(p,raw);leave(s);return raw;
}
static uint32_t retire_cb(void * state,roswasmc_capability const * source,
  uwvm_rmw_publisher_receipt const * expected) {
  uwvm_rmw_publisher_transport * s=state;uint32_t raw=enter(s);if(raw)return raw;
  transport_slot * p=find(s,source,expected);if(!p) {leave(s);return 13;}
  if(p->diagnostic.retire_attempted) {leave(s);return 125;}
  /* Mark BEFORE the import. Neither an error nor later local destruction can
   * replay it. Binding-only width-mismatch receipts intentionally reach here.
   */
  p->diagnostic.retire_attempted=1;
  raw=uwvm_ros2_rmw_v1_publisher_retire(source->handle,p->admitted.native_incarnation);
  p->diagnostic.retire_code=raw;leave(s);return raw;
}
uint32_t uwvm_rmw_publisher_transport_create(size_t capacity,
  rcutils_allocator_t const * allocator,uwvm_rmw_publisher_transport ** output) {
  if(!output||!allocator||!rcutils_allocator_is_valid(allocator)||!capacity||
    capacity>UWVM_RMW_PUBLISHER_TRANSPORT_MAX_SLOTS||
    capacity>(SIZE_MAX-sizeof(uwvm_rmw_publisher_transport))/sizeof(transport_slot))return 22;
  size_t bytes=sizeof(uwvm_rmw_publisher_transport)+capacity*sizeof(transport_slot);
  uwvm_rmw_publisher_transport * s=allocator->zero_allocate(1,bytes,allocator->state);
  if(!s)return 12;
  s->allocator=*allocator;s->capacity=capacity;s->magic=TRANSPORT_MAGIC;atomic_init(&s->busy,false);
  *output=s;return 0;
}
uint32_t uwvm_rmw_publisher_transport_callbacks(uwvm_rmw_publisher_transport * s,
  uwvm_rmw_publisher_provider * output) {
  uint32_t raw=enter(s);if(raw)return raw;if(!output) {leave(s);return 22;}
  uwvm_rmw_publisher_provider next={1,s,activate_cb,inspect_cb,publish_cb,retire_cb};
  *output=next;leave(s);return 0;
}
uint32_t uwvm_rmw_publisher_transport_count(uwvm_rmw_publisher_transport * s,size_t * output) {
  uint32_t raw=enter(s);if(raw)return raw;if(!output) {leave(s);return 22;}
  *output=s->count;leave(s);return 0;
}
uint32_t uwvm_rmw_publisher_transport_diagnostics(uwvm_rmw_publisher_transport * s,
  size_t index,uwvm_rmw_publisher_transport_diagnostic * output) {
  uint32_t raw=enter(s);if(raw)return raw;
  if(!output||index>=s->count) {leave(s);return 22;}
  *output=s->slots[index].diagnostic;leave(s);return 0;
}
uint32_t uwvm_rmw_publisher_transport_destroy(uwvm_rmw_publisher_transport ** state) {
  if(!state||!*state)return 22;
  uwvm_rmw_publisher_transport * s=*state;uint32_t raw=enter(s);if(raw)return raw;
  for(size_t i=0;i<s->count;++i)if(s->slots[i].diagnostic.associated&&!s->slots[i].diagnostic.retire_attempted) {leave(s);return 17;}
  /* No cached source/context pointer is dereferenced here: B may already have
   * finalized its storage. Caller has uninstalled/finalized ALL borrowers first.
   * This releases only local diagnostics, not a native endpoint or raw handle.
   */
  rcutils_allocator_t a=s->allocator;s->magic=0;*state=NULL;a.deallocate(s,a.state);return 0;
}
