typedef unsigned u32;
typedef unsigned long long u64;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("path_open"))) extern u32 open_path(u32,u32,const char*,u32,u32,u64,u64,u32,u32*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_seek"))) extern u32 seek(u32,long long,u32,u64*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_tell"))) extern u32 tell(u32,u64*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_close"))) extern u32 close_fd(u32);
static char path[] __attribute__((aligned(16)))="long-file-component";
static u32 descriptor[4] __attribute__((aligned(16)))={0xa5a5a5a5u,0xa5a5a5a5u,0xa5a5a5a5u,0xa5a5a5a5u};
static u64 position[2] __attribute__((aligned(16)));
__attribute__((export_name("wasi_thread_start"))) void wasi_thread_start(u32 tid,u32 argument){(void)tid;(void)argument;}
__attribute__((export_name("_start"))) void _start(void){
 char* input=__builtin_wasm_memtag_insert(0,path,3);
 u32* output=__builtin_wasm_memtag_insert(0,descriptor,4);
 u64* valid=__builtin_wasm_memtag_insert(0,position,5);
#if CASE!=1 && CASE!=9
 __builtin_wasm_memtag_store(0,input,CASE==2?16:19);
#endif
 __builtin_wasm_memtag_store(0,output,16);
 __builtin_wasm_memtag_store(0,valid,16);
#if CASE==3
 output=__builtin_wasm_memtag_insert(0,descriptor,6);
#elif CASE==4
 output=(u32*)131072u;
#endif
 u32 code=open_path(3,0,input,19,0,102,0,0,output);
#if CASE==9
 if(code!=76||*output!=0xa5a5a5a5u)__builtin_trap();
 return;
#endif
 if(code)__builtin_trap();
 u32 fd=*output;
#if CASE>=5 && CASE<=8
 if(seek(fd,3,0,valid)||*valid!=3)__builtin_trap();
#if CASE==5 || CASE==6
 u64* bad=__builtin_wasm_memtag_insert(0,position,7);
#else
 u64* bad=(u64*)131072u;
#endif
#if CASE==5 || CASE==7
 if(tell(fd,bad)||*valid!=3)__builtin_trap();
#else
 if(seek(fd,4294967301LL,0,bad)||*valid!=4294967301ULL)__builtin_trap();
#endif
#else
 if(tell(fd,valid)||*valid!=0)__builtin_trap();
 if(seek(fd,4294967301LL,0,valid)||*valid!=4294967301ULL)__builtin_trap();
#endif
 if(close_fd(fd))__builtin_trap();
}
