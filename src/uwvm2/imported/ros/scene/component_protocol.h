/** @file
 * @brief Closed pure-message ABI for an isolated component and its role proxy.
 * @details The child sees role slots and immutable configuration, never guest
 * capability values, host paths, native descriptors, loader flags or a generic
 * host-call selector. A role is a typed topic publisher or subscriber. Request
 * sequences are contiguous; one request is in flight. The host supplies native
 * endpoint identities, transaction keys and steady deadlines. Publication
 * acceptance, receive reservation and explicit acknowledgement remain distinct.
 */
#pragma once
#include "robot_provider_contract.h"
namespace uwvm2::imported::ros::scene::component_protocol
{
    inline constexpr std::string_view ready="uwvm.component.filtered\n",attached="uwvm.component.attached\n";
    enum class action:std::uint64_t {publish=1,reserve=2,acknowledge=3,release=4};
    enum class frame:std::uint64_t {call=1,finished=2,started=3};
    struct role {std::uint64_t slot{};bool publisher{};robot_contract::profile selected;};
    struct invocation
    {
        std::uint64_t generation{},instance{};robot_contract::profile selected;wire::bytes configuration;
        std::array<role,16> roles{};std::size_t count{};
    };
    /** @brief Validate the complete authenticated startup record without allocation. */
    inline bool decode(wire::bytes bytes,invocation& output)noexcept
    {
        robot_contract::decoder in{bytes,{bytes.size(),bytes.size(),255,16}};invocation value;std::uint64_t count{};
        if(!in.identity(value.generation)||!in.identity(value.instance)||!in.selected(value.selected)||!in.input().blob(value.configuration)||
            !in.number(count)||count>value.roles.size())return false;
        value.count=static_cast<std::size_t>(count);
        for(std::size_t i{};i<value.count;++i)
        {auto& role=value.roles[i];if(!in.identity(role.slot)||role.slot!=i+1||!in.input().boolean(role.publisher)||!in.selected(role.selected))return false;}
        if(!in.done()){return false;}output=value;return true;
    }
    /** @brief Synchronous child-only callback; borrowed request/reply storage lasts for the call.
     * @details No host pointer is transmitted. Response bytes use the canonical
     * ROSv2 topic receipt/reservation record. A nonzero status is an explicit
     * refusal or unresolved attempt, never permission to invent a new key.
     */
    using call_function=std::uint32_t(*)(void*,std::uint64_t,action,wire::bytes,std::span<std::uint8_t>,std::size_t&)noexcept;
    using backend_function=bool(*)(invocation const&,call_function,void*)noexcept;
}
