/* SPDX-License-Identifier: Apache-2.0
 * ORIGINAL RMW C API with ORIGINAL generated Empty/Add C objects and the pure-C
 * Registry. No C++ companion is linked. Source only, not executed/qualified.
 */
#include "common.h"
static uwvm_rmw_profile profile;static uwvm_rmw_c_profile c_profile;
static uwvm_rmw_node_selection selected_node;static uwvm_rmw_endpoint_selection selected_endpoint;
static uwvm_rmw_service_transport*transport;
int main(int argc,char**argv){
 int failed=1,options_live=0,context_live=0;unsigned cleanup=0;uint32_t raw=0;size_t domain=0,count=0,index=0;int client=0,add=0;example_values values={0};
 rcutils_allocator_t allocator=rcutils_get_default_allocator();rmw_context_t context=rmw_get_zero_initialized_context();rmw_init_options_t options=rmw_get_zero_initialized_init_options();
 rmw_node_t*node=NULL;rmw_client_t*call=NULL;rmw_service_t*service=NULL;uwvm_rmw_codec const*codecs=NULL;
 if(argc!=18||((client=!strcmp(argv[1],"client"))==0&&strcmp(argv[1],"server"))||((add=!strcmp(argv[2],"add"))==0&&strcmp(argv[2],"empty"))||!argv[3][0]||!argv[4][0]||!strcmp(argv[3],argv[4])||argv[5][0]!='/'||!example_decimal(argv[6],&domain)){
 fputs("Usage: checked_service_c client|server empty|add exact-primary-alias exact-peer-alias /service domain-id 11-independent-rihs01-hex64-in-C-Identity-order\n",stderr);goto finish;}
 raw=example_prepare((char const*const*)(argv+7),&codecs,&count);if(raw)goto finish;index=add?UWVM_RMW_C_CODEC_ADD_SERVICE:UWVM_RMW_C_CODEC_EMPTY_SERVICE;
 raw=example_values_init(&values,add!=0);if(raw)goto finish;
 raw=uwvm_rmw_service_transport_create(1,64,add?16:0,add?8:0,&allocator,&transport);if(raw)goto finish;
 raw=uwvm_rmw_service_transport_callbacks(transport,&c_profile.activation,&c_profile.requests,&c_profile.metadata);if(raw)goto finish;c_profile.version=1;
 selected_node=(uwvm_rmw_node_selection){client?"checked_rmw_service_client":"checked_rmw_service_server","/"};
 rmw_qos_profile_t qos={0};qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;qos.depth=4;qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;qos.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
 qos.deadline=(rmw_time_t)RMW_DURATION_INFINITE;qos.lifespan=(rmw_time_t)RMW_DURATION_INFINITE;qos.liveliness_lease_duration=(rmw_time_t)RMW_DURATION_INFINITE;
 selected_endpoint.node_index=0;selected_endpoint.codec_index=index;selected_endpoint.kind=client?UWVM_RMW_CLIENT:UWVM_RMW_SERVICE;selected_endpoint.resolved_name=argv[5];selected_endpoint.requested_qos=selected_endpoint.effective_qos=qos;selected_endpoint.aliases[0]=argv[3];selected_endpoint.aliases[1]=argv[4];selected_endpoint.alias_count=2;
 profile.version=1;profile.domain_id=domain;profile.backend=roswasmc_ros2_backend();profile.nodes=&selected_node;profile.node_count=1;profile.codecs=codecs;profile.codec_count=count;profile.endpoints=&selected_endpoint;profile.endpoint_count=1;profile.graph=uwvm_rmw_c_graph_provider();
 if(uwvm_rmw_install_profile(&profile)!=RMW_RET_OK||uwvm_rmw_c_install_profile(&c_profile)!=RMW_RET_OK)goto finish;
 // The static profile and actual pure-C registry/state have process lifetime.
 // No C-singleton uninstall/free is invented. Warm Native TypeSupport BEFORE
 // stopped-map checkpoints under the independently retained actual loader phase.
 if(rmw_init_options_init(&options,allocator)!=RMW_RET_OK)goto finish;options_live=1;options.instance_id=1;options.domain_id=domain;options.enclave=rcutils_strdup("/",allocator);if(!options.enclave)goto finish;
 if(rmw_init(&options,&context)!=RMW_RET_OK)goto finish;context_live=1;if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto finish;}options_live=0;
 node=rmw_create_node(&context,selected_node.name,selected_node.namespace_);if(!node)goto finish;
 rosidl_service_type_support_t const*support=codecs[index].generated_support;
 if(client){
  call=rmw_create_client(node,support,argv[5],&qos);if(!call)goto finish;
  // Existing RMW availability remains explicitly95. Do not turn graph lists,
  // loops or timers into readiness. The controller coordinates the real peer.
  if(example_set_request(&values))goto finish;int64_t sequence=0;
  if(rmw_send_request(call,example_request(&values),&sequence)!=RMW_RET_OK||sequence<=0)goto finish;
  bool taken=false;rmw_service_info_t info={0};
  for(size_t i=0;i!=4096&&!taken;++i)if(rmw_take_response(call,&info,example_response(&values),&taken)!=RMW_RET_OK)goto finish;
  if(!taken||info.request_id.sequence_number!=sequence||example_check_response(&values))goto finish;example_print_service_info(&info);
  uwvm_rmw_c_service_diagnostics diagnostics;if(uwvm_rmw_c_client_diagnostics(call,&diagnostics)!=RMW_RET_OK||!diagnostics.actual_native_info_present)goto finish;
  // Public RMW response ID uses the legitimate local request ID. Preserve and
  // print the separate ACTUAL native request writer/sequence/times, never claim
  // these identities are interchangeable or fabricate a native cancellation.
  example_print_service_info(&diagnostics.actual_native_info);
 }else{
  service=rmw_create_service(node,support,argv[5],&qos);if(!service)goto finish;
  bool taken=false;rmw_service_info_t info={0};
  for(size_t i=0;i!=4096&&!taken;++i)if(rmw_take_request(service,&info,example_request(&values),&taken)!=RMW_RET_OK)goto finish;
  if(!taken||example_check_request(&values)||example_set_response(&values))goto finish;example_print_service_info(&info);
  if(rmw_send_response(service,&info.request_id,example_response(&values))!=RMW_RET_OK)goto finish;
 }
 failed=0;
finish:
 if(call&&rmw_destroy_client(node,call)!=RMW_RET_OK)++cleanup;
 if(service&&rmw_destroy_service(node,service)!=RMW_RET_OK)++cleanup;
 if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup;uwvm_rmw_diagnostics d;
 if(uwvm_rmw_get_diagnostics(&context,&d)!=RMW_RET_OK)++cleanup;
 else{for(unsigned i=0;i<2;++i)if(d.owner_close_attempted[i]&&transport)(void)uwvm_rmw_service_transport_record_raw_close(transport,0,i,d.owner_close_codes[i]);
 if(d.owner_close_failures||d.native_retire_failures||d.local_retirement_failures||(d.graph_close_attempted[0]&&d.graph_close_codes[0]))++cleanup;}
 if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup;}
 if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup;
 if(transport){size_t n=0;if(uwvm_rmw_service_transport_count(transport,&n))++cleanup;for(size_t i=0;i<n;++i){uwvm_rmw_service_transport_diagnostic d;
 if(uwvm_rmw_service_transport_diagnostics(transport,i,&d))++cleanup;else if(d.first_primary||(d.retire_attempted&&d.retire_code)||(d.raw_close_reported[0]&&d.raw_close_code[0])||(d.raw_close_reported[1]&&d.raw_close_code[1]))++cleanup;}}
 example_values_fini(&values);
 if(raw)fprintf(stderr,"Service bootstrap raw error %u\n",raw);
 if(failed||cleanup)return EXIT_FAILURE;puts("PASS_ORIGINAL_RMW_C_SERVICE_SELECTED_FULL_OBJECTS_NOT_COMPLETE_UPSTREAM_ACCEPTANCE");return EXIT_SUCCESS;
}
