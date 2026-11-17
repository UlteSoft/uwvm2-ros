/** @file
 * @brief Complete authenticated model output for one inert robot-task proposal.
 * @details The terminal model output SHA256 covers every field, including tool,
 * argument schema, exact target alias/profile/resource, action and frame. It is
 * never a digest of arguments alone. The integrating dispatcher compares all
 * outer fields with immutable deployment policy and actual target provenance.
 * Input observations come exclusively from separately captured sealed snapshots;
 * this record has no field that can forge sensor provenance or grant authority.
 */
#pragma once
#include "buffers.h"
#include "proposal_contract.h"

namespace uwvm2::imported::ros::scene::proposals
{
    inline constexpr std::string_view envelope_magic="uwvm.ros.model.proposal";
    inline constexpr std::string_view model_output_type="ros.proposal",model_output_encoding="bytes";
    inline constexpr std::size_t maximum_envelope_bytes=8192;
    struct envelope
    {
        buffers::profile tool,schema,target_profile;
        std::string_view target_alias,frame,argument_type,encoding;
        proposal_contract::target_action action{};
        resources::handle target{};
        wire::bytes arguments;
    };
    inline bool envelope_text(std::string_view value,std::size_t maximum) noexcept
    {return !value.empty()&&value.size()<=maximum&&value.find('\0')==value.npos&&wire::utf8({reinterpret_cast<std::uint8_t const*>(value.data()),value.size()});}
    inline bool envelope_profile(buffers::profile const& value) noexcept
    {return value.identity.size<=buffers::maximum_identity&&envelope_text(value.identity.view(),buffers::maximum_identity);}
    inline bool read_profile(wire::reader& input,buffers::profile& output) noexcept
    {
        std::string_view identity;wire::bytes digest;
        if(!input.text(identity)||!envelope_text(identity,buffers::maximum_identity)||!output.identity.assign(identity)||!input.blob(digest)||digest.size()!=32)return false;
        std::copy(digest.begin(),digest.end(),output.sha256.begin());return true;
    }
    inline bool write_profile(wire::writer& output,buffers::profile const& value) noexcept
    {return envelope_profile(value)&&output.text(value.identity.view())&&output.blob(value.sha256);}
    /** @brief Decode a closed full-output record without allocation or side effects. */
    inline bool decode_envelope(wire::bytes bytes,envelope& output) noexcept
    {
        if(bytes.size()>maximum_envelope_bytes)return false;
        wire::reader input{bytes};envelope value;std::string_view magic,action;
        if(!input.text(magic)||magic!=envelope_magic||!read_profile(input,value.tool)||!read_profile(input,value.schema)||
           !input.text(value.target_alias)||!envelope_text(value.target_alias,255)||!read_profile(input,value.target_profile)||
           !input.u64(value.target)||!value.target||!input.text(action)||!input.text(value.frame)||!envelope_text(value.frame,96)||
           !input.text(value.argument_type)||!envelope_text(value.argument_type,96)||!input.text(value.encoding)||!envelope_text(value.encoding,32)||
           !input.blob(value.arguments)||value.arguments.size()>4096||!input.done())return false;
        auto found=std::find(proposal_contract::target_names.begin(),proposal_contract::target_names.end(),action);
        if(found==proposal_contract::target_names.end())return false;
        value.action=static_cast<proposal_contract::target_action>(found-proposal_contract::target_names.begin());output=value;return true;
    }
    /** @brief Encode all proposal meaning into the complete terminal model result. */
    inline bool encode_envelope(wire::writer& output,envelope const& value) noexcept
    {
        if(!proposal_contract::valid(value.action)||!value.target||!envelope_text(value.target_alias,255)||!envelope_text(value.frame,96)||
           !envelope_text(value.argument_type,96)||!envelope_text(value.encoding,32)||value.arguments.size()>4096)return false;
        return output.text(envelope_magic)&&write_profile(output,value.tool)&&write_profile(output,value.schema)&&output.text(value.target_alias)&&
            write_profile(output,value.target_profile)&&output.u64(value.target)&&output.text(proposal_contract::target_names[static_cast<std::size_t>(value.action)])&&
            output.text(value.frame)&&output.text(value.argument_type)&&output.text(value.encoding)&&output.blob(value.arguments);
    }
}
