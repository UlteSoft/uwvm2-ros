/* SPDX-License-Identifier: Apache-2.0
 * Explicit provider bootstrap only. Native fact providers are independent
 * protocol implementations, never a source of invented GUIDs/timestamps. */
#include "c_internal.h"
#include <limits.h>
static uwvm_rmw_c_profile const*configuration;static int started;
void uwvm_rmw_c_mark_started(void){started=1;}
uwvm_rmw_c_profile const*uwvm_rmw_c_configuration(void){return configuration;}
rmw_ret_t uwvm_rmw_c_install_profile(uwvm_rmw_c_profile const*p){
 UWVM_RMW_ENTRY(RMW_RET_ERROR);
 if(configuration||started)return uwvm_rmw_error(NULL,17,"C provider bootstrap is immutable after installation");
 if(!p)return RMW_RET_OK;
 if(p->version!=UWVM_RMW_C_PROVIDER_VERSION)return uwvm_rmw_error(NULL,95,"unknown C provider contract version");
 if(p->activation.version){
  if(p->activation.version!=1||!p->activation.activate||!p->activation.inspect||!p->activation.retire)return uwvm_rmw_error(NULL,22,"actual endpoint activation provider is incomplete");
 }else if(p->activation.state||p->activation.activate||p->activation.inspect||p->activation.retire)return uwvm_rmw_error(NULL,22,"unversioned activation callback refused");
 if(p->requests.version){
  if(p->requests.version!=1||(!p->requests.submit&&!p->requests.respond))return uwvm_rmw_error(NULL,22,"versioned real request provider is incomplete");
 }else if(p->requests.state||p->requests.submit||p->requests.respond)return uwvm_rmw_error(NULL,22,"unversioned request provider refused");
 if(p->wait.version){
  if(p->wait.version!=1||!p->wait.wait_slice||!p->wait.maximum_wait_ns||p->wait.maximum_wait_ns>INT64_MAX||
   !p->wait.maximum_slice_ns||p->wait.maximum_slice_ns>p->wait.maximum_wait_ns||
   !p->wait.maximum_iterations||p->wait.maximum_iterations>1048576)
   return uwvm_rmw_error(NULL,22,"explicit finite real wait provider bounds required");
 }else if(p->wait.wait_slice||p->wait.state||p->wait.maximum_wait_ns||p->wait.maximum_slice_ns||p->wait.maximum_iterations)
  return uwvm_rmw_error(NULL,22,"unversioned wait fields refused");
 if(p->metadata.version&&p->metadata.version!=1)return uwvm_rmw_error(NULL,95,"unknown native metadata provider version");
 if(!p->metadata.version&&(p->metadata.state||p->metadata.take_topic||p->metadata.take_request||p->metadata.take_response))
  return uwvm_rmw_error(NULL,22,"unversioned native metadata callbacks refused");
 if(p->availability.version){
  if(p->availability.version!=1||!p->availability.query)return uwvm_rmw_error(NULL,22,"versioned same-client availability provider is incomplete");
 }else if(p->availability.state||p->availability.query)return uwvm_rmw_error(NULL,22,"unversioned client availability callbacks refused");
 configuration=p;return RMW_RET_OK;
}

uint32_t uwvm_rmw_c_endpoint_valid(uwvm_rmw_c_native_endpoint const*e,roswasmc_capability const*source,size_t count){
 if(!e||!source||!roswasmc_is_open(source)||e->version!=1||e->backend!=2||
  source->context->backend->version!=2||e->source!=source->handle||!(uint32_t)e->source||!(e->source>>32)||!e->instance||!e->runtime_generation||
  !e->session||!e->native_incarnation||e->qos_count!=count||count<1||count>2||e->native_gid_bytes>32)return 22;
 for(size_t i=e->native_gid_bytes;i<32;++i)if(e->native_gid[i])return 22;
 /* Extent is explicit actual-provider presence. Zero bytes are native data,
  * not an invented universal invalid-GID sentinel. Same-binding inspect and
  * exact extent/tail checks still apply before any public output. */
 for(size_t i=0;i<count;++i)if(!uwvm_rmw_qos_concrete(e->actual_qos+i))return 22;
 return 0;
}
uint32_t uwvm_rmw_c_endpoint_inspect(uwvm_rmw_c_native_endpoint const*expected,
 roswasmc_capability const*source,roswasmc_capability const*auxiliary){
 uwvm_rmw_c_profile const*p=configuration;uwvm_rmw_c_native_endpoint observed={0};
 if(!p||p->activation.version!=1)return 95;
 uint32_t raw=p->activation.inspect(p->activation.state,source,auxiliary,expected,&observed);
 if(raw)return raw;raw=uwvm_rmw_c_endpoint_valid(&observed,source,expected->qos_count);if(raw)return raw;
 if(observed.source!=expected->source||observed.instance!=expected->instance||
  observed.runtime_generation!=expected->runtime_generation||observed.session!=expected->session||
  observed.native_incarnation!=expected->native_incarnation||observed.native_gid_bytes!=expected->native_gid_bytes||
  memcmp(observed.native_gid,expected->native_gid,32))return 13;
 for(size_t i=0;i<expected->qos_count;++i)if(!uwvm_rmw_qos_equal(observed.actual_qos+i,expected->actual_qos+i))return 13;
 return 0;
}
