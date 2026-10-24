/* Derived SOURCE model: original35 groups plus17 explicit availability groups.
 * No model/native/Guest execution by this author. */
/* SPDX-License-Identifier: Apache-2.0
 * Unexecuted host-local transport MODELS linked to the ORIGINAL installed RMW,
 * rosidl generated String/AddTwoInts, actual A and B/C implementation objects.
 * The fake Host here is deliberately explicit: its values are not native DDS
 * evidence, Runtime policy/URMI validation, actual clock, ROS, WASI or SDK credit.
 * Select one named group per fresh process because C bootstrap is immutable.
 */
#include <uwvm_rmw_c/provider.h>
#include <uwvm_rmw_platform.h>
#include <rcutils/allocator.h>
#include <rcutils/strdup.h>
#include <rmw/error_handling.h>
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/detail/string__functions.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <rosidl_runtime_c/type_hash.h>
#ifdef NDEBUG
#error "RMW C model operations require active assertions in every build configuration"
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned query_calls,query_answer=1;static uint32_t query_error;static int query_unwritten;static uint64_t closed_order[64];
static unsigned opens,closes,activations,retires,submits,replies,inspect_calls,events,order,retire_order,close_order;
static uint32_t activation_error,reply_error;static int qos_drift,missing_gid=1,wide_gid;
static uint64_t next_handle=1,next_ticket=101,event_ticket,event_sequence=1;
static uint16_t event_kind=3;static unsigned event_flags;static uint32_t event_error;static size_t response_bytes=8;
static int pending_response,pending_request,topic_pending;static unsigned char submitted[32];static size_t submitted_length;
static int64_t clock_value=100;static unsigned clock_reads,wait_calls;static int rollback_clock;
static uwvm_rmw_codec codecs[4];static uwvm_rmw_endpoint_selection endpoints[3];static uwvm_rmw_profile selection;
static rmw_qos_profile_t qos;static rmw_context_t context;static rmw_node_t*node;static char const*group;
static void w64(unsigned char*p,uint64_t v){for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(v>>(8*i));}
static uint32_t abi(void){return 1;}
static uint32_t open_alias(uint32_t memory,uint64_t name,uint64_t length,uint64_t output){
 (void)memory;(void)name;(void)length;++opens;
 if(!strcmp(group,"availability-third-open-denied")&&opens==3)return 13;
 w64((unsigned char*)(uintptr_t)output,UINT64_C(0x100000000)+next_handle++);return 0;
}
static uint32_t close_alias(uint64_t h){assert(h);assert(closes<64);closed_order[closes]=h;++closes;close_order=++order;
 return !strcmp(group,"availability-close-errors")&&closes==1?17:0;}
static uint32_t time_request(uint64_t h,uint32_t memory,uint64_t input,uint64_t length,uint64_t output,uint64_t capacity,uint64_t out_length){
 (void)h;(void)memory;(void)input;assert(length==0&&capacity>=8);++clock_reads;
 w64((unsigned char*)(uintptr_t)output,(uint64_t)clock_value);w64((unsigned char*)(uintptr_t)out_length,8);return 0;
}
static roswasmc_backend backend={.version=2,.abi_version=abi,.open=open_alias,.close=close_alias,
 .request={[ROSWASMC_TIME_READ_SYSTEM]=time_request,[ROSWASMC_TIME_READ_STEADY]=time_request}};
static uint32_t activate(void*state,enum uwvm_rmw_endpoint_kind kind,roswasmc_capability const*source,
 roswasmc_capability const*aux,uwvm_rmw_codec const*codec,rmw_qos_profile_t const*requested,uwvm_rmw_c_native_endpoint*out){
 (void)state;(void)codec;assert(source&&source->handle);assert(kind==UWVM_RMW_SUBSCRIPTION||aux);++activations;
 if(activation_error)return activation_error;
 uwvm_rmw_c_native_endpoint next={.version=1,.backend=2,.source=source->handle,.instance=77,.runtime_generation=88,.session=99,
  .native_incarnation=1000+source->handle,.qos_count=kind==UWVM_RMW_SUBSCRIPTION?1:2};
 next.actual_qos[0]=next.actual_qos[1]=*requested;
 if(kind==UWVM_RMW_CLIENT&&!missing_gid){next.native_gid_bytes=wide_gid?24:16;memset(next.native_gid,0x44,next.native_gid_bytes);}
 *out=next;return 0;
}
static uint32_t inspect(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,
 uwvm_rmw_c_native_endpoint const*expected,uwvm_rmw_c_native_endpoint*out){
 (void)state;(void)aux;assert(source->handle==expected->source);++inspect_calls;*out=*expected;
 if(qos_drift)out->actual_qos[0].depth++;return 0;
}
static uint32_t retire(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_c_native_endpoint const*expected){
 (void)state;(void)aux;assert(source->handle==expected->source);++retires;retire_order=++order;return 0;
}
static uint32_t submit(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,
 roswasmc_capability const*observer,void const*bytes,size_t length,uint64_t*ticket){
 (void)state;assert(e->source==source->handle&&observer->handle);assert(length==16&&length<=sizeof submitted);
 memcpy(submitted,bytes,length);submitted_length=length;++submits;*ticket=next_ticket++;event_ticket=*ticket;return 0;
}
static uint32_t respond(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*advertise,
 roswasmc_capability const*reply,uint64_t ticket,void const*bytes,size_t length){
 (void)state;assert(e->source==advertise->handle&&reply->handle&&ticket==500);assert(length==8&&bytes);++replies;return reply_error;
}
static void native_info(rmw_service_info_t*out){*out=(rmw_service_info_t){0};out->source_timestamp=101;out->received_timestamp=102;
 memset(out->request_id.writer_guid,0x77,RMW_GID_STORAGE_SIZE);out->request_id.sequence_number=1234;
}
static uint32_t response(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*observer,
 roswasmc_capability const*source,void*bytes,size_t capacity,size_t*length,rmw_service_info_t*info,unsigned*present){
 (void)state;assert(observer->handle&&source->handle==e->source);if(!pending_response)return 11;
 size_t body=event_kind>=6?0:response_bytes;if(capacity<64+body)return 90;unsigned char*p=bytes;memset(p,0,64+body);
 w64(p,UINT64_C(0x0003000145525755));p[6]=(unsigned char)event_kind;p[8]=2;p[9]=(unsigned char)event_flags;
 p[12]=(unsigned char)event_error;w64(p+16,source->handle);w64(p+24,event_ticket);w64(p+32,event_sequence++);
 w64(p+40,e->instance);w64(p+48,e->session);w64(p+56,body);if(body>=8)w64(p+64,42);
 *length=64+body;*present=event_kind==3;native_info(info);pending_response=0;++events;return 0;
}
static uint32_t request(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*advertise,
 void*bytes,size_t capacity,size_t*length,uint64_t*ticket,rmw_service_info_t*info){
 (void)state;assert(advertise->handle==e->source);if(!pending_request)return 11;if(capacity<16)return 90;
 w64(bytes,4);w64((unsigned char*)bytes+8,5);*length=16;*ticket=500;native_info(info);pending_request=0;++events;return 0;
}
static uint32_t topic(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,
 void*bytes,size_t capacity,size_t*length,rmw_message_info_t*info){
 (void)state;assert(source->handle==e->source);if(!topic_pending)return 11;if(capacity<7)return 90;
 unsigned char*p=bytes;p[0]=3;p[1]=p[2]=p[3]=0;memcpy(p+4,"abc",3);*length=7;*info=(rmw_message_info_t){0};
 memset(info->publisher_gid.data,0x55,RMW_GID_STORAGE_SIZE);info->source_timestamp=201;info->received_timestamp=202;
 info->publication_sequence_number=17;info->reception_sequence_number=23;topic_pending=0;++events;return 0;
}
static uint32_t wait_slice(void*state,uint64_t ns){(void)state;assert(ns&&ns<=50);++wait_calls;clock_value+=rollback_clock?-1:(int64_t)ns;return 0;}
/* Regression variants wrap the original fake transport rather than changing
 * its 27 original groups. These are deliberately logical MODEL facts, not
 * actual native metadata. Zero values are accepted only with complete explicit
 * callback success/presence and the original binding/extent checks. */
static int zero_gid_drift;
static uint32_t activate_zero_gid(void*state,enum uwvm_rmw_endpoint_kind kind,roswasmc_capability const*source,
 roswasmc_capability const*aux,uwvm_rmw_codec const*codec,rmw_qos_profile_t const*requested,uwvm_rmw_c_native_endpoint*out){
 uint32_t raw=activate(state,kind,source,aux,codec,requested,out);
 if(!raw){out->native_gid_bytes=RMW_GID_STORAGE_SIZE;memset(out->native_gid,0,sizeof out->native_gid);}return raw;
}
static uint32_t inspect_zero_gid(void*state,roswasmc_capability const*source,roswasmc_capability const*aux,
 uwvm_rmw_c_native_endpoint const*expected,uwvm_rmw_c_native_endpoint*out){
 uint32_t raw=inspect(state,source,aux,expected,out);if(!raw&&zero_gid_drift)out->native_gid[0]=1;return raw;
}
static uint32_t topic_zero_gid(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,
 void*bytes,size_t capacity,size_t*length,rmw_message_info_t*info){
 uint32_t raw=topic(state,e,source,bytes,capacity,length,info);if(!raw)memset(info->publisher_gid.data,0,RMW_GID_STORAGE_SIZE);return raw;
}
static uint32_t request_zero_writer(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,
 void*bytes,size_t capacity,size_t*length,uint64_t*ticket,rmw_service_info_t*info){
 uint32_t raw=request(state,e,source,bytes,capacity,length,ticket,info);if(!raw)memset(info->request_id.writer_guid,0,RMW_GID_STORAGE_SIZE);return raw;
}
static uint32_t response_zero_writer(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*observer,
 roswasmc_capability const*source,void*bytes,size_t capacity,size_t*length,rmw_service_info_t*info,unsigned*present){
 uint32_t raw=response(state,e,observer,source,bytes,capacity,length,info,present);if(!raw)memset(info->request_id.writer_guid,0,RMW_GID_STORAGE_SIZE);return raw;
}
static uint32_t response_absent_info(void*state,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*observer,
 roswasmc_capability const*source,void*bytes,size_t capacity,size_t*length,rmw_service_info_t*info,unsigned*present){
 uint32_t raw=response(state,e,observer,source,bytes,capacity,length,info,present);if(!raw)*present=0;return raw;
}
static uint32_t wait_second_slice(void*state,uint64_t ns){
 (void)state;assert(ns&&ns<=UINT64_C(1000000007));++wait_calls;clock_value+=(int64_t)ns;return 0;
}
/* Explicit LOCAL provider model only; no DDS/native/query permission proof.
 * Actual SDK objects and the B/C owner/scope implementation remain real. */
static uint32_t query(void*state,uwvm_rmw_c_native_endpoint const*e,
 roswasmc_capability const*source,roswasmc_capability const*availability,unsigned*out){
 (void)state;assert(e&&source&&availability&&out&&e->source==source->handle);
 assert(source->context==availability->context&&source->handle!=availability->handle);
 ++query_calls;if(query_error)return query_error;if(!query_unwritten)*out=query_answer;return 0;
}
static void setup(void){
 qos=rmw_qos_profile_default;qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;qos.depth=10;qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;
 qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;qos.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
 rosidl_message_type_support_t const*str=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);
 assert(str&&str->get_type_hash_func);rosidl_type_hash_t const*h=str->get_type_hash_func(str);assert(h&&h->version==1);
 assert(!uwvm_rmw_string_codec_init(codecs,str,h->value,128));
 rosidl_message_type_support_t const*req=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Request),*res=ROSIDL_GET_MSG_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts_Response);
 rosidl_service_type_support_t const*srv=ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts);
 uint8_t hashes[3][32];memcpy(hashes[0],req->get_type_hash_func(req)->value,32);memcpy(hashes[1],res->get_type_hash_func(res)->value,32);memcpy(hashes[2],srv->get_type_hash_func(srv)->value,32);
 /* Unit-model selection obtains actual hashes from this actual linked SDK only.
  * This is NOT independent deployment identity qualification or native export. */
 assert(!uwvm_rmw_c_add_two_ints_codecs_init(codecs+1,hashes));
 static uwvm_rmw_node_selection nodes[]={{"model","/"}};
 endpoints[0]=(uwvm_rmw_endpoint_selection){0,0,UWVM_RMW_SUBSCRIPTION,"/model/string",qos,qos,{"sub",NULL},1};
 endpoints[1]=(uwvm_rmw_endpoint_selection){0,3,UWVM_RMW_CLIENT,"/model/add",qos,qos,{"call","observe"},2};
 endpoints[2]=(uwvm_rmw_endpoint_selection){0,3,UWVM_RMW_SERVICE,"/model/add",qos,qos,{"advertise","respond"},2};
 selection=(uwvm_rmw_profile){.version=1,.backend=&backend,.nodes=nodes,.node_count=1,.codecs=codecs,.codec_count=4,.endpoints=endpoints,.endpoint_count=3,.graph=uwvm_rmw_c_graph_provider()};
 static uwvm_rmw_c_profile cp;cp.version=1;
 if(strcmp(group,"missing-native-provider")){
  cp.activation=(uwvm_rmw_c_activation_provider){1,NULL,activate,inspect,retire};cp.requests=(uwvm_rmw_c_request_provider){1,NULL,submit,respond};
  cp.metadata=(uwvm_rmw_c_native_metadata_provider){1,NULL,topic,request,response};
 }
 if(!strcmp(group,"measured-wait")||!strcmp(group,"clock-rollback"))cp.wait=(uwvm_rmw_c_wait_provider){1,NULL,wait_slice,1000,50,32};
 if(!strcmp(group,"global-gid-zero")){cp.activation.activate=activate_zero_gid;cp.activation.inspect=inspect_zero_gid;}
 if(!strcmp(group,"topic-zero-gid"))cp.metadata.take_topic=topic_zero_gid;
 if(!strcmp(group,"incoming-zero-writer"))cp.metadata.take_request=request_zero_writer;
 if(!strcmp(group,"response-zero-writer"))cp.metadata.take_response=response_zero_writer;
 if(!strcmp(group,"response-missing-presence"))cp.metadata.take_response=response_absent_info;
 if(!strcmp(group,"nonnormalized-wait-equivalent"))cp.wait=(uwvm_rmw_c_wait_provider){1,NULL,wait_second_slice,UINT64_C(1000000007),UINT64_C(1000000007),4};
 if(!strncmp(group,"availability-",13)){
  if(strcmp(group,"availability-default95"))endpoints[1].availability_alias="available";
  if(strcmp(group,"availability-provider95"))cp.availability=(uwvm_rmw_c_availability_provider){1,NULL,query};
  if(!strcmp(group,"availability-table-uniqueness")){
   for(size_t e=0;e<3;++e)for(size_t k=0;k<endpoints[e].alias_count;++k){
    endpoints[1].availability_alias=endpoints[e].aliases[k];assert(uwvm_rmw_install_profile(&selection)==RMW_RET_INVALID_ARGUMENT);rmw_reset_error();}
   endpoints[1].availability_alias="available";endpoints[2].availability_alias="unowned-server-query";
   assert(uwvm_rmw_install_profile(&selection)==RMW_RET_INVALID_ARGUMENT);rmw_reset_error();endpoints[2].availability_alias=NULL;
   /* A future ordinary alias also cannot collide with an earlier query row. */
   char const*saved=endpoints[2].aliases[0];endpoints[2].aliases[0]="available";
   assert(uwvm_rmw_install_profile(&selection)==RMW_RET_INVALID_ARGUMENT);rmw_reset_error();endpoints[2].aliases[0]=saved;
  }
 }
 assert(uwvm_rmw_c_install_profile(&cp)==RMW_RET_OK);assert(uwvm_rmw_install_profile(&selection)==RMW_RET_OK);
 rmw_init_options_t options=rmw_get_zero_initialized_init_options();rcutils_allocator_t a=rcutils_get_default_allocator();
 assert(rmw_init_options_init(&options,a)==RMW_RET_OK);options.instance_id=1;options.enclave=rcutils_strdup("/",a);assert(options.enclave);
 context=rmw_get_zero_initialized_context();assert(rmw_init(&options,&context)==RMW_RET_OK);assert(rmw_init_options_fini(&options)==RMW_RET_OK);
 node=rmw_create_node(&context,"model","/");assert(node);
}
static rmw_client_t*client(void){rmw_client_t*c=rmw_create_client(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos);assert(c);return c;}
static rmw_service_t*service(void){rmw_service_t*s=rmw_create_service(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos);assert(s);return s;}
static int64_t send(rmw_client_t*c){example_interfaces__srv__AddTwoInts_Request req={.a=10,.b=32};int64_t seq=-1;assert(rmw_send_request(c,&req,&seq)==RMW_RET_OK&&seq>0);req.a=900;assert(submitted_length==16&&uwvm_ros_load_u64(submitted)==10);return seq;}
static void finish(void){assert(rmw_destroy_node(node)==RMW_RET_OK);assert(rmw_shutdown(&context)==RMW_RET_OK);assert(rmw_context_fini(&context)==RMW_RET_OK);rmw_reset_error();}
static int all_zero(unsigned char const*data,size_t size){for(size_t i=0;i<size;++i)if(data[i])return 0;return 1;}
static void boundary_models(void){
 if(!strcmp(group,"global-gid-zero")){
  rmw_client_t*c=client();rmw_gid_t gid;memset(&gid,0xa5,sizeof gid);
  assert(rmw_get_gid_for_client(c,&gid)==RMW_RET_OK&&all_zero(gid.data,RMW_GID_STORAGE_SIZE));
  rmw_gid_t original=gid;zero_gid_drift=1;
  assert(rmw_get_gid_for_client(c,&gid)!=RMW_RET_OK&&!memcmp(&gid,&original,sizeof gid));
  zero_gid_drift=0;assert(rmw_destroy_client(node,c)==RMW_RET_OK);
 }else if(!strcmp(group,"topic-zero-gid")){
  rmw_subscription_options_t options=rmw_get_default_subscription_options();
  rmw_subscription_t*s=rmw_create_subscription(node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"/model/string",&qos,&options);assert(s);
  std_msgs__msg__String message;assert(std_msgs__msg__String__init(&message));topic_pending=1;
  rmw_message_info_t info;memset(&info,0xa5,sizeof info);bool taken=false;
  assert(rmw_take_with_info(s,&message,&taken,&info,NULL)==RMW_RET_OK&&taken&&message.data.size==3&&!memcmp(message.data.data,"abc",3));
  assert(all_zero(info.publisher_gid.data,RMW_GID_STORAGE_SIZE)&&info.source_timestamp==201&&info.received_timestamp==202&&info.publication_sequence_number==17&&info.reception_sequence_number==23);
  assert(rmw_destroy_subscription(node,s)==RMW_RET_OK);std_msgs__msg__String__fini(&message);
 }else if(!strcmp(group,"incoming-zero-writer")){
  rmw_service_t*s=service();pending_request=1;example_interfaces__srv__AddTwoInts_Request req={0};rmw_service_info_t info;bool taken=false;
  assert(rmw_take_request(s,&info,&req,&taken)==RMW_RET_OK&&taken&&req.a==4&&req.b==5&&info.request_id.sequence_number==1234&&all_zero(info.request_id.writer_guid,RMW_GID_STORAGE_SIZE));
  example_interfaces__srv__AddTwoInts_Response res={.sum=9};
  assert(rmw_send_response(s,&info.request_id,&res)==RMW_RET_OK&&replies==1);
  assert(rmw_send_response(s,&info.request_id,&res)!=RMW_RET_OK&&replies==1);
  assert(rmw_destroy_service(node,s)==RMW_RET_OK);
 }else if(!strcmp(group,"response-zero-writer")||!strcmp(group,"response-missing-presence")){
  rmw_client_t*c=client();int64_t sequence=send(c);pending_response=1;
  example_interfaces__srv__AddTwoInts_Response res={.sum=999};rmw_service_info_t info;memset(&info,0xa5,sizeof info);rmw_service_info_t original=info;bool taken=false;
  rmw_ret_t result=rmw_take_response(c,&info,&res,&taken);
  if(!strcmp(group,"response-zero-writer")){
   uwvm_rmw_c_service_diagnostics d;
   assert(result==RMW_RET_OK&&taken&&res.sum==42&&info.request_id.sequence_number==sequence);
   assert(uwvm_rmw_c_client_diagnostics(c,&d)==RMW_RET_OK&&d.actual_native_info_present&&d.actual_native_info.request_id.sequence_number==1234&&all_zero(d.actual_native_info.request_id.writer_guid,RMW_GID_STORAGE_SIZE));
  }else assert(result!=RMW_RET_OK&&!taken&&res.sum==999&&!memcmp(&info,&original,sizeof info)&&events==1);
  assert(rmw_destroy_client(node,c)==RMW_RET_OK);
 }else if(!strcmp(group,"nonnormalized-wait-equivalent")){
  rmw_wait_set_t*w=rmw_create_wait_set(&context,0);assert(w);
  uwvm_rmw_platform_profile pp={1,&backend,"system","steady",NULL,0};assert(!uwvm_rmw_platform_prepare(&pp));
  rmw_time_t normalized={1,7},nonnormalized={0,UINT64_C(1000000007)};
  assert(rmw_wait(NULL,NULL,NULL,NULL,NULL,w,&normalized)==RMW_RET_TIMEOUT&&clock_value==INT64_C(1000000107)&&wait_calls==1);
  assert(rmw_wait(NULL,NULL,NULL,NULL,NULL,w,&nonnormalized)==RMW_RET_TIMEOUT&&clock_value==INT64_C(2000000114)&&wait_calls==2&&clock_reads>=6);
  assert(!uwvm_rmw_platform_close());assert(rmw_destroy_wait_set(w)==RMW_RET_OK);
 }else{
  rmw_guard_condition_t*g=rmw_create_guard_condition(&context);rmw_wait_set_t*w=rmw_create_wait_set(&context,1);assert(g&&w);assert(rmw_trigger_guard_condition(g)==RMW_RET_OK);
  void*entries[]={g->data};rmw_guard_conditions_t list={1,entries};
  rmw_time_t durations[]={{0,UINT64_C(1000000007)},{UINT64_C(9223372036),UINT64_C(854775807)},{UINT64_C(9223372036),UINT64_C(854775808)},{UINT64_MAX,UINT64_MAX}};
  size_t count=!strcmp(group,"nonnormalized-no-driver")?1:sizeof durations/sizeof*durations;
  for(size_t i=0;i<count;++i)assert(rmw_wait(NULL,&list,NULL,NULL,NULL,w,durations+i)==RMW_RET_UNSUPPORTED&&entries[0]==g->data&&clock_reads==0&&wait_calls==0);
  rmw_time_t zero={0,0};assert(rmw_wait(NULL,&list,NULL,NULL,NULL,w,&zero)==RMW_RET_OK&&entries[0]==g->data);
  assert(rmw_destroy_wait_set(w)==RMW_RET_OK);assert(rmw_destroy_guard_condition(g)==RMW_RET_OK);
 }
}
static void availability_models(void){
 unsigned before=opens;
 if(!strcmp(group,"availability-provider95")){
  assert(!rmw_create_client(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos)&&opens==before&&!activations);return;
 }
 if(!strcmp(group,"availability-capacity-before-open")){
  uwvm_rmw_context*c=uwvm_rmw_context_find(&context,0);assert(c);size_t saved=c->session.count;c->session.count=UWVM_RMW_MAX_OWNERS-2;
  assert(!rmw_create_client(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos)&&opens==before&&!activations);
  c->session.count=saved;return;
 }
 if(!strcmp(group,"availability-third-open-denied")||!strcmp(group,"availability-activation-failure")){
  if(!strcmp(group,"availability-activation-failure"))activation_error=13;
  assert(!rmw_create_client(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos));
  assert(opens==before+3&&closes==(!strcmp(group,"availability-third-open-denied")?2u:3u));
  assert(activations==(!strcmp(group,"availability-third-open-denied")?0u:1u));return;
 }
 rmw_client_t*c=client();bool value=true;unsigned need=!strcmp(group,"availability-default95")?2u:3u;
 assert(opens==before+need&&activations==1&&submits==0);
 if(!strcmp(group,"availability-default95")){assert(rmw_service_server_is_available(node,c,&value)==RMW_RET_UNSUPPORTED&&value&&query_calls==0);}
 else if(!strcmp(group,"availability-foreign-node")){
  rmw_node_t*other=rmw_create_node(&context,"model","/");assert(other&&other!=node);
  assert(rmw_service_server_is_available(other,c,&value)==RMW_RET_INVALID_ARGUMENT&&value&&!query_calls);assert(rmw_destroy_node(other)==RMW_RET_OK);
 }else if(!strcmp(group,"availability-context-busy")){
  uwvm_rmw_scope scope;assert(uwvm_rmw_scope_enter(uwvm_rmw_context_find(&context,0),&scope,0)==RMW_RET_OK);
  assert(rmw_service_server_is_available(node,c,&value)!=RMW_RET_OK&&value&&!query_calls);uwvm_rmw_scope_leave(&scope);
 }else if(!strcmp(group,"availability-source-drift")){
  qos_drift=1;assert(rmw_service_server_is_available(node,c,&value)!=RMW_RET_OK&&value&&!query_calls);qos_drift=0;
 }else if(!strcmp(group,"availability-query-denied")||!strcmp(group,"availability-query-revoked")){
  query_error=!strcmp(group,"availability-query-denied")?13:125;
  assert(rmw_service_server_is_available(node,c,&value)!=RMW_RET_OK&&value&&query_calls==1&&!submits);
 }else if(!strcmp(group,"availability-invalid-bool")||!strcmp(group,"availability-unwritten")){
  query_answer=2;query_unwritten=!strcmp(group,"availability-unwritten");
  assert(rmw_service_server_is_available(node,c,&value)==RMW_RET_INVALID_ARGUMENT&&value&&query_calls==1);
 }else if(!strcmp(group,"availability-shutdown-once")){
  assert(rmw_shutdown(&context)==RMW_RET_OK&&retires==1&&closes==3&&retire_order<close_order);
  assert(rmw_service_server_is_available(node,c,&value)!=RMW_RET_OK&&value&&!query_calls);
  assert(rmw_destroy_client(node,c)==RMW_RET_OK&&retires==1&&closes==3);return;
 }else if(!strcmp(group,"availability-close-errors")){
  assert(rmw_destroy_client(node,c)!=RMW_RET_OK&&closes==3&&retires==1&&retire_order<close_order);
  uwvm_rmw_diagnostics d;assert(uwvm_rmw_get_diagnostics(&context,&d)==RMW_RET_OK);
  assert(d.owner_close_attempted[2]&&d.owner_close_codes[2]==17&&d.owner_close_attempted[1]&&d.owner_close_codes[1]==0&&d.owner_close_attempted[0]&&d.owner_close_codes[0]==0);
  assert(closed_order[0]>closed_order[1]&&closed_order[1]>closed_order[2]);return;
 }else{
  query_answer=!strcmp(group,"availability-immediate-false")?0u:1u;value=!query_answer;
  assert(rmw_service_server_is_available(node,c,&value)==RMW_RET_OK&&value==(query_answer!=0)&&query_calls==1&&!submits);
 }
 assert(rmw_destroy_client(node,c)==RMW_RET_OK&&closes==need&&retires==1&&retire_order<close_order);
 assert(closed_order[0]>closed_order[1]);
}
static char const*groups[]={
 "missing-native-provider","guard-trigger-consumed","context-child-lifetime","foreign-wait-entry","indefinite-wait","missing-wait-driver",
 "measured-wait","clock-rollback","incoming-native-header","reply-once","post-take-upper-scope","activation-failure-cleanup",
 "topic-owned-info","codec-complete-extent","global-gid-missing","global-gid-wide","global-gid-complete","pending-cap-before-submit",
 "native-qos-drift","shutdown-retire-order","unknown-ticket","malformed-response","sequence-gap","closed-flags-preserved",
 "idle-output-preserved","client-copy-native-local","owned-prefetch",
 "global-gid-zero","topic-zero-gid","incoming-zero-writer","response-zero-writer","response-missing-presence","nonnormalized-wait-equivalent","nonnormalized-no-driver","duration-bounded-refusal",
 "availability-default95","availability-provider95","availability-immediate-true","availability-immediate-false","availability-query-denied","availability-query-revoked","availability-invalid-bool","availability-unwritten","availability-foreign-node","availability-context-busy","availability-source-drift","availability-capacity-before-open","availability-third-open-denied","availability-activation-failure","availability-shutdown-once","availability-close-errors","availability-table-uniqueness"};
int main(int argc,char**argv){if(argc!=2)return 2;group=argv[1];size_t admitted=0;
 for(;admitted<sizeof groups/sizeof*groups;++admitted)if(!strcmp(group,groups[admitted]))break;
 if(admitted==sizeof groups/sizeof*groups)return 2;setup();
 if(admitted>=35)availability_models();
 else if(admitted>=27)boundary_models();
 else if(!strcmp(group,"missing-native-provider")){unsigned before=opens;assert(!rmw_create_client(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos));assert(opens==before);}
 else if(!strcmp(group,"guard-trigger-consumed")){rmw_guard_condition_t*g=rmw_create_guard_condition(&context);rmw_wait_set_t*w=rmw_create_wait_set(&context,1);assert(g&&w);assert(rmw_trigger_guard_condition(g)==RMW_RET_OK);
  void*entries[]={g->data};rmw_guard_conditions_t guards={1,entries};rmw_time_t zero={0,0};assert(rmw_wait(NULL,&guards,NULL,NULL,NULL,w,&zero)==RMW_RET_OK&&entries[0]);
  entries[0]=g->data;assert(rmw_wait(NULL,&guards,NULL,NULL,NULL,w,&zero)==RMW_RET_TIMEOUT&&!entries[0]);assert(rmw_destroy_guard_condition(g)==RMW_RET_OK);assert(rmw_destroy_wait_set(w)==RMW_RET_OK);}
 else if(!strcmp(group,"context-child-lifetime")){rmw_wait_set_t*w=rmw_create_wait_set(&context,0);rmw_guard_condition_t*g=rmw_create_guard_condition(&context);assert(w&&g);assert(rmw_destroy_node(node)==RMW_RET_OK);node=NULL;
  assert(rmw_shutdown(&context)==RMW_RET_OK);assert(rmw_context_fini(&context)!=RMW_RET_OK);assert(rmw_destroy_wait_set(w)==RMW_RET_OK);assert(rmw_context_fini(&context)!=RMW_RET_OK);assert(rmw_destroy_guard_condition(g)==RMW_RET_OK);assert(rmw_context_fini(&context)==RMW_RET_OK);puts("PASS context-child-lifetime MODEL_ONLY");return 0;}
 else if(!strcmp(group,"foreign-wait-entry")||!strcmp(group,"indefinite-wait")||!strcmp(group,"missing-wait-driver")||!strcmp(group,"measured-wait")||!strcmp(group,"clock-rollback")){
  rmw_wait_set_t*w=rmw_create_wait_set(&context,1);assert(w);rmw_time_t duration={0,100};rmw_ret_t result;
  if(!strcmp(group,"foreign-wait-entry")){void*bad[]={(void*)(uintptr_t)1};rmw_guard_conditions_t guards={1,bad};rmw_time_t zero={0,0};assert(rmw_wait(NULL,&guards,NULL,NULL,NULL,w,&zero)!=RMW_RET_OK&&bad[0]==(void*)(uintptr_t)1);}
  else if(!strcmp(group,"indefinite-wait"))assert(rmw_wait(NULL,NULL,NULL,NULL,NULL,w,NULL)==RMW_RET_UNSUPPORTED);
  else if(!strcmp(group,"missing-wait-driver"))assert(rmw_wait(NULL,NULL,NULL,NULL,NULL,w,&duration)==RMW_RET_UNSUPPORTED);
  else{uwvm_rmw_platform_profile pp={1,&backend,"system","steady",NULL,0};assert(!uwvm_rmw_platform_prepare(&pp));rollback_clock=!strcmp(group,"clock-rollback");
   result=rmw_wait(NULL,NULL,NULL,NULL,NULL,w,&duration);if(rollback_clock)assert(result!=RMW_RET_OK&&result!=RMW_RET_TIMEOUT);else assert(result==RMW_RET_TIMEOUT&&clock_value==200&&wait_calls==2&&clock_reads>=3);
   assert(!uwvm_rmw_platform_close());}
  assert(rmw_destroy_wait_set(w)==RMW_RET_OK);
 }
 else if(!strcmp(group,"incoming-native-header")||!strcmp(group,"reply-once")||!strcmp(group,"post-take-upper-scope")){
  rmw_service_t*s=service();pending_request=1;example_interfaces__srv__AddTwoInts_Request req={0};rmw_service_info_t h;bool taken=false;
  assert(rmw_take_request(s,&h,&req,&taken)==RMW_RET_OK&&taken&&req.a==4&&req.b==5&&h.request_id.sequence_number==1234&&h.request_id.writer_guid[0]==0x77);
  if(!strcmp(group,"post-take-upper-scope")){rmw_guard_condition_t*g=rmw_create_guard_condition(&context);assert(g);assert(rmw_destroy_guard_condition(g)==RMW_RET_OK);}
  example_interfaces__srv__AddTwoInts_Response res={.sum=9};reply_error=!strcmp(group,"reply-once")?110:0;rmw_ret_t rc=rmw_send_response(s,&h.request_id,&res);
  assert((reply_error&&rc!=RMW_RET_OK)||(!reply_error&&rc==RMW_RET_OK));assert(replies==1);assert(rmw_send_response(s,&h.request_id,&res)!=RMW_RET_OK&&replies==1);assert(rmw_destroy_service(node,s)==RMW_RET_OK);
 }
 else if(!strcmp(group,"activation-failure-cleanup")){activation_error=13;unsigned before=closes;assert(!rmw_create_client(node,ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),"/model/add",&qos));assert(closes==before+2);}
 else if(!strcmp(group,"topic-owned-info")){
  rmw_subscription_options_t options=rmw_get_default_subscription_options();rmw_subscription_t*s=rmw_create_subscription(node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"/model/string",&qos,&options);assert(s);
  std_msgs__msg__String message;assert(std_msgs__msg__String__init(&message));topic_pending=1;rmw_message_info_t info;bool taken=false;
  assert(rmw_take_with_info(s,&message,&taken,&info,NULL)==RMW_RET_OK&&taken&&message.data.size==3&&!memcmp(message.data.data,"abc",3));
  assert(info.publication_sequence_number==17&&info.reception_sequence_number==23&&info.publisher_gid.data[0]==0x55&&!info.from_intra_process);
  assert(rmw_destroy_subscription(node,s)==RMW_RET_OK);assert(!memcmp(message.data.data,"abc",3));std_msgs__msg__String__fini(&message);
 }
 else if(!strcmp(group,"codec-complete-extent")){
  example_interfaces__srv__AddTwoInts_Response out={.sum=999};unsigned char bad[9]={0};assert(codecs[2].decode(bad,7,&out)==22&&out.sum==999);assert(codecs[2].decode(bad,9,&out)==22&&out.sum==999);
 }
 else{
  if(!strcmp(group,"global-gid-complete")){missing_gid=0;}if(!strcmp(group,"global-gid-wide")){missing_gid=0;wide_gid=1;}
  rmw_client_t*c=client();
  if(!strcmp(group,"global-gid-missing")||!strcmp(group,"global-gid-wide")||!strcmp(group,"global-gid-complete")){
   rmw_gid_t gid;memset(&gid,0xa5,sizeof gid);rmw_gid_t old=gid;rmw_ret_t rc=rmw_get_gid_for_client(c,&gid);
   if(!strcmp(group,"global-gid-complete"))assert(rc==RMW_RET_OK&&gid.data[0]==0x44);else assert(rc==RMW_RET_UNSUPPORTED&&!memcmp(&old,&gid,sizeof gid));
  }else if(!strcmp(group,"pending-cap-before-submit")){for(unsigned i=0;i<64;++i)(void)send(c);unsigned before=submits;example_interfaces__srv__AddTwoInts_Request req={0};int64_t seq=-1;assert(rmw_send_request(c,&req,&seq)!=RMW_RET_OK&&seq==-1&&submits==before);}
  else if(!strcmp(group,"native-qos-drift")){qos_drift=1;rmw_qos_profile_t actual=qos;assert(rmw_client_request_publisher_get_actual_qos(c,&actual)!=RMW_RET_OK&&actual.depth==qos.depth);assert(submits==0);qos_drift=0;}
  else if(!strcmp(group,"shutdown-retire-order")){assert(rmw_shutdown(&context)==RMW_RET_OK&&retires==1&&retire_order<close_order);unsigned before=closes;assert(rmw_destroy_client(node,c)==RMW_RET_OK&&closes==before&&retires==1);c=NULL;}
  else{
   int64_t seq=send(c);if(!strcmp(group,"unknown-ticket"))event_ticket+=100;
   if(!strcmp(group,"malformed-response"))response_bytes=7;if(!strcmp(group,"sequence-gap"))event_sequence=2;
   if(!strcmp(group,"closed-flags-preserved")){event_kind=7;event_flags=3;event_error=125;}
   example_interfaces__srv__AddTwoInts_Response res={.sum=999};rmw_service_info_t h;memset(&h,0xa5,sizeof h);bool taken=false;
   if(!strcmp(group,"idle-output-preserved")){assert(rmw_take_response(c,&h,&res,&taken)==RMW_RET_OK&&!taken&&res.sum==999&&events==0);}
   else{pending_response=1;
    if(!strcmp(group,"owned-prefetch")){
     rmw_wait_set_t*w=rmw_create_wait_set(&context,1);assert(w);void*entry[]={c->data};rmw_clients_t list={1,entry};rmw_time_t zero={0,0};
     assert(rmw_wait(NULL,NULL,NULL,&list,NULL,w,&zero)==RMW_RET_OK&&entry[0]&&events==1);
     assert(rmw_destroy_wait_set(w)==RMW_RET_OK);
    }
    rmw_ret_t rc=rmw_take_response(c,&h,&res,&taken);
    if(!strcmp(group,"client-copy-native-local")||!strcmp(group,"owned-prefetch")){
     assert(rc==RMW_RET_OK&&taken&&res.sum==42&&h.request_id.sequence_number==seq&&h.source_timestamp==101&&h.received_timestamp==102&&events==1);
     uwvm_rmw_c_service_diagnostics d;assert(uwvm_rmw_c_client_diagnostics(c,&d)==RMW_RET_OK&&d.actual_native_info_present&&d.actual_native_info.request_id.sequence_number==1234&&d.gateway_ticket==event_ticket&&d.local_sequence==seq);
    }else{assert(rc!=RMW_RET_OK&&!taken&&res.sum==999);if(!strcmp(group,"closed-flags-preserved")){uwvm_rmw_c_service_diagnostics d;assert(uwvm_rmw_c_client_diagnostics(c,&d)==RMW_RET_OK&&d.event_flags==3&&d.terminal_error==125);}}
   }
  }
  if(c)assert(rmw_destroy_client(node,c)==RMW_RET_OK);
 }
 finish();printf("PASS %s MODEL_ONLY_NOT_NATIVE_TRANSPORT\n",group);return 0;
}
