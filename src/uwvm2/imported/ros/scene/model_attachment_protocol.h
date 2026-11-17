/** @file
 * @brief Authenticated, bounded model messages for deployment-owned providers.
 * @details A dedicated provisioned seqpacket channel carries this closed
 * protocol. The magic separates it from driver traffic. All identities and the
 * approved contract digest come from trusted host configuration. They confer
 * no authority. Request/response fragments are contiguous, never interleaved,
 * and at most 65536 bytes each. End frames bind the exact aggregate length.
 * Model payloads use model_worker_protocol; each response is one bounded
 * partial or terminal result. No filename, FD, executable or lifecycle command
 * is accepted. Release only abandons this session, never stops the provider.
 */
#pragma once
#include "driver_protocol.h"
namespace uwvm2::imported::ros::scene::model_attachment::protocol
{
    inline constexpr std::uint64_t magic=UINT64_C(0x4d534f52564d5755);
    inline constexpr std::size_t header_bytes=176,fragment_bytes=65536,maximum_message=4*1024*1024;
    using identity=drivers::protocol::identity;
    struct binding
    {
        identity deployment{},vm{},scope{},provider{},session{};
        std::array<std::uint64_t,4> contract{};
        friend bool operator==(binding const&,binding const&)=default;
    };
    enum class kind:std::uint64_t{hello=1,ready,release,request,request_end,response,response_end};
    struct header
    {
        kind type{};binding bound;std::uint64_t sequence{},deadline{},offset{};
        friend bool operator==(header const&,header const&)=default;
    };
    inline bool valid(binding const& value)noexcept
    {return drivers::protocol::present(value.deployment)&&drivers::protocol::present(value.vm)&&
        drivers::protocol::present(value.scope)&&drivers::protocol::present(value.provider)&&drivers::protocol::present(value.session)&&
        std::any_of(value.contract.begin(),value.contract.end(),[](auto n){return n!=0;});}
    inline bool valid(header const& h,std::size_t length)noexcept
    {
        auto type=static_cast<unsigned>(h.type);
        if(!valid(h.bound)||type<1||type>7||length>fragment_bytes)return false;
        if(type<=3)return !h.sequence&&!h.deadline&&!h.offset&&(h.type==kind::release?!length:length<=1024);
        if(!h.sequence||h.sequence==UINT64_MAX||!h.deadline||h.deadline>INT64_MAX||h.offset>maximum_message)return false;
        if(h.type==kind::request_end||h.type==kind::response_end)return !length&&h.offset;
        return length&&length<=maximum_message-h.offset;
    }
    /** @brief Encode only the fixed endian-independent header after full validation. */
    inline bool encode(header const& h,std::size_t length,std::span<std::uint8_t> output)noexcept
    {
        if(output.size()!=header_bytes||!valid(h,length))return false;
        wire::writer out{output};out.u64(magic);out.u64(header_bytes);out.u64(static_cast<std::uint64_t>(h.type));out.u64(0);
        for(auto const* id:{&h.bound.deployment,&h.bound.vm,&h.bound.scope,&h.bound.provider,&h.bound.session})for(auto n:*id)out.u64(n);
        for(auto n:h.bound.contract)out.u64(n);
        out.u64(h.sequence);out.u64(h.deadline);out.u64(length);out.u64(h.offset);return out.good();
    }
    /** @brief Validate the entire frame transactionally, including exact body length. */
    inline bool decode(wire::bytes input,header& output,wire::bytes& body)noexcept
    {
        if(input.size()<header_bytes||input.size()>header_bytes+fragment_bytes)return false;
        wire::reader in{input.first(header_bytes)};header h;std::uint64_t tag{},bytes{},type{},flags{},length{};
        if(!in.u64(tag)||tag!=magic||!in.u64(bytes)||bytes!=header_bytes||!in.u64(type)||type<1||type>7||!in.u64(flags)||flags)return false;
        h.type=static_cast<kind>(type);
        for(auto* id:{&h.bound.deployment,&h.bound.vm,&h.bound.scope,&h.bound.provider,&h.bound.session})for(auto& n:*id)if(!in.u64(n))return false;
        for(auto& n:h.bound.contract)if(!in.u64(n))return false;
        if(!in.u64(h.sequence)||!in.u64(h.deadline)||!in.u64(length)||!in.u64(h.offset)||!in.done()||
            length!=input.size()-header_bytes||!valid(h,static_cast<std::size_t>(length)))return false;
        output=h;body=input.subspan(header_bytes);return true;
    }
}
