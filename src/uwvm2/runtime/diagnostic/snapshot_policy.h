// Licensed under APL-2.0 (see LICENSE).
#pragma once
#ifndef UWVM_MODULE
# include <cstdint>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::diagnostic::snapshot_policy
{
    enum class permission : unsigned char { none, limited, read_only, full };
    enum class level : unsigned char { diagnostic, inspection, checkpoint, replay };
    enum class action : unsigned char { save, inspect, restore, replay };
    enum class refusal : unsigned char
    { none, invalid_selection, disabled, above_startup_limit, modification_denied, diagnostic_only,
      incomplete_state, incomplete_events, environment_selection_required };
    // Constructed by the host's startup configuration. No setter, wire
    // deserializer, Guest import or debugger command can widen this ceiling.
    class policy final
    {
        permission permission_{};
        level ceiling_{};
    public:
        constexpr explicit policy(permission permissions,level ceiling=level::replay) noexcept
            :permission_{permissions},ceiling_{ceiling} {}
        [[nodiscard]] constexpr permission permissions() const noexcept{return permission_;}
        [[nodiscard]] constexpr level maximum() const noexcept
        {return permission_==permission::limited?level::diagnostic:ceiling_;}
        [[nodiscard]] constexpr refusal authorize(action requested,level content,bool complete=false,
            bool events_complete=false,bool environments_selected=false) const noexcept
        {
            if(permission_>permission::full || ceiling_>level::replay || content>level::replay || requested>action::replay)
                return refusal::invalid_selection;
            if(permission_==permission::none)return refusal::disabled;
            if(content>maximum())return refusal::above_startup_limit;
            if(requested==action::save || requested==action::inspect)return refusal::none;
            if(permission_!=permission::full)return refusal::modification_denied;
            if(content<level::checkpoint)return refusal::diagnostic_only;
            if(!complete)return refusal::incomplete_state;
            if(!environments_selected)return refusal::environment_selection_required;
            if(requested==action::replay && (content!=level::replay||!events_complete))return refusal::incomplete_events;
            return refusal::none;
        }
    };
    enum class environment : unsigned char { wasip1=1, ros1=2, ros2=3 };
    enum class environment_mode : unsigned char { unspecified, saved, reconfigure };
    struct environment_selection
    {
        environment_mode wasip1{},ros1{},ros2{};
        [[nodiscard]] constexpr bool selected(environment domain) const noexcept
        {
            auto mode=domain==environment::wasip1?wasip1:domain==environment::ros1?ros1:domain==environment::ros2?ros2:environment_mode::unspecified;
            return mode==environment_mode::saved||mode==environment_mode::reconfigure;
        }
    };
}
