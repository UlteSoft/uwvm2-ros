/* Qualification collector loaded by the official Intel static SDK. No
 * replacement SDK ABI: event layouts come from its official header. The test
 * launcher owns these environment values; no guest authority is involved. */
#define _POSIX_C_SOURCE 200809L
#include <jitprofiling.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
static int output = -1;
static int reject_load;
__attribute__((constructor)) static void initialize_output(void)
{
    char const* path=getenv("UWVM_TEST_JIT_EVENTS");
    if(path) { output=open(path,O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC,0600); }
    reject_load=getenv("UWVM_TEST_JIT_REJECT_LOAD")!=NULL;
}
unsigned Initialize(void) { return 1; }
static uint64_t checksum(void const* address,unsigned size)
{
    unsigned char const* bytes=address;uint64_t hash=14695981039346656037ULL;
    for(unsigned n=0;n<size;++n) { hash^=bytes[n];hash*=1099511628211ULL; }
    return hash;
}
unsigned NotifyEvent(unsigned kind,void* data)
{
    if(output<0) { return 0; }
    char line[4608];int length=0;
    if(kind==iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED_V2)
    {
        iJIT_Method_Load_V2 const* event=data;
        length=snprintf(line,sizeof(line),"LOAD %u %p %u %llu %u %s\n",event->method_id,event->method_load_address,
            event->method_size,(unsigned long long)checksum(event->method_load_address,event->method_size),
            event->line_number_size,event->method_name);
    }
    else if(kind==iJVM_EVENT_TYPE_METHOD_UNLOAD_START)
    {
        iJIT_Method_Load const* event=data;
        length=snprintf(line,sizeof(line),"UNLOAD %u %p %u %llu\n",event->method_id,event->method_load_address,
            event->method_size,(unsigned long long)checksum(event->method_load_address,event->method_size));
    }
    else { length=snprintf(line,sizeof(line),"EVENT %u\n",kind); }
    if(length<0 || (unsigned)length>=sizeof(line) || write(output,line,(unsigned)length)!=length) { _exit(95); }
    return !(reject_load && kind==iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED_V2);
}
