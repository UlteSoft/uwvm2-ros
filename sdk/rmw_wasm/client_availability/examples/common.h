/* SPDX-License-Identifier: Apache-2.0
 * Complete pure-C bootstrap helpers. This header is authored source, not an
 * actual generated message or an identity exporter. Every object below is an
 * ORIGINAL included C type; every identity is an external actual SDK input.
 */
#ifndef UWVM_RMW_C_EXAMPLE_COMMON_H
#define UWVM_RMW_C_EXAMPLE_COMMON_H
#include <uwvm_rmw/c_codecs.h>
#include <uwvm_rmw/service_transport.h>
#include <uwvm_rmw/subscription_transport.h>
#include <uwvm_rmw/publisher_transport.h>
#include <std_msgs/msg/string.h>
#include <std_srvs/srv/empty.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <rosidl_runtime_c/string_functions.h>
#include <roswasm/ros2.h>
#include <rcutils/strdup.h>
#include <rmw/qos_profiles.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uwvm_rmw_c_codecs_registry*example_registry;
static int example_identity(char const*t,uint8_t out[32]){
 if(!t)return 0;unsigned nz=0;for(size_t i=0;i<64;++i){unsigned char c=(unsigned char)t[i];if(!c)return 0;unsigned v;
 if(c>='0'&&c<='9')v=c-'0';else if(c>='a'&&c<='f')v=c-'a'+10;else if(c>='A'&&c<='F')v=c-'A'+10;else return 0;
 if(i%2)out[i/2]|=(uint8_t)v;else out[i/2]=(uint8_t)(v<<4);nz|=v;}return !t[64]&&nz;
}
static int example_decimal(char const*t,size_t*out){if(!t||!*t)return 0;size_t n=0;for(size_t i=0;t[i];++i){if(i>20||t[i]<'0'||t[i]>'9')return 0;size_t d=(size_t)(t[i]-'0');if(n>(SIZE_MAX-d)/10)return 0;n=n*10+d;}*out=n;return 1;}
static uint32_t example_prepare(char const*const*text,uwvm_rmw_codec const**codecs,size_t*count){
 uwvm_rmw_c_native_identities ids={0};for(size_t i=0;i<UWVM_RMW_C_ID_COUNT;++i)if(!example_identity(text[i],ids.hashes[i]))return 22;
 rcutils_allocator_t a=rcutils_get_default_allocator();uint32_t raw=uwvm_rmw_c_codecs_create(&ids,1024,&a,&example_registry);if(raw)return raw;
 /* Export marks process lifetime BEFORE publishing the seven descriptors.
  * The current immutable C singleton cannot uninstall; never free these later. */
 return uwvm_rmw_c_codecs_export(example_registry,codecs,count);
}
static rmw_qos_profile_t example_qos(void){rmw_qos_profile_t q={0};q.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;q.depth=4;
 q.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;q.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;q.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
 q.deadline=(rmw_time_t)RMW_DURATION_INFINITE;q.lifespan=(rmw_time_t)RMW_DURATION_INFINITE;q.liveliness_lease_duration=(rmw_time_t)RMW_DURATION_INFINITE;return q;}
static void example_print_service_info(rmw_service_info_t const*i){printf("RMW_SERVICE_INFO source_time=%lld received_time=%lld sequence=%lld writer=",(long long)i->source_timestamp,(long long)i->received_timestamp,(long long)i->request_id.sequence_number);for(size_t n=0;n<RMW_GID_STORAGE_SIZE;++n)printf("%02x",i->request_id.writer_guid[n]);putchar('\n');}
/* Keep the original concrete C objects distinct, including real Empty
 * placeholder bytes. A C object is NEVER passed to a C++ codec or companion. */
typedef struct example_values {
 bool add,request_live,response_live;
 union {std_srvs__srv__Empty_Request empty;example_interfaces__srv__AddTwoInts_Request add;} request;
 union {std_srvs__srv__Empty_Response empty;example_interfaces__srv__AddTwoInts_Response add;} response;
} example_values;
static uint32_t example_values_init(example_values*v,bool add){memset(v,0,sizeof*v);v->add=add;
 if(add){if(!example_interfaces__srv__AddTwoInts_Request__init(&v->request.add))return 12;v->request_live=true;
 if(!example_interfaces__srv__AddTwoInts_Response__init(&v->response.add))return 12;v->response_live=true;}
 else {if(!std_srvs__srv__Empty_Request__init(&v->request.empty))return 12;v->request_live=true;
 if(!std_srvs__srv__Empty_Response__init(&v->response.empty))return 12;v->response_live=true;}
 return 0;
}
static void*example_request(example_values*v){return v->add?(void*)&v->request.add:(void*)&v->request.empty;}
static void*example_response(example_values*v){return v->add?(void*)&v->response.add:(void*)&v->response.empty;}
static uint32_t example_set_request(example_values*v){if(v->add){v->request.add.a=17;v->request.add.b=-9;}else v->request.empty.structure_needs_at_least_one_member=0;return 0;}
static uint32_t example_check_request(example_values const*v){return v->add?(v->request.add.a!=17||v->request.add.b!=-9):(v->request.empty.structure_needs_at_least_one_member!=0);}
static uint32_t example_set_response(example_values*v){if(v->add)v->response.add.sum=8;else v->response.empty.structure_needs_at_least_one_member=0;return 0;}
static uint32_t example_check_response(example_values const*v){return v->add?(v->response.add.sum!=8):(v->response.empty.structure_needs_at_least_one_member!=0);}
static void example_values_fini(example_values*v){if(v->response_live){if(v->add)example_interfaces__srv__AddTwoInts_Response__fini(&v->response.add);else std_srvs__srv__Empty_Response__fini(&v->response.empty);v->response_live=false;}
 if(v->request_live){if(v->add)example_interfaces__srv__AddTwoInts_Request__fini(&v->request.add);else std_srvs__srv__Empty_Request__fini(&v->request.empty);v->request_live=false;}}
static unsigned example_context_diagnostics(rmw_context_t const*c){uwvm_rmw_diagnostics d;if(uwvm_rmw_get_diagnostics(c,&d)!=RMW_RET_OK)return 1;
 /* Preserve every actual B attempt separately from native provider diagnostics.
  * No guessed raw-owner-index to transport-slot association is fabricated. */
 for(size_t i=0;i<UWVM_RMW_MAX_OWNERS;++i)if(d.owner_close_attempted[i])printf("B_RAW_CLOSE slot=%zu code=%u\n",i,d.owner_close_codes[i]);
 for(size_t i=0;i<UWVM_RMW_MAX_NODES;++i)if(d.graph_close_attempted[i])printf("B_GRAPH_CLOSE slot=%zu code=%u\n",i,d.graph_close_codes[i]);
 return (unsigned)(d.owner_close_failures||d.native_retire_failures||d.local_retirement_failures);
}
static unsigned example_service_diagnostics(uwvm_rmw_service_transport*t){size_t n=0;if(!t||uwvm_rmw_service_transport_count(t,&n))return t?1:0;unsigned bad=0;
 for(size_t i=0;i<n;++i){uwvm_rmw_service_transport_diagnostic d;if(uwvm_rmw_service_transport_diagnostics(t,i,&d)){++bad;continue;}
 printf("SERVICE_NATIVE_RETIRE slot=%zu source=%llu attempted=%u code=%u primary=%u\n",i,(unsigned long long)d.source,d.retire_attempted,d.retire_code,d.first_primary);
 if(d.first_primary||(d.retire_attempted&&d.retire_code))++bad;}return bad;}
static unsigned example_subscription_diagnostics(uwvm_rmw_subscription_transport*t){size_t n=0;if(!t||uwvm_rmw_subscription_transport_count(t,&n))return t?1:0;unsigned bad=0;
 for(size_t i=0;i<n;++i){uwvm_rmw_subscription_transport_diagnostic d;if(uwvm_rmw_subscription_transport_diagnostics(t,i,&d)){++bad;continue;}
 printf("SUB_NATIVE_RETIRE slot=%zu source=%llu attempted=%u code=%u primary=%u\n",i,(unsigned long long)d.source,d.retire_attempted,d.retire_code,d.first_primary);
 if(d.first_primary||(d.retire_attempted&&d.retire_code))++bad;}return bad;}
static unsigned example_publisher_diagnostics(uwvm_rmw_publisher_transport*t){size_t n=0;if(!t||uwvm_rmw_publisher_transport_count(t,&n))return t?1:0;unsigned bad=0;
 for(size_t i=0;i<n;++i){uwvm_rmw_publisher_transport_diagnostic d;if(uwvm_rmw_publisher_transport_diagnostics(t,i,&d)){++bad;continue;}
 printf("PUB_NATIVE_RETIRE slot=%zu source=%llu attempted=%u code=%u primary=%u\n",i,(unsigned long long)d.source,d.retire_attempted,d.retire_code,d.first_primary);
 if(d.first_primary||(d.retire_attempted&&d.retire_code))++bad;}return bad;}
#endif
