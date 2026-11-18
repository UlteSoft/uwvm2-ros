/** @file
 * @brief Process-local counters for bounded ROSv2 host result observations.
 * @details A debugger reads these counters at a cooperative stop. Admission
 * reserves a sequence and charges its scopes before writing to the sink.
 * A sink failure can therefore also be an admitted observation. Counts
 * saturate at UINT64_MAX. No guest authority or payload is retained here.
 */
#pragma once
#include <cstdint>

extern "C++" {
namespace uwvm2::imported::ros::scene::call_audit
{
    struct statistics
    {
        bool configured{};
        std::uint64_t admitted{}, limited{}, busy{}, clock_failed{}, sink_failed{};
    };
}
}
