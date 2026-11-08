/* CASE 0: valid write; 1: wrong payload tag; 2: wrong iovec tag;
   3: wrong result tag; 4: interface denial before invalid payload access;
   5: valid random output; 6: wrong random output tag.
   Shared declarations exercise host-buffer access, not the threads profile. */
typedef __SIZE_TYPE__ word;
typedef unsigned u32;
struct iovec { void* buffer; word length; };
#if __SIZEOF_POINTER__ == 8
#define WRITE_IMPORT "fd_write_wasm64"
#define RANDOM_IMPORT "random_get_wasm64"
#else
#define WRITE_IMPORT "fd_write"
#define RANDOM_IMPORT "random_get"
#endif
__attribute__((import_module("wasi_snapshot_preview1"),import_name(WRITE_IMPORT)))
extern u32 fd_write(u32,const struct iovec*,word,word*);
__attribute__((import_module("wasi_snapshot_preview1"),import_name(RANDOM_IMPORT)))
extern u32 random_get(void*,word);
static unsigned char data[64] __attribute__((aligned(16)));
static struct iovec vectors[2] __attribute__((aligned(16)));
static word result[2] __attribute__((aligned(16)));
static void check(int condition){if(!condition)__builtin_trap();}
__attribute__((export_name("_start")))void _start(void){
 data[0]='M';data[1]='T';data[2]='-';data[3]='W';data[4]='A';data[5]='S';data[6]='I';data[7]='\n';
 void* payload=__builtin_wasm_memtag_insert(0,data,3);
 struct iovec* table=__builtin_wasm_memtag_insert(0,vectors,5);
 word* output=__builtin_wasm_memtag_insert(0,result,7);
 vectors[0].buffer=payload;vectors[0].length=8;
 __builtin_wasm_memtag_store(0,payload,sizeof(data));
 __builtin_wasm_memtag_store(0,table,sizeof(vectors));
 __builtin_wasm_memtag_store(0,output,sizeof(result));
#if CASE == 1 || CASE == 4
 table[0].buffer=__builtin_wasm_memtag_insert(0,data,4);
#elif CASE == 2
 table=__builtin_wasm_memtag_insert(0,vectors,4);
#elif CASE == 3
 output=__builtin_wasm_memtag_insert(0,result,4);
#endif
#if CASE == 5 || CASE == 6
#if CASE == 6
 payload=__builtin_wasm_memtag_insert(0,data,4);
#endif
 check(random_get(payload,32)==0);
#else
 u32 error=fd_write(1,table,1,output);
#if CASE == 4
 check(error==76);
#else
 check(error==0);check(*output==8);
#endif
#endif
}
