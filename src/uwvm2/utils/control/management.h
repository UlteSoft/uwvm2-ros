/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)
 * Licensed under the APL-2.0 License (see LICENSE file).
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::control::management
{
    enum class execution_profile : unsigned char { release, dev };
    // Process-local trusted boot policy. Never serialized, exposed to a Guest,
    // restored by checkpoint, or reset with runtime/module storage.
    class launch_policy
    {
        enum class state : unsigned char { unconfigured, development, release_sealed };
        ::std::atomic<state> value_{state::unconfigured};
    public:
        [[nodiscard]] bool configure_trusted_launch(execution_profile profile) noexcept
        {
            auto expected{state::unconfigured};
            auto const desired{profile == execution_profile::dev ? state::development : state::release_sealed};
            // A repeated identical boot request is harmless; RELEASE -> DEV is
            // impossible, including after reset, reconnect or a failed update.
            return value_.compare_exchange_strong(expected, desired, ::std::memory_order_acq_rel) || expected == desired;
        }
        void seal_default_before_execution() noexcept
        {
            auto expected{state::unconfigured};
            (void)value_.compare_exchange_strong(expected, state::release_sealed, ::std::memory_order_acq_rel);
        }
        void seal_release() noexcept { value_.store(state::release_sealed, ::std::memory_order_release); }
        [[nodiscard]] bool debug_allowed() const noexcept
        { return value_.load(::std::memory_order_acquire) == state::development; }
        [[nodiscard]] bool release_sealed() const noexcept
        { return value_.load(::std::memory_order_acquire) == state::release_sealed; }
    };
    inline launch_policy runtime_policy{};
    [[nodiscard]] inline bool debug_allowed() noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        return runtime_policy.debug_allowed();
#else
        return !runtime_policy.release_sealed();
#endif
    }
    [[nodiscard]] inline bool configure_trusted_launch(execution_profile profile) noexcept
    { return runtime_policy.configure_trusted_launch(profile); }
    inline void seal_default_before_execution() noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        runtime_policy.seal_default_before_execution();
#endif
    }
    inline void seal_release() noexcept { runtime_policy.seal_release(); }
}
