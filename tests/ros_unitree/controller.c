/**
 * @file controller.c
 * @brief Identical native/Wasm binary controller protocol, without ROS rights.
 *
 * Input is exactly twelve bytes: ASCII UCK1, LE u32 step count [1,10000], LE
 * u32 timestep in nanoseconds [100000,20000000]. Output is ASCII UCM1 followed
 * by the same count and timestep (12 bytes), then count records. Each record
 * is LE u32 logical step [1,count], then twenty motors, each six LE u32 words:
 * mode, IEEE754 binary32 q,dq,kp,kd,tau. Record size is 484 bytes. There is no
 * native struct/padding serialization. This frame is a fixture interchange,
 * not CDR or the Unitree CRC input. The real native peer constructs official
 * message objects and calculates their official CRC separately.
 *
 * Wasm exports _start and explicitly imports only basic WASIp1 fd_read,
 * fd_write, and proc_exit. No clock, random, file, socket or ROS capability is
 * requested. Root builds native and Wasm from this source with the same kernel
 * header, retains compile/link provenance, and supplies the actual binaries.
 */
#include "controller_kernel.h"
#include <stddef.h>
#include <string.h>
#if defined(__wasm__)
typedef struct { void *data; size_t bytes; } fixture_iov;
__attribute__((import_module("wasi_snapshot_preview1"), import_name("fd_read")))
extern uint32_t fixture_read(uint32_t, fixture_iov const*, uint32_t, uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("fd_write")))
extern uint32_t fixture_write(uint32_t, fixture_iov const*, uint32_t, uint32_t*);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("proc_exit"), noreturn))
extern void fixture_exit(uint32_t);
static int transfer(int output, unsigned char *data, size_t size) {
    while (size) {
        fixture_iov iov = {data, size}; uint32_t count = 0;
        uint32_t error = output ? fixture_write(1, &iov, 1, &count) : fixture_read(0, &iov, 1, &count);
        if (error || !count || count > size) return 1;
        data += count; size -= count;
    }
    return 0;
}
#else
#include <errno.h>
#include <unistd.h>
static int transfer(int output, unsigned char *data, size_t size) {
    while (size) {
        ssize_t count = output ? write(1, data, size) : read(0, data, size);
        if (count < 0 && errno == EINTR) continue;
        if (count <= 0 || (size_t)count > size) return 1;
        data += count; size -= (size_t)count;
    }
    return 0;
}
#endif
static uint32_t get32(unsigned char const *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}
static void put32(unsigned char *p, uint32_t n) {
    for (unsigned i = 0; i != 4; ++i) p[i] = (unsigned char)(n >> (8*i));
}
static void put_float(unsigned char *p, float value) {
    uint32_t bits; _Static_assert(sizeof(float) == 4, "binary32 required");
    memcpy(&bits, &value, 4); put32(p, bits);
}
static int run_controller(void) {
    unsigned char header[12], record[484];
    if (transfer(0, header, sizeof header) || memcmp(header, "UCK1", 4)) return 22;
    uint32_t count = get32(header + 4), nanos = get32(header + 8);
    if (!count || count > 10000 || nanos < 100000 || nanos > 20000000) return 22;
    memcpy(header, "UCM1", 4);
    if (transfer(1, header, sizeof header)) return 5;
    for (uint32_t step = 1; step <= count; ++step) {
        uwvm_unitree_fixture_motor motors[20];
        if (uwvm_unitree_go2_fixture_step(step, (double)nanos / 1E9, motors)) return 22;
        put32(record, step);
        for (unsigned i = 0; i != 20; ++i) {
            unsigned char *p = record + 4 + 24*i;
            put32(p, motors[i].mode); put_float(p+4, motors[i].q); put_float(p+8, motors[i].dq);
            put_float(p+12, motors[i].kp); put_float(p+16, motors[i].kd); put_float(p+20, motors[i].tau);
        }
        if (transfer(1, record, sizeof record)) return 5;
    }
    return 0;
}
#if defined(__wasm__)
__attribute__((export_name("_start"), noreturn)) void fixture_start(void) { fixture_exit((uint32_t)run_controller()); }
#else
int main(void) { return run_controller(); }
#endif
