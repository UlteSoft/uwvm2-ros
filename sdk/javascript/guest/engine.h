/** @file
 * @brief Private C boundary for the embedded, single-threaded Wasm JS engine.
 *
 * These are guest functions, not trusted native plugins. Build exactly one
 * bounded engine into each Wasm principal. The producer supplies every numeric
 * limit and a closed immutable source table; no filesystem loader is present.
 */
#ifndef UWVM_JS_GUEST_ENGINE_H
#define UWVM_JS_GUEST_ENGINE_H
#include <stddef.h>
#include <stdint.h>
/* External official header selected by the source lock and explicit -I path.
 * Full compiler dependency closure must retain its actual staged file pin.
 */
#include <quickjs.h>
#if !defined(__wasm32__)
#error "The embedded JavaScript guest currently requires actual wasm32"
#endif
#if !defined(UWVM_JS_BACKEND) || UWVM_JS_BACKEND < 0 || UWVM_JS_BACKEND > 2
#error "Select backend 0 (pure library), 1 (ROS1) or 2 (ROS2) explicitly"
#endif
#ifndef UWVM_JS_HEAP_BYTES
#error "The producer must explicitly select engine heap bytes"
#endif
#ifndef UWVM_JS_STACK_BYTES
#error "The producer must explicitly select engine stack bytes"
#endif
/** stack_bytes is the physical low-address Wasm C-stack reservation. QuickJS
 * subtracts its evaluation budget from an actual in-function frame address;
 * using the whole reservation would underflow uintptr_t below address zero.
 * Keep a separate nonzero half-reserve budget and check practical entry
 * headroom before refreshing QuickJS's top. Neither guard is disabled.
 */
#if UWVM_JS_STACK_BYTES < 65536 || UWVM_JS_STACK_BYTES > 4194304
#error "Select a bounded 64-KiB..4-MiB Wasm C-stack reservation"
#endif
#define UWVM_JS_EVALUATION_STACK_BYTES (UWVM_JS_STACK_BYTES / 2u)
#define UWVM_JS_STACK_ENTRY_HEADROOM_BYTES 16384u
_Static_assert(UWVM_JS_EVALUATION_STACK_BYTES > 0 &&
    UWVM_JS_EVALUATION_STACK_BYTES + UWVM_JS_STACK_ENTRY_HEADROOM_BYTES < UWVM_JS_STACK_BYTES,
    "A finite evaluation budget and entry headroom must fit inside the C-stack reservation");
#ifndef UWVM_JS_MAX_JOBS
#error "The producer must explicitly select the total job bound"
#endif
#ifndef UWVM_JS_INTERRUPT_POLLS
#error "The producer must explicitly select the engine interrupt-poll bound"
#endif
#ifndef UWVM_JS_INPUT_BYTES
#error "The producer must explicitly select input scratch bytes"
#endif
#ifndef UWVM_JS_OUTPUT_BYTES
#error "The producer must explicitly select output scratch bytes"
#endif
#ifndef UWVM_JS_PRINT_BYTES
#error "The producer must explicitly select the basic output bound"
#endif

/** Local lifecycle results. They are not Runtime ROS errno values. */
enum uwvm_js_lifecycle {
    UWVM_JS_OK = 0, UWVM_JS_PENDING = 1, UWVM_JS_EXCEPTION = 2,
    UWVM_JS_LIMIT = 3, UWVM_JS_BUSY = 4, UWVM_JS_CLOSED = 5,
    UWVM_JS_INVALID = 6
};
struct uwvm_js_source { char const *name; unsigned char const *bytes; size_t length; };
extern struct uwvm_js_source const uwvm_js_sources[];
extern size_t const uwvm_js_source_count;
extern char const uwvm_js_main_module[];
extern char const uwvm_js_bootstrap_module[];
extern JSClassID uwvm_js_uint8_class, uwvm_js_array_class;
extern JSContext *uwvm_js_context;

int uwvm_js_checked_u32(JSContext *, JSValueConst, uint32_t *);
int uwvm_js_checked_i32(JSContext *, JSValueConst, int32_t *);
int uwvm_js_checked_u64(JSContext *, JSValueConst, uint64_t *);
int uwvm_js_checked_i64(JSContext *, JSValueConst, int64_t *);
/** Wasm floating scalar values use Number only, preserving NaN/Inf/signed zero.
 * These FFI conversions deliberately do not relax finite ROS codec validation.
 */
int uwvm_js_checked_f32(JSContext *, JSValueConst, float *);
int uwvm_js_checked_f64(JSContext *, JSValueConst, double *);
int uwvm_js_read_bytes(JSContext *, JSValueConst, uint8_t const **, size_t *, JSValue *);
JSValue uwvm_js_new_bytes(JSContext *, uint8_t const *, size_t);
JSModuleDef *uwvm_js_register_utf8(JSContext *, char const *);
#if UWVM_JS_BACKEND != 0
JSModuleDef *uwvm_js_register_ros(JSContext *, char const *);
uint32_t uwvm_js_basic_write(uint8_t const *, size_t);
uint32_t uwvm_js_basic_read(uint8_t *);
#endif
/** Independent optional owned ROS2 Service bridge; absent by default. */
#ifndef UWVM_JS_RMW_SERVICE
#define UWVM_JS_RMW_SERVICE 0
#endif
#if UWVM_JS_RMW_SERVICE != 0 && UWVM_JS_RMW_SERVICE != 1
#error "Select the optional owned Service bridge explicitly as0 or1"
#endif
#if UWVM_JS_RMW_SERVICE && UWVM_JS_BACKEND != 2
#error "The owned Service bridge requires actual ROS2 backend2"
#endif
#if UWVM_JS_RMW_SERVICE
JSModuleDef *uwvm_js_register_rmw_service(JSContext *, char const *);
#endif
/** Generated optional library-import module; its stubs call real Wasm imports. */
JSModuleDef *uwvm_js_register_library_imports(JSContext *, char const *);
/** Optional simulator-only Guest-local math module. It is absent from normal
 * builds. Its explicit modular simulator selection adds only checked pure
 * scalar Wasm imports, never ROS authority. The explicit Unitree
 * command producer must compile its actual C kernel TU into this SAME Wasm. */
#ifndef UWVM_JS_UNITREE_CONTROL
#define UWVM_JS_UNITREE_CONTROL 0
#endif
#if UWVM_JS_UNITREE_CONTROL != 0 && UWVM_JS_UNITREE_CONTROL != 1
#error "The optional Unitree computation module is an explicit 0/1 selection"
#endif
#if UWVM_JS_UNITREE_CONTROL
JSModuleDef *uwvm_js_register_unitree_control(JSContext *, char const *);
#endif

uint32_t roswasm_js_init(void);
uint32_t roswasm_js_pump(uint32_t);
uint32_t roswasm_js_close(void);
uint32_t roswasm_js_status(void);
JSValue uwvm_js_call_export(char const *, int, JSValueConst *);
/** Poison an errored reactor call so no scalar fallback can restore success. */
void uwvm_js_fail(uint32_t);
uint32_t uwvm_js_run_main(void);
#endif
