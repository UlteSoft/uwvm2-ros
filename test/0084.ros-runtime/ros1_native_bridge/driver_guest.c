/** @file Exercise an installed C SDK against a deployment-owned ROS String bridge. */
#include <stdint.h>
#include <stddef.h>
#include <wros/scene.h>
#include "case_values.h"

typedef uint32_t driver_operation(uint64_t, uint32_t, uint64_t, uint64_t,
                                  uint64_t, uint64_t, uint64_t);

static uint8_t reply[1024];
static uint64_t reply_size;
static uint64_t capability;

static void check(int value)
{
    if (!value) __builtin_trap();
}

static uint32_t complete(driver_operation *operation, void const *input, size_t bytes)
{
    for (unsigned attempt = 0; attempt < 20000000; ++attempt) {
        uint32_t status = operation(capability, 0, (uintptr_t)input, bytes,
                                    (uintptr_t)reply, sizeof reply, (uintptr_t)&reply_size);
        if (status != 11) return status;
    }
    __builtin_trap();
}

void _start(void)
{
    static const char alias[] = "chatter";
    static const uint64_t sequence = 1;
    check(wr_capability_open(0, (uintptr_t)alias, sizeof alias - 1, (uintptr_t)&capability) == 0);
    check(capability != 0);
    check(complete(wr_driver_connect, 0, 0) == 0);
    check(complete(wr_driver_submit, submit_request, sizeof submit_request) == expected_submit);
    if (expected_submit == 0) {
        check(complete(wr_driver_result, &sequence, sizeof sequence) == 0);
        check(reply_size == sizeof expected_reply);
        for (size_t i = 0; i < sizeof expected_reply; ++i)
            check(reply[i] == expected_reply[i]);
    }
    check(complete(wr_driver_disconnect, 0, 0) == 0);
}
