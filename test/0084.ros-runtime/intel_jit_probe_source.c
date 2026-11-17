/* Source labels for the exact independently generated x86-64 probe only. */
#include <stdint.h>

uint64_t uwvm_profile_probe_kernel(uint64_t count) {
    uint64_t value = 0;
    while (count != 0) {
        /* Each native instruction has the matching checked range. */
        value *= 1664525;
        value += 1013904223;
        value ^= count;
        --count;
    }
    return value;
}
