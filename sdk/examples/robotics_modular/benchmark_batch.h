/** @file One exact bounded batch, shared by one-shot and session root entries.
 * The caller explicitly initializes vision once. The seed is reset for each
 * batch; callback registration is acquired/retired on every batch, with a fresh
 * generation. A returned checksum commits only after successful close.
 */
#ifndef ROSWASM_ROBOTICS_BENCHMARK_BATCH_H
#define ROSWASM_ROBOTICS_BENCHMARK_BATCH_H
#include "graph.h"
#include "robotics_math.h"
#include "main_io.h"
#ifndef ROBOT_BENCHMARK_SEED
#define ROBOT_BENCHMARK_SEED 1
#endif
#ifndef ROBOT_BENCHMARK_ITERATIONS
#define ROBOT_BENCHMARK_ITERATIONS 10000
#endif
#if ROBOT_BENCHMARK_SEED < 0 || ROBOT_BENCHMARK_SEED > 4294967295ULL
#error "Select a uint32 seed, without implicit truncation"
#endif
#if ROBOT_BENCHMARK_ITERATIONS < 1 || ROBOT_BENCHMARK_ITERATIONS > 1000000
#error "Select a finite explicit iteration count in 1..1000000"
#endif
static uint32_t robot_benchmark_batch(uint64_t* output) {
    uint32_t seed=ROBOT_BENCHMARK_SEED,generation,n,error=0,closed;
    uint64_t checksum=ROBOT_MATH_CHECKSUM_INITIAL;
    if(!output) return 22;
    generation=robot_begin(ROBOT_OWNER); if(!generation) return 22;
    for(n=0;n!=ROBOT_BENCHMARK_ITERATIONS;++n) {
        int32_t samples[4],requested,clearance,command;
        robot_math_sample(&seed,samples,&requested);
        clearance=robot_clearance(samples[0],samples[1],samples[2],samples[3]);
        if(clearance<0 || clearance>5000) {error=22;break;}
        command=robot_step(ROBOT_OWNER,generation,requested,clearance);
        if(command < -100 || command > 100) {error=22;break;}
        checksum=robot_math_accumulate(checksum,command);
    }
    if(!error && robot_count(ROBOT_OWNER,generation)!=ROBOT_BENCHMARK_ITERATIONS) error=22;
    closed=robot_end(ROBOT_OWNER,generation); if(!error) error=closed;
    if(!error) *output=checksum; return error;
}
/** Exact binary framing; a short write is a terminal failed frame, not retried. */
static uint32_t robot_benchmark_emit(uint64_t checksum) {
    unsigned n; uint32_t written=UINT32_MAX,error; unsigned char wire[8]; robot_iovec vector={wire,8};
    for(n=0;n!=8;++n) wire[n]=(unsigned char)(checksum>>(n*8u));
    error=robot_write(1,&vector,1,&written); return error ? error : written==8 ? 0 : 22;
}
#endif
