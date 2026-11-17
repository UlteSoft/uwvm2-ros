/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/control/host_interfaces.h>
#include <uwvm2/utils/control/host_diagnostics.h>

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace uwvm2::imported::ros
{
    enum class version : ::std::uint8_t { ros1 = 1, ros2 = 2 };
    enum class scope : ::std::uint8_t { global, single, group };
    enum class errc : ::std::uint32_t
    {
        success = 0, unknown_module = 2, io_error = 5, bad_handle = 9, rate_limited = 11, out_of_memory = 12, permission_denied = 13,
        conflict = 17, invalid_argument = 22, too_large = 90, unsupported = 95, disconnected = 107,
        timeout = 110, revoked = 125, cancelled = 125
    };

    struct result
    {
        errc code{errc::success};
        ::uwvm2::utils::container::std_string detail{};
        explicit operator bool() const noexcept { return code == errc::success; }
    };

    inline result quota_failure(errc code,::std::string_view detail,::std::string_view field,::std::string_view alias={})
    {
        ::uwvm2::utils::control::host_diagnostics::limited(field,false,alias);
        return {code,::uwvm2::utils::container::std_string{detail}};
    }
    struct capability_spec
    {
        ::uwvm2::utils::container::std_string alias{};
        ::uwvm2::utils::container::std_string operation{};
        ::uwvm2::utils::container::std_string resource{};
        ::uwvm2::utils::container::std_string type{};
        ::uwvm2::utils::container::std_string type_identity{};
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string, ::uwvm2::utils::container::std_string, ::std::less<>> limits{};
    };

    struct target_policy
    {
        version ros_version{version::ros1};
        scope target_scope{scope::global};
        ::uwvm2::utils::container::std_string name{};
        bool enabled{};
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string, ::uwvm2::utils::container::std_string, ::std::less<>> connection{};
        ::uwvm2::utils::container::std_map<::uwvm2::utils::container::std_string, capability_spec, ::std::less<>> capabilities{};
        ::uwvm2::utils::control::host_interfaces::policy interfaces{};
    };

    constexpr ::std::string_view to_string(version v) noexcept { return v == version::ros1 ? "ros1" : "ros2"; }
    constexpr ::std::string_view to_string(scope s) noexcept
    {
        switch(s) { case scope::global: return "global"; case scope::single: return "single"; case scope::group: return "group"; }
        return "invalid";
    }
    constexpr bool valid_version(version v) noexcept { return v == version::ros1 || v == version::ros2; }
}
