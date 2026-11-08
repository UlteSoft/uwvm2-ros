typedef unsigned u32;
typedef unsigned long long u64;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_filestat_get")))
extern u32 fdstat(u32,void*);
static unsigned char buffer[64] __attribute__((aligned(16)));
__attribute__((export_name("wasi_thread_start"))) void wasi_thread_start(u32 tid,u32 arg){(void)tid;(void)arg;}
__attribute__((export_name("_start"))) void _start(void){
 for(u32 i=0;i<64;++i)buffer[i]=0xa5;
 unsigned char* valid=__builtin_wasm_memtag_insert(0,buffer,5);
 __builtin_wasm_memtag_store(0,valid,sizeof(buffer));
#if CASE==2
 unsigned char* second=__builtin_wasm_memtag_insert(0,buffer+48,6);
 __builtin_wasm_memtag_store(0,second,16);
#endif
 unsigned char* passed=__builtin_wasm_memtag_insert(0,buffer,(CASE==1||CASE==3)?6:5);
 u32 result=fdstat(3,passed);
#if CASE==3
 if(result!=76||valid[0]!=0xa5||valid[63]!=0xa5)__builtin_trap();
#else
 if(result||*(u64*)(valid+16)!=3)__builtin_trap();
#endif
}
