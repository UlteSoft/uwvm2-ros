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
     * A function-replacement notification closes authority only after the
     * Runtime has explicitly retired that module's existing incarnation.
     * DEV replacement within an authorized incarnation preserves its policy
     * and quota accounting. This hook never grants authority, replenishes a
     * quota, or reopens a retired alias; retirement requires a fresh trusted
     * launch with explicit policy before ROS can be used again.
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
