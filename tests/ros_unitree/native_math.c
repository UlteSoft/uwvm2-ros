/**
 * @file native_math.c
 * @brief Fused portable math baseline for the modular Wasm example benchmark.
 *
 * Compile with the same ROBOT_BENCHMARK_SEED and ROBOT_BENCHMARK_ITERATIONS
 * definitions as sdk/examples/robotics_modular/benchmark_main.c. This baseline
 * calls the actual shared oracle, not a reimplementation. Its exact output is
 * eight little-endian checksum bytes. It excludes Wasm module/callback dispatch,
 * ROS, DDS, MuJoCo and physical robot behavior; report these scopes separately.
 */
#include "../../sdk/examples/robotics_modular/robotics_math.h"
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#ifndef ROBOT_BENCHMARK_SEED
#define ROBOT_BENCHMARK_SEED 1
#endif
#ifndef ROBOT_BENCHMARK_ITERATIONS
#define ROBOT_BENCHMARK_ITERATIONS 10000
#endif
#ifndef ROBOT_BENCHMARK_SESSION
#define ROBOT_BENCHMARK_SESSION 0
#endif
static int emit_batch(void) {
    uint64_t value = robot_math_batch(ROBOT_BENCHMARK_SEED, ROBOT_BENCHMARK_ITERATIONS);
    if (value == UINT64_MAX) return 22;
    unsigned char bytes[8];
    for (unsigned i = 0; i != 8; ++i) bytes[i] = (unsigned char)(value >> (8*i));
    size_t offset = 0;
    while (offset != sizeof bytes) {
        ssize_t count = write(1, bytes + offset, sizeof bytes - offset);
        if (count < 0 && errno == EINTR) continue;
        if (count <= 0) return 5;
        offset += (size_t)count;
    }
    return 0;
}
int main(void) {
#if ROBOT_BENCHMARK_SESSION
    static unsigned char const ready[] = "READY\n";
    if (write(1, ready, sizeof ready - 1) != sizeof ready - 1) return 5;
    for (unsigned pass = 0; pass != 256; ++pass) {
        unsigned char operation;
        ssize_t count;
        do { count = read(0, &operation, 1); } while (count < 0 && errno == EINTR);
        if (count != 1) return 5;
        if (operation == 'Q') return 0;
        if (operation != 'W' && operation != 'P') return 22;
        int status = emit_batch();
        if (status) return status;
    }
    return 110;
#else
    return emit_batch();
#endif
}
