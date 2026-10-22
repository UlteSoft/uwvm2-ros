/*************************************************************
 * UWVM2 explicit compiled ROS schema capacity. APL-2.0.      *
 *************************************************************/
#pragma once
#include <cstddef>
#include <cstdint>

#if !defined(UWVM_ROS_MAX_CUSTOM_INTERFACES)
#define UWVM_ROS_MAX_CUSTOM_INTERFACES 128
#endif
#if !defined(UWVM_ROS_MAX_CUSTOM_CELLS)
#define UWVM_ROS_MAX_CUSTOM_CELLS 16384
#endif

namespace uwvm2::imported::ros::ipc
{
    // This bounds trusted compiled declarations, not Guest rights or active
    // resources. Keep every Runtime/Worker/Gateway translation unit paired.
    // Byte, expanded-node, identity, schema and policy checks remain separate.
    static_assert(UWVM_ROS_MAX_CUSTOM_INTERFACES >= 1 && UWVM_ROS_MAX_CUSTOM_INTERFACES <= 512,
                  "explicit compiled ROS interface capacity must be 1..512");
    inline constexpr ::std::size_t custom_registry_max_interfaces{UWVM_ROS_MAX_CUSTOM_INTERFACES};
    static_assert(UWVM_ROS_MAX_CUSTOM_CELLS >= 1 && UWVM_ROS_MAX_CUSTOM_CELLS <= 1048576,
                  "explicit compiled ROS registry cell capacity must be 1..1048576");
    inline constexpr ::std::uint64_t custom_registry_max_cells{UWVM_ROS_MAX_CUSTOM_CELLS};

    [[nodiscard]] inline constexpr bool custom_registry_cells_fit(::std::uint64_t used,
        ::std::uint64_t added) noexcept
    {
        // Aggregate trusted declarations only. Per-record byte, node, depth,
        // identity and permission checks remain independent and unchanged.
        return used <= custom_registry_max_cells && added <= custom_registry_max_cells - used;
    }

    [[nodiscard]] inline constexpr bool custom_registry_counts_fit(::std::size_t topics,
        ::std::size_t services, ::std::size_t actions) noexcept
    {
        // Subtract only after the preceding bound succeeds. Malformed counts
        // must never wrap the aggregate or dereference a descriptor table.
        return topics <= custom_registry_max_interfaces &&
            services <= custom_registry_max_interfaces - topics &&
            actions <= custom_registry_max_interfaces - topics - services;
    }
}
