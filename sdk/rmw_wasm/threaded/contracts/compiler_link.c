/* SPDX-License-Identifier: Apache-2.0
 * Compile and LINK contract only. Root must separately execute real fixtures.
 * No facade headers/fake pthread declarations or synthetic wait functions. */
#define _GNU_SOURCE 1
#include <stdint.h>
#include <pthread.h>
#include <stdatomic.h>
#include <limits.h>
#if !defined(__GNUC__) && !defined(__clang__)
#error "Actual lexical cleanup extension is required"
#endif
static _Thread_local unsigned nesting;
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static _Atomic uint32_t word;
static void cleanup(unsigned*p){nesting-=*p;}
static void*thread_main(void*p){(void)p;unsigned n __attribute__((cleanup(cleanup)))=1;nesting+=n;return 0;}
#if defined(__wasm__)
#if !defined(__wasm_atomics__) || !__has_builtin(__builtin_wasm_memory_atomic_wait32) || !__has_builtin(__builtin_wasm_memory_atomic_notify)
#error "Real WASI shared-memory atomic wait/notify required"
#endif
static int physical(void){int result=__builtin_wasm_memory_atomic_wait32((int*)&word,0,0);return result+(int)__builtin_wasm_memory_atomic_notify((int*)&word,UINT32_MAX);}
#elif defined(__linux__)
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
static int physical(void){return (int)syscall(SYS_futex,&word,FUTEX_WAKE_PRIVATE,INT_MAX,0,0,0);}
#else
#error "Unsupported physical notification target"
#endif
int main(void){pthread_t thread;int code=pthread_mutex_lock(&mutex);code|=pthread_mutex_unlock(&mutex);
 if(pthread_create(&thread,0,thread_main,0))return 1;if(pthread_join(thread,0))return 2;
 return code+physical();}
