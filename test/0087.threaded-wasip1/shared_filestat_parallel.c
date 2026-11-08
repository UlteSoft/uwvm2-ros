typedef unsigned u32;
typedef unsigned long long u64;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_filestat_get"))) extern u32 filestat(u32,void*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_fdstat_set_rights"))) extern u32 reduce(u32,u64,u64);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_renumber"))) extern u32 renumber(u32,u32);
__attribute__((import_module("wasi"),import_name("thread-spawn"))) extern int spawn(u32);
static unsigned char buffers[2][64] __attribute__((aligned(16)));
static unsigned char expected[64] __attribute__((aligned(16)));
static _Atomic(u32) ready,go,done;
static void publish(_Atomic(u32)* address){__c11_atomic_store(address,1,__ATOMIC_RELEASE);__builtin_wasm_memory_atomic_notify((int*)address,1);}
static void wait(_Atomic(u32)* address){while(!__c11_atomic_load(address,__ATOMIC_ACQUIRE))__builtin_wasm_memory_atomic_wait32((int*)address,0,-1LL);}
static void observe(unsigned char* output){
 for(u32 i=0;i<10000;++i){
  for(u32 j=0;j<64;++j)output[j]=0xa5;
  u32 code=filestat(3,output);
#if CASE==0
  if(code!=0&&code!=76)__builtin_trap();
#else
  if(code!=0&&code!=8)__builtin_trap();
#endif
  if(code==0){for(u32 j=0;j<64;++j)if(output[j]!=expected[j])__builtin_trap();}
  else {for(u32 j=0;j<64;++j)if(output[j]!=0xa5)__builtin_trap();break;}
 }
}
__attribute__((export_name("wasi_thread_start"))) void wasi_thread_start(u32 tid,u32 arg){
 (void)tid;(void)arg;unsigned char* output=__builtin_wasm_memtag_insert(0,buffers[1],6);publish(&ready);wait(&go);observe(output);publish(&done);
}
__attribute__((export_name("_start"))) void _start(void){
 unsigned char* output=__builtin_wasm_memtag_insert(0,buffers[0],5);unsigned char* child=__builtin_wasm_memtag_insert(0,buffers[1],6);__builtin_wasm_memtag_store(0,output,64);__builtin_wasm_memtag_store(0,child,64);
 if(filestat(3,expected)||*(u64*)(expected+16)!=3)__builtin_trap();
 if(spawn(0)<0)__builtin_trap();wait(&ready);publish(&go);
#if CASE==0
 if(reduce(3,0,0))__builtin_trap();
#else
 if(renumber(3,1))__builtin_trap();
#endif
 if(__builtin_wasm_memory_grow(0,1)!=(u32)2)__builtin_trap();
 observe(output);wait(&done);
#if CASE!=0
 if(filestat(1,output))__builtin_trap();
 for(u32 j=0;j<64;++j)if(output[j]!=expected[j])__builtin_trap();
#endif
}
