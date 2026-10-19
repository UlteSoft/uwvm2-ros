/* SPDX-License-Identifier: Apache-2.0
 * Complete same-Context pure-C composition: genuine String publisher+subscriber,
 * Empty service and AddTwoInts client. The actual existing C9 and C14 provider
 * states coexist; B's publisher provider is separate. Four endpoint aliases
 * cannot secretly replace each other's callbacks. Every grant remains explicit.
 */
#include "common.h"
static uwvm_rmw_profile profile;static uwvm_rmw_c_profile c_profile;
static uwvm_rmw_node_selection selected_node;static uwvm_rmw_endpoint_selection endpoints[4];
static uwvm_rmw_subscription_transport*sub_transport;static uwvm_rmw_publisher_transport*pub_transport;static uwvm_rmw_service_transport*service_transport;
int main(int argc,char**argv){
 int failed=1,options_live=0,context_live=0,message_live=0;unsigned cleanup=0;uint32_t raw=0;size_t domain=0,count=0;uwvm_rmw_codec const*codecs=NULL;
 rcutils_allocator_t a=rcutils_get_default_allocator();rmw_context_t context=rmw_get_zero_initialized_context();rmw_init_options_t options=rmw_get_zero_initialized_init_options();
 rmw_node_t*node=NULL;rmw_publisher_t*pub=NULL;rmw_subscription_t*sub=NULL;rmw_service_t*server=NULL;rmw_client_t*client=NULL;
 std_msgs__msg__String message={0};example_values empty={0},add={0};
 static char const expected[7]={'w','a','s','m','\0','r','o'};
 if(argc!=22||argv[7][0]!='/'||argv[8][0]!='/'||argv[9][0]!='/'||!example_decimal(argv[10],&domain)){
 fputs("Usage: checked_composite_c pub-alias sub-alias advertise-alias respond-alias call-alias observe-alias /topic /empty /add domain 11-independent-native-rihs01-hex64\n",stderr);goto finish;}
 for(size_t i=1;i<=6;++i){if(!argv[i][0])goto finish;for(size_t j=1;j<i;++j)if(!strcmp(argv[i],argv[j]))goto finish;}
 if(!strcmp(argv[8],argv[9]))goto finish;
 raw=example_prepare((char const*const*)(argv+11),&codecs,&count);if(raw)goto finish;
 raw=example_values_init(&empty,false);if(raw)goto finish;raw=example_values_init(&add,true);if(raw)goto finish;
 if(!std_msgs__msg__String__init(&message))goto finish;message_live=1;if(!rosidl_runtime_c__String__assignn(&message.data,expected,sizeof expected))goto finish;
 raw=uwvm_rmw_publisher_transport_create(1,&a,&pub_transport);if(raw)goto finish;raw=uwvm_rmw_publisher_transport_callbacks(pub_transport,&profile.publisher);if(raw)goto finish;
 raw=uwvm_rmw_subscription_transport_create(1,1024,&a,&sub_transport);if(raw)goto finish;
 uwvm_rmw_c_activation_provider sub_activation={0};uwvm_rmw_c_native_metadata_provider sub_metadata={0};
 raw=uwvm_rmw_subscription_transport_callbacks(sub_transport,&sub_activation,&sub_metadata);if(raw)goto finish;
 raw=uwvm_rmw_service_transport_create(2,64,16,8,&a,&service_transport);if(raw)goto finish;
 raw=uwvm_rmw_service_transport_compose_subscription(service_transport,&sub_activation,&sub_metadata,&c_profile.activation,&c_profile.requests,&c_profile.metadata);if(raw)goto finish;c_profile.version=1;
 selected_node=(uwvm_rmw_node_selection){"checked_pure_c_composite","/"};rmw_qos_profile_t qos=example_qos();
 for(size_t i=0;i<4;++i){endpoints[i].node_index=0;endpoints[i].requested_qos=endpoints[i].effective_qos=qos;}
 endpoints[0].kind=UWVM_RMW_PUBLISHER;endpoints[0].codec_index=0;endpoints[0].resolved_name=argv[7];endpoints[0].aliases[0]=argv[1];endpoints[0].alias_count=1;
 endpoints[1].kind=UWVM_RMW_SUBSCRIPTION;endpoints[1].codec_index=0;endpoints[1].resolved_name=argv[7];endpoints[1].aliases[0]=argv[2];endpoints[1].alias_count=1;
 endpoints[2].kind=UWVM_RMW_SERVICE;endpoints[2].codec_index=UWVM_RMW_C_CODEC_EMPTY_SERVICE;endpoints[2].resolved_name=argv[8];endpoints[2].aliases[0]=argv[3];endpoints[2].aliases[1]=argv[4];endpoints[2].alias_count=2;
 endpoints[3].kind=UWVM_RMW_CLIENT;endpoints[3].codec_index=UWVM_RMW_C_CODEC_ADD_SERVICE;endpoints[3].resolved_name=argv[9];endpoints[3].aliases[0]=argv[5];endpoints[3].aliases[1]=argv[6];endpoints[3].alias_count=2;
 profile.version=1;profile.domain_id=domain;profile.backend=roswasmc_ros2_backend();profile.nodes=&selected_node;profile.node_count=1;profile.codecs=codecs;profile.codec_count=count;profile.endpoints=endpoints;profile.endpoint_count=4;profile.graph=uwvm_rmw_c_graph_provider();
 if(uwvm_rmw_install_profile(&profile)!=RMW_RET_OK||uwvm_rmw_c_install_profile(&c_profile)!=RMW_RET_OK)goto finish;
 if(rmw_init_options_init(&options,a)!=RMW_RET_OK)goto finish;options_live=1;options.instance_id=1;options.domain_id=domain;options.enclave=rcutils_strdup("/",a);if(!options.enclave)goto finish;
 if(rmw_init(&options,&context)!=RMW_RET_OK)goto finish;context_live=1;if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto finish;}options_live=0;
 node=rmw_create_node(&context,selected_node.name,selected_node.namespace_);if(!node)goto finish;
 rmw_publisher_options_t po=rmw_get_default_publisher_options();rmw_subscription_options_t so=rmw_get_default_subscription_options();
 pub=rmw_create_publisher(node,codecs[0].generated_support,argv[7],&qos,&po);if(!pub)goto finish;
 sub=rmw_create_subscription(node,codecs[0].generated_support,argv[7],&qos,&so);if(!sub)goto finish;
 server=rmw_create_service(node,codecs[UWVM_RMW_C_CODEC_EMPTY_SERVICE].generated_support,argv[8],&qos);if(!server)goto finish;
 client=rmw_create_client(node,codecs[UWVM_RMW_C_CODEC_ADD_SERVICE].generated_support,argv[9],&qos);if(!client)goto finish;
 if(rmw_publish(pub,&message,NULL)!=RMW_RET_OK)goto finish;example_set_request(&add);int64_t request_sequence=0;
 if(rmw_send_request(client,example_request(&add),&request_sequence)!=RMW_RET_OK||request_sequence<=0)goto finish;
 bool topic_taken=false,empty_taken=false,add_taken=false;rmw_message_info_t topic_info={0};rmw_service_info_t empty_info={0},add_info={0};
 for(size_t i=0;i<4096&&!(topic_taken&&empty_taken&&add_taken);++i){
 if(!topic_taken&&rmw_take_with_info(sub,&message,&topic_taken,&topic_info,NULL)!=RMW_RET_OK)goto finish;
 if(topic_taken&&(message.data.size!=sizeof expected||memcmp(message.data.data,expected,sizeof expected)))goto finish;
 if(!empty_taken){if(rmw_take_request(server,&empty_info,example_request(&empty),&empty_taken)!=RMW_RET_OK)goto finish;
 if(empty_taken){if(example_check_request(&empty))goto finish;example_set_response(&empty);if(rmw_send_response(server,&empty_info.request_id,example_response(&empty))!=RMW_RET_OK)goto finish;}}
 if(!add_taken&&rmw_take_response(client,&add_info,example_response(&add),&add_taken)!=RMW_RET_OK)goto finish;
 }
 if(!topic_taken||!empty_taken||!add_taken||add_info.request_id.sequence_number!=request_sequence||example_check_response(&add))goto finish;
 example_print_service_info(&empty_info);example_print_service_info(&add_info);
 uwvm_rmw_c_service_diagnostics d;if(uwvm_rmw_c_client_diagnostics(client,&d)!=RMW_RET_OK||!d.actual_native_info_present)goto finish;example_print_service_info(&d.actual_native_info);failed=0;
finish:
 /* Reverse actual endpoint acquisition. Each failed destroy is independent;
  * shutdown still retires remaining same-source Native and raw owners once. */
 if(client&&rmw_destroy_client(node,client)!=RMW_RET_OK)++cleanup;if(server&&rmw_destroy_service(node,server)!=RMW_RET_OK)++cleanup;
 if(sub&&rmw_destroy_subscription(node,sub)!=RMW_RET_OK)++cleanup;if(pub&&rmw_destroy_publisher(node,pub)!=RMW_RET_OK)++cleanup;
 if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup;cleanup+=example_context_diagnostics(&context);if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup;}
 if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup;
 if(message_live)std_msgs__msg__String__fini(&message);example_values_fini(&add);example_values_fini(&empty);
 cleanup+=example_service_diagnostics(service_transport);cleanup+=example_subscription_diagnostics(sub_transport);cleanup+=example_publisher_diagnostics(pub_transport);
 if(raw)fprintf(stderr,"Pure-C composite bootstrap raw=%u\n",raw);
 if(failed||cleanup)return EXIT_FAILURE;puts("PASS_ORIGINAL_RMW_PURE_C_COMPOSITE_REQUIRES_REAL_INDEPENDENT_PEERS");return EXIT_SUCCESS;
}
