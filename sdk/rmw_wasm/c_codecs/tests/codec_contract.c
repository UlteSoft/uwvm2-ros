/* SPDX-License-Identifier: Apache-2.0
 * Twenty-two authored actual-SDK contract groups, NEVER executed by this author.
 * Eleven independent actual RIHS01 values are mandatory CLI inputs. Negative
 * mutations are explicitly bad inputs, never replacement native identities.
 * No fake native object, hash, introspection callback or TypeSupport is linked.
 * Native allocation failure remains a separate actual-owner qualification;
 * the deterministic allocator case below faults only THIS registry arena.
 */
#include <uwvm_rmw/c_codecs.h>
#include <std_msgs/msg/string.h>
#include <std_srvs/srv/empty.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <builtin_interfaces/msg/time.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_typesupport_introspection_c/identifier.h>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/service_introspection.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifdef NDEBUG
#error Actual contract operations must remain active, regardless of build mode.
#endif
static unsigned checks,failures,groups;
#define CHECK(x) do{++checks;if(!(x)){++failures;fprintf(stderr,"FAIL_C_CODEC line=%u\n",(unsigned)__LINE__);}}while(0)
static uwvm_rmw_c_native_identities identities;
static rcutils_allocator_t allocator;
static int hex(char const*t,uint8_t out[32]){if(!t)return 0;unsigned nz=0;for(size_t i=0;i<64;++i){unsigned char c=(unsigned char)t[i];if(!c)return 0;unsigned v;
 if(c>='0'&&c<='9')v=c-'0';else if(c>='a'&&c<='f')v=c-'a'+10;else if(c>='A'&&c<='F')v=c-'A'+10;else return 0;
 if(i%2)out[i/2]|=(uint8_t)v;else out[i/2]=(uint8_t)(v<<4);nz|=v;}return !t[64]&&nz;}
static uwvm_rmw_c_codecs_registry*make(void){uwvm_rmw_c_codecs_registry*r=NULL;uint32_t code=uwvm_rmw_c_codecs_create(&identities,128,&allocator,&r);CHECK(code==0&&r);if(code||!r)abort();return r;}
static uwvm_rmw_codec const*get(uwvm_rmw_c_codecs_registry*r,enum uwvm_rmw_c_codec_index i){uwvm_rmw_codec const*c=NULL;CHECK(uwvm_rmw_c_codecs_get(r,i,&c)==0&&c);if(!c)abort();return c;}
static void never_exported_destroy(void){uwvm_rmw_c_codecs_registry*r=make();bool exported=true;size_t bound=0;CHECK(uwvm_rmw_c_codecs_observe(r,&exported,&bound)==0&&!exported&&bound==128);CHECK(uwvm_rmw_c_codecs_destroy_unexported(&r)==0&&!r);++groups;}
static void export_freezes_lifetime(void){uwvm_rmw_c_codecs_registry*r=make(),*saved=r;uwvm_rmw_codec const*all=NULL;size_t count=0;
 CHECK(uwvm_rmw_c_codecs_export(r,&all,&count)==0&&all&&count==7);CHECK(uwvm_rmw_c_codecs_destroy_unexported(&r)==17&&r==saved);bool x=false;size_t n=0;CHECK(!uwvm_rmw_c_codecs_observe(r,&x,&n)&&x);++groups;}
static void get_freezes_lifetime(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*c=get(r,UWVM_RMW_C_CODEC_STRING);CHECK(c->validation_state);CHECK(uwvm_rmw_c_codecs_destroy_unexported(&r)==17&&r);++groups;}
static void all_seven_actual_handles(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*c=NULL;size_t n=0;CHECK(!uwvm_rmw_c_codecs_export(r,&c,&n)&&n==7);
 for(size_t i=0;i<n;++i)CHECK(c[i].validate(c+i,c[i].generated_support)==0&&c[i].validation_state);
 CHECK(c[0].generated_support==ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String));rosidl_message_type_support_t const*original=c[0].generated_support;
 CHECK(c[0].validate(c,original->func(original,rosidl_typesupport_introspection_c__identifier))==0);++groups;}
static void every_hash_required(void){for(size_t i=0;i<11;++i){uwvm_rmw_c_native_identities ids=identities;memset(ids.hashes[i],0,32);uwvm_rmw_c_codecs_registry*r=NULL;CHECK(uwvm_rmw_c_codecs_create(&ids,128,&allocator,&r)==22&&!r);}++groups;}
static void every_hash_matches_original(void){for(size_t i=0;i<11;++i){uwvm_rmw_c_native_identities ids=identities;ids.hashes[i][0]^=1;uwvm_rmw_c_codecs_registry*r=NULL;CHECK(uwvm_rmw_c_codecs_create(&ids,128,&allocator,&r)==22&&!r);}++groups;}
static void actual_bound_checks(void){uwvm_rmw_c_codecs_registry*r=NULL;CHECK(uwvm_rmw_c_codecs_create(&identities,3,&allocator,&r)==22&&!r);CHECK(uwvm_rmw_c_codecs_create(&identities,4194305,&allocator,&r)==22&&!r);++groups;}
typedef struct{rcutils_allocator_t real;unsigned calls,frees;bool fail;} Fault;
static void*fa(size_t n,void*x){Fault*f=x;++f->calls;return f->fail?NULL:f->real.allocate(n,f->real.state);}
static void*fz(size_t n,size_t s,void*x){Fault*f=x;++f->calls;return f->fail?NULL:f->real.zero_allocate(n,s,f->real.state);}
static void*fr(void*p,size_t n,void*x){Fault*f=x;++f->calls;return f->fail?NULL:f->real.reallocate(p,n,f->real.state);}
static void fd(void*p,void*x){Fault*f=x;++f->frees;f->real.deallocate(p,f->real.state);}
static void arena_allocation_failure(void){Fault f={allocator,0,0,true};rcutils_allocator_t a={fa,fd,fr,fz,&f};uwvm_rmw_c_codecs_registry*r=NULL;
 CHECK(uwvm_rmw_c_codecs_create(&identities,128,&a,&r)==12&&!r&&f.calls==1&&!f.frees);f.fail=false;
 CHECK(!uwvm_rmw_c_codecs_create(&identities,128,&a,&r)&&r);CHECK(!uwvm_rmw_c_codecs_destroy_unexported(&r)&&!r&&f.calls==2&&f.frees==1);++groups;}
static void string_all_fields_nul_utf8(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);std_msgs__msg__String a={0},b={0};CHECK(std_msgs__msg__String__init(&a)&&std_msgs__msg__String__init(&b));
 char text[10]={'A',0,'B',(char)0xef,(char)0xbb,(char)0xbf,(char)0xf0,(char)0x9f,(char)0x98,(char)0x80};CHECK(rosidl_runtime_c__String__assignn(&a.data,text,sizeof text));
 unsigned char bytes[128]={0};size_t n=77;CHECK(!c->encode(&a,bytes,sizeof bytes,&n)&&n==14&&bytes[0]==10);CHECK(!c->decode(bytes,n,&b)&&b.data.size==10&&!memcmp(b.data.data,text,10)&&b.data.data[10]==0);
 std_msgs__msg__String__fini(&b);std_msgs__msg__String__fini(&a);++groups;}
static void string_invalid_native(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);std_msgs__msg__String a={0};unsigned char bytes[8];memset(bytes,0x55,sizeof bytes);size_t n=77;
 CHECK(c->encode(&a,bytes,sizeof bytes,&n)==22&&n==77);CHECK(std_msgs__msg__String__init(&a));CHECK(rosidl_runtime_c__String__assignn(&a.data,"\xc0\xaf",2));CHECK(c->encode(&a,bytes,sizeof bytes,&n)==22&&n==77&&bytes[0]==0x55);std_msgs__msg__String__fini(&a);++groups;}
static void string_short_output(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);std_msgs__msg__String a={0};CHECK(std_msgs__msg__String__init(&a)&&rosidl_runtime_c__String__assignn(&a.data,"abc",3));unsigned char bytes[8];memset(bytes,0x55,8);size_t n=77;
 CHECK(c->encode(&a,bytes,6,&n)==90&&n==77&&bytes[0]==0x55&&bytes[7]==0x55);std_msgs__msg__String__fini(&a);++groups;}
static void string_bad_wire_atomic(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);std_msgs__msg__String a={0};CHECK(std_msgs__msg__String__init(&a)&&rosidl_runtime_c__String__assignn(&a.data,"old",3));unsigned char bytes[6]={1,0,0,0,0xc0,0};char*old=a.data.data;
 CHECK(c->decode(bytes,5,&a)==22&&a.data.data==old&&a.data.size==3&&!memcmp(a.data.data,"old",3));bytes[4]='A';CHECK(c->decode(bytes,6,&a)==22&&a.data.data==old&&a.data.size==3);CHECK(c->decode(bytes,4,&a)==22&&a.data.data==old);std_msgs__msg__String__fini(&a);++groups;}
static void string_empty_and_alias_transaction(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);std_msgs__msg__String a={0};CHECK(std_msgs__msg__String__init(&a));unsigned char empty[4]={0};CHECK(!c->decode(empty,4,&a)&&!a.data.size&&a.data.data[0]==0);
 char wire[7]={3,0,0,0,'A',0,'B'};CHECK(rosidl_runtime_c__String__assignn(&a.data,wire,7));char const*input=a.data.data;CHECK(!c->decode(input,7,&a)&&a.data.size==3&&!memcmp(a.data.data,"A\0B",3));std_msgs__msg__String__fini(&a);++groups;}
static void empty_complete_logical_zero(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*a=get(r,UWVM_RMW_C_CODEC_EMPTY_REQUEST),*b=get(r,UWVM_RMW_C_CODEC_EMPTY_RESPONSE);
 std_srvs__srv__Empty_Request q={0};std_srvs__srv__Empty_Response v={0};CHECK(std_srvs__srv__Empty_Request__init(&q)&&std_srvs__srv__Empty_Response__init(&v));unsigned char physical=0x55;size_t n=77;
 CHECK(!a->maximum_wire_bytes&&!b->maximum_wire_bytes&&!a->encode(&q,&physical,0,&n)&&n==0&&physical==0x55);n=77;CHECK(!b->encode(&v,NULL,0,&n)&&n==0);
 q.structure_needs_at_least_one_member=77;CHECK(!a->decode(NULL,0,&q)&&q.structure_needs_at_least_one_member==0);v.structure_needs_at_least_one_member=88;CHECK(b->decode(&physical,1,&v)==22&&v.structure_needs_at_least_one_member==88);
 n=77;CHECK(b->encode(&v,&physical,0,&n)==22&&n==77&&physical==0x55);std_srvs__srv__Empty_Response__fini(&v);std_srvs__srv__Empty_Request__fini(&q);++groups;}
static void add_all_signed_fields(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*a=get(r,UWVM_RMW_C_CODEC_ADD_REQUEST),*b=get(r,UWVM_RMW_C_CODEC_ADD_RESPONSE);
 example_interfaces__srv__AddTwoInts_Request q={0},out={0};example_interfaces__srv__AddTwoInts_Response v={0},response={0};
 CHECK(example_interfaces__srv__AddTwoInts_Request__init(&q)&&example_interfaces__srv__AddTwoInts_Request__init(&out)&&example_interfaces__srv__AddTwoInts_Response__init(&v)&&example_interfaces__srv__AddTwoInts_Response__init(&response));q.a=INT64_MIN;q.b=INT64_MAX;v.sum=-42;
 unsigned char bytes[16]={0};size_t n=77;CHECK(!a->encode(&q,bytes,16,&n)&&n==16&&bytes[7]==0x80&&bytes[8]==0xff&&bytes[15]==0x7f);CHECK(!a->decode(bytes,16,&out)&&out.a==q.a&&out.b==q.b);CHECK(!b->encode(&v,bytes,8,&n)&&n==8&&!b->decode(bytes,8,&response)&&response.sum==-42);
 example_interfaces__srv__AddTwoInts_Response__fini(&response);example_interfaces__srv__AddTwoInts_Response__fini(&v);example_interfaces__srv__AddTwoInts_Request__fini(&out);example_interfaces__srv__AddTwoInts_Request__fini(&q);++groups;}
static void add_malformed_preserves_objects(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*a=get(r,UWVM_RMW_C_CODEC_ADD_REQUEST),*b=get(r,UWVM_RMW_C_CODEC_ADD_RESPONSE);example_interfaces__srv__AddTwoInts_Request q={123,456};example_interfaces__srv__AddTwoInts_Response v={789};unsigned char bytes[16];memset(bytes,0x55,16);size_t n=77;
 CHECK(a->decode(bytes,15,&q)==22&&q.a==123&&q.b==456);CHECK(b->decode(bytes,7,&v)==22&&v.sum==789);CHECK(b->encode(&v,bytes,7,&n)==90&&n==77&&bytes[0]==0x55);++groups;}
static void original_support_must_match(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);CHECK(c->validate(c,ROSIDL_GET_MSG_TYPE_SUPPORT(builtin_interfaces,msg,Time))==22);++groups;}
static void descriptor_address_is_not_clone(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_STRING);uwvm_rmw_codec copy=*c;CHECK(copy.validate(&copy,copy.generated_support)==22);copy.validation_state=NULL;CHECK(copy.validate(&copy,copy.generated_support)==22);++groups;}
static void complete_service_cross_components(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*e=get(r,UWVM_RMW_C_CODEC_EMPTY_SERVICE),*a=get(r,UWVM_RMW_C_CODEC_ADD_SERVICE);
 CHECK(e->kind==UWVM_RMW_SERVICE_CODEC&&!e->encode&&!e->decode&&!e->maximum_wire_bytes&&e->request==get(r,UWVM_RMW_C_CODEC_EMPTY_REQUEST)&&e->response==get(r,UWVM_RMW_C_CODEC_EMPTY_RESPONSE));
 CHECK(a->maximum_wire_bytes==16&&a->request==get(r,UWVM_RMW_C_CODEC_ADD_REQUEST)&&a->response==get(r,UWVM_RMW_C_CODEC_ADD_RESPONSE));CHECK(a->generated_support==ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts));++groups;}
static void invalid_export_has_no_lifetime_effect(void){uwvm_rmw_c_codecs_registry*r=make();uwvm_rmw_codec const*c=NULL;size_t n=99;bool exported=true;size_t bound=0;
 CHECK(uwvm_rmw_c_codecs_export(r,NULL,&n)==22&&n==99);CHECK(uwvm_rmw_c_codecs_get(r,(enum uwvm_rmw_c_codec_index)99,&c)==22&&!c);CHECK(!uwvm_rmw_c_codecs_observe(r,&exported,&bound)&&!exported);CHECK(!uwvm_rmw_c_codecs_destroy_unexported(&r)&&!r);++groups;}

static void actual_empty_event_callbacks(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_EMPTY_SERVICE);rosidl_service_type_support_t const*h=c->generated_support;
 std_srvs__srv__Empty_Request q={0};std_srvs__srv__Empty_Response v={0};CHECK(std_srvs__srv__Empty_Request__init(&q)&&std_srvs__srv__Empty_Response__init(&v));
 rosidl_service_introspection_info_t i={0};i.event_type=1;i.stamp_sec=-2;i.stamp_nanosec=300000000;i.sequence_number=42;
 std_srvs__srv__Empty_Event*e=h->event_message_create_handle_function(&i,&allocator,&q,&v);CHECK(e);
 if(e){CHECK(e->info.event_type==1&&e->info.stamp.sec==-2&&e->info.stamp.nanosec==300000000&&e->info.sequence_number==42&&e->request.size==1&&e->response.size==1);
 unsigned nz=0;for(size_t k=0;k<16;++k)nz|=e->info.client_gid[k];CHECK(!nz&&e->request.data[0].structure_needs_at_least_one_member==0&&e->response.data[0].structure_needs_at_least_one_member==0);CHECK(h->event_message_destroy_handle_function(e,&allocator));}
 std_srvs__srv__Empty_Response__fini(&v);std_srvs__srv__Empty_Request__fini(&q);++groups;}
static void actual_add_event_callbacks(void){uwvm_rmw_codec const*c=get(make(),UWVM_RMW_C_CODEC_ADD_SERVICE);rosidl_service_type_support_t const*h=c->generated_support;
 example_interfaces__srv__AddTwoInts_Request q={0};example_interfaces__srv__AddTwoInts_Response v={0};CHECK(example_interfaces__srv__AddTwoInts_Request__init(&q)&&example_interfaces__srv__AddTwoInts_Response__init(&v));q.a=17;q.b=-9;v.sum=8;
 rosidl_service_introspection_info_t i={0};i.event_type=3;i.stamp_sec=42;i.stamp_nanosec=300000000;i.sequence_number=INT64_MIN;for(size_t k=0;k<16;++k)i.client_gid[k]=(uint8_t)k;
 example_interfaces__srv__AddTwoInts_Event*e=h->event_message_create_handle_function(&i,&allocator,&q,&v);CHECK(e);
 if(e){CHECK(e->info.event_type==3&&e->info.stamp.sec==42&&e->info.stamp.nanosec==300000000&&e->info.sequence_number==INT64_MIN&&!memcmp(e->info.client_gid,i.client_gid,16)&&e->request.size==1&&e->response.size==1);
 CHECK(e->request.data[0].a==17&&e->request.data[0].b==-9&&e->response.data[0].sum==8);CHECK(h->event_message_destroy_handle_function(e,&allocator));}
 example_interfaces__srv__AddTwoInts_Response__fini(&v);example_interfaces__srv__AddTwoInts_Request__fini(&q);++groups;}
int main(int argc,char**argv){if(argc!=12){fputs("11 independent actual C NativeIdentities required, in public enum order\n",stderr);return 2;}for(size_t i=0;i<11;++i)if(!hex(argv[i+1],identities.hashes[i]))return 2;allocator=rcutils_get_default_allocator();
 never_exported_destroy();export_freezes_lifetime();get_freezes_lifetime();all_seven_actual_handles();every_hash_required();every_hash_matches_original();actual_bound_checks();arena_allocation_failure();string_all_fields_nul_utf8();string_invalid_native();string_short_output();string_bad_wire_atomic();string_empty_and_alias_transaction();empty_complete_logical_zero();add_all_signed_fields();add_malformed_preserves_objects();original_support_must_match();descriptor_address_is_not_clone();complete_service_cross_components();invalid_export_has_no_lifetime_effect();
 actual_empty_event_callbacks();actual_add_event_callbacks();CHECK(groups==22);printf("%s_ORIGINAL_RMW_PURE_C_CODEC_CONTRACT groups=%u checks=%u failures=%u native_delivery=0\n",failures?"FAIL":"PASS",groups,checks,failures);return failures?1:0;}
