/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <string_view>

namespace uwvm2::imported::ros
{
    enum class version : ::std::uint8_t { ros1 = 1, ros2 = 2 };
    enum class scope : ::std::uint8_t { global, single, group };
    enum class errc : ::std::uint32_t
    {
        success = 0, unknown_module = 2, bad_handle = 9, rate_limited = 11, permission_denied = 13,
        conflict = 17, invalid_argument = 22, too_large = 90, unsupported = 95, disconnected = 107,
        timeout = 110, revoked = 125
    };

    struct result
    {
        errc code{errc::success};
        ::std::string detail{};
        explicit operator bool() const noexcept { return code == errc::success; }
    };

    struct capability_spec
    {
        ::std::string alias{};
        ::std::string operation{};
        ::std::string resource{};
        ::std::string type{};
        ::std::string type_identity{};
        ::std::map<::std::string, ::std::string, ::std::less<>> limits{};
    };

    struct target_policy
    {
        version ros_version{version::ros1};
        scope target_scope{scope::global};
        ::std::string name{};
        bool enabled{};
        ::std::map<::std::string, ::std::string, ::std::less<>> connection{};
        ::std::map<::std::string, capability_spec, ::std::less<>> capabilities{};
    };

    constexpr ::std::string_view to_string(version v) noexcept { return v == version::ros1 ? "ros1" : "ros2"; }
    constexpr ::std::string_view to_string(scope s) noexcept
    {
        switch(s) { case scope::global: return "global"; case scope::single: return "single"; case scope::group: return "group"; }
        return "invalid";
    }
    constexpr bool valid_version(version v) noexcept { return v == version::ros1 || v == version::ros2; }
}
