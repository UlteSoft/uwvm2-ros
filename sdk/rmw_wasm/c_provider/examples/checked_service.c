/* SPDX-License-Identifier: Apache-2.0
 * Future REAL-RMW integration witness, not a transport model. Bootstrap is a
 * mandatory separately reviewed actual deployment object. Its absence is a link
 * error; no weak fallback, generated identity or native provider is invented.
 * The application and Host caller must impose independent whole/tree deadlines.
 */
#include <uwvm_rmw_c/provider.h>
#include <rcutils/strdup.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <stdio.h>
#include <string.h>
extern uint32_t uwvm_rmw_c_selected_bootstrap(void);
int main(void){
 int status=1;rmw_context_t context=rmw_get_zero_initialized_context();
 rmw_init_options_t options=rmw_get_zero_initialized_init_options();
 rmw_node_t*node=NULL;rmw_client_t*client=NULL;rmw_service_t*server=NULL;rmw_wait_set_t*wait=NULL;
 int options_live=0,context_live=0;unsigned cleanup_errors=0;bool sent_response=false,completed=false;
 if(uwvm_rmw_c_selected_bootstrap())goto done;
 rcutils_allocator_t a=rcutils_get_default_allocator();
 if(rmw_init_options_init(&options,a)!=RMW_RET_OK)goto done;options_live=1;
 options.instance_id=1;options.enclave=rcutils_strdup("/",a);if(!options.enclave)goto done;
 if(rmw_init(&options,&context)!=RMW_RET_OK)goto done;context_live=1;
 if(rmw_init_options_fini(&options)!=RMW_RET_OK){options_live=0;goto done;}options_live=0;
 node=rmw_create_node(&context,"checked_rmw_service","/");if(!node)goto done;
 rmw_qos_profile_t qos=rmw_qos_profile_services_default;
 rosidl_service_type_support_t const*support=ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts);
 server=rmw_create_service(node,support,"/checked_rmw/add",&qos);if(!server)goto done;
 client=rmw_create_client(node,support,"/checked_rmw/add",&qos);if(!client)goto done;
 wait=rmw_create_wait_set(&context,2);if(!wait)goto done;
 example_interfaces__srv__AddTwoInts_Request request={.a=10,.b=32};int64_t local_sequence=-1;
 if(rmw_send_request(client,&request,&local_sequence)!=RMW_RET_OK||local_sequence<=0)goto done;
 /* Only actual sources/real measured finite waits progress work. No graph-list,
  * loop iteration or stdout credit is claimed to be native readiness/time. */
 for(unsigned pass=0;pass<128&&!completed;++pass){
  void*sc[]={server->data},*cc[]={client->data};rmw_services_t ss={1,sc};rmw_clients_t cs={1,cc};rmw_time_t timeout={0,1000000};
  rmw_ret_t rc=rmw_wait(NULL,NULL,&ss,&cs,NULL,wait,&timeout);if(rc==RMW_RET_TIMEOUT)continue;if(rc!=RMW_RET_OK)goto done;
  if(sc[0]){rmw_service_info_t info;example_interfaces__srv__AddTwoInts_Request incoming={0};bool taken=false;
   if(rmw_take_request(server,&info,&incoming,&taken)!=RMW_RET_OK)goto done;
   if(taken){if(sent_response||incoming.a!=10||incoming.b!=32)goto done;
    example_interfaces__srv__AddTwoInts_Response answer={.sum=42};
    if(rmw_send_response(server,&info.request_id,&answer)!=RMW_RET_OK)goto done;sent_response=true;
   }
  }
  if(cc[0]){rmw_service_info_t info;example_interfaces__srv__AddTwoInts_Response response={0};bool taken=false;
   if(rmw_take_response(client,&info,&response,&taken)!=RMW_RET_OK)goto done;
   if(taken){if(!sent_response||info.request_id.sequence_number!=local_sequence||response.sum!=42)goto done;
    uwvm_rmw_c_service_diagnostics native;if(uwvm_rmw_c_client_diagnostics(client,&native)!=RMW_RET_OK||!native.actual_native_info_present||!native.gateway_ticket)goto done;
    completed=true;
   }
  }
 }
 if(completed&&sent_response)status=0;
 done:
 if(wait&&rmw_destroy_wait_set(wait)!=RMW_RET_OK)++cleanup_errors;
 if(client&&rmw_destroy_client(node,client)!=RMW_RET_OK)++cleanup_errors;
 if(server&&rmw_destroy_service(node,server)!=RMW_RET_OK)++cleanup_errors;
 if(node&&rmw_destroy_node(node)!=RMW_RET_OK)++cleanup_errors;
 if(context_live){if(rmw_shutdown(&context)!=RMW_RET_OK)++cleanup_errors;if(rmw_context_fini(&context)!=RMW_RET_OK)++cleanup_errors;}
 if(options_live&&rmw_init_options_fini(&options)!=RMW_RET_OK)++cleanup_errors;
 if(cleanup_errors)status=1;
 if(!status)puts("PASS_RMW_SERVICE_FULL_VALUE_AND_CLEANUP_NOT_ALL_RMW_OR_VENDOR_ACCEPTANCE");
 return status;
}
