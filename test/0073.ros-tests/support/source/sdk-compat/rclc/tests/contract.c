/* Unexecuted source contracts: actual adapter implementation + complete original
 * upstream TUs, backed by a local logic bridge. No native ROS/Guest claim. */
#include <rclc/rclc.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
/* Genuine public source types/signatures: these assertions are compiled with
 * this real adapter, including on wasm32; they do not assert native ABI layout. */
_Static_assert(sizeof(((rcutils_error_state_t*)0)->line_number)==sizeof(uint64_t),
    "rcutils line_number retains its original uint64_t width");
_Static_assert(RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE==2,
    "deprecated original enum spelling/value stays self-contained");
static rcl_ret_t(*const executor_init_exact_signature)(rclc_executor_t*,
    rcl_context_t*,size_t,rcl_allocator_t const*)=rclc_executor_init;
int upstream_executor_main(int,char const**);
int upstream_sub_context_main(int,char const**);
static unsigned groups,checks;
#define CHECK(v) do{++checks;if(!(v)){fprintf(stderr,"FAIL_RCLC_SOURCE_CONTRACT line=%u\n",(unsigned)__LINE__);abort();}}while(0)
static unsigned opens,closes,publishes,polls,time_calls,steps,callbacks;
static unsigned next_handle,mode;
static bool configured=true;
static uint64_t now;
static unsigned char sent[1028];
static size_t sent_size;
static struct {
    bool live;
    unsigned role,topic;
    bool queued;
    unsigned char data[1028];
    size_t length;
}
handles[257];
static void store64(uint64_t address,uint64_t v){
    unsigned char*p=(unsigned char*)(uintptr_t)address;
    size_t i;
    for(i=0;
    i<8;
    ++i)p[i]=(unsigned char)(v>>(8*i));
}
static uint32_t abi_version(void){
    return mode==6?0:1;
}
static uint32_t open_alias(uint32_t memory,uint64_t address,uint64_t length,uint64_t out){
    char alias[32];
    unsigned role=0,topic=0,h;
    CHECK(memory==0&&length<sizeof alias);
    memcpy(alias,(void*)(uintptr_t)address,(size_t)length);
    alias[length]=0;
    ++opens;
    if(mode==1)return 13;
    if(!strcmp(alias,"steady")){
        role=3;
    }
    else if(!strncmp(alias,"pub-",4)){
        role=1;
        topic=(unsigned)(alias[4]-'0');
    }
    else if(!strncmp(alias,"sub-",4)){
        role=2;
        topic=(unsigned)(alias[4]-'0');
    }
    else return 13;
    CHECK(next_handle<256);
    h=++next_handle;
    handles[h].live=true;
    handles[h].role=role;
    handles[h].topic=topic;
    store64(out,h);
    return 0;
}
static uint32_t close_alias(uint64_t h){
    CHECK(h&&h<=next_handle&&handles[h].live);
    handles[h].live=false;
    ++closes;
    return mode==2||mode==3?107:0;
}
static uint32_t subscribe_alias(uint64_t h){
    CHECK(h&&h<=next_handle&&handles[h].live&&handles[h].role==2);
    return mode==2?13:0;
}
static uint32_t publish_alias(uint64_t h,uint32_t mem,uint64_t input,uint64_t n){
    unsigned i;
    CHECK(mem==0&&h&&handles[h].live&&handles[h].role==1&&n<=sizeof sent);
    ++publishes;
    memcpy(sent,(void*)(uintptr_t)input,(size_t)n);
    sent_size=(size_t)n;
    for(i=1;
    i<=next_handle;
    ++i)if(handles[i].live&&handles[i].role==2&&handles[i].topic==handles[h].topic){
        memcpy(handles[i].data,sent,sent_size);
        handles[i].length=sent_size;
        handles[i].queued=true;
    }
    return 0;
}
static uint32_t poll_alias(uint64_t h,uint32_t mem,uint64_t out,uint64_t cap,uint64_t length){
    CHECK(mem==0&&h&&handles[h].live&&handles[h].role==2);
    if(mode==4){
        CHECK(cap>=1);
        *(unsigned char*)(uintptr_t)out=255;
        store64(length,1);
        return 0;
    }
    if(!handles[h].queued)return 11;
    CHECK(cap>=handles[h].length);
    memcpy((void*)(uintptr_t)out,handles[h].data,handles[h].length);
    store64(length,handles[h].length);
    handles[h].queued=false;
    ++polls;
    return 0;
}
static uint32_t clock_request(uint64_t h,uint32_t mem,uint64_t in,uint64_t n,uint64_t out,uint64_t cap,uint64_t len){
    (void)in;
    CHECK(h&&handles[h].live&&handles[h].role==3&&mem==0&&n==0&&cap==8);
    ++time_calls;
    if(mode==5)return 110;
    store64(out,now);
    store64(len,8);
    return 0;
}
static uint32_t step(void*user,uint64_t timeout){
    (void)user;
    (void)timeout;
    ++steps;
    now+=1000000000ULL;
    return mode==10&&steps>2?125:0;
}
static roswasmc_backend backend;
static uwvm_rclc_topic_binding topics[8];
static uwvm_rclc_node_binding const nodes[]={
    {
        "node_0","executor_examples"
    }
}
;
static uwvm_rclc_selection selection;
uwvm_rclc_selection const*uwvm_rclc_selected_configuration(void){
    return configured?&selection:NULL;
}
static void reset(unsigned m){
    size_t i;
    mode=m;
    opens=closes=publishes=polls=time_calls=steps=callbacks=0;
    next_handle=0;
    now=0;
    sent_size=0;
    configured=true;
    memset(handles,0,sizeof handles);
    memset(&backend,0,sizeof backend);
    backend.version=2;
    backend.abi_version=abi_version;
    backend.open=open_alias;
    backend.close=close_alias;
    backend.publish=publish_alias;
    backend.subscribe=subscribe_alias;
    backend.poll=poll_alias;
    backend.request[ROSWASMC_TIME_READ_STEADY]=clock_request;
    for(i=0;
    i<8;
    ++i){
        static char const*names[]={
            "topic_0","topic_foo","topic_bar","topic_baz"
        }
        ;
        static char const*aliases[]={
            "pub-0","sub-0","pub-1","sub-1","pub-2","sub-2","pub-3","sub-3"
        }
        ;
        topics[i].node="node_0";
        topics[i].namespace_="executor_examples";
        topics[i].topic=names[i/2];
        topics[i].type="std_msgs/msg/String";
        topics[i].role=i%2?UWVM_RCLC_SUBSCRIBE:UWVM_RCLC_PUBLISH;
        topics[i].alias=aliases[i];
        topics[i].qos=rmw_qos_profile_default;
        topics[i].maximum_wire_bytes=1028;
    }
    selection.backend=&backend;
    selection.topics=topics;
    selection.topic_count=8;
    selection.nodes=nodes;
    selection.node_count=1;
    selection.steady_clock_alias="steady";
    selection.step=step;
    selection.step_context=NULL;
    selection.maximum_spin_steps=4096;
    rcutils_reset_error();
}
static void all_closed(void){
    unsigned i;
    for(i=1;
    i<=next_handle;
    ++i)CHECK(!handles[i].live);
}
static void begin(rclc_support_t*s,rcl_node_t*n){
    rcl_allocator_t a=rcl_get_default_allocator();
    /* support stores allocator pointer only for source shape; context holds an actual callback/state COPY for ownership. */CHECK(rclc_support_init(s,0,NULL,&a)==0);
    *n=rcl_get_zero_initialized_node();
    CHECK(rclc_node_init_default(n,"node_0","executor_examples",s)==0);
}
static void end(rclc_support_t*s,rcl_node_t*n){
    CHECK(rcl_node_fini(n)==0);
    CHECK(rclc_support_fini(s)==0);
    all_closed();
}
static void count_callback(void const*m){
    CHECK(m!=NULL);
    ++callbacks;
}
static rclc_support_t*borrow_support;
static rcl_publisher_t*borrow_pub;
static rclc_executor_t*borrow_executor;
static unsigned rejected;
static void lease_callback(void const*m){
    rcl_node_t extra=rcl_get_zero_initialized_node();
    CHECK(m);
    CHECK(rcl_shutdown(&borrow_support->context)==RCL_RET_ERROR);
    CHECK(borrow_support->context.uwvm_diagnostics.raw_primary==17);
    CHECK(rcl_publisher_fini(borrow_pub,(rcl_node_t*)borrow_pub->uwvm_node)==RCL_RET_ERROR);
    CHECK(rclc_support_fini(borrow_support)==RCL_RET_ERROR);
    CHECK(rclc_executor_fini(borrow_executor)==RCL_RET_ERROR);
    CHECK(rclc_node_init_default(&extra,"node_0","executor_examples",borrow_support)==RCL_RET_ERROR);
    CHECK(rcl_context_is_valid(&borrow_support->context));
    rejected+=5;
}
static int64_t elapsed_values[8];
static unsigned timer_count;
static void timer_callback(rcl_timer_t*t,int64_t elapsed){
    CHECK(t);
    elapsed_values[timer_count++]=elapsed;
}
static void*fail_alloc(size_t n,void*s){
    (void)n;
    (void)s;
    return NULL;
}
static void*fail_zero(size_t n,size_t width,void*state){(void)n;(void)width;(void)state;return NULL;}
int main(void){
    char const*args[]={
        "official",NULL
    }
    ;
    rcl_allocator_t allocator=rcl_get_default_allocator();
    rcl_allocator_t const immutable_allocator=allocator;
    /*1: complete unchanged publisher/subscription/timer/executor original. */
    reset(10);
    CHECK(upstream_executor_main(1,args)==0);
    CHECK(publishes==2&&polls==1&&closes==3);
    {
        roswasmc_string text;
        CHECK(roswasmc_decode_string(sent,sent_size,&text)==0);
        CHECK(text.size==12&&!memcmp(text.data,"Hello World!",12));
    }
    all_closed();
    ++groups;
    /*2: original full3topic/context example; malloc-created native-style
  * uninitialized publishers must initialize without reading their old bytes. */
    reset(0);
    CHECK(upstream_sub_context_main(1,args)==0);
    CHECK(publishes==30&&polls==30&&opens==6&&time_calls==0);
    all_closed();
    ++groups;
    /*3: no trusted selection and wrong ABI produce zero opens. */
    {
        rclc_support_t s;
        reset(0);
        configured=false;
        CHECK(rclc_support_init(&s,0,NULL,&allocator)==RCL_RET_ERROR);
        CHECK(opens==0&&rcl_error_is_set());
        reset(6);
        CHECK(rclc_support_init(&s,0,NULL,&allocator)==RCL_RET_UNSUPPORTED);
        CHECK(!rcl_context_is_valid(&s.context)&&opens==0);
        /* Public setter still takes size_t, while the retained state and its
         * formatting use uint64_t. The host64 case reaches all20 digits; the
         * unconditional type assertion above covers the wasm32 distinction. */
        rcutils_set_error_state("line","file",(size_t)7);
        CHECK(rcutils_get_error_state()->line_number==UINT64_C(7));
        CHECK(!strcmp(rcutils_get_error_string().str,"line, at file:7"));
#if SIZE_MAX == UINT64_MAX
        rcutils_set_error_state("line","file",SIZE_MAX);
        CHECK(rcutils_get_error_state()->line_number==UINT64_MAX);
        CHECK(!strcmp(rcutils_get_error_string().str,
            "line, at file:18446744073709551615"));
#endif
        rcutils_reset_error();
    }
    ++groups;
    /*4: exact QoS and node spelling must match BEFORE first capability effect. */
    {
        rclc_support_t s;
        rcl_node_t n;
        rcl_publisher_t p;
        reset(0);
        begin(&s,&n);
        memset(&p,0xa5,sizeof p);
        CHECK(rclc_publisher_init_best_effort(&p,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==RCL_RET_UNSUPPORTED);
        CHECK(opens==0);
        CHECK(rclc_publisher_init_default(&p,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==0);
        CHECK(rcl_publisher_fini(&p,&n)==0);
        end(&s,&n);
    }
    ++groups;
    /*5: denied activation retains original13 AND failed close107 independently;
  * no retry and no invented failed-open close. */
    {
        rclc_support_t s;
        rcl_node_t n;
        rcl_subscription_t sub;
        uwvm_rclc_diagnostics const*d;
        reset(2);
        begin(&s,&n);
        CHECK(rclc_subscription_init_default(&sub,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==RCL_RET_ERROR);
        d=uwvm_rclc_context_diagnostics(&s.context);
        CHECK(d->raw_primary==13&&d->cleanup.cleanup_error==107&&closes==1&&d->close_attempts[0]==1);
        CHECK(rcl_node_fini(&n)==0);
        CHECK(rclc_support_fini(&s)==RCL_RET_ERROR);
        all_closed();
        reset(1);
        CHECK(rclc_support_init(&s,0,NULL,&allocator)==0);
        n=rcl_get_zero_initialized_node();
        CHECK(rclc_node_init_default(&n,"node_0","executor_examples",&s)==0);
        CHECK(rclc_subscription_init_default(&sub,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==RCL_RET_ERROR);
        CHECK(closes==0);
        CHECK(rcl_node_fini(&n)==0);
        CHECK(rclc_support_fini(&s)==0);
    }
    ++groups;
    /*6: one whole-context lease rejects all teardown/new registration BEFORE
  * mutation while a real selected callback runs. */
    {
        rclc_support_t s;
        rcl_node_t n;
        rcl_publisher_t p;
        rcl_subscription_t sub;
        rclc_executor_t e=rclc_executor_get_zero_initialized_executor();
        std_msgs__msg__String msg;
        reset(0);
        begin(&s,&n);
        CHECK(std_msgs__msg__String__init(&msg));
        CHECK(rclc_publisher_init_default(&p,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==0);
        CHECK(rclc_subscription_init_default(&sub,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==0);
        CHECK(rclc_executor_init(&e,&s.context,1,&allocator)==0);
        CHECK(rclc_executor_add_subscription(&e,&sub,&msg,lease_callback,ON_NEW_DATA)==0);
        CHECK(rosidl_runtime_c__String__assign(&msg.data,"lease"));
        CHECK(rcl_publish(&p,&msg,NULL)==0);
        borrow_support=&s;
        borrow_pub=&p;
        borrow_executor=&e;
        rejected=0;
        CHECK(rclc_executor_spin_some(&e,0)==0);
        CHECK(rejected==5&&closes==0);
        CHECK(rclc_executor_fini(&e)==0);
        CHECK(rcl_subscription_fini(&sub,&n)==0);
        CHECK(rcl_publisher_fini(&p,&n)==0);
        CHECK(rcl_node_fini(&n)==0);
        /* expected refusal remains diagnostic, not a fictitious close failure */CHECK(rclc_support_fini(&s)==0);
        std_msgs__msg__String__fini(&msg);
        all_closed();
    }
    ++groups;
    /*7: malformed complete event never invokes observer; consumed data is not
  * replayed, all real owners still retire once. */
    {
        rclc_support_t s;
        rcl_node_t n;
        rcl_subscription_t sub;
        rclc_executor_t e=rclc_executor_get_zero_initialized_executor();
        std_msgs__msg__String msg;
        reset(4);
        begin(&s,&n);
        CHECK(std_msgs__msg__String__init(&msg));
        CHECK(rclc_subscription_init_default(&sub,&n,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==0);
        CHECK(rclc_executor_init(&e,&s.context,1,&allocator)==0);
        CHECK(rclc_executor_add_subscription(&e,&sub,&msg,count_callback,ON_NEW_DATA)==0);
        CHECK(rclc_executor_spin_some(&e,0)==RCL_RET_ERROR);
        CHECK(callbacks==0&&s.context.uwvm_diagnostics.raw_primary==22);
        CHECK(rclc_executor_fini(&e)==0);
        CHECK(rcl_subscription_fini(&sub,&n)==0);
        CHECK(rcl_node_fini(&n)==0);
        CHECK(rclc_support_fini(&s)==0);
        std_msgs__msg__String__fini(&msg);
        all_closed();
    }
    ++groups;
    /*8: genuine allocation/capacity failures remain local. Existing subscription
     * owners are never silently adopted/closed by a failed registration. */
    {
        rclc_support_t support;
        rcl_node_t node;
        rcl_subscription_t subscription;
        std_msgs__msg__String message;
        rclc_executor_t executor=rclc_executor_get_zero_initialized_executor();
        rcl_allocator_t bad=allocator;
        reset(0);
        begin(&support,&node);
        bad.zero_allocate=NULL;
        CHECK(rclc_executor_init(&executor,&support.context,1,&bad)==RCL_RET_INVALID_ARGUMENT);
        bad=allocator;
        bad.zero_allocate=fail_zero;
        CHECK(rclc_executor_init(&executor,&support.context,1,&bad)==RCL_RET_BAD_ALLOC);
        CHECK(opens==0);
        CHECK(std_msgs__msg__String__init(&message));
        CHECK(rclc_subscription_init_default(&subscription,&node,ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs,msg,String),"topic_0")==0);
        bad=allocator;
        bad.allocate=fail_alloc;
        CHECK(rclc_executor_init(&executor,&support.context,1,&bad)==0);
        CHECK(rclc_executor_add_subscription(&executor,&subscription,&message,count_callback,ON_NEW_DATA)==RCL_RET_BAD_ALLOC);
        CHECK(executor.index==0&&closes==0);
        CHECK(rclc_executor_fini(&executor)==0);
        CHECK(executor_init_exact_signature(&executor,&support.context,1,&immutable_allocator)==0);
        CHECK(rclc_executor_add_subscription(&executor,&subscription,&message,count_callback,ON_NEW_DATA)==0);
        CHECK(rclc_executor_add_subscription(&executor,&subscription,&message,count_callback,ON_NEW_DATA)==RCL_RET_ERROR);
        CHECK(support.context.uwvm_diagnostics.raw_primary==90&&executor.index==1&&closes==0);
        CHECK(rclc_executor_fini(&executor)==0);
        CHECK(rcl_subscription_fini(&subscription,&node)==0);
        CHECK(rcl_node_fini(&node)==0);
        CHECK(rclc_support_fini(&support)==0);
        std_msgs__msg__String__fini(&message);
        all_closed();
    }
    ++groups;
    /*9: timer phase remains original start+N*period, not now+period; real
  * independent clock failures roll back the fresh clock owner. */
    {
        rclc_support_t s;
        rcl_node_t n;
        rcl_timer_t t;
        rclc_executor_t e=rclc_executor_get_zero_initialized_executor();
        reset(0);
        begin(&s,&n);
        now=0;
        CHECK(rclc_timer_init_default2(&t,&s,600000000ULL,timer_callback,true)==0);
        CHECK(rclc_executor_init(&e,&s.context,1,&allocator)==0);
        CHECK(rclc_executor_add_timer(&e,&t)==0);
        timer_count=0;
        CHECK(rclc_executor_spin_some(&e,0)==0);
        CHECK(t.uwvm_due==1200000000ULL&&elapsed_values[0]==1000000000LL);
        CHECK(rclc_executor_spin_some(&e,0)==0);
        CHECK(t.uwvm_due==2400000000ULL&&timer_count==2);
        CHECK(rclc_executor_fini(&e)==0);
        CHECK(rcl_timer_fini(&t)==0);
        end(&s,&n);
        reset(5);
        begin(&s,&n);
        CHECK(rclc_timer_init_default2(&t,&s,1,timer_callback,true)==RCL_RET_ERROR);
        CHECK(opens==1&&closes==1&&s.context.uwvm_clock==NULL);
        CHECK(rcl_node_fini(&n)==0);
        CHECK(rclc_support_fini(&s)==0);
        all_closed();
    }
    ++groups;
    /*10: String data remains genuinely owned and NUL-terminated across assign,
  * independent copy, equal and fini; the canonical codec has actual UTF8. */
    {
        std_msgs__msg__String a,b;
        reset(0);
        CHECK(std_msgs__msg__String__init(&a)&&std_msgs__msg__String__init(&b));
        CHECK(a.data.size==0&&a.data.capacity==1&&a.data.data[0]==0);
        CHECK(rosidl_runtime_c__String__assign(&a.data,"complete"));
        CHECK(std_msgs__msg__String__copy(&a,&b));
        CHECK(a.data.data!=b.data.data&&std_msgs__msg__String__are_equal(&a,&b));
        a.data.data[0]='C';
        CHECK(!std_msgs__msg__String__are_equal(&a,&b));
        std_msgs__msg__String__fini(&a);
        std_msgs__msg__String__fini(&b);
        CHECK(a.data.data==NULL&&a.data.size==0&&a.data.capacity==0);
    }
    ++groups;
    printf("PASS_RCLC_SOURCE_CONTRACT groups=%u checks=%u\n",groups,checks);
    return 0;
}
