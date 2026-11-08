// Genuine LLVM mt-2 stack/global instrumentation, no host imports.
typedef __SIZE_TYPE__ word;
static volatile unsigned char* volatile escaped;
static volatile unsigned char global_data[64] __attribute__((aligned(16)))={13,17};
__attribute__((noinline)) static word frame(word n)
{
    volatile unsigned char bytes[48];
    escaped=bytes;
    for(word i=0;i<48;++i)bytes[i]=(unsigned char)(i+n);
    return bytes[0]+bytes[47]+global_data[0]+global_data[1];
}
__attribute__((export_name("_start"))) void _start(void)
{
    for(word i=0;i<8;++i)if(frame(i)!=77+2*i)__builtin_trap();
    global_data[1]=19;
    if(frame(2)!=83)__builtin_trap();
    word status=__builtin_wasm_memtag_status(0);
    if(status!=0 && status!=2)__builtin_trap();
}
