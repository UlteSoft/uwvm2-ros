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
}
#endif
