/*************************************************************
 * UWVM2 actual native ROS record metadata. APL-2.0.          *
 *************************************************************/
#pragma once
#include "rmw_endpoint.h"

namespace uwvm2::imported::ros::ipc::rmw_metadata
{
    /** A same-record sidecar for a NEW delivery path, not an extension guessed
     * inside legacy payload bytes. Header and canonical body must be owned and
     * queued together, peeked together and committed with the same sequence.
     * No second metadata queue or "last message" cache is allowed. */
    inline constexpr std::uint32_t magic{0x494d5255}; // "URMI"
    inline constexpr std::size_t header_bytes{192};
    inline constexpr std::uint32_t source_time{1},received_time{2},publisher_gid{4},
        publication_sequence{8},reception_sequence{16},native_request_id{32};
    inline constexpr std::uint32_t known_flags{63};
    struct header
    {
        event_kind kind{event_kind::topic_message};rmw_endpoint::binding owner{};
        std::uint64_t incarnation{},sequence{},correlation{},payload_bytes{};
        std::uint32_t flags{},error{};
        // Native signed nanoseconds and signed service sequence retain every
        // bit. Zero is data only when its explicit presence bit is set.
        std::int64_t source_timestamp{},received_timestamp{},request_sequence{};
        std::uint64_t publication_number{UINT64_MAX},reception_number{UINT64_MAX};
        std::uint8_t gid_size{},writer_size{},control_flags{};
        std::array<std::uint8_t,32> gid{};
        std::array<std::uint8_t,16> writer{};
    };
    inline std::int64_t signed_value(std::uint64_t value) noexcept
    {std::int64_t result;static_assert(sizeof(result)==sizeof(value));std::memcpy(&result,&value,sizeof(result));return result;}
    inline bool valid(header const& h) noexcept
    {
        if(!rmw_endpoint::valid(h.owner)||!h.incarnation||!h.sequence||h.payload_bytes>max_payload_bytes-header_bytes||
            (h.flags&~known_flags)||h.gid_size>32||h.writer_size>16||
            !rmw_endpoint::zero(h.gid.data()+h.gid_size,32-h.gid_size)||
            !rmw_endpoint::zero(h.writer.data()+h.writer_size,16-h.writer_size)){return false;}
        if(!(h.flags&source_time)&&h.source_timestamp){return false;}
        if(!(h.flags&received_time)&&h.received_timestamp){return false;}
        if(bool(h.flags&publisher_gid)!=bool(h.gid_size)||bool(h.flags&native_request_id)!=bool(h.writer_size)){return false;}
        if(!(h.flags&publication_sequence)&&h.publication_number!=UINT64_MAX){return false;}
        if(!(h.flags&reception_sequence)&&h.reception_number!=UINT64_MAX){return false;}
        if((h.flags&publication_sequence)&&h.publication_number==UINT64_MAX){return false;}
        if((h.flags&reception_sequence)&&h.reception_number==UINT64_MAX){return false;}
        if(!(h.flags&native_request_id)&&h.request_sequence){return false;}
        if(event_control(h.kind))
        {
            return !h.flags&&!h.payload_bytes&&!h.gid_size&&!h.writer_size&&
                !(h.control_flags&~std::uint8_t{3})&&
                (h.kind==event_kind::resource_error?h.error!=0:bool(h.control_flags&event_source_retired));
        }
        if(h.error||h.control_flags){return false;}
        if(h.kind==event_kind::topic_message){return !h.correlation&&!(h.flags&native_request_id);}
        if(h.kind==event_kind::service_request||h.kind==event_kind::service_response)
        {return h.correlation&&!(h.flags&(publisher_gid|publication_sequence|reception_sequence));}
        return false;
    }
    /** Missing native metadata is represented by absent bits, never fake GUIDs
     * or timestamps. Strict upstream adapters check the complete required mask
     * before consuming. An unavailable capability returns95 with no consumption.
     * IPC delivery itself is genuinely not Guest rclcpp intra-process delivery;
     * a native executor's own intra-process bit is not transferred as that claim. */
    inline bool complete(header const& h) noexcept
    {
        auto required{source_time|received_time|(h.kind==event_kind::topic_message?publisher_gid:native_request_id)};
        return valid(h)&&!event_control(h.kind)&&(h.flags&required)==required;
    }
    inline bool encode(header const& h,std::array<std::uint8_t,header_bytes>& output) noexcept
    {
        if(!valid(h)){return false;}std::array<std::uint8_t,header_bytes> b{};
        event_put(b.data(),magic,4);event_put(b.data()+4,1,2);b[6]=static_cast<std::uint8_t>(h.kind);b[7]=h.owner.backend;
        event_put(b.data()+8,header_bytes,4);event_put(b.data()+12,h.flags,4);
        event_put(b.data()+16,h.owner.source,8);event_put(b.data()+24,h.owner.instance,8);
        event_put(b.data()+32,h.owner.generation,8);event_put(b.data()+40,h.owner.session,8);
        event_put(b.data()+48,h.incarnation,8);event_put(b.data()+56,h.sequence,8);event_put(b.data()+64,h.correlation,8);
        event_put(b.data()+72,static_cast<std::uint64_t>(h.source_timestamp),8);event_put(b.data()+80,static_cast<std::uint64_t>(h.received_timestamp),8);
        event_put(b.data()+88,h.publication_number,8);event_put(b.data()+96,h.reception_number,8);
        event_put(b.data()+104,static_cast<std::uint64_t>(h.request_sequence),8);event_put(b.data()+112,h.payload_bytes,8);
        b[120]=h.gid_size;b[121]=h.writer_size;b[122]=h.control_flags;event_put(b.data()+124,h.error,4);
        std::copy(h.gid.begin(),h.gid.end(),b.begin()+128);std::copy(h.writer.begin(),h.writer.end(),b.begin()+160);
        output=b;return true;
    }
    /** Decode a header only, with the independently supplied complete body
     * extent. The caller still validates the canonical Message/Service codec
     * and current source grant before any Guest copy or queue commit. */
    inline bool decode(std::uint8_t const* b,std::size_t n,std::size_t body_bytes,header& output) noexcept
    {
        if(!b||n!=header_bytes||get(b,4)!=magic||get(b+4,2)!=1||get(b+8,4)!=header_bytes||
            b[123]||!rmw_endpoint::zero(b+176,16)){return false;}
        header h{};h.kind=static_cast<event_kind>(b[6]);h.owner={b[7],get(b+16,8),get(b+24,8),get(b+32,8),get(b+40,8)};
        h.flags=static_cast<std::uint32_t>(get(b+12,4));h.incarnation=get(b+48,8);h.sequence=get(b+56,8);h.correlation=get(b+64,8);
        h.source_timestamp=signed_value(get(b+72,8));h.received_timestamp=signed_value(get(b+80,8));
        h.publication_number=get(b+88,8);h.reception_number=get(b+96,8);h.request_sequence=signed_value(get(b+104,8));
        h.payload_bytes=get(b+112,8);h.gid_size=b[120];h.writer_size=b[121];h.control_flags=b[122];h.error=static_cast<std::uint32_t>(get(b+124,4));
        std::copy(b+128,b+160,h.gid.begin());std::copy(b+160,b+176,h.writer.begin());
        if(h.payload_bytes!=body_bytes||!valid(h)){return false;}output=h;return true;
    }
}
