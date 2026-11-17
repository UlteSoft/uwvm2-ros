/** @file
 * @brief Trusted communication-provider retirement and bilateral activation.
 * @details These objects are native deployment machinery, never guest-carried
 * authority. The broker retains the lifetime until stop and drain complete,
 * before destroying dispatchers whose resources the provider may reference.
 */
#pragma once
#include "communication.h"
#include "authority.h"
namespace uwvm2::imported::ros::scene::communication
{
    /** @brief Retain workers and cleanup debt independently of guest handles.
     * @details request_stop is allocation-free and nonblocking under revocation.
     * retire runs outside authority/registry locks, drains RPC and relay users,
     * then reaps workers. A false cleanup_confirmed is retained debt, including
     * an early launcher failure without a verified namespace-init identity; no
     * timeout, launcher exit or destructor turns that uncertainty into success.
     */
    class provider_lifetime
    {
    public:
        provider_lifetime()=default;provider_lifetime(provider_lifetime const&)=delete;
        provider_lifetime& operator=(provider_lifetime const&)=delete;
        virtual void request_stop() noexcept=0;
        virtual void retire() noexcept=0;
        virtual bool cleanup_confirmed() const noexcept=0;
        virtual ~provider_lifetime()=default;
    };
    /** @brief Exact immutable origins retained by each staged node-open attempt. */
    struct activation_origin
    {node_plan const* node{};context_plan const* context{};node_stage_request const* request{};};
    /** @brief Two current admission groups held until the private link activates.
     * @details Both groups must belong to the SAME authority environment. The
     * callback checks each caller incarnation and both exact capabilities using
     * authority::admit_group with validation-only node.open/context.open claims.
     * The first recursive lease remains held while the second is checked, so a
     * revoker cannot interleave. There are no cross-environment lock orders or
     * inferred permissions. The callback is trusted and may not retain origins.
     */
    struct activation_guard
    {
        std::array<group_lease,2> participants;
        explicit operator bool()const noexcept
        {return participants[0].size()>=2&&participants[1].size()>=2;}
    };
    using admit_pair=error(*)(void*,std::array<activation_origin,2> const&,activation_guard&)noexcept;
}
