/* Complete three-Service Guest lesson, source-only until independently built
 * and run. argv: deny | client | server. The stdout facts alone never prove
 * native delivery; a separate native full-value oracle and retained owner are
 * required. Every setup/fini result is accounted for, without implicit replay. */
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <std_srvs/srv/trigger.h>
#include <std_srvs/srv/set_bool.h>
#include <stdio.h>
static rcl_service_t servers[3];static unsigned complete[3],bad;
static void add_callback(void const*q,void*r){example_interfaces__srv__AddTwoInts_Request const*a=q;example_interfaces__srv__AddTwoInts_Response*b=r;if(a->a!=24||a->b!=42){++bad;(void)uwvm_rclc_service_callback_error(servers,22);return;}b->sum=66;++complete[0];}
static void trigger_callback(void const*q,void*r){std_srvs__srv__Trigger_Response*b=r;(void)q;if(!rosidl_runtime_c__String__assign(&b->message,"ready")){++bad;(void)uwvm_rclc_service_callback_error(servers+1,12);return;}b->success=true;++complete[1];}
static void bool_callback(void const*q,void*r){std_srvs__srv__SetBool_Request const*a=q;std_srvs__srv__SetBool_Response*b=r;if(!a->data){++bad;(void)uwvm_rclc_service_callback_error(servers+2,22);return;}if(!rosidl_runtime_c__String__assign(&b->message,"set:true")){++bad;(void)uwvm_rclc_service_callback_error(servers+2,12);return;}b->success=true;++complete[2];}
static void add_response(void const*r){example_interfaces__srv__AddTwoInts_Response const*b=r;if(b->sum==66)++complete[0];else ++bad;}
static void trigger_response(void const*r){std_srvs__srv__Trigger_Response const*b=r;if(b->success&&b->message.size==5&&!memcmp(b->message.data,"ready",5))++complete[1];else ++bad;}
static void bool_response(void const*r){std_srvs__srv__SetBool_Response const*b=r;if(b->success&&b->message.size==8&&!memcmp(b->message.data,"set:true",8))++complete[2];else ++bad;}
int main(int argc,char const*argv[]){
 rcl_allocator_t allocator=rcl_get_default_allocator();rclc_support_t support;rcl_node_t node=rcl_get_zero_initialized_node();rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();rcl_client_t clients[3]={{0}};
 example_interfaces__srv__AddTwoInts_Request aq={24,42};example_interfaces__srv__AddTwoInts_Response ar={0};std_srvs__srv__Trigger_Request tq={0};std_srvs__srv__Trigger_Response tr={0};std_srvs__srv__SetBool_Request bq={true};std_srvs__srv__SetBool_Response br={0};
 void*requests[]={&aq,&tq,&bq};void*responses[]={&ar,&tr,&br};char const*names[]={"/rclc_profile/add","/rclc_profile/trigger","/rclc_profile/bool"};
 rosidl_service_type_support_t const*types[]={ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,Trigger),ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,SetBool)};
 rclc_service_callback_t service_callbacks[]={add_callback,trigger_callback,bool_callback};rclc_client_callback_t client_callbacks[]={add_response,trigger_response,bool_response};
 bool support_live=false,node_live_=false,executor_live_=false,text_live[2]={false,false},client_live[3]={false,false,false},server_live[3]={false,false,false};
 rcl_ret_t primary=0,cleanup=0,rc;int cleanup_codes[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};unsigned i,tick;bool client,deny;
 if(argc!=2||strcmp(argv[1],"deny")&&strcmp(argv[1],"client")&&strcmp(argv[1],"server"))return 2;deny=!strcmp(argv[1],"deny");client=strcmp(argv[1],"server")!=0;
 if((primary=rclc_support_init(&support,0,NULL,&allocator)))goto done;support_live=true;
 if((primary=rclc_node_init_default(&node,"checked-service","",&support)))goto done;node_live_=true;
 if(!std_srvs__srv__Trigger_Response__init(&tr)){primary=RCL_RET_BAD_ALLOC;goto done;}text_live[0]=true;
 if(!std_srvs__srv__SetBool_Response__init(&br)){primary=RCL_RET_BAD_ALLOC;goto done;}text_live[1]=true;
 /* All message objects, callback arrays, ownership flags and diagnostic slots
  * exist before the first resource open. Only initialized owners are closed. */
 if(deny){rc=rclc_client_init_default(clients,&node,types[0],names[0]);if(!rc){client_live[0]=true;primary=RCL_RET_ERROR;}else if(support.context.uwvm_diagnostics.raw_primary!=13)primary=RCL_RET_ERROR;else{for(i=0;i<support.context.uwvm_session.count;++i)if(support.context.uwvm_owners[i].handle)primary=RCL_RET_ERROR;if(!primary)printf("OBSERVED_RCLC_SERVICE_DEFAULT_DENIAL error=13 handles=0\n");}goto done;}
 if((primary=rclc_executor_init(&executor,&support.context,3,&allocator)))goto done;executor_live_=true;
 for(i=0;i<3;++i){
  if(client){primary=rclc_client_init_default(clients+i,&node,types[i],names[i]);if(primary)goto done;client_live[i]=true;primary=rclc_executor_add_client(&executor,clients+i,responses[i],client_callbacks[i]);}
  else{primary=rclc_service_init_default(servers+i,&node,types[i],names[i]);if(primary)goto done;server_live[i]=true;primary=rclc_executor_add_service(&executor,servers+i,requests[i],responses[i],service_callbacks[i]);}
  if(primary)goto done;
 }
 if((primary=rclc_executor_prepare(&executor)))goto done;
 if(client)for(i=0;i<3;++i){int64_t opaque_ticket;if((primary=rcl_send_request(clients+i,requests[i],&opaque_ticket)))goto done;/* No native-RMW identity inferred from this opaque bit pattern. */}
 for(tick=0;tick<128&&(!complete[0]||!complete[1]||!complete[2])&&!bad;++tick){rc=rclc_executor_spin_some(&executor,100000000u);if(rc&&rc!=RCL_RET_TIMEOUT){primary=rc;goto done;}}
 if(!complete[0]||!complete[1]||!complete[2]||bad){primary=RCL_RET_ERROR;goto done;}
 printf("OBSERVED_RCLC_SERVICE_FULL_VALUE role=%s AddTwoInts={a:24,b:42,sum:66} Trigger={success:true,message:ready} SetBool={data:true,success:true,message:set:true}\n",argv[1]);
 done:
 /* Native owners are individually attempted even when an earlier close fails. */
 if(executor_live_){rc=rclc_executor_fini(&executor);cleanup_codes[0]=rc;if(rc&&!cleanup)cleanup=rc;}
 for(i=3;i;){unsigned j=--i;if(client_live[j]){rc=rcl_client_fini(clients+j,&node);cleanup_codes[1+j]=rc;if(rc&&!cleanup)cleanup=rc;}if(server_live[j]){rc=rcl_service_fini(servers+j,&node);cleanup_codes[1+j]=rc;if(rc&&!cleanup)cleanup=rc;}}
 if(text_live[1]){std_srvs__srv__SetBool_Response__fini(&br);cleanup_codes[4]=0;}if(text_live[0]){std_srvs__srv__Trigger_Response__fini(&tr);cleanup_codes[5]=0;}
 if(node_live_){rc=rcl_node_fini(&node);cleanup_codes[6]=rc;if(rc&&!cleanup)cleanup=rc;}
 if(support_live){rc=rclc_support_fini(&support);cleanup_codes[7]=rc;if(rc&&!cleanup)cleanup=rc;}
 fprintf(stderr,"RCLC_SERVICE_CHECKED_TERMINAL primary=%d cleanup=%d attempts=[%d,%d,%d,%d,%d,%d,%d,%d]\n",(int)primary,(int)cleanup,cleanup_codes[0],cleanup_codes[1],cleanup_codes[2],cleanup_codes[3],cleanup_codes[4],cleanup_codes[5],cleanup_codes[6],cleanup_codes[7]);return primary||cleanup?1:0;
}
