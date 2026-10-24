/* SPDX-License-Identifier: Apache-2.0
 * Actual pthread + original installed RMW/rcutils API contracts. The backend
 * below is an explicitly LOCAL deny-open model: never ROS/Host/native evidence.
 * Each named group is a fresh process. Root owns external deadlines and joins.
 */
#include <platform/ros2/c/uwvm_rmw/internal.h>
#include <platform/ros2/c/uwvm_rmw/threaded.h>
#include <platform/ros2/c/uwvm_rmw_c/provider.h>
#include <platform/ros2/c/uwvm_rmw_platform.h>
#include <rcutils/allocator.h>
#include <rcutils/strdup.h>
#include <rmw/error_handling.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef NDEBUG
#error "Real assertions must execute in every configuration"
#endif
#include <assert.h>
static atomic_uint opens,closes,allocator_reentries,allocator_reentry_failures;
static atomic_int test_reentry;
static rmw_context_t contexts[2];
static uint32_t abi(void){return 1;}
static uint32_t deny_open(uint32_t m,uint64_t n,uint64_t z,uint64_t out){(void)m;(void)n;(void)z;(void)out;atomic_fetch_add(&opens,1);return 13;}
static uint32_t close_raw(uint64_t h){assert(h);atomic_fetch_add(&closes,1);return 0;}
static roswasmc_backend const deny_backend={.version=2,.abi_version=abi,.open=deny_open,.close=close_raw};
static uwvm_rmw_profile profile;
static void reenter(void){if(atomic_load(&test_reentry)){
 atomic_fetch_add(&allocator_reentries,1);
 if(rmw_create_guard_condition(&contexts[0])!=NULL)atomic_fetch_add(&allocator_reentry_failures,1);
 rmw_reset_error();}}
static void*allocate(size_t n,void*s){(void)s;reenter();return malloc(n);}
static void deallocate(void*p,void*s){(void)s;reenter();free(p);}
static void*reallocate(void*p,size_t n,void*s){(void)s;reenter();return realloc(p,n);}
static void*zero_allocate(size_t n,size_t z,void*s){(void)s;reenter();return calloc(n,z);}
static rcutils_allocator_t allocator(void){return (rcutils_allocator_t){allocate,deallocate,reallocate,zero_allocate,NULL};}
static void setup(void){
 profile=(uwvm_rmw_profile){.version=1,.backend=&deny_backend,.graph=uwvm_rmw_c_graph_provider()};
 assert(uwvm_rmw_install_profile(&profile)==RMW_RET_OK);
 static uwvm_rmw_c_profile cprofile={.version=1};assert(uwvm_rmw_c_install_profile(&cprofile)==RMW_RET_OK);
}
static void init_context(size_t index){
 rmw_init_options_t o=rmw_get_zero_initialized_init_options();rcutils_allocator_t a=allocator();
 assert(rmw_init_options_init(&o,a)==RMW_RET_OK);o.instance_id=index+1;o.enclave=rcutils_strdup("/",a);assert(o.enclave);
 contexts[index]=rmw_get_zero_initialized_context();assert(rmw_init(&o,&contexts[index])==RMW_RET_OK);
 assert(rmw_init_options_fini(&o)==RMW_RET_OK);
}
static void finish_context(size_t index){assert(rmw_shutdown(&contexts[index])==RMW_RET_OK);assert(rmw_context_fini(&contexts[index])==RMW_RET_OK);}
typedef struct wait_args {rmw_context_t*c;rmw_guard_condition_t*g;rmw_wait_set_t*w;rmw_ret_t result;void*returned;atomic_int entered;} wait_args;
static void*waiter(void*object){wait_args*a=object;void*slot=a->g;rmw_guard_conditions_t list={1,&slot};
 atomic_store_explicit(&a->entered,1,memory_order_release);
 a->result=rmw_wait(NULL,&list,NULL,NULL,NULL,a->w,NULL);a->returned=slot;return NULL;}
static void wait_admitted(wait_args*a){
 /* No poll count is interpreted as elapsed time. The outer owned process must
  * have an actual deadline; this loop observes real admission diagnostics. */
 for(;;){uwvm_rmw_threaded_diagnostics d;
  assert(uwvm_rmw_get_threaded_diagnostics(a->c,&d)==RMW_RET_OK);
  if(d.active_waits)return;sched_yield();}
}
static wait_args make_wait(size_t i){wait_args a={0};a.c=&contexts[i];
 a.g=rmw_create_guard_condition(a.c);a.w=rmw_create_wait_set(a.c,0);assert(a.g&&a.w);return a;}
static void retire_wait(wait_args*a,int guard_live,int set_live){
 if(guard_live)assert(rmw_destroy_guard_condition(a->g)==RMW_RET_OK);
 if(set_live)assert(rmw_destroy_wait_set(a->w)==RMW_RET_OK);
}
static void infinite_guard(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 assert(rmw_trigger_guard_condition(a.g)==RMW_RET_OK);assert(!pthread_join(t,NULL));
 assert(a.result==RMW_RET_OK&&a.returned==a.g);retire_wait(&a,1,1);finish_context(0);
}
static void trigger_before_wait(void){
 init_context(0);wait_args a=make_wait(0);assert(rmw_trigger_guard_condition(a.g)==RMW_RET_OK);
 assert(waiter(&a)==NULL&&a.result==RMW_RET_OK&&a.returned==a.g);
 void*slot=a.g;rmw_guard_conditions_t list={1,&slot};rmw_time_t zero={0};
 assert(rmw_wait(NULL,&list,NULL,NULL,NULL,a.w,&zero)==RMW_RET_TIMEOUT&&slot==NULL);
 retire_wait(&a,1,1);finish_context(0);
}
static void shutdown_wakes_before_join(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 assert(rmw_shutdown(a.c)==RMW_RET_OK);assert(!pthread_join(t,NULL));
 assert(a.result==RMW_RET_OK&&a.returned==a.g); /* actual locally triggered guard */
 /* Original on_shutdown callback may trigger the same guard after RMW close. */
 assert(rmw_trigger_guard_condition(a.g)==RMW_RET_OK);retire_wait(&a,1,1);finish_context(0);
}
static void destroy_guard_while_pinned(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 assert(rmw_destroy_guard_condition(a.g)==RMW_RET_OK);assert(!pthread_join(t,NULL));
 assert(a.result!=RMW_RET_OK&&a.result!=RMW_RET_TIMEOUT&&a.returned==a.g);rmw_reset_error();
 retire_wait(&a,0,1);finish_context(0);
}
static void destroy_set_while_pinned(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 assert(rmw_destroy_wait_set(a.w)==RMW_RET_OK);assert(!pthread_join(t,NULL));
 assert(a.result!=RMW_RET_OK&&a.result!=RMW_RET_TIMEOUT);rmw_reset_error();retire_wait(&a,1,0);finish_context(0);
}
static void same_waitset_refused(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 void*slot=a.g;rmw_guard_conditions_t list={1,&slot};rmw_time_t zero={0};
 assert(rmw_wait(NULL,&list,NULL,NULL,NULL,a.w,&zero)==RMW_RET_ERROR&&slot==a.g);rmw_reset_error();
 assert(rmw_trigger_guard_condition(a.g)==RMW_RET_OK);assert(!pthread_join(t,NULL));retire_wait(&a,1,1);finish_context(0);
}
static void same_entity_refused(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 rmw_wait_set_t*other=rmw_create_wait_set(a.c,1);assert(other);void*slot=a.g;rmw_guard_conditions_t list={1,&slot};rmw_time_t zero={0};
 assert(rmw_wait(NULL,&list,NULL,NULL,NULL,other,&zero)==RMW_RET_ERROR&&slot==a.g);rmw_reset_error();
 assert(rmw_trigger_guard_condition(a.g)==RMW_RET_OK);assert(!pthread_join(t,NULL));assert(rmw_destroy_wait_set(other)==RMW_RET_OK);
 retire_wait(&a,1,1);finish_context(0);
}
static void two_contexts(void){
 init_context(0);init_context(1);wait_args a=make_wait(0),b=make_wait(1);pthread_t x,y;
 assert(!pthread_create(&x,NULL,waiter,&a)&&!pthread_create(&y,NULL,waiter,&b));wait_admitted(&a);wait_admitted(&b);
 assert(rmw_trigger_guard_condition(b.g)==RMW_RET_OK&&rmw_trigger_guard_condition(a.g)==RMW_RET_OK);
 assert(!pthread_join(x,NULL)&&!pthread_join(y,NULL));assert(a.result==RMW_RET_OK&&b.result==RMW_RET_OK);
 retire_wait(&a,1,1);retire_wait(&b,1,1);finish_context(1);finish_context(0);
}
static void cross_context_refused(void){
 init_context(0);init_context(1);wait_args a=make_wait(0),b=make_wait(1);void*slot=b.g;rmw_guard_conditions_t list={1,&slot};rmw_time_t zero={0};
 assert(rmw_wait(NULL,&list,NULL,NULL,NULL,a.w,&zero)==RMW_RET_INVALID_ARGUMENT&&slot==b.g);rmw_reset_error();
 retire_wait(&a,1,1);retire_wait(&b,1,1);finish_context(1);finish_context(0);
}
static void allocator_reentry(void){
 init_context(0);atomic_store(&test_reentry,1);wait_args a=make_wait(0);retire_wait(&a,1,1);
 atomic_store(&test_reentry,0);assert(atomic_load(&allocator_reentries)>0&&!atomic_load(&allocator_reentry_failures));uwvm_rmw_threaded_diagnostics d;assert(uwvm_rmw_get_threaded_diagnostics(&contexts[0],&d)==RMW_RET_OK&&d.current_thread_last_entry_refusal==17);finish_context(0);
}
static void no_implicit_clock(void){
 init_context(0);wait_args a=make_wait(0);void*slot=a.g;rmw_guard_conditions_t list={1,&slot};rmw_time_t positive={0,1};
 assert(rmw_wait(NULL,&list,NULL,NULL,NULL,a.w,&positive)==RMW_RET_UNSUPPORTED&&slot==a.g);
 assert(!atomic_load(&opens));rmw_reset_error();retire_wait(&a,1,1);finish_context(0);
}
static void fini_cannot_free_borrowers(void){
 init_context(0);wait_args a=make_wait(0);pthread_t t;assert(!pthread_create(&t,NULL,waiter,&a));wait_admitted(&a);
 assert(rmw_shutdown(a.c)==RMW_RET_OK);assert(rmw_context_fini(a.c)!=RMW_RET_OK);rmw_reset_error();
 assert(!pthread_join(t,NULL));retire_wait(&a,1,1);finish_context(0);
}
int main(int argc,char**argv){
 if(argc!=2)return 2;setup();
 if(!strcmp(argv[1],"infinite-guard"))infinite_guard();
 else if(!strcmp(argv[1],"trigger-before-wait"))trigger_before_wait();
 else if(!strcmp(argv[1],"shutdown-before-join"))shutdown_wakes_before_join();
 else if(!strcmp(argv[1],"retired-guard-pin"))destroy_guard_while_pinned();
 else if(!strcmp(argv[1],"retired-waitset-pin"))destroy_set_while_pinned();
 else if(!strcmp(argv[1],"same-set-busy"))same_waitset_refused();
 else if(!strcmp(argv[1],"same-entity-busy"))same_entity_refused();
 else if(!strcmp(argv[1],"two-contexts"))two_contexts();
 else if(!strcmp(argv[1],"cross-context"))cross_context_refused();
 else if(!strcmp(argv[1],"allocator-reentry"))allocator_reentry();
 else if(!strcmp(argv[1],"no-clock"))no_implicit_clock();
 else if(!strcmp(argv[1],"fini-borrowed"))fini_cannot_free_borrowers();else return 2;
 assert(!atomic_load(&opens)&&!atomic_load(&closes));assert(uwvm_rmw_install_profile(NULL)==RMW_RET_OK);
 puts("PASS_LOCAL_PTHREAD_GUARD_CONTRACT_NOT_NATIVE_ROS");return 0;
}
