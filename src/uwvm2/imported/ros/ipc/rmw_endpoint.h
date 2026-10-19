/*************************************************************
 * UWVM2 owned native RMW endpoints. APL-2.0.                 *
 *************************************************************/
#pragma once
#include "event.h"
#include <algorithm>
#include <cstring>

namespace uwvm2::imported::ros::ipc::rmw_endpoint
{
    /** Additive IPC format. Neither the original81 Guest imports nor UWRE-v1
     * changes. These associations are supplied by the trusted Host AFTER its
     * current source capability lease. They are not IPC authentication tokens.
     * Every request still needs the authenticated transport session/policy. */
    inline constexpr std::uint32_t magic{0x45524d55}; // "UMRE"
    inline constexpr std::size_t packet_bytes{256},qos_bytes{80};
    enum class command:std::uint8_t { activate=1,inspect=2,retire=3 };
    enum class role:std::uint8_t { publisher=1,subscription=2,client=3,service=4 };
    struct binding
    {
        std::uint8_t backend{};
        std::uint64_t source{},instance{},generation{},session{};
    };
    inline bool full_handle(std::uint64_t h) noexcept
    {return static_cast<std::uint32_t>(h)&&static_cast<std::uint32_t>(h>>32);}
    inline bool valid(binding const& b) noexcept
    {return b.backend==2&&full_handle(b.source)&&b.instance&&b.generation&&b.session;}
    inline bool same(binding const& a,binding const& b) noexcept
    {return a.backend==b.backend&&a.source==b.source&&a.instance==b.instance&&a.generation==b.generation&&a.session==b.session;}
    inline bool valid(role r) noexcept
    {return r==role::publisher||r==role::subscription||r==role::client||r==role::service;}
    /** Protocol enums are our explicit vocabulary, not memcpy'd native enums:
     * history1 KEEP_LAST/2 KEEP_ALL; reliability1 RELIABLE/2 BEST_EFFORT;
     * durability1 TRANSIENT_LOCAL/2 VOLATILE; liveliness1 AUTOMATIC/2 MANUAL_BY_TOPIC.
     * SYSTEM_DEFAULT/UNKNOWN cannot attest an effective setting. Native adapters
     * map genuine get_actual_qos() values; declaration profiles cannot do so. */
    struct duration {std::uint64_t seconds{},nanoseconds{};};
    struct qos
    {
        std::uint32_t history{},reliability{},durability{},liveliness{};
        std::uint64_t depth{};
        duration deadline{},lifespan{},lease{};
        bool avoid_ros_namespace_conventions{};
    };
    inline bool valid(duration const& v) noexcept
    {return v.nanoseconds<1000000000ULL;}
    inline bool valid(qos const& q) noexcept
    {
        return q.history>=1&&q.history<=2&&q.reliability>=1&&q.reliability<=2&&
            q.durability>=1&&q.durability<=2&&q.liveliness>=1&&q.liveliness<=2&&
            (q.history!=1||q.depth)&&valid(q.deadline)&&valid(q.lifespan)&&valid(q.lease);
    }
    inline bool same(duration const& a,duration const& b) noexcept
    {return a.seconds==b.seconds&&a.nanoseconds==b.nanoseconds;}
    inline bool same(qos const& a,qos const& b) noexcept
    {
        return a.history==b.history&&a.reliability==b.reliability&&a.durability==b.durability&&
            a.liveliness==b.liveliness&&a.depth==b.depth&&same(a.deadline,b.deadline)&&
            same(a.lifespan,b.lifespan)&&same(a.lease,b.lease)&&
            a.avoid_ros_namespace_conventions==b.avoid_ros_namespace_conventions;
    }
    inline bool zero(std::uint8_t const* p,std::size_t n) noexcept
    {for(std::size_t i{};i<n;++i){if(p[i]){return false;}}return true;}
    inline void encode_qos(qos const& q,std::uint8_t* b) noexcept
    {
        event_put(b,q.history,4);event_put(b+4,q.reliability,4);event_put(b+8,q.durability,4);
        event_put(b+12,q.liveliness,4);event_put(b+16,q.depth,8);
        event_put(b+24,q.deadline.seconds,8);event_put(b+32,q.deadline.nanoseconds,8);
        event_put(b+40,q.lifespan.seconds,8);event_put(b+48,q.lifespan.nanoseconds,8);
        event_put(b+56,q.lease.seconds,8);event_put(b+64,q.lease.nanoseconds,8);
        b[72]=q.avoid_ros_namespace_conventions?1:0;std::fill(b+73,b+80,0);
    }
    inline bool decode_qos(std::uint8_t const* b,qos& output) noexcept
    {
        if(b[72]>1||!zero(b+73,7)){return false;}
        qos q{static_cast<std::uint32_t>(get(b,4)),static_cast<std::uint32_t>(get(b+4,4)),
            static_cast<std::uint32_t>(get(b+8,4)),static_cast<std::uint32_t>(get(b+12,4)),get(b+16,8),
            {get(b+24,8),get(b+32,8)},{get(b+40,8),get(b+48,8)},{get(b+56,8),get(b+64,8)},b[72]!=0};
        if(!valid(q)){return false;}output=q;return true;
    }
    struct packet
    {
        command operation{command::activate};role kind{role::publisher};binding owner{};
        // Session-owned never-reused native incarnation, minted on activation.
        // It identifies the SAME retained native object used by later traffic.
        std::uint64_t incarnation{};
        std::uint8_t profiles{},gid_size{};
        std::array<qos,2> actual{};
        std::array<std::uint8_t,32> gid{};
    };
    inline bool valid_request(packet const& p) noexcept
    {
        if(!valid(p.owner)||!valid(p.kind)||p.profiles||p.gid_size||!zero(p.gid.data(),p.gid.size())){return false;}
        return p.operation==command::activate?!p.incarnation:
            ((p.operation==command::inspect||p.operation==command::retire)&&p.incarnation);
    }
    inline bool valid_response(packet const& p) noexcept
    {
        if(!valid(p.owner)||!valid(p.kind)||!p.incarnation){return false;}
        if(p.operation==command::retire){return !p.profiles&&!p.gid_size&&zero(p.gid.data(),32);}
        if(p.operation!=command::activate&&p.operation!=command::inspect){return false;}
        auto count{p.kind==role::client||p.kind==role::service?2u:1u};
        if(p.profiles!=count||!valid(p.actual[0])||(count==2&&!valid(p.actual[1]))){return false;}
        // A publisher reply includes its actual native GID, never a source
        // handle/queue ordinal. The native implementation chooses its extent.
        if(p.kind==role::publisher||p.kind==role::client)
        {return (p.kind!=role::publisher||p.gid_size)&&p.gid_size<=32&&zero(p.gid.data()+p.gid_size,32-p.gid_size);}
        return !p.gid_size&&zero(p.gid.data(),32);
    }
    inline bool encode(packet const& p,bool response,std::array<std::uint8_t,packet_bytes>& output) noexcept
    {
        if(response?!valid_response(p):!valid_request(p)){return false;}
        std::array<std::uint8_t,packet_bytes> b{};
        event_put(b.data(),magic,4);event_put(b.data()+4,1,2);b[6]=static_cast<std::uint8_t>(p.operation);b[7]=p.owner.backend;
        event_put(b.data()+8,packet_bytes,4);b[12]=static_cast<std::uint8_t>(p.kind);b[13]=p.profiles;b[14]=p.gid_size;
        event_put(b.data()+16,p.owner.source,8);event_put(b.data()+24,p.owner.instance,8);
        event_put(b.data()+32,p.owner.generation,8);event_put(b.data()+40,p.owner.session,8);event_put(b.data()+48,p.incarnation,8);
        for(unsigned i{};i<p.profiles;++i){encode_qos(p.actual[i],b.data()+64+qos_bytes*i);}
        std::copy(p.gid.begin(),p.gid.end(),b.begin()+224);output=b;return true;
    }
    inline bool decode(std::uint8_t const* b,std::size_t n,bool response,packet& output) noexcept
    {
        if(!b||n!=packet_bytes||get(b,4)!=magic||get(b+4,2)!=1||get(b+8,4)!=packet_bytes||b[15]||!zero(b+56,8)){return false;}
        packet p{};p.operation=static_cast<command>(b[6]);p.owner={b[7],get(b+16,8),get(b+24,8),get(b+32,8),get(b+40,8)};
        p.kind=static_cast<role>(b[12]);p.profiles=b[13];p.gid_size=b[14];p.incarnation=get(b+48,8);
        if(p.profiles>2){return false;}
        for(unsigned i{};i<p.profiles;++i){if(!decode_qos(b+64+qos_bytes*i,p.actual[i])){return false;}}
        if(!zero(b+64+qos_bytes*p.profiles,qos_bytes*(2-p.profiles))){return false;}
        std::copy(b+224,b+256,p.gid.begin());
        if(response?!valid_response(p):!valid_request(p)){return false;}output=p;return true;
    }
    inline bool associated_response(std::uint8_t const* b,std::size_t n,packet const& request,packet& output) noexcept
    {
        packet p{};if(!valid_request(request)||!decode(b,n,true,p)||!same(p.owner,request.owner)||
            p.kind!=request.kind||p.operation!=request.operation||(request.incarnation&&p.incarnation!=request.incarnation)){return false;}
        output=p;return true;
    }
}
