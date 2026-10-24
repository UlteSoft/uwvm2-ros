/** Optional freestanding WASIp1 command entry for the portable test sources.
 * The only non-ROS import is ordinary process termination. No environment,
 * filesystem, clock, random, network, policy or debugger import is acquired.
 */
#if defined(__wasm__)
int main(void);
__attribute__((import_module("wasi_snapshot_preview1"),import_name("proc_exit"),noreturn))
void roswasmc_test_exit(unsigned code);
__attribute__((export_name("_start"),noreturn)) void _start(void) {roswasmc_test_exit((unsigned)main());}
#endif
