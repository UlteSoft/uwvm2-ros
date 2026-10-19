/** @file Optional owned ROS2 Service imports inside the actual JS Wasm.
 *
 * No JavaScript address, memory index, native pointer or caller identity enters
 * this bridge. Every operation calls the original numeric import in this same
 * Wasm. Actual Runtime grants, live owners and native incarnations remain the
 * authority. Build and binary-import admission are independently explicit.
 *
 * invoke(index, args, input, output) returns a u32. Index0 returns ABI version;
 * other operations copy output only on status0. Both byte arguments must be
 * genuine Uint8Array objects and all u64 arguments must be exact BigInt values.
 * The caller allocates its output and owner ledger BEFORE this call. There is
 * no result-object/BigInt allocation or JavaScript callback after the import.
 * A Host trap or a fatal guard violation is an unknown outcome: discard this
 * instance and never replay send/respond/commit/cancel automatically.
 */
#include "engine.h"
#include "../../include/roswasm/rmw_service_v1.h"
#include <string.h>

#if !UWVM_JS_RMW_SERVICE || UWVM_JS_BACKEND != 2
#error "Explicit optional ROS2 owned-Service build selection required"
#endif
_Static_assert(UWVM_JS_INPUT_BYTES > 0 && UWVM_JS_INPUT_BYTES <= 4193786, "bounded request/response input");
_Static_assert(UWVM_JS_OUTPUT_BYTES >= 352 && UWVM_JS_OUTPUT_BYTES <= 4193786, "bounded complete delivery output");
_Static_assert(UWVM_ROS_RMW_SERVICE_ENDPOINT_BYTES == 256 &&
    UWVM_ROS_RMW_SERVICE_MINIMUM_DELIVERY_BYTES == 352, "exact owned-Service byte ABI");

enum { GUARD = 16, FUNCTION_COUNT = 12, MAX_ARGUMENTS = 4 };
static uint8_t input_storage[GUARD + UWVM_JS_INPUT_BYTES + GUARD];
static uint8_t output_storage[GUARD + UWVM_JS_OUTPUT_BYTES + GUARD];
static int invoking;
static uint8_t *input(void) { return input_storage + GUARD; }
static uint8_t *output(void) { return output_storage + GUARD; }
static uint64_t offset(void const *p) { return (uint64_t)(uintptr_t)p; }

/** Business scalar arguments, with memory/offset/byte extents supplied by C.
 * Index7's expected_writer is u32; every other listed scalar is full u64.
 * The producer binds all12 original full Wasm signatures independently.
 */
static uint8_t const arity[FUNCTION_COUNT] = {0,2,2,2,2,3,4,3,4,4,4,2};

static int shape(uint32_t index, size_t in_size, size_t out_size)
{
    if (in_size > UWVM_JS_INPUT_BYTES || out_size > UWVM_JS_OUTPUT_BYTES) return 0;
    if (index != 4 && index != 6 && in_size != 0) return 0;
    switch (index) {
    case 1: case 2: case 3: return out_size == 256;
    case 4: return out_size == 8;
    case 7: return out_size >= 352;
    case 10: return out_size == 1;
    default: return out_size == 0;
    }
}

static uint32_t dispatch(uint32_t index, uint64_t const *a, size_t in_size, size_t out_size)
{
    switch (index) {
    case 0: return uwvm_ros2_rmw_srv_v1_abi_version();
    case 1: return uwvm_ros2_rmw_srv_v1_client_activate(a[0],a[1],0,offset(output()));
    case 2: return uwvm_ros2_rmw_srv_v1_service_activate(a[0],a[1],0,offset(output()));
    case 3: return uwvm_ros2_rmw_srv_v1_endpoint_inspect(a[0],a[1],0,offset(output()));
    case 4: return uwvm_ros2_rmw_srv_v1_client_submit(a[0],a[1],0,offset(input()),in_size,offset(output()));
    case 5: return uwvm_ros2_rmw_srv_v1_client_abandon(a[0],a[1],a[2]);
    case 6: return uwvm_ros2_rmw_srv_v1_service_respond(a[0],a[1],a[2],a[3],0,offset(input()),in_size);
    case 7: return uwvm_ros2_rmw_srv_v1_endpoint_peek(a[0],a[1],(uint32_t)a[2],0,offset(output()),out_size);
    case 8: return uwvm_ros2_rmw_srv_v1_endpoint_commit(a[0],a[1],a[2],a[3]);
    case 9: return uwvm_ros2_rmw_srv_v1_endpoint_cancel(a[0],a[1],a[2],a[3]);
    case 10: return uwvm_ros2_rmw_srv_v1_client_available(a[0],a[1],a[2],a[3],0,offset(output()));
    case 11: return uwvm_ros2_rmw_srv_v1_endpoint_retire(a[0],a[1]);
    }
    __builtin_trap();
}

static void seed_guards(size_t in_size, size_t out_size)
{
    memset(input_storage,0x6d,GUARD);
    memset(input()+in_size,0x6d,GUARD);
    memset(input()+UWVM_JS_INPUT_BYTES,0x6d,GUARD);
    memset(output_storage,0x6d,GUARD);
    memset(output()+out_size,0x6d,GUARD);
    memset(output()+UWVM_JS_OUTPUT_BYTES,0x6d,GUARD);
}

static int guards_intact(size_t in_size, size_t out_size)
{
    for (size_t i=0; i<GUARD; ++i) {
        if (input_storage[i] != 0x6d || input()[in_size+i] != 0x6d ||
            input()[UWVM_JS_INPUT_BYTES+i] != 0x6d || output_storage[i] != 0x6d ||
            output()[out_size+i] != 0x6d || output()[UWVM_JS_OUTPUT_BYTES+i] != 0x6d) return 0;
    }
    return 1;
}

static JSValue invoke(JSContext *ctx, JSValueConst self, int argc, JSValueConst *argv)
{
    (void)self;
    if (invoking) return JS_ThrowTypeError(ctx,"Reentrant owned Service call");
    if (argc != 4) return JS_ThrowTypeError(ctx,"Owned Service invoke requires four arguments");
    /* Set before any Array getter can run arbitrary JavaScript. The private
     * input/output storage cannot be overwritten by a nested invocation.
     */
    invoking = 1;
    JSValue input_root = JS_UNDEFINED, output_root = JS_UNDEFINED;
    uint32_t index, count;
    uint64_t args[MAX_ARGUMENTS] = {0};
    uint8_t const *in_bytes = NULL, *out_bytes = NULL;
    size_t in_size = 0, out_size = 0;
    if (uwvm_js_checked_u32(ctx,argv[0],&index)<0) goto failed;
    if (index >= FUNCTION_COUNT || JS_GetClassID(argv[1]) != uwvm_js_array_class) goto invalid;
    JSValue length = JS_GetPropertyStr(ctx,argv[1],"length");
    int valid_length = uwvm_js_checked_u32(ctx,length,&count);
    JS_FreeValue(ctx,length);
    if (valid_length < 0) goto failed;
    if (count != arity[index]) goto invalid;
    for (uint32_t i=0; i<count; ++i) {
        JSValue value = JS_GetPropertyUint32(ctx,argv[1],i);
        int valid;
        if (index == 7 && i == 2) {
            uint32_t small = 0;
            valid = uwvm_js_checked_u32(ctx,value,&small); args[i] = small;
        } else valid = uwvm_js_checked_u64(ctx,value,&args[i]);
        JS_FreeValue(ctx,value);
        if (valid < 0) goto failed;
    }
    /* No property getters or other JS execution occur after these genuine
     * buffer extractions. Retain both backing objects through call and copy.
     * Input and output may alias: copy input BEFORE initializing output scratch.
     */
    if (uwvm_js_read_bytes(ctx,argv[2],&in_bytes,&in_size,&input_root)<0 ||
        uwvm_js_read_bytes(ctx,argv[3],&out_bytes,&out_size,&output_root)<0) goto failed;
    if (!shape(index,in_size,out_size)) goto invalid;
    /* Bound setup work by actual bytes plus fixed guards, rather than clearing
     * both maximum-capacity arrays for every ACK/retire/idle operation.
     */
    seed_guards(in_size,out_size);
    if (in_size) memcpy(input(),in_bytes,in_size);
    /* Peek writes only its complete declared record prefix. Never expose an
     * earlier call's tail when returning the caller's full selected capacity.
     * Facades must validate UMRC record_bytes and ignore this initialized tail.
     */
    if (out_size) memset(output(),0xa5,out_size);
    uint32_t status = dispatch(index,args,in_size,out_size);
    /* A violation is fatal after a possible native effect. Do not allocate a
     * JS error/result or pretend a known non-attempted refusal at this point.
     */
    if (!guards_intact(in_size,out_size)) __builtin_trap();
    if (status == 0 && out_size) memcpy((uint8_t *)out_bytes,output(),out_size);
    JS_FreeValue(ctx,output_root); JS_FreeValue(ctx,input_root);
    invoking = 0;
    /* The pinned QuickJS API returns a Number scalar here, with no result
     * object, BigInt allocation, coercion, property lookup or user callback.
     */
    return JS_NewUint32(ctx,status);
invalid:
    JS_ThrowTypeError(ctx,"Owned Service argument or byte-buffer shape");
failed:
    JS_FreeValue(ctx,output_root); JS_FreeValue(ctx,input_root);
    invoking = 0;
    return JS_EXCEPTION;
}

static JSCFunctionListEntry const functions[] = {JS_CFUNC_DEF("invoke",4,invoke)};
static int initialize(JSContext *ctx, JSModuleDef *module)
{ return JS_SetModuleExportList(ctx,module,functions,sizeof functions/sizeof *functions); }
JSModuleDef *uwvm_js_register_rmw_service(JSContext *ctx, char const *name)
{
    JSModuleDef *module = JS_NewCModule(ctx,name,initialize);
    if (!module || JS_AddModuleExportList(ctx,module,functions,sizeof functions/sizeof *functions)<0) return NULL;
    return module;
}
