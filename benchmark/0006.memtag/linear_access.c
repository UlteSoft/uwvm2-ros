// Matched tagged/ignored/ordinary binaries; no guest host imports.
typedef __UINT32_TYPE__ u32;
#ifndef ITERATIONS
#define ITERATIONS 268435456u
#endif
static volatile u32 words[1024] __attribute__((aligned(16)));
static volatile u32 result __attribute__((aligned(16)));
static u32 kernel(void)
{
    for(u32 i=0;i<1024;++i)words[i]=i*17u+1u;
    u32 acc=0x13579bdfu;
    for(u32 i=0;i<ITERATIONS;++i)
    {
        u32 index=(i*13u)&1023u;
        u32 value=words[index];
        acc=(acc<<5)|(acc>>27);
        acc^=value+i;
        words[index]=value+3u;
    }
    result=acc;return result;
}
#if defined(BENCH_NATIVE)
#include <stdio.h>
int main(void){printf("%u\n",kernel());return 0;}
#else
__attribute__((export_name("_start"))) void _start(void)
{
#ifndef ROUNDS
#define ROUNDS 16u
#endif
    for(u32 round=0;round<ROUNDS;++round)
    {
        u32 actual=kernel();
#if defined(EXPECTED)
        if(actual!=(u32)EXPECTED)__builtin_trap();
#else
        if(!actual)__builtin_trap();
#endif
    }
}
#endif
