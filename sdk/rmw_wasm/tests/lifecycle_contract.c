/* SPDX-License-Identifier: Apache-2.0
 * Source-authored LOCAL contracts. Link genuine generated std_msgs C/runtime/
 * introspection/type-description libraries and original rmw/rcutils utilities.
 * This transport and local guard are explicit test models, NOT a DDS/native
 * provider, installed-sdk qualification, real C wait provider or Guest proof. */
#include "uwvm_rmw/internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/empty.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/type_hash.h>
#include <rmw/qos_profiles.h>
#include <rmw/error_handling.h>
#include <rmw/time.h>
static unsigned checks,failures,opens,closes,publishes,notifications;
static uint32_t open_error,close_error;
static unsigned char actual_bytes[128];static size_t actual_length;
static uint64_t next_handle=UINT64_C(0x100000001);
static unsigned activations,inspections,native_retires,source_drains;
static uint32_t activation_error,native_retire_error;
static int activation_bad_binding,activation_bad_qos,inspect_bad_binding,model_zero_gid;
static rmw_qos_profile_t model_native_qos;
typedef struct model_endpoint {int live;uwvm_rmw_publisher_receipt receipt;} model_endpoint;
static model_endpoint native_endpoints[256];static size_t native_count;
static uint64_t next_native_incarnation=1;
static model_endpoint * native_find(uwvm_rmw_publisher_receipt const * r) {
  for(size_t i=0;i<native_count;++i){model_endpoint * e=native_endpoints+i;
    if(e->receipt.source==r->source&&e->receipt.instance==r->instance&&
      e->receipt.runtime_generation==r->runtime_generation&&e->receipt.session==r->session&&
      e->receipt.native_incarnation==r->native_incarnation)return e;}
  return NULL;
}
#define CHECK(condition) do{++checks;if(!(condition)){++failures;fprintf(stderr,"FAIL_UPSTREAM_RMW_LOCAL line=%u\n",(unsigned)__LINE__);}}while(0)
static uint32_t abi(void){return 1;}
static uint32_t open_alias(uint32_t memory,uint64_t offset,uint64_t length,uint64_t out) {
  ++opens;if(memory||!offset||!length||!out)return 22;if(open_error)return open_error;
  unsigned char * p=(unsigned char *)(uintptr_t)out;uint64_t h=next_handle++;
  for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(h>>(i*8));return 0;
}
static uint32_t close_alias(uint64_t handle){
  CHECK(handle!=0);++closes;
  /* Explicit model of source ownership: a partial/unknown activation is still
   * drained by source close, even if its malformed reply was not trusted. */
  for(size_t i=0;i<native_count;++i)if(native_endpoints[i].live&&native_endpoints[i].receipt.source==handle){native_endpoints[i].live=0;++source_drains;}
  return close_error;
}
static uint32_t copy_model_native_bytes(uint64_t handle,uint32_t memory,uint64_t bytes,uint64_t length) {
  CHECK(handle&&memory==0&&length<=sizeof(actual_bytes));if(length>sizeof(actual_bytes))return 90;
  memcpy(actual_bytes,(void const *)(uintptr_t)bytes,(size_t)length);actual_length=(size_t)length;++publishes;return 0;
}
/* Deliberately no legacy publish callback: B must use the retained provider. */
static roswasmc_backend const model={.version=2,.abi_version=abi,.open=open_alias,.close=close_alias};
static uint32_t activate_native(void * state,roswasmc_capability const * owner,
  uwvm_rmw_endpoint_selection const * row,uwvm_rmw_codec const * codec,
  rmw_publisher_options_t const * options,uwvm_rmw_publisher_receipt * out) {
  (void)state;CHECK(owner&&owner->handle&&row&&codec&&options&&out);++activations;
  CHECK(native_count<256&&next_native_incarnation);if(native_count==256)return 90;
  model_endpoint * e=native_endpoints+native_count++;e->live=1;
  e->receipt=(uwvm_rmw_publisher_receipt){1,2,owner->handle,31,47,59,next_native_incarnation++,model_native_qos};
  e->receipt.native_gid_bytes=RMW_GID_STORAGE_SIZE;
  /* Explicit MODEL-only native GID fixture; no domain/global native acceptance.
   * B must copy/query this provider fact instead of manufacturing a counter. */
  for(size_t i=0;i<RMW_GID_STORAGE_SIZE;++i)e->receipt.native_gid[i]=(uint8_t)(0x80u+i);
  for(size_t i=0;i<8&&i<RMW_GID_STORAGE_SIZE;++i)e->receipt.native_gid[RMW_GID_STORAGE_SIZE-1-i]=(uint8_t)(e->receipt.native_incarnation>>(i*8));
  if(model_zero_gid)memset(e->receipt.native_gid,0,RMW_GID_STORAGE_SIZE);
  *out=e->receipt;if(activation_bad_binding)out->source^=1;
  if(activation_bad_qos)out->actual_qos.liveliness=RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT;
  return activation_error;
}
static uint32_t inspect_native(void * state,roswasmc_capability const * owner,
  uwvm_rmw_publisher_receipt const * receipt,uwvm_rmw_publisher_receipt * out) {
  (void)state;model_endpoint * e=native_find(receipt);++inspections;
  CHECK(owner&&owner->handle==receipt->source);if(!e||!e->live)return 9;
  *out=e->receipt;if(inspect_bad_binding)++out->native_incarnation;return 0;
}
static uint32_t publish_native(void * state,roswasmc_capability const * owner,
  uwvm_rmw_publisher_receipt const * receipt,void const * bytes,size_t length) {
  (void)state;model_endpoint * e=native_find(receipt);CHECK(owner&&owner->handle==receipt->source);
  if(!e||!e->live)return 9;
  return copy_model_native_bytes(owner->handle,0,(uint64_t)(uintptr_t)bytes,length);
}
static uint32_t retire_native(void * state,roswasmc_capability const * owner,
  uwvm_rmw_publisher_receipt const * receipt) {
  (void)state;model_endpoint * e=native_find(receipt);++native_retires;
  CHECK(owner&&owner->handle==receipt->source&&e&&e->live);if(!e||!e->live)return 9;
  e->live=0;return native_retire_error;
}
static uwvm_rmw_publisher_provider const native_provider={1,NULL,activate_native,inspect_native,publish_native,retire_native};
/* A genuine allocated local trigger state is modeled here, not a constant
 * success guard. Production must use the independently reviewed C provider. */
typedef struct local_guard {rmw_guard_condition_t public_handle;unsigned triggered;} local_guard;
static rmw_guard_condition_t * guard_create(rmw_context_t * context,void * state) {
  (void)state;local_guard * g=calloc(1,sizeof(*g));if(!g)return NULL;
  g->public_handle.implementation_identifier=uwvm_rmw_identifier;g->public_handle.context=context;
  g->public_handle.data=g;
  if(uwvm_rmw_context_child_retain(uwvm_rmw_context_find(context,0))){free(g);return NULL;}
  return &g->public_handle;
}
static rmw_ret_t guard_destroy(rmw_guard_condition_t * guard,void * state){
  (void)state;uwvm_rmw_context * c=uwvm_rmw_context_find(guard->context,1);free(guard);
  return uwvm_rmw_context_child_release(c)?RMW_RET_ERROR:RMW_RET_OK;
}
static rmw_ret_t guard_notify(rmw_guard_condition_t const * guard,void * state) {
  (void)state;local_guard * g=(local_guard *)guard->data;++g->triggered;++notifications;return RMW_RET_OK;
}
static uwvm_rmw_codec codec;
static uwvm_rmw_node_selection const nodes[]={{"original_node","/"}};
static uwvm_rmw_endpoint_selection rows[2];static uwvm_rmw_profile profile;
static rosidl_message_type_support_t const * support;
static void selection(void) {
  support=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);
  CHECK(support&&support->get_type_hash_func);if(!support||!support->get_type_hash_func)return;
  rosidl_type_hash_t const * h=support->get_type_hash_func(support);CHECK(h&&h->version==1);if(!h||h->version!=1)return;
  CHECK(uwvm_rmw_string_codec_init(&codec,support,h->value,sizeof(actual_bytes))==0);
  rmw_qos_profile_t actual=rmw_qos_profile_default;
  actual.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
  model_native_qos=actual; /* State of the EXPLICIT LOCAL native endpoint model. */
  rows[0]=(uwvm_rmw_endpoint_selection){0,0,UWVM_RMW_PUBLISHER,"/selected",
    rmw_qos_profile_default,actual,{"selected-publish-a",NULL},1};
  rows[1]=rows[0];rows[1].resolved_name="/second";rows[1].aliases[0]="selected-publish-b";
  profile=(uwvm_rmw_profile){1,0,&model,nodes,1,&codec,1,rows,2,{1,NULL,guard_create,guard_destroy,guard_notify},native_provider};
}
/* The real admitted context must stay at its original caller-owned address. */
static void init_at(rmw_context_t * context) {
  CHECK(uwvm_rmw_install_profile(&profile)==RMW_RET_OK);
  rmw_init_options_t options=rmw_get_zero_initialized_init_options();rcutils_allocator_t a=rcutils_get_default_allocator();
  CHECK(rmw_init_options_init(&options,a)==RMW_RET_OK);options.instance_id=42;
  options.enclave=a.allocate(2,a.state);CHECK(options.enclave!=NULL);if(options.enclave)memcpy(options.enclave,"/",2);
  *context=rmw_get_zero_initialized_context();CHECK(rmw_init(&options,context)==RMW_RET_OK);
  CHECK(rmw_init_options_fini(&options)==RMW_RET_OK);
}
static rmw_node_t * node(rmw_context_t * c){return rmw_create_node(c,"original_node","/");}
static rmw_publisher_t * pub(rmw_node_t * n,char const * name) {
  rmw_publisher_options_t options=rmw_get_default_publisher_options();
  return rmw_create_publisher(n,support,name,&rmw_qos_profile_default,&options);
}
static void retire_public_endpoints(rmw_context_t * c,rmw_node_t * n) {
  uwvm_rmw_context * internal=uwvm_rmw_context_find(c,1);if(!internal)return;
  for(size_t count=internal->resource_count;count;--count) {
    uwvm_rmw_resource * r=internal->resources+count-1;
    if(r->live)CHECK(rmw_destroy_publisher(n,(rmw_publisher_t *)r->object)==RMW_RET_OK);
  }
}
static void finish(rmw_context_t * c,rmw_node_t * n) {
  CHECK(rmw_shutdown(c)==RMW_RET_OK);retire_public_endpoints(c,n);if(n)CHECK(rmw_destroy_node(n)==RMW_RET_OK);
  CHECK(rmw_context_fini(c)==RMW_RET_OK);CHECK(uwvm_rmw_install_profile(NULL)==RMW_RET_OK);rmw_reset_error();
}
static void test_default_no_selection(void) {
  unsigned before=opens;CHECK(uwvm_rmw_install_profile(NULL)==RMW_RET_OK);
  rmw_init_options_t o=rmw_get_zero_initialized_init_options();
  CHECK(rmw_init_options_init(&o,rcutils_get_default_allocator())==RMW_RET_ERROR);CHECK(!o.impl&&opens==before);rmw_reset_error();
}
static void test_missing_real_guard(void) {
  uwvm_rmw_graph_guard_provider saved=profile.graph;profile.graph=(uwvm_rmw_graph_guard_provider){0};
  rmw_context_t c;init_at(&c);unsigned before=opens;CHECK(node(&c)==NULL);CHECK(opens==before);finish(&c,NULL);profile.graph=saved;
}
static void test_exact_selection(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);CHECK(n!=NULL);unsigned before=opens;
  CHECK(pub(n,"/unselected")==NULL);rmw_publisher_options_t options=rmw_get_default_publisher_options();
  CHECK(rmw_create_publisher(n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,Bool),"/selected",&rmw_qos_profile_default,&options)==NULL);
  rmw_qos_profile_t wrong=rmw_qos_profile_default;++wrong.depth;
  CHECK(rmw_create_publisher(n,support,"/selected",&wrong,&options)==NULL);CHECK(opens==before);finish(&c,n);
}
static void test_real_object_canonical_value(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");CHECK(p!=NULL);
  std_msgs__msg__String msg;CHECK(std_msgs__msg__String__init(&msg));CHECK(rosidl_runtime_c__String__assignn(&msg.data,"A\0B",3));
  unsigned before=publishes;CHECK(rmw_publish(p,&msg,NULL)==RMW_RET_OK);CHECK(publishes==before+1&&actual_length==7);
  CHECK(actual_bytes[0]==3&&!actual_bytes[1]&&!actual_bytes[2]&&!actual_bytes[3]&&!memcmp(actual_bytes+4,"A\0B",3));
  std_msgs__msg__String__fini(&msg);finish(&c,n);
}
static void test_bad_native_value_before_effect(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");
  std_msgs__msg__String msg;CHECK(std_msgs__msg__String__init(&msg));size_t old=msg.data.size;msg.data.size=msg.data.capacity;
  unsigned before=publishes;CHECK(rmw_publish(p,&msg,NULL)==RMW_RET_INVALID_ARGUMENT);CHECK(publishes==before);
  msg.data.size=old;std_msgs__msg__String__fini(&msg);finish(&c,n);
}
static void test_native_denial_no_owner_leak(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);unsigned before=closes;open_error=13;
  CHECK(pub(n,"/selected")==NULL);open_error=0;uwvm_rmw_diagnostics d;CHECK(uwvm_rmw_get_diagnostics(&c,&d)==RMW_RET_OK);
  CHECK(d.first_raw_primary==13&&d.owner_acquisitions==0&&closes==before);finish(&c,n);
}
static void test_actual_provider_gid_query(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * a=pub(n,"/selected"),*b=pub(n,"/second");
  rmw_gid_t x={0},y={0};bool same=true;CHECK(rmw_get_gid_for_publisher(a,&x)==RMW_RET_OK&&rmw_get_gid_for_publisher(b,&y)==RMW_RET_OK);
  CHECK(rmw_compare_gids_equal(&x,&y,&same)==RMW_RET_OK&&!same);
  CHECK(native_count>=2&&!memcmp(x.data,native_endpoints[native_count-2].receipt.native_gid,RMW_GID_STORAGE_SIZE));
  CHECK(!memcmp(y.data,native_endpoints[native_count-1].receipt.native_gid,RMW_GID_STORAGE_SIZE));
  /* A complete actually written zero-valued model GID remains data, not a
   * universal missing-value sentinel. This is not native/global proof. */
  model_zero_gid=1;rmw_publisher_t * z=pub(n,"/selected");model_zero_gid=0;
  rmw_gid_t zero={0},zid={0};CHECK(z&&rmw_get_gid_for_publisher(z,&zid)==RMW_RET_OK&&
    zid.implementation_identifier==uwvm_rmw_identifier&&!memcmp(zid.data,zero.data,RMW_GID_STORAGE_SIZE));finish(&c,n);
}
static void test_effective_qos_not_request_projection(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");rmw_qos_profile_t q;
  CHECK(rmw_publisher_get_actual_qos(p,&q)==RMW_RET_OK);CHECK(q.liveliness==RMW_QOS_POLICY_LIVELINESS_AUTOMATIC);
  CHECK(q.depth==rmw_qos_profile_default.depth);finish(&c,n);
}
static void test_all_cleanup_outcomes(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);CHECK(pub(n,"/selected")&&pub(n,"/second"));
  unsigned before=closes;close_error=107;CHECK(rmw_shutdown(&c)==RMW_RET_ERROR);close_error=0;
  uwvm_rmw_diagnostics d;CHECK(uwvm_rmw_get_diagnostics(&c,&d)==RMW_RET_OK);
  CHECK(closes==before+2&&d.owner_close_attempts==2&&d.owner_close_failures==2&&
    d.owner_close_codes[0]==107&&d.owner_close_codes[1]==107&&d.first_raw_primary==0);
  CHECK(rmw_shutdown(&c)==RMW_RET_OK&&closes==before+2);retire_public_endpoints(&c,n);CHECK(closes==before+2);CHECK(rmw_destroy_node(n)==RMW_RET_OK);
  CHECK(rmw_context_fini(&c)==RMW_RET_OK);CHECK(uwvm_rmw_install_profile(NULL)==RMW_RET_OK);rmw_reset_error();
}
static void test_explicit_rmw_function_scope(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");
  uwvm_rmw_scope scope;CHECK(uwvm_rmw_scope_enter(uwvm_rmw_context_find(&c,0),&scope,0)==RMW_RET_OK);
  CHECK(rmw_destroy_publisher(n,p)==RMW_RET_ERROR);CHECK(rmw_shutdown(&c)==RMW_RET_ERROR);CHECK(pub(n,"/second")==NULL);
  std_msgs__msg__String msg;CHECK(std_msgs__msg__String__init(&msg));CHECK(rmw_publish(p,&msg,NULL)==RMW_RET_OK);
  std_msgs__msg__String__fini(&msg);uwvm_rmw_scope_leave(&scope);finish(&c,n);
}
static void test_real_local_graph_lifetime(void) {
  rmw_context_t c;init_at(&c);unsigned before=notifications;rmw_node_t * n=node(&c);
  rmw_guard_condition_t const * g=rmw_node_get_graph_guard_condition(n);CHECK(g&&((local_guard *)g->data)->triggered==1);
  rmw_publisher_t * p=pub(n,"/selected");CHECK(p&&notifications==before+2);
  CHECK(rmw_destroy_node(n)==RMW_RET_ERROR);CHECK(rmw_destroy_publisher(n,p)==RMW_RET_OK);
  CHECK(notifications==before+3);finish(&c,n);
}
static void test_options_complete_copy(void) {
  CHECK(uwvm_rmw_install_profile(&profile)==RMW_RET_OK);rcutils_allocator_t a=rcutils_get_default_allocator();
  rmw_init_options_t x=rmw_get_zero_initialized_init_options(),y=rmw_get_zero_initialized_init_options();
  CHECK(rmw_init_options_init(&x,a)==RMW_RET_OK);x.enclave=a.allocate(5,a.state);CHECK(x.enclave!=NULL);if(x.enclave)memcpy(x.enclave,"/app",5);
  CHECK(rmw_init_options_copy(&x,&y)==RMW_RET_OK);CHECK(y.enclave&&x.enclave!=y.enclave&&!strcmp(x.enclave,y.enclave));
  CHECK(uwvm_rmw_install_profile(NULL)==RMW_RET_ERROR);CHECK(rmw_init_options_fini(&x)==RMW_RET_OK&&rmw_init_options_fini(&y)==RMW_RET_OK);
  CHECK(uwvm_rmw_install_profile(NULL)==RMW_RET_OK);rmw_reset_error();
}
static void test_missing_native_publisher_provider(void) {
  uwvm_rmw_publisher_provider old=profile.publisher;profile.publisher=(uwvm_rmw_publisher_provider){0};
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);unsigned before=opens;
  CHECK(pub(n,"/selected")==NULL&&opens==before);uwvm_rmw_diagnostics d;
  CHECK(uwvm_rmw_get_diagnostics(&c,&d)==RMW_RET_OK&&d.first_raw_primary==95);
  finish(&c,n);profile.publisher=old;
}
static void test_string_descriptor_association(void) {
  uwvm_rmw_codec changed=codec;unsigned before=opens;
  changed.native_type="example_msgs/msg/String";CHECK(changed.validate(&changed,support)==22);
  changed=codec;changed.kind=UWVM_RMW_SERVICE_CODEC;CHECK(changed.validate(&changed,support)==22);
  changed=codec;changed.maximum_wire_bytes=0;CHECK(changed.validate(&changed,support)==22);
  CHECK(opens==before);
}
static void test_invalid_activation_cleanup(void) {
  for(unsigned mode=0;mode<3;++mode){
    rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);
    unsigned raw_before=closes,native_before=native_retires,drains_before=source_drains;
    activation_bad_binding=mode==0;activation_bad_qos=mode==1;activation_error=mode==2?107:0;
    CHECK(pub(n,"/selected")==NULL);uwvm_rmw_diagnostics d;CHECK(uwvm_rmw_get_diagnostics(&c,&d)==RMW_RET_OK);
    CHECK(closes==raw_before+1&&d.owner_close_attempts==1);
    CHECK(native_retires==native_before+(mode?1u:0u)&&source_drains==drains_before+(mode?0u:1u));
    CHECK(d.first_raw_primary==(mode==2?107u:22u));
    activation_bad_binding=activation_bad_qos=0;activation_error=0;finish(&c,n);
  }
}
static void test_native_inspection_is_associated(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");
  rmw_qos_profile_t untouched=rmw_qos_profile_default,q=untouched;unsigned before=inspections;
  inspect_bad_binding=1;CHECK(rmw_publisher_get_actual_qos(p,&q)==RMW_RET_INVALID_ARGUMENT);
  CHECK(inspections==before+1&&uwvm_rmw_qos_equal(&q,&untouched));
  rmw_gid_t gid={0},old_gid=gid;CHECK(rmw_get_gid_for_publisher(p,&gid)==RMW_RET_INVALID_ARGUMENT&&
    gid.implementation_identifier==old_gid.implementation_identifier&&!memcmp(gid.data,old_gid.data,RMW_GID_STORAGE_SIZE));inspect_bad_binding=0;
  CHECK(rmw_publisher_get_actual_qos(p,&q)==RMW_RET_OK&&uwvm_rmw_qos_equal(&q,&model_native_qos));finish(&c,n);
}
static void test_native_retire_failure_still_closes_raw(void) {
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);CHECK(pub(n,"/selected")&&pub(n,"/second"));
  unsigned raw_before=closes,native_before=native_retires;native_retire_error=107;close_error=110;
  CHECK(rmw_shutdown(&c)==RMW_RET_ERROR);uwvm_rmw_diagnostics d;
  CHECK(uwvm_rmw_get_diagnostics(&c,&d)==RMW_RET_OK);
  CHECK(native_retires==native_before+2&&closes==raw_before+2&&d.native_retire_attempts==2&&d.native_retire_failures==2);
  CHECK(d.native_retire_codes[0]==107&&d.native_retire_codes[1]==107&&d.owner_close_codes[0]==110&&d.owner_close_codes[1]==110);
  native_retire_error=close_error=0;retire_public_endpoints(&c,n);CHECK(native_retires==native_before+2&&closes==raw_before+2);
  CHECK(rmw_destroy_node(n)==RMW_RET_OK&&rmw_context_fini(&c)==RMW_RET_OK&&uwvm_rmw_install_profile(NULL)==RMW_RET_OK);rmw_reset_error();
}
/* LOCAL known-type Empty codec over the actual installed generated Empty
 * object/accessor, never a fake C struct or a production native exporter. This
 * regression exercises the generic registry's genuine zero canonical capacity
 * and nonzero private allocation. Runtime/native qualification remains absent.
 */
static uint32_t empty_validate(uwvm_rmw_codec const * c,void const * supplied) {
  rosidl_message_type_support_t const * expected=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,Empty);
  if(!c||c->version!=1||c->kind!=UWVM_RMW_MESSAGE_CODEC||!c->native_type||
    strcmp(c->native_type,"std_msgs/msg/Empty")||c->maximum_wire_bytes||supplied!=expected||
    !expected||!expected->get_type_hash_func||!expected->get_type_description_func||!expected->get_type_description_sources_func)return 22;
  rosidl_type_hash_t const * hash=expected->get_type_hash_func(expected);
  return !hash||hash->version!=1||memcmp(hash->value,c->native_rihs01,32)||
    !expected->get_type_description_func(expected)||!expected->get_type_description_sources_func(expected)?22:0;
}
static uint32_t empty_encode(void const * object,void * bytes,size_t cap,size_t * length) {
  (void)bytes;if(!object||cap||!length)return 22;*length=0;return 0;
}
static uint32_t empty_decode(void const * bytes,size_t length,void * object) {
  (void)bytes;if(length||!object)return 22;
  return std_msgs__msg__Empty__init((std_msgs__msg__Empty *)object)?0:12;
}
static void test_actual_generated_empty_zero_canonical_extent(void) {
  rosidl_message_type_support_t const * old_support=support;
  uwvm_rmw_codec const * old_codecs=profile.codecs;
  support=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,Empty);
  CHECK(support&&support->get_type_hash_func);if(!support||!support->get_type_hash_func){support=old_support;return;}
  rosidl_type_hash_t const * hash=support->get_type_hash_func(support);
  CHECK(hash&&hash->version==1);if(!hash||hash->version!=1){support=old_support;return;}
  uwvm_rmw_codec empty={1,UWVM_RMW_MESSAGE_CODEC,"std_msgs/msg/Empty",support,{0},0,empty_validate,empty_encode,empty_decode,NULL,NULL};
  memcpy(empty.native_rihs01,hash->value,32);profile.codecs=&empty;
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");CHECK(p!=NULL);
  std_msgs__msg__Empty message;CHECK(std_msgs__msg__Empty__init(&message));
  unsigned before=publishes;CHECK(rmw_publish(p,&message,NULL)==RMW_RET_OK&&publishes==before+1&&actual_length==0);
  std_msgs__msg__Empty__fini(&message);finish(&c,n);profile.codecs=old_codecs;support=old_support;
}
static void test_provider_child_retains_actual_context(void) {
  rmw_context_t c;init_at(&c);uwvm_rmw_context * state=uwvm_rmw_context_find(&c,0);uwvm_rmw_scope scope;
  CHECK(uwvm_rmw_context_child_retain(state)==22);
  CHECK(uwvm_rmw_scope_enter(state,&scope,0)==RMW_RET_OK&&uwvm_rmw_context_child_retain(state)==0);uwvm_rmw_scope_leave(&scope);
  CHECK(rmw_shutdown(&c)==RMW_RET_OK&&rmw_context_fini(&c)==RMW_RET_INVALID_ARGUMENT);
  CHECK(uwvm_rmw_scope_enter(state,&scope,1)==RMW_RET_OK&&uwvm_rmw_context_child_retain(state)==22&&uwvm_rmw_context_child_release(state)==0);uwvm_rmw_scope_leave(&scope);
  CHECK(rmw_context_fini(&c)==RMW_RET_OK&&uwvm_rmw_install_profile(NULL)==RMW_RET_OK);rmw_reset_error();
}
/* These are LOCAL regression groups linked to original rmw/src/time.c, not
 * a copied duration implementation, new native transport or Host QoS proof. */
static void test_original_rmw_duration_equivalence(void) {
  rmw_qos_profile_t a=model_native_qos,b=a;
  a.deadline=(rmw_time_t){0,UINT64_C(1000000000)};b.deadline=(rmw_time_t){1,0};
  a.lifespan=(rmw_time_t){1,UINT64_C(1000000100)};b.lifespan=(rmw_time_t){2,100};
  a.liveliness_lease_duration=(rmw_time_t){0,UINT64_C(2000000000)};
  b.liveliness_lease_duration=(rmw_time_t){2,0};
  CHECK(uwvm_rmw_qos_concrete(&a)&&uwvm_rmw_qos_concrete(&b));
  CHECK(uwvm_rmw_qos_equal(&a,&b));
  ++b.deadline.nsec;CHECK(!uwvm_rmw_qos_equal(&a,&b));--b.deadline.nsec;
  ++b.lifespan.nsec;CHECK(!uwvm_rmw_qos_equal(&a,&b));--b.lifespan.nsec;
  ++b.liveliness_lease_duration.nsec;CHECK(!uwvm_rmw_qos_equal(&a,&b));
  rmw_time_t infinity=RMW_DURATION_INFINITE;
  a.deadline=(rmw_time_t){UINT64_MAX,0};b.deadline=infinity;
  a.lifespan=(rmw_time_t){1,UINT64_MAX};b.lifespan=infinity;
  a.liveliness_lease_duration=(rmw_time_t){0,UINT64_C(2000000000)};
  b.liveliness_lease_duration=(rmw_time_t){2,0};
  CHECK(rmw_time_equal(a.deadline,b.deadline)&&rmw_time_equal(a.lifespan,b.lifespan));
  CHECK(uwvm_rmw_qos_concrete(&a)&&uwvm_rmw_qos_equal(&a,&b));
  --b.deadline.nsec;CHECK(!uwvm_rmw_qos_equal(&a,&b));
}
static void test_nonnormalized_request_matches_selected_qos(void) {
  rmw_qos_profile_t saved=rows[0].requested_qos,request=saved;
  rows[0].requested_qos.deadline=(rmw_time_t){0,UINT64_C(1000000000)};
  rows[0].requested_qos.lifespan=(rmw_time_t){1,UINT64_C(1000000100)};
  rows[0].requested_qos.liveliness_lease_duration=(rmw_time_t){0,UINT64_C(2000000000)};
  request.deadline=(rmw_time_t){1,0};request.lifespan=(rmw_time_t){2,100};
  request.liveliness_lease_duration=(rmw_time_t){2,0};
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);unsigned before=opens;
  rmw_publisher_options_t options=rmw_get_default_publisher_options();
  rmw_publisher_t * p=rmw_create_publisher(n,support,"/selected",&request,&options);
  CHECK(p!=NULL&&opens==before+1);finish(&c,n);rows[0].requested_qos=saved;
}
static void test_nonnormalized_effective_qos_keeps_actual_reply(void) {
  rmw_qos_profile_t saved=rows[0].effective_qos,saved_native=model_native_qos;
  rows[0].effective_qos.deadline=(rmw_time_t){0,UINT64_C(1000000000)};
  rows[0].effective_qos.lifespan=(rmw_time_t){1,UINT64_C(1000000100)};
  rows[0].effective_qos.liveliness_lease_duration=(rmw_time_t){UINT64_MAX,0};
  model_native_qos.deadline=(rmw_time_t){1,0};model_native_qos.lifespan=(rmw_time_t){2,100};
  model_native_qos.liveliness_lease_duration=(rmw_time_t)RMW_DURATION_INFINITE;
  rmw_context_t c;init_at(&c);rmw_node_t * n=node(&c);rmw_publisher_t * p=pub(n,"/selected");
  CHECK(p!=NULL);rmw_qos_profile_t out=rmw_qos_profile_default;
  CHECK(rmw_publisher_get_actual_qos(p,&out)==RMW_RET_OK);
  /* Preserve the actual provider's representation instead of substituting or
   * normalizing the independently selected assertion row. */
  CHECK(out.deadline.sec==1&&out.deadline.nsec==0&&out.lifespan.sec==2&&out.lifespan.nsec==100);
  CHECK(out.liveliness_lease_duration.sec==model_native_qos.liveliness_lease_duration.sec&&
    out.liveliness_lease_duration.nsec==model_native_qos.liveliness_lease_duration.nsec);
  CHECK(uwvm_rmw_qos_equal(&out,&rows[0].effective_qos));finish(&c,n);
  rows[0].effective_qos=saved;model_native_qos=saved_native;
}
int main(void) {
  selection();if(failures)return 1;test_default_no_selection();test_missing_real_guard();test_exact_selection();test_real_object_canonical_value();
  test_bad_native_value_before_effect();test_native_denial_no_owner_leak();test_actual_provider_gid_query();test_effective_qos_not_request_projection();
  test_all_cleanup_outcomes();test_explicit_rmw_function_scope();test_real_local_graph_lifetime();test_options_complete_copy();
  test_missing_native_publisher_provider();test_string_descriptor_association();test_invalid_activation_cleanup();
  test_native_inspection_is_associated();test_native_retire_failure_still_closes_raw();test_actual_generated_empty_zero_canonical_extent();test_provider_child_retains_actual_context();
  test_original_rmw_duration_equivalence();test_nonnormalized_request_matches_selected_qos();test_nonnormalized_effective_qos_keeps_actual_reply();
  printf("%s_UPSTREAM_RMW_LOCAL groups=22 checks=%u failures=%u\n",failures?"FAIL":"PASS",checks,failures);return failures?1:0;
}
