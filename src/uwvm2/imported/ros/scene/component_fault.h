/** @file
 * @brief Closed, bounded first-failure evidence for one native incarnation.
 * @details These values describe host observations, never an inferred signal,
 * vendor exception or physical stopped state. The supervisor latches the first
 * failure under its state lock. Stop, close, successful reaping and observation
 * cannot clear it. No native stderr, path, PID or private bytes are retained.
 * The code is read only through the separately authorized component.describe
 * operation or privileged debugger metadata. It grants no recovery authority.
 */
#pragma once
#include <cstdint>
extern "C++" {
namespace uwvm2::imported::ros::scene::components
{
    enum class fault : std::uint64_t
    {
        none, admission_refused, launch_failed, startup_protocol, channel_failed,
        protocol_violation, deadline_elapsed, authority_lost, diagnostic_limit,
        backend_failed
    };
    /** @brief Reject unknown future observations instead of interpreting them. */
    inline constexpr bool valid_fault(std::uint64_t value)noexcept
    {return value<=static_cast<std::uint64_t>(fault::backend_failed);}
}
}
