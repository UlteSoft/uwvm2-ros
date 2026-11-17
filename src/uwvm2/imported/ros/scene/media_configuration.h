/** @file
 * @brief Digest-bound raw media configuration with explicit independent capacities.
 * @details Selection, authenticated semantics and every capacity are distinct
 * trusted deployment steps. Parsing neither allocates media frames nor opens
 * files, sockets or plugins. No defaults enable streams, queues or workspace.
 */
#pragma once
#include "media_dispatch.h"
namespace uwvm2::imported::ros::scene
{
    class media_configuration
    {
        media_dispatch::plan policy_;unsigned assigned_{};bool selected_{},defined_{};
    public:
        inline static constexpr std::string_view magic="uwvm.ros.media.raw";
        inline static constexpr std::array<std::string_view,6> names{
            "streams","frames","retained.bytes","workspace.bytes","workspace.calls","deadline.ns"};
        /** @brief Select a logical profile before reading its exact canonical definition. */
        error profile(std::string_view alias,buffers::profile selected)noexcept
        {
            if(selected_) { return error::conflict; }
            if(!identifier(alias)||!proposals::envelope_profile(selected)) { return error::invalid; }
            policy_.alias.assign(alias);policy_.selected=selected;selected_=true;return error::success;
        }
        bool definition_pending()const noexcept{return selected_&&!defined_;}
        /** @brief Authenticate exact aliases/profiles/dimensions and the compiled copy or RGB swap codec.
         * @details Canonical fields: magic, identity, input alias/profile,
         * output alias/profile, codec, input/output color, width,height,channels.
         * Width/height/channels must describe packed mono or RGB/BGR uint8 data.
         * Input and output may share a grant only if their exact profiles match.
         */
        error definition(wire::bytes bytes)noexcept
        {
            if(!selected_) { return error::denied; }if(defined_) { return error::conflict; }
            if(bytes.size()>4096||buffers::hash(bytes)!=policy_.selected.sha256) { return error::invalid; }
            auto value=policy_;wire::reader in{bytes};std::string_view name,identity,input,output,codec,color;
            if(!in.text(name)||name!=magic||!in.text(identity)||identity!=value.selected.identity.view()||
                !in.text(input)||!identifier(input)||!proposals::read_profile(in,value.input_profile)||
                !in.text(output)||!identifier(output)||!proposals::read_profile(in,value.output_profile)||!in.text(codec))return error::invalid;
            if(codec=="copy")value.transform=media_dispatch::codec::copy;
            else if(codec=="swap_red_blue")value.transform=media_dispatch::codec::swap_red_blue;else return error::invalid;
            auto read_color=[&](buffers::color_space& result)
            {if(!in.text(color)) { return false; }auto found=std::find(buffers::color_names.begin(),buffers::color_names.end(),color);
                if(found==buffers::color_names.end()) { return false; }result=static_cast<buffers::color_space>(found-buffers::color_names.begin());return true;};
            if(!read_color(value.input_color)||!read_color(value.output_color)||!in.u64(value.width)||!in.u64(value.height)||!in.u64(value.channels)||!in.done()) { return error::invalid; }
            value.input_alias.assign(input);value.output_alias.assign(output);
            if(input==output&&value.input_profile!=value.output_profile) { return error::invalid; }
            auto checked=value;checked.deadline_ns=1;std::uint64_t needed{};
            if(!media_dispatch::required_storage(checked,needed)) { return error::invalid; }
            policy_=value;defined_=true;return error::success;
        }
        /** @brief Require all six finite values once; explicit zero reserves no capacity. */
        error limit(std::string_view name,std::uint64_t value)noexcept
        {
            if(!defined_) { return error::denied; }
            auto found=std::find(names.begin(),names.end(),name);if(found==names.end()) { return error::invalid; }
            auto index=static_cast<unsigned>(found-names.begin());if(assigned_&(1u<<index)) { return error::conflict; }
            if(value>INT64_MAX||(index==4&&value>1024)||(index==5&&!value)) { return error::too_large; }
            std::array fields{&policy_.streams,&policy_.frames,&policy_.storage_bytes,&policy_.workspace_bytes,&policy_.workspace_calls,&policy_.deadline_ns};
            *fields[index]=value;assigned_|=1u<<index;return error::success;
        }
        /** @brief Freeze only complete finite storage; never infer input/output grants. */
        error freeze(media_dispatch::plan& output)const noexcept
        {
            if(!defined_||assigned_!=(1u<<names.size())-1) { return error::invalid; }
            std::uint64_t required{};if(!media_dispatch::required_storage(policy_,required)||required>policy_.storage_bytes) { return error::too_large; }
            output=policy_;return error::success;
        }
    };
}
