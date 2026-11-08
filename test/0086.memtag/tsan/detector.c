#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
static pthread_barrier_t start;
static volatile int racy;
static _Atomic int safe;
static int controlled;
static void* worker(void* unused){(void)unused;pthread_barrier_wait(&start);for(int i=0;i<10000;++i){if(controlled)++racy;else atomic_fetch_add_explicit(&safe,1,memory_order_relaxed);}return 0;}
int main(int argc,char** argv){struct rlimit core={0,0};setrlimit(RLIMIT_CORE,&core);controlled=argc>1&&!strcmp(argv[1],"race");pthread_barrier_init(&start,0,2);pthread_t a,b;pthread_create(&a,0,worker,0);pthread_create(&b,0,worker,0);pthread_join(a,0);pthread_join(b,0);pthread_barrier_destroy(&start);printf("count=%d\n",controlled?racy:atomic_load(&safe));return controlled?0:atomic_load(&safe)!=20000;}
