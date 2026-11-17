// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/common/checkpoint_restore.h>
#endif
// Stable v1 only. Local queues may rewind; native endpoints, service requests,
// permissions, quotas, audit trails and external effects never do.
namespace uwvm2::uwvm::imported::ros::host
{
    class recorded_environment_handoff final : public core::checkpoint::handoff
    {
        using cp_status=core::checkpoint::status;
        current_environment_handoff current_{};
        ::uwvm2::utils::container::std_vector<core::checkpoint::source_member> sources_{};
        ::uwvm2::utils::container::std_vector<core::checkpoint::environment_owner> saved_{};
        unsigned backends_{};
        struct pending_session
        {
            ::std::shared_ptr<session> owner{};
            decltype(session::observations) previous{},replacement{};
            ::std::uint64_t native_session{},next_service_request{};
            bool connected{};
            ::std::unique_lock<::std::mutex> lock{};
        };
        ::uwvm2::utils::container::std_vector<pending_session> sessions_{};
        [[nodiscard]] bool validate_current() const noexcept
        {
            ::uwvm2::utils::container::std_vector<core::checkpoint::environment_owner> now;
            if(saved_.empty() || capture_checkpoint_environment(sources_,saved_.front()->recording,backends_,now)!=cp_status::ready ||
               now.size()!=saved_.size())return false;
            for(::std::size_t i{};i<now.size();++i)
            {
                auto const& a=*saved_[i];auto const& b=*now[i];
                if(a.backend!=b.backend || a.groups.size()!=b.groups.size())return false;
                for(::std::size_t j{};j<a.groups.size();++j)
                    if(!core::checkpoint::recorded_group_matches_current(a.groups[j],b.groups[j]))return false;
            }
            return true;
        }
        void unlock_sessions() noexcept
        {for(auto& item:sessions_)if(item.lock.owns_lock())item.lock.unlock();}
        [[nodiscard]] bool lock_sessions() noexcept
        {
            for(auto& item:sessions_)
            {
                if(!item.lock.try_lock()){unlock_sessions();return false;}
                auto const& s=*item.owner;
                if(s.availability_native_session!=item.native_session || s.connected!=item.connected ||
                   s.next_service_request!=item.next_service_request || s.observations.size()!=item.previous.size())
                {unlock_sessions();return false;}
                auto current=s.observations.begin();
                for(auto const& [handle,old]:item.previous)
                {
                    if(current==s.observations.end() || current->first!=handle){unlock_sessions();return false;}
                    auto const& o=current->second;
                    if(o.local!=old.local || o.retired!=old.retired || o.native_session!=old.native_session ||
                       o.source_handle!=old.source_handle || o.native_sequence!=old.native_sequence ||
                       o.delivered!=old.delivered || o.requests!=old.requests){unlock_sessions();return false;}
                    ++current;
                }
            }
            return true;
        }
    public:
        [[nodiscard]] cp_status prepare(::std::span<core::checkpoint::member const> members,
            ::std::span<core::checkpoint::environment_owner const> saved)
        {
            if(saved.empty())return cp_status::malformed;
            for(auto const& m:members)
            {
                if(m.module_id==UINT64_MAX)return cp_status::malformed;
                sources_.push_back({m.previous,m.previous_epoch,m.module_id,m.module,m.authority_retired});
            }
            unsigned previous{};
            for(auto const& environment:saved)
            {
                if(!environment || environment->backend<=previous || environment->backend>2 ||
                   environment->recording!=saved.front()->recording)return cp_status::malformed;
                previous=environment->backend;backends_|=1u<<(previous-1u);saved_.push_back(environment);
            }
            auto result=current_.prepare(members);if(result!=cp_status::ready)return result;
            if(!validate_current())return cp_status::stale;
            ::std::lock_guard registry{mutex};
            for(auto const& environment:saved_)for(auto const& group:environment->groups)
            {
                if(!group.registered)continue;
                ::std::shared_ptr<session> selected;
                for(auto const& member:group.members)
                {
                    if(!member.registered)continue;
                    auto source=::std::find_if(sources_.begin(),sources_.end(),[&](auto const& s){return s.module==member.module && s.name==member.name;});
                    if(source==sources_.end())return cp_status::stale;
                    auto found=instances.find(source->instance);if(found==instances.end())return cp_status::stale;
                    auto backend=found->second->backends[environment->backend-1u];
                    if(selected && selected!=backend)return cp_status::stale;
                    selected=::std::move(backend);
                }
                if(!selected)return cp_status::stale;
                for(auto const& prior:sessions_)if(prior.owner==selected)return cp_status::malformed;
                ::std::unique_lock lock{selected->mutex,::std::try_to_lock};if(!lock.owns_lock())return cp_status::stale;
                pending_session next{};next.owner=selected;next.native_session=selected->availability_native_session;
                next.next_service_request=selected->next_service_request;next.connected=selected->connected;
                next.previous=selected->observations;next.replacement=selected->observations;
                next.lock=::std::unique_lock<::std::mutex>{selected->mutex,::std::defer_lock};
                for(auto const& observation:group.observations)
                {
                    auto found=next.replacement.find(observation.handle);if(found==next.replacement.end())return cp_status::stale;
                    auto queue=found->second.local->prepare_checkpoint(observation.local);
                    if(!queue)return cp_status::unsupported;
                    found->second.local=::std::move(queue);found->second.delivered=observation.delivered;
                }
                sessions_.push_back(::std::move(next));
            }
            return cp_status::ready;
        }
        [[nodiscard]] bool current() noexcept override
        {
            if(!validate_current())return false;
            ::std::lock_guard registry{mutex};
            if(!current_.current_locked() || !lock_sessions())return false;
            unlock_sessions();return true;
        }
        [[nodiscard]] bool publish() noexcept override
        {
            if(!validate_current())return false;
            ::std::lock_guard registry{mutex};
            if(!current_.current_locked() || !lock_sessions())return false;
            if(!current_.capabilities_.publish()){unlock_sessions();return false;}
            // Every validation/allocation precedes publication. Preserve actual
            // connection owners and monotonic request IDs; publish only private
            // local queues plus the complete current module registration map.
            for(auto& item:sessions_)item.owner->observations.swap(item.replacement);
            instances.swap(current_.replacement_);current_.published_=true;
            unlock_sessions();return true;
        }
    };
    inline core::checkpoint::status prepare_recorded_environment_handoff(
        ::std::span<core::checkpoint::member const> members,
        ::std::span<core::checkpoint::environment_owner const> saved,core::checkpoint::owner& output) noexcept
    {
        if(output)return core::checkpoint::status::malformed;
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
        {
            auto next=::uwvm2::utils::container::make_unique<recorded_environment_handoff>();auto result=next->prepare(members,saved);
            if(result==core::checkpoint::status::ready)output=::std::move(next);
            return result;
        }
        catch(...){return core::checkpoint::status::allocation_failed;}
#else
        return core::checkpoint::status::unsupported;
#endif
    }
}
