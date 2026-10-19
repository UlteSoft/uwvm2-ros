/* SPDX-License-Identifier: Apache-2.0
 * One process business gate shared by RMW and the real A platform Session.
 * Registry lock scope NEVER calls user allocators, providers, Host IO or wait.
 */
#define _GNU_SOURCE 1
#include <uwvm_rmw/dispatch.h>
#if !UWVM_RMW_ENABLE_THREADED_PROFILE
#error "dispatch.c belongs only to the explicit threaded profile"
#endif
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#if defined(__linux__) && !defined(__wasm__)
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#elif defined(__wasm__) && defined(__wasm_atomics__)
#if !__has_builtin(__builtin_wasm_memory_atomic_wait32) || !__has_builtin(__builtin_wasm_memory_atomic_notify)
#error "Actual compiler must provide genuine shared-memory wait32/notify"
#endif
#else
#error "No real physical notification implementation for this target"
#endif
_Static_assert(sizeof(int)==4&&sizeof(uint32_t)==4,"actual wait32 word extent required");
_Static_assert(__atomic_always_lock_free(4,0),"wake epoch must be actually lock free");
static pthread_mutex_t business=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t registry=PTHREAD_MUTEX_INITIALIZER;
static _Thread_local int owner,platform_depth;
static _Thread_local uint32_t last_refusal;
static void lock(pthread_mutex_t*m){if(pthread_mutex_lock(m))abort();}
static void unlock(pthread_mutex_t*m){if(pthread_mutex_unlock(m))abort();}
uint32_t uwvm_rmw_dispatch_enter(uwvm_rmw_dispatch_guard*g){
 if(!g)return 22;
 if(owner||platform_depth){last_refusal=17;return 17;} /* before allocator/provider recursive work */
 lock(&business);owner=1;*g=(uwvm_rmw_dispatch_guard){1,0,0};return 0;
}
uint32_t uwvm_rmw_dispatch_platform_enter(uwvm_rmw_dispatch_guard*g){
 if(!g)return 22;
 if(platform_depth){last_refusal=17;return 17;} /* do not overwrite an outer lexical guard */
 if(owner){platform_depth=1;*g=(uwvm_rmw_dispatch_guard){0,1,1};return 0;}
 lock(&business);owner=1;platform_depth=1;*g=(uwvm_rmw_dispatch_guard){1,1,0};return 0;
}
void uwvm_rmw_dispatch_leave(uwvm_rmw_dispatch_guard*g){
 if(!g)return;
 if(g->platform){if(!platform_depth||!owner)abort();platform_depth=0;}
 if(g->owned){if(!owner||platform_depth)abort();owner=0;unlock(&business);}
 *g=(uwvm_rmw_dispatch_guard){0};
}
void uwvm_rmw_dispatch_suspend(uwvm_rmw_dispatch_guard*g){
 if(!g||!g->owned||g->platform||!owner||platform_depth)abort();
 g->owned=0;owner=0;unlock(&business);
}
void uwvm_rmw_dispatch_resume(uwvm_rmw_dispatch_guard*g){
 if(!g||g->owned||g->platform||owner||platform_depth)abort();
 lock(&business);owner=1;g->owned=1;
}
int uwvm_rmw_dispatch_owned(void){return owner&&!platform_depth;}
int uwvm_rmw_dispatch_active(void){return owner;}
uint32_t uwvm_rmw_dispatch_last_refusal(void){return last_refusal;}
void uwvm_rmw_registry_lock(void){lock(&registry);}
void uwvm_rmw_registry_unlock(void){unlock(&registry);}
uint32_t uwvm_rmw_dispatch_notify(uint32_t*word){
 uint32_t old=__atomic_load_n(word,__ATOMIC_ACQUIRE);
 while(old!=UINT32_MAX&&!__atomic_compare_exchange_n(word,&old,old+1,0,__ATOMIC_RELEASE,__ATOMIC_ACQUIRE)){}
#if defined(__wasm__)
 (void)__builtin_wasm_memory_atomic_notify((int*)word,UINT32_MAX);
#else
 (void)syscall(SYS_futex,word,FUTEX_WAKE_PRIVATE,INT_MAX,NULL,NULL,0);
#endif
 return old==UINT32_MAX?90:0;
}
uint32_t uwvm_rmw_dispatch_wait(uint32_t*word,uint32_t expected,int64_t ns){
 if(ns< -1)return 22;
 if(expected==UINT32_MAX||__atomic_load_n(word,__ATOMIC_ACQUIRE)==UINT32_MAX)return 90;
#if defined(__wasm__)
 int code=__builtin_wasm_memory_atomic_wait32((int*)word,(int)expected,ns);
 return (code>=0&&code<=2)?0:5;
#else
 struct timespec relative,*timeout=NULL;
 if(ns>=0){relative.tv_sec=(time_t)(ns/INT64_C(1000000000));relative.tv_nsec=(long)(ns%INT64_C(1000000000));timeout=&relative;}
 long code=syscall(SYS_futex,word,FUTEX_WAIT_PRIVATE,expected,timeout,NULL,0);
 if(code==0||errno==EAGAIN||errno==EINTR||errno==ETIMEDOUT)return 0;
 return 5;
#endif
}
