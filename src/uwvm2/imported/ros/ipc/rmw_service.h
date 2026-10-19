/** @file
 * Explicit same-native-client/service transport, separate from original81 and
 * the topic delivery queue. UMRC-v1 contains values, never native pointers or
 * bearer credentials. The authenticated IPC/Host must lease all actual source,
 * paired observer/responder, and independent availability capabilities.
 */
#pragma once
#include "rmw_metadata.h"
#include <new>
#include <vector>

namespace uwvm2::imported::ros::ipc::rmw_service
{
    namespace endpoint=rmw_endpoint;
    namespace metadata=rmw_metadata;
    inline constexpr std::uint32_t magic{0x43524d55}; // "UMRC"
    inline constexpr std::size_t header_bytes{160};
    inline constexpr std::size_t minimum_delivery_bytes{header_bytes+metadata::header_bytes};
    inline constexpr std::size_t endpoint_response_bytes{header_bytes+endpoint::packet_bytes};
    inline constexpr std::size_t maximum_input_bytes{max_payload_bytes-518-header_bytes};
    inline constexpr std::uint64_t maximum_wait_ns{60000000000ULL};
    enum class command:std::uint8_t
    {activate=1,inspect=2,retire=3,submit=4,peek=5,commit=6,cancel=7,respond=8,available=9,abandon=10};
    struct control
    {
        command operation{command::activate};endpoint::role kind{endpoint::role::client};
        endpoint::binding owner{};
        std::uint64_t incarnation{},peer{},ticket{},sequence{},reservation{},
            body_bytes{},capacity{},record_bytes{},wait_ns{},value{};
        std::uint8_t writer_bytes{};
    };
    inline bool endpoint_command(command c) noexcept
    {return c==command::activate||c==command::inspect||c==command::retire;}
    /** Peer is a distinct full capability handle in THIS exact instance,
     * generation and authenticated native session. For availability it is the
     * independently granted service.available owner. Otherwise it is the
     * endpoint's original event.observe or service.respond pairing. The trusted
     * caller must validate the actual operation/resource/type/QoS/alias, not
     * infer authority from these integers or a matching service name alone.
     */
    inline bool valid(control const& p,bool response) noexcept
    {
        if(!endpoint::valid(p.owner)||(p.kind!=endpoint::role::client&&p.kind!=endpoint::role::service)||
            !endpoint::full_handle(p.peer)||p.peer==p.owner.source){return false;}
        if(p.operation==command::activate)
        {
            return (response?bool(p.incarnation):!p.incarnation)&&!p.ticket&&!p.sequence&&!p.reservation&&
                !p.body_bytes&&!p.capacity&&!p.record_bytes&&!p.wait_ns&&!p.value&&!p.writer_bytes;
        }
        if(!p.incarnation){return false;}
        if(p.operation==command::inspect||p.operation==command::retire)
        {return !p.ticket&&!p.sequence&&!p.reservation&&!p.body_bytes&&!p.capacity&&!p.record_bytes&&!p.wait_ns&&!p.value&&!p.writer_bytes;}
        if(p.operation==command::submit)
        {
            return p.kind==endpoint::role::client&&(response?bool(p.ticket):!p.ticket)&&!p.sequence&&!p.reservation&&
                (response?!p.body_bytes:p.body_bytes<=maximum_input_bytes)&&!p.capacity&&!p.record_bytes&&!p.wait_ns&&!p.value&&!p.writer_bytes;
        }
        if(p.operation==command::peek)
        {
            if(p.writer_bytes==0||p.writer_bytes>16||p.capacity<minimum_delivery_bytes||p.capacity>max_payload_bytes||
                p.body_bytes||p.wait_ns||p.value){return false;}
            return response?p.sequence&&p.reservation&&p.record_bytes>=metadata::header_bytes&&p.record_bytes<=p.capacity-header_bytes:
                !p.ticket&&!p.sequence&&!p.reservation&&!p.record_bytes;
        }
        if(p.operation==command::commit||p.operation==command::cancel)
        {return p.sequence&&p.reservation&&!p.ticket&&!p.body_bytes&&!p.capacity&&!p.record_bytes&&!p.wait_ns&&!p.value&&!p.writer_bytes;}
        if(p.operation==command::respond)
        {
            return p.kind==endpoint::role::service&&p.ticket&&!p.sequence&&!p.reservation&&
                (response?!p.body_bytes:p.body_bytes<=maximum_input_bytes)&&!p.capacity&&!p.record_bytes&&!p.wait_ns&&!p.value&&!p.writer_bytes;
        }
        if(p.operation==command::available)
        {
            return p.kind==endpoint::role::client&&!p.ticket&&!p.sequence&&!p.reservation&&!p.body_bytes&&
                !p.capacity&&!p.record_bytes&&p.wait_ns<=maximum_wait_ns&&(response?p.value<=1:!p.value)&&!p.writer_bytes;
        }
        if(p.operation==command::abandon)
        {return p.kind==endpoint::role::client&&p.ticket&&!p.sequence&&!p.reservation&&!p.body_bytes&&!p.capacity&&!p.record_bytes&&!p.wait_ns&&!p.value&&!p.writer_bytes;}
        return false;
    }
    inline bool encode(control const& p,bool response,std::array<std::uint8_t,header_bytes>& output) noexcept
    {
        if(!valid(p,response)){return false;}std::array<std::uint8_t,header_bytes> b{};
        event_put(b.data(),magic,4);event_put(b.data()+4,1,2);b[6]=static_cast<std::uint8_t>(p.operation);b[7]=p.owner.backend;
        event_put(b.data()+8,header_bytes,4);b[12]=static_cast<std::uint8_t>(p.kind);b[13]=p.writer_bytes;
        event_put(b.data()+16,p.owner.source,8);event_put(b.data()+24,p.owner.instance,8);
        event_put(b.data()+32,p.owner.generation,8);event_put(b.data()+40,p.owner.session,8);
        event_put(b.data()+48,p.incarnation,8);event_put(b.data()+56,p.peer,8);event_put(b.data()+64,p.ticket,8);
        event_put(b.data()+72,p.sequence,8);event_put(b.data()+80,p.reservation,8);event_put(b.data()+88,p.body_bytes,8);
        event_put(b.data()+96,p.capacity,8);event_put(b.data()+104,p.record_bytes,8);event_put(b.data()+112,p.wait_ns,8);
        event_put(b.data()+120,p.value,8);output=b;return true;
    }
    /** Header-only decode. Packet helpers below separately enforce exact total
     * extents; accepting a valid prefix is never a complete input validation.
     */
    inline bool decode(std::uint8_t const* b,std::size_t n,bool response,control& output) noexcept
    {
        if(!b||n!=header_bytes||get(b,4)!=magic||get(b+4,2)!=1||get(b+8,4)!=header_bytes||
            !endpoint::zero(b+14,2)||!endpoint::zero(b+128,32)){return false;}
        control p{};p.operation=static_cast<command>(b[6]);p.kind=static_cast<endpoint::role>(b[12]);p.writer_bytes=b[13];
        p.owner={b[7],get(b+16,8),get(b+24,8),get(b+32,8),get(b+40,8)};
        p.incarnation=get(b+48,8);p.peer=get(b+56,8);p.ticket=get(b+64,8);p.sequence=get(b+72,8);
        p.reservation=get(b+80,8);p.body_bytes=get(b+88,8);p.capacity=get(b+96,8);
        p.record_bytes=get(b+104,8);p.wait_ns=get(b+112,8);p.value=get(b+120,8);
        if(!valid(p,response)){return false;}output=p;return true;
    }
    inline bool request(std::uint8_t const* p,std::size_t n,control& output) noexcept
    {
        control h{};if(!p||n<header_bytes||n>max_payload_bytes-518||!decode(p,header_bytes,false,h)||h.body_bytes!=n-header_bytes){return false;}
        output=h;return true;
    }
    inline bool same_call(control const& request,control const& response) noexcept
    {
        if(!valid(request,false)||!valid(response,true)||request.operation!=response.operation||request.kind!=response.kind||
            !endpoint::same(request.owner,response.owner)||request.peer!=response.peer||
            (request.incarnation&&request.incarnation!=response.incarnation)){return false;}
        if(request.operation==command::peek)
        {return request.capacity==response.capacity&&request.writer_bytes==response.writer_bytes;}
        if(request.operation==command::submit){return true;}
        return request.ticket==response.ticket&&request.sequence==response.sequence&&request.reservation==response.reservation&&
            request.wait_ns==response.wait_ns;
    }
    inline bool associated_ack(std::uint8_t const* p,std::size_t n,control const& request,control& output) noexcept
    {
        if(endpoint_command(request.operation)||request.operation==command::peek){return false;}
        control h{};if(!decode(p,n,true,h)||!same_call(request,h)){return false;}output=h;return true;
    }
    /** UMRE is the genuine retained object receipt, not a requested profile.
     * Pairing is additionally echoed/checked by UMRC. Full original GID extent
     * is preserved; client GID is never reinterpreted as a request writer GUID.
     */
    inline bool associated_endpoint(std::uint8_t const* p,std::size_t n,control const& request,
        control& output,endpoint::packet& observed) noexcept
    {
        if(!p||!endpoint_command(request.operation)||n!=endpoint_response_bytes){return false;}
        control h{};endpoint::packet actual{};
        if(!decode(p,header_bytes,true,h)||!same_call(request,h)||!endpoint::decode(p+header_bytes,endpoint::packet_bytes,true,actual)||
            !endpoint::same(actual.owner,h.owner)||actual.incarnation!=h.incarnation||actual.kind!=h.kind||
            static_cast<unsigned>(actual.operation)!=static_cast<unsigned>(request.operation)){return false;}
        output=h;observed=actual;return true;
    }
    inline bool associated_record(control const& reply,std::uint8_t const* p,std::size_t n,metadata::header& output) noexcept
    {
        if(reply.operation!=command::peek||!valid(reply,true)||!p||n!=reply.record_bytes||n<metadata::header_bytes){return false;}
        metadata::header h{};
        if(!metadata::decode(p,metadata::header_bytes,n-metadata::header_bytes,h)||!endpoint::same(h.owner,reply.owner)||
            h.incarnation!=reply.incarnation||h.sequence!=reply.sequence||h.correlation!=reply.ticket){return false;}
        if(!event_control(h.kind))
        {
            auto expected{reply.kind==endpoint::role::client?event_kind::service_response:event_kind::service_request};
            if(h.kind!=expected||!metadata::complete(h)||h.writer_size!=reply.writer_bytes){return false;}
        }
        output=h;return true;
    }
    inline bool associated_delivery(std::uint8_t const* p,std::size_t n,control const& request,
        control& output,metadata::header& information) noexcept
    {
        if(!p||request.operation!=command::peek||n<minimum_delivery_bytes||n>max_payload_bytes){return false;}
        control h{};metadata::header received{};
        if(!decode(p,header_bytes,true,h)||!same_call(request,h)||h.record_bytes!=n-header_bytes||
            !associated_record(h,p+header_bytes,n-header_bytes,received)){return false;}
        output=h;information=received;return true;
    }
    /** Allocate one complete output before exposing any header or record. The
     * actual owner must explicitly cancel a previously acquired borrow on pack
     * failure; do not consume a native queue merely because allocation failed.
     */
    inline std::uint32_t pack_delivery(control const& reply,std::uint8_t const* record,std::size_t n,
        std::vector<std::uint8_t>& output) noexcept
    {
        std::array<std::uint8_t,header_bytes> b{};metadata::header h{};
        if(!encode(reply,true,b)||!associated_record(reply,record,n,h)){return 22;}
        try
        {
            std::vector<std::uint8_t> next(header_bytes+n);std::copy(b.begin(),b.end(),next.begin());
            std::copy(record,record+n,next.begin()+header_bytes);output=std::move(next);return 0;
        }
        catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
    }
}
