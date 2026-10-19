/* SPDX-License-Identifier: Apache-2.0
 * Original RMW C application with a COMPLETE explicit Wasm publisher bootstrap.
 * All rmw/rcutils/rosidl/std_msgs types and functions are original SDK APIs.
 * No weak deployment function, fabricated type hash, default alias or native
 * transport model is supplied. Merely selecting an alias grants no authority.
 *
 * Arguments: exact-granted-alias /absolute/topic actual-RIHS01-hex64 domain-id.
 * Domain-id is parsed as a decimal size_t. It must match the Host target; this
 * profile does not select or reconfigure a Gateway domain. QoS is documented
 * below and must match the separately configured actual Host resource.
 */
#include <uwvm_rmw/bridge.h>
#include <uwvm_rmw_c/provider.h>
#include <uwvm_rmw/publisher_transport.h>
#include <roswasm/ros2.h>
#include <rcutils/strdup.h>
#include <rmw/qos_profiles.h>
#include <rmw/time.h>
#include <std_msgs/msg/string.h>
#include <rosidl_runtime_c/string_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* This source-selected deployment storage lives until process termination.
 * A cleanup refusal must never leave the installed profile pointing at a
 * returned stack frame. This example is one standalone process, not a reusable
 * concurrent library. An embedding application must own equivalent storage. */
static uwvm_rmw_profile profile;
static uwvm_rmw_node_selection node_selection;
static uwvm_rmw_endpoint_selection endpoint_selection;
static uwvm_rmw_codec codec;
static uwvm_rmw_publisher_transport * transport;
static int hex_digit(unsigned char c) {
  if(c>='0'&&c<='9')return c-'0';
  if(c>='a'&&c<='f')return c-'a'+10;
  if(c>='A'&&c<='F')return c-'A'+10;
  return -1;
}
static int identity(char const * text,uint8_t output[32]) {
  /* argv strings are accessible C strings. Bound the read to exactly64 hex
   * digits plus NUL; reject an all-zero identity before any import or open. */
  if(!text)return 0;
  unsigned nonzero=0;
  for(size_t i=0;i<64;++i){if(!text[i])return 0;int n=hex_digit((unsigned char)text[i]);if(n<0)return 0;
    if((i&1)==0)output[i/2]=(uint8_t)(n<<4);else output[i/2]|=(uint8_t)n;nonzero|=(unsigned)n;}
  return text[64]=='\0'&&nonzero;
}
static int domain(char const * text,size_t * output) {
  if(!text||!*text)return 0;
  size_t n=0;for(size_t i=0;text[i];++i){if(i>20||text[i]<'0'||text[i]>'9')return 0;
    size_t d=(size_t)(text[i]-'0');if(n>(SIZE_MAX-d)/10)return 0;n=n*10+d;}
  *output=n;return 1;
}
static int same_qos(rmw_qos_profile_t const * a,rmw_qos_profile_t const * b) {
  return a->history==b->history&&a->depth==b->depth&&a->reliability==b->reliability&&
    a->durability==b->durability&&a->liveliness==b->liveliness&&
    a->avoid_ros_namespace_conventions==b->avoid_ros_namespace_conventions&&
    rmw_time_equal(a->deadline,b->deadline)&&rmw_time_equal(a->lifespan,b->lifespan)&&
    rmw_time_equal(a->liveliness_lease_duration,b->liveliness_lease_duration);
}
int main(int argc,char ** argv) {
  int failed=1,profile_live=0,options_live=0,context_live=0,message_live=0;
  unsigned cleanup_errors=0;uint32_t raw=0;uint8_t expected_hash[32];size_t domain_id;
  rmw_context_t context=rmw_get_zero_initialized_context();
  rmw_init_options_t options=rmw_get_zero_initialized_init_options();
  rmw_node_t * node=NULL;rmw_publisher_t * publisher=NULL;
  std_msgs__msg__String message;memset(&message,0,sizeof message);
  rcutils_allocator_t allocator=rcutils_get_default_allocator();
  if(argc!=5||!identity(argv[3],expected_hash)||!domain(argv[4],&domain_id)) {
    fputs("Usage: checked_publisher exact-alias /topic native-rihs01-hex64 domain-id\n",stderr);goto finish;
  }
  /* The accessor is from the original generated std_msgs library. The codec
   * proves its full actual native hash/introspection graph against the explicit
   * independently exported expected hash. Four bytes encode canonical length. */
  rosidl_message_type_support_t const * support=ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String);
  raw=uwvm_rmw_string_codec_init(&codec,support,expected_hash,1024);if(raw)goto finish;
  raw=uwvm_rmw_publisher_transport_create(1,&allocator,&transport);if(raw)goto finish;
  node_selection=(uwvm_rmw_node_selection){"checked_rmw_publisher","/"};
  rmw_qos_profile_t qos={0};qos.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;qos.depth=10;
  qos.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;qos.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
  qos.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;qos.avoid_ros_namespace_conventions=false;
  qos.deadline=(rmw_time_t)RMW_DURATION_INFINITE;
  qos.lifespan=(rmw_time_t)RMW_DURATION_INFINITE;
  qos.liveliness_lease_duration=(rmw_time_t)RMW_DURATION_INFINITE;
  /* All three durations explicitly use the original RMW infinite constant.
   * Host policy must say infinite too; actual getters must confirm it. */
  endpoint_selection.node_index=0;endpoint_selection.codec_index=0;
  endpoint_selection.kind=UWVM_RMW_PUBLISHER;endpoint_selection.resolved_name=argv[2];
  endpoint_selection.requested_qos=endpoint_selection.effective_qos=qos;
  endpoint_selection.aliases[0]=argv[1];endpoint_selection.alias_count=1;
  profile.version=1;profile.domain_id=domain_id;profile.backend=roswasmc_ros2_backend();
  profile.nodes=&node_selection;profile.node_count=1;profile.codecs=&codec;profile.codec_count=1;
  profile.endpoints=&endpoint_selection;profile.endpoint_count=1;profile.graph=uwvm_rmw_c_graph_provider();
  raw=uwvm_rmw_publisher_transport_callbacks(transport,&profile.publisher);if(raw)goto finish;
  if(uwvm_rmw_install_profile(&profile)!=RMW_RET_OK)goto finish;profile_live=1;
  /* No A clock/logging aliases are needed by this direct-RMW example. Original
   * rcl applications must additionally prepare their real platform clock/log
   * profile; this example does not silently suppress original rcl side effects. */
  if(rmw_init_options_init(&options,allocator)!=RMW_RET_OK)goto finish;options_live=1;
  options.instance_id=1;options.domain_id=domain_id;
  options.enclave=rcutils_strdup("/",allocator);if(!options.enclave)goto finish;
  if(rmw_init(&options,&context)!=RMW_RET_OK)goto finish;context_live=1;
  if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto finish;}options_live=0;
  node=rmw_create_node(&context,node_selection.name,node_selection.namespace_);if(!node)goto finish;
  rmw_publisher_options_t publisher_options=rmw_get_default_publisher_options();
  publisher=rmw_create_publisher(node,support,argv[2],&qos,&publisher_options);if(!publisher)goto finish;
  rmw_qos_profile_t actual;rmw_gid_t gid;
  if(rmw_publisher_get_actual_qos(publisher,&actual)!=RMW_RET_OK||!same_qos(&qos,&actual)||
     rmw_get_gid_for_publisher(publisher,&gid)!=RMW_RET_OK)goto finish;
  if(!std_msgs__msg__String__init(&message))goto finish;message_live=1;
  /* Seven actual bytes, including an embedded NUL. The native receiver must
   * compare both length and all bytes; strlen/console text is not a witness. */
  static char const payload[7]={'w','a','s','m','\0','r','o'};
  if(!rosidl_runtime_c__String__assignn(&message.data,payload,sizeof payload))goto finish;
  if(rmw_publish(publisher,&message,NULL)!=RMW_RET_OK)goto finish;
  failed=0;
finish:
  if(message_live)std_msgs__msg__String__fini(&message);
  if(publisher&&rmw_destroy_publisher(node,publisher)!=RMW_RET_OK)++cleanup_errors;
  if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup_errors;
  if(context_live) {
    if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup_errors;
    uwvm_rmw_diagnostics d;
    if(uwvm_rmw_get_diagnostics(&context,&d)!=RMW_RET_OK)++cleanup_errors;
    else if(d.native_retire_failures||d.owner_close_failures||d.local_retirement_failures||(d.graph_close_attempted[0]&&d.graph_close_codes[0]))++cleanup_errors;
    if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup_errors;
  }
  if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup_errors;
  /* Refusal keeps static immutable profile+provider alive until process/Host
   * drain. Never free its callbacks underneath a retained original context. */
  int uninstalled=!profile_live;
  if(profile_live){uninstalled=uwvm_rmw_install_profile(NULL)==RMW_RET_OK;if(!uninstalled)++cleanup_errors;}
  if(transport&&uninstalled) {
    size_t count=0;
    if(uwvm_rmw_publisher_transport_count(transport,&count))++cleanup_errors;
    else for(size_t i=0;i<count;++i){uwvm_rmw_publisher_transport_diagnostic d;
      if(uwvm_rmw_publisher_transport_diagnostics(transport,i,&d))++cleanup_errors;
      else if(d.first_primary||(d.retire_attempted&&d.retire_code))++cleanup_errors;}
    if(uwvm_rmw_publisher_transport_destroy(&transport))++cleanup_errors;
  }
  if(raw)fprintf(stderr,"Publisher bootstrap raw error %u\n",(unsigned)raw);
  if(failed||cleanup_errors)return EXIT_FAILURE;
  puts("PASS_ORIGINAL_RMW_PUBLISH_SUBMIT_AND_CLEANUP_REQUIRES_SEPARATE_NATIVE_RECEIVER");
  return EXIT_SUCCESS;
}
