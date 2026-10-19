/*************************************************************
 * UWVM2 trusted ROS management lifecycle. APL-2.0.           *
 *************************************************************/
#pragma once
#include <atomic>
#include <cstdint>

namespace uwvm2::imported::ros::management
{
    /** Native management notifications, deliberately outside the Guest ABI.
     *
     * A function replacement permanently retires the affected module's ROS
     * authority for its existing Runtime incarnation. It does not reconfigure
     * policy, replenish a quota, reopen a revoked alias or authorize new code.
     * Reusing ROS requires a fresh trusted launch and explicit launch policy.
     * Runtime retirement is emitted only after actual execution drain, before
     * the Runtime epoch or module storage is destroyed.
     *
     * The caller must leave the stopped-domain/publication mutex before calling
     * retire(): an already admitted native ROS operation may own a Host mutex.
     * The Host callback waits for those existing leases, closes local IPC file
     * descriptors and discards queues; it never performs an RPC or Guest call.
     */
    enum class transition { function_replacement, runtime_retirement };
    using retire_function = void(*)(transition,void const*,std::uint64_t) noexcept;
    inline std::atomic<retire_function> retirement_hook{};

    inline void retire(transition reason,void const* object,std::uint64_t epoch) noexcept
    {
        auto callback{retirement_hook.load(std::memory_order_acquire)};
        if(callback) { callback(reason,object,epoch); }
    }
}
