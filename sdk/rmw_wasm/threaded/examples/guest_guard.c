/* SPDX-License-Identifier: Apache-2.0
 * Actual ROS2 Guest ABI backend, actual original RMW APIs and real pthread.
 * No capability is opened. This witnesses shared Guest guard notification and
 * physical thread join only, never DDS/native topic/service interoperability.
 */
#include <uwvm_rmw/bridge.h>
#include <uwvm_rmw_c/provider.h>
#include <roswasm/ros2.h>
#include <rcutils/strdup.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
static uwvm_rmw_profile selection;
static uwvm_rmw_c_profile provider={.version=1};
static rmw_context_t context;static rmw_guard_condition_t*guard;static rmw_wait_set_t*set;
static rmw_ret_t wait_result;static void*wait_slot;
static void*worker(void*unused){(void)unused;wait_slot=guard;rmw_guard_conditions_t list={1,&wait_slot};
 wait_result=rmw_wait(NULL,&list,NULL,NULL,NULL,set,NULL);return NULL;}
int main(void){
 int result=1,context_live=0,options_live=0;unsigned cleanup=0;pthread_t thread;
 rmw_init_options_t options=rmw_get_zero_initialized_init_options();rcutils_allocator_t a=rcutils_get_default_allocator();
 selection.version=1;selection.backend=roswasmc_ros2_backend();selection.graph=uwvm_rmw_c_graph_provider();
 if(uwvm_rmw_install_profile(&selection)!=RMW_RET_OK||uwvm_rmw_c_install_profile(&provider)!=RMW_RET_OK)goto done;
 if(rmw_init_options_init(&options,a)!=RMW_RET_OK)goto done;options_live=1;options.instance_id=1;
 options.enclave=rcutils_strdup("/",a);if(!options.enclave)goto done;
 context=rmw_get_zero_initialized_context();if(rmw_init(&options,&context)!=RMW_RET_OK)goto done;context_live=1;
 if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto done;}options_live=0;
 guard=rmw_create_guard_condition(&context);set=rmw_create_wait_set(&context,1);if(!guard||!set)goto done;
 if(pthread_create(&thread,NULL,worker,NULL))goto done;
 /* A trigger before the worker's first wait is retained by the same actual
  * local guard. No timing sleep, fabricated readiness or poll-count clock. */
 if(rmw_trigger_guard_condition(guard)!=RMW_RET_OK){
  (void)rmw_shutdown(&context);(void)pthread_join(thread,NULL);goto done;}
 if(pthread_join(thread,NULL))return 125; /* cannot retire a possibly live borrower */
 if(wait_result!=RMW_RET_OK||wait_slot!=guard)goto done;result=0;
 done:
 if(set&&rmw_destroy_wait_set(set)!=RMW_RET_OK)++cleanup;
 if(guard&&rmw_destroy_guard_condition(guard)!=RMW_RET_OK)++cleanup;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup;if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup;}
 if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup;
 if(uwvm_rmw_install_profile(NULL)!=RMW_RET_OK)++cleanup;
 if(result||cleanup)return 1;
 puts("PASS_ACTUAL_GUEST_RMW_GUARD_JOIN_NOT_NATIVE_ROS");return 0;
}
