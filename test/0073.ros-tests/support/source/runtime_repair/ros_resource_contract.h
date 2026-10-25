/**
 * @file ros_resource_contract.h
 * @brief Share bounded result checks between native IPC and Wasm measurements.
 * @details Each case observes the same isolated native peer. Graph comparisons
 * require a known live row and reject rows outside the granted subtree. This
 * is test code; it neither supplies production replies nor changes policy.
 */
#pragma once
#include <stddef.h>
#include <stdint.h>

#define UWVM_BENCH_SCOPE "/uwvm_perf/peer"
#define UWVM_BENCH_NODE UWVM_BENCH_SCOPE "/resource_peer"
#define UWVM_BENCH_VALUE UWVM_BENCH_NODE "/value"

/** @brief Map the stable fixture case number to its public gateway operation. */
static inline char const* resource_operation(unsigned which)
{
    static char const* const names[] = {
        "", "graph.list-topics", "graph.inspect-topic", "graph.list-nodes",
        "graph.inspect-node", "graph.list-services", "graph.inspect-service",
        "param.get", "param.set", "param.list", "param.describe", "param.exists", "param.search"
    };
    return which < sizeof(names) / sizeof(*names) ? names[which] : "";
}

/** @brief Compare a bounded byte span with one complete literal string. */
static inline int resource_equal(uint8_t const* bytes, size_t size, char const* text)
{
    size_t index = 0;
    for(; text[index]; ++index)
    {
        if(index >= size || bytes[index] != (uint8_t)text[index]) { return 0; }
    }
    return index == size;
}

/** @brief Find one complete expected row without reading beyond the reply. */
static inline int resource_contains(uint8_t const* bytes, size_t size, char const* text)
{
    size_t length = 0;
    while(text[length]) { ++length; }
    if(length > size) { return 0; }
    for(size_t offset = 0; offset <= size - length; ++offset)
    {
        if(resource_equal(bytes + offset, length, text)) { return 1; }
    }
    return 0;
}

/** @brief Require the canonical result and the actual peer's expected value. */
static inline int resource_valid(unsigned which, unsigned version, uint8_t const* bytes, size_t size)
{
    if(which == 8) { return size == 0; }
    if(which == 11) { return size == 1 && bytes[0] == 1; }
    if(size < 4 || size > 16384) { return 0; }
    uint32_t length = (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8 |
        (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;
    if(length != size - 4) { return 0; }
    bytes += 4;
    size -= 4;
    if(which == 7) { return resource_equal(bytes, size, "ok"); }
    if(which == 10)
    {
        return resource_equal(bytes, size, "name=value\ntype=4\nread_only=false\ndescription=\nconstraints=");
    }
    if(which == 12) { return resource_equal(bytes, size, UWVM_BENCH_VALUE); }
    char const* expected = "";
    if(which == 1 || which == 2)
    {
        expected = version == 1 ? UWVM_BENCH_SCOPE "/topic\tstd_msgs/String\n" :
            UWVM_BENCH_SCOPE "/topic\tstd_msgs/msg/String\n";
    }
    else if(which == 3 || which == 4) { expected = UWVM_BENCH_NODE "\n"; }
    else if(which == 5 || which == 6)
    {
        expected = version == 1 ? UWVM_BENCH_SCOPE "/trigger\tnot-applicable\n" :
            UWVM_BENCH_SCOPE "/trigger\tstd_srvs/srv/Trigger\n";
    }
    else if(which == 9) { expected = UWVM_BENCH_VALUE "\n"; }
    else { return 0; }
    if(!resource_contains(bytes, size, expected)) { return 0; }
    if(which == 2 || which == 4 || which == 6) { return resource_equal(bytes, size, expected); }
    size_t start = 0;
    while(start < size)
    {
        size_t end = start;
        while(end < size && bytes[end] != '\n') { ++end; }
        size_t prefix = sizeof(UWVM_BENCH_SCOPE) - 1;
        if(end == size || end - start <= prefix || bytes[start + prefix] != '/' ||
           !resource_equal(bytes + start, prefix, UWVM_BENCH_SCOPE)) { return 0; }
        start = end + 1;
    }
    return 1;
}
