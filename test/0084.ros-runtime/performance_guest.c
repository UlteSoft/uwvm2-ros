/** @file
 * @brief Real Wasm32 clock and guard/wait benchmark through uwvm_ros_v2.
 * @details Each logical iteration matches performance.cc: one clock sample,
 * one trigger/observe/consume cycle, or both. The guest has no libc, filesystem,
 * network or implicit host authority. Independent WASIp1 monotonic-clock and
 * stdout.write grants are used only to time batches and report after timing.
 * Measured batch times include the two timer-boundary costs; calibration is
 * reported separately and is never subtracted. Startup, LLVM compilation,
 * configuration and warmup are outside the reported steady loop interval.
 */
typedef unsigned int u32;
typedef unsigned long long u64;
typedef __UINTPTR_TYPE__ uptr;

#ifndef BENCH_WORKLOAD
#define BENCH_WORKLOAD 2
#endif
#ifndef BENCH_ITERATIONS
#define BENCH_ITERATIONS 20000
#endif
#ifndef BENCH_BATCHES
#define BENCH_BATCHES 101
#endif
_Static_assert(BENCH_WORKLOAD>=0&&BENCH_WORKLOAD<=2,"workload is clock, guard or mixed");
_Static_assert(BENCH_ITERATIONS>0&&BENCH_ITERATIONS<=1000000,"bounded iteration count");
_Static_assert(BENCH_BATCHES>=3&&BENCH_BATCHES<=501,"bounded batches");
#define P(value) ((u64)(uptr)(value))
#define IMPORT(name) __attribute__((import_module("uwvm_ros_v2"),import_name(name)))
#define RPC(symbol,name) IMPORT(name) u32 symbol(u64,u32,u64,u64,u64,u64,u64)
IMPORT("abi") u32 abi(void);
IMPORT("capability.open") u32 capability_open(u32,u64,u64,u64);
RPC(clock_create,"clock.create");
RPC(clock_read,"clock.read");
RPC(guard_create,"guard.create");
RPC(guard_trigger,"guard.trigger");
RPC(guard_take,"guard.take");
RPC(wait_create,"wait.create");
RPC(wait_add,"wait.add");
RPC(wait_poll,"wait.poll");
__attribute__((import_module("wasi_snapshot_preview1"),import_name("clock_time_get")))
u32 clock_time_get(u32,u64,u64*);
struct iovec {char const* data;u32 bytes;};
__attribute__((import_module("wasi_snapshot_preview1"),import_name("fd_write")))
u32 fd_write(u32,struct iovec const*,u32,u32*);

static u64 output[4],used,clock_cap,guard_cap,wait_cap,clock_id,guard_id,wait_id;
static u64 clock_request[1],guard_request[1],wait_request[2];
static u64 samples[BENCH_BATCHES],calibration[BENCH_BATCHES],checksum;
static char report[2048];
static u32 report_size;

static void check(int condition){if(!condition)__builtin_trap();}
static u64 timestamp(void)
{
    u64 value=0;check(clock_time_get(1,0,&value)==0);return value;
}
#define CALL(fn,cap,data,bytes) (check(fn(cap,0,P(data),bytes,P(output),sizeof(output),P(&used))==0),output[0])

__attribute__((noinline)) static u64 clock_step(void)
{
    u64 value=CALL(clock_read,clock_cap,clock_request,8);
    check(used==32&&output[2]==1);return value;
}
__attribute__((noinline)) static u64 guard_step(void)
{
    (void)CALL(guard_trigger,guard_cap,guard_request,8);
    check(CALL(wait_poll,wait_cap,wait_request,16)==guard_id&&used==32&&output[2]==1);
    u64 count=CALL(guard_take,guard_cap,guard_request,8);check(count==1&&used==8);return count;
}
static void step(void)
{
    if(BENCH_WORKLOAD!=1)checksum^=clock_step();
    if(BENCH_WORKLOAD!=0)checksum+=guard_step();
}
static void sort(u64* data)
{
    for(u32 i=1;i<BENCH_BATCHES;++i)
    {
        u64 value=data[i];u32 j=i;
        while(j&&data[j-1]>value){data[j]=data[j-1];--j;}
        data[j]=value;
    }
}
static void text(char const* value)
{
    while(*value){check(report_size<sizeof(report));report[report_size++]=*value++;}
}
static void number(u64 value)
{
    char digits[20];u32 count=0;
    do{digits[count++]=(char)('0'+value%10);value/=10;}while(value);
    while(count){check(report_size<sizeof(report));report[report_size++]=digits[--count];}
}

void _start(void)
{
    check(abi()==2);
    if(BENCH_WORKLOAD!=1)
    {
        check(capability_open(0,P("clock"),5,P(&clock_cap))==0);
        clock_id=CALL(clock_create,clock_cap,clock_request,8);check(used==8);
        clock_request[0]=clock_id;
    }
    if(BENCH_WORKLOAD!=0)
    {
        check(capability_open(0,P("guard"),5,P(&guard_cap))==0);
        check(capability_open(0,P("wait"),4,P(&wait_cap))==0);
        guard_id=CALL(guard_create,guard_cap,0,0);check(used==8);
        wait_id=CALL(wait_create,wait_cap,0,0);check(used==8);
        u64 attach[5]={wait_id,guard_cap,guard_id,1,1};
        (void)CALL(wait_add,wait_cap,attach,sizeof(attach));
        guard_request[0]=guard_id;wait_request[0]=wait_id;
    }
    for(u32 i=0;i<1000;++i)step();
    for(u32 batch=0;batch<BENCH_BATCHES;++batch)
    {
        u64 begin=timestamp(),end=timestamp();check(end>=begin);calibration[batch]=end-begin;
    }
    u64 started=timestamp();
    for(u32 batch=0;batch<BENCH_BATCHES;++batch)
    {
        u64 begin=timestamp();
        for(u32 i=0;i<BENCH_ITERATIONS;++i)step();
        u64 end=timestamp();check(end>=begin);samples[batch]=end-begin;
    }
    u64 end=timestamp();check(end>=started);u64 elapsed=end-started;
    sort(samples);sort(calibration);
    text("{\"provider\":\"scene-wasm-aot\",\"workload\":\"");
    text(BENCH_WORKLOAD==0?"clock":BENCH_WORKLOAD==1?"guard":"mixed");
    text("\",\"iterations_per_batch\":");number(BENCH_ITERATIONS);
    text(",\"batches\":");number(BENCH_BATCHES);
    text(",\"logical_operations\":");number((u64)BENCH_ITERATIONS*BENCH_BATCHES);
    text(",\"elapsed_ns\":");number(elapsed);
    text(",\"batch_ns_p50\":");number(samples[(BENCH_BATCHES-1)*50/100]);
    text(",\"batch_ns_p95\":");number(samples[(BENCH_BATCHES-1)*95/100]);
    text(",\"batch_ns_p99\":");number(samples[(BENCH_BATCHES-1)*99/100]);
    text(",\"timer_pair_ns_p50\":");number(calibration[(BENCH_BATCHES-1)*50/100]);
    text(",\"timer_pair_ns_p99\":");number(calibration[(BENCH_BATCHES-1)*99/100]);
    text(",\"checksum\":");number(checksum);
    text(",\"wasm_executed\":true}\n");
    struct iovec iov={report,report_size};u32 written=0;
    check(fd_write(1,&iov,1,&written)==0&&written==report_size);
}
