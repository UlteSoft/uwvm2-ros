// Licensed under APL-2.0 (see LICENSE).
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::control::debug_permissions
{
    enum class permission_level : unsigned char { limited, full, read_only, none };
    // Debugger authority only. Imports, drivers and signed updates have their
    // own launch grants. Neither Guest bytes nor a debugger command can change
    // this selection, including across reset or code replacement.
    class launch_policy
    {
        enum class state : unsigned char { unconfigured, limited, full, read_only, none };
        ::std::atomic<state> value_{state::unconfigured};
    public:
        [[nodiscard]] bool configure_trusted_launch(permission_level level) noexcept
        {
            state desired{};
            switch(level)
            {
                case permission_level::limited: desired=state::limited; break;
                case permission_level::full: desired=state::full; break;
                case permission_level::read_only: desired=state::read_only; break;
                case permission_level::none: desired=state::none; break;
                default: return false;
            }
            auto expected{state::unconfigured};
            return value_.compare_exchange_strong(expected,desired,::std::memory_order_acq_rel) || expected==desired;
        }
        void seal_default_before_execution() noexcept
        {
            auto expected{state::unconfigured};
            (void)value_.compare_exchange_strong(expected,state::limited,::std::memory_order_acq_rel);
        }
        [[nodiscard]] permission_level selected() const noexcept
        {
            switch(value_.load(::std::memory_order_acquire))
            {
                case state::full:return permission_level::full;
                case state::read_only:return permission_level::read_only;
                case state::none:return permission_level::none;
                case state::limited:return permission_level::limited;
                case state::unconfigured:
#if !defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
                    // Standalone upstream component harnesses have no ROS CLI.
                    return permission_level::full;
#else
                    return permission_level::limited;
#endif
            }
            return permission_level::none;
        }
    };
    inline launch_policy runtime_policy{};
    [[nodiscard]] inline permission_level selected() noexcept {return runtime_policy.selected();}
    [[nodiscard]] inline char const* name() noexcept
    {
        switch(selected())
        {case permission_level::full:return "full";case permission_level::read_only:return "read-only";
         case permission_level::limited:return "limited";case permission_level::none:return "none";}
        return "none";
    }
    [[nodiscard]] inline bool full_allowed() noexcept {return selected()==permission_level::full;}
    [[nodiscard]] inline bool inspection_allowed() noexcept
    {auto p=selected();return p==permission_level::full || p==permission_level::read_only;}
    [[nodiscard]] inline bool diagnostics_allowed() noexcept {return selected()!=permission_level::none;}
    [[nodiscard]] inline bool configure_trusted_launch(permission_level p) noexcept
    {return runtime_policy.configure_trusted_launch(p);}
    inline void seal_default_before_execution() noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        runtime_policy.seal_default_before_execution();
#endif
    }
}
