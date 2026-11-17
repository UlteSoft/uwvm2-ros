/** @file
 * @brief Trusted scene retirement notifications, outside every guest ABI.
 * @details Call only after leaving runtime publication locks. Retirement does
 * not grant authority; the host callback revokes membership or the environment
 * and wakes pending waits. Old ROS hooks are independent and unchanged.
 */
#pragma once
#include <atomic>
#include <cstdint>
/** @brief Share the trusted retirement hook between module and header runtime glue.
 * @details Explicit C++ language linkage retains global-module attachment in
 * the scene model interface. Retirement must reach the installed callback
 * regardless of whether its caller was built with textual or named imports.
 */
extern "C++" {
namespace uwvm2::imported::ros::scene::lifecycle
{
    using callback=void(*)(void const*,std::uint64_t,bool) noexcept;
    inline std::atomic<callback> hook{};
    inline void retire(void const* instance,std::uint64_t generation,bool all) noexcept
    {if(auto notify=hook.load(std::memory_order_acquire))notify(instance,generation,all);}
}
}
