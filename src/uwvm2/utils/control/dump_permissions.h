// Licensed under APL-2.0 (see LICENSE).
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include "debug_permissions.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::control::dump_permissions
{
    // Disclosure, not completeness or execution authority. A complete
    // checkpoint and an ordinary inspection dump can both carry full data.
    enum class disclosure : unsigned char { none, diagnostic, full };
    enum class constraint : unsigned char { maximum, forced };
    struct selection
    {
        constraint kind{constraint::maximum};disclosure level{disclosure::full};
        friend bool operator==(selection const&,selection const&)=default;
    };
    enum class denial : unsigned char { none, invalid_selection, disabled, above_maximum, forced_mismatch, debug_permission };
    struct decision {disclosure level{disclosure::none};denial status{denial::disabled};};
    [[nodiscard]] inline constexpr char const* name(disclosure value) noexcept
    {return value==disclosure::diagnostic?"diagnostic":value==disclosure::full?"full":"none";}
    [[nodiscard]] inline constexpr char const* message(denial value) noexcept
    {
        switch(value)
        {
            case denial::none:return "allowed";
            case denial::invalid_selection:return "invalid dump disclosure selection";
            case denial::disabled:return "dumps are disabled by the immutable dump or debugger launch policy";
            case denial::above_maximum:return "requested disclosure exceeds the dump or debugger launch maximum";
            case denial::forced_mismatch:return "requested disclosure conflicts with --debug-dump-force";
            case denial::debug_permission:return "forced disclosure exceeds debugger permissions";
        }
        return "invalid dump policy";
    }
    [[nodiscard]] inline constexpr bool valid(selection s) noexcept
    {return s.kind<=constraint::forced && s.level<=disclosure::full && (s.kind!=constraint::maximum||s.level!=disclosure::none);}
    [[nodiscard]] inline constexpr disclosure debug_ceiling(debug_permissions::permission_level p) noexcept
    {
        switch(p)
        {case debug_permissions::permission_level::none:return disclosure::none;
         case debug_permissions::permission_level::limited:return disclosure::diagnostic;
         case debug_permissions::permission_level::full:case debug_permissions::permission_level::read_only:return disclosure::full;}
        return disclosure::none;
    }
    [[nodiscard]] inline constexpr decision resolve(selection s,debug_permissions::permission_level permission,
        disclosure requested=disclosure::none) noexcept
    {
        // requested=none means no per-export selection. Disabling exports is
        // exclusively a forced launch selection, not an empty successful file.
        if(!valid(s)||requested>disclosure::full)return {disclosure::none,denial::invalid_selection};
        auto const maximum=debug_ceiling(permission);
        if(s.level==disclosure::none||maximum==disclosure::none)return {disclosure::none,denial::disabled};
        if(s.kind==constraint::forced)
        {
            if(s.level>maximum)return {disclosure::none,denial::debug_permission};
            if(requested!=disclosure::none&&requested!=s.level)return {disclosure::none,denial::forced_mismatch};
            return {s.level,denial::none};
        }
        auto const effective=s.level<maximum?s.level:maximum;
        if(requested!=disclosure::none&&requested>effective)return {disclosure::none,denial::above_maximum};
        return {requested==disclosure::none?effective:requested,denial::none};
    }
    class launch_policy final
    {
        ::std::atomic<unsigned char> value_{};
    public:
        [[nodiscard]] bool configure_trusted_launch(selection s) noexcept
        {
            if(!valid(s))return false;
            auto const value=static_cast<unsigned char>(1u+static_cast<unsigned>(s.level)+4u*static_cast<unsigned>(s.kind));
            unsigned char expected{};
            return value_.compare_exchange_strong(expected,value,::std::memory_order_acq_rel)||expected==value;
        }
        [[nodiscard]] selection selected() const noexcept
        {
            auto value=value_.load(::std::memory_order_acquire);if(!value)return {};
            --value;return {static_cast<constraint>(value/4u),static_cast<disclosure>(value%4u)};
        }
        [[nodiscard]] decision permit(debug_permissions::permission_level permission,disclosure requested=disclosure::none) const noexcept
        {return resolve(selected(),permission,requested);}
        void seal_default_before_execution() noexcept
        {unsigned char expected{};(void)value_.compare_exchange_strong(expected,3u,::std::memory_order_acq_rel);}
    };
    inline launch_policy runtime_policy{};
    [[nodiscard]] inline decision permit(disclosure requested=disclosure::none) noexcept
    {return runtime_policy.permit(debug_permissions::selected(),requested);}
}
