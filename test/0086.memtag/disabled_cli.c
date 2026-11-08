// Compiled by the pinned LLVM mt-2 toolchain; no WASI imports or host grants.
typedef __SIZE_TYPE__ word;
static volatile unsigned char data[256] __attribute__((aligned(16)));
static void check(int value) { if(!value) { __builtin_trap(); } }
__attribute__((export_name("_start"))) void _start(void)
{
    void* p=(void*)(data+32);
    check(__builtin_wasm_memtag_status(0)==0);
    check(__builtin_wasm_memtag_tagbits(0)==0);
    check(__builtin_wasm_memtag_startbit(0)==sizeof(word)*8);
    check(__builtin_wasm_memtag_extract(0,p)==0);
    check(__builtin_wasm_memtag_insert(0,p,7)==p);
    check(__builtin_wasm_memtag_copy(0,p,(void*)~(word)0)==p);
    check(__builtin_wasm_memtag_sub(0,(void*)(data+48),p)==16);
    check(__builtin_wasm_memtag_load(0,(void*)~(word)0)==(void*)~(word)0);
    check(__builtin_wasm_memtag_untag(0,p)==p);
    check(__builtin_wasm_memtag_random(0,p)==p);
    check(__builtin_wasm_memtag_hint(0,p,p,9)==p);
    // Ignored tag-only effects do not enforce a range or inspect shadow state.
    __builtin_wasm_memtag_store(0,p,~(word)0);
    check(__builtin_wasm_memtag_untagstore(0,p,~(word)0)==p);
    check(__builtin_wasm_memtag_randomstore(0,p,~(word)0)==p);
    check(__builtin_wasm_memtag_hintstore(0,p,~(word)0,p,7)==p);
    for(unsigned op=0;op!=4;++op)
    {
        for(unsigned i=0;i!=256;++i) {data[i]=0x5a;}
        if(op==0) {__builtin_wasm_memtag_storez(0,p,33);}
        if(op==1) {check(__builtin_wasm_memtag_untagstorez(0,p,33)==p);}
        if(op==2) {check(__builtin_wasm_memtag_randomstorez(0,p,33)==p);}
        if(op==3) {check(__builtin_wasm_memtag_hintstorez(0,p,33,p,4)==p);}
        for(unsigned i=0;i!=256;++i) {check(data[i]==(i>=32&&i<65?0:0x5a));}
    }
}
