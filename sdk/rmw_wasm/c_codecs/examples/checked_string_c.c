/* SPDX-License-Identifier: Apache-2.0
 * Complete original RMW pure-C String publisher OR subscription. The seven-byte
 * native value and eleven-byte canonical bytes retain their embedded NUL.
 * Actual opposite peers must already be genuinely coordinated by Root; no
 * publish marker or finite idle loop claims DDS match/delivery/native readiness.
 */
#include "common.h"
static uwvm_rmw_profile profile;static uwvm_rmw_c_profile c_profile;
static uwvm_rmw_node_selection selected_node;static uwvm_rmw_endpoint_selection endpoint;
static uwvm_rmw_subscription_transport*sub_transport;static uwvm_rmw_publisher_transport*pub_transport;
int main(int argc,char**argv){
 int failed=1,options_live=0,context_live=0,message_live=0,publish=0;unsigned cleanup=0;uint32_t raw=0;size_t domain=0,count=0;
 rmw_context_t context=rmw_get_zero_initialized_context();rmw_init_options_t options=rmw_get_zero_initialized_init_options();rcutils_allocator_t a=rcutils_get_default_allocator();
 rmw_node_t*node=NULL;rmw_publisher_t*pub=NULL;rmw_subscription_t*sub=NULL;std_msgs__msg__String message={0};uwvm_rmw_codec const*codecs=NULL;
 static char const expected[7]={'w','a','s','m','\0','r','o'};
 if(argc!=16||((publish=!strcmp(argv[1],"publish"))==0&&strcmp(argv[1],"subscribe"))||!argv[2][0]||argv[3][0]!='/'||!example_decimal(argv[4],&domain)){
 fputs("Usage: checked_string_c publish|subscribe exact-alias /topic domain-id 11-independent-native-rihs01-hex64\n",stderr);goto finish;}
 raw=example_prepare((char const*const*)(argv+5),&codecs,&count);if(raw)goto finish;
 if(!std_msgs__msg__String__init(&message))goto finish;message_live=1;
 if(publish&&!rosidl_runtime_c__String__assignn(&message.data,expected,sizeof expected))goto finish;
 if(publish){raw=uwvm_rmw_publisher_transport_create(1,&a,&pub_transport);if(raw)goto finish;raw=uwvm_rmw_publisher_transport_callbacks(pub_transport,&profile.publisher);}
 else {raw=uwvm_rmw_subscription_transport_create(1,1024,&a,&sub_transport);if(raw)goto finish;raw=uwvm_rmw_subscription_transport_callbacks(sub_transport,&c_profile.activation,&c_profile.metadata);}if(raw)goto finish;c_profile.version=1;
 selected_node=(uwvm_rmw_node_selection){publish?"checked_c_string_pub":"checked_c_string_sub","/"};rmw_qos_profile_t qos=example_qos();
 endpoint.node_index=0;endpoint.codec_index=UWVM_RMW_C_CODEC_STRING;endpoint.kind=publish?UWVM_RMW_PUBLISHER:UWVM_RMW_SUBSCRIPTION;
 endpoint.resolved_name=argv[3];endpoint.requested_qos=endpoint.effective_qos=qos;endpoint.aliases[0]=argv[2];endpoint.alias_count=1;
 profile.version=1;profile.domain_id=domain;profile.backend=roswasmc_ros2_backend();profile.nodes=&selected_node;profile.node_count=1;profile.codecs=codecs;profile.codec_count=count;profile.endpoints=&endpoint;profile.endpoint_count=1;profile.graph=uwvm_rmw_c_graph_provider();
 if(uwvm_rmw_install_profile(&profile)!=RMW_RET_OK||uwvm_rmw_c_install_profile(&c_profile)!=RMW_RET_OK)goto finish;
 if(rmw_init_options_init(&options,a)!=RMW_RET_OK)goto finish;options_live=1;options.instance_id=1;options.domain_id=domain;options.enclave=rcutils_strdup("/",a);if(!options.enclave)goto finish;
 if(rmw_init(&options,&context)!=RMW_RET_OK)goto finish;context_live=1;if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto finish;}options_live=0;
 node=rmw_create_node(&context,selected_node.name,selected_node.namespace_);if(!node)goto finish;
 rosidl_message_type_support_t const*support=codecs[0].generated_support;
 if(publish){rmw_publisher_options_t opts=rmw_get_default_publisher_options();pub=rmw_create_publisher(node,support,argv[3],&qos,&opts);if(!pub)goto finish;
 if(rmw_publish(pub,&message,NULL)!=RMW_RET_OK)goto finish;rmw_gid_t gid={0};if(rmw_get_gid_for_publisher(pub,&gid)!=RMW_RET_OK)goto finish;
 fputs("PUBLISHED_C_STRING bytes=7 genuine_gid=",stdout);for(size_t i=0;i<RMW_GID_STORAGE_SIZE;++i)printf("%02x",gid.data[i]);putchar('\n');
 }else{rmw_subscription_options_t opts=rmw_get_default_subscription_options();sub=rmw_create_subscription(node,support,argv[3],&qos,&opts);if(!sub)goto finish;
 bool taken=false;rmw_message_info_t info={0};for(size_t n=0;n<4096&&!taken;++n)if(rmw_take_with_info(sub,&message,&taken,&info,NULL)!=RMW_RET_OK)goto finish;
 if(!taken||message.data.size!=sizeof expected||memcmp(message.data.data,expected,sizeof expected)||info.from_intra_process||!info.publisher_gid.implementation_identifier)goto finish;
 printf("RECEIVED_C_STRING bytes=%zu source_time=%lld received_time=%lld publication=%llu reception=%llu gid=",message.data.size,(long long)info.source_timestamp,(long long)info.received_timestamp,(unsigned long long)info.publication_sequence_number,(unsigned long long)info.reception_sequence_number);
 for(size_t i=0;i<RMW_GID_STORAGE_SIZE;++i)printf("%02x",info.publisher_gid.data[i]);putchar('\n');}
 failed=0;
finish:
 if(pub&&rmw_destroy_publisher(node,pub)!=RMW_RET_OK)++cleanup;if(sub&&rmw_destroy_subscription(node,sub)!=RMW_RET_OK)++cleanup;
 if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup;cleanup+=example_context_diagnostics(&context);if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup;}
 if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup;
 if(message_live)std_msgs__msg__String__fini(&message);
 cleanup+=example_subscription_diagnostics(sub_transport);cleanup+=example_publisher_diagnostics(pub_transport);
 if(raw)fprintf(stderr,"Pure-C String bootstrap raw=%u\n",raw);
 /* Exported registry and borrowed immutable profiles/transports deliberately
  * retain process lifetime. Native/raw objects were independently retired. */
 if(failed||cleanup)return EXIT_FAILURE;puts("PASS_ORIGINAL_RMW_PURE_C_STRING_REQUIRES_INDEPENDENT_NATIVE_DELIVERY");return EXIT_SUCCESS;
}
