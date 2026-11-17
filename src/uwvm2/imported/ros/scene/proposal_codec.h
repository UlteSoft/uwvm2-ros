/** @file @brief Canonical proposal records; authority and provenance remain host checks. */
#pragma once
#include "proposals.h"
#include "model_contract.h"

namespace uwvm2::imported::ros::proposal_contract
{
    inline constexpr std::size_t maximum_request_bytes=16384,maximum_capture_sources=8;
    struct capture_source{std::uint64_t index{},capability{},resource{},buffer_capability{};};
    struct capture_request
    {scene::proposal_admission::request_key key;scene::buffers::profile selected;std::array<capture_source,maximum_capture_sources> sources{};std::size_t count{};std::uint64_t expiry{};};
    struct publish_request
    {scene::proposal_admission::request_key key;std::uint64_t capture{},model_request{},expiry{};scene::wire::bytes envelope;};
    struct captured_buffer{std::uint64_t capability{},buffer{};};
    struct capture_receipt{std::uint64_t capture{};std::array<captured_buffer,maximum_capture_sources> inputs{};std::size_t count{};};
    /** @brief Canonical bounded capture request; no guest timestamp/provenance fields exist. */
    inline bool decode_capture(scene::wire::bytes input,capture_request& output)noexcept
    {
        if(input.size()>maximum_request_bytes)return false;
        scene::wire::reader reader{input};capture_request value;std::uint64_t count{};
        if(!reader.u64(value.key.session)||!value.key.session||!reader.u64(value.key.sequence)||!value.key.sequence||
            !scene::proposals::read_profile(reader,value.selected)||!reader.u64(count)||count>maximum_capture_sources)return false;
        value.count=static_cast<std::size_t>(count);
        for(std::size_t index{};index<value.count;++index)
        {auto& source=value.sources[index];if(!reader.u64(source.index)||source.index>=16||!reader.u64(source.capability)||!source.capability||
            !reader.u64(source.resource)||!source.resource||!reader.u64(source.buffer_capability)||!source.buffer_capability)return false;}
        if(!reader.u64(value.expiry)||!value.expiry||value.expiry>INT64_MAX||!reader.done())return false;
        output=value;return true;
    }
    /** @brief Publication supplies complete terminal bytes, never only an argument digest. */
    inline bool decode_publish(scene::wire::bytes input,publish_request& output)noexcept
    {
        if(input.size()>maximum_request_bytes)return false;
        scene::wire::reader reader{input};publish_request value;
        if(!reader.u64(value.key.session)||!value.key.session||!reader.u64(value.key.sequence)||!value.key.sequence||!reader.u64(value.capture)||!value.capture||
            !reader.u64(value.model_request)||!value.model_request||!reader.blob(value.envelope)||value.envelope.empty()||
            value.envelope.size()>scene::proposals::maximum_envelope_bytes||!reader.u64(value.expiry)||!value.expiry||value.expiry>INT64_MAX||!reader.done())return false;
        output=value;return true;
    }
    inline bool encode_capture(scene::wire::writer& output,capture_receipt const& value)noexcept
    {
        if(value.count>maximum_capture_sources||!output.u64(value.capture)||!output.u64(value.count))return false;
        for(std::size_t index{};index<value.count;++index)if(!output.u64(value.inputs[index].capability)||!output.u64(value.inputs[index].buffer))return false;
        return true;
    }
    /** @brief Decode the existing fixed request; no capability is granted. */
    inline bool decode(operation action, scene::wire::bytes bytes, scene::proposal_admission::accept_request& output) noexcept
    {
        scene::wire::reader input{bytes};
        scene::proposal_admission::accept_request value;
        if(action == operation::read)
        {
            if(!input.u64(value.proposal)) { return false; }
        }
        else if(action == operation::accept)
        {
            if(!input.u64(value.key.session) || !input.u64(value.key.sequence) || !input.u64(value.proposal_capability) || !input.u64(value.proposal) ||
               !input.u64(value.target_capability) || !input.u64(value.target) || !input.u64(value.deadline))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        if(!input.done()) { return false; }
        output = value;
        return true;
    }

    /** @brief Encode the original captured provenance, never a newer sensor sample. */
    inline bool encode(scene::wire::writer& output,
                       scene::proposals::core::record const& record,
                       scene::proposals::tool_profile const& tool,
                       std::span<std::shared_ptr<scene::proposals::source_snapshot const> const> inputs) noexcept
    {
        if(record.argument_size > record.arguments.size() || inputs.size() != record.observation_count || !output.u64(record.model_request) ||
           !model_contract::write_profile(output, tool.profile) || !output.text(tool.argument_type.view()) || !output.text(tool.encoding.view()) ||
           !output.blob({record.arguments.data(), record.argument_size}) || !output.u64(inputs.size()))
        {
            return false;
        }
        for(std::size_t i{}; i != inputs.size(); ++i)
        {
            if(!inputs[i] || inputs[i]->metadata != record.observations[i]) { return false; }
            auto const& value = *inputs[i];
            auto const& source = value.metadata;
            if(!output.u64(source.capability) || !output.u64(source.buffer) || !output.u64(source.source) || !output.u64(source.source_generation) ||
               !output.boolean(true) || !output.u64(source.clock) || !output.u64(source.epoch) || !output.i64(source.source_ns) || !output.boolean(true) ||
               !output.text(value.frame.view()))
            {
                return false;
            }
        }
        return output.u64(record.expiry);
    }

    /** @brief Encode historical local admission, never physical completion. */
    inline bool encode_receipt(scene::wire::writer& output, scene::proposal_admission::receipt const& receipt) noexcept
    {
        return output.u64(receipt.request) && output.text(receipt_state) && output.text(receipt_effect) && output.u64(receipt.provider_generation) &&
               output.text({});
    }
}  // namespace uwvm2::imported::ros::proposal_contract
