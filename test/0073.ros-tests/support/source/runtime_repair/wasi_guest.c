/**
 * @file wasi_guest.c
 * @brief Require actual WASI clock and output imports from the AOT runtime.
 * @details Import the defined wasm32 Preview 1 ABI directly to keep this
 * regression independent of a particular guest libc or language compiler SDK.
 */
typedef unsigned u32;
typedef unsigned long long u64;

struct iovec
{
    char const* data;
    u32 size;
};

__attribute__((import_module("wasi_snapshot_preview1"), import_name("fd_write"))) extern u32
    fd_write(u32 descriptor, struct iovec const* vectors, u32 count, u32* written);
__attribute__((import_module("wasi_snapshot_preview1"), import_name("clock_time_get"))) extern u32 clock_time_get(u32 clock, u64 precision, u64* timestamp);

void _start(void)
{
    u64 timestamp = 0;
    u32 written = 0;
    struct iovec output = {"AOT-WASI-OK\n", sizeof("AOT-WASI-OK\n") - 1};
    u32 status = clock_time_get(1, 1, &timestamp);
    if(status == 76u)
    {
        /* Preview 1 ENOTCAPABLE must leave the denied output untouched. */
        if(timestamp != 0) { __builtin_trap(); }
        output.data = "AOT-WASI-DENIED:76\n";
        output.size = sizeof("AOT-WASI-DENIED:76\n") - 1;
    }
    else if(status != 0 || timestamp == 0) { __builtin_trap(); }
    if(fd_write(1, &output, 1, &written) != 0 || written != output.size) { __builtin_trap(); }
}
