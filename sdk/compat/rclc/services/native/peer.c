/* Genuine INSTALLED native rcl/rclc + generated C Service peer. Build without
 * sdk/compat/rclc/include, compat.c or services.inc. This is NOT a local bridge.
 * No execution is claimed. The future external owner must qualify full SDK,
 * real Service identity graphs, MD/ELF/maps and every I/M/P/F/C checkpoint. */
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <example_interfaces/srv/add_two_ints.h>
#include <std_srvs/srv/trigger.h>
#include <std_srvs/srv/set_bool.h>
#include <rosidl_runtime_c/string_functions.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned values[3],bad;
static void add_request(void const*q,void*r){example_interfaces__srv__AddTwoInts_Request const*a=q;example_interfaces__srv__AddTwoInts_Response*b=r;if(a->a!=24||a->b!=42){++bad;b->sum=0;return;}b->sum=66;++values[0];}
static void trigger_request(void const*q,void*r){std_srvs__srv__Trigger_Response*b=r;(void)q;if(!rosidl_runtime_c__String__assign(&b->message,"ready")){++bad;b->success=false;return;}b->success=true;++values[1];}
static void bool_request(void const*q,void*r){std_srvs__srv__SetBool_Request const*a=q;std_srvs__srv__SetBool_Response*b=r;if(!a->data||!rosidl_runtime_c__String__assign(&b->message,"set:true")){++bad;b->success=false;return;}b->success=true;++values[2];}
static void add_result(void const*r){example_interfaces__srv__AddTwoInts_Response const*b=r;if(b->sum!=66)++bad;else ++values[0];}
static void trigger_result(void const*r){std_srvs__srv__Trigger_Response const*b=r;if(!b->success||b->message.size!=5||memcmp(b->message.data,"ready",5))++bad;else ++values[1];}
static void bool_result(void const*r){std_srvs__srv__SetBool_Response const*b=r;if(!b->success||b->message.size!=8||memcmp(b->message.data,"set:true",8))++bad;else ++values[2];}
static int gate(int expected){return getchar()==expected&&getchar()=='\n';}
int main(int argc,char const*argv[]){
 rcl_allocator_t allocator=rcl_get_default_allocator();rclc_support_t support;rcl_node_t node=rcl_get_zero_initialized_node();rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();rcl_client_t clients[3];rcl_service_t servers[3];
 example_interfaces__srv__AddTwoInts_Request aq={0};example_interfaces__srv__AddTwoInts_Response ar={0};std_srvs__srv__Trigger_Request tq={0};std_srvs__srv__Trigger_Response tr={0};std_srvs__srv__SetBool_Request bq={0};std_srvs__srv__SetBool_Response br={0};
 void*requests[]={&aq,&tq,&bq};void*responses[]={&ar,&tr,&br};char const*names[]={"/rclc_profile/add","/rclc_profile/trigger","/rclc_profile/bool"};
 rosidl_service_type_support_t const*types[]={ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts),ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,Trigger),ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,SetBool)};
 rclc_service_callback_t service_callbacks[]={add_request,trigger_request,bool_request};rclc_client_callback_t client_callbacks[]={add_result,trigger_result,bool_result};
 bool support_live=false,node_live=false,executor_live=false,client_live[3]={false,false,false},server_live[3]={false,false,false},messages[6]={false,false,false,false,false,false};
 rcl_ret_t primary=0,cleanup=0,rc;int codes[12]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};unsigned i,tick;bool client,available[3]={false,false,false};int64_t native_sequence[3]={0,0,0};
 if(argc!=2||strcmp(argv[1],"client")&&strcmp(argv[1],"server"))return 2;client=!strcmp(argv[1],"client");
 for(i=0;i<3;++i){clients[i]=rcl_get_zero_initialized_client();servers[i]=rcl_get_zero_initialized_service();}
 if(!example_interfaces__srv__AddTwoInts_Request__init(&aq)){primary=RCL_RET_BAD_ALLOC;goto done;}messages[0]=true;
 if(!example_interfaces__srv__AddTwoInts_Response__init(&ar)){primary=RCL_RET_BAD_ALLOC;goto done;}messages[1]=true;
 if(!std_srvs__srv__Trigger_Request__init(&tq)){primary=RCL_RET_BAD_ALLOC;goto done;}messages[2]=true;
 if(!std_srvs__srv__Trigger_Response__init(&tr)){primary=RCL_RET_BAD_ALLOC;goto done;}messages[3]=true;
 if(!std_srvs__srv__SetBool_Request__init(&bq)){primary=RCL_RET_BAD_ALLOC;goto done;}messages[4]=true;
 if(!std_srvs__srv__SetBool_Response__init(&br)){primary=RCL_RET_BAD_ALLOC;goto done;}messages[5]=true;
 aq.a=24;aq.b=42;bq.data=true;
 if((primary=rclc_support_init(&support,0,NULL,&allocator)))goto done;support_live=true;
 if((primary=rclc_node_init_default(&node,"native_rclc_service_oracle","",&support)))goto done;node_live=true;
 if((primary=rclc_executor_init(&executor,&support.context,3,&allocator)))goto done;executor_live=true;
 for(i=0;i<3;++i){if(client){primary=rclc_client_init_default(clients+i,&node,types[i],names[i]);if(primary)goto done;client_live[i]=true;primary=rclc_executor_add_client(&executor,clients+i,responses[i],client_callbacks[i]);}
  else{primary=rclc_service_init_default(servers+i,&node,types[i],names[i]);if(primary)goto done;server_live[i]=true;primary=rclc_executor_add_service(&executor,servers+i,requests[i],responses[i],service_callbacks[i]);}if(primary)goto done;}
 if((primary=rclc_executor_prepare(&executor)))goto done;
 /* Actual middleware entities and all3 typed native Service supports were used
  * before the stopped-image proof. No business request is sent before I+M. */
 if(raise(SIGSTOP)||!gate('I')||!gate('M')){primary=RCL_RET_ERROR;goto done;}
 if(client){
  for(tick=0;tick<200&&(!available[0]||!available[1]||!available[2]);++tick){
   for(i=0;i<3;++i)if(!available[i]){primary=rcl_service_server_is_available(&node,clients+i,available+i);if(primary)goto done;}
   if(!available[0]||!available[1]||!available[2]){rc=rclc_executor_spin_some(&executor,10000000u);if(rc&&rc!=RCL_RET_TIMEOUT){primary=rc;goto done;}}}
  if(!available[0]||!available[1]||!available[2]){primary=RCL_RET_TIMEOUT;goto done;}
  for(i=0;i<3;++i)if((primary=rcl_send_request(clients+i,requests[i],native_sequence+i)))goto done;
 }
 for(tick=0;tick<200&&(!values[0]||!values[1]||!values[2])&&!bad;++tick){rc=rclc_executor_spin_some(&executor,10000000u);if(rc&&rc!=RCL_RET_TIMEOUT){primary=rc;goto done;}}
 if(!values[0]||!values[1]||!values[2]||bad){primary=RCL_RET_ERROR;goto done;}
 if(!gate('P')){primary=RCL_RET_ERROR;goto done;}
 printf("OBSERVED_NATIVE_RCLC_SERVICE_FULL_VALUE role=%s AddTwoInts={a:24,b:42,sum:66} Trigger={success:true,message:ready} SetBool={data:true,success:true,message:set:true} native_sequence=[%lld,%lld,%lld]\n",argv[1],(long long)native_sequence[0],(long long)native_sequence[1],(long long)native_sequence[2]);fflush(stdout);
 if(!gate('F')||!gate('C'))primary=RCL_RET_ERROR;
 done:
 /* Attempt every entered native resource, retain primary independently, never
  * retry a failed close and never infer native acceptance from the final rc. */
 if(executor_live){rc=rclc_executor_fini(&executor);codes[0]=rc;if(rc&&!cleanup)cleanup=rc;}
 for(i=3;i;){unsigned j=--i;if(client_live[j]){rc=rcl_client_fini(clients+j,&node);codes[1+j]=rc;if(rc&&!cleanup)cleanup=rc;}if(server_live[j]){rc=rcl_service_fini(servers+j,&node);codes[1+j]=rc;if(rc&&!cleanup)cleanup=rc;}}
 if(node_live){rc=rcl_node_fini(&node);codes[4]=rc;if(rc&&!cleanup)cleanup=rc;}
 if(support_live){rc=rclc_support_fini(&support);codes[5]=rc;if(rc&&!cleanup)cleanup=rc;}
 if(messages[5]){std_srvs__srv__SetBool_Response__fini(&br);codes[6]=0;}if(messages[4]){std_srvs__srv__SetBool_Request__fini(&bq);codes[7]=0;}if(messages[3]){std_srvs__srv__Trigger_Response__fini(&tr);codes[8]=0;}if(messages[2]){std_srvs__srv__Trigger_Request__fini(&tq);codes[9]=0;}if(messages[1]){example_interfaces__srv__AddTwoInts_Response__fini(&ar);codes[10]=0;}if(messages[0]){example_interfaces__srv__AddTwoInts_Request__fini(&aq);codes[11]=0;}
 fprintf(stderr,"NATIVE_RCLC_SERVICE_TERMINAL primary=%d cleanup=%d attempts=[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n",(int)primary,(int)cleanup,codes[0],codes[1],codes[2],codes[3],codes[4],codes[5],codes[6],codes[7],codes[8],codes[9],codes[10],codes[11]);return primary||cleanup?1:0;
}
