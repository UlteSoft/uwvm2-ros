/** @file
 * Additive owned-RMW publication envelope. This is value transport over an
 * authenticated existing IPC session, never a permission or an auth token.
 * The complete body uses the existing canonical codec, not native memory/CDR.
 */
#pragma once
#include "rmw_endpoint.h"

namespace uwvm2::imported::ros::ipc::rmw_publish
{
    inline constexpr std::uint32_t magic{0x50524d55}; // "UMRP"
    inline constexpr std::size_t header_bytes{64};
    struct header
    {
        rmw_endpoint::binding owner{};
        std::uint64_t incarnation{},payload_bytes{};
    };
    inline bool valid(header const& h) noexcept
    {return rmw_endpoint::valid(h.owner)&&h.incarnation&&h.payload_bytes<=max_payload_bytes-header_bytes;}
    inline bool encode(header const& h,std::array<std::uint8_t,header_bytes>& output) noexcept
    {
        if(!valid(h)){return false;}
        std::array<std::uint8_t,header_bytes> b{};
        event_put(b.data(),magic,4);event_put(b.data()+4,1,2);b[6]=h.owner.backend;
        event_put(b.data()+8,header_bytes,8);event_put(b.data()+16,h.owner.source,8);
        event_put(b.data()+24,h.owner.instance,8);event_put(b.data()+32,h.owner.generation,8);
        event_put(b.data()+40,h.owner.session,8);event_put(b.data()+48,h.incarnation,8);
        event_put(b.data()+56,h.payload_bytes,8);output=b;return true;
    }
    inline bool decode(std::uint8_t const* b,std::size_t size,header& output) noexcept
    {
        if(!b||size<header_bytes||get(b,4)!=magic||get(b+4,2)!=1||b[7]||get(b+8,8)!=header_bytes){return false;}
        header h{{b[6],get(b+16,8),get(b+24,8),get(b+32,8),get(b+40,8)},get(b+48,8),get(b+56,8)};
        if(!valid(h)||h.payload_bytes!=size-header_bytes){return false;}output=h;return true;
    }
}
