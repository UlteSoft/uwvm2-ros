/* SPDX-License-Identifier: Apache-2.0
 * Always-active LOCAL protocol/ownership models over the ORIGINAL installed
 * RMW C types. Strong definitions below replace only the five raw imports in
 * this executable. They are not linked into the production transport library,
 * are not a native DDS endpoint and supply no native type/hash/GID witness.
 * The tiny descriptor below deliberately cannot be installed as a B profile:
 * it has no claimed native RIHS. It exercises provider argument delegation only.
 */
#if defined(__wasm__)
#error "Local strong-definition raw models require a native SDK build; production Wasm imports remain real"
#endif
#if defined(NDEBUG)
#error "Do not compile source contracts with NDEBUG"
#endif
#include <uwvm_rmw/publisher_transport.h>
#include <roswasm/rmw_publisher_v1.h>
#include <rmw/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define CHECK(x) do {if(!(x)){fprintf(stderr,"publisher transport contract line%u: %s\n",(unsigned)__LINE__,#x);exit(1);}} while(0)
static uint8_t activation[256],inspection[256];
static uint32_t version=1,activate_error,inspect_error,publish_error,retire_error;
static size_t version_calls,activate_calls,inspect_calls,publish_calls,retire_calls;
static uint64_t expected_source,expected_incarnation;
static size_t last_bytes;
static uint8_t last_body[8];
static int reenter;
static uint32_t reenter_code;
static uwvm_rmw_publisher_transport * reenter_state;
static void put(uint8_t * p,uint64_t value,size_t n) {for(size_t i=0;i<n;++i)p[i]=(uint8_t)(value>>(8*i));}
uint32_t uwvm_ros2_rmw_v1_abi_version(void) {++version_calls;return version;}
uint32_t uwvm_ros2_rmw_v1_publisher_activate(uint64_t source,uint32_t memory,uint64_t output) {
  CHECK(source==expected_source&&memory==0&&output);++activate_calls;
  if(!activate_error)memcpy((void *)(uintptr_t)output,activation,256);return activate_error;
}
uint32_t uwvm_ros2_rmw_v1_publisher_inspect(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t output) {
  CHECK(source==expected_source&&incarnation==expected_incarnation&&memory==0&&output);++inspect_calls;
  if(!inspect_error)memcpy((void *)(uintptr_t)output,inspection,256);return inspect_error;
}
uint32_t uwvm_ros2_rmw_v1_publisher_publish(uint64_t source,uint64_t incarnation,uint32_t memory,uint64_t bytes,uint64_t n) {
  CHECK(source==expected_source&&incarnation==expected_incarnation&&memory==0);++publish_calls;
  CHECK(n<=sizeof last_body);last_bytes=(size_t)n;if(n){CHECK(bytes);memcpy(last_body,(void const *)(uintptr_t)bytes,(size_t)n);}return publish_error;
}
uint32_t uwvm_ros2_rmw_v1_publisher_retire(uint64_t source,uint64_t incarnation) {
  CHECK(source==expected_source&&incarnation==expected_incarnation);++retire_calls;return retire_error;
}
static unsigned char model_support_marker;
static uint32_t model_validate(uwvm_rmw_codec const * c,void const * supplied) {
  if(reenter){size_t n=999;reenter_code=uwvm_rmw_publisher_transport_count(reenter_state,&n);CHECK(n==999);}
  return c&&supplied==&model_support_marker?0:22;
}
static uint32_t model_encode(void const * n,void * b,size_t c,size_t * l) {(void)n;(void)b;(void)c;(void)l;return 95;}
static uint32_t model_decode(void const * b,size_t l,void * n) {(void)b;(void)l;(void)n;return 95;}
typedef struct fixture {
  uwvm_rmw_publisher_transport * state;
  uwvm_rmw_publisher_provider api;
  roswasmc_backend backend;
  roswasmc_context context;
  roswasmc_capability source;
  uwvm_rmw_endpoint_selection row;
  uwvm_rmw_codec codec;
  rmw_publisher_options_t options;
  uwvm_rmw_publisher_receipt receipt;
} fixture;
static rmw_qos_profile_t actual_qos(void) {
  rmw_qos_profile_t q={0};q.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;q.depth=7;
  q.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;q.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
  q.liveliness=RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
  q.deadline=(rmw_time_t){1,2};q.lifespan=(rmw_time_t){3,4};q.liveliness_lease_duration=(rmw_time_t){5,6};
  q.avoid_ros_namespace_conventions=true;return q;
}
static void build_wire(uint8_t * p,uint8_t operation) {
  memset(p,0,256);put(p,UINT32_C(0x45524d55),4);put(p+4,1,2);p[6]=operation;p[7]=2;put(p+8,256,4);
  p[12]=1;p[13]=1;p[14]=RMW_GID_STORAGE_SIZE;put(p+16,expected_source,8);put(p+24,2,8);
  put(p+32,3,8);put(p+40,4,8);put(p+48,expected_incarnation,8);
  put(p+64,1,4);put(p+68,1,4);put(p+72,2,4);put(p+76,2,4);put(p+80,7,8);
  put(p+88,1,8);put(p+96,2,8);put(p+104,3,8);put(p+112,4,8);put(p+120,5,8);put(p+128,6,8);p[136]=1;
  /* All-zero complete GID is an intentionally VALID value, not absence. */
}
static void init(fixture * f,size_t capacity) {
  memset(f,0,sizeof(*f));version=1;activate_error=inspect_error=publish_error=retire_error=0;
  version_calls=activate_calls=inspect_calls=publish_calls=retire_calls=0;last_bytes=999;reenter=0;reenter_code=0;
  expected_source=UINT64_C(0x1234567800000001);expected_incarnation=6;
  build_wire(activation,1);build_wire(inspection,2);
  f->backend.version=2;f->context.backend=&f->backend;f->source.context=&f->context;f->source.handle=expected_source;
  f->row.kind=UWVM_RMW_PUBLISHER;f->row.alias_count=1;f->row.aliases[0]="selected-publisher";
  f->row.requested_qos=f->row.effective_qos=actual_qos();
  f->codec.version=1;f->codec.kind=UWVM_RMW_MESSAGE_CODEC;f->codec.generated_support=&model_support_marker;
  f->codec.maximum_wire_bytes=8;f->codec.validate=model_validate;f->codec.encode=model_encode;f->codec.decode=model_decode;
  f->options.require_unique_network_flow_endpoints=RMW_UNIQUE_NETWORK_FLOW_ENDPOINTS_NOT_REQUIRED;
  rcutils_allocator_t a=rcutils_get_default_allocator();CHECK(uwvm_rmw_publisher_transport_create(capacity,&a,&f->state)==0);
  CHECK(uwvm_rmw_publisher_transport_callbacks(f->state,&f->api)==0);CHECK(!version_calls&&!activate_calls);
}
static uint32_t activate(fixture * f) {return f->api.activate(f->api.state,&f->source,&f->row,&f->codec,&f->options,&f->receipt);}
static uwvm_rmw_publisher_transport_diagnostic diagnostic(fixture * f,size_t index) {
  uwvm_rmw_publisher_transport_diagnostic d={0};CHECK(uwvm_rmw_publisher_transport_diagnostics(f->state,index,&d)==0);return d;
}
static void finish(fixture * f) {
  size_t n=0;CHECK(uwvm_rmw_publisher_transport_count(f->state,&n)==0);
  if(n&&f->receipt.native_incarnation) {uwvm_rmw_publisher_transport_diagnostic d=diagnostic(f,0);
    if(d.associated&&!d.retire_attempted)(void)f->api.retire(f->api.state,&f->source,&f->receipt);}
  CHECK(uwvm_rmw_publisher_transport_destroy(&f->state)==0);CHECK(!f->state);
}
static void * fail_zero_allocate(size_t n,size_t bytes,void * state) {(void)n;(void)bytes;(void)state;return NULL;}
static void create_capacity_and_allocator_preserve_outputs(void) {
  rcutils_allocator_t a=rcutils_get_default_allocator();uwvm_rmw_publisher_transport * output=(void *)(uintptr_t)1;
  CHECK(uwvm_rmw_publisher_transport_create(0,&a,&output)==22&&output==(void *)(uintptr_t)1);
  CHECK(uwvm_rmw_publisher_transport_create(UWVM_RMW_PUBLISHER_TRANSPORT_MAX_SLOTS+1,&a,&output)==22);
  rcutils_allocator_t broken={0};CHECK(uwvm_rmw_publisher_transport_create(1,&broken,&output)==22&&output==(void *)(uintptr_t)1);
  a.zero_allocate=fail_zero_allocate;CHECK(uwvm_rmw_publisher_transport_create(1,&a,&output)==12&&output==(void *)(uintptr_t)1);
}
static void complete_zero_gid_and_all_qos_fields(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);CHECK(f.receipt.version==1&&f.receipt.backend==2);
  CHECK(f.receipt.source==expected_source&&f.receipt.instance==2&&f.receipt.runtime_generation==3&&f.receipt.session==4&&f.receipt.native_incarnation==6);
  CHECK(f.receipt.native_gid_bytes==RMW_GID_STORAGE_SIZE);for(size_t i=0;i<RMW_GID_STORAGE_SIZE;++i)CHECK(f.receipt.native_gid[i]==0);
  rmw_qos_profile_t q=f.receipt.actual_qos;CHECK(q.history==RMW_QOS_POLICY_HISTORY_KEEP_LAST&&q.depth==7);
  CHECK(q.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&q.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE);
  CHECK(q.liveliness==RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC&&q.avoid_ros_namespace_conventions);
  CHECK(q.deadline.sec==1&&q.deadline.nsec==2&&q.lifespan.sec==3&&q.lifespan.nsec==4&&q.liveliness_lease_duration.sec==5&&q.liveliness_lease_duration.nsec==6);
  CHECK(diagnostic(&f,0).gid_compatible);finish(&f);
}
static void native_enum_mapping_is_explicit(void) {
  fixture f;init(&f,1);put(activation+64,2,4);put(activation+68,2,4);put(activation+72,1,4);put(activation+76,1,4);activation[136]=0;
  f.row.effective_qos.history=RMW_QOS_POLICY_HISTORY_KEEP_ALL;f.row.effective_qos.reliability=RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
  f.row.effective_qos.durability=RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;f.row.effective_qos.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;f.row.effective_qos.avoid_ros_namespace_conventions=false;
  CHECK(activate(&f)==0);CHECK(f.receipt.actual_qos.history==RMW_QOS_POLICY_HISTORY_KEEP_ALL);finish(&f);
}
static void complete_wire_envelope_and_reserved_refusal(void) {
  size_t const positions[]={0,4,6,7,8,12,13,14,15,56,143,144,RMW_GID_STORAGE_SIZE<32?255:15};
  for(size_t i=0;i<sizeof positions/sizeof positions[0];++i){fixture f;init(&f,1);activation[positions[i]]=255;
    memset(&f.receipt,0x55,sizeof f.receipt);uwvm_rmw_publisher_receipt before=f.receipt;
    CHECK(activate(&f)==22);CHECK(!memcmp(&before,&f.receipt,sizeof before));
    f.receipt=(uwvm_rmw_publisher_receipt){0};CHECK(!diagnostic(&f,0).associated);CHECK(diagnostic(&f,0).original_wire_present);finish(&f);}
}
static void every_binding_member_is_mandatory(void) {
  size_t const fields[]={16,24,32,40,48};
  for(size_t i=0;i<sizeof fields/sizeof fields[0];++i){fixture f;init(&f,1);put(activation+fields[i],0,8);CHECK(activate(&f)==22&&!f.receipt.native_incarnation);finish(&f);}
  fixture f;init(&f,1);put(activation+16,expected_source+1,8);CHECK(activate(&f)==22);finish(&f);
}
static void malformed_qos_is_not_declaration_attestation(void) {
  size_t const fields[]={64,68,72,76};for(size_t i=0;i<4;++i){fixture f;init(&f,1);put(activation+fields[i],0,4);CHECK(activate(&f)==22);finish(&f);}
  size_t const durations[]={96,112,128};for(size_t i=0;i<3;++i){fixture f;init(&f,1);put(activation+durations[i],1000000000,8);CHECK(activate(&f)==22);finish(&f);}
  fixture f;init(&f,1);put(activation+80,0,8);CHECK(activate(&f)==22);finish(&f);
}
static void incompatible_gid_retains_complete_cleanup_binding(void) {
  fixture f;init(&f,1);uint8_t width=RMW_GID_STORAGE_SIZE==24?16:24;activation[14]=width;
  for(size_t i=0;i<width;++i)activation[224+i]=(uint8_t)(i+1);
  CHECK(activate(&f)==95);CHECK(f.receipt.native_gid_bytes==width&&f.receipt.source==expected_source&&f.receipt.native_incarnation==6);
  for(size_t i=0;i<RMW_GID_STORAGE_SIZE;++i)CHECK(f.receipt.native_gid[i]==0);
  uwvm_rmw_publisher_transport_diagnostic d=diagnostic(&f,0);CHECK(d.associated&&!d.gid_compatible&&!memcmp(d.original_wire,activation,256));
  uwvm_rmw_publisher_receipt out={0};CHECK(f.api.inspect(f.api.state,&f.source,&f.receipt,&out)==95&&!inspect_calls);
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)==95&&!publish_calls);
  CHECK(f.api.retire(f.api.state,&f.source,&f.receipt)==0&&retire_calls==1);finish(&f);
}
static void actual_qos_mismatch_stays_cleanup_only(void) {
  fixture f;init(&f,1);f.row.effective_qos.depth=8;CHECK(activate(&f)==22&&f.receipt.native_incarnation==6);
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)==22&&!inspect_calls&&!publish_calls);
  finish(&f);CHECK(retire_calls==1);
}
static void equivalent_requested_durations_use_real_rmw_time(void) {
  fixture f;init(&f,1);put(activation+88,2,8);put(activation+96,2,8);
  f.row.effective_qos.deadline=(rmw_time_t){1,1000000002};CHECK(activate(&f)==0);
  CHECK(rmw_time_equal(f.row.effective_qos.deadline,f.receipt.actual_qos.deadline));finish(&f);
}
static void version_absence_has_no_native_activation_and_no_replay(void) {
  fixture f;init(&f,1);version=0;CHECK(activate(&f)==95&&version_calls==1&&!activate_calls);
  version=1;CHECK(activate(&f)==13&&version_calls==1&&!activate_calls);finish(&f);
}
static void activation_error_has_no_fabricated_partial_receipt(void) {
  fixture f;init(&f,1);activate_error=110;CHECK(activate(&f)==110&&!f.receipt.native_incarnation);
  CHECK(activate(&f)==13&&activate_calls==1);uwvm_rmw_publisher_transport_diagnostic d=diagnostic(&f,0);
  CHECK(d.activation_attempted&&d.first_primary==110&&!d.original_wire_present&&!d.associated);finish(&f);
}
static void source_and_capacity_slots_never_reuse(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);CHECK(f.api.retire(f.api.state,&f.source,&f.receipt)==0);
  CHECK(activate(&f)==13&&activate_calls==1);f.source.handle=++expected_source;
  CHECK(activate(&f)==90&&activate_calls==1);f.source.handle=--expected_source;finish(&f);
}
static void inspect_whole_snapshot_preserves_original_and_output(void) {
  size_t const fields[]={16,24,32,40,48,80,88,104,120,136,224};
  for(size_t i=0;i<sizeof fields/sizeof fields[0];++i){fixture f;init(&f,1);CHECK(activate(&f)==0);inspection[fields[i]]^=1;
    uwvm_rmw_publisher_receipt out;memset(&out,0x33,sizeof out);uwvm_rmw_publisher_receipt before=out;
    CHECK(f.api.inspect(f.api.state,&f.source,&f.receipt,&out)!=0&&!memcmp(&out,&before,sizeof out));
    uwvm_rmw_publisher_transport_diagnostic d=diagnostic(&f,0);CHECK(d.last_wire_present&&!memcmp(d.original_wire,activation,256)&&!memcmp(d.last_wire,inspection,256));
    CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)!=0&&!publish_calls);finish(&f);}
}
static void copied_owner_and_changed_public_receipt_are_not_authority(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);uwvm_rmw_publisher_receipt out={0};roswasmc_capability copied=f.source;
  CHECK(f.api.inspect(f.api.state,&copied,&f.receipt,&out)==13&&!inspect_calls);
  uwvm_rmw_publisher_receipt changed=f.receipt;changed.native_incarnation++;CHECK(f.api.retire(f.api.state,&f.source,&changed)==13&&!retire_calls);
  changed=f.receipt;changed.native_gid[0]=1;CHECK(f.api.inspect(f.api.state,&f.source,&changed,&out)==13&&!inspect_calls);finish(&f);
}
static void publish_uses_new_import_once_and_full_payload_bound(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);uint8_t body[8]={1,2,3,4,5,6,7,8};
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,body,8)==0&&publish_calls==1&&inspect_calls==1&&last_bytes==8&&!memcmp(last_body,body,8));
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,body,9)==90&&publish_calls==1&&inspect_calls==1);
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,1)==22&&publish_calls==1);finish(&f);
}
static void logical_empty_is_exactly_zero(void) {
  fixture f;init(&f,1);f.codec.maximum_wire_bytes=0;CHECK(activate(&f)==0);
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)==0&&publish_calls==1&&last_bytes==0);finish(&f);
}
static void publish_timeout_is_not_replayed_and_primary_survives_cleanup(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);publish_error=110;CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)==110&&publish_calls==1);
  retire_error=5;CHECK(f.api.retire(f.api.state,&f.source,&f.receipt)==5&&retire_calls==1);
  CHECK(f.api.retire(f.api.state,&f.source,&f.receipt)==125&&retire_calls==1);
  uwvm_rmw_publisher_transport_diagnostic d=diagnostic(&f,0);CHECK(d.first_primary==110&&d.publish_code==110&&d.retire_code==5&&d.retire_attempted);finish(&f);
}
static void inspection_error_is_not_publication_or_implicit_retry(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);inspect_error=107;
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)==107&&inspect_calls==1&&!publish_calls);
  CHECK(diagnostic(&f,0).first_primary==107&&!diagnostic(&f,0).last_wire_present);finish(&f);
}
static void retirement_precedes_raw_owner_close_and_blocks_business(void) {
  fixture f;init(&f,1);CHECK(activate(&f)==0);CHECK(uwvm_rmw_publisher_transport_destroy(&f.state)==17&&f.state);
  CHECK(f.api.retire(f.api.state,&f.source,&f.receipt)==0&&retire_calls==1&&f.source.handle==expected_source);
  CHECK(f.api.publish(f.api.state,&f.source,&f.receipt,NULL,0)==125&&!publish_calls);
  CHECK(f.api.retire(f.api.state,&f.source,&f.receipt)==125&&retire_calls==1);finish(&f);
}
static void unsupported_options_and_recursion_are_before_effects(void) {
  fixture f;init(&f,1);f.options.rmw_specific_publisher_payload=(void *)(uintptr_t)1;
  CHECK(activate(&f)==95&&!version_calls&&!activate_calls);size_t n=999;CHECK(uwvm_rmw_publisher_transport_count(f.state,&n)==0&&n==0);
  f.options.rmw_specific_publisher_payload=NULL;reenter=1;reenter_state=f.state;CHECK(activate(&f)==0&&reenter_code==17);reenter=0;finish(&f);
}
int main(void) {
  create_capacity_and_allocator_preserve_outputs();complete_zero_gid_and_all_qos_fields();native_enum_mapping_is_explicit();
  complete_wire_envelope_and_reserved_refusal();every_binding_member_is_mandatory();malformed_qos_is_not_declaration_attestation();
  incompatible_gid_retains_complete_cleanup_binding();actual_qos_mismatch_stays_cleanup_only();equivalent_requested_durations_use_real_rmw_time();
  version_absence_has_no_native_activation_and_no_replay();activation_error_has_no_fabricated_partial_receipt();source_and_capacity_slots_never_reuse();
  inspect_whole_snapshot_preserves_original_and_output();copied_owner_and_changed_public_receipt_are_not_authority();publish_uses_new_import_once_and_full_payload_bound();
  logical_empty_is_exactly_zero();publish_timeout_is_not_replayed_and_primary_survives_cleanup();inspection_error_is_not_publication_or_implicit_retry();
  retirement_precedes_raw_owner_close_and_blocks_business();unsupported_options_and_recursion_are_before_effects();
  puts("PASS_LOCAL_PUBLISHER_TRANSPORT_20_GROUPS_NOT_NATIVE_OR_ROS_ACCEPTANCE");return 0;
}
