/** @file
 * Additive transactional subscription delivery control. Source-only candidate.
 * UMRE-v1 stays the 256-byte actual endpoint receipt. UMRS-v1 below controls
 * borrowing the one owned URMI+canonical record; it is not a new permission.
 */
#pragma once
#include "rmw_metadata.h"
#include <new>
#include <vector>

namespace uwvm2::imported::ros::ipc::rmw_subscription
{
    namespace endpoint=rmw_endpoint;
    namespace metadata=rmw_metadata;
    inline constexpr std::uint32_t magic{0x53524d55}; // "UMRS"
    inline constexpr std::size_t header_bytes{96};
    inline constexpr std::size_t minimum_delivery_bytes{header_bytes+metadata::header_bytes};
    inline constexpr std::size_t maximum_body_bytes{max_payload_bytes-minimum_delivery_bytes};
    enum class command:std::uint8_t {peek=1,commit=2,cancel=3};
    struct control
    {
        command operation{command::peek};endpoint::binding owner{};
        std::uint64_t incarnation{},sequence{},reservation{},capacity{},record_bytes{};
        std::uint8_t gid_bytes{};
    };
    /** Capacity is the complete UMRS96+URMI192+canonical-body extent accepted
     * by the independently preflighted consumer, never just the body size.
     * Sequence belongs to the accepted stream. Reservation belongs to one
     * borrowing attempt and must not be guessed from that stream sequence.
     */
    inline bool valid(control const& p,bool response) noexcept
    {
        if(!endpoint::valid(p.owner)||!p.incarnation){return false;}
        if(p.operation==command::peek)
        {
            if(!p.gid_bytes||p.gid_bytes>32||p.capacity<minimum_delivery_bytes||p.capacity>max_payload_bytes){return false;}
            if(!response){return !p.sequence&&!p.reservation&&!p.record_bytes;}
            return p.sequence&&p.reservation&&p.record_bytes>=metadata::header_bytes&&
                p.record_bytes<=p.capacity-header_bytes;
        }
        return (p.operation==command::commit||p.operation==command::cancel)&&p.sequence&&p.reservation&&
            !p.capacity&&!p.record_bytes&&!p.gid_bytes;
    }
    inline bool encode(control const& p,bool response,std::array<std::uint8_t,header_bytes>& output) noexcept
    {
        if(!valid(p,response)){return false;}std::array<std::uint8_t,header_bytes> b{};
        event_put(b.data(),magic,4);event_put(b.data()+4,1,2);b[6]=static_cast<std::uint8_t>(p.operation);b[7]=p.owner.backend;
        event_put(b.data()+8,header_bytes,4);b[12]=p.gid_bytes;
        event_put(b.data()+16,p.owner.source,8);event_put(b.data()+24,p.owner.instance,8);
        event_put(b.data()+32,p.owner.generation,8);event_put(b.data()+40,p.owner.session,8);
        event_put(b.data()+48,p.incarnation,8);event_put(b.data()+56,p.sequence,8);
        event_put(b.data()+64,p.reservation,8);event_put(b.data()+72,p.capacity,8);
        event_put(b.data()+80,p.record_bytes,8);output=b;return true;
    }
    /** This decodes exactly a control header. The packet-level API below checks
     * that a peek reply has exactly its one complete associated record too.
     */
    inline bool decode(std::uint8_t const* b,std::size_t n,bool response,control& output) noexcept
    {
        if(!b||n!=header_bytes||get(b,4)!=magic||get(b+4,2)!=1||get(b+8,4)!=header_bytes||
            !endpoint::zero(b+13,3)||!endpoint::zero(b+88,8)){return false;}
        control p{};p.operation=static_cast<command>(b[6]);p.gid_bytes=b[12];
        p.owner={b[7],get(b+16,8),get(b+24,8),get(b+32,8),get(b+40,8)};
        p.incarnation=get(b+48,8);p.sequence=get(b+56,8);p.reservation=get(b+64,8);
        p.capacity=get(b+72,8);p.record_bytes=get(b+80,8);
        if(!valid(p,response)){return false;}output=p;return true;
    }
    inline bool same_call(control const& request,control const& response) noexcept
    {
        if(!valid(request,false)||!valid(response,true)||request.operation!=response.operation||
            !endpoint::same(request.owner,response.owner)||request.incarnation!=response.incarnation){return false;}
        if(request.operation==command::peek)
        {return request.capacity==response.capacity&&request.gid_bytes==response.gid_bytes;}
        return request.sequence==response.sequence&&request.reservation==response.reservation;
    }
    /** An acknowledgment echoes the exact attempt; there is no payload and no
     * extra length slot. Allocation/transport is outside this value helper.
     */
    inline bool associated_ack(std::uint8_t const* p,std::size_t n,control const& request,control& output) noexcept
    {
        control next{};if(request.operation==command::peek||!decode(p,n,true,next)||!same_call(request,next)){return false;}
        output=next;return true;
    }
    inline bool associated_record(control const& reply,std::uint8_t const* record,std::size_t n,
        metadata::header& output) noexcept
    {
        if(reply.operation!=command::peek||!valid(reply,true)||!record||n!=reply.record_bytes||n<metadata::header_bytes){return false;}
        metadata::header h{};
        if(!metadata::decode(record,metadata::header_bytes,n-metadata::header_bytes,h)||!metadata::complete(h)||
            h.kind!=event_kind::topic_message||!endpoint::same(h.owner,reply.owner)||h.incarnation!=reply.incarnation||
            h.sequence!=reply.sequence||h.gid_size!=reply.gid_bytes){return false;}
        output=h;return true;
    }
    /** Check one bounded complete packet before exposing any header/output.
     * Canonical type-specific decoding and real source/session leases remain
     * mandatory at the trusted consumer; this helper adds no native authority.
     */
    inline bool associated_delivery(std::uint8_t const* p,std::size_t n,control const& request,
        control& output,metadata::header& information) noexcept
    {
        if(!p||request.operation!=command::peek||n<minimum_delivery_bytes||n>max_payload_bytes){return false;}
        control reply{};metadata::header h{};
        if(!decode(p,header_bytes,true,reply)||!same_call(request,reply)||reply.record_bytes!=n-header_bytes||
            !associated_record(reply,p+header_bytes,n-header_bytes,h)){return false;}
        output=reply;information=h;return true;
    }
    /** Construct one owned packet atomically. Allocation or validation failure
     * preserves output. A caller that already borrowed a record must explicitly
     * cancel that reservation on failure and retain both error observations.
     */
    inline std::uint32_t pack_delivery(control const& reply,std::uint8_t const* record,std::size_t n,
        std::vector<std::uint8_t>& output) noexcept
    {
        std::array<std::uint8_t,header_bytes> head{};metadata::header h{};
        if(!encode(reply,true,head)||!associated_record(reply,record,n,h)){return 22;}
        try
        {
            std::vector<std::uint8_t> next(header_bytes+n);
            std::copy(head.begin(),head.end(),next.begin());std::copy(record,record+n,next.begin()+header_bytes);
            output=std::move(next);return 0;
        }
        catch(std::bad_alloc const&){return 12;}catch(...){return 5;}
    }
}
