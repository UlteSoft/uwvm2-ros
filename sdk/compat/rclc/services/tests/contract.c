/* Authored LOCAL model contracts, not native ROS/ABI/Wasm execution evidence.
 * Compile with the real compat.c and this finite bridge only after peer. */
#include <rclc/rclc.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
static unsigned groups,checks;
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"FAIL_RCLC_SERVICE_MODEL line=%u\n",(unsigned)__LINE__);abort();}}while(0)
static uint64_t now,last_id;static unsigned next_handle,opens,closes,submits,responses,callbacks,driver_calls;
static unsigned fail_open,fail_submit,fail_close,allocation_calls,fail_allocation,active_allocations;
static bool configured=true,without_clock;
static struct {bool live;unsigned role,kind;uint64_t watched;unsigned char bytes[512];size_t length;bool queued;} handles[257];
static unsigned char last_response[512],last_request[512];static size_t last_response_size,last_request_size;
static void put(void*p,uint64_t v){unsigned char*b=p;uwvm_ros_store_u32(b,(uint32_t)v);uwvm_ros_store_u32(b+4,(uint32_t)(v>>32));}
static void slot(uint64_t p,uint64_t v){put((void*)(uintptr_t)p,v);}
static void reset(void){CHECK(active_allocations==0);memset(handles,0,sizeof handles);now=0;last_id=UINT64_MAX;next_handle=opens=closes=submits=responses=callbacks=driver_calls=fail_open=fail_submit=fail_close=allocation_calls=fail_allocation=0;configured=true;without_clock=false;last_response_size=last_request_size=0;}
static uint32_t abi(void){return 1;}
static uint32_t open_role(uint32_t mem,uint64_t p,uint64_t n,uint64_t out){char a[64];unsigned role=0,kind=0,h;CHECK(mem==0&&n<sizeof a);memcpy(a,(void*)(uintptr_t)p,(size_t)n);a[n]=0;++opens;if(fail_open&&opens==fail_open)return 13;
 if(!strcmp(a,"steady")){role=5;}else{if(!strncmp(a,"call-",5))role=1;else if(!strncmp(a,"obs-",4))role=2;else if(!strncmp(a,"srv-",4))role=3;else if(!strncmp(a,"res-",4))role=4;kind=a[strlen(a)-1]=='a'?1:a[strlen(a)-1]=='t'?2:a[strlen(a)-1]=='b'?3:0;}
 if(!role||(!kind&&role!=5))return 13;CHECK(next_handle<256);h=++next_handle;handles[h].live=true;handles[h].role=role;handles[h].kind=kind;slot(out,h);return 0;}
static uint32_t close_role(uint64_t h){CHECK(h&&h<=next_handle&&handles[h].live);handles[h].live=false;++closes;return fail_close?107:0;}
static uint32_t advertise(uint64_t h){CHECK(h&&handles[h].live&&handles[h].role==3);return 0;}
static uint32_t legacy_poll(uint64_t h,uint32_t mem,uint64_t out,uint64_t cap,uint64_t len){CHECK(mem==0&&handles[h].live&&handles[h].role==3);if(!handles[h].queued)return 11;if(cap<handles[h].length)return 90;memcpy((void*)(uintptr_t)out,handles[h].bytes,handles[h].length);slot(len,handles[h].length);handles[h].queued=false;return 0;}
static uint32_t respond(uint64_t h,uint32_t mem,uint64_t in,uint64_t n,uint64_t out,uint64_t cap,uint64_t len){CHECK(h&&handles[h].live&&handles[h].role==4&&mem==0&&out==0&&cap==0&&n<=sizeof last_response);++responses;memcpy(last_response,(void*)(uintptr_t)in,(size_t)n);last_response_size=(size_t)n;slot(len,0);return 0;}
static uint32_t clock_request(uint64_t h,uint32_t mem,uint64_t in,uint64_t n,uint64_t out,uint64_t cap,uint64_t len){(void)in;CHECK(h&&handles[h].live&&handles[h].role==5&&mem==0&&!n&&cap==8);slot(out,now);slot(len,8);return 0;}
static uint32_t watch(uint64_t obs,uint64_t source,uint64_t correlation){CHECK(handles[obs].live&&handles[obs].role==2&&handles[source].live&&handles[source].role==1&&!correlation);CHECK(!handles[obs].watched);handles[obs].watched=source;return 0;}
static void event(uint64_t obs,uint16_t kind,uint64_t id,uint64_t sequence,void const*body,size_t n,unsigned error,unsigned flags){unsigned char*b=handles[obs].bytes;CHECK(!handles[obs].queued&&64+n<=sizeof handles[obs].bytes);memset(b,0,64);uwvm_ros_store_u32(b,0x45525755u);b[4]=1;b[6]=(unsigned char)kind;b[8]=2;b[9]=(unsigned char)flags;uwvm_ros_store_u32(b+12,error);put(b+16,handles[obs].watched);put(b+24,id);put(b+32,sequence);put(b+40,51);put(b+48,7);put(b+56,n);if(n)memcpy(b+64,body,n);handles[obs].length=64+n;handles[obs].queued=true;}
static uint32_t submit(uint64_t source,uint64_t obs,uint32_t mem,uint64_t in,uint64_t n,uint64_t out){CHECK(mem==0&&handles[source].live&&handles[obs].live&&handles[obs].watched==source);++submits;CHECK(n<=sizeof last_request);if(n)memcpy(last_request,(void*)(uintptr_t)in,(size_t)n);last_request_size=(size_t)n;if(fail_submit)return fail_submit;slot(out,last_id--);return 0;}
static uint32_t event_poll(uint64_t obs,uint32_t mem,uint64_t out,uint64_t cap,uint64_t len){CHECK(mem==0&&handles[obs].live&&handles[obs].role==2);if(!handles[obs].queued)return 11;if(cap<handles[obs].length){slot(len,handles[obs].length);return 90;}memcpy((void*)(uintptr_t)out,handles[obs].bytes,handles[obs].length);slot(len,handles[obs].length);handles[obs].queued=false;return 0;}
static uint32_t step(void*ctx,uint64_t timeout){(void)ctx;(void)timeout;++driver_calls;now+=100000000u;return 0;}
static roswasmc_backend const backend={.version=2,.abi_version=abi,.open=open_role,.close=close_role,.poll=legacy_poll,.advertise_service=advertise,.request={[ROSWASMC_SERVICE_RESPOND]=respond,[ROSWASMC_TIME_READ_STEADY]=clock_request}};
static roswasmc_event_backend const events={.version=2,.watch=watch,.poll=event_poll,.submit=submit};
#define Q {RMW_QOS_POLICY_HISTORY_KEEP_LAST,10,RMW_QOS_POLICY_RELIABILITY_RELIABLE,RMW_QOS_POLICY_DURABILITY_VOLATILE,{0,0},{0,0},RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,{0,0},false}
static uwvm_rclc_service_binding const bindings[]={
 {"service-model","","/add","example_interfaces/srv/AddTwoInts",UWVM_RCLC_SERVICE_CLIENT,"call-a","obs-a",Q,128,4},
 {"service-model","","/trigger","std_srvs/srv/Trigger",UWVM_RCLC_SERVICE_CLIENT,"call-t","obs-t",Q,128,4},
 {"service-model","","/bool","std_srvs/srv/SetBool",UWVM_RCLC_SERVICE_CLIENT,"call-b","obs-b",Q,128,4},
 {"service-model","","/add","example_interfaces/srv/AddTwoInts",UWVM_RCLC_SERVICE_SERVER,"srv-a","res-a",Q,128,4},
 {"service-model","","/trigger","std_srvs/srv/Trigger",UWVM_RCLC_SERVICE_SERVER,"srv-t","res-t",Q,128,4},
 {"service-model","","/bool","std_srvs/srv/SetBool",UWVM_RCLC_SERVICE_SERVER,"srv-b","res-b",Q,128,4}};
static struct uwvm_rclc_service_profile profile={&events,bindings,6,3000000000u,32};
static uwvm_rclc_node_binding const nodes[]={{"service-model",""}};
static uwvm_rclc_selection selection={&backend,NULL,0,nodes,1,"steady",step,NULL,128,&profile};
uwvm_rclc_selection const*uwvm_rclc_selected_configuration(void){selection.steady_clock_alias=without_clock?NULL:"steady";return configured?&selection:NULL;}
static void*allocate(size_t n,void*state){void*p;(void)state;++allocation_calls;if(allocation_calls==fail_allocation)return NULL;p=malloc(n);if(p)++active_allocations;return p;}
static void*zero_allocate(size_t n,size_t w,void*state){void*p;if(n&&w>SIZE_MAX/n)return NULL;p=allocate(n*w,state);if(p)memset(p,0,n*w);return p;}
static void deallocate(void*p,void*state){(void)state;if(p){CHECK(active_allocations>0);--active_allocations;free(p);}}
static void*reallocate(void*p,size_t n,void*state){void*q;(void)state;if(!p)return allocate(n,state);++allocation_calls;if(allocation_calls==fail_allocation)return NULL;q=realloc(p,n);return q;}
static rcl_allocator_t allocator={allocate,deallocate,reallocate,zero_allocate,NULL};
typedef struct {rclc_support_t support;rcl_node_t node;rclc_executor_t executor;} fixture;
static void start(fixture*f){memset(f,0,sizeof*f);CHECK(rclc_support_init(&f->support,0,NULL,&allocator)==0);CHECK(rclc_node_init_default(&f->node,"service-model","",&f->support)==0);CHECK(rclc_executor_init(&f->executor,&f->support.context,4,&allocator)==0);}
static void stop(fixture*f){CHECK(rclc_executor_fini(&f->executor)==0);CHECK(rcl_node_fini(&f->node)==0);if(rcl_context_is_valid(&f->support.context))CHECK(rcl_shutdown(&f->support.context)==0);CHECK(rclc_support_fini(&f->support)==0||fail_close);CHECK(active_allocations==0);}
static rosidl_service_type_support_t const*type(unsigned k){return k==1?ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces,srv,AddTwoInts):k==2?ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,Trigger):ROSIDL_GET_SRV_TYPE_SUPPORT(std_srvs,srv,SetBool);}
static void got(void const*message){CHECK(message);++callbacks;}
static uint64_t ticket(int64_t sequence){uint64_t id;memcpy(&id,&sequence,8);return id;}
static unsigned observer(void){unsigned i;for(i=1;i<=next_handle;++i)if(handles[i].role==2&&handles[i].live)return i;abort();}
static unsigned server_source(void){unsigned i;for(i=1;i<=next_handle;++i)if(handles[i].role==3&&handles[i].live)return i;abort();}
static void request_add(unsigned source,uint64_t id,int64_t a,int64_t b){uint64_t x,y;memcpy(&x,&a,8);memcpy(&y,&b,8);put(handles[source].bytes,id);put(handles[source].bytes+8,x);put(handles[source].bytes+16,y);handles[source].length=24;handles[source].queued=true;}
static void case_full64_response(void){fixture f;rcl_client_t c=rcl_get_zero_initialized_client();example_interfaces__srv__AddTwoInts_Request q={24,42};example_interfaces__srv__AddTwoInts_Response r={0};unsigned char body[8];int64_t sequence;unsigned state;uint32_t error;reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_client(&f.executor,&c,&r,got)==0);CHECK(rcl_send_request(&c,&q,&sequence)==0&&ticket(sequence)==UINT64_MAX);CHECK(last_request_size==16&&uwvm_ros_load_u64(last_request)==24&&uwvm_ros_load_u64(last_request+8)==42);put(body,66);event(observer(),3,ticket(sequence),1,body,8,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)==0&&callbacks==1&&r.sum==66);CHECK(uwvm_rclc_request_status(&c,ticket(sequence),&state,&error)==0&&state==3&&!error);CHECK(rcl_client_fini(&c,&f.node)==0&&closes==2);stop(&f);++groups;}
static void case_malformed_tombstone(void){fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={1,2};example_interfaces__srv__AddTwoInts_Response r={0};int64_t sequence;unsigned char bytes[8]={0};reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_client(&f.executor,&c,&r,got)==0);CHECK(rcl_send_request(&c,&q,&sequence)==0);CHECK(uwvm_rclc_remove_pending_request(&c,ticket(sequence))==0);event(observer(),3,ticket(sequence),1,bytes,7,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==0&&f.support.context.uwvm_diagnostics.raw_primary==22);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static void case_valid_tombstone_then_duplicate(void){fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={1,2};example_interfaces__srv__AddTwoInts_Response r={0};int64_t sequence;unsigned state;uint32_t error;unsigned char bytes[8];reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_client(&f.executor,&c,&r,got)==0);CHECK(rcl_send_request(&c,&q,&sequence)==0);CHECK(uwvm_rclc_remove_pending_request(&c,ticket(sequence))==0);put(bytes,3);event(observer(),3,ticket(sequence),1,bytes,8,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)==RCL_RET_TIMEOUT&&callbacks==0);CHECK(uwvm_rclc_request_status(&c,ticket(sequence),&state,&error)==0&&state==2&&error==125);event(observer(),3,ticket(sequence),2,bytes,8,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&f.support.context.uwvm_diagnostics.raw_primary==22);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static void case_bad_event(unsigned variant){fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={1,2};example_interfaces__srv__AddTwoInts_Response r={0};int64_t sequence;unsigned char bytes[9]={0};unsigned obs;reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_client(&f.executor,&c,&r,got)==0);CHECK(rcl_send_request(&c,&q,&sequence)==0);obs=observer();event(obs,3,variant==0?3:ticket(sequence),variant==1?2:1,bytes,variant==2?9:8,0,0);if(variant==3)handles[obs].bytes[8]=1;if(variant==4)put(handles[obs].bytes+16,155);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==0&&f.support.context.uwvm_diagnostics.raw_primary==22);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static void case_retirement(void){fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={1,2};int64_t a,b;example_interfaces__srv__AddTwoInts_Response r={0};unsigned state;uint32_t error;reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_client(&f.executor,&c,&r,got)==0);CHECK(rcl_send_request(&c,&q,&a)==0&&rcl_send_request(&c,&q,&b)==0);event(observer(),7,0,1,NULL,0,0,1);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==0);CHECK(uwvm_rclc_request_status(&c,ticket(a),&state,&error)==0&&state==4&&error==9);CHECK(uwvm_rclc_request_status(&c,ticket(b),&state,&error)==0&&state==4&&error==9);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static fixture*borrowed;static rcl_service_t*borrowed_service;
static void server_callback(void const*request,void*response){example_interfaces__srv__AddTwoInts_Request const*q=request;example_interfaces__srv__AddTwoInts_Response*r=response;rcl_client_t other={0};unsigned before=opens;++callbacks;CHECK(rcl_service_fini(borrowed_service,&borrowed->node)!=0);CHECK(rcl_node_fini(&borrowed->node)!=0);CHECK(rcl_shutdown(&borrowed->support.context)!=0);CHECK(rclc_client_init_default(&other,&borrowed->node,type(1),"/add")!=0&&opens==before);r->sum=q->a+q->b;}
static void case_server_and_group(void){fixture f;rcl_service_t s={0};example_interfaces__srv__AddTwoInts_Request q={0};example_interfaces__srv__AddTwoInts_Response r={0};reset();start(&f);borrowed=&f;borrowed_service=&s;CHECK(rclc_service_init_default(&s,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_service(&f.executor,&s,&q,&r,server_callback)==0);request_add(server_source(),UINT64_MAX,24,42);CHECK(rclc_executor_spin_some(&f.executor,0)==0&&callbacks==1&&responses==1&&last_response_size==16);CHECK(uwvm_ros_load_u64(last_response)==UINT64_MAX&&uwvm_ros_load_u64(last_response+8)==66);CHECK(rcl_service_fini(&s,&f.node)==0&&closes==2);CHECK(rclc_executor_spin_some(&f.executor,0)!=0);stop(&f);++groups;}
static void case_server_overflow(void){fixture f;rcl_service_t s={0};example_interfaces__srv__AddTwoInts_Request q={0};example_interfaces__srv__AddTwoInts_Response r={0};reset();start(&f);borrowed=&f;borrowed_service=&s;CHECK(rclc_service_init_default(&s,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_service(&f.executor,&s,&q,&r,server_callback)==0);request_add(server_source(),1,INT64_MAX,1);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==0&&responses==0);CHECK(rcl_service_fini(&s,&f.node)==0);stop(&f);++groups;}
static void case_rollback_and_allocations(void){unsigned n;for(n=1;n<=3;++n){fixture f;rcl_client_t c={0};reset();start(&f);fail_allocation=allocation_calls+n;CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")!=0&&opens==0);stop(&f);} {fixture f;rcl_client_t c={0};reset();start(&f);fail_open=2;fail_close=1;CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")!=0&&closes==1&&f.support.context.uwvm_diagnostics.raw_primary==13&&f.support.context.uwvm_diagnostics.cleanup.cleanup_error==107);stop(&f);}++groups;}
static void case_submit_no_replay(void){fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={1,2};int64_t id=17;reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);fail_submit=11;CHECK(rcl_send_request(&c,&q,&id)!=0&&id==17&&submits==1);fail_submit=110;CHECK(rcl_send_request(&c,&q,&id)!=0&&id==17&&submits==2);fail_submit=0;CHECK(rcl_send_request(&c,&q,&id)!=0&&submits==2);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static void case_sleep_and_unsupported(void){fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={1,2};int64_t id=7;rmw_request_id_t header;bool available=true;reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);CHECK(rcl_take_response(&c,&header,&q)==RCL_RET_UNSUPPORTED&&submits==0);CHECK(rcl_service_server_is_available(&f.node,&c,&available)==RCL_RET_UNSUPPORTED&&available);rclc_sleep_ms(2000);CHECK(!f.support.context.uwvm_sleep_error&&now>=2000000000u&&driver_calls>=20);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);reset();without_clock=true;start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);rclc_sleep_ms(2000);CHECK(f.support.context.uwvm_sleep_error==13);CHECK(rcl_send_request(&c,&q,&id)!=0&&submits==0);CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static void case_string_records(void){std_srvs__srv__Trigger_Response a,b;std_srvs__srv__SetBool_Response__Sequence x={0},y={0};CHECK(std_srvs__srv__Trigger_Response__init(&a)&&std_srvs__srv__Trigger_Response__init(&b));a.success=true;CHECK(rosidl_runtime_c__String__assign(&a.message,"complete"));CHECK(std_srvs__srv__Trigger_Response__copy(&a,&b)&&std_srvs__srv__Trigger_Response__are_equal(&a,&b));CHECK(std_srvs__srv__SetBool_Response__Sequence__init(&x,2));CHECK(rosidl_runtime_c__String__assign(&x.data[1].message,"all fields"));CHECK(std_srvs__srv__SetBool_Response__Sequence__copy(&x,&y)&&std_srvs__srv__SetBool_Response__Sequence__are_equal(&x,&y));std_srvs__srv__Trigger_Response__fini(&a);std_srvs__srv__Trigger_Response__fini(&b);std_srvs__srv__SetBool_Response__Sequence__fini(&x);std_srvs__srv__SetBool_Response__Sequence__fini(&y);++groups;}
static void case_complete_text_response(unsigned kind){fixture f;rcl_client_t c={0};std_srvs__srv__Trigger_Request tq={91};std_srvs__srv__SetBool_Request bq={true};std_srvs__srv__Trigger_Response tr;std_srvs__srv__SetBool_Response br;void*response=kind==2?(void*)&tr:(void*)&br;int64_t id;unsigned char body[]={1,5,0,0,0,'r','e','a','d','y'};reset();start(&f);CHECK(kind==2?std_srvs__srv__Trigger_Response__init(&tr):std_srvs__srv__SetBool_Response__init(&br));CHECK(rclc_client_init_default(&c,&f.node,type(kind),kind==2?"/trigger":"/bool")==0);CHECK(rclc_executor_add_client(&f.executor,&c,response,got)==0);CHECK(rcl_send_request(&c,kind==2?(void*)&tq:(void*)&bq,&id)==0);CHECK(kind==2?last_request_size==0:(last_request_size==1&&last_request[0]==1));event(observer(),3,ticket(id),1,body,sizeof body,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)==0&&callbacks==1);if(kind==2){CHECK(tr.success&&tr.message.size==5&&!memcmp(tr.message.data,"ready",5));std_srvs__srv__Trigger_Response__fini(&tr);}else{CHECK(br.success&&br.message.size==5&&!memcmp(br.message.data,"ready",5));std_srvs__srv__SetBool_Response__fini(&br);}CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;}
static void fail_callback(void const*q,void*r){(void)q;(void)r;++callbacks;CHECK(uwvm_rclc_service_callback_error(borrowed_service,12)==0);}
static void case_explicit_callback_failure(void){fixture f;rcl_service_t s={0};example_interfaces__srv__AddTwoInts_Request q={0};example_interfaces__srv__AddTwoInts_Response r={0};reset();start(&f);borrowed_service=&s;CHECK(rclc_service_init_default(&s,&f.node,type(1),"/add")==0);CHECK(rclc_executor_add_service(&f.executor,&s,&q,&r,fail_callback)==0);request_add(server_source(),1,24,42);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==1&&responses==0&&f.support.context.uwvm_diagnostics.raw_primary==12);CHECK(rcl_service_fini(&s,&f.node)==0);stop(&f);++groups;}

/* Full actual canonical response fields for both dynamic-string Services;
 * no response marker substitutes for these byte-for-byte observations. */
static void text_server_callback(void const*q,void*r,void*kind_pointer){
 unsigned kind=*(unsigned*)kind_pointer;++callbacks;
 if(kind==2){std_srvs__srv__Trigger_Response*value=r;(void)q;value->success=true;CHECK(rosidl_runtime_c__String__assign(&value->message,"ready"));}
 else{std_srvs__srv__SetBool_Request const*request=q;std_srvs__srv__SetBool_Response*value=r;CHECK(request->data);value->success=true;CHECK(rosidl_runtime_c__String__assign(&value->message,"set:true"));}
}
static void case_complete_text_server(unsigned kind){
 fixture f;rcl_service_t s={0};std_srvs__srv__Trigger_Request tq={71};std_srvs__srv__SetBool_Request bq={false};std_srvs__srv__Trigger_Response tr;std_srvs__srv__SetBool_Response br;unsigned source;roswasmc_service_response decoded;
 void*request=kind==2?(void*)&tq:(void*)&bq;void*response=kind==2?(void*)&tr:(void*)&br;
 reset();start(&f);CHECK(kind==2?std_srvs__srv__Trigger_Response__init(&tr):std_srvs__srv__SetBool_Response__init(&br));
 CHECK(rclc_service_init_default(&s,&f.node,type(kind),kind==2?"/trigger":"/bool")==0);
 CHECK(rclc_executor_add_service_with_context(&f.executor,&s,request,response,text_server_callback,&kind)==0);
 source=server_source();put(handles[source].bytes,UINT64_MAX);handles[source].length=8;if(kind==3){handles[source].bytes[8]=1;++handles[source].length;}handles[source].queued=true;
 CHECK(rclc_executor_spin_some(&f.executor,0)==0&&callbacks==1&&responses==1&&uwvm_ros_load_u64(last_response)==UINT64_MAX);
 CHECK(roswasmc_decode_service_response(last_response+8,last_response_size-8,&decoded)==0&&decoded.success);
 CHECK(kind==2?(decoded.message.size==5&&!memcmp(decoded.message.data,"ready",5)):(decoded.message.size==8&&!memcmp(decoded.message.data,"set:true",8)));
 if(kind==2)CHECK(tq.structure_needs_at_least_one_member==0);else CHECK(bq.data);
 CHECK(rcl_service_fini(&s,&f.node)==0);if(kind==2)std_srvs__srv__Trigger_Response__fini(&tr);else std_srvs__srv__SetBool_Response__fini(&br);stop(&f);++groups;
}
static void case_bad_text_response(unsigned variant){
 fixture f;rcl_client_t c={0};std_srvs__srv__Trigger_Request q={0};std_srvs__srv__Trigger_Response r;int64_t id;unsigned char body[129]={1,1,0,0,0,'x'};size_t n=6;
 reset();start(&f);CHECK(std_srvs__srv__Trigger_Response__init(&r));CHECK(rosidl_runtime_c__String__assign(&r.message,"unchanged"));
 CHECK(rclc_client_init_default(&c,&f.node,type(2),"/trigger")==0&&rclc_executor_add_client(&f.executor,&c,&r,got)==0&&rcl_send_request(&c,&q,&id)==0);
 if(variant==0)body[0]=2;else if(variant==1)body[5]=0xff;else if(variant==2)n=5;else if(variant==3)n=7;else n=129;
 event(observer(),3,ticket(id),1,body,n,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==0);
 CHECK(r.message.size==9&&!memcmp(r.message.data,"unchanged",9)&&!r.success);
 CHECK(f.support.context.uwvm_diagnostics.raw_primary==(variant==4?90u:22u));
 CHECK(rcl_client_fini(&c,&f.node)==0);std_srvs__srv__Trigger_Response__fini(&r);stop(&f);++groups;
}
static void case_lifetime_budget_and_context_cleanup(void){
 fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={24,42};int64_t ids[5]={0,0,0,0,23};unsigned i;reset();start(&f);
 CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0);for(i=0;i<4;++i)CHECK(rcl_send_request(&c,&q,ids+i)==0);
 CHECK(rcl_send_request(&c,&q,ids+4)!=0&&submits==4&&ids[4]==23&&f.support.context.uwvm_diagnostics.raw_primary==90);
 CHECK(rclc_executor_fini(&f.executor)==0);CHECK(rcl_shutdown(&f.support.context)==0);CHECK(rclc_support_fini(&f.support)==0&&closes==2&&active_allocations==0);
 CHECK(!rcl_client_is_valid(&c));/* Context fini releases additive heap even when caller omitted client fini. */
 ++groups;
}


static void case_per_request_error_keeps_other_pending(void){
 fixture f;rcl_client_t c={0};example_interfaces__srv__AddTwoInts_Request q={24,42};example_interfaces__srv__AddTwoInts_Response r={0};int64_t first,second;unsigned state;uint32_t error;unsigned char body[8];
 reset();start(&f);CHECK(rclc_client_init_default(&c,&f.node,type(1),"/add")==0&&rclc_executor_add_client(&f.executor,&c,&r,got)==0);
 CHECK(rcl_send_request(&c,&q,&first)==0&&rcl_send_request(&c,&q,&second)==0);
 event(observer(),6,ticket(first),1,NULL,0,110,2);CHECK(rclc_executor_spin_some(&f.executor,0)!=0&&callbacks==0&&rcl_client_is_valid(&c));
 CHECK(uwvm_rclc_request_status(&c,ticket(first),&state,&error)==0&&state==4&&error==110);
 CHECK(uwvm_rclc_request_status(&c,ticket(second),&state,&error)==0&&state==1&&!error);
 put(body,66);event(observer(),3,ticket(second),2,body,8,0,0);CHECK(rclc_executor_spin_some(&f.executor,0)==0&&callbacks==1&&r.sum==66);
 CHECK(rcl_client_fini(&c,&f.node)==0);stop(&f);++groups;
}

int main(void){unsigned i;case_full64_response();case_malformed_tombstone();case_valid_tombstone_then_duplicate();for(i=0;i<5;++i)case_bad_event(i);case_retirement();case_server_and_group();case_server_overflow();case_rollback_and_allocations();case_submit_no_replay();case_sleep_and_unsupported();case_string_records();case_complete_text_response(2);case_complete_text_response(3);case_explicit_callback_failure();case_complete_text_server(2);case_complete_text_server(3);for(i=0;i<5;++i)case_bad_text_response(i);case_lifetime_budget_and_context_cleanup();case_per_request_error_keeps_other_pending();printf("PASS_RCLC_SERVICE_LOCAL_MODELS groups=%u checks=%u NOT_NATIVE_ROS_OR_WASM\n",groups,checks);return 0;}
