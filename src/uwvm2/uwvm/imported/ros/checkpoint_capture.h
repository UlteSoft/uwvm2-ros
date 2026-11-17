// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
// Stable v1 local environment capture. Never makes an RPC or creates a native
// registration. Native transport credentials are intentionally not copied.
namespace uwvm2::uwvm::imported::ros::host
{
    inline core::checkpoint::status capture_checkpoint_environment(
        ::std::span<core::checkpoint::source_member const> members,
        ::std::array<::std::byte,16u> const& recording,unsigned backends,
        ::uwvm2::utils::container::std_vector<core::checkpoint::environment_owner>& output) noexcept
    {
        namespace cp=core::checkpoint;
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
        try
        {
            if(!output.empty() || !backends || backends>3 || members.empty())return cp::status::malformed;
            ::std::unique_lock registry{mutex,::std::try_to_lock};
            if(!registry.owns_lock())return cp::status::stale;
            ::uwvm2::utils::container::std_set<void const*,::std::less<>> identities;
            ::uwvm2::utils::container::std_set<::std::uint64_t> numbers;
            ::uwvm2::utils::container::std_set<::std::string_view> names;
            for(auto const& source:members)
                if(!source.instance || !source.epoch || !identities.insert(source.instance).second ||
                   !numbers.insert(source.module).second || !names.insert(source.name).second)
                    return cp::status::malformed;
            for(auto const& [key,value]:instances)
            {
                auto found=::std::find_if(members.begin(),members.end(),[&](auto const& m){return m.instance==key;});
                if(found==members.end() || found->epoch!=value->generation || found->retired!=value->retired)
                    return cp::status::stale;
            }
            ::uwvm2::utils::container::std_vector<cp::environment_owner> result;
            for(unsigned backend=1;backend<=2;++backend)
            {
                if(!(backends&(1u<<(backend-1u))))continue;
                auto env=::uwvm2::utils::container::make_shared<cp::data::environment>();env->recording=recording;env->backend=backend;
                using key_type=::std::pair<unsigned,::uwvm2::utils::container::std_string>;
                ::uwvm2::utils::container::std_map<key_type,::uwvm2::utils::container::std_vector<cp::source_member>> groups;
                for(auto const& source:members)
                {
                    auto target=runtime_policy.effective(static_cast<core::version>(backend),source.name);
                    groups[{target?static_cast<unsigned>(target->target_scope):0u,target?target->name: ::uwvm2::utils::container::std_string{}}].push_back(source);
                }
                for(auto& [key,sources]:groups)
                {
                    ::std::sort(sources.begin(),sources.end(),[](auto const& a,auto const& b){return a.module<b.module;});
                    cp::data::group group{};group.scope=key.first;group.name=key.second;
                    auto target=runtime_policy.effective(static_cast<core::version>(backend),sources.front().name);
                    if(target)
                    {
                        group.enabled=target->enabled;
                        for(auto const& [k,v]:target->connection)group.connection.push_back({k,v});
                        namespace hi=::uwvm2::utils::control::host_interfaces;
                        for(::std::size_t i{};i<hi::catalogue.size();++i)
                        {
                            auto const& entry=hi::catalogue[i];
                            auto const family=backend==1?hi::family::ros1:hi::family::ros2;
                            if(entry.group!=family || entry.name_space.find("_v1")==::std::string_view::npos)continue;
                            group.interfaces.push_back({::uwvm2::utils::container::std_string{entry.key},::uwvm2::utils::container::concat(static_cast<unsigned>(target->interfaces.entries[i]))});
                        }
                        ::std::sort(group.interfaces.begin(),group.interfaces.end(),[](auto const& a,auto const& b){return a.name<b.name;});
                        for(auto const& [name,cap]:target->capabilities)
                        {
                            cp::data::capability c{};c.alias=name;c.operation=cap.operation;c.resource=cap.resource;
                            c.type=cap.type;c.type_identity=cap.type_identity;
                            for(auto const& [k,v]:cap.limits)c.limits.push_back({k,v});
                            group.capabilities.push_back(::std::move(c));
                        }
                    }
                    ::std::shared_ptr<session> selected;
                    for(auto const& source:sources)
                    {
                        auto found=instances.find(source.instance);auto registered=target && found!=instances.end();
                        group.members.push_back({source.module,::uwvm2::utils::container::std_string{source.name},registered,source.retired});
                        if(!registered)continue;
                        auto current=found->second->backends[backend-1u];
                        if(current->target!=target || (selected && selected!=current))return cp::status::stale;
                        selected=::std::move(current);
                    }
                    auto status=target?capabilities.copy_checkpoint_backend(sources,static_cast<core::version>(backend),group):cp::status::ready;
                    if(status!=cp::status::ready)return status;
                    if(group.registered!=bool(selected))return cp::status::stale;
                    if(selected)
                    {
                        ::std::unique_lock lock{selected->mutex,::std::try_to_lock};
                        if(!lock.owns_lock())return cp::status::stale;
                        auto const& s=*selected;
                        if(!s.rmw_endpoints.rows().empty() || !s.rmw_service_pairs.rows().empty() || !s.rmw_service_journals.empty())
                            return cp::status::unsupported;
                        group.session_present=true;group.connected=s.connected;group.timeout=s.timeout;
                        group.next_service_request=s.next_service_request;group.session_label=s.availability_debug_session;
                        group.availability_sealed=s.availability_history.sealed();
                        for(auto const& [handle,o]:s.observations)
                        {
                            cp::data::observation row{};row.handle=handle;row.source_handle=o.source_handle;
                            row.correlation=o.correlation;row.native_sequence=o.native_sequence;row.delivered=o.delivered;
                            row.observer_alias=o.observer.alias;row.source_alias=o.source.alias;row.retired=o.retired;
                            row.current_session=o.native_session==s.availability_native_session;
                            row.requests.assign(o.requests.begin(),o.requests.end());
                            if(!o.local || !o.local->copy_checkpoint(row.local))return cp::status::stale;
                            group.observations.push_back(::std::move(row));
                        }
                        for(auto const& [handle,a]:s.availability_history.rows())
                            group.availability_history.push_back({handle,a.binding.source,a.availability_alias,a.source_alias,
                                a.retired,a.source_retired,a.availability_revoked,a.source_revoked,a.native_session==s.availability_native_session});
                    }
                    env->groups.push_back(::std::move(group));
                }
                result.push_back(::std::move(env));
            }
            output.swap(result);return cp::status::ready;
        }
        catch(...) {return cp::status::allocation_failed;}
#else
        return cp::status::unsupported;
#endif
    }
}
