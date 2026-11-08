typedef unsigned u32;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_prestat_get")))
extern u32 prestat_get(u32,void*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_prestat_dir_name")))
extern u32 prestat_name(u32,void*,u32);
static unsigned char buffer[64] __attribute__((aligned(16)));
__attribute__((export_name("wasi_thread_start"))) void wasi_thread_start(u32 tid,u32 arg){(void)tid;(void)arg;}
__attribute__((export_name("_start"))) void _start(void){
 unsigned char* valid=__builtin_wasm_memtag_insert(0,buffer,5);
 __builtin_wasm_memtag_store(0,valid,sizeof(buffer));
 unsigned char* passed=__builtin_wasm_memtag_insert(0,buffer,(CASE&1)?6:5);
 u32 status;
#if CASE<2
 status=prestat_get(3,passed);
 if(status||*(u32*)valid!=0||*(u32*)(valid+4)!=8)__builtin_trap();
#else
 status=prestat_name(3,passed,8);
 if(status||valid[0]!='/'||valid[7]!='x')__builtin_trap();
#endif
}
