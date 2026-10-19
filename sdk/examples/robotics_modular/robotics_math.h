/** @file Portable shared integer oracle, also usable by an independent native
 * fixture. No ROS, Wasm, allocator, IO, floating point, clock or hardware API.
 * Unsigned overflow in the sample generator/checksum is explicitly modulo2^N.
 * A checksum comparison proves the same synthetic computation, not physical
 * dynamics, sensor perception or correctness of a robot's control loop.
 */
#ifndef ROSWASM_ROBOTICS_MATH_H
#define ROSWASM_ROBOTICS_MATH_H
#include <stdint.h>
#define ROBOT_MATH_INVALID (INT32_MIN + 22)
#define ROBOT_MATH_MAX_ITERATIONS UINT32_C(1000000)
#define ROBOT_MATH_CHECKSUM_INITIAL UINT64_C(14695981039346656037)
static inline int32_t robot_math_nearest(int32_t a,int32_t b,int32_t c,int32_t d) {
    int32_t value;
    if(a<0 || a>5000 || b<0 || b>5000 || c<0 || c>5000 || d<0 || d>5000) return ROBOT_MATH_INVALID;
    value=a<b?a:b; if(c<value) value=c; if(d<value) value=d; return value;
}
static inline int32_t robot_math_command(int32_t requested,int32_t clearance) {
    if(requested < -100 || requested > 100 || clearance < 0 || clearance > 5000) return ROBOT_MATH_INVALID;
    return clearance < 400 ? 0 : requested;
}
static inline uint32_t robot_math_next(uint32_t* state) {
    *state = *state * UINT32_C(1664525) + UINT32_C(1013904223); return *state;
}
/** @brief Deterministic fixtures, not randomness or a noise/sensor model. */
static inline void robot_math_sample(uint32_t* state,int32_t samples[4],int32_t* requested) {
    unsigned n; for(n=0;n!=4;++n) samples[n]=(int32_t)(robot_math_next(state)%UINT32_C(5001));
    *requested=(int32_t)(robot_math_next(state)%UINT32_C(201))-100;
}
static inline uint64_t robot_math_accumulate(uint64_t checksum,int32_t command) {
    return (checksum ^ (uint64_t)(uint32_t)command) * UINT64_C(1099511628211);
}
/** @brief Fused native oracle; UINT64_MAX is reserved for invalid iteration bounds.
 * A modular Worker additionally measures import/callback/lifetime overhead.
 * A performance report must label this difference rather than claiming equal
 * process topology, dispatch machinery or native ROS transport work.
 */
static inline uint64_t robot_math_batch(uint32_t seed,uint32_t iterations) {
    uint32_t n; uint64_t checksum=ROBOT_MATH_CHECKSUM_INITIAL;
    if(!iterations || iterations>ROBOT_MATH_MAX_ITERATIONS) return UINT64_MAX;
    for(n=0;n!=iterations;++n) {
        int32_t samples[4],requested,command;
        robot_math_sample(&seed,samples,&requested);
        command=robot_math_command(requested,robot_math_nearest(samples[0],samples[1],samples[2],samples[3]));
        checksum=robot_math_accumulate(checksum,command);
    }
    return checksum;
}
#endif
