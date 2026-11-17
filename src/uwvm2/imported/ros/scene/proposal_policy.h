/** @file
 * @brief Inert proposal policy with exact model/source/tool profiles and finite capacities.
 * @details Configuration never supplies native resource IDs or provenance epochs.
 * Runtime adapters resolve those from real typed providers. The logical profile
 * digest covers the entire canonical definition; capacities are mandatory,
 * independently named deployment restrictions. No definition grants a source,
 * arbiter or robot target. All structures are fixed-size and parsing allocates
 * nothing, opens no file and starts no worker.
 */
#pragma once
#include "proposal_envelope.h"
#include "proposal_admission.h"
#include "authority.h"

namespace uwvm2::imported::ros::scene::proposals
{
    inline constexpr std::string_view definition_magic="uwvm.ros.proposal.definition";
    inline constexpr std::size_t maximum_definition_bytes=32768;
    struct source_rule
    {
        buffers::text<255> alias,buffer_alias;
        buffers::profile profile,buffer_profile;
        buffers::text<96> frame;
    };
    struct tool_rule
    {
        buffers::profile profile,schema,target_profile;
        buffers::text<255> target_alias;
        buffers::text<96> argument_type,frame;
        buffers::text<32> encoding;
        proposal_contract::target_action action{};
        proposal_admission::effect_class effect{};
        proposal_admission::observation_policy observations{};
        std::uint64_t max_observation_age_ns{},minimum_observations{};
    };
    struct plan
    {
        buffers::profile selected,model,task;
        buffers::text<255> model_alias;
        proposal_admission::limits storage;
        std::array<source_rule,16> sources{};
        std::array<tool_rule,8> tools{};
        std::size_t source_count{},tool_count{};
        std::uint64_t captures{},workspace_bytes{},workspace_calls{};
    };
    struct arbiter_plan
    {buffers::text<255> proposal_alias,target_alias;std::uint64_t tool_mask{};};
    template<std::size_t Extent> inline bool policy_text(buffers::text<Extent> const& value,bool alias=false) noexcept
    {return value.size<=Extent&&envelope_text(value.view(),Extent)&&(!alias||identifier(value.view()));}
    inline bool valid(tool_rule const& value) noexcept
    {
        using namespace proposal_admission;
        return envelope_profile(value.profile)&&envelope_profile(value.schema)&&envelope_profile(value.target_profile)&&
            policy_text(value.target_alias,true)&&policy_text(value.argument_type)&&policy_text(value.encoding)&&
            policy_text(value.frame)&&proposal_contract::valid(value.action)&&value.max_observation_age_ns<=INT64_MAX&&
            value.minimum_observations<=8&&(value.effect==effect_class::physical||value.effect==effect_class::non_physical)&&
            (value.effect!=effect_class::physical||value.minimum_observations)&&
            (value.observations==observation_policy::latest_required||value.observations==observation_policy::bounded_age);
    }
    inline bool valid(plan const& value) noexcept
    {
        auto const& size=value.storage;
        if(!envelope_profile(value.selected)||!envelope_profile(value.model)||!envelope_profile(value.task)||!policy_text(value.model_alias,true)||
           size.proposals>16||size.argument_bytes>4096||size.observations>8||size.readers>8||size.tools>8||size.sources>16||
           size.max_lifetime_ns>INT64_MAX||size.max_accept_deadline_ns>INT64_MAX||value.source_count>size.sources||value.tool_count>size.tools||
           value.captures>1024||value.workspace_bytes>INT64_MAX||value.workspace_calls>1024) { return false; }
        for(std::size_t i{};i<value.source_count;++i)
        {
            auto const& source=value.sources[i];
            if(!policy_text(source.alias,true)||!policy_text(source.buffer_alias,true)||!envelope_profile(source.profile)||
               !envelope_profile(source.buffer_profile)||!policy_text(source.frame)) { return false; }
            for(std::size_t j{};j<i;++j)if(value.sources[j].alias==source.alias) { return false; }
        }
        for(std::size_t i{};i<value.tool_count;++i)
        {
            auto const& tool=value.tools[i];if(!valid(tool)||tool.minimum_observations>size.observations||tool.minimum_observations>value.source_count) { return false; }
            for(std::size_t j{};j<i;++j)
            {
                auto const& previous=value.tools[j];
                if(previous.profile.identity==tool.profile.identity) { return false; }
                if(previous.target_alias==tool.target_alias&&previous.target_profile!=tool.target_profile) { return false; }
                if(previous.schema.identity==tool.schema.identity&&previous.schema!=tool.schema) { return false; }
            }
        }
        return true;
    }
    inline bool matches(envelope const& value,tool_rule const& rule) noexcept
    {
        return value.tool==rule.profile&&value.schema==rule.schema&&value.target_profile==rule.target_profile&&
            value.target_alias==rule.target_alias.view()&&value.action==rule.action&&value.frame==rule.frame.view()&&
            value.argument_type==rule.argument_type.view()&&value.encoding==rule.encoding.view();
    }
    /** @brief Independent fixed-scope arbiter rules; no target resource number is configuration. */
    inline bool valid(arbiter_plan const& value,plan const& source) noexcept
    {
        if(source.tool_count>source.tools.size()||!policy_text(value.proposal_alias,true)||!policy_text(value.target_alias,true)||!value.tool_mask||value.tool_mask>>source.tool_count) { return false; }
        for(std::size_t index{};index<source.tool_count;++index)
            if((value.tool_mask&(std::uint64_t{1}<<index))&&source.tools[index].target_alias!=value.target_alias) { return false; }
        return true;
    }
    class configuration
    {
        plan prepared_;
        std::uint16_t assigned_{};
        bool profiled_{},defined_{},frozen_{};
    public:
        inline static constexpr std::array<std::string_view,11> limit_names{
            "proposals","argument.bytes","observations","readers","tools","sources","lifetime.ns","accept.deadline.ns","captures","workspace.bytes","workspace.calls"};
        /** @brief Select exact proposal/model/task profiles and one model alias. */
        error profile(buffers::profile selected,std::string_view model_alias,buffers::profile model,buffers::profile task) noexcept
        {
            if(profiled_||defined_||frozen_) { return error::conflict; }
            if(!envelope_profile(selected)||!envelope_profile(model)||!envelope_profile(task)||!identifier(model_alias)||!prepared_.model_alias.assign(model_alias)) { return error::invalid; }
            prepared_.selected=selected;prepared_.model=model;prepared_.task=task;profiled_=true;return error::success;
        }
        /** @brief Set each capacity exactly once; zero never enables an operation. */
        error limit(std::string_view name,std::uint64_t value) noexcept
        {
            if(frozen_) { return error::conflict; }
            auto found=std::find(limit_names.begin(),limit_names.end(),name);if(found==limit_names.end()) { return error::invalid; }
            auto index=static_cast<std::size_t>(found-limit_names.begin());auto bit=std::uint16_t{1}<<index;if(assigned_&bit) { return error::conflict; }
            constexpr std::array<std::uint64_t,11> maximum{16,4096,8,8,8,16,INT64_MAX,INT64_MAX,1024,INT64_MAX,1024};
            if(value>maximum[index]) { return error::too_large; }
            auto& size=prepared_.storage;
            switch(index)
            {
            case 0:size.proposals=static_cast<std::size_t>(value);break;
            case 1:size.argument_bytes=static_cast<std::size_t>(value);break;
            case 2:size.observations=static_cast<std::size_t>(value);break;
            case 3:size.readers=static_cast<std::size_t>(value);break;
            case 4:size.tools=static_cast<std::size_t>(value);break;
            case 5:size.sources=static_cast<std::size_t>(value);break;
            case 6:size.max_lifetime_ns=value;break;
            case 7:size.max_accept_deadline_ns=value;break;
            case 8:prepared_.captures=value;break;
            case 9:prepared_.workspace_bytes=value;break;
            case 10:prepared_.workspace_calls=value;break;
            }
            assigned_|=bit;return error::success;
        }
        /** @brief Parse exact source and tool rules after authenticating the whole record.
         * @details Fields are magic/identity/model alias/model profile/task profile,
         * source array (alias/profile/buffer alias/buffer profile/frame), then
         * tool array (tool/schema/target profiles, target alias, action, effect,
         * observation policy, argument type, encoding, frame, max age, minimum
         * observations). Every string is bounded; no native symbols or handles
         * can be expressed. Effects are physical/non_physical and observation
         * modes are latest_required/bounded_age. Unknown words fail closed.
         */
        error definition(wire::bytes bytes) noexcept
        {
            if(defined_||frozen_) { return error::conflict; }if(!profiled_) { return error::denied; }
            if(bytes.size()>maximum_definition_bytes) { return error::too_large; }
            if(buffers::hash(bytes)!=prepared_.selected.sha256) { return error::conflict; }
            auto next=prepared_;wire::reader input{bytes};std::string_view magic,identity,model;buffers::profile selected_model,selected_task;
            if(!input.text(magic)||magic!=definition_magic||!input.text(identity)||identity!=prepared_.selected.identity.view()||!input.text(model)||
               model!=prepared_.model_alias.view()||!read_profile(input,selected_model)||selected_model!=prepared_.model||
               !read_profile(input,selected_task)||selected_task!=prepared_.task) { return error::invalid; }
            std::uint64_t count{};if(!input.u64(count)||count>16) { return error::invalid; }next.source_count=static_cast<std::size_t>(count);
            for(std::size_t index{};index<next.source_count;++index)
            {
                auto& source=next.sources[index];std::string_view alias,buffer,frame;
                if(!input.text(alias)||!source.alias.assign(alias)||!read_profile(input,source.profile)||!input.text(buffer)||!source.buffer_alias.assign(buffer)||
                   !read_profile(input,source.buffer_profile)||!input.text(frame)||!source.frame.assign(frame)) { return error::invalid; }
            }
            if(!input.u64(count)||count>8) { return error::invalid; }next.tool_count=static_cast<std::size_t>(count);
            for(std::size_t index{};index<next.tool_count;++index)
            {
                auto& tool=next.tools[index];std::string_view alias,action,effect,observations,type,encoding,frame;
                if(!read_profile(input,tool.profile)||!read_profile(input,tool.schema)||!read_profile(input,tool.target_profile)||!input.text(alias)||!tool.target_alias.assign(alias)||
                   !input.text(action)||!input.text(effect)||!input.text(observations)||!input.text(type)||!tool.argument_type.assign(type)||!input.text(encoding)||
                   !tool.encoding.assign(encoding)||!input.text(frame)||!tool.frame.assign(frame)||!input.u64(tool.max_observation_age_ns)||!input.u64(tool.minimum_observations)) { return error::invalid; }
                auto found=std::find(proposal_contract::target_names.begin(),proposal_contract::target_names.end(),action);if(found==proposal_contract::target_names.end()) { return error::invalid; }
                tool.action=static_cast<proposal_contract::target_action>(found-proposal_contract::target_names.begin());
                tool.effect=effect=="physical"?proposal_admission::effect_class::physical:effect=="non_physical"?proposal_admission::effect_class::non_physical:proposal_admission::effect_class::unclassified;
                tool.observations=observations=="latest_required"?proposal_admission::observation_policy::latest_required:
                    observations=="bounded_age"?proposal_admission::observation_policy::bounded_age:proposal_admission::observation_policy::unconfigured;
            }
            if(!input.done()) { return error::invalid; }
            // Definition and capacities may arrive in either order; freeze is the publication point.
            auto structural=next;structural.storage={16,4096,8,8,8,16,INT64_MAX,INT64_MAX};
            if(!valid(structural)) { return error::invalid; }prepared_=next;defined_=true;return error::success;
        }
        error freeze(plan& output) noexcept
        {
            if(frozen_) { return error::conflict; }
            if(!profiled_||!defined_||assigned_!=((std::uint16_t{1}<<limit_names.size())-1)||!valid(prepared_)) { return error::invalid; }
            output=prepared_;frozen_=true;return error::success;
        }
    };
}
