typedef unsigned u32;
typedef unsigned long long u64;
__attribute__((import_module("wasi_snapshot_preview1"),import_name("path_open"))) extern u32 open_path(u32,u32,const char*,u32,u32,u64,u64,u32,u32*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_seek"))) extern u32 seek(u32,long long,u32,u64*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_tell"))) extern u32 tell(u32,u64*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write"))) extern u32 write_fd(u32,void*,u32,u32*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_read"))) extern u32 read_fd(u32,void*,u32,u32*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_close"))) extern u32 close_fd(u32);
__attribute__((import_module("wasi"),import_name("thread-spawn"))) extern int spawn(u32);
static const char paths[2][6]={"first","other"};
static u32 descriptors[2]={0xa5a5a5a5u,0xa5a5a5a5u},results[2];
static _Atomic(u32) ready,go,done;
static u64 position __attribute__((aligned(8)));
static u32 amount;
static char byte='R';
static struct {char* address;u32 length;} iovec={&byte,1};
static void publish(_Atomic(u32)* p){__c11_atomic_store(p,1,__ATOMIC_RELEASE);__builtin_wasm_memory_atomic_notify((int*)p,1);}
static void wait(_Atomic(u32)* p){while(!__c11_atomic_load(p,__ATOMIC_ACQUIRE))__builtin_wasm_memory_atomic_wait32((int*)p,0,-1LL);}
static void open_one(u32 index){results[index]=open_path(3,0,paths[index],5,CASE?8:1,102,0,0,&descriptors[index]);}
__attribute__((export_name("wasi_thread_start"))) void wasi_thread_start(u32 tid,u32 argument){(void)tid;(void)argument;publish(&ready);wait(&go);open_one(1);publish(&done);}
__attribute__((export_name("_start"))) void _start(void){
 if(spawn(0)<0)__builtin_trap();wait(&ready);publish(&go);open_one(0);wait(&done);
 if(!((results[0]==0&&results[1]==33)||(results[1]==0&&results[0]==33)))__builtin_trap();
 u32 winner=results[0]?1:0,loser=winner^1u;
 if(descriptors[winner]!=4||descriptors[loser]!=0xa5a5a5a5u)__builtin_trap();
 u32 fd=descriptors[winner];
 if(tell(fd,&position)||position!=0)__builtin_trap();
 if(seek(fd,4294967301LL,0,&position)||position!=4294967301ULL)__builtin_trap();
 if(write_fd(fd,&iovec,1,&amount)||amount!=1)__builtin_trap();
 if(tell(fd,&position)||position!=4294967302ULL)__builtin_trap();
 if(seek(fd,4294967301LL,0,&position)||position!=4294967301ULL)__builtin_trap();
 byte=0;if(read_fd(fd,&iovec,1,&amount)||amount!=1||byte!='R')__builtin_trap();
 if(close_fd(fd)||close_fd(fd)!=8)__builtin_trap();
}
