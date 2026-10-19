/** @file
 * @brief Bounded QuickJS interpreter, closed ESM loader and reactor lifecycle.
 *
 * The official pinned engine is compiled into this actual Wasm. No Node,
 * quickjs-libc, native module loader, environment, file or event-loop service is
 * used. JavaScript user logic and its callbacks therefore execute within this
 * instance's linear-memory and Runtime principal boundary.
 */
#include "engine.h"
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>
#define EXPORT(name) __attribute__((export_name(name)))
#ifndef UWVM_JS_MAX_MODULES
#error "Explicit module count bound required"
#endif
#ifndef UWVM_JS_MAX_SOURCE_BYTES
#error "Explicit total source byte bound required"
#endif

JSContext *uwvm_js_context;
JSClassID uwvm_js_uint8_class, uwvm_js_array_class;
static JSRuntime *runtime;
static JSModuleDef *root_module;
static JSValue namespace_value, pending_value;
static uint32_t phase, error_status, entered, interrupt_remaining, jobs_used;
enum { NEW=0, BOOT=1, MAIN=2, READY=3, FAILED=4, CLOSED=5 };

/** Count requested allocation plus an aligned owned prefix. The Wasm maximum
 * and Root RSS guard separately bound libc overhead and nonengine allocations.
 * No platform malloc_usable_size API or host allocation callback is required.
 */
union allocation { max_align_t alignment; struct { size_t bytes; } stored; };
static size_t usable(void const *pointer)
{ return pointer ? ((union allocation const *)pointer-1)->stored.bytes : 0; }
static void *allocate(JSMallocState *state,size_t bytes)
{
    if (!bytes) return NULL;
    if (bytes>SIZE_MAX-sizeof(union allocation)) { error_status=UWVM_JS_LIMIT; return NULL; }
    size_t total=bytes+sizeof(union allocation), limit=state->malloc_limit;
    if (limit>UWVM_JS_HEAP_BYTES) limit=UWVM_JS_HEAP_BYTES;
    if (state->malloc_size>limit || total>limit-state->malloc_size) { error_status=UWVM_JS_LIMIT; return NULL; }
    union allocation *value=malloc(total);
    if (!value) { error_status=UWVM_JS_LIMIT; return NULL; }
    value->stored.bytes=bytes; state->malloc_size+=total; ++state->malloc_count;
    return value+1;
}
static void release(JSMallocState *state,void *pointer)
{
    if (!pointer) return;
    union allocation *value=(union allocation *)pointer-1;
    state->malloc_size-=value->stored.bytes+sizeof *value; --state->malloc_count;
    free(value);
}
static void *resize(JSMallocState *state,void *pointer,size_t bytes)
{
    if (!pointer) return allocate(state,bytes);
    if (!bytes) { release(state,pointer); return NULL; }
    if (bytes>SIZE_MAX-sizeof(union allocation)) { error_status=UWVM_JS_LIMIT; return NULL; }
    union allocation *previous=(union allocation *)pointer-1;
    size_t old=previous->stored.bytes+sizeof *previous, total=bytes+sizeof *previous;
    size_t limit=state->malloc_limit;
    if (limit>UWVM_JS_HEAP_BYTES) limit=UWVM_JS_HEAP_BYTES;
    if (state->malloc_size<old || state->malloc_size-old>limit || total>limit-(state->malloc_size-old)) { error_status=UWVM_JS_LIMIT; return NULL; }
    union allocation *value=realloc(previous,total);
    if (!value) { error_status=UWVM_JS_LIMIT; return NULL; }
    value->stored.bytes=bytes; state->malloc_size=state->malloc_size-old+total;
    return value+1;
}
static JSMallocFunctions const allocator={allocate,release,resize,usable};

/** No clock is used. A callback poll is not one hardware instruction; the
 * owner's deadline remains necessary. Never replenish this in a raw ROS call.
 */
static int interrupt(JSRuntime *rt,void *opaque)
{
    (void)rt; (void)opaque;
    if (!interrupt_remaining) { error_status=UWVM_JS_LIMIT; return 1; }
    --interrupt_remaining; return 0;
}
void uwvm_js_fail(uint32_t status)
{ if (!error_status) error_status=status ? status : UWVM_JS_EXCEPTION; phase=FAILED; }
static void rejection(JSContext *ctx,JSValueConst promise,JSValueConst reason,
                       JS_BOOL handled,void *opaque)
{
    (void)ctx; (void)promise; (void)reason; (void)opaque;
    /* A later handler cannot erase an unobserved failure from this bounded
     * application pass. SDK Future attaches its handler before rejecting. */
    if (!handled) uwvm_js_fail(UWVM_JS_EXCEPTION);
}

static struct uwvm_js_source const *source(char const *name)
{
    for (size_t i=0;i<uwvm_js_source_count;++i)
        if (!strcmp(name,uwvm_js_sources[i].name)) return &uwvm_js_sources[i];
    return NULL;
}
static int builtin(char const *name)
{
    if (!strcmp(name,"roswasm:utf8") || !strcmp(name,"roswasm:library_imports")) return 1;
#if UWVM_JS_RMW_SERVICE
    if (!strcmp(name,"roswasm:rmw_service_native")) return 1;
#endif
#if UWVM_JS_UNITREE_CONTROL
    if (!strcmp(name,"roswasm:unitree_control")) return 1;
#endif
#if UWVM_JS_BACKEND != 0
    if (!strcmp(name,"roswasm:guest_native")) return 1;
#endif
    return 0;
}
/** Normalize only inside the immutable map. The result is a module ID, never a
 * filesystem path. Dot segments may not escape that map's root. This routine
 * does not call realpath, getenv, package resolution or any host loader.
 */
static char *normalize(JSContext *ctx,char const *base,char const *requested,void *opaque)
{
    (void)opaque;
    size_t n=strlen(requested), b=strlen(base);
    if (n>1024 || b>1024) goto refused;
    if (builtin(requested)) return js_strdup(ctx,requested);
    if (!n || requested[0]=='/' || strchr(requested,':') || strchr(requested,'\\')) goto refused;
    char joined[2050]; size_t length=0;
    if (requested[0]=='.') {
        char const *slash=strrchr(base,'/');
        if (slash) { length=(size_t)(slash-base)+1; memcpy(joined,base,length); }
    }
    memcpy(joined+length,requested,n+1); length+=n;
    char normalized[2050]; size_t used=0, starts[1025], depth=0;
    for (size_t i=0;i<length;) {
        size_t start=i;
        while (i<length && joined[i]!='/') {
            unsigned char c=(unsigned char)joined[i];
            if (!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.')) goto refused;
            ++i;
        }
        size_t size=i-start; if (i<length) ++i;
        if (!size || (size==1 && joined[start]=='.')) continue;
        if (size==2 && joined[start]=='.' && joined[start+1]=='.') {
            if (!depth) goto refused;
            used=starts[--depth]; continue;
        }
        if (depth>=1025) goto refused;
        starts[depth++]=used;
        if (used) normalized[used++]='/';
        memcpy(normalized+used,joined+start,size); used+=size;
    }
    normalized[used]=0;
    if (!source(normalized)) goto refused;
    return js_strndup(ctx,normalized,used);
refused:
    JS_ThrowReferenceError(ctx,"Module is outside the closed source map"); return NULL;
}
static int attributes(JSContext *ctx,void *opaque,JSValueConst value)
{
    (void)opaque;
    if (!JS_IsUndefined(value)) { JS_ThrowTypeError(ctx,"Import attributes are not configured"); return -1; }
    return 0;
}
static JSModuleDef *loader(JSContext *ctx,char const *name,void *opaque,JSValueConst attrs)
{
    (void)opaque;
    if (attributes(ctx,NULL,attrs)<0) return NULL;
    if (!strcmp(name,"roswasm:utf8")) return uwvm_js_register_utf8(ctx,name);
    if (!strcmp(name,"roswasm:library_imports")) return uwvm_js_register_library_imports(ctx,name);
#if UWVM_JS_RMW_SERVICE
    if (!strcmp(name,"roswasm:rmw_service_native")) return uwvm_js_register_rmw_service(ctx,name);
#endif
#if UWVM_JS_UNITREE_CONTROL
    if (!strcmp(name,"roswasm:unitree_control")) return uwvm_js_register_unitree_control(ctx,name);
#endif
#if UWVM_JS_BACKEND != 0
    if (!strcmp(name,"roswasm:guest_native")) return uwvm_js_register_ros(ctx,name);
#endif
    struct uwvm_js_source const *item=source(name);
    if (!item) { JS_ThrowReferenceError(ctx,"Module is not embedded"); return NULL; }
    JSValue compiled=JS_Eval(ctx,(char const *)item->bytes,item->length,item->name,
                             JS_EVAL_TYPE_MODULE|JS_EVAL_FLAG_COMPILE_ONLY);
    if (JS_IsException(compiled)) return NULL;
    JSModuleDef *module=JS_VALUE_GET_PTR(compiled);
    JS_FreeValue(ctx,compiled); return module;
}
static JSValue evaluate(char const *name,JSModuleDef **module)
{
    struct uwvm_js_source const *item=source(name);
    if (!item) return JS_ThrowReferenceError(uwvm_js_context,"Missing embedded entry");
    JSValue value=JS_Eval(uwvm_js_context,(char const *)item->bytes,item->length,item->name,
                          JS_EVAL_TYPE_MODULE|JS_EVAL_FLAG_COMPILE_ONLY);
    if (JS_IsException(value)) return value;
    if (module) *module=JS_VALUE_GET_PTR(value);
    return JS_EvalFunction(uwvm_js_context,value);
}
static int complete(JSValueConst value)
{
    if (JS_IsException(value)) { uwvm_js_fail(UWVM_JS_EXCEPTION); return -1; }
    int state=JS_PromiseState(uwvm_js_context,value);
    if (state==JS_PROMISE_REJECTED) { uwvm_js_fail(UWVM_JS_EXCEPTION); return -1; }
    return state==JS_PROMISE_PENDING ? 0 : 1;
}
static void advance(void)
{
    while ((phase==BOOT || phase==MAIN) && complete(pending_value)>0) {
        JS_FreeValue(uwvm_js_context,pending_value); pending_value=JS_UNDEFINED;
        if (phase==BOOT) { phase=MAIN; pending_value=evaluate(uwvm_js_main_module,&root_module); }
        else {
            namespace_value=JS_GetModuleNamespace(uwvm_js_context,root_module);
            if (JS_IsException(namespace_value)) uwvm_js_fail(UWVM_JS_EXCEPTION);
            else phase=READY;
        }
    }
}
static uint32_t pump(uint32_t maximum)
{
    advance();
    for (uint32_t count=0;count<maximum && phase!=FAILED && JS_IsJobPending(runtime);++count) {
        if (jobs_used>=UWVM_JS_MAX_JOBS) { uwvm_js_fail(UWVM_JS_LIMIT); break; }
        JSContext *job_context=NULL; ++jobs_used;
        if (JS_ExecutePendingJob(runtime,&job_context)<0) { uwvm_js_fail(UWVM_JS_EXCEPTION); break; }
        advance();
    }
    if (error_status) { phase=FAILED; return error_status; }
    return phase==READY ? UWVM_JS_OK : UWVM_JS_PENDING;
}
/** Keep QuickJS's unsigned downward-stack limit above Wasm address zero.
 * The producer reserves stack_bytes at the start of this instance's memory;
 * a function frame is already below that reservation's nominal upper end.
 * Reject an entry without practical C-call headroom instead of wrapping the
 * QuickJS subtraction or silently switching off its nonzero byte guard.
 * Actual compiler frames and recursive refusal remain Runtime test concerns.
 */
static int refresh_stack_top(void)
{
    uintptr_t frame=(uintptr_t)__builtin_frame_address(0);
    if (frame<=UWVM_JS_EVALUATION_STACK_BYTES+UWVM_JS_STACK_ENTRY_HEADROOM_BYTES) {
        uwvm_js_fail(UWVM_JS_LIMIT); return 0;
    }
    JS_UpdateStackTop(runtime); return 1;
}
static int enter(void)
{
    if (entered) return 0;
    entered=1; interrupt_remaining=UWVM_JS_INTERRUPT_POLLS;
    if (runtime && !refresh_stack_top()) { entered=0; return 0; }
    return 1;
}
EXPORT("roswasm_js_init") uint32_t roswasm_js_init(void)
{
    if (entered) return UWVM_JS_BUSY;
    if (phase==READY) return error_status;
    if (phase==FAILED) return error_status;
    if (phase==CLOSED) return UWVM_JS_CLOSED;
    if (!enter()) return error_status ? error_status : UWVM_JS_BUSY;
    if (phase==NEW) {
        size_t bytes=0;
        if (!uwvm_js_source_count || uwvm_js_source_count>UWVM_JS_MAX_MODULES) goto invalid;
        for (size_t i=0;i<uwvm_js_source_count;++i) {
            if (uwvm_js_sources[i].length>UWVM_JS_MAX_SOURCE_BYTES-bytes) goto invalid;
            bytes+=uwvm_js_sources[i].length;
            for (size_t j=0;j<i;++j) if (!strcmp(uwvm_js_sources[j].name,uwvm_js_sources[i].name)) goto invalid;
        }
        namespace_value=JS_UNDEFINED; pending_value=JS_UNDEFINED;
        runtime=JS_NewRuntime2(&allocator,NULL);
        if (!runtime) goto limited;
        JS_SetMemoryLimit(runtime,UWVM_JS_HEAP_BYTES);
        JS_SetMaxStackSize(runtime,UWVM_JS_EVALUATION_STACK_BYTES);
        if (!refresh_stack_top()) goto limited;
        JS_SetInterruptHandler(runtime,interrupt,NULL);
        JS_SetCanBlock(runtime,0);
        JS_SetHostPromiseRejectionTracker(runtime,rejection,NULL);
        uwvm_js_context=JS_NewContextRaw(runtime);
        if (!uwvm_js_context) goto limited;
        if (JS_AddIntrinsicBaseObjects(uwvm_js_context)<0 || JS_AddIntrinsicEval(uwvm_js_context)<0 ||
            JS_AddIntrinsicStringNormalize(uwvm_js_context)<0 || JS_AddIntrinsicRegExp(uwvm_js_context)<0 ||
            JS_AddIntrinsicJSON(uwvm_js_context)<0 || JS_AddIntrinsicProxy(uwvm_js_context)<0 ||
            JS_AddIntrinsicMapSet(uwvm_js_context)<0 || JS_AddIntrinsicTypedArrays(uwvm_js_context)<0 ||
            JS_AddIntrinsicPromise(uwvm_js_context)<0 || JS_AddIntrinsicWeakRef(uwvm_js_context)<0) goto limited;
        /* The pinned C constructor reads argv[0..2] without argc padding.
         * Use explicit zero length and undefined optional arguments even for
         * this empty class-identity probe; never pass a null argument vector.
         */
        JSValue array_arguments[3]={JS_NewInt32(uwvm_js_context,0),JS_UNDEFINED,JS_UNDEFINED};
        JSValue array=JS_NewTypedArray(uwvm_js_context,3,array_arguments,JS_TYPED_ARRAY_UINT8);
        if (JS_IsException(array)) goto limited;
        uwvm_js_uint8_class=JS_GetClassID(array); JS_FreeValue(uwvm_js_context,array);
        array=JS_NewArray(uwvm_js_context);
        if (JS_IsException(array)) goto limited;
        uwvm_js_array_class=JS_GetClassID(array); JS_FreeValue(uwvm_js_context,array);
        JS_SetModuleLoaderFunc2(runtime,normalize,loader,attributes,NULL);
        phase=BOOT; pending_value=evaluate(uwvm_js_bootstrap_module,NULL);
    }
    { uint32_t result=pump(UWVM_JS_MAX_JOBS); entered=0; return result; }
invalid:
    uwvm_js_fail(UWVM_JS_INVALID); entered=0; return error_status;
limited:
    uwvm_js_fail(UWVM_JS_LIMIT); entered=0; return error_status;
}
EXPORT("roswasm_js_pump") uint32_t roswasm_js_pump(uint32_t maximum)
{
    if (entered) return UWVM_JS_BUSY;
    if (phase==CLOSED || phase==NEW) return UWVM_JS_CLOSED;
    if (phase==FAILED) return error_status;
    if (!maximum || maximum>UWVM_JS_MAX_JOBS) return UWVM_JS_INVALID;
    if (!enter()) return error_status ? error_status : UWVM_JS_BUSY;
    uint32_t result=pump(maximum); entered=0; return result;
}
EXPORT("roswasm_js_status") uint32_t roswasm_js_status(void)
{ return error_status ? error_status : phase==READY ? UWVM_JS_OK : phase==CLOSED ? UWVM_JS_CLOSED : UWVM_JS_PENDING; }
JSValue uwvm_js_call_export(char const *name,int argc,JSValueConst *argv)
{
    if (phase!=READY || error_status || !enter()) {
        if (uwvm_js_context) return JS_ThrowTypeError(uwvm_js_context,"Engine is not ready or is reentrant");
        return JS_EXCEPTION;
    }
    JSValue function=JS_GetPropertyStr(uwvm_js_context,namespace_value,name), result;
    if (JS_IsException(function)) result=JS_EXCEPTION;
    else if (!JS_IsFunction(uwvm_js_context,function)) result=JS_ThrowTypeError(uwvm_js_context,"Missing configured JavaScript export");
    else result=JS_Call(uwvm_js_context,function,JS_UNDEFINED,argc,argv);
    JS_FreeValue(uwvm_js_context,function);
    if (JS_IsException(result) || error_status) uwvm_js_fail(error_status ? error_status : UWVM_JS_EXCEPTION);
    entered=0; return result;
}
uint32_t uwvm_js_run_main(void)
{
    uint32_t status=roswasm_js_init();
    if (status!=UWVM_JS_OK) return status;
    JSValue result=uwvm_js_call_export("main",0,NULL);
    if (JS_IsException(result)) return error_status;
    while (!error_status && complete(result)==0) {
        if (!JS_IsJobPending(runtime)) { status=UWVM_JS_PENDING; break; }
        status=roswasm_js_pump(UWVM_JS_MAX_JOBS);
        if (status!=UWVM_JS_OK) break;
    }
    if (!error_status && status==UWVM_JS_OK && JS_IsJobPending(runtime)) {
        status=roswasm_js_pump(UWVM_JS_MAX_JOBS);
        if (!error_status && JS_IsJobPending(runtime)) { uwvm_js_fail(UWVM_JS_LIMIT); status=UWVM_JS_LIMIT; }
    }
    if (!error_status && complete(result)>0 && status==UWVM_JS_OK) {
        /* The application result must explicitly be integer zero. Undefined,
         * rejected promises or a user error cannot accidentally become PASS. */
        JSValue final=JS_PromiseState(uwvm_js_context,result)==JS_PROMISE_FULFILLED ?
            JS_PromiseResult(uwvm_js_context,result) : JS_DupValue(uwvm_js_context,result);
        uint32_t code;
        if (uwvm_js_checked_u32(uwvm_js_context,final,&code)<0 || code) status=UWVM_JS_EXCEPTION;
        JS_FreeValue(uwvm_js_context,final);
    }
    JS_FreeValue(uwvm_js_context,result);
    return error_status ? error_status : status;
}
EXPORT("roswasm_js_close") uint32_t roswasm_js_close(void)
{
    if (entered) return UWVM_JS_BUSY;
    if (phase==CLOSED) return UWVM_JS_CLOSED;
    uint32_t previous=error_status;
    entered=1;
    if (uwvm_js_context) {
        JS_FreeValue(uwvm_js_context,namespace_value);
        JS_FreeValue(uwvm_js_context,pending_value);
        JS_FreeContext(uwvm_js_context); uwvm_js_context=NULL;
    }
    if (runtime) { JS_FreeRuntime(runtime); runtime=NULL; }
    phase=CLOSED; entered=0; return previous;
}

/** libc failure paths trap inside Wasm. They must not acquire a signal/process
 * right or silently return fabricated time/environment data.
 */
_Noreturn void abort(void) { __builtin_trap(); }
_Noreturn void __assert_fail(char const *expression,char const *file,int line,char const *function)
{ (void)expression; (void)file; (void)line; (void)function; __builtin_trap(); }
