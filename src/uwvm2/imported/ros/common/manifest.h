/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <vector>
#include "types.h"

namespace uwvm2::imported::ros
{
    // Protocol bytes, independent of STL/C++ ABI and accepted by C++17 gateways.
    // Scope/name share policy only: they are not gateway security principals.
    // Socket mount paths, expected peer uid and connection timeout are worker-local.
    inline ::std::vector<::std::uint8_t> binding_manifest(target_policy const& target)
    {
        ::std::vector<::std::uint8_t> out{'U', 'W', 'R', 'P', 1, static_cast<::std::uint8_t>(target.ros_version), static_cast<::std::uint8_t>(target.enabled)};
        auto integer = [&](::std::uint32_t n) { for(unsigned i{}; i != 4; ++i) { out.push_back(static_cast<::std::uint8_t>(n >> (8 * i))); } };
        auto string = [&](::std::string_view text) { integer(static_cast<::std::uint32_t>(text.size())); out.insert(out.end(), text.begin(), text.end()); };
        auto backend_field = [](::std::string_view key) { return key != "gateway-socket" && key != "gateway-uid" && key != "connect-timeout-ms" && key != "call-boundary" && key.substr(0, 11) != "audit-deny-"; };
        ::std::uint32_t connection_count{};
        for(auto const& item : target.connection) { if(backend_field(item.first)) { ++connection_count; } }
        integer(connection_count);
        for(auto const& item : target.connection) { if(backend_field(item.first)) { string(item.first); string(item.second); } }
        integer(static_cast<::std::uint32_t>(target.capabilities.size()));
        for(auto const& item : target.capabilities)
        {
            auto const& cap{item.second};
            string(item.first); string(cap.alias); string(cap.operation); string(cap.resource); string(cap.type); string(cap.type_identity);
            integer(static_cast<::std::uint32_t>(cap.limits.size()));
            for(auto const& limit : cap.limits) { string(limit.first); string(limit.second); }
        }
        return out;
    }
    inline ::std::vector<::std::uint8_t> serialize_manifest(target_policy const& target) { return binding_manifest(target); }
}
