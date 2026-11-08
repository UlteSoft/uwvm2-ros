typedef unsigned u32;
typedef unsigned long long u64;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_fdstat_get")))
extern u32 fdstat(u32,void*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_fdstat_set_rights")))
extern u32 reduce(u32,u64,u64);
__attribute__((import_module("wasi"),import_name("thread-spawn")))
extern int spawn(u32);
static unsigned char buffers[2][32] __attribute__((aligned(16)));
static _Atomic(u32) ready,go,done;
static void publish(_Atomic(u32)* address){
 __c11_atomic_store(address,1,__ATOMIC_RELEASE);
 __builtin_wasm_memory_atomic_notify((int*)address,1);
}
static void wait(_Atomic(u32)* address){
 while(!__c11_atomic_load(address,__ATOMIC_ACQUIRE))
  __builtin_wasm_memory_atomic_wait32((int*)address,0,-1LL);
}
static void observe(unsigned char* output){
 for(u32 i=0;i<10000;++i){
  if(fdstat(3,output))__builtin_trap();
  u64 base=*(u64*)(output+8),inheriting=*(u64*)(output+16);
  if(*(u32*)output!=3||*(u32*)(output+4)!=0||
     !((base==9216&&inheriting==66)||(base==1024&&inheriting==2)))__builtin_trap();
 }
}
__attribute__((export_name("wasi_thread_start"))) void wasi_thread_start(u32 tid,u32 arg){
 (void)tid;(void)arg;
 unsigned char* output=__builtin_wasm_memtag_insert(0,buffers[1],6);
 publish(&ready);wait(&go);observe(output);publish(&done);
}
__attribute__((export_name("_start"))) void _start(void){
 unsigned char* output=__builtin_wasm_memtag_insert(0,buffers[0],5);
 unsigned char* child=__builtin_wasm_memtag_insert(0,buffers[1],6);
 __builtin_wasm_memtag_store(0,output,32);
 __builtin_wasm_memtag_store(0,child,32);
 if(spawn(0)<0)__builtin_trap();
 wait(&ready);publish(&go);
 if(reduce(3,1024,2))__builtin_trap();
 observe(output);wait(&done);
}
