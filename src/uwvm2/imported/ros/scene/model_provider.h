/** @file
 * @brief Trusted native model service lifetime independent of guest handles.
 */
#pragma once
namespace uwvm2::imported::ros::scene::models
{
    /** @brief Own local provider activity until all VM-side access has stopped.
     * @details request_stop() is nonblocking and may run while authority is
     * revoked under its lock: it must not wait for a worker that reacquires
     * authority. retire() and destruction join local threads and close owned
     * channel duplicates, only outside authority/provider locks. An external
     * provider process remains deployment-owned. Joining this local object
     * proves neither remote completion nor erasure of disclosed copies; unknown
     * work keeps its existing reconciliation debt. The broker destroys this
     * owner before its dispatcher. Implementations may reference that dispatcher
     * but must not keep an owning reference back to the broker.
     *
     * Construct derived owners with utils::container::make_shared or an
     * equivalent recoverable fast_io allocator. The interface itself allocates
     * nothing and grants no permission to spawn an executable.
     */
    class provider_lifetime
    {
    public:
        provider_lifetime()=default;
        provider_lifetime(provider_lifetime const&)=delete;
        provider_lifetime& operator=(provider_lifetime const&)=delete;
        virtual void request_stop() noexcept=0;
        virtual void retire() noexcept=0;
        virtual ~provider_lifetime()=default;
    };
}
