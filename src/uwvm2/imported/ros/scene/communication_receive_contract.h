/** @file
 * @brief Explicit topic/service receive reservation carriers with separate completion.
 * @details reserve is endpoint/deadline; acknowledge and release are an exact
 * mutation key plus a reference to one retained record. Consumer identity and
 * authority are obtained from the current VM caller, never from these bytes.
 * The provider generation is the verified worker incarnation, not a capability
 * generation or a guessed native identifier. A successful reservation copy is
 * not acknowledgement. Publication must follow full bounded output preflight.
 *
 * Failed output preserves the same consumer's original record/reference for
 * retry. Expiry never implies acknowledgement. Release has reserved cleanup
 * capacity independent of traffic attenuation; a lost native ACK remains an
 * unknown result until trusted reconciliation, never an automatic re-consume.
 */
#pragma once
#include <array>
#include <string_view>
#define UWVM_ROS_COMMUNICATION_RECEIVE_FUNCTIONS(X) \
    X(topic_reserve, topic_reserve, "topic.reserve") \
    X(topic_acknowledge, topic_acknowledge, "topic.acknowledge") \
    X(topic_release, topic_release, "topic.release") \
    X(service_reserve, service_reserve, "service.reserve") \
    X(service_acknowledge, service_acknowledge, "service.acknowledge") \
    X(service_release, service_release, "service.release")
#ifndef UWVM_SCENE_COMMUNICATION_RECEIVE_CONTRACT_MACROS_ONLY
#include "communication_contract.h"
namespace uwvm2::imported::ros::communication_receive_contract
{
    namespace c=scene::communication;namespace wire=scene::wire;
    enum class operation:std::uint8_t
    {
#define UWVM_COMMUNICATION_RECEIVE_ENUM(symbol,action,name) action,
        UWVM_ROS_COMMUNICATION_RECEIVE_FUNCTIONS(UWVM_COMMUNICATION_RECEIVE_ENUM)
#undef UWVM_COMMUNICATION_RECEIVE_ENUM
    };
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_COMMUNICATION_RECEIVE_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_COMMUNICATION_RECEIVE_FUNCTIONS(UWVM_COMMUNICATION_RECEIVE_DESCRIPTOR)
#undef UWVM_COMMUNICATION_RECEIVE_DESCRIPTOR
    };
    struct reference
    {
        c::handle endpoint{},provider_generation{},sequence{},nonce{};
        friend bool operator==(reference const&,reference const&)=default;
    };
    struct request{c::request_key key;reference reservation;std::uint64_t deadline{};};
    inline bool reserves(operation action)noexcept{return action==operation::topic_reserve||action==operation::service_reserve;}
    inline bool acknowledges(operation action)noexcept{return action==operation::topic_acknowledge||action==operation::service_acknowledge;}
    inline bool valid(reference const& value)noexcept{return value.endpoint&&value.provider_generation&&value.sequence&&value.nonce;}
    inline bool decode(operation action,wire::bytes bytes,request& output)noexcept
    {
        if(static_cast<std::size_t>(action)>=operations.size())return false;
        wire::reader input{bytes};request value;
        if(reserves(action))
        {
            if(!input.u64(value.reservation.endpoint)||!value.reservation.endpoint||!input.u64(value.deadline)||
                !value.deadline||value.deadline>INT64_MAX)return false;
        }
        else
        {
            if(!input.u64(value.key.session)||!input.u64(value.key.sequence)||!value.key.sequence||
                !input.u64(value.reservation.endpoint)||value.key.session!=value.reservation.endpoint||
                !input.u64(value.reservation.provider_generation)||!input.u64(value.reservation.sequence)||
                !input.u64(value.reservation.nonce)||!valid(value.reservation))return false;
        }
        if(!input.done())return false;
        output=value;return true;
    }
    /** @brief Encode an actual retained provider reference and its bounded detached record.
     * @details The adapter must validate the record against its original native
     * correlation and schema. No translation of callback timestamps into an
     * authenticated clock/epoch occurs without the explicit trusted mapping.
     */
    inline bool encode(wire::writer& output,reference const& selected,std::uint64_t deadline,c::metadata const& metadata,
                       c::handle correlation,std::string_view kind,communication_contract::message_view message,
                       communication_contract::stamp_mapping stamps={})noexcept
    {
        if(!valid(selected)||!deadline||deadline>INT64_MAX||metadata.source!=selected.endpoint||metadata.sequence!=selected.sequence||
            (kind!="topic.message"&&kind!="service.request"&&kind!="service.response")||
            ((kind=="topic.message")?correlation!=0:!correlation))return false;
        return output.u64(selected.endpoint)&&output.u64(selected.provider_generation)&&output.u64(selected.sequence)&&
            output.u64(selected.nonce)&&output.u64(deadline)&&communication_contract::encode_delivery(output,metadata,correlation,kind,message,stamps);
    }
}
#endif
