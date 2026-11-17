/** @file
 * @brief Canonical attachment-only messages between a VM and an external driver.
 * @details The deployment layer supplies the immutable origin, driver generation,
 * channel nonce and contract digest. Wasm bytes cannot choose them. One channel
 * admits one exact resource/operation/schema contract and one outstanding call.
 * These records convey no authority by themselves; authenticated transport and
 * independent receiver admission remain mandatory. Process creation, shutdown,
 * restart, update, arbitrary FFI, paths and descriptors have no wire operation.
 *
 * All scalars are little-endian u64; identifiers and digests are fixed words.
 * Decode validates the complete frame before publishing any output. There is
 * no allocation, unaligned native access or implicit protocol fallback.
 */
#pragma once
#include <array>
#include <algorithm>
#include <cstdint>
#include <limits>
#include "wire.h"

namespace uwvm2::imported::ros::scene::drivers::protocol
{
    inline constexpr std::uint64_t magic=UINT64_C(0x44534f52564d5755);
    inline constexpr std::size_t header_bytes=176;
    inline constexpr std::size_t maximum_payload=65536;
    enum class kind:std::uint64_t{hello=1,ready,request,result,release};
    using identity=std::array<std::uint64_t,2>;
    struct binding
    {
        identity deployment{},vm{},module{},driver{},session{};
        std::array<std::uint64_t,4> contract{};
        friend bool operator==(binding const&,binding const&)=default;
    };
    struct header
    {
        kind type{};binding bound;std::uint64_t sequence{},deadline{},status{};
        friend bool operator==(header const&,header const&)=default;
    };
    inline bool present(identity const& value)noexcept{return value[0]||value[1];}
    inline bool valid(binding const& value)noexcept
    {return present(value.deployment)&&present(value.vm)&&present(value.module)&&present(value.driver)&&present(value.session)&&
        std::any_of(value.contract.begin(),value.contract.end(),[](auto n){return n!=0;});}
    /** @brief Reject unknown kinds, flags, invalid sequencing and control payloads.
     * @details Readiness is a protocol observation, not a physical safety claim.
     * Request deadlines use the deployment-approved monotonic clock domain.
     * Result status is a bounded application result, never authority or a signal.
     */
    inline bool valid(header const& h,std::size_t payload)noexcept
    {
        if(!valid(h.bound)||payload>maximum_payload||static_cast<unsigned>(h.type)<1||static_cast<unsigned>(h.type)>5)return false;
        if(h.type==kind::hello||h.type==kind::ready||h.type==kind::release)return !h.sequence&&!h.deadline&&!h.status&&!payload;
        if(!h.sequence||h.sequence==UINT64_MAX||!h.deadline||h.deadline>INT64_MAX)return false;
        return h.type==kind::request?!h.status:h.status<=UINT32_MAX;
    }
    /** @brief Write the fixed header only after full output and input validation. */
    inline bool encode(header const& h,std::size_t payload,std::span<std::uint8_t> output)noexcept
    {
        if(output.size()!=header_bytes||!valid(h,payload))return false;
        wire::writer out{output};out.u64(magic);out.u64(header_bytes);out.u64(static_cast<std::uint64_t>(h.type));out.u64(0);
        for(auto const* id:{&h.bound.deployment,&h.bound.vm,&h.bound.module,&h.bound.driver,&h.bound.session})for(auto v:*id)out.u64(v);
        for(auto v:h.bound.contract)out.u64(v);
        out.u64(h.sequence);out.u64(h.deadline);out.u64(payload);out.u64(h.status);return out.good();
    }
    /** @brief Decode one whole packet transactionally; output remains unchanged on refusal. */
    inline bool decode(wire::bytes input,header& output,wire::bytes& payload)noexcept
    {
        if(input.size()<header_bytes||input.size()>header_bytes+maximum_payload)return false;
        wire::reader in{input.first(header_bytes)};std::uint64_t tag{},size{},type{},flags{},length{};header h;
        if(!in.u64(tag)||tag!=magic||!in.u64(size)||size!=header_bytes||!in.u64(type)||type<1||type>5||!in.u64(flags)||flags)return false;
        h.type=static_cast<kind>(type);
        for(auto* id:{&h.bound.deployment,&h.bound.vm,&h.bound.module,&h.bound.driver,&h.bound.session})for(auto& v:*id)if(!in.u64(v))return false;
        for(auto& v:h.bound.contract)if(!in.u64(v))return false;
        if(!in.u64(h.sequence)||!in.u64(h.deadline)||!in.u64(length)||length!=input.size()-header_bytes||!in.u64(h.status)||!in.done()||!valid(h,static_cast<std::size_t>(length)))return false;
        output=h;payload=input.subspan(header_bytes);return true;
    }
}
