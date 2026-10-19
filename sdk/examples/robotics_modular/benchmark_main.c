/** @file Bounded modular compute entry, independent of native ROS or a robot.
 * Compile-time seed/count are explicit artifact inputs; no CLI/environment or
 * guest clock is read. The host owns measured intervals and repetitions. Stdout
 * is exactly eight canonical LE checksum bytes, followed by proc_exit(0), or
 * a nonzero exit with no successful checksum on an error. No pointer is passed
 * between module instances. The actual pure callback variant is mandatory.
 */
#include "benchmark_batch.h"
static uint32_t robot_benchmark(void) {
    uint64_t checksum=0; uint32_t error;
    if(robot_vision_initialize()) return 22;
    error=robot_benchmark_batch(&checksum); if(!error) error=robot_benchmark_emit(checksum); return error;
}
ROBOT_EXTERN ROBOT_EXPORT("_start") __attribute__((noreturn)) void _start(void) {robot_exit(robot_benchmark());}
