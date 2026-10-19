/**
 * @file relay.c
 * @brief Freestanding command relay into this Wasm Instance's real ROS imports.
 *
 * Compile explicitly for wasm32 with ROSWASM_BRIDGE_ROS_VERSION=1 or2,
 * -ffreestanding -fno-builtin -nostdlib, an exported memory and _start entry.
 * The selected Runtime supplies all ROS authorization. This command creates
 * no policy, authenticates no peer itself and receives no caller/Instance ID.
 * Its only extra imports are basic WASIp1 fd_read, fd_write and proc_exit.
 * stdin/stdout are exclusive binary pipes; Runtime audit stderr stays separate.
 * See RELAY.md for the closed protocol and the supervisor's identity duties.
 */
#if !defined(__wasm32__)
#error "The command relay requires wasm32 memory0."
#endif
#if !defined(ROSWASM_BRIDGE_ROS_VERSION)
#error "Select ROSWASM_BRIDGE_ROS_VERSION explicitly; no backend fallback."
#endif
#include "bridge.c"
#include "relay_protocol.h"

_Static_assert(sizeof(void*) == 4, "WASIp1 vectors require wasm32 pointers");
_Static_assert(ROSWASM_BRIDGE_INPUT_CAPACITY > UWVM_JS_RELAY_GUARD_BYTES, "input guard needs reserved bytes");
_Static_assert(ROSWASM_BRIDGE_OUTPUT_CAPACITY > UWVM_JS_RELAY_GUARD_BYTES, "output guard needs reserved bytes");

/** WASIp1 memory0 vector layout. Host POSIX errno/size_t layouts are not used. */
struct uwvm_js_iovec { uint32_t address, length; };
_Static_assert(sizeof(struct uwvm_js_iovec) == 8, "exact WASIp1 iovec layout");
__attribute__((import_module("wasi_snapshot_preview1"), import_name("fd_read")))
extern uint32_t uwvm_js_fd_read(uint32_t, struct uwvm_js_iovec const*, uint32_t, uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("fd_write")))
extern uint32_t uwvm_js_fd_write(uint32_t, struct uwvm_js_iovec const*, uint32_t, uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("proc_exit"), noreturn))
extern void uwvm_js_proc_exit(uint32_t);

enum { UWVM_JS_WASI_INTR = 27, UWVM_JS_OUTPUT_FILL = 0xa5, UWVM_JS_GUARD_FILL = 0x6d };
#define UWVM_JS_INPUT_BYTES (ROSWASM_BRIDGE_INPUT_CAPACITY - UWVM_JS_RELAY_GUARD_BYTES)
#define UWVM_JS_OUTPUT_BYTES (ROSWASM_BRIDGE_OUTPUT_CAPACITY - UWVM_JS_RELAY_GUARD_BYTES)
#define UWVM_JS_MAX_BODY (UWVM_JS_RELAY_MAX_ARGUMENTS * 8u + 12u + UWVM_JS_INPUT_BYTES)
static unsigned char uwvm_js_header[UWVM_JS_RELAY_HEADER_BYTES];
static unsigned char uwvm_js_body[UWVM_JS_MAX_BODY > (20u + UWVM_JS_OUTPUT_BYTES) ? UWVM_JS_MAX_BODY : (20u + UWVM_JS_OUTPUT_BYTES)];
static uint64_t uwvm_js_sequence;

/** Explicit byte arithmetic avoids host struct packing and unaligned loads. */
static uint64_t uwvm_js_get(unsigned char const* bytes, unsigned count)
{
    uint64_t value=0;
    for(unsigned i=0;i<count;++i) { value |= (uint64_t)bytes[i] << (8u*i); }
    return value;
}
static void uwvm_js_put(unsigned char* bytes, uint64_t value, unsigned count)
{
    for(unsigned i=0;i<count;++i) { bytes[i]=(unsigned char)(value>>(8u*i)); }
}
/** The build disables implicit libc calls; all copies are bounded owned bytes. */
static void uwvm_js_copy(unsigned char* to, unsigned char const* from, uint32_t count)
{
    for(uint32_t i=0;i<count;++i) { to[i]=from[i]; }
}
static void uwvm_js_fill(unsigned char* to, unsigned char byte, uint32_t count)
{
    for(uint32_t i=0;i<count;++i) { to[i]=byte; }
}
static unsigned char* uwvm_js_input(void) { return roswasm_scratch+UWVM_JS_RELAY_GUARD_BYTES; }
static unsigned char* uwvm_js_slot(void) { return roswasm_scratch+ROSWASM_BRIDGE_INPUT_CAPACITY; }
static unsigned char* uwvm_js_output(void) { return uwvm_js_slot()+8u; }
static uint64_t uwvm_js_offset(void const* pointer) { return (uint64_t)(uintptr_t)pointer; }

/** Short successful transfers advance only by their returned count. Retry at
 * most sixteen consecutive WASI EINTRs without progress. EOF, zero-progress
 * writes, EAGAIN and all other errors are terminal; no ROS request is replayed.
 */
static int uwvm_js_transfer(unsigned char* bytes, uint32_t length, int reading)
{
    uint32_t consumed=0, interruptions=0;
    while(consumed<length)
    {
        struct uwvm_js_iovec vector={(uint32_t)(uintptr_t)(bytes+consumed),length-consumed};
        uint32_t actual=0;
        uint32_t error=reading ? uwvm_js_fd_read(0,&vector,1,&actual) : uwvm_js_fd_write(1,&vector,1,&actual);
        if(error==UWVM_JS_WASI_INTR)
        {
            if(++interruptions>UWVM_JS_RELAY_MAX_INTERRUPTS) { return 0; }
            continue;
        }
        if(error || !actual || actual>length-consumed) { return 0; }
        consumed+=actual;
        interruptions=0;
    }
    return 1;
}
static void uwvm_js_make_header(unsigned kind, uint64_t sequence, unsigned index, unsigned arity, uint32_t bytes)
{
    uwvm_js_put(uwvm_js_header,UWVM_JS_RELAY_MAGIC,4);
    uwvm_js_put(uwvm_js_header+4,UWVM_JS_RELAY_VERSION,2);
    uwvm_js_header[6]=ROSWASM_BRIDGE_ROS_VERSION;
    uwvm_js_header[7]=(unsigned char)kind;
    uwvm_js_put(uwvm_js_header+8,sequence,8);
    uwvm_js_put(uwvm_js_header+16,index,2);
    uwvm_js_header[18]=(unsigned char)arity;
    uwvm_js_header[19]=0;
    uwvm_js_put(uwvm_js_header+20,bytes,4);
}
/** A fatal protocol record is distinct from a Runtime status and never success.
 * If stdout is already broken, its best-effort fatal frame may be incomplete;
 * the driver must reject that transport. No dynamic data is printed to stderr.
 */
static __attribute__((noreturn)) void uwvm_js_fail(unsigned code)
{
    uwvm_js_make_header(UWVM_JS_FATAL,uwvm_js_sequence,UWVM_JS_RELAY_NO_FUNCTION,0,4);
    uwvm_js_put(uwvm_js_body,code,4);
    if(uwvm_js_transfer(uwvm_js_header,sizeof uwvm_js_header,0)) { (void)uwvm_js_transfer(uwvm_js_body,4,0); }
    uwvm_js_proc_exit(1);
}
static void uwvm_js_emit(unsigned kind, unsigned index, unsigned arity, uint32_t bytes)
{
    uwvm_js_make_header(kind,uwvm_js_sequence,index,arity,bytes);
    if(!uwvm_js_transfer(uwvm_js_header,sizeof uwvm_js_header,0) || !uwvm_js_transfer(uwvm_js_body,bytes,0))
    { uwvm_js_fail(UWVM_JS_OUTPUT_IO); }
}
static int uwvm_js_guards(void)
{
    for(unsigned i=0;i<UWVM_JS_RELAY_GUARD_BYTES;++i)
    {
        if(roswasm_scratch[i]!=UWVM_JS_GUARD_FILL || uwvm_js_output()[UWVM_JS_OUTPUT_BYTES+i]!=UWVM_JS_GUARD_FILL)
        { return 0; }
    }
    return 1;
}

/** Dispatch invokes original ABI-shaped forwarders inside the current Wasm
 * execution frame. No function pointer, caller identity or native ROS path is
 * supplied by the pipe. Each selected import remains statically referenced.
 */
#define UWVM_JS_INVOKE_VERSION(name,a) roswasm_forward_##name()
#define UWVM_JS_INVOKE_OPEN(name,a) roswasm_forward_##name((uint32_t)a[0],a[1],a[2],a[3])
#define UWVM_JS_INVOKE_HANDLE(name,a) roswasm_forward_##name(a[0])
#define UWVM_JS_INVOKE_WRITE(name,a) roswasm_forward_##name(a[0],(uint32_t)a[1],a[2],a[3])
#define UWVM_JS_INVOKE_POLL(name,a) roswasm_forward_##name(a[0],(uint32_t)a[1],a[2],a[3],a[4])
#define UWVM_JS_INVOKE_RPC(name,a) roswasm_forward_##name(a[0],(uint32_t)a[1],a[2],a[3],a[4],a[5],a[6])
#define UWVM_JS_INVOKE_WATCH(name,a) roswasm_forward_##name(a[0],a[1],a[2])
#define UWVM_JS_INVOKE_READY(name,a) roswasm_forward_##name(a[0],(uint32_t)a[1],a[2])
#define UWVM_JS_INVOKE_SUBMIT(name,a) roswasm_forward_##name(a[0],a[1],(uint32_t)a[2],a[3],a[4],a[5])
static uint32_t uwvm_js_dispatch(unsigned index, uint64_t const* arguments)
{
    switch(index)
    {
#define UWVM_JS_CASE(index,name,form,arity,mask) case index: return UWVM_JS_INVOKE_##form(name,arguments);
        UWVM_JS_RELAY_ABI_ROWS(UWVM_JS_CASE)
#undef UWVM_JS_CASE
        default: uwvm_js_fail(UWVM_JS_BAD_BODY);
    }
}

/** Validate every transfer and typed argument before touching input or calling
 * ROS. The transport deliberately exposes only its reserved memory0 ranges;
 * it is not an arbitrary guest-memory read/write API. uint64 subtraction and
 * explicit maxima prevent narrowing/overflow before copying a payload.
 */
static uint32_t uwvm_js_bind(struct uwvm_js_relay_entry entry, uint64_t const* a, uint32_t input_bytes, uint32_t snapshot)
{
    uint64_t input=uwvm_js_offset(uwvm_js_input()),slot=uwvm_js_offset(uwvm_js_slot()),output=uwvm_js_offset(uwvm_js_output());
    for(unsigned i=0;i<entry.arity;++i)
    { if(!(entry.i64_mask&(1u<<i)) && a[i]>UINT32_MAX) { uwvm_js_fail(UWVM_JS_BAD_BODY); } }
    switch(entry.form)
    {
        case UWVM_JS_VERSION: case UWVM_JS_HANDLE:
            if(input_bytes) { uwvm_js_fail(UWVM_JS_BAD_BODY); }
            return 0;
        case UWVM_JS_WATCH:
            if(input_bytes||snapshot) {uwvm_js_fail(UWVM_JS_BAD_BODY);}return 0;
        case UWVM_JS_READY:
            if(input_bytes||a[1]!=0||a[2]!=output||snapshot!=32||UWVM_JS_OUTPUT_BYTES<32) {uwvm_js_fail(UWVM_JS_BAD_BODY);}return 32;
        case UWVM_JS_SUBMIT:
            if(a[2]!=0||a[3]!=input||a[4]!=input_bytes||a[5]!=slot||snapshot) {uwvm_js_fail(UWVM_JS_BAD_BODY);}return 0;
        case UWVM_JS_OPEN:
            if(a[0]!=0 || a[1]!=input || a[2]!=input_bytes || a[3]!=slot) { uwvm_js_fail(UWVM_JS_BAD_BODY); }
            return 0;
        case UWVM_JS_WRITE:
            if(a[1]!=0 || a[2]!=input || a[3]!=input_bytes) { uwvm_js_fail(UWVM_JS_BAD_BODY); }
            return 0;
        case UWVM_JS_POLL:
            if(input_bytes || a[1]!=0 || a[2]!=output || a[3]>UWVM_JS_OUTPUT_BYTES || a[4]!=slot) { uwvm_js_fail(UWVM_JS_BAD_BODY); }
            return (uint32_t)a[3];
        case UWVM_JS_RPC:
            if(a[1]!=0 || a[2]!=input || a[3]!=input_bytes || a[4]!=output || a[5]>UWVM_JS_OUTPUT_BYTES || a[6]!=slot)
            { uwvm_js_fail(UWVM_JS_BAD_BODY); }
            return (uint32_t)a[5];
        default: uwvm_js_fail(UWVM_JS_BAD_BODY);
    }
}

/** Command entry keeps this one admitted Instance alive until explicit shutdown.
 * It opens no alias automatically. HELLO reports compatibility/layout only;
 * the external supervisor proves actual images/PIDs/policy/session/cgroup.
 */
__attribute__((export_name("_start"), noreturn)) void _start(void)
{
    if(roswasm_forward_abi_version()!=1) { uwvm_js_fail(UWVM_JS_ABI); }
    uwvm_js_put(uwvm_js_body,1,4);
    uwvm_js_put(uwvm_js_body+4,UWVM_JS_RELAY_FUNCTION_COUNT,4);
    uwvm_js_copy(uwvm_js_body+8,uwvm_js_relay_table_digest,32);
    uwvm_js_put(uwvm_js_body+40,(uint64_t)__builtin_wasm_memory_size(0)*UINT64_C(65536),8);
    uwvm_js_put(uwvm_js_body+48,uwvm_js_offset(uwvm_js_input()),8);
    uwvm_js_put(uwvm_js_body+56,uwvm_js_offset(uwvm_js_slot()),8);
    uwvm_js_put(uwvm_js_body+64,uwvm_js_offset(uwvm_js_output()),8);
    uwvm_js_put(uwvm_js_body+72,UWVM_JS_INPUT_BYTES,4);
    uwvm_js_put(uwvm_js_body+76,UWVM_JS_OUTPUT_BYTES,4);
    uwvm_js_emit(UWVM_JS_HELLO,UWVM_JS_RELAY_NO_FUNCTION,0,UWVM_JS_RELAY_HELLO_BYTES);
    for(;;)
    {
        if(!uwvm_js_transfer(uwvm_js_header,sizeof uwvm_js_header,1)) { uwvm_js_fail(UWVM_JS_INPUT_IO); }
        if(uwvm_js_get(uwvm_js_header,4)!=UWVM_JS_RELAY_MAGIC || uwvm_js_get(uwvm_js_header+4,2)!=UWVM_JS_RELAY_VERSION ||
           uwvm_js_header[6]!=ROSWASM_BRIDGE_ROS_VERSION || uwvm_js_header[19]!=0) { uwvm_js_fail(UWVM_JS_BAD_HEADER); }
        uint64_t sequence=uwvm_js_get(uwvm_js_header+8,8);
        if(uwvm_js_sequence==UINT64_MAX || sequence!=uwvm_js_sequence+1) { uwvm_js_fail(UWVM_JS_SEQUENCE); }
        uwvm_js_sequence=sequence;
        unsigned kind=uwvm_js_header[7],index=(unsigned)uwvm_js_get(uwvm_js_header+16,2),arity=uwvm_js_header[18];
        uint32_t bytes=(uint32_t)uwvm_js_get(uwvm_js_header+20,4);
        if(kind==UWVM_JS_SHUTDOWN)
        {
            if(index!=UWVM_JS_RELAY_NO_FUNCTION || arity || bytes) { uwvm_js_fail(UWVM_JS_BAD_HEADER); }
            uwvm_js_fill(uwvm_js_body,0,20);
            uwvm_js_put(uwvm_js_body+16,1,4);
            uwvm_js_emit(UWVM_JS_RESULT,index,0,20);
            uwvm_js_proc_exit(0);
        }
        if(kind!=UWVM_JS_CALL || index>=UWVM_JS_RELAY_FUNCTION_COUNT || arity!=uwvm_js_relay_entries[index].arity ||
           bytes<8u*arity+12u || bytes>UWVM_JS_MAX_BODY) { uwvm_js_fail(UWVM_JS_BAD_HEADER); }
        if(!uwvm_js_transfer(uwvm_js_body,bytes,1)) { uwvm_js_fail(UWVM_JS_INPUT_IO); }
        uint64_t arguments[UWVM_JS_RELAY_MAX_ARGUMENTS]={0};
        for(unsigned i=0;i<arity;++i) { arguments[i]=uwvm_js_get(uwvm_js_body+8u*i,8); }
        unsigned char seed[8];uwvm_js_copy(seed,uwvm_js_body+8u*arity,8);
        uint32_t snapshot=(uint32_t)uwvm_js_get(uwvm_js_body+8u*arity+8u,4),input_bytes=bytes-8u*arity-12u;
        if(input_bytes>UWVM_JS_INPUT_BYTES || snapshot>UWVM_JS_OUTPUT_BYTES) { uwvm_js_fail(UWVM_JS_BAD_BODY); }
        struct uwvm_js_relay_entry entry=uwvm_js_relay_entries[index];
        uint32_t capacity=uwvm_js_bind(entry,arguments,input_bytes,snapshot);
        uwvm_js_fill(roswasm_scratch,UWVM_JS_GUARD_FILL,UWVM_JS_RELAY_GUARD_BYTES);
        uwvm_js_fill(uwvm_js_output()+UWVM_JS_OUTPUT_BYTES,UWVM_JS_GUARD_FILL,UWVM_JS_RELAY_GUARD_BYTES);
        uwvm_js_fill(uwvm_js_input(),0,UWVM_JS_INPUT_BYTES);
        uwvm_js_fill(uwvm_js_output(),UWVM_JS_OUTPUT_FILL,UWVM_JS_OUTPUT_BYTES);
        uwvm_js_copy(uwvm_js_slot(),seed,8);
        uwvm_js_copy(uwvm_js_input(),uwvm_js_body+8u*arity+12u,input_bytes);
        uint32_t status=uwvm_js_dispatch(index,arguments);
        if(!uwvm_js_guards()) { uwvm_js_fail(UWVM_JS_GUARD); }
        // Success and capacity-refusal slots retain their exact raw bytes.
        // Only success requires an in-range response length here. Error90 can
        // retain the seed or expose a consumed event's larger required length.
        if(!status && (entry.form==UWVM_JS_POLL || entry.form==UWVM_JS_RPC) && uwvm_js_get(uwvm_js_slot(),8)>capacity)
        { uwvm_js_fail(UWVM_JS_BAD_BODY); }
        uwvm_js_put(uwvm_js_body,status,4);
        uwvm_js_copy(uwvm_js_body+4,uwvm_js_slot(),8);
        uwvm_js_put(uwvm_js_body+12,snapshot,4);
        uwvm_js_put(uwvm_js_body+16,1,4);
        uwvm_js_copy(uwvm_js_body+20,uwvm_js_output(),snapshot);
        uwvm_js_emit(UWVM_JS_RESULT,index,arity,20u+snapshot);
    }
}
