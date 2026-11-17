/** @file
 * @brief Finite, exactly representable resource ceilings for isolated children.
 * @details Trusted deployment amounts are unsigned 64-bit integers, whereas
 * the libc resource-limit type depends on the target ABI. Narrowing or an
 * infinity sentinel must never turn a finite reservation into unlimited access.
 * Preparation rejects unrepresentable amounts; the gated child repeats the
 * check and verifies both kernel limits before executing the sealed launcher.
 * Failure is irreversible in that child: its caller must exit, without running
 * provider code or retrying with a weaker limit. No VM limit or cgroup changes.
 */
#pragma once
#include <cstdint>
#include <limits>
#if defined(__linux__)
#include <sys/resource.h>
#endif

namespace uwvm2::imported::ros::scene::linux_resource_limits
{
    /** @brief Reject truncation and the ABI's unlimited sentinel before casting.
     * @details Zero is a valid ceiling, used for core dumps and locked memory.
     * This does not grant a process or replace per-resource policy validation.
     */
    template<typename NativeLimit>
    constexpr bool representable(std::uint64_t amount,NativeLimit infinity)noexcept
    {
        static_assert(std::numeric_limits<NativeLimit>::is_integer);
        static_assert(!std::numeric_limits<NativeLimit>::is_signed);
        return amount<=std::numeric_limits<NativeLimit>::max()&&
            static_cast<NativeLimit>(amount)!=infinity;
    }
#if defined(__linux__)
    constexpr bool finite(std::uint64_t amount)noexcept
    {return representable(amount,static_cast<rlim_t>(RLIM_INFINITY));}

    /** @brief Set equal finite soft/hard ceilings and verify exact kernel readback.
     * @details Call only in the owned, gated post-clone child before any external
     * instructions. These calls allocate no storage. A libc/kernel conversion,
     * denied setter, failed readback or changed value refuses launch. The helper
     * performs no fallback, limit restoration, resource admission or signalling.
     * Startup-only verification adds no syscall to the steady-state IPC path.
     */
    inline bool apply(int resource,std::uint64_t amount)noexcept
    {
        if(!finite(amount))return false;
        auto value=static_cast<rlim_t>(amount);
        rlimit selected{value,value},observed{};
        if(::setrlimit(resource,&selected)||::getrlimit(resource,&observed))return false;
        return observed.rlim_cur==value&&observed.rlim_max==value;
    }
#endif
}
