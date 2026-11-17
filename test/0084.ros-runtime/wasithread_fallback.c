/* The same pthread Wasm explicitly supports a refused creation. The VM does
 * not and cannot silently serialize arbitrary guest pthread entry points. */
#include <pthread.h>
#include <stdint.h>
static uint32_t result;
static void* work(void* opaque)
{
    uint32_t n=(uint32_t)(uintptr_t)opaque;
    uint32_t total=0;
    for(uint32_t i=0;i<n;++i)total+=i*i;
    result=total;
    return opaque;
}
int main(void)
{
    pthread_t child;
    void* input=(void*)(uintptr_t)31;
    int refused=pthread_create(&child,0,work,input);
    if(refused)work(input);
    else
    {
        void* returned=0;
        if(pthread_join(child,&returned)||returned!=input)return 90;
    }
    if(result!=9455)return 91;
    return refused?61:62;
}
