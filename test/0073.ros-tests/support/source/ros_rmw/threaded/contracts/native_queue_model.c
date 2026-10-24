/* SPDX-License-Identifier: Apache-2.0
 * LOCAL transport models with actual generated String objects and actual
 * pthread threads. Native ids/times below are MODEL values, never DDS evidence.
 * Linux clock_gettime here tests real elapsed logic only; it is not Host policy.
 */
#define _POSIX_C_SOURCE 200809L
#include <platform/ros2/c/uwvm_rmw/internal.h>
#include <platform/ros2/c/uwvm_rmw/threaded.h>
#include <platform/ros2/c/uwvm_rmw_c/provider.h>
#include <platform/ros2/c/uwvm_rmw_platform.h>
#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string_functions.h>
#include <rosidl_runtime_c/type_hash.h>
#include <rmw/qos_profiles.h>
#include <rcutils/strdup.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sched.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#ifdef NDEBUG
#error "Active assertions are required"
#endif
#include <assert.h>
static atomic_uint opens,closes,native_retires,inspects,topic_calls,clock_calls,pinned_retire;
static atomic_int pending,retire_fault,close_fault,clock_regression;
static rmw_context_t context;static rmw_node_t*node;static rmw_subscription_t*subscription;static rmw_wait_set_t*set;
static uwvm_rmw_codec codec;static rmw_qos_profile_t qos;
static void store64(unsigned char*p,uint64_t v){for(unsigned i=0;i<8;++i)p[i]=(unsigned char)(v>>(8*i));}
static uint32_t abi(void){return 1;}
static uint32_t open_alias(uint32_t m,uint64_t n,uint64_t z,uint64_t out){assert(!m&&n&&z&&out);unsigned i=atomic_fetch_add(&opens,1)+1;store64((unsigned char*)(uintptr_t)out,UINT64_C(0x100000000)+i);return 0;}
static uint32_t close_alias(uint64_t h){assert(h);atomic_fetch_add(&closes,1);return atomic_load(&close_fault)?110:0;}
static uint32_t time_read(uint64_t h,uint32_t m,uint64_t input,uint64_t n,uint64_t out,uint64_t cap,uint64_t length){
 (void)input;assert(h&&!m&&!n&&cap==8);unsigned call=atomic_fetch_add(&clock_calls,1);struct timespec actual;
 assert(!clock_gettime(CLOCK_MONOTONIC,&actual));uint64_t ns=(uint64_t)actual.tv_sec*UINT64_C(1000000000)+(uint64_t)actual.tv_nsec;
 if(atomic_load(&clock_regression)&&call)ns=1;store64((unsigned char*)(uintptr_t)out,ns);store64((unsigned char*)(uintptr_t)length,8);return 0;
}
static roswasmc_backend const backend={.version=2,.abi_version=abi,.open=open_alias,.close=close_alias,
 .request={[ROSWASMC_TIME_READ_SYSTEM]=time_read,[ROSWASMC_TIME_READ_STEADY]=time_read}};
static uint32_t activate(void*state,enum uwvm_rmw_endpoint_kind kind,roswasmc_capability const*source,roswasmc_capability const*aux,
 uwvm_rmw_codec const*c,rmw_qos_profile_t const*requested,uwvm_rmw_c_native_endpoint*out){
 (void)state;assert(kind==UWVM_RMW_SUBSCRIPTION&&source&&source->handle&&!aux&&c==&codec);
 uwvm_rmw_c_native_endpoint value={.version=1,.backend=2,.source=source->handle,.instance=77,.runtime_generation=88,.session=99,
 .native_incarnation=1000,.qos_count=1,.native_gid_bytes=RMW_GID_STORAGE_SIZE};value.actual_qos[0]=*requested;*out=value;return 0;
}
static uint32_t inspect(void*s,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_c_native_endpoint const*expected,uwvm_rmw_c_native_endpoint*out){
 (void)s;assert(source->handle==expected->source&&!aux);atomic_fetch_add(&inspects,1);*out=*expected;return 0;}
static uint32_t retire(void*s,roswasmc_capability const*source,roswasmc_capability const*aux,uwvm_rmw_c_native_endpoint const*expected){
 (void)s;assert(source->handle==expected->source&&!aux);atomic_fetch_add(&native_retires,1);
 /* Trusted fixture reads the actual internal row while its caller genuinely
  * owns dispatch; this is not an application-supplied lease claim. */
 uwvm_rmw_context*c=uwvm_rmw_context_find(&context,1);assert(c&&c->leased);
 for(size_t i=0;i<c->resource_count;++i)if(c->resources[i].object==subscription&&c->resources[i].wait_pins)atomic_fetch_add(&pinned_retire,1);
 return atomic_load(&retire_fault)?107:0;
}
static uint32_t topic(void*s,uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,void*out,size_t cap,size_t*length,rmw_message_info_t*info){
 (void)s;assert(e->source==source->handle);atomic_fetch_add(&topic_calls,1);
 if(!atomic_load(&pending))return 11;if(cap<7)return 90;if(!atomic_exchange(&pending,0))return 11;
 unsigned char*p=out;p[0]=3;p[1]=p[2]=p[3]=0;memcpy(p+4,"A\0B",3);
 rmw_message_info_t value={0};value.source_timestamp=201;value.received_timestamp=202;
 value.publication_sequence_number=17;value.reception_sequence_number=23;*info=value;*length=7;return 0;
}
static uint32_t unused_driver(void*s,uint64_t ns){(void)s;(void)ns;assert(!"thread profile must use its real interruptible physical wait");return 5;}
static void setup(int positive){
 static uwvm_rmw_profile p;static uwvm_rmw_c_profile cp;static uwvm_rmw_node_selection n={"thread_model","/"};static uwvm_rmw_endpoint_selection ep;
 qos=rmw_qos_profile_default;qos.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
 rosidl_message_type_support_t const*support=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);assert(support&&support->get_type_hash_func);
 rosidl_type_hash_t const*h=support->get_type_hash_func(support);assert(h&&h->version==1);
 assert(!uwvm_rmw_string_codec_init(&codec,support,h->value,128));
 ep=(uwvm_rmw_endpoint_selection){0,0,UWVM_RMW_SUBSCRIPTION,"/thread/string",qos,qos,{"thread-sub",NULL},1};
 p=(uwvm_rmw_profile){.version=1,.backend=&backend,.nodes=&n,.node_count=1,.codecs=&codec,.codec_count=1,.endpoints=&ep,.endpoint_count=1,.graph=uwvm_rmw_c_graph_provider()};
 cp.version=1;cp.activation=(uwvm_rmw_c_activation_provider){1,NULL,activate,inspect,retire};cp.metadata=(uwvm_rmw_c_native_metadata_provider){.version=1,.take_topic=topic};
 if(positive)cp.wait=(uwvm_rmw_c_wait_provider){1,NULL,unused_driver,UINT64_C(1000000000),UINT64_C(1000000),10000};
 assert(uwvm_rmw_install_profile(&p)==RMW_RET_OK&&uwvm_rmw_c_install_profile(&cp)==RMW_RET_OK);
 rmw_init_options_t options=rmw_get_zero_initialized_init_options();rcutils_allocator_t a=rcutils_get_default_allocator();
 assert(rmw_init_options_init(&options,a)==RMW_RET_OK);options.instance_id=1;options.enclave=rcutils_strdup("/",a);assert(options.enclave);
 context=rmw_get_zero_initialized_context();assert(rmw_init(&options,&context)==RMW_RET_OK&&rmw_init_options_fini(&options)==RMW_RET_OK);
 node=rmw_create_node(&context,"thread_model","/");assert(node);rmw_subscription_options_t so=rmw_get_default_subscription_options();
 subscription=rmw_create_subscription(node,support,"/thread/string",&qos,&so);assert(subscription);set=rmw_create_wait_set(&context,1);assert(set);
}
typedef struct {rmw_ret_t result;void*slot;} wait_result;
static void*waiter(void*p){wait_result*r=p;r->slot=subscription;rmw_subscriptions_t list={1,&r->slot};r->result=rmw_wait(&list,NULL,NULL,NULL,NULL,set,NULL);return NULL;}
static void await_wait(void){for(;;){uwvm_rmw_threaded_diagnostics d;assert(uwvm_rmw_get_threaded_diagnostics(&context,&d)==RMW_RET_OK);if(d.active_waits)return;sched_yield();}}
static void finish(int sub_live){if(sub_live)assert(rmw_destroy_subscription(node,subscription)==RMW_RET_OK);
 assert(rmw_destroy_wait_set(set)==RMW_RET_OK&&rmw_destroy_node(node)==RMW_RET_OK&&rmw_shutdown(&context)==RMW_RET_OK&&rmw_context_fini(&context)==RMW_RET_OK);}
static void*parallel_clock(void*unused){(void)unused;for(unsigned i=0;i<200;++i){int64_t now=-1;assert(!uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&now)&&now>=0);}return NULL;}
static void*parallel_references(void*unused){(void)unused;for(unsigned i=0;i<200;++i){assert(!uwvm_rmw_platform_retain());assert(!uwvm_rmw_platform_release());}return NULL;}
static void*parallel_rmw(void*unused){(void)unused;for(unsigned i=0;i<200;++i){rmw_guard_condition_t*g=rmw_create_guard_condition(&context);assert(g);assert(rmw_destroy_guard_condition(g)==RMW_RET_OK);}return NULL;}
int main(int argc,char**argv){
 if(argc!=2)return 2;int positive=!strcmp(argv[1],"elapsed-timeout")||!strcmp(argv[1],"clock-regression");setup(positive);
 if(!strcmp(argv[1],"platform-parallel")){
  uwvm_rmw_platform_profile pp={1,&backend,"system","steady",NULL,0};assert(!uwvm_rmw_platform_prepare(&pp));pthread_t a,b,c;
  assert(!pthread_create(&a,NULL,parallel_clock,NULL)&&!pthread_create(&b,NULL,parallel_references,NULL)&&!pthread_create(&c,NULL,parallel_rmw,NULL));
  assert(!pthread_join(a,NULL)&&!pthread_join(b,NULL)&&!pthread_join(c,NULL));
  uwvm_rmw_platform_diagnostics d;assert(!uwvm_rmw_platform_get_diagnostics(&d)&&!d.lifetime_references&&d.clock_reads==200);
  assert(!uwvm_rmw_platform_close());finish(1);
 }else if(positive){uwvm_rmw_platform_profile pp={1,&backend,"system","steady",NULL,0};assert(!uwvm_rmw_platform_prepare(&pp));
  atomic_store(&clock_regression,!strcmp(argv[1],"clock-regression"));void*slot=subscription;rmw_subscriptions_t list={1,&slot};rmw_time_t wait={0,UINT64_C(5000000)};
  struct timespec first,last;assert(!clock_gettime(CLOCK_MONOTONIC,&first));rmw_ret_t result=rmw_wait(&list,NULL,NULL,NULL,NULL,set,&wait);assert(!clock_gettime(CLOCK_MONOTONIC,&last));
  if(atomic_load(&clock_regression))assert(result==RMW_RET_INVALID_ARGUMENT&&slot==subscription);
  else {uint64_t elapsed=(uint64_t)(last.tv_sec-first.tv_sec)*UINT64_C(1000000000)+(uint64_t)last.tv_nsec-(uint64_t)first.tv_nsec;
   assert(result==RMW_RET_TIMEOUT&&slot==NULL&&elapsed>=UINT64_C(5000000));}
  uwvm_rmw_platform_diagnostics pd;assert(!uwvm_rmw_platform_get_diagnostics(&pd)&&!pd.lifetime_references);assert(!uwvm_rmw_platform_close());finish(1);
 }else {wait_result r={0};pthread_t thread;assert(!pthread_create(&thread,NULL,waiter,&r));await_wait();
  if(!strcmp(argv[1],"queue-full-info")){atomic_store(&pending,1);assert(!pthread_join(thread,NULL)&&r.result==RMW_RET_OK&&r.slot==subscription);
   std_msgs__msg__String message;assert(std_msgs__msg__String__init(&message));bool taken=false;rmw_message_info_t info={0};
   assert(rmw_take_with_info(subscription,&message,&taken,&info,NULL)==RMW_RET_OK&&taken&&message.data.size==3&&!memcmp(message.data.data,"A\0B",3));
   assert(info.source_timestamp==201&&info.received_timestamp==202&&info.publication_sequence_number==17&&info.reception_sequence_number==23);
   std_msgs__msg__String__fini(&message);finish(1);
  }else if(!strcmp(argv[1],"retire-pinned-once")){atomic_store(&retire_fault,1);atomic_store(&close_fault,1);
   assert(rmw_destroy_subscription(node,subscription)==RMW_RET_ERROR);assert(!pthread_join(thread,NULL));
   assert(r.result!=RMW_RET_OK&&r.result!=RMW_RET_TIMEOUT&&atomic_load(&pinned_retire)==1);
   uwvm_rmw_diagnostics d;uwvm_rmw_threaded_diagnostics td;
   assert(uwvm_rmw_get_diagnostics(&context,&d)==RMW_RET_OK&&uwvm_rmw_get_threaded_diagnostics(&context,&td)==RMW_RET_OK);
   assert(d.native_retire_attempts==1&&d.native_retire_codes[0]==107&&d.owner_close_attempts==1&&d.owner_close_codes[0]==110&&!td.retired_local_pending);
   assert(atomic_load(&native_retires)==1&&atomic_load(&closes)==1);atomic_store(&retire_fault,0);atomic_store(&close_fault,0);finish(0);
  }else if(!strcmp(argv[1],"idle-shutdown")){assert(rmw_shutdown(&context)==RMW_RET_OK);assert(!pthread_join(thread,NULL));
   assert(r.result!=RMW_RET_OK&&r.result!=RMW_RET_TIMEOUT);finish(1);
  }else return 2;
 }
 puts("PASS_LOCAL_THREADED_CANONICAL_MODEL_NOT_NATIVE_ROS");return 0;
}
