// Run the NVIDIA timestamp synchronizer on fixed camera frame identifiers.
#include <cstdint>
#include <cstddef>
#include "isaac_common/messaging/message_stream_synchronizer.hpp"

static void verify(bool ok) {
    if (!ok) __builtin_trap();
}

static void qualification() {
    using Sync = nvidia::isaac_common::messaging::MessageStreamSynchronizer<std::uint32_t>;
    struct Frame { unsigned camera; std::int64_t time; std::uint32_t id; unsigned emissions; };
    constexpr Frame frames[] = {
        {0, 0, 100, 0}, {1, 0, 200, 1},
        {1, 12, 201, 1}, {0, 10, 101, 2},
        {1, 20, 202, 2}, {1, 30, 203, 2}, {0, 20, 102, 3}, {0, 30, 103, 4},
        {0, 40, 104, 4}, {0, 50, 105, 4}, {1, 50, 205, 5}
    };
    constexpr std::uint32_t pairs[][2] = {{100,200},{101,201},{102,202},{103,203},{105,205}};
    unsigned emissions{};
    Sync synchronizer(2, 4, 2, 10);
    synchronizer.RegisterCallback([&](std::int64_t, auto const& messages) {
        verify(emissions < 5 && messages.size() == 2);
        for (unsigned index{}; index != 2; ++index) {
            verify(messages[index].first == static_cast<int>(index));
            verify(messages[index].second == pairs[emissions][index]);
        }
        ++emissions;
    });
    for (auto const& frame : frames) {
        synchronizer.AddMessage(frame.camera, frame.time, frame.id);
        verify(emissions == frame.emissions);
    }
#if defined(WR_SYNC_WRONG_PAIR_COUNT)
    verify(emissions == 6);
#endif
}
#if defined(__wasm__)
extern "C" void _start() { qualification(); }
#else
int main() { qualification(); }
#endif
