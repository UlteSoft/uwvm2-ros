/** @file
 * @brief Strict C-side scalar and byte validation for real embedded JS.
 *
 * JavaScript wrappers are convenience checks. This boundary independently
 * rejects coercion, fractions and narrowing before any original ROS import.
 * API signatures are from the pinned QuickJS 2026-06-04 release.
 */
#include "engine.h"

int uwvm_js_checked_u32(JSContext *ctx, JSValueConst value, uint32_t *output)
{
    double number;
    if (!JS_IsNumber(value) || JS_ToFloat64(ctx, &number, value) < 0) {
        JS_ThrowTypeError(ctx, "ABI i32 requires a Number"); return -1;
    }
    if (!(number >= 0.0 && number <= 4294967295.0)) {
        JS_ThrowRangeError(ctx, "ABI i32 range"); return -1;
    }
    uint32_t integer = (uint32_t)number;
    if ((double)integer != number) { JS_ThrowRangeError(ctx, "ABI i32 integer"); return -1; }
    *output = integer; return 0;
}
int uwvm_js_checked_i32(JSContext *ctx, JSValueConst value, int32_t *output)
{
    double number;
    if (!JS_IsNumber(value) || JS_ToFloat64(ctx, &number, value) < 0) {
        JS_ThrowTypeError(ctx, "Scalar i32 requires a Number"); return -1;
    }
    if (!(number >= -2147483648.0 && number <= 2147483647.0)) {
        JS_ThrowRangeError(ctx, "Scalar i32 range"); return -1;
    }
    int32_t integer = (int32_t)number;
    if ((double)integer != number) { JS_ThrowRangeError(ctx, "Scalar i32 integer"); return -1; }
    *output = integer; return 0;
}
int uwvm_js_checked_u64(JSContext *ctx, JSValueConst value, uint64_t *output)
{
    if (!JS_IsBigInt(ctx, value)) { JS_ThrowTypeError(ctx, "ABI i64 requires a BigInt"); return -1; }
    int64_t bits;
    if (JS_ToBigInt64(ctx, &bits, value) < 0) return -1;
    JSValue restored = JS_NewBigUint64(ctx, (uint64_t)bits);
    if (JS_IsException(restored)) return -1;
    int equal = JS_StrictEq(ctx, value, restored);
    JS_FreeValue(ctx, restored);
    if (!equal) { JS_ThrowRangeError(ctx, "ABI i64 range"); return -1; }
    *output = (uint64_t)bits; return 0;
}
int uwvm_js_checked_i64(JSContext *ctx, JSValueConst value, int64_t *output)
{
    if (!JS_IsBigInt(ctx, value)) { JS_ThrowTypeError(ctx, "Scalar i64 requires a BigInt"); return -1; }
    int64_t bits;
    if (JS_ToBigInt64(ctx, &bits, value) < 0) return -1;
    JSValue restored = JS_NewBigInt64(ctx, bits);
    if (JS_IsException(restored)) return -1;
    int equal = JS_StrictEq(ctx, value, restored);
    JS_FreeValue(ctx, restored);
    if (!equal) { JS_ThrowRangeError(ctx, "Scalar i64 range"); return -1; }
    *output = bits; return 0;
}
/** Core Wasm floating-point values are not ROS wire fields. Accept only an
 * actual ECMAScript Number, preserving Infinity, NaN and negative zero. The
 * engine may canonicalize a NaN payload; no cross-language NaN payload identity
 * is promised. ROS codecs independently retain their finite-field checks.
 */
int uwvm_js_checked_f64(JSContext *ctx, JSValueConst value, double *output)
{
    if (!JS_IsNumber(value)) {
        JS_ThrowTypeError(ctx, "Scalar f64 requires a Number"); return -1;
    }
    return JS_ToFloat64(ctx, output, value);
}
int uwvm_js_checked_f32(JSContext *ctx, JSValueConst value, float *output)
{
    double number;
    if (uwvm_js_checked_f64(ctx, value, &number) < 0) return -1;
    /* Compiled as Wasm f32.demote_f64 without fast-math: the selected IEEE
     * rounding, overflow to infinity and signed-zero behavior remain real.
     */
    *output = (float)number; return 0;
}
/** Root the genuine Uint8Array backing buffer until the caller has copied it.
 * Reject proxies, clamped/int8 arrays, detached storage and arithmetic overflow.
 * Reading named JavaScript properties is deliberately unnecessary here.
 */
int uwvm_js_read_bytes(JSContext *ctx, JSValueConst value, uint8_t const **output,
                       size_t *length, JSValue *root)
{
    if (JS_GetClassID(value) != uwvm_js_uint8_class) {
        JS_ThrowTypeError(ctx, "Genuine Uint8Array required"); return -1;
    }
    size_t offset, bytes, element, capacity;
    JSValue buffer = JS_GetTypedArrayBuffer(ctx, value, &offset, &bytes, &element);
    if (JS_IsException(buffer)) return -1;
    uint8_t *data = JS_GetArrayBuffer(ctx, &capacity, buffer);
    if ((!data && capacity) || element != 1 || offset > capacity || bytes > capacity - offset) {
        JS_FreeValue(ctx, buffer); JS_ThrowTypeError(ctx, "Invalid Uint8Array backing storage"); return -1;
    }
    *root = buffer; *length = bytes; *output = bytes ? data + offset : data;
    return 0;
}
/** Return a fresh engine-owned copy; no C scratch pointer escapes the binding. */
JSValue uwvm_js_new_bytes(JSContext *ctx, uint8_t const *data, size_t length)
{
    JSValue buffer = JS_NewArrayBufferCopy(ctx, data, length);
    if (JS_IsException(buffer)) return buffer;
    /* The pinned QuickJS 2026-06-04 C API forwards argv directly to its
     * internal constructor, which reads all three slots regardless of argc.
     * Unlike a JavaScript call, a C caller receives no automatic undefined
     * padding. Supply the complete borrowed buffer/offset/length argument
     * vector; undefined offset and length retain the entire copied buffer.
     */
    JSValue arguments[3] = { buffer, JS_UNDEFINED, JS_UNDEFINED };
    JSValue array = JS_NewTypedArray(ctx, 3, arguments, JS_TYPED_ARRAY_UINT8);
    JS_FreeValue(ctx, buffer); return array;
}
