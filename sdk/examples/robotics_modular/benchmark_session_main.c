/** @file Caller-driven warmed-instance protocol, separate from the one-shot.
 * Emit READY\n after graph initialization. Accept at most256 single-byte
 * commands: W=unmeasured warmup batch, P=measured batch, Q=checked normal exit.
 * W/P have identical computation and eight-byte checksum framing. The host
 * chooses the measurement boundary; the guest never reads a clock. EOF,
 * malformed/short IO or exhaustion without Q is a nonzero terminal failure.
 * The external supervisor must impose an actual finite wall-clock deadline.
 */
#include "benchmark_batch.h"
static uint32_t robot_session(void) {
    unsigned pass; uint32_t error;
    if(robot_vision_initialize()) return 22;
    error=ROBOT_MARKER("READY\n"); if(error) return error;
    for(pass=0;pass!=256;++pass) {
        char command=0; uint32_t count=UINT32_MAX; uint64_t checksum=0;
        robot_iovec vector={&command,1};
        error=robot_read(0,&vector,1,&count); if(error) return error;
        if(count!=1) return 22;
        if(command=='Q') return 0;
        if(command!='W' && command!='P') return 22;
        error=robot_benchmark_batch(&checksum); if(error) return error;
        error=robot_benchmark_emit(checksum); if(error) return error;
    }
    return 110;
}
ROBOT_EXTERN ROBOT_EXPORT("_start") __attribute__((noreturn)) void _start(void) {robot_exit(robot_session());}
