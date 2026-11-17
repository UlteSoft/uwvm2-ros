/** @file
 * @brief Closed validation of an isolated worker's retained receive reply.
 * @details All identities are compared with an independently retained binding
 * and actual worker generation before payload validation. The decoder allocates
 * nothing, changes no state and returns borrowed views into owned reply storage.
 * Native correlations remain explicitly native: a host adapter must resolve its
 * finite correlation map before exposing them to Wasm. Discovery/publisher bytes
 * are observations, never authentication or grants. This protocol has no trusted
 * clock mapping, so it rejects worker-invented timestamp/epoch claims.
 */
#pragma once
#include "communication_receive_contract.h"
#include "communication_validation.h"
namespace uwvm2::imported::ros::scene::communication_worker
{
    namespace receive_wire=::uwvm2::imported::ros::communication_receive_contract;
    enum class receive_direction:std::uint8_t{topic,service_request,service_response};
    struct receive_schema
    {
        std::uint64_t native_endpoint{},provider_generation{},payload_bytes{};
        receive_direction direction{};
        std::string_view type,encoding;
        communication::message_validator validate;
    };
    /** @brief Internal decoded native evidence, never an already-authorized guest record. */
    struct received_record
    {
        receive_wire::reference native_reference;
        std::uint64_t deadline{},native_correlation{};
        communication::metadata native_metadata;
        wire::bytes payload;
    };
    /** @brief Validate full framing and exact sealed schema before borrowing payload bytes. */
    inline bool decode_received(wire::bytes body,receive_schema const& selected,std::uint64_t expected_deadline,received_record& output)noexcept
    {
        if(!selected.native_endpoint||!selected.provider_generation||!selected.payload_bytes||selected.payload_bytes>4u*1024u*1024u||
            selected.direction>receive_direction::service_response||selected.type.empty()||selected.type.size()>255||
            selected.encoding.empty()||selected.encoding.size()>255||!selected.validate||!expected_deadline||expected_deadline>INT64_MAX||
            body.size()>selected.payload_bytes+1024)return false;
        wire::reader input{body};received_record value;auto& reference=value.native_reference;auto& info=value.native_metadata;
        if(!input.u64(reference.endpoint)||reference.endpoint!=selected.native_endpoint||!input.u64(reference.provider_generation)||
            reference.provider_generation!=selected.provider_generation||!input.u64(reference.sequence)||!reference.sequence||
            !input.u64(reference.nonce)||!reference.nonce||!input.u64(value.deadline)||value.deadline!=expected_deadline)return false;
        bool present{};wire::bytes publisher;
        if(!input.u64(info.source)||info.source!=selected.native_endpoint||!input.u64(info.generation)||info.generation!=selected.provider_generation||
            !input.u64(info.sequence)||info.sequence!=reference.sequence||!input.boolean(present)||present||!input.boolean(present)||present||
            !input.boolean(present)||(present&&(!input.blob(publisher)||publisher.empty()||publisher.size()>info.publisher.size()))||
            !input.u64(info.loss_before)||!input.u64(value.native_correlation))return false;
        bool topic=selected.direction==receive_direction::topic;
        if(topic?value.native_correlation!=0:!value.native_correlation)return false;
        if(!topic&&!publisher.empty())return false;
        std::string_view kind,type,encoding;
        auto expected=topic?"topic.message":selected.direction==receive_direction::service_request?"service.request":"service.response";
        if(!input.text(kind)||kind!=expected||!input.text(type)||type!=selected.type||!input.text(encoding)||encoding!=selected.encoding||
            !input.blob(value.payload)||value.payload.size()>selected.payload_bytes||!input.done()||!selected.validate(value.payload))return false;
        info.publisher_bytes=publisher.size();std::copy(publisher.begin(),publisher.end(),info.publisher.begin());
        output=value;return true;
    }
    /** @brief Check a settlement receipt against its independently decoded RPC envelope.
     * @details The worker's receipt identity is returned only as native evidence.
     * Host request IDs remain host-issued. Full canonical re-encoding rejects
     * inconsistent text, invalid statuses, foreign worker generations and tails.
     */
    inline bool decode_receive_settlement(wire::bytes body,std::uint64_t generation,communication::error status,
                                          communication::effect effect,std::uint64_t& native_receipt)noexcept
    {
        if(!generation||!communication::known_status(status)||effect>communication::effect::unknown||body.size()>160)return false;
        wire::reader input{body};std::uint64_t request{};
        if(!input.u64(request)||!request)return false;
        std::array<std::uint8_t,160> canonical;wire::writer output{canonical};
        if(!::uwvm2::imported::ros::communication_contract::encode_receipt(output,{request,generation,{},status,effect})||
            !std::equal(body.begin(),body.end(),output.written().begin(),output.written().end()))return false;
        native_receipt=request;return true;
    }
}
