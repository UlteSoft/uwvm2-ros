/* SPDX-License-Identifier: Apache-2.0
 * Explicit ROS2 metadata-only example. These alias strings are source choices,
 * NOT deployment grants or evidence that any native service exists. Host must
 * independently grant available+call and bind the exact native resource/QoS.
 */
#include <roswasm/ros2.h>
#include <roswasm/service_client_ros2.h>
#include <roswasm/session.h>
int main(void){
  roswasmc_session session;roswasmc_capability slots[2],*source=0,*available=0;
  roswasmc_service_client_backend const * api=roswasmc_ros2_service_client_backend();
  uint32_t code;uint64_t bits=0;int ready=0;uint32_t cleanup_statuses[2]={0,0};
  code=roswasmc_session_init(&session,roswasmc_ros2_backend(),slots,2);if(code)return (int)code;
  code=roswasmc_session_check_abi(&session);
  if(!code&&api->abi_version()!=1)code=95;
  if(!code)code=roswasmc_session_open(&session,"service-call",12,&source);
  if(!code)code=roswasmc_session_open(&session,"service-available",17,&available);
  if(!code)code=roswasmc_service_client_features(api,available,source,&bits);
  if(!code&&(bits&UWVM_ROS_CLIENT_TYPED_QUERY)==0)code=95;
  if(!code)code=roswasmc_service_client_query(api,available,source,UINT64_C(1000000000),&ready);
  /* False is a successful metadata observation. No request is submitted here.
   * A later call needs its own admitted operation and may still fail. */
  roswasmc_cleanup_report report=roswasmc_session_close(&session,code,cleanup_statuses,2);
  if(report.primary_error)return (int)report.primary_error;
  if(report.cleanup_error)return (int)report.cleanup_error;
  return ready?0:1;
}
