// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
// Included after the concrete stable v1 Host implementation. No v2 adapter is
// installed here. The runtime must supply a genuine stopped, closed cohort.
namespace uwvm2::uwvm::imported::ros::host
{
    class current_environment_handoff final : public core::checkpoint::handoff
    {
        friend class recorded_environment_handoff;
        decltype(instances) previous_,replacement_;
        ::uwvm2::utils::container::std_map<void const*,bool,::std::less<>> retired_;
        core::capability_manager::checkpoint_rebinding capabilities_;
        bool published_{};
        [[nodiscard]] bool current_locked() noexcept
        {
            if(published_ || previous_.size()!=instances.size())return false;
            auto actual=instances.begin();
            for(auto const& [key,value]:previous_)
            {
                auto const retired=retired_.find(key);
                if(actual==instances.end() || actual->first!=key || actual->second!=value ||
                   retired==retired_.end() || value->retired!=retired->second)return false;
                ++actual;
            }
            return capabilities_.current();
        }
    public:
        [[nodiscard]] core::checkpoint::status prepare(::std::span<core::checkpoint::member const> members)
        {
            ::std::lock_guard registry{mutex};
            // Registry -> capability registry is the existing admission order.
            // The inner preparer validates the complete supplied mapping first.
            auto const status=capabilities.prepare_checkpoint_rebinding(members,capabilities_);
            if(status!=core::checkpoint::status::ready)return status;
            for(auto const& [key,value]:instances)
            {
                core::checkpoint::member const* selected{};
                for(auto const& member:members)if(member.previous==key && member.previous_epoch==value->generation)
                    {selected=::std::addressof(member);break;}
                if(!selected || selected->authority_retired!=value->retired)return core::checkpoint::status::stale;
                // Current-session handoff retains live v1 peer identity and
                // queued work. It never pretends that robot effects roll back.
                // Newer RMW/portable endpoints require their own future adapter.
                for(auto const& backend:value->backends)
                {
                    ::std::lock_guard lock{backend->mutex};
                    if(!backend->rmw_endpoints.rows().empty() || !backend->rmw_service_pairs.rows().empty() ||
                       !backend->rmw_service_journals.empty())return core::checkpoint::status::unsupported;
                }
                auto next=::uwvm2::utils::container::make_shared<instance>();
                next->generation=selected->replacement_epoch;next->retired=value->retired;
                next->backends=value->backends;
                previous_.emplace(key,value);retired_.emplace(key,value->retired);
                if(!replacement_.emplace(selected->replacement,::std::move(next)).second)
                    return core::checkpoint::status::malformed;
            }
            return core::checkpoint::status::ready;
        }
        [[nodiscard]] bool current() noexcept override
        {::std::lock_guard registry{mutex};return current_locked();}
        [[nodiscard]] bool publish() noexcept override
        {
            ::std::lock_guard registry{mutex};
            if(!current_locked() || !capabilities_.publish())return false;
            // No subsequent fallible operation: both registries publish the
            // same mapping before host admission can reopen.
            instances.swap(replacement_);published_=true;return true;
        }
    };
    inline core::checkpoint::status prepare_current_environment_handoff(
        ::std::span<core::checkpoint::member const> members,core::checkpoint::owner& output) noexcept
    {
        if(output)return core::checkpoint::status::malformed;
#if defined(UWVM_CPP_EXCEPTIONS) || defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
        {
            auto candidate=::uwvm2::utils::container::make_unique<current_environment_handoff>();
            auto status=candidate->prepare(members);
            if(status==core::checkpoint::status::ready)output=::std::move(candidate);
            return status;
        }
        catch(...) {return core::checkpoint::status::allocation_failed;}
#else
        return core::checkpoint::status::unsupported;
#endif
    }
}
