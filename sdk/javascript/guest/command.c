/** @file Custom command entry: no argument/environment CRT or Wasm start section.
 * User JavaScript main must explicitly complete with integer zero. Engine
 * startup, pending/deadlocked work, exceptions and cleanup errors exit nonzero.
 */
#include "engine.h"
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
extern void exit_process(uint32_t);
__attribute__((export_name("_start"),noreturn)) void _start(void)
{
#if UWVM_JS_UNITREE_CONTROL
    /* The optional real simulator command holds before engine allocation,
     * ESM evaluation or any application/ROS operation. Root checks the actual
     * live Runtime image and PID-bound Gateway before supplying G. */
    static uint8_t const ready[]="READY_UNITREE_MAIN\n";
    uint8_t gate=0;uint32_t held=uwvm_js_basic_write(ready,sizeof ready-1);
    if(!held)held=uwvm_js_basic_read(&gate);
    if(held||gate!='G')exit_process(held?held:22);
#endif
    uint32_t status=uwvm_js_run_main(), closed=roswasm_js_close();
    exit_process(status!=UWVM_JS_OK ? status : closed);
}
