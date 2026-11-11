/*************************************************************
 * UWVM2 cold configured ROS policy debugger DATA. APL-2.0. *
 *************************************************************/
#pragma once
#include "policy.h"
#include "../common/debug.h"

namespace uwvm2::imported::ros
{
    inline void debug_policy_snapshot(policy_store const& store,debug::request const& query,debug::view& output) noexcept
    {
        namespace data=debug;
        namespace api=::uwvm2::utils::control::host_interfaces;
        output={};output.selected=query.selected;
        if(!data::valid(query)||query.selected!=data::selection::policy)
        {output.result=data::status::invalid_request;return;}
        output.result=data::status::ok;
        auto fail=[&]() noexcept
        {output={};output.selected=query.selected;output.result=data::status::resource_limit;};
        auto scope_of=[](scope value) noexcept
        {
            return value==scope::global?data::policy_scope::global:
                value==scope::single?data::policy_scope::single:data::policy_scope::group;
        };
        auto selected=[&](target_policy const& target) noexcept
        {
            return (!query.backend||query.backend==static_cast<unsigned>(target.ros_version))&&
                (query.scope==data::policy_scope::all||query.scope==scope_of(target.target_scope))&&
                (query.target.size==0||query.target.view()==target.name)&&
                (query.module.size==0||store.effective(target.ros_version,query.module.view())==&target);
        };
        auto add=[&](data::policy_kind kind,target_policy const& target,::std::string_view module,
            ::std::string_view alias,::std::string_view key,::std::string_view value,bool configured=true,bool redacted=false) noexcept
        {
            // Validated identities stay complete. Unsupported configured
            // type strings may be longer, so stream their exact bytes in
            // explicitly numbered chunks without an inventory/scan ceiling.
            auto const chunks{value.empty()?::std::uint64_t{1}:static_cast<::std::uint64_t>((value.size()-1)/1024+1)};
            if(target.name.size()>1024||module.size()>1024||alias.size()>255||key.size()>192||chunks>UINT64_MAX-output.total)
            {fail();return false;}
            auto const begin{output.total};output.total+=chunks;
            auto index{query.first>begin?query.first-begin:0};
            for(;index<chunks;++index)
            {
                if(output.policy_size>=query.count) {output.more=true;break;}
                auto const position{begin+index};auto const offset{static_cast<::std::size_t>(index)*1024};
                auto& row{output.policy[output.policy_size++]};
                row.kind=kind;row.scope=scope_of(target.target_scope);row.backend=static_cast<unsigned>(target.ros_version);
                row.target.assign(target.name);row.module.assign(module);row.alias.assign(alias);row.key.assign(key);row.value.assign(value.substr(offset,1024));
                row.value_offset=offset;row.value_total=value.size();row.configured=configured;row.enabled=target.enabled;row.redacted=redacted;output.next=position+1;
            }
            return true;
        };
        // Stable map order, independent of live handles, counters or aliases.
        // Module and alias filters retain target/interface context. A module
        // selects its actual effective target; only explicit bindings are rows.
        if(!store.visit_configured_targets([&](target_policy const& target) noexcept
        {
            return !selected(target)||add(data::policy_kind::target,target,{},{},
                "enabled",target.enabled?"true":"false");
        })) {return;}
        if(!store.visit_configured_bindings([&](version backend,::std::string_view module,scope kind,::std::string_view name) noexcept
        {
            auto const* target{store.find_target(backend,kind,name)};
            if(!target) {fail();return false;}
            return !selected(*target)||(query.module.size&&query.module.view()!=module)||
                add(data::policy_kind::binding,*target,module,{},"binding","explicit");
        })) {return;}
        if(!store.visit_configured_targets([&](target_policy const& target) noexcept
        {
            if(!selected(target)) {return true;}
            auto const family{target.ros_version==version::ros1?api::family::ros1:api::family::ros2};
            for(::std::size_t i{};i<api::catalogue.size();++i)
            {
                if(api::catalogue[i].group!=family) {continue;}
                if(!add(data::policy_kind::interface,target,{},{},api::catalogue[i].key,
                    api::name(target.interfaces.entries[i]),target.interfaces.configured[i])) {return false;}
            }
            for(auto const& [alias,cap]:target.capabilities)
            {
                if(query.alias.size&&query.alias.view()!=alias) {continue;}
                if(!add(data::policy_kind::capability,target,{},alias,"operation",cap.operation)||
                   !add(data::policy_kind::capability,target,{},alias,"resource",cap.resource)||
                   !add(data::policy_kind::capability,target,{},alias,"type",cap.type)||
                   !add(data::policy_kind::capability,target,{},alias,"type_identity",cap.type_identity)) {return false;}
                for(auto const& [key,value]:cap.limits)
                {if(!add(data::policy_kind::limit,target,{},alias,key,value)) {return false;}}
            }
            for(auto const& [key,value]:target.connection)
            {
                // Controls and quotas are policy DATA. Host connection
                // addresses, security roots/identities and paths stay hidden.
                bool const public_control{key=="connect-timeout-ms"||key=="connect-timeout-ms.on-proc-exit"||key=="call-boundary"||
                    key=="audit-deny-max-rate-hz"||key=="audit-deny-burst"||key=="audit-deny-max-records"||key=="audit-deny-max-bytes"};
                if(!add(data::policy_kind::connection,target,{},{},key,public_control ? ::std::string_view{value} : ::std::string_view{},true,!public_control))
                {return false;}
            }
            return true;
        })) {return;}
    }
}
