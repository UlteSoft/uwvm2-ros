/* SPDX-License-Identifier: Apache-2.0
 * Actual original-RMW composition with a real finite Preview1 timer provider.
 * Every ROS owner, WASI import and monotonic permission is selected explicitly
 * outside this program. Real A steady-clock observations define elapsed time;
 * poll returns, loop counts and native service readiness never substitute time.
 * Independent native String/Empty/Add peers are required, not simulated here.
 */
#include "../../c_codecs/examples/common.h"
#include <uwvm_rmw/wasi_wait.h>
#include <uwvm_rmw_platform.h>
#include <limits.h>
static uwvm_rmw_profile profile;static uwvm_rmw_c_profile c_profile;
static uwvm_rmw_node_selection selected_node;static uwvm_rmw_endpoint_selection endpoints[4];
static uwvm_rmw_wasi_wait*wait_transport;
static uwvm_rmw_subscription_transport*sub_transport;static uwvm_rmw_publisher_transport*pub_transport;static uwvm_rmw_service_transport*service_transport;
int main(int argc,char**argv){
 int failed=1,options_live=0,context_live=0,message_live=0,platform_prepared=0,platform_retained=0,quiescent=1;unsigned cleanup=0;uint32_t raw=0;size_t domain=0,count=0;uwvm_rmw_codec const*codecs=NULL;
 rcutils_allocator_t a=rcutils_get_default_allocator();rmw_context_t context=rmw_get_zero_initialized_context();rmw_init_options_t options=rmw_get_zero_initialized_init_options();
 rmw_wait_set_t*waitset=NULL;rmw_node_t*node=NULL;rmw_publisher_t*pub=NULL;rmw_subscription_t*sub=NULL;rmw_service_t*server=NULL;rmw_client_t*client=NULL;
 std_msgs__msg__String message={0};example_values empty={0},add={0};
 static char const expected[7]={'w','a','s','m','\0','r','o'};
 if(argc!=25||argv[7][0]!='/'||argv[8][0]!='/'||argv[9][0]!='/'||!example_decimal(argv[10],&domain)||!argv[11][0]||!argv[12][0]||!argv[13][0]){
 fputs("Usage: wasi_wait_original_rmw pub sub advertise respond call observe /topic /empty /add domain system-clock-alias steady-clock-alias query-alias-or-- 11-independent-native-rihs01-hex64\n",stderr);goto finish;}
 for(size_t i=1;i<=6;++i)if(!strcmp(argv[i],argv[11])||!strcmp(argv[i],argv[12])||(!strcmp(argv[13],"-")?0:!strcmp(argv[i],argv[13])))goto finish;
 if(!strcmp(argv[11],argv[12])||(!strcmp(argv[13],"-")?0:(!strcmp(argv[13],argv[11])||!strcmp(argv[13],argv[12]))))goto finish;
 for(size_t i=1;i<=6;++i){if(!argv[i][0])goto finish;for(size_t j=1;j<i;++j)if(!strcmp(argv[i],argv[j]))goto finish;}
 if(!strcmp(argv[8],argv[9]))goto finish;
 raw=example_prepare((char const*const*)(argv+14),&codecs,&count);if(raw)goto finish;
 /* Platform prepare opens ONLY the two explicitly selected ROS clock aliases.
  * It never admits the distinct WASI import or monotonic-clock permission. */
 uwvm_rmw_platform_profile clocks={1,roswasmc_ros2_backend(),argv[11],argv[12],NULL,0};
 raw=uwvm_rmw_platform_prepare(&clocks);if(raw)goto finish;platform_prepared=1;
 raw=uwvm_rmw_platform_retain();if(raw)goto finish;platform_retained=1;
 raw=uwvm_rmw_wasi_wait_create(UINT64_C(5000000000),UINT64_C(100000000),65536,&a,&wait_transport);if(raw)goto finish;
 raw=uwvm_rmw_wasi_wait_callbacks(wait_transport,&c_profile.wait);if(raw)goto finish;
 raw=example_values_init(&empty,false);if(raw)goto finish;raw=example_values_init(&add,true);if(raw)goto finish;
 if(!std_msgs__msg__String__init(&message))goto finish;message_live=1;if(!rosidl_runtime_c__String__assignn(&message.data,expected,sizeof expected))goto finish;
 raw=uwvm_rmw_publisher_transport_create(1,&a,&pub_transport);if(raw)goto finish;raw=uwvm_rmw_publisher_transport_callbacks(pub_transport,&profile.publisher);if(raw)goto finish;
 raw=uwvm_rmw_subscription_transport_create(1,1024,&a,&sub_transport);if(raw)goto finish;
 uwvm_rmw_c_activation_provider sub_activation={0};uwvm_rmw_c_native_metadata_provider sub_metadata={0};
 raw=uwvm_rmw_subscription_transport_callbacks(sub_transport,&sub_activation,&sub_metadata);if(raw)goto finish;
 raw=uwvm_rmw_service_transport_create(2,64,16,8,&a,&service_transport);if(raw)goto finish;
 raw=uwvm_rmw_service_transport_compose_subscription(service_transport,&sub_activation,&sub_metadata,&c_profile.activation,&c_profile.requests,&c_profile.metadata);if(raw)goto finish;c_profile.version=1;
 if(strcmp(argv[13],"-")){raw=uwvm_rmw_service_transport_availability_callbacks(service_transport,&c_profile.availability);if(raw)goto finish;}
 selected_node=(uwvm_rmw_node_selection){"checked_pure_c_composite","/"};rmw_qos_profile_t qos=example_qos();
 for(size_t i=0;i<4;++i){endpoints[i].node_index=0;endpoints[i].requested_qos=endpoints[i].effective_qos=qos;}
 endpoints[0].kind=UWVM_RMW_PUBLISHER;endpoints[0].codec_index=0;endpoints[0].resolved_name=argv[7];endpoints[0].aliases[0]=argv[1];endpoints[0].alias_count=1;
 endpoints[1].kind=UWVM_RMW_SUBSCRIPTION;endpoints[1].codec_index=0;endpoints[1].resolved_name=argv[7];endpoints[1].aliases[0]=argv[2];endpoints[1].alias_count=1;
 endpoints[2].kind=UWVM_RMW_SERVICE;endpoints[2].codec_index=UWVM_RMW_C_CODEC_EMPTY_SERVICE;endpoints[2].resolved_name=argv[8];endpoints[2].aliases[0]=argv[3];endpoints[2].aliases[1]=argv[4];endpoints[2].alias_count=2;
 endpoints[3].kind=UWVM_RMW_CLIENT;endpoints[3].codec_index=UWVM_RMW_C_CODEC_ADD_SERVICE;endpoints[3].resolved_name=argv[9];endpoints[3].aliases[0]=argv[5];endpoints[3].aliases[1]=argv[6];endpoints[3].alias_count=2;endpoints[3].availability_alias=strcmp(argv[13],"-")?argv[13]:NULL;
 profile.version=1;profile.domain_id=domain;profile.backend=roswasmc_ros2_backend();profile.nodes=&selected_node;profile.node_count=1;profile.codecs=codecs;profile.codec_count=count;profile.endpoints=endpoints;profile.endpoint_count=4;profile.graph=uwvm_rmw_c_graph_provider();
 if(uwvm_rmw_install_profile(&profile)!=RMW_RET_OK||uwvm_rmw_c_install_profile(&c_profile)!=RMW_RET_OK)goto finish;
 if(rmw_init_options_init(&options,a)!=RMW_RET_OK)goto finish;options_live=1;options.instance_id=1;options.domain_id=domain;options.enclave=rcutils_strdup("/",a);if(!options.enclave)goto finish;
 if(rmw_init(&options,&context)!=RMW_RET_OK)goto finish;context_live=1;quiescent=0;if(rmw_init_options_fini(&options)!=RMW_RET_OK)goto finish;options_live=0;
 node=rmw_create_node(&context,selected_node.name,selected_node.namespace_);if(!node)goto finish;
 rmw_publisher_options_t po=rmw_get_default_publisher_options();rmw_subscription_options_t so=rmw_get_default_subscription_options();
 pub=rmw_create_publisher(node,codecs[0].generated_support,argv[7],&qos,&po);if(!pub)goto finish;
 sub=rmw_create_subscription(node,codecs[0].generated_support,argv[7],&qos,&so);if(!sub)goto finish;
 server=rmw_create_service(node,codecs[UWVM_RMW_C_CODEC_EMPTY_SERVICE].generated_support,argv[8],&qos);if(!server)goto finish;
 client=rmw_create_client(node,codecs[UWVM_RMW_C_CODEC_ADD_SERVICE].generated_support,argv[9],&qos);if(!client)goto finish;
 waitset=rmw_create_wait_set(&context,3);if(!waitset)goto finish;
 /* Optional third query owner is explicit and observes this SAME real client.
  * False is actual data and does not cause hidden retries or graph-name waits. */
 if(endpoints[3].availability_alias){bool ready=false;
 if(rmw_service_server_is_available(node,client,&ready)!=RMW_RET_OK)goto finish;
 if(!ready){raw=11;goto finish;}}
 /* Empty-list original rmw_wait MUST measure a real 20ms A-clock interval.
  * This is an actual future Guest contract, not a timing claim from source. */
 int64_t empty_before=0,empty_after=0;
 raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&empty_before);if(raw)goto finish;
 rmw_time_t empty_timeout={0,UINT64_C(20000000)};
 if(rmw_wait(NULL,NULL,NULL,NULL,NULL,waitset,&empty_timeout)!=RMW_RET_TIMEOUT)goto finish;
 raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&empty_after);
 if(raw||empty_before<0||empty_after<empty_before||(uint64_t)(empty_after-empty_before)<UINT64_C(20000000))goto finish;
 if(rmw_publish(pub,&message,NULL)!=RMW_RET_OK)goto finish;example_set_request(&add);int64_t request_sequence=0;
 if(rmw_send_request(client,example_request(&add),&request_sequence)!=RMW_RET_OK||request_sequence<=0)goto finish;
 bool topic_taken=false,empty_taken=false,add_taken=false;rmw_message_info_t topic_info={0};rmw_service_info_t empty_info={0},add_info={0};
 int64_t started=0,previous=0,now=0;uint64_t until=0;
 raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&started);
 if(raw||started<0||(uint64_t)started>(uint64_t)INT64_MAX-UINT64_C(5000000000))goto finish;
 previous=started;until=(uint64_t)started+UINT64_C(5000000000);
 while(!(topic_taken&&empty_taken&&add_taken)){
  raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&now);
  if(raw||now<previous)goto finish;previous=now;
  if((uint64_t)now>=until){raw=110;goto finish;}
  uint64_t remaining=until-(uint64_t)now;if(remaining>UINT64_C(100000000))remaining=UINT64_C(100000000);
  rmw_time_t timeout={remaining/UINT64_C(1000000000),remaining%UINT64_C(1000000000)};
  /* ORIGINAL rmw_wait takes implementation .data addresses and sets unready
   * slots NULL. Rebuild every complete input list, never pass previous NULLs. */
  void*subscription_slots[1]={sub->data};void*server_slots[1]={server->data};void*client_slots[1]={client->data};
  rmw_subscriptions_t subscriptions={topic_taken?0:1,subscription_slots};
  rmw_services_t services={empty_taken?0:1,server_slots};rmw_clients_t clients={add_taken?0:1,client_slots};
  rmw_ret_t result=rmw_wait(&subscriptions,NULL,&services,&clients,NULL,waitset,&timeout);
  if(result!=RMW_RET_OK&&result!=RMW_RET_TIMEOUT)goto finish;
  if(result==RMW_RET_TIMEOUT)continue; /* Actual A-clock timeout, not a poll credit. */
  if(!topic_taken&&subscription_slots[0]){
   if(rmw_take_with_info(sub,&message,&topic_taken,&topic_info,NULL)!=RMW_RET_OK)goto finish;
   if(topic_taken&&(message.data.size!=sizeof expected||memcmp(message.data.data,expected,sizeof expected)))goto finish;
  }
  if(!empty_taken&&server_slots[0]){
   if(rmw_take_request(server,&empty_info,example_request(&empty),&empty_taken)!=RMW_RET_OK)goto finish;
   if(empty_taken){if(example_check_request(&empty))goto finish;example_set_response(&empty);
    if(rmw_send_response(server,&empty_info.request_id,example_response(&empty))!=RMW_RET_OK)goto finish;}
  }
  if(!add_taken&&client_slots[0]&&rmw_take_response(client,&add_info,example_response(&add),&add_taken)!=RMW_RET_OK)goto finish;
 }
 raw=uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&now);
 if(raw||now<previous||(uint64_t)now>=until){if(!raw)raw=110;goto finish;}
 if(!topic_taken||!empty_taken||!add_taken||add_info.request_id.sequence_number!=request_sequence||example_check_response(&add))goto finish;
 example_print_service_info(&empty_info);example_print_service_info(&add_info);
 uwvm_rmw_c_service_diagnostics d;if(uwvm_rmw_c_client_diagnostics(client,&d)!=RMW_RET_OK||!d.actual_native_info_present)goto finish;example_print_service_info(&d.actual_native_info);failed=0;
finish:
 /* Reverse actual endpoint acquisition. Each failed destroy is independent;
  * shutdown still retires remaining same-source Native and raw owners once. */
 if(waitset&&rmw_destroy_wait_set(waitset)!=RMW_RET_OK)++cleanup;
 if(client&&rmw_destroy_client(node,client)!=RMW_RET_OK)++cleanup;if(server&&rmw_destroy_service(node,server)!=RMW_RET_OK)++cleanup;
 if(sub&&rmw_destroy_subscription(node,sub)!=RMW_RET_OK)++cleanup;if(pub&&rmw_destroy_publisher(node,pub)!=RMW_RET_OK)++cleanup;
 if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup;cleanup+=example_context_diagnostics(&context);if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup;else{context_live=0;quiescent=1;}}
 if(options_live){if(rmw_init_options_fini(&options)!=RMW_RET_OK){++cleanup;quiescent=0;}else options_live=0;}
 if(message_live)std_msgs__msg__String__fini(&message);example_values_fini(&add);example_values_fini(&empty);
 cleanup+=example_service_diagnostics(service_transport);cleanup+=example_subscription_diagnostics(sub_transport);cleanup+=example_publisher_diagnostics(pub_transport);
 /* No C/provider/codec uninstall exists. Exported static/heap state remains
  * process-live even if setup or native cleanup failed. Do not fabricate free.
  * A clock owners close only after ALL genuine original RMW objects/options
  * are gone and B's immutable profile can actually be removed. */
 if(quiescent&&!options_live&&uwvm_rmw_install_profile(NULL)!=RMW_RET_OK){++cleanup;quiescent=0;}
 if(platform_retained&&quiescent){if(uwvm_rmw_platform_release())++cleanup;else platform_retained=0;}
 if(platform_prepared&&!platform_retained){if(uwvm_rmw_platform_close())++cleanup;}
 if(wait_transport){uwvm_rmw_wasi_wait_diagnostics facts;
  if(uwvm_rmw_wasi_wait_observe(wait_transport,&facts))++cleanup;
  else if(!failed&&(!facts.successful_events||facts.first_error))++cleanup;}
 if(raw)fprintf(stderr,"Actual wait composite bootstrap raw=%u\n",raw);
 if(failed||cleanup)return EXIT_FAILURE;puts("PASS_ORIGINAL_RMW_REAL_WASI_WAIT_COMPOSITE_REQUIRES_REAL_INDEPENDENT_PEERS");return EXIT_SUCCESS;
}
