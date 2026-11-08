#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#ifndef HEAP_CASE
#define HEAP_CASE 0
#endif
static volatile uintptr_t escaped;
static volatile unsigned observed;
static void check(int value){if(!value)__builtin_trap();}
static void pattern(unsigned char* p,size_t n,unsigned seed){for(size_t i=0;i<n;++i)p[i]=(unsigned char)(seed+i);}
static void verify(unsigned char const* p,size_t n,unsigned seed){for(size_t i=0;i<n;++i)check(p[i]==(unsigned char)(seed+i));}
static void live(void* p,size_t n){
 check(p!=0);check(((uintptr_t)p&15)==0);check(malloc_usable_size(p)>=n);
 if(__builtin_wasm_memtag_status(0))check(__builtin_wasm_memtag_extract(0,p)!=0);
}
__attribute__((export_name("_start"))) void heap_probe(void){
#if HEAP_CASE==0
 static const size_t sizes[]={0,1,15,16,17,31,32,63,64,255,256,4096};
 for(unsigned round=0;round!=8;++round)for(unsigned i=0;i!=sizeof(sizes)/sizeof(sizes[0]);++i){
  size_t n=sizes[i];unsigned char* p=malloc(n);live(p,n);pattern(p,n,i);verify(p,n,i);free(p);
  p=calloc(n,1);live(p,n);for(size_t j=0;j<n;++j)check(p[j]==0);free(p);
 }
 unsigned char* p=malloc(33);live(p,33);pattern(p,33,9);
 p=realloc(p,200);live(p,200);verify(p,33,9);pattern(p,200,19);
 unsigned char* block=malloc(96);live(block,96);pattern(block,96,7);
 p=realloc(p,8192);live(p,8192);verify(p,200,19);pattern(p,8192,3);
 p=realloc(p,32);live(p,32);verify(p,32,3);verify(block,96,7);free(block);
 errno=0;void* failed=realloc(p,SIZE_MAX);check(!failed && errno==ENOMEM);verify(p,32,3);free(p);
 p=realloc(0,48);live(p,48);free(p);free(0);
 p=malloc(64);live(p,64);p=realloc(p,0);if(p)free(p);
 for(size_t alignment=16;alignment<=4096;alignment*=2){
  void* q=0;check(posix_memalign(&q,alignment,129)==0);live(q,129);check(((uintptr_t)q&(alignment-1))==0);
  pattern(q,129,17);verify(q,129,17);free(q);
 }
 void* unchanged=(void*)(uintptr_t)1;check(posix_memalign(&unchanged,3,17)==EINVAL && unchanged==(void*)(uintptr_t)1);
 p=aligned_alloc(64,128);live(p,128);check(((uintptr_t)p&63)==0);free(p);
 errno=0;check(calloc(SIZE_MAX,2)==0 && errno==ENOMEM);
 errno=0;check(malloc(SIZE_MAX)==0 && errno==ENOMEM);
 errno=0;check(malloc(32u*1024u*1024u)==0 && errno==ENOMEM);
 unsigned char* blocks[128];
 for(unsigned i=0;i!=128;++i){blocks[i]=malloc(16+i*3);live(blocks[i],16+i*3);pattern(blocks[i],16+i*3,i);}
 for(unsigned i=0;i!=128;i+=2){verify(blocks[i],16+i*3,i);free(blocks[i]);}
 for(unsigned i=1;i<128;i+=2){verify(blocks[i],16+i*3,i);free(blocks[i]);}
 struct slot {unsigned char* data;size_t size;unsigned seed;} slots[32];
 memset(slots,0,sizeof(slots));
 uint32_t random=0x92a6617du;
 for(unsigned iteration=0;iteration!=1024;++iteration){
  random=random*1664525u+1013904223u;
  struct slot* slot=&slots[(random>>16)&31];
  if(slot->data)verify(slot->data,slot->size,slot->seed);
  if((random&7)==0){free(slot->data);slot->data=0;slot->size=0;continue;}
  size_t next=(random>>5)&8191;
  unsigned char* q=realloc(slot->data,next);live(q,next);
  if(slot->data)verify(q,slot->size<next?slot->size:next,slot->seed);
  slot->data=q;slot->size=next;slot->seed=iteration;
  pattern(q,next,iteration);
 }
 for(unsigned i=0;i!=32;++i){if(slots[i].data)verify(slots[i].data,slots[i].size,slots[i].seed);free(slots[i].data);}
#elif HEAP_CASE==1 || HEAP_CASE==2
 unsigned char* p=malloc(32);live(p,32);pattern(p,32,7);escaped=(uintptr_t)p;free(p);
 if(HEAP_CASE==1)observed=*(volatile unsigned char*)escaped;
 else *(volatile unsigned char*)escaped=99;
#elif HEAP_CASE==3
 void* p=malloc(32);live(p,32);escaped=(uintptr_t)p;free(p);free((void*)escaped);
#elif HEAP_CASE==4 || HEAP_CASE==5 || HEAP_CASE==6
 void* p=malloc(32);live(p,32);
 uintptr_t tag=__builtin_wasm_memtag_extract(0,p);
 void* wrong=__builtin_wasm_memtag_insert(0,p,tag+1);
 if(HEAP_CASE==4)free(wrong);
 else if(HEAP_CASE==5)observed=realloc(wrong,64)!=0;
 else observed=(unsigned)malloc_usable_size(wrong);
#elif HEAP_CASE==7
 unsigned char* p=malloc(32);live(p,32);size_t bytes=malloc_usable_size(p);
 ((volatile unsigned char*)p)[bytes]=99;
#elif HEAP_CASE==8
 unsigned char* p=malloc(512);live(p,512);escaped=(uintptr_t)p;
 p=realloc(p,32);live(p,32);check((uintptr_t)p==escaped);
 observed=((volatile unsigned char*)escaped)[128];
#elif HEAP_CASE==9
 unsigned char* p=malloc(128);live(p,128);pattern(p,128,0);
 free(p+16);
#else
#error unsupported HEAP_CASE
#endif
}
