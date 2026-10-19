/** @file Explicit standalone C/C++ Go2 entry inside the same Wasm principal.
 *
 * Compile the selected C or C++ application with
 * -Dmain=uwvm_unitree_direct_main. Compile this TU separately without that
 * rename and with UWVM_UNITREE_DIRECT_C_MAIN=1 for C,0 for C++. This preserves
 * each language's real linkage and avoids a WASI CRT that would inspect argv
 * or the environment. The actual binary must have no Wasm start section and
 * one local void _start export. Root calls that exact local-defined index.
 *
 * The linker-generated constructor function runs once before the actual main.
 * proc_exit receives the application's original checked status. No ROS API,
 * native bridge, new authority or success stub is supplied by this entry.
 */
#include <stdint.h>
#include "basic_io.h"
#if !defined(__wasm32__) || !defined(UWVM_UNITREE_DIRECT_C_MAIN) || \
    (UWVM_UNITREE_DIRECT_C_MAIN != 0 && UWVM_UNITREE_DIRECT_C_MAIN != 1)
#error "Select one real wasm32 C/C++ standalone entry recipe"
#endif
#if UWVM_UNITREE_DIRECT_C_MAIN
extern "C" int uwvm_unitree_direct_main(void);
#else
extern int uwvm_unitree_direct_main(void);
#endif
extern "C" void __wasm_call_ctors(void);
extern "C" __attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
void uwvm_unitree_direct_exit(uint32_t);
extern "C" __attribute__((export_name("_start"),noreturn)) void _start(void) {
    /* Hold before language constructors or user main so Root can prove the
     * actual Worker mappings and create its PID-bound Gateway first. This
     * byte only releases execution; it confers no Guest capability. */
    uint32_t status=uwvm_unitree_example_output("READY_UNITREE_MAIN\n",sizeof("READY_UNITREE_MAIN\n")-1);
    char gate=0;if(!status)status=uwvm_unitree_example_input(&gate,1);
    if(status||gate!='G')uwvm_unitree_direct_exit(status?status:22);
    __wasm_call_ctors();uwvm_unitree_direct_exit((uint32_t)uwvm_unitree_direct_main());
}
