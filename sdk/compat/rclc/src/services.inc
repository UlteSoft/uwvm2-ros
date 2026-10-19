/* Additive single-TU service adapter, included only by compat.c. No rcl/rmw
 * native objects are linked. Original declarations, licenses and schemas are
 * retained in services/upstream and services/README.md. */
#define SERVICE_TOKEN "uwvm_guest_source_token_not_native_typesupport"
#define DEFINE_SCALAR_RECORD(T,EQ) \
 bool T##__init(T*m){if(!m)return false;memset(m,0,sizeof*m);return true;} \
 void T##__fini(T*m){(void)m;} \
 bool T##__copy(T const*a,T*b){if(!a||!b)return false;*b=*a;return true;} \
 bool T##__are_equal(T const*a,T const*b){return a&&b&&(EQ);}
DEFINE_SCALAR_RECORD(example_interfaces__srv__AddTwoInts_Request,a->a==b->a&&a->b==b->b)
DEFINE_SCALAR_RECORD(example_interfaces__srv__AddTwoInts_Response,a->sum==b->sum)
DEFINE_SCALAR_RECORD(std_srvs__srv__Trigger_Request,a->structure_needs_at_least_one_member==b->structure_needs_at_least_one_member)
DEFINE_SCALAR_RECORD(std_srvs__srv__SetBool_Request,a->data==b->data)
#undef DEFINE_SCALAR_RECORD
#define DEFINE_TEXT_RECORD(T) \
 bool T##__init(T*m){if(!m)return false;m->success=false;memset(&m->message,0,sizeof m->message);return rosidl_runtime_c__String__init(&m->message);} \
 void T##__fini(T*m){if(m)rosidl_runtime_c__String__fini(&m->message);} \
 bool T##__copy(T const*a,T*b){rosidl_runtime_c__String temp={0};if(!a||!b)return false;if(a==b)return true; \
  if(!rosidl_runtime_c__String__init(&temp))return false; \
  if(!rosidl_runtime_c__String__copy(&a->message,&temp)){rosidl_runtime_c__String__fini(&temp);return false;} \
  rosidl_runtime_c__String__fini(&b->message);b->message=temp;b->success=a->success;return true;} \
 bool T##__are_equal(T const*a,T const*b){return a&&b&&a->success==b->success&&rosidl_runtime_c__String__are_equal(&a->message,&b->message);}
DEFINE_TEXT_RECORD(std_srvs__srv__Trigger_Response)
DEFINE_TEXT_RECORD(std_srvs__srv__SetBool_Response)
#undef DEFINE_TEXT_RECORD
/* Sequence copy builds a separate complete owned replacement before releasing
 * the destination. An allocation/copy failure preserves the old destination. */
#define DEFINE_RECORD_STORAGE(T) \
 T*T##__create(void){rcl_allocator_t a=rcutils_get_default_allocator();T*m=a.zero_allocate(1,sizeof*m,a.state);if(m&&!T##__init(m)){a.deallocate(m,a.state);return NULL;}return m;} \
 void T##__destroy(T*m){rcl_allocator_t a=rcutils_get_default_allocator();if(m)T##__fini(m);a.deallocate(m,a.state);} \
 bool T##__Sequence__init(T##__Sequence*s,size_t n){size_t i;rcl_allocator_t a=rcutils_get_default_allocator();T*p=NULL;if(!s||n>SIZE_MAX/sizeof(T))return false; \
  if(n){p=a.zero_allocate(n,sizeof(T),a.state);if(!p)return false;for(i=0;i<n;++i)if(!T##__init(p+i)){while(i)T##__fini(p+--i);a.deallocate(p,a.state);return false;}} \
  s->data=p;s->size=s->capacity=n;return true;} \
 void T##__Sequence__fini(T##__Sequence*s){size_t i;rcl_allocator_t a=rcutils_get_default_allocator();if(!s)return; \
  if((s->data&&(!s->capacity||s->size>s->capacity))||(!s->data&&(s->size||s->capacity)))abort(); \
  for(i=0;i<s->capacity;++i)T##__fini(s->data+i);a.deallocate(s->data,a.state);s->data=NULL;s->size=s->capacity=0;} \
 T##__Sequence*T##__Sequence__create(size_t n){rcl_allocator_t a=rcutils_get_default_allocator();T##__Sequence*s=a.zero_allocate(1,sizeof*s,a.state);if(s&&!T##__Sequence__init(s,n)){a.deallocate(s,a.state);return NULL;}return s;} \
 void T##__Sequence__destroy(T##__Sequence*s){rcl_allocator_t a=rcutils_get_default_allocator();if(s)T##__Sequence__fini(s);a.deallocate(s,a.state);} \
 bool T##__Sequence__copy(T##__Sequence const*a,T##__Sequence*b){T##__Sequence t={0};size_t i;if(!a||!b||a->size>a->capacity||(!a->data&&a->size))return false;if(a==b)return true; \
  if(!T##__Sequence__init(&t,a->size))return false;for(i=0;i<a->size;++i)if(!T##__copy(a->data+i,t.data+i)){T##__Sequence__fini(&t);return false;}T##__Sequence__fini(b);*b=t;return true;} \
 bool T##__Sequence__are_equal(T##__Sequence const*a,T##__Sequence const*b){size_t i;if(!a||!b||a->size!=b->size||a->size>a->capacity||b->size>b->capacity||(!a->data&&a->size)||(!b->data&&b->size))return false;for(i=0;i<a->size;++i)if(!T##__are_equal(a->data+i,b->data+i))return false;return true;}
DEFINE_RECORD_STORAGE(example_interfaces__srv__AddTwoInts_Request)
DEFINE_RECORD_STORAGE(example_interfaces__srv__AddTwoInts_Response)
DEFINE_RECORD_STORAGE(std_srvs__srv__Trigger_Request)
DEFINE_RECORD_STORAGE(std_srvs__srv__Trigger_Response)
DEFINE_RECORD_STORAGE(std_srvs__srv__SetBool_Request)
DEFINE_RECORD_STORAGE(std_srvs__srv__SetBool_Response)
#undef DEFINE_RECORD_STORAGE

#define DEFINE_SERVICE_SUPPORT(P,N,K,KEY) \
 static int const service_kind_##N=K; \
 static rosidl_message_type_support_t const request_support_##N={SERVICE_TOKEN,KEY "_Request",NULL,NULL,NULL,NULL}; \
 static rosidl_message_type_support_t const response_support_##N={SERVICE_TOKEN,KEY "_Response",NULL,NULL,NULL,NULL}; \
 static rosidl_service_type_support_t const support_##N={SERVICE_TOKEN,&service_kind_##N,NULL,&request_support_##N,&response_support_##N,NULL,NULL,NULL,NULL,NULL,NULL}; \
 rosidl_service_type_support_t const*rosidl_typesupport_c__get_service_type_support_handle__##P##__srv__##N(void){return &support_##N;} \
 rosidl_message_type_support_t const*rosidl_typesupport_c__get_message_type_support_handle__##P##__srv__##N##_Request(void){return &request_support_##N;} \
 rosidl_message_type_support_t const*rosidl_typesupport_c__get_message_type_support_handle__##P##__srv__##N##_Response(void){return &response_support_##N;}
DEFINE_SERVICE_SUPPORT(example_interfaces,AddTwoInts,1,"example_interfaces/srv/AddTwoInts")
DEFINE_SERVICE_SUPPORT(std_srvs,Trigger,2,"std_srvs/srv/Trigger")
DEFINE_SERVICE_SUPPORT(std_srvs,SetBool,3,"std_srvs/srv/SetBool")
#undef DEFINE_SERVICE_SUPPORT
static int service_kind(rosidl_service_type_support_t const*t){return t==&support_AddTwoInts?1:t==&support_Trigger?2:t==&support_SetBool?3:0;}
static char const*service_type_key(int k){return k==1?"example_interfaces/srv/AddTwoInts":k==2?"std_srvs/srv/Trigger":k==3?"std_srvs/srv/SetBool":NULL;}
rmw_qos_profile_t const rmw_qos_profile_services_default={RMW_QOS_POLICY_HISTORY_KEEP_LAST,10,RMW_QOS_POLICY_RELIABILITY_RELIABLE,RMW_QOS_POLICY_DURABILITY_VOLATILE,{0,0},{0,0},RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,{0,0},false};
static bool service_qos_valid(rmw_qos_profile_t const*q){rmw_qos_profile_t b=rmw_qos_profile_services_default;b.reliability=RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;return qos_equal(q,&rmw_qos_profile_services_default)||qos_equal(q,&b);}
static bool service_alias_conflict(uwvm_rclc_selection const*s,char const*alias,size_t stop){size_t i;struct uwvm_rclc_service_profile const*p=s->services;
 if(s->steady_clock_alias&&same(alias,s->steady_clock_alias))return true;
 for(i=0;i<s->topic_count;++i)if(same(alias,s->topics[i].alias))return true;
 for(i=0;i<stop;++i)if(same(alias,p->bindings[i].source_alias)||same(alias,p->bindings[i].auxiliary_alias))return true;return false;}
static bool service_selection_valid(uwvm_rclc_selection const*s){size_t i,j;struct uwvm_rclc_service_profile const*p=s->services;if(!p)return true;
 if(p->binding_count>256||(!p->bindings&&p->binding_count)||p->maximum_sleep_steps>1048576||p->maximum_sleep_ns>INT64_MAX)return false;
 if((p->maximum_sleep_ns==0)!=(p->maximum_sleep_steps==0))return false;
 for(i=0;i<p->binding_count;++i){uwvm_rclc_service_binding const*b=p->bindings+i;bool found=false;int kind=0;
  for(j=1;j<=3;++j)if(same(b->type,service_type_key((int)j)))kind=(int)j;
  if(!kind||!bounded_text(b->node,false)||!bounded_text(b->namespace_,true)||!bounded_text(b->service,false)||
   !bounded_text(b->source_alias,false)||!bounded_text(b->auxiliary_alias,false)||same(b->source_alias,b->auxiliary_alias)||
   (b->role!=UWVM_RCLC_SERVICE_SERVER&&b->role!=UWVM_RCLC_SERVICE_CLIENT)||!service_qos_valid(&b->qos)||
   b->maximum_wire_bytes<16||b->maximum_wire_bytes>UWVM_RCLC_MAX_WIRE||!b->maximum_requests||b->maximum_requests>UWVM_RCLC_MAX_SERVICE_REQUESTS)return false;
  if(service_alias_conflict(s,b->source_alias,i)||service_alias_conflict(s,b->auxiliary_alias,i))return false;
  for(j=0;j<s->node_count;++j)if(same(b->node,s->nodes[j].node)&&same(b->namespace_,s->nodes[j].namespace_))found=true;
  if(!found)return false;
  for(j=0;j<i;++j){uwvm_rclc_service_binding const*a=p->bindings+j;if(a->role==b->role&&same(a->node,b->node)&&same(a->namespace_,b->namespace_)&&same(a->service,b->service))return false;}
  if(b->role==UWVM_RCLC_SERVICE_CLIENT&&(!p->events||p->events->version!=2||!p->events->watch||!p->events->poll||!p->events->submit))return false;
 }return true;}

typedef struct {uint64_t id;unsigned status;uint32_t error;bool terminal;} service_pending;
struct uwvm_rclc_service_state {
 uwvm_rclc_service_binding const*binding;roswasmc_capability*source,*auxiliary;
 rcl_allocator_t allocator;rcl_client_options_t client_options;rcl_service_options_t server_options;
 int kind;bool client,poisoned,busy;uint32_t terminal_error,callback_error;
 unsigned char*wire,*send;size_t capacity,send_capacity,used;
 service_pending pending[UWVM_RCLC_MAX_SERVICE_REQUESTS];
 roswasmc_event_binding event_binding;uint64_t incoming_id;
};
static uwvm_rclc_service_binding const*service_binding(rcl_node_t const*n,char const*name,int kind,uwvm_rclc_service_role role,rmw_qos_profile_t const*q){
 size_t i;struct uwvm_rclc_service_profile const*p=n->context->uwvm_selection->services;if(!p)return NULL;
 for(i=0;i<p->binding_count;++i){uwvm_rclc_service_binding const*b=p->bindings+i;
  if(b->role==role&&same(b->node,n->uwvm_name)&&same(b->namespace_,n->uwvm_namespace)&&same(b->service,name)&&same(b->type,service_type_key(kind))&&qos_equal(q,&b->qos))return b;
 }return NULL;}
static void service_free(uwvm_rclc_service_state*s){rcl_allocator_t a;if(!s)return;a=s->allocator;if(s->wire)a.deallocate(s->wire,a.state);if(s->send)a.deallocate(s->send,a.state);a.deallocate(s,a.state);}
static uint32_t service_close(rcl_context_t*c,uwvm_rclc_service_state*s){uint32_t a=0,b=0;if(!s)return 0;a=close_owner(c,s->auxiliary);b=close_owner(c,s->source);s->poisoned=true;s->terminal_error=a?a:b;return a?a:b;}
static rcl_ret_t service_open(rcl_node_t const*n,char const*name,rosidl_service_type_support_t const*t,rmw_qos_profile_t const*q,rcl_allocator_t const*a,bool client,uwvm_rclc_service_state**out){
 rcl_context_t*c=n->context;uwvm_rclc_service_binding const*b;uwvm_rclc_service_state*s;int kind=service_kind(t);uint32_t raw;size_t cap;
 if(!kind||!q||!rcutils_allocator_is_valid(a))return failure(c,RCL_RET_INVALID_ARGUMENT,22,"unsupported service token/allocator");
 b=service_binding(n,name,kind,client?UWVM_RCLC_SERVICE_CLIENT:UWVM_RCLC_SERVICE_SERVER,q);
 if(!b)return failure(c,RCL_RET_UNSUPPORTED,95,"exact service node/name/type/QoS/aliases absent");
 if(c->uwvm_session.capacity-c->uwvm_session.count<2)return failure(c,RCL_RET_BAD_ALLOC,90,"reserve two ownership slots before service effects");
 if(!client&&(!c->uwvm_selection->backend->poll||!c->uwvm_selection->backend->advertise_service||!c->uwvm_selection->backend->request[ROSWASMC_SERVICE_RESPOND]))return failure(c,RCL_RET_UNSUPPORTED,95,"server imports absent before open");
 /* All bookkeeping, ownership and bounded I/O storage exist BEFORE first open. */
 s=a->zero_allocate(1,sizeof*s,a->state);if(!s)return failure(c,RCL_RET_BAD_ALLOC,0,"service state allocation failed");s->allocator=*a;s->binding=b;s->kind=kind;s->client=client;
 cap=b->maximum_wire_bytes+64;s->capacity=cap;s->send_capacity=b->maximum_wire_bytes+8;
 s->wire=a->allocate(cap,a->state);s->send=a->allocate(s->send_capacity,a->state);
 if(!s->wire||!s->send){service_free(s);return failure(c,RCL_RET_BAD_ALLOC,0,"service storage allocation failed before open");}
 s->client_options.qos=*q;s->client_options.allocator=*a;s->server_options.qos=*q;s->server_options.allocator=*a;
 raw=roswasmc_session_open(&c->uwvm_session,b->source_alias,strlen(b->source_alias),&s->source);
 if(!raw)raw=roswasmc_session_open(&c->uwvm_session,b->auxiliary_alias,strlen(b->auxiliary_alias),&s->auxiliary);
 if(!raw&&s->source->handle==s->auxiliary->handle)raw=22;
 if(!raw&&client)raw=roswasmc_event_binding_init(&s->event_binding,2,s->source->handle,0);
 if(!raw&&client)raw=roswasmc_event_watch(c->uwvm_selection->services->events,s->auxiliary,s->source,0);
 if(!raw&&!client)raw=roswasmc_service_advertise(s->source);
 if(raw){(void)service_close(c,s);service_free(s);return raw_result(c,raw,"service acquisition failed; every rollback result retained");}
 *out=s;return 0;
}
rcl_client_t rcl_get_zero_initialized_client(void){rcl_client_t c={0};return c;}
rcl_service_t rcl_get_zero_initialized_service(void){rcl_service_t s={0};return s;}
rcl_client_options_t rcl_client_get_default_options(void){rcl_client_options_t o;o.qos=rmw_qos_profile_services_default;o.allocator=rcutils_get_default_allocator();return o;}
rcl_service_options_t rcl_service_get_default_options(void){rcl_service_options_t o;o.qos=rmw_qos_profile_services_default;o.allocator=rcutils_get_default_allocator();return o;}
rcl_ret_t rcl_client_init(rcl_client_t*p,rcl_node_t const*n,rosidl_service_type_support_t const*t,char const*name,rcl_client_options_t const*o){
 object_entry*slot;uwvm_rclc_service_state*s=NULL;rcl_ret_t rc;if(!p||!node_live(n)||!bounded_text(name,false)||!o)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid client arguments");
 rc=mutable_context(n->context);if(rc)return rc;slot=object_slot(p);if(!slot)return object_admission_error(n->context,p,"client address already initialized/capacity exhausted");
 ++n->context->uwvm_dispatch;rc=service_open(n,name,t,&o->qos,&o->allocator,true,&s);--n->context->uwvm_dispatch;if(rc)return rc;p->uwvm_node=n;p->uwvm_generation=n->uwvm_generation;p->uwvm_state=s;++((rcl_node_t*)n)->uwvm_children;remember_object(slot,p,n->context,6);return 0;
}
rcl_ret_t rcl_service_init(rcl_service_t*p,rcl_node_t const*n,rosidl_service_type_support_t const*t,char const*name,rcl_service_options_t const*o){
 object_entry*slot;uwvm_rclc_service_state*s=NULL;rcl_ret_t rc;if(!p||!node_live(n)||!bounded_text(name,false)||!o)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid server arguments");
 rc=mutable_context(n->context);if(rc)return rc;slot=object_slot(p);if(!slot)return object_admission_error(n->context,p,"service address already initialized/capacity exhausted");
 ++n->context->uwvm_dispatch;rc=service_open(n,name,t,&o->qos,&o->allocator,false,&s);--n->context->uwvm_dispatch;if(rc)return rc;p->uwvm_node=n;p->uwvm_generation=n->uwvm_generation;p->uwvm_state=s;++((rcl_node_t*)n)->uwvm_children;remember_object(slot,p,n->context,7);return 0;
}
#define SERVICE_RCLC_INIT(N,T,O) \
 rcl_ret_t rclc_##N##_init(T*p,rcl_node_t const*n,rosidl_service_type_support_t const*t,char const*name,rmw_qos_profile_t const*q){O o;if(!node_live(n)||!q)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid service profile arguments");o.qos=*q;o.allocator=n->context->uwvm_allocator;return rcl_##N##_init(p,n,t,name,&o);} \
 rcl_ret_t rclc_##N##_init_default(T*p,rcl_node_t const*n,rosidl_service_type_support_t const*t,char const*name){return rclc_##N##_init(p,n,t,name,&rmw_qos_profile_services_default);} \
 rcl_ret_t rclc_##N##_init_best_effort(T*p,rcl_node_t const*n,rosidl_service_type_support_t const*t,char const*name){rmw_qos_profile_t q=rmw_qos_profile_services_default;q.reliability=RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;return rclc_##N##_init(p,n,t,name,&q);}
SERVICE_RCLC_INIT(client,rcl_client_t,rcl_client_options_t)
SERVICE_RCLC_INIT(service,rcl_service_t,rcl_service_options_t)
#undef SERVICE_RCLC_INIT
static void service_detach(void const*);
static bool client_storage(rcl_client_t const*p){return p&&known_object(p,6)&&p->uwvm_node&&context_storage(p->uwvm_node->context,p->uwvm_generation)&&p->uwvm_state;}
static bool server_storage(rcl_service_t const*p){return p&&known_object(p,7)&&p->uwvm_node&&context_storage(p->uwvm_node->context,p->uwvm_generation)&&p->uwvm_state;}
bool rcl_client_is_valid(rcl_client_t const*p){return client_storage(p)&&node_live(p->uwvm_node)&&roswasmc_is_open(p->uwvm_state->source)&&roswasmc_is_open(p->uwvm_state->auxiliary)&&!p->uwvm_state->poisoned;}
bool rcl_service_is_valid(rcl_service_t const*p){return server_storage(p)&&node_live(p->uwvm_node)&&roswasmc_is_open(p->uwvm_state->source)&&roswasmc_is_open(p->uwvm_state->auxiliary)&&!p->uwvm_state->poisoned;}
char const*rcl_client_get_service_name(rcl_client_t const*p){return client_storage(p)?p->uwvm_state->binding->service:NULL;}
char const*rcl_service_get_service_name(rcl_service_t const*p){return server_storage(p)?p->uwvm_state->binding->service:NULL;}
rcl_client_options_t const*rcl_client_get_options(rcl_client_t const*p){return client_storage(p)?&p->uwvm_state->client_options:NULL;}
rcl_service_options_t const*rcl_service_get_options(rcl_service_t const*p){return server_storage(p)?&p->uwvm_state->server_options:NULL;}
rcl_ret_t rcl_client_fini(rcl_client_t*p,rcl_node_t*n){rcl_context_t*c;uwvm_rclc_service_state*state;uint32_t raw;if(!client_storage(p)||p->uwvm_node!=n)return failure(NULL,RCL_RET_CLIENT_INVALID,9,"invalid client owner/node");c=n->context;if(c->uwvm_dispatch||p->uwvm_state->busy)return failure(c,RCL_RET_ERROR,17,"client group borrowed");++c->uwvm_dispatch;state=p->uwvm_state;raw=service_close(c,state);service_detach(p);p->uwvm_state=NULL;forget_object(p);if(n->uwvm_children)--n->uwvm_children;service_free(state);--c->uwvm_dispatch;return raw?cleanup_result(c,raw,"client independent cleanup failed"):0;}
rcl_ret_t rcl_service_fini(rcl_service_t*p,rcl_node_t*n){rcl_context_t*c;uwvm_rclc_service_state*state;uint32_t raw;if(!server_storage(p)||p->uwvm_node!=n)return failure(NULL,RCL_RET_SERVICE_INVALID,9,"invalid service owner/node");c=n->context;if(c->uwvm_dispatch||p->uwvm_state->busy)return failure(c,RCL_RET_ERROR,17,"service group borrowed");++c->uwvm_dispatch;state=p->uwvm_state;raw=service_close(c,state);service_detach(p);p->uwvm_state=NULL;forget_object(p);if(n->uwvm_children)--n->uwvm_children;service_free(state);--c->uwvm_dispatch;return raw?cleanup_result(c,raw,"service independent cleanup failed"):0;}
/* Context fini first closes every native owner independently. Then release all
 * additive storage even if a caller omitted individual service/client fini. */
static void service_context_release(rcl_context_t*c){size_t i;++c->uwvm_dispatch;for(i=0;i<UWVM_RCLC_MAX_OBJECTS;++i)if(objects[i].context==c){
 uwvm_rclc_service_state*state=NULL;rcl_node_t const*node=NULL;
 if(objects[i].kind==6){rcl_client_t*p=(rcl_client_t*)objects[i].address;state=p->uwvm_state;node=p->uwvm_node;p->uwvm_state=NULL;}
 else if(objects[i].kind==7){rcl_service_t*p=(rcl_service_t*)objects[i].address;state=p->uwvm_state;node=p->uwvm_node;p->uwvm_state=NULL;}
 if(state){memset(objects+i,0,sizeof objects[i]);if(node&&((rcl_node_t*)node)->uwvm_children)--((rcl_node_t*)node)->uwvm_children;service_free(state);}
}--c->uwvm_dispatch;}

static bool service_owned_string(rosidl_runtime_c__String const*s){return s&&s->data&&s->capacity&&s->size<s->capacity&&s->data[s->size]==0;}
/* Scalar transport uses unsigned bit copies, avoiding implementation-defined
 * signed narrowing and signed-overflow UB. Canonical wire has no native padding. */
static int64_t service_signed(uint64_t bits){int64_t v;memcpy(&v,&bits,8);return v;}
static uint64_t service_bits(int64_t v){uint64_t bits;memcpy(&bits,&v,8);return bits;}
static uint32_t service_encode(uwvm_rclc_service_state*s,bool response,void const*message,roswasmc_writer*w){
 if(!message)return 22;
 if(s->kind==1){if(response){example_interfaces__srv__AddTwoInts_Response const*m=message;return roswasmc_write_integer(w,service_bits(m->sum),8)?0:90;}
  else{example_interfaces__srv__AddTwoInts_Request const*m=message;return roswasmc_write_integer(w,service_bits(m->a),8)&&roswasmc_write_integer(w,service_bits(m->b),8)?0:90;}}
 if(!response){if(s->kind==2)return 0;return roswasmc_write_integer(w,((std_srvs__srv__SetBool_Request const*)message)->data?1u:0u,1)?0:90;}
 {rosidl_runtime_c__String const*text;bool success;roswasmc_string view;
  if(s->kind==2){std_srvs__srv__Trigger_Response const*m=message;text=&m->message;success=m->success;}
  else{std_srvs__srv__SetBool_Response const*m=message;text=&m->message;success=m->success;}
  if(!service_owned_string(text))return 22;view.data=text->data;view.size=text->size;
  if(!roswasmc_utf8(view,1))return 22;
  return roswasmc_write_integer(w,success?1u:0u,1)&&roswasmc_write_string(w,view)?0:90;
 }
}
/* NULL output still performs the COMPLETE decoder, used by abandoned late
 * responses. Never dereference expired caller response storage for a tombstone. */
static uint32_t service_decode(uwvm_rclc_service_state*s,bool response,unsigned char const*wire,size_t size,void*out){
 roswasmc_reader r=roswasmc_reader_init(wire,size);uint64_t a,b;
 if(size>s->binding->maximum_wire_bytes)return 90;
 if(s->kind==1){if(response){if(!roswasmc_read_integer(&r,&a,8)||!roswasmc_reader_done(&r))return 22;if(out)((example_interfaces__srv__AddTwoInts_Response*)out)->sum=service_signed(a);return 0;}
  if(!roswasmc_read_integer(&r,&a,8)||!roswasmc_read_integer(&r,&b,8)||!roswasmc_reader_done(&r))return 22;
  {int64_t x=service_signed(a),y=service_signed(b);/* Original official callback adds signed a+b. Refuse UB before it. */
   if((y>0&&x>INT64_MAX-y)||(y<0&&x<INT64_MIN-y))return 22;
   if(out){example_interfaces__srv__AddTwoInts_Request*m=out;m->a=x;m->b=y;}return 0;}}
 if(!response){if(s->kind==2){if(size)return 22;if(out)((std_srvs__srv__Trigger_Request*)out)->structure_needs_at_least_one_member=0;return 0;}
  if(!roswasmc_read_integer(&r,&a,1)||a>1||!roswasmc_reader_done(&r))return 22;if(out)((std_srvs__srv__SetBool_Request*)out)->data=a!=0;return 0;}
 {roswasmc_service_response value;uint32_t raw=roswasmc_decode_service_response(wire,size,&value);rosidl_runtime_c__String*text;if(raw||!out)return raw;
  text=s->kind==2?&((std_srvs__srv__Trigger_Response*)out)->message:&((std_srvs__srv__SetBool_Response*)out)->message;
  if(!service_owned_string(text))return 22;if(!rosidl_runtime_c__String__assignn(text,value.message.data,value.message.size))return 12;
  if(s->kind==2)((std_srvs__srv__Trigger_Response*)out)->success=value.success!=0;else((std_srvs__srv__SetBool_Response*)out)->success=value.success!=0;return 0;}
}
static service_pending*service_pending_find(uwvm_rclc_service_state*s,uint64_t id){size_t i;for(i=0;i<s->used;++i)if(s->pending[i].id==id)return s->pending+i;return NULL;}
static void service_fail(uwvm_rclc_service_state*s,uint32_t code){size_t i;s->poisoned=true;s->terminal_error=code;for(i=0;i<s->used;++i)if(s->pending[i].status==1){s->pending[i].status=4;s->pending[i].error=code;s->pending[i].terminal=true;}}
rcl_ret_t rcl_send_request(rcl_client_t const*p,void const*message,int64_t*out){
 uwvm_rclc_service_state*s;rcl_context_t*c;roswasmc_writer w;uint32_t raw;uint64_t id=0;service_pending*pending;
 if(!client_storage(p))return failure(NULL,RCL_RET_CLIENT_INVALID,9,"invalid client owner");c=p->uwvm_node->context;s=p->uwvm_state;
 if(!message||!out)return failure(c,RCL_RET_INVALID_ARGUMENT,22,"null request/output");
 if(c->uwvm_sleep_error)return raw_result(c,c->uwvm_sleep_error,"prior physical sleep failed; request refused before submit");
 if(s->poisoned)return raw_result(c,s->terminal_error,"client terminal after previous admission/delivery failure");
 if(!rcl_client_is_valid(p))return failure(c,RCL_RET_CLIENT_INVALID,9,"client source/context is not live");
 if(s->busy)return failure(c,RCL_RET_ERROR,17,"same client is already borrowed");
 if(s->used>=s->binding->maximum_requests)return failure(c,RCL_RET_ERROR,90,"finite lifetime request slots exhausted before submit");
 /* Reserved slot + all codec storage precede the one actual submit. */
 pending=s->pending+s->used;w=roswasmc_writer_init(s->send,s->binding->maximum_wire_bytes);
 raw=service_encode(s,false,message,&w);if(raw)return raw_result(c,raw,"complete request encoding rejected before submit");
 s->busy=true;++c->uwvm_dispatch;
 raw=roswasmc_service_submit(c->uwvm_selection->services->events,s->source,s->auxiliary,w.data,w.size,&id);
 --c->uwvm_dispatch;s->busy=false;
 if(raw){if(raw!=11)service_fail(s,raw);return raw_result(c,raw,"async admission failed; no replay");}
 if(!id||service_pending_find(s,id)){service_fail(s,22);return failure(c,RCL_RET_ERROR,22,"actual Gateway returned zero/reused ticket after admission");}
 pending->id=id;pending->status=1;pending->error=0;pending->terminal=false;++s->used;memcpy(out,&id,8);return 0;
}
rcl_ret_t uwvm_rclc_remove_pending_request(rcl_client_t*p,uint64_t id){service_pending*q;rcl_context_t*c;if(!client_storage(p)||!id)return failure(NULL,RCL_RET_CLIENT_INVALID,9,"invalid local request removal");c=p->uwvm_node->context;if(c->uwvm_dispatch||p->uwvm_state->busy)return failure(c,RCL_RET_ERROR,17,"request removal during group dispatch refused");q=service_pending_find(p->uwvm_state,id);if(!q||q->status!=1)return failure(c,RCL_RET_NOT_FOUND,9,"request is not pending");q->status=2;q->error=125;return 0;}
rcl_ret_t uwvm_rclc_request_status(rcl_client_t const*p,uint64_t id,unsigned*state,uint32_t*error){service_pending*q;if(!client_storage(p)||!id||!state||!error)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid request status query");q=service_pending_find(p->uwvm_state,id);if(!q)return RCL_RET_NOT_FOUND;*state=q->status;*error=q->error;return 0;}
/* Native request headers and real matched-type availability need separate
 * protocol extensions. Refusal neither polls a queue nor modifies caller out. */
rcl_ret_t rcl_take_response(rcl_client_t const*p,rmw_request_id_t*h,void*out){(void)h;(void)out;return failure(client_storage(p)?p->uwvm_node->context:NULL,RCL_RET_UNSUPPORTED,95,"native RMW response header is unavailable");}
rcl_ret_t rcl_take_response_with_info(rcl_client_t const*p,rmw_service_info_t*h,void*out){(void)h;(void)out;return failure(client_storage(p)?p->uwvm_node->context:NULL,RCL_RET_UNSUPPORTED,95,"native RMW response metadata is unavailable");}
rcl_ret_t rcl_take_request(rcl_service_t const*p,rmw_request_id_t*h,void*out){(void)h;(void)out;return failure(server_storage(p)?p->uwvm_node->context:NULL,RCL_RET_UNSUPPORTED,95,"native RMW request header is unavailable");}
rcl_ret_t rcl_take_request_with_info(rcl_service_t const*p,rmw_service_info_t*h,void*out){(void)h;(void)out;return failure(server_storage(p)?p->uwvm_node->context:NULL,RCL_RET_UNSUPPORTED,95,"native RMW request metadata is unavailable");}
rcl_ret_t rcl_send_response(rcl_service_t const*p,rmw_request_id_t*h,void*out){(void)h;(void)out;return failure(server_storage(p)?p->uwvm_node->context:NULL,RCL_RET_UNSUPPORTED,95,"RMW header is not a Gateway-owned response ticket");}
rcl_ret_t rcl_service_server_is_available(rcl_node_t const*n,rcl_client_t const*p,bool*out){(void)out;return failure(node_storage(n)&&client_storage(p)?n->context:NULL,RCL_RET_UNSUPPORTED,95,"matched native service availability extension is not selected");}
static bool service_registered(void const*p){size_t i,j;for(i=0;i<UWVM_RCLC_MAX_OBJECTS;++i)if(objects[i].kind==5){rclc_executor_t const*e=objects[i].address;for(j=0;j<e->index;++j)if(e->handles[j].uwvm_client==p||e->handles[j].uwvm_service==p)return true;}return false;}
static void service_detach(void const*p){size_t i,j;for(i=0;i<UWVM_RCLC_MAX_OBJECTS;++i)if(objects[i].kind==5){rclc_executor_t*e=(rclc_executor_t*)objects[i].address;for(j=0;j<e->index;++j){rclc_executor_handle_t*h=e->handles+j;if(h->uwvm_client==p)h->uwvm_client=NULL;if(h->uwvm_service==p)h->uwvm_service=NULL;}}}
static rcl_ret_t service_add(rclc_executor_t*e,rcl_service_t*server,rcl_client_t*client,void*request,void*response,rclc_service_callback_t cb,rclc_service_callback_with_context_t ctxcb,rclc_client_callback_t clientcb,void*user){
 rcl_context_t*c;rclc_executor_handle_t h;rcl_ret_t rc;void const*p=server?(void const*)server:(void const*)client;
 if(!executor_live(e)||!response||(!server&&!client))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid service executor registration");c=e->context;rc=mutable_context(c);if(rc)return rc;
 if(server?(!rcl_service_is_valid(server)||!request||(!cb&&!ctxcb)||server->uwvm_node->context!=c):(!rcl_client_is_valid(client)||!clientcb||client->uwvm_node->context!=c))return failure(c,RCL_RET_INVALID_ARGUMENT,22,"invalid/cross-context service registration");
 if(e->index>=e->max_handles)return failure(c,RCL_RET_ERROR,90,"executor capacity exhausted before service registration");
 if(service_registered(p))return failure(c,RCL_RET_ERROR,17,"source already registered in an executor");
 memset(&h,0,sizeof h);h.kind=server?3:4;h.uwvm_service=server;h.uwvm_client=client;h.uwvm_request=request;h.uwvm_response=response;h.uwvm_service_callback=cb;h.uwvm_service_context_callback=ctxcb;h.uwvm_client_callback=clientcb;h.user=user;
 e->handles[e->index]=h;++e->index;return 0;
}
rcl_ret_t rclc_executor_add_service(rclc_executor_t*e,rcl_service_t*s,void*q,void*r,rclc_service_callback_t cb){return service_add(e,s,NULL,q,r,cb,NULL,NULL,NULL);}
rcl_ret_t rclc_executor_add_service_with_context(rclc_executor_t*e,rcl_service_t*s,void*q,void*r,rclc_service_callback_with_context_t cb,void*user){return service_add(e,s,NULL,q,r,NULL,cb,NULL,user);}
rcl_ret_t rclc_executor_add_client(rclc_executor_t*e,rcl_client_t*c,void*r,rclc_client_callback_t cb){return service_add(e,NULL,c,NULL,r,NULL,NULL,cb,NULL);}
rcl_ret_t rclc_executor_add_service_with_request_id(rclc_executor_t*e,rcl_service_t*s,void*q,void*r,rclc_service_callback_with_request_id_t cb){(void)s;(void)q;(void)r;(void)cb;return failure(executor_live(e)?e->context:NULL,RCL_RET_UNSUPPORTED,95,"native request-ID callbacks require unavailable RMW metadata");}
rcl_ret_t rclc_executor_add_client_with_request_id(rclc_executor_t*e,rcl_client_t*c,void*r,rclc_client_callback_with_request_id_t cb){(void)c;(void)r;(void)cb;return failure(executor_live(e)?e->context:NULL,RCL_RET_UNSUPPORTED,95,"native request-ID callbacks require unavailable RMW metadata");}
static rcl_ret_t service_handle_validate(rclc_executor_t*e,rclc_executor_handle_t*h){
 uwvm_rclc_service_state*s=NULL;
 if(h->kind==3){rcl_service_t*p=h->uwvm_service;if(!server_storage(p)||p->uwvm_node->context!=e->context||!node_live(p->uwvm_node))return failure(e->context,RCL_RET_SERVICE_INVALID,9,"stale/detached service executor source");s=p->uwvm_state;}
 else if(h->kind==4){rcl_client_t*p=h->uwvm_client;if(!client_storage(p)||p->uwvm_node->context!=e->context||!node_live(p->uwvm_node))return failure(e->context,RCL_RET_CLIENT_INVALID,9,"stale/detached client executor source");s=p->uwvm_state;}
 else return failure(e->context,RCL_RET_INVALID_ARGUMENT,22,"unknown additive executor handle kind");
 if(s->poisoned)return raw_result(e->context,s->terminal_error,"service stream is terminal after previous failure");
 if(!roswasmc_is_open(s->source)||!roswasmc_is_open(s->auxiliary))return failure(e->context,RCL_RET_ERROR,9,"source/auxiliary owner retired");
 if(s->busy)return failure(e->context,RCL_RET_ERROR,17,"service scratch already borrowed");
 if(h->kind==3&&s->incoming_id){service_fail(s,22);return failure(e->context,RCL_RET_ERROR,22,"previous consumed request was not dispatched; close required");}
 return 0;
}
static rcl_ret_t service_handle_gather(rclc_executor_t*e,rclc_executor_handle_t*h){
 rcl_context_t*c=e->context;uwvm_rclc_service_state*s=h->kind==3?((rcl_service_t*)h->uwvm_service)->uwvm_state:((rcl_client_t*)h->uwvm_client)->uwvm_state;
 roswasmc_result result;uint32_t raw=0;service_pending*p=NULL;uint64_t id;
 if(h->kind==3&&s->used>=s->binding->maximum_requests)return failure(c,RCL_RET_ERROR,90,"server lifetime request budget exhausted before poll");
 s->busy=true;
 if(h->kind==3){
  result=roswasmc_poll(s->source,s->wire,s->capacity);
  if(result.error==11){s->busy=false;return 0;}
  raw=result.error;
  if(!raw&&(!result.has_length||result.length<8||result.length>s->binding->maximum_wire_bytes+8))raw=22;
  if(!raw){id=uwvm_ros_load_u64(s->wire);if(!id||service_pending_find(s,id))raw=22;
   else raw=service_decode(s,false,s->wire+8,(size_t)result.length-8,h->uwvm_request);
   if(!raw){p=s->pending+s->used;p->id=id;p->status=1;p->error=0;p->terminal=false;++s->used;s->incoming_id=id;h->ready=true;}}
 }
 else{
  roswasmc_event_header event;
  result=roswasmc_event_poll(c->uwvm_selection->services->events,s->auxiliary,s->wire,s->capacity);
  if(result.error==11){s->busy=false;return 0;}
  raw=result.error;if(!raw)raw=roswasmc_event_binding_decode(&s->event_binding,s->wire,result.length,&event);
  if(!raw){
   if(event.kind==ROSWASMC_SERVICE_RESPONSE){
    p=service_pending_find(s,event.correlation);
    if(!p||p->terminal||(p->status!=1&&p->status!=2))raw=22;
    else{
     /* Mandatory complete decoding also covers locally abandoned requests. */
     raw=service_decode(s,true,s->wire+64,(size_t)event.payload_bytes,NULL);
     if(!raw&&p->status==1)raw=service_decode(s,true,s->wire+64,(size_t)event.payload_bytes,h->uwvm_response);
     if(!raw){p->terminal=true;if(p->status==1){p->status=3;p->error=0;h->ready=true;}}
    }
   }
   else if(event.kind==ROSWASMC_RESOURCE_ERROR||event.kind==ROSWASMC_RESOURCE_CLOSED){
    uint32_t terminal=event.error?event.error:9;
    if(event.correlation){p=service_pending_find(s,event.correlation);if(!p||p->terminal||(p->status!=1&&p->status!=2))raw=22;
     else{p->terminal=true;if(p->status==1){p->status=4;p->error=terminal;}}}
    if(!raw){if(!event.correlation||(event.flags&1u))service_fail(s,terminal);s->busy=false;return raw_result(c,terminal,"actual service error/retirement event; no native replay");}
   }
   else raw=22;
  }
 }
 s->busy=false;
 if(raw){service_fail(s,raw);return raw_result(c,raw,"complete service record rejected/failed; stream not replayed");}
 return 0;
}
static rcl_ret_t service_handle_dispatch(rclc_executor_t*e,rclc_executor_handle_t*h,bool*any){
 uwvm_rclc_service_state*s;uint32_t raw;service_pending*p;roswasmc_writer w;roswasmc_result result;
 if(!h->ready)return 0;h->ready=false;*any=true;
 if(h->kind==4){h->uwvm_client_callback(h->uwvm_response);return 0;}
 s=((rcl_service_t*)h->uwvm_service)->uwvm_state;s->busy=true;s->callback_error=0;
 if(h->uwvm_service_context_callback)h->uwvm_service_context_callback(h->uwvm_request,h->uwvm_response,h->user);else h->uwvm_service_callback(h->uwvm_request,h->uwvm_response);
 w=roswasmc_writer_init(s->send,s->send_capacity);
 raw=s->callback_error;
 if(!raw)raw=roswasmc_write_integer(&w,s->incoming_id,8)?0:90;
 if(!raw)raw=service_encode(s,true,h->uwvm_response,&w);
 if(!raw){result=roswasmc_service_respond(s->auxiliary,w.data,w.size,NULL,0);raw=result.error;}
 p=service_pending_find(s,s->incoming_id);if(p){p->terminal=true;p->status=raw?4:3;p->error=raw;}
 s->incoming_id=0;s->busy=false;
 if(raw){service_fail(s,raw);return raw_result(e->context,raw,"service response attempted once; no replay or hidden retry");}
 return 0;
}
rcl_ret_t uwvm_rclc_service_callback_error(rcl_service_t*p,uint32_t error){
 if(!server_storage(p)||!error)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid callback failure report");
 if(!p->uwvm_state->busy||!p->uwvm_node->context->uwvm_dispatch)return failure(p->uwvm_node->context,RCL_RET_ERROR,17,"callback error may be reported only inside its active service callback");
 if(!p->uwvm_state->callback_error)p->uwvm_state->callback_error=error;return 0;
}
void rclc_sleep_ms(unsigned int ms){
 size_t i,steps=0;rcl_context_t*c=NULL;struct uwvm_rclc_service_profile const*p;uint64_t first,now,deadline,wait=(uint64_t)ms*1000000u;uint32_t raw=0;rcl_ret_t rc;
 if(!ms)return;
 /* The official void API has no Context argument. Ambiguity must fail visibly,
  * not select an arbitrary grant. No real time is synthesized from step count. */
 for(i=0;i<UWVM_RCLC_MAX_CONTEXTS;++i)if(registered[i]&&rcl_context_is_valid(registered[i])){if(c){
  size_t j;for(j=0;j<UWVM_RCLC_MAX_CONTEXTS;++j)if(registered[j]&&rcl_context_is_valid(registered[j]))registered[j]->uwvm_sleep_error=95;
  (void)failure(c,RCL_RET_UNSUPPORTED,95,"void sleep has multiple live Contexts");return;}c=registered[i];}
 if(!c){(void)failure(NULL,RCL_RET_UNSUPPORTED,95,"void sleep has no uniquely selected Context");return;}
 p=c->uwvm_selection->services;
 if(c->uwvm_dispatch||!p||!p->maximum_sleep_steps||wait>p->maximum_sleep_ns){c->uwvm_sleep_error=c->uwvm_dispatch?17:95;(void)raw_result(c,c->uwvm_sleep_error,"physical sleep not explicitly admitted");return;}
 rc=clock_now(c,&first);if(rc){c->uwvm_sleep_error=c->uwvm_diagnostics.raw_primary?c->uwvm_diagnostics.raw_primary:95;return;}
 if(wait>(uint64_t)INT64_MAX-first){c->uwvm_sleep_error=90;(void)raw_result(c,90,"sleep deadline overflow");return;}
 deadline=first+wait;now=first;++c->uwvm_dispatch;
 while(now<deadline){uint64_t next;if(steps++>=p->maximum_sleep_steps){raw=110;break;}
  raw=c->uwvm_selection->step(c->uwvm_selection->step_context,deadline-now);
  if(raw==125){c->uwvm_shutdown=true;roswasmc_local_context_shutdown(&c->uwvm_lifecycle);break;}
  if(raw)break;rc=clock_now(c,&next);if(rc){raw=c->uwvm_diagnostics.raw_primary?c->uwvm_diagnostics.raw_primary:95;break;}
  if(next<now){raw=22;break;}now=next;
 }
 --c->uwvm_dispatch;
 if(raw){c->uwvm_sleep_error=raw;(void)raw_result(c,raw,"genuine elapsed sleep failed; subsequent request admission blocked");}
}
#undef SERVICE_TOKEN
