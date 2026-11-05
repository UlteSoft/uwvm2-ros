/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "types.h"
#include <limits>
#include <string_view>
namespace uwvm2::imported::ros
{
    inline constexpr ::std::uint64_t unlimited_quota{UINT64_MAX};
    // A missing quota is unlimited. Resource/type/QoS/security fields are not
    // quotas and remain independently explicit; no grant is inferred here.
    inline bool resource_quota_field(::std::string_view key) noexcept
    {
        return key.substr(0,4)=="max-" || key=="burst" || key=="queue-depth" || key=="timeout-ms" ||
            key=="audit-max-rate-hz" || key=="audit-burst" || key=="audit-max-records" || key=="audit-max-bytes" ||
            (key.substr(0,4)=="qos." && key.size()>6 && key.substr(key.size()-6)==".depth");
    }
    inline bool replenishing_quota(::std::string_view key) noexcept
    {
        return key=="max-rate-hz" || key=="max-bytes-per-second" || key=="max-log-bytes-per-second" ||
            key=="queue-depth" || key=="max-queue-bytes" || key=="max-event-queue-depth" || key=="max-event-queue-bytes" ||
            key=="max-pending-events" || key=="max-pending-event-bytes" || key=="max-inflight" ||
            key=="max-publishers" || key=="max-subscribers" || key=="max-services" || key=="max-actions" ||
            key=="max-active-goals" || key=="max-native-clients" || key=="audit-max-rate-hz" ||
            (key.substr(0,4)=="qos." && key.size()>6 && key.substr(key.size()-6)==".depth");
    }
    inline bool quota_integer(::std::string_view value, ::std::uint64_t& out) noexcept
    {
        if(value=="unlimited") { out=unlimited_quota; return true; }
        if(value.empty()) { return false; }
        ::std::uint64_t result{};
        for(char c:value)
        {
            if(c<'0'||c>'9') { return false; }
            auto const digit{static_cast<unsigned>(c-'0')};
            if(result>(UINT64_MAX-digit)/10u) { return false; }
            result=result*10u+digit;
        }
        out=result; return true;
    }
    inline bool read_quota(capability_spec const& cap, ::std::string_view key, ::std::uint64_t& out) noexcept
    {
        auto const found{cap.limits.find(key)};
        if(found==cap.limits.end())
        { if(!resource_quota_field(key)) { return false; } out=unlimited_quota; return true; }
        return quota_integer(found->second,out);
    }
    inline ::std::uint64_t quota_value(capability_spec const& cap, ::std::string_view key) noexcept
    { ::std::uint64_t value{}; (void)read_quota(cap,key,value); return value; }
    inline ::std::string_view quota_text(capability_spec const& cap, ::std::string_view key) noexcept
    {
        auto const found{cap.limits.find(key)};
        if(found!=cap.limits.end()) { return found->second; }
        return resource_quota_field(key)?::std::string_view{"18446744073709551615"} : ::std::string_view{};
    }
}
