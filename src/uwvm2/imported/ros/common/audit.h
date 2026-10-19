/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once

#include <algorithm>
#include <iterator>
#include <chrono>
#include <deque>
#include <vector>
#include "message.h"

namespace uwvm2::imported::ros
{
    // Per-capability, explicit bounded audit retention. Overflow drops oldest
    // retained records, and quota suppression increments a visible counter.
    // The trusted host drains records; guests have no handle to this sink.
    class bounded_audit
    {
        ::std::deque<::std::string> records{};
        ::std::uint64_t maximum_records{}, maximum_bytes{}, bytes{}, suppressed{};
        double rate{}, burst{}, tokens{};
        ::std::chrono::steady_clock::time_point updated{::std::chrono::steady_clock::now()};
    public:
        result configure(capability_spec const& cap)
        {
            auto value = [&](::std::string_view key) -> ::std::string_view { auto it{cap.limits.find(key)}; return it == cap.limits.end() ? ::std::string_view{} : it->second; };
            ::std::uint64_t burst_integer{};
            if(!parse_positive_double(value("audit-max-rate-hz"), rate) || !parse_u64(value("audit-burst"), burst_integer) || !burst_integer ||
               !parse_u64(value("audit-max-records"), maximum_records) || !maximum_records || !parse_u64(value("audit-max-bytes"), maximum_bytes) || !maximum_bytes)
            { return {errc::invalid_argument, "all audit quotas must be explicit and positive"}; }
            if(rate > 1000000. || burst_integer > 1048576 || maximum_records > 1048576 || maximum_bytes > 67108864) { return {errc::invalid_argument, "audit quotas exceed public safety ceilings"}; }
            records.clear(); bytes = suppressed = 0; updated = ::std::chrono::steady_clock::now();
            burst = static_cast<double>(burst_integer); tokens = burst;
            return {};
        }
        bool record(::std::string entry)
        {
            auto now{::std::chrono::steady_clock::now()};
            auto elapsed{::std::clamp(::std::chrono::duration<double>(now - updated).count(), 0., 86400.)};
            tokens = ::std::min(burst, tokens + elapsed * rate); updated = now;
            if(tokens < 1. || entry.size() > maximum_bytes) { ++suppressed; return false; }
            tokens -= 1.;
            while(!records.empty() && (records.size() >= maximum_records || entry.size() > maximum_bytes - bytes))
            { bytes -= records.front().size(); records.pop_front(); ++suppressed; }
            bytes += entry.size(); records.push_back(::std::move(entry));
            return true;
        }
        void drain(::std::vector<::std::string>& out, ::std::uint64_t& dropped)
        {
            out.assign(::std::make_move_iterator(records.begin()), ::std::make_move_iterator(records.end()));
            records.clear(); bytes = 0; dropped = suppressed;
        }
        ::std::uint64_t retained_bytes() const noexcept { return bytes; }
        ::std::size_t retained_records() const noexcept { return records.size(); }
        ::std::uint64_t dropped_records() const noexcept { return suppressed; }
    };
}
