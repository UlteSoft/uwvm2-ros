/*************************************************************
 * UWVM2 explicit native service-client availability. APL-2.0.*
 *************************************************************/
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

namespace uwvm2::imported::ros::ipc::client_availability
{
    // NEW operation only; existing IPC/event/business formats are unchanged.
    // Trusted Host supplies this envelope AFTER both actual capability leases.
    // Generation fields are associations, not independent IPC credentials.
    inline constexpr std::size_t bytes{64};
    inline constexpr std::uint64_t maximum_wait_ns{60000000000ULL};
    enum class command : std::uint8_t { features=0,query=1,retire=2 };
    enum class retirement : std::uint8_t
    {none=0,availability_handle=1,source_handle=2,availability_alias=4,source_alias=8};
    inline constexpr std::uint64_t typed_query{1},finite_wait{2},actual_qos_check{4},typed_md5_header{8};
    struct context
    {
        std::uint8_t backend{};
        std::uint64_t availability{},source{},instance{},runtime_generation{};
    };
    struct packet
    {
        command operation{command::features};retirement scope{retirement::none};
        std::uint32_t reason{};context binding{};
        // Request wait ns; reply feature bits/bool/zero private-retire ack.
        std::uint64_t value{};
    };
    inline bool same(context const& a,context const& b) noexcept
    {return a.backend==b.backend&&a.availability==b.availability&&a.source==b.source&&a.instance==b.instance&&a.runtime_generation==b.runtime_generation;}
    inline bool full_handle(std::uint64_t h) noexcept
    {return static_cast<std::uint32_t>(h)&&static_cast<std::uint32_t>(h>>32);}
    inline bool valid(context const& c) noexcept
    {return (c.backend==1||c.backend==2)&&full_handle(c.availability)&&full_handle(c.source)&&static_cast<std::uint32_t>(c.availability)!=static_cast<std::uint32_t>(c.source)&&c.instance&&c.runtime_generation;}
    inline std::uint64_t get(std::uint8_t const* p,unsigned n) noexcept
    {std::uint64_t x{};for(unsigned i{};i!=n;++i){x|=std::uint64_t{p[i]}<<(8*i);}return x;}
    inline void put(std::uint8_t* p,std::uint64_t x,unsigned n) noexcept
    {for(unsigned i{};i!=n;++i){p[i]=static_cast<std::uint8_t>(x>>(8*i));}}
    inline bool valid_features(unsigned backend,std::uint64_t value) noexcept
    {return (backend==1||backend==2)&&(value==0||value==(typed_query|finite_wait|(backend==1?typed_md5_header:actual_qos_check)));}
    inline bool valid_retirement(retirement s) noexcept
    {return s==retirement::availability_handle||s==retirement::source_handle||s==retirement::availability_alias||s==retirement::source_alias;}
    inline bool header(packet const& p) noexcept
    {
        if(!valid(p.binding)){return false;}
        if(p.operation==command::retire){return valid_retirement(p.scope)&&(p.reason==0||p.reason==125)&&!p.value;}
        return (p.operation==command::features||p.operation==command::query)&&p.scope==retirement::none&&!p.reason;
    }
    inline bool valid_request(packet const& p) noexcept
    {return header(p)&&(p.operation==command::retire||(p.operation==command::features?!p.value:p.value<=maximum_wait_ns));}
    inline bool valid_response(packet const& p) noexcept
    {return header(p)&&(p.operation==command::retire||(p.operation==command::query?p.value<=1:valid_features(p.binding.backend,p.value)));}
    inline std::array<std::uint8_t,bytes> encode(packet const& p) noexcept
    {
        std::array<std::uint8_t,bytes> b{};b[0]=1;b[1]=static_cast<std::uint8_t>(p.operation);b[2]=p.binding.backend;b[3]=static_cast<std::uint8_t>(p.scope);
        put(b.data()+4,p.reason,4);put(b.data()+8,p.binding.availability,8);put(b.data()+16,p.binding.source,8);
        put(b.data()+24,p.binding.instance,8);put(b.data()+32,p.binding.runtime_generation,8);put(b.data()+40,p.value,8);return b;
    }
    inline bool decode(std::uint8_t const* b,std::size_t size,packet& output,bool response) noexcept
    {
        if(!b||size!=bytes||b[0]!=1||b[1]>2){return false;}
        for(unsigned i{48};i!=64;++i){if(b[i]){return false;}}
        packet p{static_cast<command>(b[1]),static_cast<retirement>(b[3]),static_cast<std::uint32_t>(get(b+4,4)),
            {b[2],get(b+8,8),get(b+16,8),get(b+24,8),get(b+32,8)},get(b+40,8)};
        if(response?!valid_response(p):!valid_request(p)){return false;}output=p;return true;
    }
    inline bool decode_request(std::uint8_t const* b,std::size_t n,packet& output) noexcept
    {return decode(b,n,output,false);}
    inline bool decode_response(std::uint8_t const* b,std::size_t n,packet const& request,packet& output) noexcept
    {
        packet p{};
        if(!valid_request(request)||!decode(b,n,p,true)||!same(p.binding,request.binding)||p.operation!=request.operation||p.scope!=request.scope||p.reason!=request.reason){return false;}
        output=p;return true;
    }
}
