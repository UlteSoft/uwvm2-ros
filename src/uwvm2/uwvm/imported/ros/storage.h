/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
# if defined(UWVM_MODULE) && !defined(UWVM2_ROS_POLICY_STATE_OWNER)
#  error "ROS host imports currently require the header build (use-cxx-module=n)."
# endif
# ifndef UWVM_MODULE
# include <uwvm2/imported/ros/policy/policy.h>
# endif
namespace uwvm2::uwvm::imported::ros
{
    // Written by registered CLI callbacks, frozen before Guest execution.
    inline ::uwvm2::imported::ros::policy_store runtime_policy{};
    inline bool check_policy[2]{};
    inline bool dump_policy[2]{};
    inline bool policy_configured{};
    inline bool policy_any_enabled{};
    template<typename Text>
    inline bool host_import_visible(Text module, Text name_space, Text function) noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        auto view = [](Text value) noexcept { return ::std::string_view{reinterpret_cast<char const*>(value.data()), value.size()}; };
        (void)function; // Function identity/signature is resolved by the immutable host registry.
        auto const ns{view(name_space)};
        if(!ns.starts_with("uwvm_ros1_") && !ns.starts_with("uwvm_ros2_") && ns != "uwvm_portable_robot_v1") { return true; }
        auto const version{ns.starts_with("uwvm_ros1_") ? ::uwvm2::imported::ros::version::ros1 : ::uwvm2::imported::ros::version::ros2};
        return runtime_policy.host_import_visible(version, view(module), ns);
#else
        (void)module; (void)name_space; (void)function; return true;
#endif
    }
}
#endif
