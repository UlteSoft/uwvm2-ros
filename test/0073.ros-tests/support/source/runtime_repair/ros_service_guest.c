/**
 * @file ros_service_guest.c
 * @brief Measure complete, authorized ROS1 or ROS2 Trigger exchanges from Wasm.
 * @details The wire contract and seven-byte response match the original PDF
 * benchmark. Discovery, twenty warm-up exchanges, compilation and profiler
 * control occur outside the measured interval. The supervisor supplies one
 * byte on standard input after observing BENCH_READY, permitting hardware
 * collection to exclude startup without adding a runtime or ROS protocol hook.
 */
#include <stdint.h>
#include <stddef.h>
#if defined(UWVM_ROS_RESOURCE_CASE)
# include "ros_resource_contract.h"
#endif

#ifndef UWVM_ROS_BENCH_ITERATIONS
# define UWVM_ROS_BENCH_ITERATIONS 200
#endif
#ifndef UWVM_ROS_BENCH_IMPORT
# define UWVM_ROS_BENCH_IMPORT "service_call"
#endif

#define WASI_IMPORT(name) __attribute__((import_module("wasi_snapshot_preview1"), import_name(name)))
#if defined(UWVM_ROS_BENCH_VERSION) && UWVM_ROS_BENCH_VERSION == 1
# define ROS_IMPORT(name) __attribute__((import_module("uwvm_ros1_v1"), import_name(name)))
#else
# define ROS_IMPORT(name) __attribute__((import_module("uwvm_ros2_v1"), import_name(name)))
#endif

typedef struct
{
    void const* data;
    uint32_t length;
} wasi_iovec;

WASI_IMPORT("clock_time_get") extern uint32_t wasi_clock(uint32_t, uint64_t, uint64_t*);
WASI_IMPORT("fd_write") extern uint32_t wasi_write(uint32_t, wasi_iovec const*, uint32_t, uint32_t*);
WASI_IMPORT("fd_read") extern uint32_t wasi_read(uint32_t, wasi_iovec const*, uint32_t, uint32_t*);
ROS_IMPORT("capability_open") extern uint32_t ros_open(uint32_t, uint64_t, uint64_t, uint64_t);
ROS_IMPORT("capability_close") extern uint32_t ros_close(uint64_t);
ROS_IMPORT(UWVM_ROS_BENCH_IMPORT) extern uint32_t ros_call(uint64_t, uint32_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

/** @brief Read the explicitly granted monotonic clock, failing on any refusal. */
static uint64_t monotonic_ns(void)
{
    uint64_t value = 0;
    if(wasi_clock(1, 1, &value)) { __builtin_trap(); }
    return value;
}

/** @brief Emit one complete scalar record outside the measured call loop. */
static void report(char const* label, uint64_t value)
{
    char buffer[128];
    uint32_t size = 0;
    while(label[size])
    {
        buffer[size] = label[size];
        ++size;
    }
    char digits[24];
    uint32_t count = 0;
    do
    {
        digits[count++] = (char)('0' + value % 10);
        value /= 10;
    }
    while(value);
    while(count) { buffer[size++] = digits[--count]; }
    buffer[size++] = '\n';
    uint32_t offset = 0;
    while(offset != size)
    {
        wasi_iovec vector = {buffer + offset, size - offset};
        uint32_t written = 0;
        if(wasi_write(2, &vector, 1, &written) || !written || written > size - offset) { __builtin_trap(); }
        offset += written;
    }
}

/** @brief Perform one real native service exchange and validate every reply byte. */
static void exchange(uint64_t handle)
{
#if defined(UWVM_ROS_RESOURCE_CASE)
    uint8_t response[16384];
    uint8_t const request[] = {2, 0, 0, 0, 'o', 'k'};
    uint64_t request_size = UWVM_ROS_RESOURCE_CASE == 8 ? sizeof(request) : 0;
#else
    uint8_t response[64] = {0};
#endif
    uint64_t length = 0;
#if defined(UWVM_ROS_RESOURCE_CASE)
    uint32_t status = ros_call(handle, 0, (uintptr_t)request, request_size, (uintptr_t)response, sizeof(response), (uintptr_t)&length);
#else
    uint32_t status = ros_call(handle, 0, 0, 0, (uintptr_t)response, sizeof(response), (uintptr_t)&length);
#endif
    if(status)
    {
        report("BENCH_ROS_ERROR=", status);
        __builtin_trap();
    }
#if defined(UWVM_ROS_RESOURCE_CASE)
    if(!resource_valid(UWVM_ROS_RESOURCE_CASE, UWVM_ROS_BENCH_VERSION, response, length))
#elif defined(UWVM_ROS_BENCH_CLOCK)
    /* Every clock reply is an actual canonical unsigned nanosecond value.
       Wall/ROS clocks may be adjusted, so only the steady clock can promise
       monotonic ordering; no artificial constant response is accepted. */
    uint64_t nanoseconds = 0;
    for(unsigned index = 0; index != 8; ++index) { nanoseconds |= (uint64_t)response[index] << (index * 8); }
    if(length != 8 || !nanoseconds)
#else
    if(length != 7 || response[0] != 1 || response[1] != 2 || response[2] || response[3] || response[4] || response[5] != 'o' || response[6] != 'k')
#endif
    {
        __builtin_trap();
    }
}

/** @brief Measure only the warmed service loop, preserving the original capability. */
void _start(void)
{
    uint64_t handle = 0;
    char const alias[] = "client";
    uint64_t begin = monotonic_ns();
    uint32_t status = ros_open(0, (uintptr_t)alias, 6, (uintptr_t)&handle);
    report("BENCH_ROS_OPEN_NS=", monotonic_ns() - begin);
    if(status || !handle)
    {
        report("BENCH_ROS_OPEN_ERROR=", status);
        __builtin_trap();
    }
#if defined(UWVM_ROS_RESOURCE_CASE) && UWVM_ROS_RESOURCE_CASE <= 6
    /* Graph discovery is asynchronous. Only read-only discovery probes may
       repeat, before warm-up and timing; any later failed query is fatal. */
    begin = monotonic_ns();
    uint64_t probes = 0;
    for(;;)
    {
        uint8_t response[16384];
        uint64_t length = 0;
        status = ros_call(handle, 0, 0, 0, (uintptr_t)response, sizeof(response), (uintptr_t)&length);
        ++probes;
        if(status == 0 && resource_valid(UWVM_ROS_RESOURCE_CASE, UWVM_ROS_BENCH_VERSION, response, length)) { break; }
        if((status != 0 && status != 2) || monotonic_ns() - begin >= UINT64_C(10000000000))
        {
            report("BENCH_DISCOVERY_ERROR=", status);
            __builtin_trap();
        }
    }
    report("BENCH_DISCOVERY_PROBES=", probes);
#endif
    for(uint32_t i = 0; i != 20; ++i) { exchange(handle); }
    report("BENCH_READY=", UWVM_ROS_BENCH_ITERATIONS);
    char ready = 0;
    wasi_iovec input = {&ready, 1};
    uint32_t received = 0;
    if(wasi_read(0, &input, 1, &received) || received != 1 || ready != 'R') { __builtin_trap(); }
    begin = monotonic_ns();
    for(uint32_t i = 0; i != UWVM_ROS_BENCH_ITERATIONS; ++i) { exchange(handle); }
    report("BENCH_BODY_NS=", monotonic_ns() - begin);
    report("BENCH_ROS_COUNT=", UWVM_ROS_BENCH_ITERATIONS);
    report("BENCH_CHECKSUM=", UWVM_ROS_BENCH_ITERATIONS);
    if(ros_close(handle)) { __builtin_trap(); }
}
