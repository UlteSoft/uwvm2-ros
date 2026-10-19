/** @file Pure C Go2 kernel/CRC module inside the actual embedded-JS Wasm.
 *
 * This is Guest application code, never a native plugin or host bridge. It
 * compiles the exact SDK simulation_control.h used by native C++ and C/C++
 * Guests. JavaScript receives a fresh copied byte array, not a C pointer.
 * There are no ROS calls, capabilities, allocations outside QuickJS's bounded
 * copy, clocks, threads or hidden input here. The JS application remains the
 * real complete generated-codec subscriber and publisher.
 *
 * command(step,dt_ns) returns484 explicit little-endian bytes: CRC:u32 then
 * twenty records(mode:u32,q:f32,dq:f32,kp:f32,kd:f32,tau:f32). CRC covers the
 * separate official padded812-byte LowCmd layout, never this local envelope
 * or the generated canonical record. Header/reserved fields are fixed exactly
 * as in the matched native controller. No mathematical constants are copied.
 */
#include "../../../../javascript/guest/engine.h"
#include <roswasm_unitree/crc.h>
#include <roswasm_unitree/simulation_control.h>
#ifndef UWVM_UNITREE_DIRECT_ENABLED
#define UWVM_UNITREE_DIRECT_ENABLED 0
#endif
#ifndef UWVM_UNITREE_MODULAR_MOTION
#define UWVM_UNITREE_MODULAR_MOTION 0
#endif
#ifndef UWVM_UNITREE_MODULAR_PERCEPTION
#define UWVM_UNITREE_MODULAR_PERCEPTION 0
#endif
#if UWVM_UNITREE_MODULAR_MOTION && !UWVM_UNITREE_MODULAR_PERCEPTION
#error "This language's explicit modular recipe requires the complete three-provider graph"
#endif
#if UWVM_UNITREE_MODULAR_MOTION
#include <unitree_modular_control.h>
#endif
#if !UWVM_JS_UNITREE_CONTROL || UWVM_JS_BACKEND != 2
#error "Compile only the explicit same-Wasm ROS2 Unitree simulator recipe"
#endif
#if UWVM_UNITREE_DIRECT_ENABLED != 0 && UWVM_UNITREE_DIRECT_ENABLED != 1
#error "Select explicit denied0 or positive1 simulation behavior"
#endif
static JSValue enabled(JSContext* ctx,JSValueConst self,int argc,JSValueConst* argv) {
    (void)self;(void)argv;if(argc)return JS_ThrowTypeError(ctx,"enabled takes no arguments");
    return JS_NewUint32(ctx,UWVM_UNITREE_DIRECT_ENABLED);
}
static JSValue modular(JSContext* ctx,JSValueConst self,int argc,JSValueConst* argv) {
    (void)self;(void)argv;if(argc)return JS_ThrowTypeError(ctx,"modular takes no arguments");
    return JS_NewUint32(ctx,UWVM_UNITREE_MODULAR_MOTION);
}
static JSValue command(JSContext* ctx,JSValueConst self,int argc,JSValueConst* argv) {
    (void)self;uint32_t step,dt_ns;
    if(argc!=2&&argc!=3)return JS_ThrowTypeError(ctx,"command requires step, dt_ns and optional observed words");
    if(uwvm_js_checked_u32(ctx,argv[0],&step)<0||uwvm_js_checked_u32(ctx,argv[1],&dt_ns)<0)return JS_EXCEPTION;
    if(!UWVM_UNITREE_DIRECT_ENABLED||step<1||step>32||dt_ns<100000||dt_ns>20000000)
        return JS_ThrowRangeError(ctx,"explicit positive bounded simulator command required");
    uwvm_unitree_fixture_motor motors[20];
#if UWVM_UNITREE_MODULAR_MOTION
    uint8_t const* input=NULL;size_t size=0;JSValue root=JS_UNDEFINED;uint32_t observed[36];
    if(argc!=3)return JS_ThrowTypeError(ctx,"modular command requires actual observed words");
    if(uwvm_js_read_bytes(ctx,argv[2],&input,&size,&root)<0)return JS_EXCEPTION;
    if(size!=144){JS_FreeValue(ctx,root);return JS_ThrowRangeError(ctx,"exact observed LowState word envelope required");}
    for(unsigned i=0;i<36;++i)observed[i]=uwvm_unitree_get32(input+4*i);
    JS_FreeValue(ctx,root);
    uint32_t error=uwvm_unitree_modular_observe(step,observed);
    if(!error)error=uwvm_unitree_modular_compute(step,dt_ns,motors);
    if(error)return JS_ThrowRangeError(ctx,"checked pure Wasm composition failed");
#else
    if(uwvm_unitree_go2_fixture_step(step,(double)dt_ns/1000000000.0,motors))
        return JS_ThrowRangeError(ctx,"shared Go2 kernel rejected input");
#endif
    uwvm_unitree_go_lowcmd view={0};uint8_t packed[UWVM_UNITREE_GO_LOWCMD_BYTES],bytes[484];
    size_t written=0;uint32_t crc=0;view.head[0]=0xfe;view.head[1]=0xef;view.level_flag=0xff;
    for(unsigned i=0;i<20;++i){
        uwvm_unitree_go_motor_cmd* m=view.motor_cmd+i;
        m->mode=(uint8_t)motors[i].mode;m->q=motors[i].q;m->dq=motors[i].dq;
        m->kp=motors[i].kp;m->kd=motors[i].kd;m->tau=motors[i].tau;
        uint8_t* row=bytes+4+24*i;uwvm_unitree_put32(row,m->mode);
        uwvm_unitree_put_float(row+4,m->q);uwvm_unitree_put_float(row+8,m->dq);
        uwvm_unitree_put_float(row+12,m->kp);uwvm_unitree_put_float(row+16,m->kd);uwvm_unitree_put_float(row+20,m->tau);
    }
    if(uwvm_unitree_go_lowcmd_pack_crc(&view,packed,sizeof packed,&written,&crc)||written!=sizeof packed)
        return JS_ThrowTypeError(ctx,"shared full official CRC packing failed");
    uwvm_unitree_put32(bytes,crc);return uwvm_js_new_bytes(ctx,bytes,sizeof bytes);
}
static JSCFunctionListEntry const functions[]={JS_CFUNC_DEF("enabled",0,enabled),JS_CFUNC_DEF("modular",0,modular),JS_CFUNC_DEF("command",3,command)};
static int initialize(JSContext* ctx,JSModuleDef* module) {
    return JS_SetModuleExportList(ctx,module,functions,sizeof functions/sizeof *functions);
}
JSModuleDef* uwvm_js_register_unitree_control(JSContext* ctx,char const* name) {
    JSModuleDef* module=JS_NewCModule(ctx,name,initialize);
    if(!module||JS_AddModuleExportList(ctx,module,functions,sizeof functions/sizeof *functions)<0)return NULL;
    return module;
}
