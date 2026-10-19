/* Retained attribution for adapted allocator/error/String lifecycle:
 * Copyright 2015 Open Source Robotics Foundation, Inc.
 *   ros2/rcutils src/allocator.c
 * Copyright 2014 Open Source Robotics Foundation, Inc.
 *   ros2/rcutils include/rcutils/error_handling.h
 *   rcutils commit 26413034bab69688475f230ea7723e534ec3f8cd.
 * Copyright 2015 Open Source Robotics Foundation, Inc.
 *   ros2/rosidl rosidl_runtime_c/src/string_functions.c
 *   rosidl commit 85fa592b698b0f665e3120f48fac0d35e2f7d8a4.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Changed source: the lifecycle is adapted to this finite Guest-only profile,
 * using the selected allocator callbacks, reserved error/cleanup storage and
 * complete original String fields. Native ROS libraries are not linked here.
 */
/* Source adapter implementation, compiled ONCE per executable.
 * See compat.h and README: this is not linked with native rcl/rmw libraries. */
#include <roswasm_rclc/compat.h>
#include <limits.h>
#if defined(UWVM_RCLC_COMPAT_SELECTION_HEADER)
#include UWVM_RCLC_COMPAT_SELECTION_HEADER
#else
uwvm_rclc_selection const*uwvm_rclc_selected_configuration(void){
    return NULL;
}
#endif
/* One real per-thread error state shared across the original business TU and
 * this implementation. No diagnostic allocation is required. */
static _Thread_local rcutils_error_state_t error_state;
static _Thread_local bool error_set;
static rcl_context_t*registered[UWVM_RCLC_MAX_CONTEXTS];
static uint64_t generation;
/* Address admission avoids reading indeterminate native-style fresh storage:
 * official rclc_init functions overwrite fresh malloc objects. Finite static
 * metadata is reserved BEFORE native effects, and never owns their lifetime. */
#define UWVM_RCLC_MAX_OBJECTS 4096u
typedef struct {
    void const*address;
    rcl_context_t*context;
    int kind;
}
object_entry;
static object_entry objects[UWVM_RCLC_MAX_OBJECTS];
/* Additive service implementation below reuses this registry and Context. */
static bool service_selection_valid(uwvm_rclc_selection const*);
static void service_context_release(rcl_context_t*);
static rcl_ret_t service_handle_validate(rclc_executor_t*,rclc_executor_handle_t*);
static rcl_ret_t service_handle_gather(rclc_executor_t*,rclc_executor_handle_t*);
static rcl_ret_t service_handle_dispatch(rclc_executor_t*,rclc_executor_handle_t*,bool*);

static bool known_object(void const*p,int kind){
    size_t i;
    for(i=0;
    i<UWVM_RCLC_MAX_OBJECTS;
    ++i)if(objects[i].address==p&&objects[i].kind==kind)return true;
    return false;
}
static object_entry*object_slot(void const*p){
    size_t i;
    object_entry*free_slot=NULL;
    for(i=0;
    i<UWVM_RCLC_MAX_OBJECTS;
    ++i){
        if(objects[i].address==p)return NULL;
        if(!objects[i].address&&!free_slot)free_slot=objects+i;
    }
    return free_slot;
}
static void forget_object(void const*p){
    size_t i;
    for(i=0;
    i<UWVM_RCLC_MAX_OBJECTS;
    ++i)if(objects[i].address==p){
        memset(objects+i,0,sizeof objects[i]);
        return;
    }
}
static void remember_object(object_entry*slot,void const*p,rcl_context_t*c,int kind){
    slot->address=p;
    slot->context=c;
    slot->kind=kind;
}
static void text_copy(char*out,size_t capacity,char const*in){
    size_t i=0;
    if(!capacity)return;
    if(in)while(i+1<capacity&&in[i]){
        out[i]=in[i];
        ++i;
    }
    out[i]=0;
}
void rcutils_set_error_state(char const*s,char const*f,size_t line){
    text_copy(error_state.message,sizeof error_state.message,s);
    text_copy(error_state.file,sizeof error_state.file,f);
    error_state.line_number=line;
    error_set=true;
}
bool rcutils_error_is_set(void){
    return error_set;
}
rcutils_error_state_t const*rcutils_get_error_state(void){
    return error_set?&error_state:NULL;
}
static void append_text(char*out,size_t capacity,size_t*used,char const*in){
    size_t i=0;
    while(in&&in[i]&&*used+1<capacity)out[(*used)++]=in[i++];
    out[*used]=0;
}
rcutils_error_string_t rcutils_get_error_string(void){
    rcutils_error_string_t s={
        {
            0
        }
    }
    ;
    size_t used=0,n=0;
    uint64_t line=error_state.line_number;
    char digits[21]; /* Complete decimal uint64 plus one spare byte, even on wasm32. */
    if(!error_set){
        text_copy(s.str,sizeof s.str,"error not set");
        return s;
    }
    append_text(s.str,sizeof s.str,&used,error_state.message);
    append_text(s.str,sizeof s.str,&used,", at ");
    append_text(s.str,sizeof s.str,&used,error_state.file);
    append_text(s.str,sizeof s.str,&used,":");
    do{
        digits[n++]=(char)('0'+line%10);
        line/=10;
    }
    while(line);
    while(n&&used+1<sizeof s.str)s.str[used++]=digits[--n];
    s.str[used]=0;
    return s;
}
void rcutils_reset_error(void){
    memset(&error_state,0,sizeof error_state);
    error_set=false;
}
static rcl_ret_t failure(rcl_context_t*c,rcl_ret_t result,uint32_t raw,char const*message){
    if(c){
        c->uwvm_diagnostics.primary=result;
        c->uwvm_diagnostics.raw_primary=raw;
    }
    RCL_SET_ERROR_MSG(message);
    return result;
}
static rcl_ret_t object_admission_error(rcl_context_t*c,void const*object,char const*message){
    size_t i;
    for(i=0;i<UWVM_RCLC_MAX_OBJECTS;++i)if(objects[i].address==object)return failure(c,RCL_RET_ALREADY_INIT,17,message);
    return failure(c,RCL_RET_BAD_ALLOC,90,message);
}
static rcl_ret_t raw_result(rcl_context_t*c,uint32_t code,char const*message){
    if(!code)return RCL_RET_OK;
    return failure(c,code==UWVM_ROS_UNSUPPORTED?RCL_RET_UNSUPPORTED:code==UWVM_ROS_INVALID?RCL_RET_INVALID_ARGUMENT:RCL_RET_ERROR,code,message);
}
/* A cleanup failure is separate from an already retained application/setup
 * primary. close_owner has already saved the exact per-owner raw result. */
static rcl_ret_t cleanup_result(rcl_context_t*c,uint32_t raw,char const*message){
    if(!raw)return 0;
    if(!c->uwvm_diagnostics.primary)return failure(c,RCL_RET_ERROR,raw,message);
    RCL_SET_ERROR_MSG(message);
    return RCL_RET_ERROR;
}

static void*def_alloc(size_t n,void*s){
    (void)s;
    return malloc(n);
}
static void def_free(void*p,void*s){
    (void)s;
    free(p);
}
static void*def_realloc(void*p,size_t n,void*s){
    (void)s;
    return realloc(p,n);
}
static void*def_zero(size_t n,size_t w,void*s){
    (void)s;
    return calloc(n,w);
}
rcutils_allocator_t rcutils_get_default_allocator(void){
    rcutils_allocator_t a={
        def_alloc,def_free,def_realloc,def_zero,NULL
    }
    ;
    return a;
}
rcutils_allocator_t rcutils_get_zero_initialized_allocator(void){
    rcutils_allocator_t a={
        0
    }
    ;
    return a;
}
bool rcutils_allocator_is_valid(rcutils_allocator_t const*a){
    return a&&a->allocate&&a->deallocate&&a->reallocate&&a->zero_allocate;
}
void*rcutils_reallocf(void*p,size_t n,rcutils_allocator_t*a){
    void*q;
    if(!rcutils_allocator_is_valid(a))return NULL;
    q=a->reallocate(p,n,a->state);
    if(!q)a->deallocate(p,a->state);
    return q;
}
/* These follow the official scalar String lifecycle, including realloc failure
 * preserving old storage. Do not pass an uninitialized or shallow-copied owner.
 * Invalid field invariants at fini are a caller bug, as in native rosidl. */
bool rosidl_runtime_c__String__init(rosidl_runtime_c__String*s){
    rcutils_allocator_t a;
    if(!s)return false;
    a=rcutils_get_default_allocator();
    s->data=a.allocate(1,a.state);
    if(!s->data)return false;
    s->data[0]=0;
    s->size=0;
    s->capacity=1;
    return true;
}
void rosidl_runtime_c__String__fini(rosidl_runtime_c__String*s){
    rcutils_allocator_t a;
    if(!s)return;
    if((s->data&&!s->capacity)||(!s->data&&(s->size||s->capacity)))abort();
    a=rcutils_get_default_allocator();
    a.deallocate(s->data,a.state);
    s->data=NULL;
    s->size=s->capacity=0;
}
bool rosidl_runtime_c__String__assignn(rosidl_runtime_c__String*s,char const*v,size_t n){
    rcutils_allocator_t a;
    char*d;
    if(!s||!v||n==SIZE_MAX)return false;
    a=rcutils_get_default_allocator();
    d=a.reallocate(s->data,n+1,a.state);
    if(!d)return false;
    memcpy(d,v,n);
    d[n]=0;
    s->data=d;
    s->size=n;
    s->capacity=n+1;
    return true;
}
bool rosidl_runtime_c__String__assign(rosidl_runtime_c__String*s,char const*v){
    return v?rosidl_runtime_c__String__assignn(s,v,strlen(v)):false;
}
bool rosidl_runtime_c__String__copy(rosidl_runtime_c__String const*i,rosidl_runtime_c__String*o){
    return i?rosidl_runtime_c__String__assignn(o,i->data,i->size):false;
}
bool rosidl_runtime_c__String__are_equal(rosidl_runtime_c__String const*a,rosidl_runtime_c__String const*b){
    return a&&b&&a->size==b->size&&(!a->size||!memcmp(a->data,b->data,a->size));
}
bool std_msgs__msg__String__init(std_msgs__msg__String*m){
    return m&&rosidl_runtime_c__String__init(&m->data);
}
void std_msgs__msg__String__fini(std_msgs__msg__String*m){
    if(m)rosidl_runtime_c__String__fini(&m->data);
}
std_msgs__msg__String*std_msgs__msg__String__create(void){
    std_msgs__msg__String*m=calloc(1,sizeof*m);
    if(m&&!std_msgs__msg__String__init(m)){
        free(m);
        return NULL;
    }
    return m;
}
void std_msgs__msg__String__destroy(std_msgs__msg__String*m){
    if(m)std_msgs__msg__String__fini(m);
    free(m);
}
bool std_msgs__msg__String__copy(std_msgs__msg__String const*a,std_msgs__msg__String*b){
    return a&&b&&rosidl_runtime_c__String__copy(&a->data,&b->data);
}
bool std_msgs__msg__String__are_equal(std_msgs__msg__String const*a,std_msgs__msg__String const*b){
    return a&&b&&rosidl_runtime_c__String__are_equal(&a->data,&b->data);
}
static char const string_key[]="std_msgs/msg/String";
static rosidl_message_type_support_t const string_support={
    "uwvm_guest_source_token_not_native_typesupport",string_key,NULL,NULL,NULL,NULL
}
;
rosidl_message_type_support_t const*rosidl_typesupport_c__get_message_type_support_handle__std_msgs__msg__String(void){
    return &string_support;
}
rmw_qos_profile_t const rmw_qos_profile_default={
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,10,RMW_QOS_POLICY_RELIABILITY_RELIABLE,RMW_QOS_POLICY_DURABILITY_VOLATILE,{
        0,0
    }
    ,{
        0,0
    }
    ,RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,{
        0,0
    }
    ,false
}
;
rmw_qos_profile_t const rmw_qos_profile_sensor_data={
    RMW_QOS_POLICY_HISTORY_KEEP_LAST,5,RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,RMW_QOS_POLICY_DURABILITY_VOLATILE,{
        0,0
    }
    ,{
        0,0
    }
    ,RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,{
        0,0
    }
    ,false
}
;
static bool qos_equal(rmw_qos_profile_t const*a,rmw_qos_profile_t const*b){
    return a&&b&&a->history==b->history&&a->depth==b->depth&&a->reliability==b->reliability&&a->durability==b->durability&&a->deadline.sec==b->deadline.sec&&a->deadline.nsec==b->deadline.nsec&&a->lifespan.sec==b->lifespan.sec&&a->lifespan.nsec==b->lifespan.nsec&&a->liveliness==b->liveliness&&a->liveliness_lease_duration.sec==b->liveliness_lease_duration.sec&&a->liveliness_lease_duration.nsec==b->liveliness_lease_duration.nsec&&a->avoid_ros_namespace_conventions==b->avoid_ros_namespace_conventions;
}
static bool bounded_text(char const*s,bool empty){
    size_t n;
    if(!s)return false;
    for(n=0;
    n<=255;
    ++n)if(!s[n])return empty||n!=0;
    return false;
}
static bool same(char const*a,char const*b){
    return a&&b&&!strcmp(a,b);
}
static bool registered_context(rcl_context_t const*c){
    size_t i;
    for(i=0;
    i<UWVM_RCLC_MAX_CONTEXTS;
    ++i)if(registered[i]==c)return true;
    return false;
}
static bool context_storage(rcl_context_t const*c,uint64_t g){
    return c&&registered_context(c)&&c->uwvm_initialized&&c->uwvm_generation==g;
}
bool rcl_context_is_valid(rcl_context_t const*c){
    return c&&registered_context(c)&&c->uwvm_initialized&&!c->uwvm_shutdown&&c->uwvm_lifecycle.active;
}
static rcl_ret_t mutable_context(rcl_context_t*c){
    if(!rcl_context_is_valid(c))return failure(c,RCL_RET_NOT_INIT,UWVM_ROS_REVOKED,"context is not active");
    if(c->uwvm_dispatch)return failure(c,RCL_RET_ERROR,UWVM_ROS_CONFLICT,"context is borrowed by dispatch");
    return 0;
}
rcl_context_t rcl_get_zero_initialized_context(void){
    rcl_context_t c={
        0
    }
    ;
    return c;
}
rcl_node_t rcl_get_zero_initialized_node(void){
    rcl_node_t n={
        0
    }
    ;
    return n;
}
rcl_publisher_t rcl_get_zero_initialized_publisher(void){
    rcl_publisher_t p={
        0
    }
    ;
    return p;
}
rcl_subscription_t rcl_get_zero_initialized_subscription(void){
    rcl_subscription_t s={
        0
    }
    ;
    return s;
}
rcl_timer_t rcl_get_zero_initialized_timer(void){
    rcl_timer_t t={
        0
    }
    ;
    return t;
}
static bool node_storage(rcl_node_t const*n){
    return n&&known_object(n,1)&&n->uwvm_initialized&&context_storage(n->context,n->uwvm_generation);
}
static bool node_live(rcl_node_t const*n){
    return node_storage(n)&&rcl_context_is_valid(n->context);
}
static bool selection_valid(uwvm_rclc_selection const*s){
    size_t i,j;
    if(!s||!s->backend||s->backend->version!=2||!s->backend->open||!s->backend->close||!s->step||!s->maximum_spin_steps||s->maximum_spin_steps>1048576||s->topic_count>256||s->node_count>256||(!s->topics&&s->topic_count)||(!s->nodes&&s->node_count))return false;
    if(s->steady_clock_alias&&!bounded_text(s->steady_clock_alias,true))return false;
    for(i=0;
    i<s->node_count;
    ++i){
        if(!bounded_text(s->nodes[i].node,false)||!bounded_text(s->nodes[i].namespace_,true))return false;
        for(j=0;
        j<i;
        ++j)if(same(s->nodes[i].node,s->nodes[j].node)&&same(s->nodes[i].namespace_,s->nodes[j].namespace_))return false;
    }
    for(i=0;
    i<s->topic_count;
    ++i){
        uwvm_rclc_topic_binding const*b=s->topics+i;
        bool node=false;
        if(!bounded_text(b->node,false)||!bounded_text(b->namespace_,true)||!bounded_text(b->topic,false)||!same(b->type,string_key)||!bounded_text(b->alias,false)||(b->role!=UWVM_RCLC_PUBLISH&&b->role!=UWVM_RCLC_SUBSCRIBE)||b->maximum_wire_bytes<4||b->maximum_wire_bytes>UWVM_RCLC_MAX_WIRE)return false;
        for(j=0;
        j<s->node_count;
        ++j)if(same(b->node,s->nodes[j].node)&&same(b->namespace_,s->nodes[j].namespace_))node=true;
        if(!node)return false;
        for(j=0;
        j<i;
        ++j){
            uwvm_rclc_topic_binding const*a=s->topics+j;
            if(same(a->alias,b->alias))return false;
            if(same(a->node,b->node)&&same(a->namespace_,b->namespace_)&&same(a->topic,b->topic)&&a->role==b->role)return false;
        }
        if(!qos_equal(&b->qos,&rmw_qos_profile_default)&&!qos_equal(&b->qos,&rmw_qos_profile_sensor_data))return false;
        if(s->steady_clock_alias&&same(b->alias,s->steady_clock_alias))return false;
    }
    return service_selection_valid(s);
}
rcl_ret_t rclc_support_init(rclc_support_t*s,int argc,char const*const*argv,rcl_allocator_t*a){
    size_t i,slot=UWVM_RCLC_MAX_CONTEXTS;
    uwvm_rclc_selection const*select;
    uint32_t code;
    if(!s||argc<0||(argc&&(!argv||!argv[0]))||!rcutils_allocator_is_valid(a))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid support arguments/allocator");
    /* Find duplicates by address BEFORE reading/overwriting possibly uninitialized
  * caller storage. Registry holds only still-live, stable context addresses. */
    for(i=0;
    i<UWVM_RCLC_MAX_CONTEXTS;
    ++i){
        if(registered[i]==&s->context)return failure(&s->context,RCL_RET_ALREADY_INIT,17,"support already initialized");
        if(!registered[i]&&slot==UWVM_RCLC_MAX_CONTEXTS)slot=i;
    }
    if(slot==UWVM_RCLC_MAX_CONTEXTS||generation==UINT64_MAX)return failure(NULL,RCL_RET_BAD_ALLOC,90,"finite context capacity exhausted");
    if(argc>1)return failure(NULL,RCL_RET_UNSUPPORTED,95,"ROS arguments/remapping not implemented by this finite profile");
    select=uwvm_rclc_selected_configuration();
    if(!select)return failure(NULL,RCL_RET_ERROR,13,"no explicit trusted rclc selection");
    if(!selection_valid(select))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"malformed trusted rclc selection");
    memset(s,0,sizeof*s);
    s->context.uwvm_allocator=*a;
    s->context.uwvm_selection=select;
    s->context.uwvm_generation=++generation;
    roswasmc_local_context_init(&s->context.uwvm_lifecycle);
    code=roswasmc_session_init_context(&s->context.uwvm_session,select->backend,&s->context.uwvm_lifecycle,s->context.uwvm_owners,UWVM_RCLC_MAX_OWNERS);
    if(code)return raw_result(&s->context,code,"session initialization failed");
    code=roswasmc_session_check_abi(&s->context.uwvm_session);
    if(code)return raw_result(&s->context,code,"actual selected ROS2 ABI mismatch");
    s->allocator=a;
    s->clock.type=RCL_STEADY_TIME;
    s->clock.uwvm_context=&s->context;
    s->clock.uwvm_generation=s->context.uwvm_generation;
    s->context.uwvm_initialized=true;
    registered[slot]=&s->context;
    return 0;
}
static uint32_t close_owner(rcl_context_t*c,roswasmc_capability*owner){
    size_t i;
    uint32_t code;
    if(!owner||!owner->handle)return 0;
    i=(size_t)(owner-c->uwvm_owners);
    if(i>=c->uwvm_session.count)return 22;
    code=roswasmc_close(owner);
    c->uwvm_diagnostics.close_codes[i]=code;
    ++c->uwvm_diagnostics.close_attempts[i];
    ++c->uwvm_diagnostics.cleanup.attempted;
    if(code){
        ++c->uwvm_diagnostics.cleanup.failed;
        if(!c->uwvm_diagnostics.cleanup.cleanup_error)c->uwvm_diagnostics.cleanup.cleanup_error=code;
    }
    return code;
}
rcl_ret_t uwvm_rclc_close_context(rcl_context_t*c,rcl_ret_t primary,uint32_t raw){
    size_t n;
    uint32_t first=0;
    if(!c||!registered_context(c)||!c->uwvm_initialized)return failure(NULL,RCL_RET_NOT_INIT,22,"context is not initialized");
    if(c->uwvm_dispatch)return failure(c,RCL_RET_ERROR,17,"cannot close borrowed context");
    c->uwvm_diagnostics.primary=primary;
    c->uwvm_diagnostics.raw_primary=raw;
    c->uwvm_diagnostics.cleanup.primary_error=raw;
    n=c->uwvm_session.count;
    while(n){
        uint32_t code=close_owner(c,c->uwvm_owners+--n);
        if(code&&!first)first=code;
    }
    c->uwvm_clock=NULL;
    c->uwvm_shutdown=true;
    roswasmc_local_context_shutdown(&c->uwvm_lifecycle);
    return primary?primary:first?RCL_RET_ERROR:0;
}
rcl_ret_t rcl_shutdown(rcl_context_t*c){
    if(!c||!registered_context(c))return failure(NULL,RCL_RET_NOT_INIT,22,"context not initialized");
    if(c->uwvm_dispatch)return failure(c,RCL_RET_ERROR,17,"shutdown during dispatch refused");
    if(c->uwvm_shutdown)return failure(c,RCL_RET_ALREADY_SHUTDOWN,125,"already shutdown");
    c->uwvm_shutdown=true;
    roswasmc_local_context_shutdown(&c->uwvm_lifecycle);
    return 0;
}
rcl_ret_t rcl_context_fini(rcl_context_t*c){
    size_t i;
    if(!c||!registered_context(c))return failure(NULL,RCL_RET_NOT_INIT,22,"context not initialized");
    if(c->uwvm_dispatch)return failure(c,RCL_RET_ERROR,17,"context fini during dispatch refused");
    if(!c->uwvm_shutdown)return failure(c,RCL_RET_ERROR,17,"shutdown before context fini required");
    for(i=0;
    i<UWVM_RCLC_MAX_OBJECTS;
    ++i)if(objects[i].context==c&&objects[i].kind==5)return failure(c,RCL_RET_ERROR,17,"executor fini required before context fini");
    (void)uwvm_rclc_close_context(c,c->uwvm_diagnostics.primary,c->uwvm_diagnostics.raw_primary);
    service_context_release(c);
    for(i=0;
    i<UWVM_RCLC_MAX_CONTEXTS;
    ++i)if(registered[i]==c)registered[i]=NULL;
    for(i=0;
    i<UWVM_RCLC_MAX_OBJECTS;
    ++i)if(objects[i].context==c)memset(objects+i,0,sizeof objects[i]);
    c->uwvm_initialized=false;
    return c->uwvm_diagnostics.cleanup.cleanup_error?RCL_RET_ERROR:0;
}
/* fini reports its OWN cleanup status; diagnostics preserve the last actual
 * operation primary separately. A handled earlier denial is not a new close. */
rcl_ret_t rclc_support_fini(rclc_support_t*s){
    rcl_ret_t rc;
    size_t i;
    if(!s)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"NULL support");
    if(!registered_context(&s->context))return failure(NULL,RCL_RET_NOT_INIT,22,"support not initialized");
    if(s->context.uwvm_dispatch)return failure(&s->context,RCL_RET_ERROR,17,"support fini during dispatch refused");
    for(i=0;
    i<UWVM_RCLC_MAX_OBJECTS;
    ++i)if(objects[i].context==&s->context&&objects[i].kind==5)return failure(&s->context,RCL_RET_ERROR,17,"executor fini required before support fini");
    (void)uwvm_rclc_close_context(&s->context,s->context.uwvm_diagnostics.primary,s->context.uwvm_diagnostics.raw_primary);
    rc=rcl_context_fini(&s->context);
    s->clock.type=RCL_CLOCK_UNINITIALIZED;
    return rc;
}
uwvm_rclc_diagnostics const*uwvm_rclc_context_diagnostics(rcl_context_t const*c){
    return c?&c->uwvm_diagnostics:NULL;
}
rcl_ret_t rclc_node_init_default(rcl_node_t*n,char const*name,char const*ns,rclc_support_t*s){
    size_t i;
    rcl_ret_t rc;
    object_entry*slot;
    if(!n||!s||!bounded_text(name,false)||!bounded_text(ns,true))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid node arguments");
    rc=mutable_context(&s->context);
    if(rc)return rc;
    slot=object_slot(n);
    if(!slot)return object_admission_error(&s->context,n,"node already initialized or object capacity exhausted");
    for(i=0;
    i<s->context.uwvm_selection->node_count;
    ++i){
        uwvm_rclc_node_binding const*b=s->context.uwvm_selection->nodes+i;
        if(same(name,b->node)&&same(ns,b->namespace_)){
            n->context=&s->context;
            n->uwvm_generation=s->context.uwvm_generation;
            n->uwvm_name=b->node;
            n->uwvm_namespace=b->namespace_;
            n->uwvm_children=0;
            n->uwvm_initialized=true;
            remember_object(slot,n,&s->context,1);
            return 0;
        }
    }
    return failure(&s->context,RCL_RET_ERROR,13,"node spelling not in trusted selection");
}
rcl_ret_t rcl_node_fini(rcl_node_t*n){
    if(!node_storage(n))return failure(NULL,RCL_RET_NODE_INVALID,9,"invalid node");
    if(n->context->uwvm_dispatch)return failure(n->context,RCL_RET_ERROR,17,"node fini during dispatch refused");
    if(n->uwvm_children)return failure(n->context,RCL_RET_ERROR,17,"node still owns live resources");
    n->uwvm_initialized=false;
    forget_object(n);
    return 0;
}
static uwvm_rclc_topic_binding const*topic_binding(rcl_node_t const*n,char const*t,uwvm_rclc_role role,rosidl_message_type_support_t const*type,rmw_qos_profile_t const*q){
    size_t i;
    uwvm_rclc_selection const*s=n->context->uwvm_selection;
    if(type!=&string_support)return NULL;
    for(i=0;
    i<s->topic_count;
    ++i){
        uwvm_rclc_topic_binding const*b=s->topics+i;
        if(b->role==role&&same(n->uwvm_name,b->node)&&same(n->uwvm_namespace,b->namespace_)&&same(t,b->topic)&&qos_equal(q,&b->qos))return b;
    }
    return NULL;
}
rcl_ret_t rclc_publisher_init(rcl_publisher_t*p,rcl_node_t const*n,rosidl_message_type_support_t const*t,char const*topic,rmw_qos_profile_t const*q){
    uwvm_rclc_topic_binding const*b;
    roswasmc_capability*owner=NULL;
    rcl_ret_t rc;
    uint32_t raw;
    object_entry*slot;
    if(!p||!node_live(n)||!bounded_text(topic,false)||!t||!q)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid publisher arguments");
    rc=mutable_context(n->context);
    if(rc)return rc;
    slot=object_slot(p);
    if(!slot)return object_admission_error(n->context,p,"publisher already initialized or object capacity exhausted");
    b=topic_binding(n,topic,UWVM_RCLC_PUBLISH,t,q);
    if(!b)return failure(n->context,RCL_RET_UNSUPPORTED,95,"exact publisher node/topic/type/QoS binding absent");
    if(!n->context->uwvm_selection->backend->publish)return failure(n->context,RCL_RET_UNSUPPORTED,95,"publisher backend operation absent");
    raw=roswasmc_session_open(&n->context->uwvm_session,b->alias,strlen(b->alias),&owner);
    if(raw)return raw_result(n->context,raw,"publisher alias open failed");
    p->uwvm_node=n;
    p->uwvm_owner=owner;
    p->uwvm_generation=n->uwvm_generation;
    p->uwvm_binding=b;
    ++((rcl_node_t*)n)->uwvm_children;
    remember_object(slot,p,n->context,2);
    return 0;
}
rcl_ret_t rclc_publisher_init_default(rcl_publisher_t*p,rcl_node_t const*n,rosidl_message_type_support_t const*t,char const*s){
    return rclc_publisher_init(p,n,t,s,&rmw_qos_profile_default);
}
rcl_ret_t rclc_publisher_init_best_effort(rcl_publisher_t*p,rcl_node_t const*n,rosidl_message_type_support_t const*t,char const*s){
    return rclc_publisher_init(p,n,t,s,&rmw_qos_profile_sensor_data);
}
rcl_ret_t rclc_subscription_init(rcl_subscription_t*s,rcl_node_t*n,rosidl_message_type_support_t const*t,char const*topic,rmw_qos_profile_t const*q){
    uwvm_rclc_topic_binding const*b;
    roswasmc_capability*owner=NULL;
    rcl_ret_t rc;
    uint32_t raw;
    object_entry*slot;
    if(!s||!node_live(n)||!bounded_text(topic,false)||!t||!q)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid subscription arguments");
    rc=mutable_context(n->context);
    if(rc)return rc;
    slot=object_slot(s);
    if(!slot)return object_admission_error(n->context,s,"subscription already initialized or object capacity exhausted");
    b=topic_binding(n,topic,UWVM_RCLC_SUBSCRIBE,t,q);
    if(!b)return failure(n->context,RCL_RET_UNSUPPORTED,95,"exact subscription node/topic/type/QoS binding absent");
    if(!n->context->uwvm_selection->backend->subscribe||!n->context->uwvm_selection->backend->poll)return failure(n->context,RCL_RET_UNSUPPORTED,95,"subscription backend operations absent");
    raw=roswasmc_session_open(&n->context->uwvm_session,b->alias,strlen(b->alias),&owner);
    if(raw)return raw_result(n->context,raw,"subscription alias open failed");
    raw=roswasmc_subscribe(owner);
    if(raw){
        close_owner(n->context,owner);
        return raw_result(n->context,raw,"subscription activation failed; independent cleanup retained");
    }
    s->uwvm_node=n;
    s->uwvm_owner=owner;
    s->uwvm_generation=n->uwvm_generation;
    s->uwvm_binding=b;
    ++n->uwvm_children;
    remember_object(slot,s,n->context,3);
    return 0;
}
rcl_ret_t rclc_subscription_init_default(rcl_subscription_t*s,rcl_node_t*n,rosidl_message_type_support_t const*t,char const*p){
    return rclc_subscription_init(s,n,t,p,&rmw_qos_profile_default);
}
rcl_ret_t rclc_subscription_init_best_effort(rcl_subscription_t*s,rcl_node_t*n,rosidl_message_type_support_t const*t,char const*p){
    return rclc_subscription_init(s,n,t,p,&rmw_qos_profile_sensor_data);
}
rcl_ret_t rcl_publisher_fini(rcl_publisher_t*p,rcl_node_t*n){
    uint32_t raw;
    rcl_ret_t rc;
    if(!p||!known_object(p,2)||!n||p->uwvm_node!=n||!context_storage(n->context,p->uwvm_generation)||!p->uwvm_owner)return failure(NULL,RCL_RET_PUBLISHER_INVALID,9,"invalid publisher owner");
    if(n->context->uwvm_dispatch)return failure(n->context,RCL_RET_ERROR,17,"publisher borrowed during dispatch");
    rc=0;
    raw=close_owner(n->context,p->uwvm_owner);
    if(raw)rc=cleanup_result(n->context,raw,"publisher close failed");
    p->uwvm_owner=NULL;
    p->uwvm_binding=NULL;
    forget_object(p);
    if(n->uwvm_children)--n->uwvm_children;
    return rc;
}
rcl_ret_t rcl_subscription_fini(rcl_subscription_t*s,rcl_node_t*n){
    uint32_t raw;
    rcl_ret_t rc;
    if(!s||!known_object(s,3)||!n||s->uwvm_node!=n||!context_storage(n->context,s->uwvm_generation)||!s->uwvm_owner)return failure(NULL,RCL_RET_SUBSCRIPTION_INVALID,9,"invalid subscription owner");
    if(n->context->uwvm_dispatch)return failure(n->context,RCL_RET_ERROR,17,"subscription borrowed during dispatch");
    raw=close_owner(n->context,s->uwvm_owner);
    rc=raw?cleanup_result(n->context,raw,"subscription close failed"):0;
    s->uwvm_owner=NULL;
    s->uwvm_binding=NULL;
    forget_object(s);
    if(n->uwvm_children)--n->uwvm_children;
    return rc;
}
rcl_ret_t rcl_publish(rcl_publisher_t const*p,void const*message,rmw_publisher_allocation_t*allocation){
    std_msgs__msg__String const*m=message;
    unsigned char wire[UWVM_RCLC_MAX_WIRE];
    size_t length;
    uint32_t raw;
    rcl_context_t*c;
    if(!p||!known_object(p,2)||!p->uwvm_node||!node_live(p->uwvm_node)||!p->uwvm_owner||p->uwvm_generation!=p->uwvm_node->uwvm_generation||!p->uwvm_owner->handle)return failure(NULL,RCL_RET_PUBLISHER_INVALID,9,"invalid/stale publisher");
    c=p->uwvm_node->context;
    if(allocation)return failure(c,RCL_RET_UNSUPPORTED,95,"native loan allocation not supported");
    if(!m||!m->data.data||m->data.size>=m->data.capacity||m->data.data[m->data.size]!=0)return failure(c,RCL_RET_INVALID_ARGUMENT,22,"invalid owned String shape");
    {
        roswasmc_string text={
            m->data.data,m->data.size
        }
        ;
        raw=roswasmc_encode_string(text,wire,p->uwvm_binding->maximum_wire_bytes,&length);
    }
    if(raw)return raw_result(c,raw,"String encode/bound failed before publish");
    raw=roswasmc_publish(p->uwvm_owner,wire,length);
    return raw_result(c,raw,"publisher import failed without retry");
}
/* Lazily acquire the separate steady-time owner. Successful timer init needs an
 * actual8-byte time reply. No WASI/stdio wall clock substitutes for this grant. */
static rcl_ret_t clock_now(rcl_context_t*c,uint64_t*now){
    unsigned char bytes[8];
    roswasmc_result reply;
    roswasmc_capability*owner=c->uwvm_clock;
    uint32_t raw;
    bool fresh=false;
    rcl_ret_t rc;
    if(!rcl_context_is_valid(c)||!now)return failure(c,RCL_RET_NOT_INIT,125,"clock context inactive");
    if(!owner){
        char const*alias=c->uwvm_selection->steady_clock_alias;
        if(!bounded_text(alias,false))return failure(c,RCL_RET_ERROR,13,"no explicit steady clock alias");
        raw=roswasmc_check_operation(&c->uwvm_session.context,ROSWASMC_TIME_READ_STEADY);
        if(raw)return raw_result(c,raw,"steady clock backend absent");
        raw=roswasmc_session_open(&c->uwvm_session,alias,strlen(alias),&owner);
        if(raw)return raw_result(c,raw,"clock alias open failed");
        fresh=true;
    }
    reply=roswasmc_request(owner,ROSWASMC_TIME_READ_STEADY,NULL,0,bytes,8);
    raw=reply.error;
    if(!raw&&(!reply.has_length||reply.length!=8))raw=22;
    if(!raw){
        *now=uwvm_ros_load_u64(bytes);
        if(*now>INT64_MAX)raw=90;
    }
    if(raw){
        if(fresh)close_owner(c,owner);
        rc=raw_result(c,raw,"clock reply failed; no automatic retry");
        return rc;
    }
    if(fresh)c->uwvm_clock=owner;
    return 0;
}
rcl_ret_t rclc_timer_init_default2(rcl_timer_t*t,rclc_support_t*s,uint64_t period,rcl_timer_callback_t callback,bool autostart){
    uint64_t now;
    rcl_ret_t rc;
    object_entry*slot;
    bool fresh;
    if(!t||!s||!callback||period>INT64_MAX)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid timer arguments");
    rc=mutable_context(&s->context);
    if(rc)return rc;
    slot=object_slot(t);
    if(!slot)return object_admission_error(&s->context,t,"timer already initialized or object capacity exhausted");
    fresh=!s->context.uwvm_clock;
    rc=clock_now(&s->context,&now);
    if(rc)return rc;
    if(period>(uint64_t)INT64_MAX-now){
        if(fresh){
            close_owner(&s->context,s->context.uwvm_clock);
            s->context.uwvm_clock=NULL;
        }
        return failure(&s->context,RCL_RET_ERROR,90,"timer deadline overflow");
    }
    t->uwvm_context=&s->context;
    t->uwvm_generation=s->context.uwvm_generation;
    t->uwvm_callback=callback;
    t->uwvm_period=period;
    t->uwvm_last=now;
    t->uwvm_due=now+period;
    t->uwvm_canceled=!autostart;
    t->uwvm_initialized=true;
    remember_object(slot,t,&s->context,4);
    return 0;
}
rcl_ret_t rclc_timer_init_default(rcl_timer_t*t,rclc_support_t*s,uint64_t p,rcl_timer_callback_t c){
    return rclc_timer_init_default2(t,s,p,c,true);
}
static bool timer_storage(rcl_timer_t const*t){
    return t&&known_object(t,4)&&t->uwvm_initialized&&context_storage(t->uwvm_context,t->uwvm_generation);
}
static bool timer_live(rcl_timer_t const*t){
    return timer_storage(t)&&rcl_context_is_valid(t->uwvm_context);
}
rcl_ret_t rcl_timer_fini(rcl_timer_t*t){
    if(!timer_storage(t))return failure(NULL,RCL_RET_TIMER_INVALID,9,"invalid timer");
    if(t->uwvm_context->uwvm_dispatch)return failure(t->uwvm_context,RCL_RET_ERROR,17,"timer fini during dispatch refused");
    t->uwvm_initialized=false;
    t->uwvm_canceled=true;
    forget_object(t);
    return 0;
}
rcl_ret_t rcl_timer_cancel(rcl_timer_t*t){
    rcl_ret_t rc;
    if(!timer_live(t))return failure(NULL,RCL_RET_TIMER_INVALID,9,"invalid timer");
    rc=mutable_context(t->uwvm_context);
    if(rc)return rc;
    t->uwvm_canceled=true;
    return 0;
}
rcl_ret_t rcl_timer_reset(rcl_timer_t*t){
    rcl_ret_t rc;
    uint64_t now;
    if(!timer_live(t))return failure(NULL,RCL_RET_TIMER_INVALID,9,"invalid timer");
    rc=mutable_context(t->uwvm_context);
    if(rc)return rc;
    rc=clock_now(t->uwvm_context,&now);
    if(rc)return rc;
    if(t->uwvm_period>(uint64_t)INT64_MAX-now)return failure(t->uwvm_context,RCL_RET_ERROR,90,"timer reset overflow");
    t->uwvm_last=now;
    t->uwvm_due=now+t->uwvm_period;
    t->uwvm_canceled=false;
    return 0;
}
rcl_ret_t rcl_timer_is_canceled(rcl_timer_t const*t,bool*out){
    if(!timer_live(t)||!out)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid timer canceled query");
    *out=t->uwvm_canceled;
    return 0;
}
rcl_ret_t rcl_timer_get_period(rcl_timer_t const*t,int64_t*out){
    if(!timer_live(t)||!out)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid timer period query");
    *out=(int64_t)t->uwvm_period;
    return 0;
}
rclc_executor_t rclc_executor_get_zero_initialized_executor(void){
    rclc_executor_t e={
        0
    }
    ;
    return e;
}
static bool executor_live(rclc_executor_t const*e){
    return e&&known_object(e,5)&&e->uwvm_initialized&&context_storage(e->context,e->uwvm_generation)&&rcl_context_is_valid(e->context);
}
rcl_ret_t rclc_executor_init(rclc_executor_t*e,rcl_context_t*c,size_t max,rcl_allocator_t const*a){
    rclc_executor_handle_t*handles;
    rcl_ret_t rc;
    object_entry*slot;
    if(!e||!max||max>UWVM_RCLC_MAX_HANDLES||!rcutils_allocator_is_valid(a))return failure(c,RCL_RET_INVALID_ARGUMENT,22,"invalid executor arguments/capacity");
    rc=mutable_context(c);
    if(rc)return rc;
    slot=object_slot(e);
    if(!slot)return object_admission_error(c,e,"executor already initialized or object capacity exhausted");
    handles=a->zero_allocate(max,sizeof*handles,a->state);
    if(!handles)return failure(c,RCL_RET_BAD_ALLOC,0,"executor handle allocation failed before registration");
    memset(e,0,sizeof*e);
    e->context=c;
    e->uwvm_generation=c->uwvm_generation;
    e->uwvm_allocator=*a;
    e->handles=handles;
    e->max_handles=max;
    e->timeout_ns=1000000000ULL;
    e->uwvm_initialized=true;
    remember_object(slot,e,c,5);
    return 0;
}
rcl_ret_t rclc_executor_fini(rclc_executor_t*e){
    size_t i;
    if(!e)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"NULL executor");
    if(!known_object(e,5))return 0;
    if(e->uwvm_spinning||(e->context&&e->context->uwvm_dispatch))return failure(e->context,RCL_RET_ERROR,17,"executor borrowed by dispatch");
    for(i=0;
    i<e->index;
    ++i)e->uwvm_allocator.deallocate(e->handles[i].wire,e->uwvm_allocator.state);
    e->uwvm_allocator.deallocate(e->handles,e->uwvm_allocator.state);
    e->handles=NULL;
    e->index=e->max_handles=0;
    e->uwvm_initialized=false;
    forget_object(e);
    return 0;
}
static rcl_ret_t add_subscription(rclc_executor_t*e,rcl_subscription_t*s,void*m,rclc_subscription_callback_t cb,rclc_subscription_callback_with_context_t ctxcb,void*user,rclc_executor_handle_invocation_t inv){
    rclc_executor_handle_t h;
    size_t i;
    rcl_ret_t rc;
    if(!executor_live(e)||!s||!known_object(s,3)||!m||(!cb&&!ctxcb)||(inv!=ON_NEW_DATA&&inv!=ALWAYS))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid subscription registration");
    rc=mutable_context(e->context);
    if(rc)return rc;
    if(!node_live(s->uwvm_node)||s->uwvm_node->context!=e->context||s->uwvm_generation!=e->uwvm_generation||!s->uwvm_owner||!s->uwvm_owner->handle)return failure(e->context,RCL_RET_SUBSCRIPTION_INVALID,9,"cross-context/stale subscription");
    if(e->index>=e->max_handles)return failure(e->context,RCL_RET_ERROR,90,"executor capacity exhausted before allocation/poll");
    for(i=0;
    i<e->index;
    ++i)if(e->handles[i].subscription==s)return failure(e->context,RCL_RET_ERROR,17,"duplicate executor source");
    memset(&h,0,sizeof h);
    h.wire_capacity=s->uwvm_binding->maximum_wire_bytes;
    h.wire=e->uwvm_allocator.allocate(h.wire_capacity,e->uwvm_allocator.state);
    if(!h.wire)return failure(e->context,RCL_RET_BAD_ALLOC,0,"subscription scratch allocation failed");
    h.kind=1;
    h.subscription=s;
    h.message=m;
    h.callback=cb;
    h.with_context=ctxcb;
    h.user=user;
    h.invocation=inv;
    e->handles[e->index]=h;
    ++e->index;
    return 0;
}
rcl_ret_t rclc_executor_add_subscription(rclc_executor_t*e,rcl_subscription_t*s,void*m,rclc_subscription_callback_t c,rclc_executor_handle_invocation_t i){
    return add_subscription(e,s,m,c,NULL,NULL,i);
}
rcl_ret_t rclc_executor_add_subscription_with_context(rclc_executor_t*e,rcl_subscription_t*s,void*m,rclc_subscription_callback_with_context_t c,void*u,rclc_executor_handle_invocation_t i){
    return add_subscription(e,s,m,NULL,c,u,i);
}
rcl_ret_t rclc_executor_add_timer(rclc_executor_t*e,rcl_timer_t*t){
    size_t i;
    rcl_ret_t rc;
    rclc_executor_handle_t h;
    if(!executor_live(e)||!timer_live(t)||e->context!=t->uwvm_context)return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid timer registration");
    rc=mutable_context(e->context);
    if(rc)return rc;
    if(e->index>=e->max_handles)return failure(e->context,RCL_RET_ERROR,90,"executor capacity exhausted");
    for(i=0;
    i<e->index;
    ++i)if(e->handles[i].timer==t)return failure(e->context,RCL_RET_ERROR,17,"duplicate timer source");
    memset(&h,0,sizeof h);
    h.kind=2;
    h.timer=t;
    e->handles[e->index]=h;
    ++e->index;
    return 0;
}
rcl_ret_t rclc_executor_set_timeout(rclc_executor_t*e,uint64_t t){
    rcl_ret_t rc;
    if(!executor_live(e))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid executor");
    rc=mutable_context(e->context);
    if(rc)return rc;
    e->timeout_ns=t;
    return 0;
}
rcl_ret_t rclc_executor_prepare(rclc_executor_t*e){
    return executor_live(e)?mutable_context(e->context):failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid executor");
}
static rcl_ret_t timer_advance(rcl_timer_t*t,uint64_t now,int64_t*elapsed){
    uint64_t next,missed;
    if(now<t->uwvm_last)return failure(t->uwvm_context,RCL_RET_ERROR,22,"steady clock moved backwards");
    *elapsed=(int64_t)(now-t->uwvm_last);
    next=t->uwvm_due;
    if(!t->uwvm_period)next=now;
    else{
        if(t->uwvm_period>(uint64_t)INT64_MAX-next)return failure(t->uwvm_context,RCL_RET_ERROR,90,"timer phase overflow");
        next+=t->uwvm_period;
        if(next<=now){
            missed=(now-next)/t->uwvm_period+1;
            if(missed>((uint64_t)INT64_MAX-next)/t->uwvm_period)return failure(t->uwvm_context,RCL_RET_ERROR,90,"missed timer phase overflow");
            next+=missed*t->uwvm_period;
        }
    }
    t->uwvm_last=now;
    t->uwvm_due=next;
    return 0;
}
rcl_ret_t rclc_executor_spin_some(rclc_executor_t*e,uint64_t timeout){
    rcl_ret_t rc=0;
    uint32_t raw;
    size_t i;
    bool any=false,needs_clock=false;
    uint64_t now=0;
    if(!executor_live(e))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid executor");
    if(e->uwvm_spinning||e->context->uwvm_dispatch)return failure(e->context,RCL_RET_ERROR,17,"nested dispatch refused");
    if(e->uwvm_steps>=e->context->uwvm_selection->maximum_spin_steps)return failure(e->context,RCL_RET_TIMEOUT,110,"finite driver step budget exhausted");
    /* Lease the WHOLE context before the driver/callback can run. No native owner,
  * registered source, context state or executor storage can be closed/replaced
  * through the supported APIs during this pass. */
    e->uwvm_spinning=true;
    ++e->context->uwvm_dispatch;
    ++e->uwvm_steps;
    raw=e->context->uwvm_selection->step(e->context->uwvm_selection->step_context,timeout);
    if(raw==125){
        e->context->uwvm_shutdown=true;
        roswasmc_local_context_shutdown(&e->context->uwvm_lifecycle);
        goto done;
    }
    if(raw){
        rc=raw_result(e->context,raw,"explicit driver failed");
        goto done;
    }
    for(i=0;
    i<e->index;
    ++i){
        rclc_executor_handle_t*h=e->handles+i;
        h->ready=false;
        if(h->kind==2){
            if(!timer_live(h->timer)){
                rc=failure(e->context,RCL_RET_TIMER_INVALID,9,"stale timer source");
                goto done;
            }
            if(!h->timer->uwvm_canceled)needs_clock=true;
        }
        else if(h->kind>=3){
            rc=service_handle_validate(e,h);
            if(rc)goto done;
        }
        else if(!h->subscription||!node_live(h->subscription->uwvm_node)||!h->subscription->uwvm_owner||!h->subscription->uwvm_owner->handle){
            rc=failure(e->context,RCL_RET_SUBSCRIPTION_INVALID,9,"stale subscription source");
            goto done;
        }
    }
    if(needs_clock){
        rc=clock_now(e->context,&now);
        if(rc)goto done;
        for(i=0;
        i<e->index;
        ++i)if(e->handles[i].kind==2&&!e->handles[i].timer->uwvm_canceled&&now<e->handles[i].timer->uwvm_last){
            rc=failure(e->context,RCL_RET_ERROR,22,"steady time rollback before callbacks");
            goto done;
        }
    }
    /* Real selected callbacks keep original registration order and first-read,
  * then-process behavior. There is no native rcl_wait/rmw_wait fabricated here. */
    for(i=0;
    i<e->index;
    ++i){
        rclc_executor_handle_t*h=e->handles+i;
        if(h->kind==2){
            h->ready=!h->timer->uwvm_canceled&&now>=h->timer->uwvm_due;
            continue;
        }
        else if(h->kind>=3){
            rc=service_handle_gather(e,h);
            if(rc)goto done;
        }
        else{
            roswasmc_result result=roswasmc_poll(h->subscription->uwvm_owner,h->wire,h->wire_capacity);
            roswasmc_string view;
            if(result.error==UWVM_ROS_AGAIN)continue;
            if(result.error){
                rc=raw_result(e->context,result.error,"subscription poll failed without retry");
                goto done;
            }
            if(!result.has_length||result.length>h->wire_capacity||(raw=roswasmc_decode_string(h->wire,(size_t)result.length,&view))){
                rc=failure(e->context,RCL_RET_ERROR,22,"malformed complete canonical String event");
                goto done;
            }
            if(!rosidl_runtime_c__String__assignn(&h->message->data,view.data,view.size)){
                rc=failure(e->context,RCL_RET_BAD_ALLOC,0,"consumed String allocation failed; event not retried");
                goto done;
            }
            h->ready=true;
        }
    }
    for(i=0;
    i<e->index;
    ++i){
        rclc_executor_handle_t*h=e->handles+i;
        if(h->kind==2){
            if(h->ready){
                int64_t elapsed;
                rc=timer_advance(h->timer,now,&elapsed);
                if(rc)goto done;
                h->timer->uwvm_callback(h->timer,elapsed);
                any=true;
            }
        }
        else if(h->kind>=3){
            rc=service_handle_dispatch(e,h,&any);
            if(rc)goto done;
        }
        else if(h->ready||h->invocation==ALWAYS){
            void const*msg=h->ready?h->message:NULL;
            if(h->with_context)h->with_context(msg,h->user);
            else h->callback(msg);
            any=true;
        }
    }
    if(!any)rc=RCL_RET_TIMEOUT;
    done:--e->context->uwvm_dispatch;
    e->uwvm_spinning=false;
    return rc;
}
rcl_ret_t rclc_executor_spin(rclc_executor_t*e){
    rcl_ret_t rc;
    if(!executor_live(e))return failure(NULL,RCL_RET_INVALID_ARGUMENT,22,"invalid executor");
    while(rcl_context_is_valid(e->context)){
        rc=rclc_executor_spin_some(e,e->timeout_ns);
        if(rc!=RCL_RET_OK&&rc!=RCL_RET_TIMEOUT)return rc;
        if(e->uwvm_steps>=e->context->uwvm_selection->maximum_spin_steps&&rcl_context_is_valid(e->context))return failure(e->context,RCL_RET_TIMEOUT,110,"finite spin budget exhausted; explicit shutdown required");
    }
    return 0;
}

/* Compiled into this SAME translation unit: no duplicate Context/owner registry. */
#include "services.inc"
