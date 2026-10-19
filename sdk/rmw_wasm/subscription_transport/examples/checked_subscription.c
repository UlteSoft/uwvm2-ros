/* SPDX-License-Identifier: Apache-2.0
 * Complete direct original-RMW C bootstrap: one selected real String subscription
 * only. No service/client/publisher row or ambient clock/log/environment grant.
 * This source is UNEXECUTED. CLI strings select existing explicit grants, never
 * create them. Actual native identity is an independent SDK export input.
 */
#include <uwvm_rmw/subscription_transport.h>
#include <roswasm/ros2.h>
#include <rcutils/strdup.h>
#include <rmw/qos_profiles.h>
#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uwvm_rmw_profile profile;static uwvm_rmw_c_profile c_profile;
static uwvm_rmw_node_selection selected_node;static uwvm_rmw_endpoint_selection selected_endpoint;
static uwvm_rmw_codec codec;static uwvm_rmw_subscription_transport*transport;
static int identity(char const*text,uint8_t out[32]){
 if(!text)return 0;unsigned nonzero=0;
 for(size_t i=0;i<64;++i){unsigned char c=(unsigned char)text[i];if(!c)return 0;unsigned n;
 if(c>='0'&&c<='9')n=c-'0';else if(c>='a'&&c<='f')n=c-'a'+10;else if(c>='A'&&c<='F')n=c-'A'+10;else return 0;
 if(i%2)out[i/2]|=(uint8_t)n;else out[i/2]=(uint8_t)(n<<4);nonzero|=n;}
 return !text[64]&&nonzero;
}
static int decimal(char const*text,size_t*out){if(!text||!*text)return 0;size_t v=0;for(size_t i=0;text[i];++i){if(i>20||text[i]<'0'||text[i]>'9')return 0;size_t d=(size_t)(text[i]-'0');if(v>(SIZE_MAX-d)/10)return 0;v=v*10+d;}*out=v;return 1;}
int main(int argc,char**argv){
 int failed=1,options_live=0,context_live=0,message_live=0;unsigned cleanup_errors=0;uint32_t raw=0;size_t domain;uint8_t hash[32];
 rcutils_allocator_t allocator=rcutils_get_default_allocator();rmw_context_t context=rmw_get_zero_initialized_context();rmw_init_options_t options=rmw_get_zero_initialized_init_options();
 rmw_node_t*node=NULL;rmw_subscription_t*subscription=NULL;std_msgs__msg__String message;memset(&message,0,sizeof message);
 if(argc!=5||!identity(argv[3],hash)||!decimal(argv[4],&domain)){fputs("Usage: checked_subscription exact-alias /topic independent-native-rihs01-hex64 domain-id\n",stderr);goto finish;}
 rosidl_message_type_support_t const*support=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);
 raw=uwvm_rmw_string_codec_init(&codec,support,hash,1024);if(raw)goto finish;
 raw=uwvm_rmw_subscription_transport_create(1,1024,&allocator,&transport);if(raw)goto finish;
 raw=uwvm_rmw_subscription_transport_callbacks(transport,&c_profile.activation,&c_profile.metadata);if(raw)goto finish;c_profile.version=1;
 selected_node=(uwvm_rmw_node_selection){"checked_rmw_subscription","/"};
 rmw_qos_profile_t qos={0};qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;qos.depth=4;qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;
 qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;qos.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
 qos.deadline=(rmw_time_t)RMW_DURATION_INFINITE;qos.lifespan=(rmw_time_t)RMW_DURATION_INFINITE;qos.liveliness_lease_duration=(rmw_time_t)RMW_DURATION_INFINITE;
 selected_endpoint.node_index=0;selected_endpoint.codec_index=0;selected_endpoint.kind=UWVM_RMW_SUBSCRIPTION;selected_endpoint.resolved_name=argv[2];
 selected_endpoint.requested_qos=selected_endpoint.effective_qos=qos;selected_endpoint.aliases[0]=argv[1];selected_endpoint.alias_count=1;
 profile.version=1;profile.domain_id=domain;profile.backend=roswasmc_ros2_backend();profile.nodes=&selected_node;profile.node_count=1;profile.codecs=&codec;profile.codec_count=1;
 profile.endpoints=&selected_endpoint;profile.endpoint_count=1;profile.graph=uwvm_rmw_c_graph_provider();
 if(uwvm_rmw_install_profile(&profile)!=RMW_RET_OK)goto finish;
 if(uwvm_rmw_c_install_profile(&c_profile)!=RMW_RET_OK)goto finish;
 // C configuration has no uninstall. These actual static profile/codec/state
 // objects and allocator remain alive until process exit, even after an error.
 if(!std_msgs__msg__String__init(&message))goto finish;message_live=1;
 if(rmw_init_options_init(&options,allocator)!=RMW_RET_OK)goto finish;options_live=1;options.instance_id=1;options.domain_id=domain;
 options.enclave=rcutils_strdup("/",allocator);if(!options.enclave)goto finish;
 if(rmw_init(&options,&context)!=RMW_RET_OK)goto finish;context_live=1;
 if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto finish;}options_live=0;
 node=rmw_create_node(&context,selected_node.name,selected_node.namespace_);if(!node)goto finish;
 rmw_subscription_options_t subscription_options=rmw_get_default_subscription_options();
 subscription=rmw_create_subscription(node,support,argv[2],&qos,&subscription_options);if(!subscription)goto finish;
 // A finite number of genuine zero-wait observations, NOT elapsed nanoseconds.
 // Positive waits need a separately qualified clock/wait provider; none is
 // synthesized. Root's external process/IPC deadline remains mandatory.
 bool taken=false;rmw_message_info_t info={0};
 for(size_t attempt=0;attempt!=4096&&!taken;++attempt)if(rmw_take_with_info(subscription,&message,&taken,&info,NULL)!=RMW_RET_OK)goto finish;
 static char const expected[7]={'w','a','s','m','\0','r','o'};
 if(!taken||message.data.size!=sizeof expected||memcmp(message.data.data,expected,sizeof expected)||info.from_intra_process||
 !info.publisher_gid.implementation_identifier)goto finish;
 // Native zero times/GIDs remain real data. The external native peer/report
 // must compare actual GID and available sequence/timestamp facts separately.
 printf("RECEIVED_STRING bytes=%zu source_time=%lld received_time=%lld publication=%llu reception=%llu gid=",message.data.size,
 (long long)info.source_timestamp,(long long)info.received_timestamp,(unsigned long long)info.publication_sequence_number,(unsigned long long)info.reception_sequence_number);
 for(size_t i=0;i<RMW_GID_STORAGE_SIZE;++i)printf("%02x",info.publisher_gid.data[i]);putchar('\n');failed=0;
finish:
 if(message_live)std_msgs__msg__String__fini(&message);
 if(subscription&&rmw_destroy_subscription(node,subscription)!=RMW_RET_OK)++cleanup_errors;
 if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup_errors;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup_errors;uwvm_rmw_diagnostics d;
 if(uwvm_rmw_get_diagnostics(&context,&d)!=RMW_RET_OK)++cleanup_errors;
 else{if(d.owner_close_attempted[0]&&transport)(void)uwvm_rmw_subscription_transport_record_raw_close(transport,0,d.owner_close_codes[0]);
 if(d.owner_close_failures||d.native_retire_failures||d.local_retirement_failures||(d.graph_close_attempted[0]&&d.graph_close_codes[0]))++cleanup_errors;}
 if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup_errors;}
 if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup_errors;
 if(transport){size_t count=0;if(uwvm_rmw_subscription_transport_count(transport,&count))++cleanup_errors;
 for(size_t i=0;i<count;++i){uwvm_rmw_subscription_transport_diagnostic d;if(uwvm_rmw_subscription_transport_diagnostics(transport,i,&d))++cleanup_errors;
 else if(d.first_primary||(d.retire_attempted&&d.retire_code)||(d.raw_close_reported&&d.raw_close_code))++cleanup_errors;}}
 // No C-singleton uninstall/free fiction. Process exit releases local storage;
 // native and raw owners were explicitly attempted above and externally drained.
 if(raw)fprintf(stderr,"Subscription bootstrap raw error %u\n",raw);
 if(failed||cleanup_errors)return EXIT_FAILURE;puts("PASS_ORIGINAL_RMW_SUBSCRIPTION_EXPLICIT_COMMIT_REQUIRES_SEPARATE_NATIVE_PEER");return EXIT_SUCCESS;
}
