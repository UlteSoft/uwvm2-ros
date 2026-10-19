/*************************************************************
 * UWVM2 ROS Runtime Extension. APL-2.0.                     *
 *************************************************************/
#pragma once
#include "protocol.h"
#include <array>
#include <string_view>

namespace uwvm2::imported::ros::ipc
{
    // Versioned event records are an additive API. Legacy event_poll and RPC
    // bodies retain their existing layouts and destructive-too-small behavior.
    // Integers are unaligned little endian; no native struct is a wire object.
    inline constexpr std::uint32_t event_magic{0x45525755}; // "UWRE"
    inline constexpr std::uint16_t event_version{1};
    inline constexpr std::size_t event_header_bytes{64};
    inline constexpr std::size_t event_ready_bytes{32};
    inline constexpr std::string_view event_type{"builtin/event-v1"};
    inline constexpr std::string_view event_type_identity{"builtin:ros-event-envelope-le64-v1"};
    enum class event_kind : std::uint16_t
    {
        topic_message=1, service_request=2, service_response=3,
        action_feedback=4, action_result=5, resource_error=6, resource_closed=7
    };
    // Incoming Action requests and parameter projections retain their existing
    // exact source permissions/types. These flags never turn them into service
    // or topic authority. A close/error carries no native content whatsoever.
    inline constexpr std::uint8_t event_source_retired{1};
    inline constexpr std::uint8_t event_external_effects_may_remain{2};
    inline constexpr std::uint8_t event_incoming_action{4};
    inline constexpr std::uint8_t event_parameter_projection{8};
    inline constexpr std::array<std::string_view,7> event_names{{
        "topic-message","service-request","service-response","action-feedback",
        "action-result","resource-error","resource-closed"}};
    inline constexpr bool valid_event_kind(event_kind value) noexcept
    { return static_cast<unsigned>(value)>=1&&static_cast<unsigned>(value)<=7; }
    inline constexpr std::uint32_t event_bit(event_kind value) noexcept
    { return valid_event_kind(value)?std::uint32_t{1}<<(static_cast<unsigned>(value)-1):0; }
    inline constexpr bool event_control(event_kind value) noexcept
    { return value==event_kind::resource_error||value==event_kind::resource_closed; }
    inline constexpr std::string_view event_name(event_kind value) noexcept
    { return valid_event_kind(value)?event_names[static_cast<unsigned>(value)-1]:std::string_view{}; }

    // Configured masks are explicit, nonempty, duplicate-free CSV in the enum
    // order above. Wildcards, whitespace, unknown tokens and trailing commas
    // are rejected. This scanner allocates nothing and validates the whole view.
    inline bool parse_event_kinds(std::string_view text,std::uint32_t& output) noexcept
    {
        if(text.empty()||text.size()>255) { return false; }
        std::uint32_t next{};unsigned previous{};std::size_t at{};
        while(at<text.size())
        {
            auto end{text.find(',',at)};if(end==std::string_view::npos) { end=text.size(); }
            auto token{text.substr(at,end-at)};unsigned selected{};
            for(unsigned i{};i<event_names.size();++i) { if(token==event_names[i]) { selected=i+1;break; } }
            if(!selected||selected<=previous) { return false; }
            next|=std::uint32_t{1}<<(selected-1);previous=selected;
            if(end==text.size()) { output=next;return true; }
            at=end+1;if(at==text.size()) { return false; }
        }
        return false;
    }
    // Metadata observation requires its own event.observe grant. Payload kinds
    // additionally require this exact source operation in the same Instance/ROS
    // backend. Source grant identity, byte/content bounds and native ownership
    // are checked by the capability manager and Gateway, not by this classifier.
    inline bool event_source_kind(std::string_view operation,event_kind& kind,std::uint8_t& flags) noexcept
    {
        flags=0;
        if(operation=="topic.subscribe") { kind=event_kind::topic_message; }
        else if(operation=="service.advertise") { kind=event_kind::service_request; }
        else if(operation=="service.call") { kind=event_kind::service_response; }
        else if(operation=="action.receive-feedback") { kind=event_kind::action_feedback; }
        else if(operation=="action.receive-result") { kind=event_kind::action_result; }
        else if(operation=="action.advertise") { kind=event_kind::service_request;flags=event_incoming_action; }
        else if(operation=="param.subscribe-events") { kind=event_kind::topic_message;flags=event_parameter_projection; }
        else { return false; }
        return true;
    }
    inline bool event_kind_for_source(std::string_view operation,event_kind kind,std::uint8_t flags=0) noexcept
    {
        if(event_control(kind)) { return (flags&~std::uint8_t{3})==0; }
        event_kind expected{};std::uint8_t expected_flags{};
        return event_source_kind(operation,expected,expected_flags)&&kind==expected&&flags==expected_flags;
    }
    // Native outgoing Action queues belong to a goal/resource/type and receive
    // kind, not to a policy alias. A live payload watch exclusively consumes
    // that queue even when a different independently granted alias names it.
    // Metadata-only correlation0 watches do not acquire this consumer claim.
    inline bool event_action_consumer_conflict(std::string_view operation,std::string_view resource,
        std::string_view type,std::uint64_t goal,std::string_view watched_operation,
        std::string_view watched_resource,std::string_view watched_type,std::uint64_t watched_goal,
        std::uint32_t watched_mask) noexcept
    {
        event_kind kind{};std::uint8_t flags{};
        return goal&&goal==watched_goal&&(operation=="action.receive-feedback"||operation=="action.receive-result")&&
            operation==watched_operation&&resource==watched_resource&&type==watched_type&&
            event_source_kind(operation,kind,flags)&&bool(watched_mask&event_bit(kind));
    }
    // Call after checking BOTH memory ranges. Difference comparisons avoid
    // offset+length overflow at the u64 boundary. A zero-capacity output has no
    // bytes to overlap; lengthSlot is always an independent eight-byte range.
    inline constexpr bool event_output_overlaps_length(std::uint64_t output,std::uint64_t capacity,
        std::uint64_t length_slot) noexcept
    { return capacity&&(output<=length_slot?length_slot-output<capacity:output-length_slot<8); }
    struct event_header
    {
        event_kind kind{};
        std::uint8_t backend{},flags{};
        std::uint32_t error{};
        std::uint64_t source_handle{},correlation{},sequence{},instance{},session{},payload_bytes{};
    };
    inline bool valid_event_header(event_header const& value) noexcept
    {
        if(!valid_event_kind(value.kind)||value.backend<1||value.backend>2||
           !value.source_handle||!value.sequence||!value.instance||!value.session||
           value.payload_bytes>max_payload_bytes-event_header_bytes) { return false; }
        if(event_control(value.kind))
        {
            if(value.payload_bytes||(value.flags&~std::uint8_t{3})) { return false; }
            if(value.kind==event_kind::resource_error&&!value.error) { return false; }
            return value.kind!=event_kind::resource_closed||(value.flags&event_source_retired)!=0;
        }
        if(value.error) { return false; }
        if(value.kind==event_kind::topic_message)
        { return !value.correlation&&(value.flags==0||value.flags==event_parameter_projection); }
        if(value.kind==event_kind::service_request)
        { return value.correlation&&(value.flags==0||value.flags==event_incoming_action); }
        return value.correlation&&!value.flags;
    }
    inline void event_put(std::uint8_t* output,std::uint64_t value,unsigned width) noexcept
    { for(unsigned i{};i<width;++i) { output[i]=static_cast<std::uint8_t>(value>>(8*i)); } }
    inline bool encode_event_header(event_header const& value,std::uint8_t* output,std::size_t capacity) noexcept
    {
        if(!output||capacity<event_header_bytes||!valid_event_header(value)) { return false; }
        event_put(output,event_magic,4);event_put(output+4,event_version,2);event_put(output+6,static_cast<unsigned>(value.kind),2);
        output[8]=value.backend;output[9]=value.flags;event_put(output+10,0,2);event_put(output+12,value.error,4);
        event_put(output+16,value.source_handle,8);event_put(output+24,value.correlation,8);event_put(output+32,value.sequence,8);
        event_put(output+40,value.instance,8);event_put(output+48,value.session,8);event_put(output+56,value.payload_bytes,8);return true;
    }
    inline bool decode_event_header(std::uint8_t const* input,std::size_t size,event_header& value) noexcept
    {
        if(!input||size<event_header_bytes||get(input,4)!=event_magic||get(input+4,2)!=event_version||get(input+10,2)) { return false; }
        event_header next{};next.kind=static_cast<event_kind>(get(input+6,2));next.backend=input[8];next.flags=input[9];
        next.error=static_cast<std::uint32_t>(get(input+12,4));next.source_handle=get(input+16,8);next.correlation=get(input+24,8);
        next.sequence=get(input+32,8);next.instance=get(input+40,8);next.session=get(input+48,8);next.payload_bytes=get(input+56,8);
        if(!valid_event_header(next)) { return false; }value=next;return true;
    }
    // The payload is the unchanged canonical legacy operation body. In request
    // and Action bodies the old ID prefix remains present and must agree with
    // correlation; this lets existing checked codecs be reused without guessing
    // a native serialization or losing an owned request/goal identity.
    inline bool decode_event(std::uint8_t const* input,std::size_t size,event_header& value) noexcept
    {
        event_header next{};if(!decode_event_header(input,size,next)||next.payload_bytes!=size-event_header_bytes) { return false; }
        if((next.kind==event_kind::service_request||next.kind==event_kind::action_feedback||next.kind==event_kind::action_result)&&
           (next.payload_bytes<8||get(input+event_header_bytes,8)!=next.correlation)) { return false; }
        value=next;return true;
    }
    struct event_readiness
    {
        std::uint64_t ready_events{},next_event_bytes{},admission_slots{},admission_bytes{};
    };
    inline std::array<std::uint8_t,event_ready_bytes> encode_event_readiness(event_readiness const& value) noexcept
    {
        std::array<std::uint8_t,event_ready_bytes> bytes{};
        event_put(bytes.data(),value.ready_events,8);event_put(bytes.data()+8,value.next_event_bytes,8);
        event_put(bytes.data()+16,value.admission_slots,8);event_put(bytes.data()+24,value.admission_bytes,8);return bytes;
    }
}
