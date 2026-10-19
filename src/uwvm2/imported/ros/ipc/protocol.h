/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <cstddef>
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <limits>

namespace uwvm2::imported::ros::ipc
{
    inline constexpr std::uint32_t magic{0x32524f55}; // "UOR2" on the wire
    inline constexpr std::uint16_t abi_version{1};
    inline constexpr std::size_t header_bytes{48};
    inline constexpr std::uint32_t max_payload_bytes{4u * 1024u * 1024u};
    inline constexpr std::uint32_t max_timeout_ms{60000};
    enum class kind : std::uint16_t { hello=1, request=2, response=3, cancel=4, close=5 };
    struct frame
    {
        kind type{kind::request};
        std::uint8_t backend{};
        std::uint64_t request_id{}, response_id{}, session_id{};
        std::uint32_t error{}, timeout_ms{};
        std::vector<std::uint8_t> payload{};
    };
    inline void put(std::vector<std::uint8_t>& b, std::uint64_t v, unsigned n)
    {
        for(unsigned i{}; i != n; ++i) { b.push_back(static_cast<std::uint8_t>(v >> (8u * i))); }
    }
    inline std::uint64_t get(std::uint8_t const* b, unsigned n) noexcept
    {
        std::uint64_t v{};
        for(unsigned i{}; i != n; ++i) { v |= std::uint64_t{b[i]} << (8u * i); }
        return v;
    }
    inline std::array<std::uint8_t,header_bytes> encode_header_fixed(frame const& f) noexcept
    {
        std::array<std::uint8_t,header_bytes> b{};std::size_t at{};
        auto field=[&](std::uint64_t value,unsigned width) {
            for(unsigned i{};i<width;++i) { b[at++]=static_cast<std::uint8_t>(value>>(i*8u)); }
        };
        field(magic,4);field(abi_version,2);field(static_cast<std::uint16_t>(f.type),2);
        field(f.backend,1);field(0,3);field(f.request_id,8);field(f.response_id,8);field(f.session_id,8);
        field(f.payload.size(),4);field(f.error,4);field(f.timeout_ms,4);return b;
    }
    inline std::vector<std::uint8_t> encode_header(frame const& f)
    {
        auto bytes{encode_header_fixed(f)};return {bytes.begin(),bytes.end()};
    }
    inline bool decode_header(std::uint8_t const* b, std::size_t n, frame& f, std::uint32_t& length) noexcept
    {
        if(n != header_bytes || get(b,4) != magic || get(b+4,2) != abi_version || b[8] < 1 || b[8] > 2 || get(b+9,3)) { return false; }
        auto k{get(b+6,2)};
        if(k < 1 || k > 5) { return false; }
        length=static_cast<std::uint32_t>(get(b+36,4));
        if(length > max_payload_bytes || get(b+44,4) == 0 || get(b+44,4) > max_timeout_ms) { return false; }
        f.type=static_cast<kind>(k); f.backend=b[8];
        f.request_id=get(b+12,8); f.response_id=get(b+20,8); f.session_id=get(b+28,8);
        f.error=static_cast<std::uint32_t>(get(b+40,4)); f.timeout_ms=static_cast<std::uint32_t>(get(b+44,4));
        return true;
    }
    struct request
    {
        std::string operation{}, alias{};
        std::vector<std::uint8_t> data{};
    };
    // Borrow metadata and data only for the synchronous encoder call; the output owns its bytes.
    struct request_view
    {
        std::string_view operation{}, alias{};
        std::vector<std::uint8_t> const& data;
    };
    inline bool valid_token_view(std::string_view s) noexcept
    {
        if(s.empty() || s.size()>255) { return false; }
        for(auto c:s) { if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.')) { return false; } }
        return true;
    }
    inline bool valid_token(std::string const& s) noexcept { return valid_token_view(s); }
    inline bool encode_request_view(request_view const& r, std::vector<std::uint8_t>& b)
    {
        if(!valid_token_view(r.operation)||!valid_token_view(r.alias)||r.data.size()>max_payload_bytes-518) { return false; }
        b.clear(); b.reserve(8+r.operation.size()+r.alias.size()+r.data.size()); put(b,r.operation.size(),2); put(b,r.alias.size(),2); put(b,r.data.size(),4);
        b.insert(b.end(),r.operation.begin(),r.operation.end()); b.insert(b.end(),r.alias.begin(),r.alias.end()); b.insert(b.end(),r.data.begin(),r.data.end());
        return true;
    }
    inline bool encode_request(request const& r, std::vector<std::uint8_t>& b)
    {
        return encode_request_view({r.operation,r.alias,r.data},b);
    }
    inline bool decode_request(std::vector<std::uint8_t> const& b, request& r)
    {
        if(b.size()<8) { return false; }
        auto o{get(b.data(),2)},a{get(b.data()+2,2)},d{get(b.data()+4,4)};
        if(o>255||a>255||o+a+8>b.size()||d!=b.size()-8-o-a) { return false; }
        r.operation.assign(reinterpret_cast<char const*>(b.data()+8),static_cast<std::size_t>(o));
        r.alias.assign(reinterpret_cast<char const*>(b.data()+8+o),static_cast<std::size_t>(a));
        if(!valid_token(r.operation)||!valid_token(r.alias)) { return false; }
        r.data.assign(b.begin()+static_cast<std::ptrdiff_t>(8+o+a),b.end());
        return true;
    }
    struct call_result { std::uint32_t error{}; std::vector<std::uint8_t> payload{}; };
}
