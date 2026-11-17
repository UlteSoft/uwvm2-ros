/** @file
 * @brief Explicit directed bridge semantics and finite retained-message capacities.
 * @details The authenticated definition selects aliases and compiled canonical
 * codecs, never native topics or sockets. Existing native endpoint definitions
 * independently choose their ROS names, types, workers and isolation policies.
 * Every normal/stop sequence and at most one in-flight message is retained in
 * preallocated storage. No configuration field supplies a guest capability.
 */
#pragma once
#include "bridge_dispatch.h"
namespace uwvm2::imported::ros::scene
{
    class bridge_configuration
    {
        bridge_dispatch::plan policy_;unsigned assigned_{};bool selected_{},defined_{};
    public:
        inline static constexpr std::string_view magic="uwvm.ros.bridge.topic";
        inline static constexpr std::array<std::string_view,3> names{"message.bytes","retained.bytes","deadline.ns"};
        error profile(std::string_view alias,buffers::profile selected)noexcept
        {
            if(selected_)return error::conflict;
            if(!identifier(alias)||!proposals::envelope_profile(selected))return error::invalid;
            policy_.alias.assign(alias);policy_.selected=selected;selected_=true;return error::success;
        }
        bool definition_pending()const noexcept{return selected_&&!defined_;}
        /** @brief Authenticate magic, identity, source alias/profile, target alias/profile, stop profile, codec. */
        error definition(wire::bytes bytes)noexcept
        {
            if(!selected_)return error::denied;
            if(defined_)return error::conflict;
            if(bytes.size()>4096||buffers::hash(bytes)!=policy_.selected.sha256)return error::invalid;
            auto value=policy_;wire::reader in{bytes};std::string_view name,identity,from,to,codec;
            if(!in.text(name)||name!=magic||!in.text(identity)||identity!=value.selected.identity.view()||
                !in.text(from)||!identifier(from)||!proposals::read_profile(in,value.source_profile)||
                !in.text(to)||!identifier(to)||!proposals::read_profile(in,value.destination_profile)||
                !proposals::read_profile(in,value.stop)||!in.text(codec)||!value.codec.assign(codec)||!in.done())return error::invalid;
            value.source_alias.assign(from);value.destination_alias.assign(to);auto checked=value;checked.message_bytes=1;checked.deadline_ns=1;std::uint64_t required{};
            if(!bridge_dispatch::required_storage(checked,required))return error::invalid;
            policy_=value;defined_=true;return error::success;
        }
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(!defined_)return error::denied;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end())return error::invalid;
            auto index=static_cast<unsigned>(found-names.begin());if(assigned_&(1u<<index))return error::conflict;
            if(value>INT64_MAX||(index==0&&value>buffers::maximum_transfer))return error::too_large;
            std::array fields{&policy_.message_bytes,&policy_.storage_bytes,&policy_.deadline_ns};*fields[index]=value;assigned_|=1u<<index;return error::success;
        }
        error freeze(bridge_dispatch::plan& output)const noexcept
        {
            if(!defined_||assigned_!=(1u<<names.size())-1)return error::invalid;
            std::uint64_t required{};if(!bridge_dispatch::required_storage(policy_,required)||required>policy_.storage_bytes)return error::invalid;
            output=policy_;return error::success;
        }
    };
}
