/** @file
 * @brief Closed static-peer RTPS transport policy and bounded pipe envelopes.
 * @details The approved host broker alone owns UDP sockets. A worker receives
 * four directional anonymous pipes, each already bound to discovery/user and
 * receive/transmit. Frames contain no socket, address, port or selectable route.
 * The generation and strictly increasing sequence fence stale/replayed worker
 * traffic. Every default field denies use. Queue reservations are independent
 * of optional rate attenuation; an explicit zero rate denies every datagram.
 * IPv4/port/GUID checks restrict scope, but do not authenticate a local peer.
 * This profile deliberately refuses multicast, remote IPs, arbitrary RTPS
 * extension submessages and redirected source/destination participant prefixes.
 */
#pragma once
#include "wire.h"
#include "communication_worker_backend.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace uwvm2::imported::ros::scene::communication_transport
{
    using role=communication_worker::pipe_role;
    using prefix=std::array<std::uint8_t,12>;
    inline constexpr std::size_t header_bytes=32;
    inline constexpr std::uint64_t frame_magic=0x4d41524744534f52ull; // ROSDGRAM
    constexpr std::size_t index(role value) noexcept{return static_cast<std::size_t>(value);}
    constexpr bool discovery(role value) noexcept{return value==role::discovery_rx||value==role::discovery_tx;}
    using communication_worker::receiving;
    struct quota
    {
        std::uint64_t packets{},bytes{};
        bool attenuated{};
        std::uint64_t packets_per_second{},bytes_per_second{};
    };
    /** @brief Complete trusted one-worker/one-peer deployment reservation.
     * @details Ports are exact standard RTPS unicast ports for the declared
     * domain/participant. Output source ports are separately explicit: FastDDS
     * normally uses an ephemeral sender, which this restricted profile refuses.
     * Prefixes are fixed by both participant profiles. The two user data flags
     * authorize DATA/DATA_FRAG; reliability control also needs its pipe right.
     * finite max_sample_bytes bounds an individual fragment declaration, not
     * the native middleware's aggregate reassembly cache. Worker address-space
     * and RSS limits remain mandatory. There is no network effect in valid().
     */
    struct plan
    {
        std::uint64_t generation{};
        std::uint32_t domain{},local_participant{},peer_participant{};
        std::uint16_t local_discovery{},local_user{},peer_discovery{},peer_user{};
        std::uint16_t local_discovery_source{},local_user_source{},peer_source{};
        prefix local_prefix{},peer_prefix{};
        std::array<bool,4> allowed{};
        bool user_data_receive{},user_data_transmit{},allow_uncertain_attenuation{};
        std::uint64_t frame_deadline_ns{};
        std::uint32_t max_datagram_bytes{},max_sample_bytes{},max_submessages{},socket_buffer_bytes{};
        std::array<quota,4> queues{};
        bool valid() const noexcept
        {
            if(!generation||domain>232||local_participant>119||peer_participant>119||local_participant==peer_participant||
               max_datagram_bytes<64||max_datagram_bytes>65000||max_sample_bytes<max_datagram_bytes||max_sample_bytes>(1u<<26)||
               !max_submessages||max_submessages>1024||socket_buffer_bytes<2*max_datagram_bytes||socket_buffer_bytes>(1u<<24)||
               !frame_deadline_ns||frame_deadline_ns>60000000000ull||local_prefix==prefix{}||peer_prefix==prefix{}||
               std::equal(local_prefix.begin(),local_prefix.begin()+8,peer_prefix.begin()))return false;
            auto local=7400u+250u*domain+10u+2u*local_participant;
            auto peer=7400u+250u*domain+10u+2u*peer_participant;
            if(local+1>65535||peer+1>65535||local_discovery!=local||local_user!=local+1||peer_discovery!=peer||peer_user!=peer+1)return false;
            std::array<std::uint16_t,7> ports{local_discovery,local_user,peer_discovery,peer_user,local_discovery_source,local_user_source,peer_source};
            for(std::size_t i{};i<ports.size();++i)
            {if(ports[i]<1024)return false;for(std::size_t j{};j<i;++j)if(ports[i]==ports[j])return false;}
            for(std::size_t i{};i<queues.size();++i)
            {
                auto const& q=queues[i];
                if(!allowed[i]){if(q.packets||q.bytes||q.attenuated||q.packets_per_second||q.bytes_per_second)return false;continue;}
                if(!q.packets||q.packets>65536||q.bytes>(1ull<<28)||q.bytes<q.packets*max_datagram_bytes)return false;
                if(q.attenuated&&!allow_uncertain_attenuation)return false;
                if(!q.attenuated&&(q.packets_per_second||q.bytes_per_second))return false;
            }
            if(user_data_receive&&!allowed[index(role::user_rx)])return false;
            if(user_data_transmit&&!allowed[index(role::user_tx)])return false;
            return allowed[0]&&allowed[1]; // Discovery has a distinct explicit reservation.
        }
    };
    struct frame_header {std::uint64_t generation{},sequence{},length{};};
    inline bool encode_header(std::span<std::uint8_t> output,frame_header const& h) noexcept
    {if(output.size()!=header_bytes||!h.generation||!h.sequence||!h.length)return false;wire::writer w{output};return w.u64(frame_magic)&&w.u64(h.generation)&&w.u64(h.sequence)&&w.u64(h.length);}
    inline bool decode_header(wire::bytes input,plan const& p,std::uint64_t expected,frame_header& output) noexcept
    {
        if(input.size()!=header_bytes||!expected)return false;
        wire::reader r{input};std::uint64_t magic{};frame_header h;
        if(!r.u64(magic)||magic!=frame_magic||!r.u64(h.generation)||h.generation!=p.generation||
           !r.u64(h.sequence)||h.sequence!=expected||!r.u64(h.length)||h.length<20||h.length>p.max_datagram_bytes||!r.done())return false;
        output=h;return true;
    }
    inline std::uint16_t u16(wire::bytes b,std::size_t at,bool little) noexcept
    {return little?std::uint16_t(b[at]|(std::uint16_t(b[at+1])<<8)):std::uint16_t((std::uint16_t(b[at])<<8)|b[at+1]);}
    inline std::uint32_t u32(wire::bytes b,std::size_t at,bool little) noexcept
    {std::uint32_t value{};for(unsigned i{};i<4;++i)value|=std::uint32_t(b[at+i])<<(8*(little?i:3-i));return value;}
    inline bool equal_prefix(wire::bytes b,std::size_t at,prefix const& expected) noexcept
    {return at<=b.size()&&b.size()-at>=expected.size()&&std::equal(expected.begin(),expected.end(),b.begin()+static_cast<std::ptrdiff_t>(at));}
    inline bool zero_prefix(wire::bytes b,std::size_t at) noexcept
    {return equal_prefix(b,at,prefix{});}
    inline bool builtin_entity(wire::bytes b,std::size_t at) noexcept
    {return (b[at+3]&0xc0u)==0xc0u;}
    inline bool unknown_entity(wire::bytes b,std::size_t at) noexcept
    {return !(b[at]|b[at+1]|b[at+2]|b[at+3]);}
    /** @brief Validate the bounded RTPS envelope and the exact pipe direction.
     * @details This is a scope filter before the native parser, not a second
     * DDS implementation. It checks all extents before field access, fixed
     * participant prefixes, builtin/user entity separation, bounded bitmap and
     * fragment declarations. Native schema, topic, QoS and semantic validation
     * remain mandatory. No allocation, queue mutation or rate debit occurs.
     */
    inline bool packet_allowed(plan const& p,role direction,wire::bytes data) noexcept
    {
        auto n=index(direction);if(n>=4||!p.allowed[n]||data.size()<20||data.size()>p.max_datagram_bytes)return false;
        if(data[0]!='R'||data[1]!='T'||data[2]!='P'||data[3]!='S'||data[4]!=2||data[5]>4)return false;
        auto const& sender=receiving(direction)?p.peer_prefix:p.local_prefix;
        auto const& target=receiving(direction)?p.local_prefix:p.peer_prefix;
        if(!equal_prefix(data,8,sender))return false;
        std::size_t offset=20,count{};bool relevant{};
        while(offset<data.size())
        {
            if(++count>p.max_submessages||data.size()-offset<4)return false;
            auto id=data[offset],flags=data[offset+1];bool little=(flags&1u)!=0;
            std::size_t length=u16(data,offset+2,little);offset+=4;
            if(!length&&id!=0x09&&id!=0x01)length=data.size()-offset;
            if(length>data.size()-offset)return false;
            auto body=data.subspan(offset,length);offset+=length;
            std::size_t entities{};bool entity_message{},payload{};
            switch(id)
            {
                case 0x01:break; // PAD, including zero length.
                case 0x09:if((flags&2u)?length!=0:length!=8)return false;break;
                case 0x0c:if(length!=20||body[4]!=2||!equal_prefix(body,8,sender))return false;break;
                case 0x0e:if(length!=12||(!zero_prefix(body,0)&&!equal_prefix(body,0,target)))return false;break;
                case 0x80:
                {
                    // The pinned public FastDDS build appends its 56-byte
                    // network-statistics trailer even with no statistics reader.
                    // It is last, has a fixed vendor/layout, and grants no route.
                    if(!relevant||length!=56||offset!=data.size()||data[6]!=1||data[7]!=15)return false;
                    auto kind=u32(body,0,little),port=u32(body,4,little);
                    bool placeholder=kind==0&&port==0;
                    if(placeholder){for(std::size_t i=8;i<24;++i)if(body[i])return false;}
                    else
                    {
                        auto expected=receiving(direction)?(discovery(direction)?p.local_discovery:p.local_user):(discovery(direction)?p.peer_discovery:p.peer_user);
                        if(kind!=1||port!=expected)return false;
                        for(std::size_t i=8;i<20;++i)if(body[i])return false;
                        if(body[20]!=127||body[21]||body[22]||body[23]!=1)return false;
                    }
                    break;
                }
                case 0x06: // ACKNACK: bounded sequence bitmap plus count.
                    if(length<24||u32(body,16,little)>256||length!=24+4*((u32(body,16,little)+31)/32))return false;
                    entity_message=true;break;
                case 0x07:if(length!=28)return false;entity_message=true;break;
                case 0x08:
                    if(length<28||u32(body,24,little)>256||length!=28+4*((u32(body,24,little)+31)/32))return false;
                    entity_message=true;break;
                case 0x12:
                    if(length<28||u32(body,20,little)>256||length!=28+4*((u32(body,20,little)+31)/32))return false;
                    entity_message=true;break;
                case 0x13:if(length!=24)return false;entity_message=true;break;
                case 0x15:
                    if(length<20||u16(body,2,little)<16||u16(body,2,little)>length-4||(flags&12u)==12u)return false;
                    entities=4;entity_message=true;payload=true;break;
                case 0x16:
                {
                    if(length<32||u16(body,2,little)<28||u16(body,2,little)>length-4)return false;
                    auto start=u32(body,20,little),sample=u32(body,28,little);
                    auto fragments=u16(body,24,little),size=u16(body,26,little);
                    if(!start||!fragments||!size||!sample||sample>p.max_sample_bytes||
                       std::uint64_t(start-1)*size>=sample||std::uint64_t(start-1+std::uint64_t(fragments))*size>std::uint64_t(sample)+size-1)return false;
                    entities=4;entity_message=true;payload=true;break;
                }
                default:return false; // Includes arbitrary reply locators and secure extensions.
            }
            if(entity_message)
            {
                bool builtin=builtin_entity(body,entities+4);
                if(builtin!=discovery(direction)||(!unknown_entity(body,entities)&&builtin_entity(body,entities)!=builtin))return false;
                if(payload&&!builtin&&!(receiving(direction)?p.user_data_receive:p.user_data_transmit))return false;
                relevant=true;
            }
        }
        return relevant;
    }
}
