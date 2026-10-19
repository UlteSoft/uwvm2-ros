/* SPDX-License-Identifier: Apache-2.0
 * COMPLETE direct explicit provider bootstrap, NOT an rmw_* availability hook.
 * A separate actual retained client is owned here through three distinct raw
 * capabilities. It is not silently substituted for an existing rmw_client_t.
 * The actual matching CPP10 object/support companion is fully linked.
 */
#include <uwvm_rmw/service_transport.h>
#include <roswasm/ros2.h>
#include "native_objects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uwvm_rmw_service_transport*transport;
int main(int argc,char**argv){
 if(argc!=15){fputs("Usage: checked_availability call-alias observer-alias availability-alias 11-actual-independent-native-rihs01-values\n",stderr);return 22;}
 uwvm_rmw_codec const*codecs=NULL;size_t count=0,index=0;uint32_t raw=example_native_prepare((char const*const*)(argv+4),0,&codecs,&count,&index);if(raw)return (int)raw;
 rcutils_allocator_t allocator=rcutils_get_default_allocator();raw=uwvm_rmw_service_transport_create(1,1,0,0,&allocator,&transport);if(raw)return (int)raw;
 uwvm_rmw_c_activation_provider activation={0};uwvm_rmw_c_request_provider requests={0};uwvm_rmw_c_native_metadata_provider metadata={0};
 raw=uwvm_rmw_service_transport_callbacks(transport,&activation,&requests,&metadata);if(raw)return (int)raw;
 roswasmc_context context={roswasmc_ros2_backend()};roswasmc_capability owners[3]={{0}};
 for(unsigned i=0;i<3;++i)roswasmc_capability_init(owners+i,&context);
 uwvm_rmw_c_native_endpoint endpoint={0};int activated=0;unsigned cleanup=0;
 if(!context.backend->abi_version||context.backend->abi_version()!=1){raw=95;goto finish;}
 for(unsigned i=0;i<3;++i){for(unsigned j=0;j<i;++j)if(!strcmp(argv[1+i],argv[1+j])){raw=22;goto finish;}raw=roswasmc_open(owners+i,argv[1+i],strlen(argv[1+i]));if(raw)goto finish;}
 rmw_qos_profile_t requested={0};requested.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;requested.depth=4;requested.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;requested.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;requested.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;requested.deadline=(rmw_time_t)RMW_DURATION_INFINITE;requested.lifespan=(rmw_time_t)RMW_DURATION_INFINITE;requested.liveliness_lease_duration=(rmw_time_t)RMW_DURATION_INFINITE;
 raw=activation.activate(activation.state,UWVM_RMW_CLIENT,owners,owners+1,codecs+index,&requested,&endpoint);if(raw)goto finish;activated=1;
 bool available=false;raw=uwvm_rmw_service_transport_available(transport,&endpoint,owners,owners+2,0,&available);
 if(!raw)printf("ACTUAL_OWNED_CLIENT_AVAILABLE %u; original rmw_service_server_is_available remains95 until separately connected\n",available?1u:0u);
finish:
 if(activated&&activation.retire(activation.state,owners,owners+1,&endpoint))++cleanup;
 for(unsigned i=0;i<3;++i)if(roswasmc_is_open(owners+i)&&roswasmc_close(owners+i))++cleanup;
 // Exported state is process-lifetime, but all Native/raw effects were attempted
 // independently. Caller must retain actual Host/native and owner diagnostics.
 return raw?(int)raw:(cleanup?1:0);
}
