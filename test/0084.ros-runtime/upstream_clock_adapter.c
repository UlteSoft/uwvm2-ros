/** @file
 * @brief Narrow POSIX clock adapter for the upstream ROS time-source fixture.
 * @details Only realtime and monotonic reads are expressible. Exact deployment
 * aliases must separately grant clock.create/read; absent authority fails with
 * EACCES and is never replaced with WASI clock access. Stable grants impose no
 * rate limit. Explicit attenuation may fail a later read with EAGAIN. No guest
 * path, socket, raw clock, source update, sleeping or global fallback exists.
 * The adapter is single-threaded test glue, not a complete rcutils port.
 */
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#define IMPORT(name) __attribute__((import_module("uwvm_ros_v2"),import_name(name)))
#define RPC(symbol,name) IMPORT(name) uint32_t symbol(uint64_t,uint32_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t)
IMPORT("capability.open") uint32_t scene_capability_open(uint32_t,uint64_t,uint64_t,uint64_t);
RPC(scene_clock_create,"clock.create");
RPC(scene_clock_read,"clock.read");
#define ADDRESS(value) ((uint64_t)(uintptr_t)(value))
static uint64_t caps[2],clocks[2];
static int fail(uint32_t status)
{
    errno=status==13?EACCES:status==11?EAGAIN:status==12?ENOMEM:status==90?EOVERFLOW:EIO;
    return -1;
}
int scene_clock_gettime(clockid_t selected,struct timespec* destination)
{
    if(!destination){errno=EINVAL;return -1;}
    unsigned index;
    if(selected==CLOCK_MONOTONIC)index=0;
    else if(selected==CLOCK_REALTIME)index=1;
    else{errno=ENOTSUP;return -1;}
    uint32_t status;uint64_t used=0,output[4]={0},request=index;
    if(!caps[index])
    {
        const char* alias=index?"system":"steady";
        status=scene_capability_open(0,ADDRESS(alias),strlen(alias),ADDRESS(&caps[index]));
        if(status)return fail(status);
    }
    if(!clocks[index])
    {
        status=scene_clock_create(caps[index],0,ADDRESS(&request),8,ADDRESS(output),sizeof(output),ADDRESS(&used));
        if(status)return fail(status);
        if(used!=8||!output[0])return fail(22);
        clocks[index]=output[0];
    }
    request=clocks[index];
    status=scene_clock_read(caps[index],0,ADDRESS(&request),8,ADDRESS(output),sizeof(output),ADDRESS(&used));
    if(status)return fail(status);
    if(used!=32||output[2]!=1||output[3]!=index||output[0]>INT64_MAX)return fail(90);
    destination->tv_sec=(time_t)(output[0]/1000000000u);
    destination->tv_nsec=(long)(output[0]%1000000000u);
    return 0;
}
