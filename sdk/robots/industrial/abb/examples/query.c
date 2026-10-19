/** One complete typed C11 vendor service request. No retry or implicit grant.
 * Reactor driver calls initialize_case(mode), query_once(), getters, close_case().
 * Native response fields are retained even on a controller-reported failure.
 */
#include <roswasm/ros2.h>
#include "bounded_interfaces.h"
#define EXPORT(n) __attribute__((export_name(n)))
static roswasmc_context context;static roswasmc_capability owner;
static int started,closed,called,denied;static uint32_t primary,cleanup;static roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response response;
static uint32_t finish(uint32_t status){if(status&&!primary)primary=status;if(!closed){closed=1;if(roswasmc_is_open(&owner))cleanup=roswasmc_close(&owner);}return primary?primary:cleanup;}
EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode){uint32_t e;if(started||closed)return 9;if(mode>1)return 22;started=1;
 if(uwvm_ros2_abi_version()!=1)return finish(95);context.backend=roswasmc_ros2_backend();roswasmc_capability_init(&owner,&context);
 e=roswasmc_open(&owner,"vendor-query",12);if(mode==0){denied=e==13&&!owner.handle;return finish(denied?0:e?e:22);}return e?finish(e):0;}
EXPORT("query_once") uint32_t query_once(void){roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request request={0};roswasmc_result r;if(!started||closed||called)return 9;called=1;
 r=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_call(&owner,&request,&response);if(r.error)return finish(r.error);return finish(response.result_code==1&&response.speed_ratio<=100?0:5);}
EXPORT("denied_verified") uint32_t denied_verified(void){return denied;}
EXPORT("primary_status") uint32_t primary_status(void){return primary;}
EXPORT("cleanup_status") uint32_t cleanup_status(void){return cleanup;}
EXPORT("close_case") uint32_t close_case(void){if(closed)return 9;return finish(0);}
