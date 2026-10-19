/** @file
 * @brief Complete copied ROS ABI binding inside the application's actual Wasm.
 *
 * No native ROS implementation, identity parameter, IPC connection or permission
 * inference exists here. Every dispatch enters an original selected import in
 * the same Wasm instance. Header/table declarations are shared with the existing
 * checked relay; backend selection and all scratch capacities are explicit.
 */
#include "engine.h"
#include <string.h>
#if UWVM_JS_BACKEND == 1
#include "../../ros1/abi.h"
#define UWVM_JS_CALL(name) uwvm_ros1_##name
#elif UWVM_JS_BACKEND == 2
#include "../../ros2/abi.h"
#define UWVM_JS_CALL(name) uwvm_ros2_##name
#else
#error "Pure libraries must not compile the ROS binding"
#endif
#define ROSWASM_BRIDGE_ROS_VERSION UWVM_JS_BACKEND
#include "../relay_protocol.h"

_Static_assert(UWVM_JS_INPUT_BYTES > 0 && UWVM_JS_INPUT_BYTES <= 4193786, "bounded input");
_Static_assert(UWVM_JS_OUTPUT_BYTES >= 32 && UWVM_JS_OUTPUT_BYTES <= 4193786, "bounded output");
enum { GUARD = 16 };
static uint8_t scratch[GUARD + UWVM_JS_INPUT_BYTES + 8 + UWVM_JS_OUTPUT_BYTES + GUARD];
static int invoking;
static uint8_t *input(void) { return scratch + GUARD; }
static uint8_t *slot(void) { return input() + UWVM_JS_INPUT_BYTES; }
static uint8_t *output(void) { return slot() + 8; }
static uint64_t offset(void const *p) { return (uint64_t)(uintptr_t)p; }
static uint64_t get_slot(void) { uint64_t value = 0; for (unsigned i=0;i<8;++i) value |= (uint64_t)slot()[i] << (8*i); return value; }
static void seed_slot(uint64_t value) { for (unsigned i=0;i<8;++i) slot()[i] = (uint8_t)(value >> (8*i)); }
static int set(JSContext *ctx, JSValue object, char const *name, JSValue value)
{ return JS_IsException(value) ? -1 : JS_SetPropertyStr(ctx, object, name, value); }

static JSValue describe(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self; (void)argv;
    if (argc) return JS_ThrowTypeError(ctx, "describe takes no arguments");
    JSValue value = JS_NewObjectProto(ctx,JS_NULL);
    if (JS_IsException(value)) return value;
#define FIELD(name, expression) if (set(ctx, value, name, expression) < 0) goto failed
    FIELD("backend", JS_NewUint32(ctx, UWVM_JS_BACKEND));
    FIELD("abiVersion", JS_NewUint32(ctx, 1));
    FIELD("functionCount", JS_NewUint32(ctx, UWVM_JS_RELAY_FUNCTION_COUNT));
    FIELD("input", JS_NewBigUint64(ctx, offset(input())));
    FIELD("slot", JS_NewBigUint64(ctx, offset(slot())));
    FIELD("output", JS_NewBigUint64(ctx, offset(output())));
    FIELD("inputCapacity", JS_NewUint32(ctx, UWVM_JS_INPUT_BYTES));
    FIELD("outputCapacity", JS_NewUint32(ctx, UWVM_JS_OUTPUT_BYTES));
#undef FIELD
    return value;
failed:
    JS_FreeValue(ctx, value); return JS_EXCEPTION;
}

#define INVOKE_VERSION(name,a) UWVM_JS_CALL(name)()
#define INVOKE_OPEN(name,a) UWVM_JS_CALL(name)((uint32_t)a[0],a[1],a[2],a[3])
#define INVOKE_HANDLE(name,a) UWVM_JS_CALL(name)(a[0])
#define INVOKE_WRITE(name,a) UWVM_JS_CALL(name)(a[0],(uint32_t)a[1],a[2],a[3])
#define INVOKE_POLL(name,a) UWVM_JS_CALL(name)(a[0],(uint32_t)a[1],a[2],a[3],a[4])
#define INVOKE_RPC(name,a) UWVM_JS_CALL(name)(a[0],(uint32_t)a[1],a[2],a[3],a[4],a[5],a[6])
#define INVOKE_WATCH(name,a) UWVM_JS_CALL(name)(a[0],a[1],a[2])
#define INVOKE_READY(name,a) UWVM_JS_CALL(name)(a[0],(uint32_t)a[1],a[2])
#define INVOKE_SUBMIT(name,a) UWVM_JS_CALL(name)(a[0],a[1],(uint32_t)a[2],a[3],a[4],a[5])
static uint32_t dispatch(uint32_t index, uint64_t const *a)
{
    switch (index) {
#define ROW(index,name,form,arity,mask) case index: return INVOKE_##form(name,a);
        UWVM_JS_RELAY_ABI_ROWS(ROW)
#undef ROW
    }
    __builtin_trap();
}

/** Only C-owned exact memory0 ranges may be selected through copied JS calls.
 * This validates transport shape, not ROS policy. The actual import remains
 * responsible for bounds, handles, grants, quotas and native authorization.
 */
static int shape(struct uwvm_js_relay_entry entry, uint64_t const *a,
                  size_t bytes, uint32_t snapshot)
{
    uint64_t in=offset(input()), out=offset(output()), length=offset(slot());
    switch (entry.form) {
    case UWVM_JS_VERSION: case UWVM_JS_HANDLE: case UWVM_JS_WATCH:
        return bytes == 0 && snapshot == 0;
    case UWVM_JS_OPEN:
        return a[0]==0 && a[1]==in && a[2]==bytes && a[3]==length;
    case UWVM_JS_WRITE:
        return a[1]==0 && a[2]==in && a[3]==bytes && snapshot==0;
    case UWVM_JS_POLL:
        return bytes==0 && a[1]==0 && a[2]==out && a[3]==snapshot && a[4]==length;
    case UWVM_JS_RPC:
        return a[1]==0 && a[2]==in && a[3]==bytes && a[4]==out && a[5]==snapshot && a[6]==length;
    case UWVM_JS_READY:
        return bytes==0 && a[1]==0 && a[2]==out && snapshot==32;
    case UWVM_JS_SUBMIT:
        return a[2]==0 && a[3]==in && a[4]==bytes && a[5]==length && snapshot==0;
    }
    return 0;
}

static JSValue invoke(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self;
    if (invoking) return JS_ThrowTypeError(ctx, "Reentrant copied ROS call");
    if (argc != 5) return JS_ThrowTypeError(ctx, "invoke requires five arguments");
    invoking = 1;
    JSValue buffer = JS_UNDEFINED, value = JS_UNDEFINED;
    uint32_t index, snapshot, count;
    uint64_t seed, args[UWVM_JS_RELAY_MAX_ARGUMENTS] = {0};
    uint8_t const *bytes; size_t size;
    if (uwvm_js_checked_u32(ctx,argv[0],&index)<0 || index>=UWVM_JS_RELAY_FUNCTION_COUNT ||
        uwvm_js_checked_u64(ctx,argv[3],&seed)<0 || uwvm_js_checked_u32(ctx,argv[4],&snapshot)<0) goto invalid;
    if (snapshot>UWVM_JS_OUTPUT_BYTES || JS_GetClassID(argv[1])!=uwvm_js_array_class) goto invalid;
    JSValue length=JS_GetPropertyStr(ctx,argv[1],"length");
    int length_ok=uwvm_js_checked_u32(ctx,length,&count);
    JS_FreeValue(ctx,length);
    struct uwvm_js_relay_entry entry=uwvm_js_relay_entries[index];
    if (length_ok<0) goto failed;
    if (count!=entry.arity) goto invalid;
    for (uint32_t i=0;i<count;++i) {
        JSValue argument=JS_GetPropertyUint32(ctx,argv[1],i);
        int good;
        if (entry.i64_mask & (1u<<i)) good=uwvm_js_checked_u64(ctx,argument,&args[i]);
        else { uint32_t small; good=uwvm_js_checked_u32(ctx,argument,&small); args[i]=good<0?0:small; }
        JS_FreeValue(ctx,argument);
        if (good<0) goto failed;
    }
    if (uwvm_js_read_bytes(ctx,argv[2],&bytes,&size,&buffer)<0) goto failed;
    if (size>UWVM_JS_INPUT_BYTES || !shape(entry,args,size,snapshot)) goto invalid;
    memset(scratch,0x6d,GUARD);
    memset(output()+UWVM_JS_OUTPUT_BYTES,0x6d,GUARD);
    if (size) memcpy(input(),bytes,size);
    seed_slot(seed); memset(output(),0xa5,snapshot);
    uint32_t status=dispatch(index,args);
    for (unsigned i=0;i<GUARD;++i)
        if (scratch[i]!=0x6d || output()[UWVM_JS_OUTPUT_BYTES+i]!=0x6d) {
            uwvm_js_fail(UWVM_JS_INVALID); goto invalid;
        }
    value=JS_NewObjectProto(ctx,JS_NULL);
    if (JS_IsException(value)) goto failed;
    if (set(ctx,value,"status",JS_NewUint32(ctx,status))<0 ||
        set(ctx,value,"slot",JS_NewBigUint64(ctx,get_slot()))<0 ||
        set(ctx,value,"snapshot",uwvm_js_new_bytes(ctx,output(),snapshot))<0) goto failed;
    JS_FreeValue(ctx,buffer); invoking=0; return value;
invalid:
    JS_ThrowTypeError(ctx,"Invalid copied ABI form or range");
failed:
    JS_FreeValue(ctx,value); JS_FreeValue(ctx,buffer); invoking=0; return JS_EXCEPTION;
}

static JSValue write_output(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self;
    if (argc!=1) return JS_ThrowTypeError(ctx,"write requires one Uint8Array");
    uint8_t const *bytes; size_t length; JSValue root;
    if (uwvm_js_read_bytes(ctx,argv[0],&bytes,&length,&root)<0) return JS_EXCEPTION;
    if (length>UWVM_JS_PRINT_BYTES) { JS_FreeValue(ctx,root); return JS_ThrowRangeError(ctx,"Basic output bound"); }
    uint32_t status=uwvm_js_basic_write(bytes,length);
    JS_FreeValue(ctx,root); return JS_NewUint32(ctx,status);
}
static JSValue read_input(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self; (void)argv;
    if (argc) return JS_ThrowTypeError(ctx,"read takes no arguments");
    uint8_t byte;
    if (uwvm_js_basic_read(&byte)) return JS_ThrowTypeError(ctx,"Basic input gate failed");
    return JS_NewUint32(ctx,byte);
}
static JSCFunctionListEntry const exports[] = {
    JS_CFUNC_DEF("describe",0,describe), JS_CFUNC_DEF("invoke",5,invoke),
    JS_CFUNC_DEF("write",1,write_output), JS_CFUNC_DEF("read",0,read_input)
};
static int initialize(JSContext *ctx, JSModuleDef *module)
{ return JS_SetModuleExportList(ctx,module,exports,sizeof exports/sizeof *exports); }
JSModuleDef *uwvm_js_register_ros(JSContext *ctx, char const *name)
{
    JSModuleDef *module=JS_NewCModule(ctx,name,initialize);
    if (!module || JS_AddModuleExportList(ctx,module,exports,sizeof exports/sizeof *exports)<0) return NULL;
    return module;
}
