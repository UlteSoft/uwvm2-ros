// Test-only historical process owner. Never include or link into the VM product.
/** @file
 * @brief Digest-bound component roles, immutable configuration and finite IPC.
 * @details Roles expose only one typed topic direction. Publishers never receive;
 * subscribers explicitly reserve and settle messages. Every native profile,
 * canonical codec and configuration generation is fixed before activation.
 * A component has one native incarnation: automatic restart, shared address
 * spaces and in-VM loading are deliberately absent from this binding.
 */
#pragma once
#include <uwvm2/imported/ros/scene/communication_codecs.h>
#include <uwvm2/imported/ros/scene/proposal_policy.h>
#include "model_worker_manifest.h"
namespace uwvm2::imported::ros::scene::components
{
    struct role
    {buffers::text<255> alias;buffers::profile selected;buffers::text<32> codec;bool publisher{};};
    struct plan
    {
        buffers::text<255> alias,configuration_alias,configuration_identity;
        buffers::profile selected,stop;buffers::digest configuration_digest{};
        std::uint64_t configuration_generation{},configuration_bytes{},ipc_bytes{},message_bytes{},deadline_ns{},lifetime_ns{},shutdown_ns{},retained_bytes{};
        std::array<role,16> roles{};std::size_t role_count{};
    };
    inline bool valid(plan const& p)noexcept
    {
        if(!identifier(p.alias.view())||!identifier(p.configuration_alias.view())||!proposals::envelope_text(p.configuration_identity.view(),255)||
            !proposals::envelope_profile(p.selected)||!proposals::envelope_profile(p.stop)||!p.configuration_generation||
            p.ipc_bytes<4096||p.ipc_bytes>1048576||p.configuration_bytes>p.ipc_bytes-4096||p.message_bytes>p.ipc_bytes-4096||
            !p.deadline_ns||p.deadline_ns>INT64_MAX||!p.lifetime_ns||p.lifetime_ns>INT64_MAX||p.deadline_ns>p.lifetime_ns||
            !p.shutdown_ns||p.shutdown_ns>UINT64_C(5000000000)||p.retained_bytes>INT64_MAX||p.role_count>p.roles.size())return false;
        for(std::size_t i{};i<p.role_count;++i)
        {
            auto const& r=p.roles[i];if(!identifier(r.alias.view())||!proposals::envelope_profile(r.selected)||!communication::codecs::find(r.codec.view()))return false;
            for(std::size_t j{};j<i;++j)if(p.roles[j].alias==r.alias)return false;
        }
        return true;
    }
    /** @brief Inert definition and exact image closure; no native I/O occurs while parsing. */
    class configuration
    {
        plan policy_;unsigned assigned_{};bool selected_{},defined_{};
    public:
        model_worker::configuration images;
        inline static constexpr std::string_view magic="uwvm.ros.component.roles";
        inline static constexpr std::array<std::string_view,7> names{"configuration.bytes","ipc.bytes","message.bytes","deadline.ns","lifetime.ns","shutdown.ns","retained.bytes"};
        error profile(std::string_view alias,buffers::profile selected)noexcept
        {
            if(selected_)return error::conflict;
            if(!identifier(alias)||!proposals::envelope_profile(selected))return error::invalid;
            policy_.alias.assign(alias);policy_.selected=selected;selected_=true;return error::success;
        }
        bool definition_pending()const noexcept{return selected_&&!defined_;}
        /** @brief Verify the full semantic definition before accepting any capacity assignment. */
        error definition(wire::bytes bytes)noexcept
        {
            if(!selected_)return error::denied;
            if(defined_)return error::conflict;
            if(bytes.size()>16384||buffers::hash(bytes)!=policy_.selected.sha256)return error::invalid;
            auto value=policy_;wire::reader in{bytes};std::string_view kind,identity,alias,resource;wire::bytes digest;std::uint64_t count{};
            if(!in.text(kind)||kind!=magic||!in.text(identity)||identity!=value.selected.identity.view()||!proposals::read_profile(in,value.stop)||
                !in.text(alias)||!identifier(alias)||!in.text(resource)||!proposals::envelope_text(resource,255)||!in.u64(value.configuration_generation)||
                !value.configuration_generation||!in.blob(digest)||digest.size()!=32||!in.u64(count)||count>value.roles.size())return error::invalid;
            value.configuration_alias.assign(alias);value.configuration_identity.assign(resource);std::copy(digest.begin(),digest.end(),value.configuration_digest.begin());
            value.role_count=static_cast<std::size_t>(count);
            for(std::size_t i{};i<value.role_count;++i)
            {
                auto& role=value.roles[i];std::string_view name,codec;
                if(!in.text(name)||!identifier(name)||!proposals::read_profile(in,role.selected)||!in.boolean(role.publisher)||
                    !in.text(codec)||!communication::codecs::find(codec))return error::invalid;
                role.alias.assign(name);role.codec.assign(codec);
                for(std::size_t j{};j<i;++j)if(value.roles[j].alias==role.alias)return error::invalid;
            }
            if(!in.done())return error::invalid;
            policy_=value;defined_=true;return error::success;
        }
        /** @brief Assign explicit finite storage/deadline resources once; optional rates stay in authority. */
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(!defined_)return error::denied;
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end()||value>INT64_MAX)return error::invalid;
            auto index=static_cast<unsigned>(found-names.begin());if(assigned_&(1u<<index))return error::conflict;
            std::array fields{&policy_.configuration_bytes,&policy_.ipc_bytes,&policy_.message_bytes,&policy_.deadline_ns,&policy_.lifetime_ns,&policy_.shutdown_ns,&policy_.retained_bytes};
            *fields[index]=value;assigned_|=1u<<index;return error::success;
        }
        /** @brief Freeze pure-message policy and required platform image limits without opening files. */
        error freeze(plan& output,model_worker::configuration& prepared)const
        {
            if(!defined_||assigned_!=(1u<<names.size())-1||!valid(policy_))return error::invalid;
            auto copy=images;auto status=copy.freeze_component(policy_.selected,policy_.stop);if(status!=error::success)return status;
            auto limits=copy.limits();if(limits.request_bytes!=policy_.ipc_bytes||limits.response_bytes!=policy_.ipc_bytes||limits.poll_ns>policy_.shutdown_ns/2||
                limits.startup_ns>policy_.lifetime_ns)return error::invalid;
            output=policy_;prepared=std::move(copy);return error::success;
        }
    };
}
