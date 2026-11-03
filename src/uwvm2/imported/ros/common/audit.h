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
#include <limits>
#include "message.h"

namespace uwvm2::imported::ros
{
    enum class audit_record_status : unsigned char { retained, unconfigured, rate_limited, oversized, allocation_failed, sequence_exhausted };
    struct audit_statistics
    {
        ::std::uint64_t attempted{}, accepted{}, overwritten{}, rate_limited{}, oversized{}, unconfigured{}, allocation_failed{}, sequence_exhausted{}, drain_failed{};
        bool saturated{};
    };
    // Per-capability, explicit bounded audit retention. Overflow drops oldest
    // retained records, and quota suppression increments a visible counter.
    // The trusted host drains records; guests have no handle to this sink.
    class bounded_audit
    {
        ::std::deque<::std::string> records{};
        ::std::uint64_t maximum_records{}, maximum_bytes{}, bytes{}, suppressed{};
        double rate{}, burst{}, tokens{};
        audit_statistics statistics_{};
        audit_record_status last_status_{audit_record_status::unconfigured};
        bool configured_{};
        ::std::chrono::steady_clock::time_point updated{::std::chrono::steady_clock::now()};
        void increment(::std::uint64_t& value) noexcept
        { if(value!=::std::numeric_limits<::std::uint64_t>::max()) { ++value; } else { statistics_.saturated=true; } }
        bool reject(audit_record_status status,::std::uint64_t& reason) noexcept
        { last_status_=status;increment(reason);increment(suppressed);return false; }
    public:
        result configure(capability_spec const& cap)
        {
            auto value = [&](::std::string_view key) -> ::std::string_view { auto it{cap.limits.find(key)}; return it == cap.limits.end() ? ::std::string_view{} : it->second; };
            ::std::uint64_t burst_integer{},candidate_records{},candidate_bytes{};double candidate_rate{};
            if(!parse_positive_double(value("audit-max-rate-hz"), candidate_rate) || !parse_u64(value("audit-burst"), burst_integer) || !burst_integer ||
               !parse_u64(value("audit-max-records"), candidate_records) || !candidate_records || !parse_u64(value("audit-max-bytes"), candidate_bytes) || !candidate_bytes)
            { return {errc::invalid_argument, "all audit quotas must be explicit and positive"}; }
            if(candidate_rate > 1000000. || burst_integer > 1048576 || candidate_records > 1048576 || candidate_bytes > 67108864) { return {errc::invalid_argument, "audit quotas exceed public safety ceilings"}; }
            rate=candidate_rate;maximum_records=candidate_records;maximum_bytes=candidate_bytes;
            records.clear(); bytes = suppressed = 0; updated = ::std::chrono::steady_clock::now();
            burst = static_cast<double>(burst_integer); tokens = burst;statistics_={};configured_=true;last_status_=audit_record_status::retained;
            return {};
        }
        bool record(::std::string entry) noexcept
        {
            if(statistics_.attempted==::std::numeric_limits<::std::uint64_t>::max())
            { return reject(audit_record_status::sequence_exhausted,statistics_.sequence_exhausted); }
            increment(statistics_.attempted);
            if(!configured_) { return reject(audit_record_status::unconfigured,statistics_.unconfigured); }
            if(entry.size()>maximum_bytes) { return reject(audit_record_status::oversized,statistics_.oversized); }
            auto now{::std::chrono::steady_clock::now()};
            auto elapsed{::std::clamp(::std::chrono::duration<double>(now - updated).count(), 0., 86400.)};
            tokens = ::std::min(burst, tokens + elapsed * rate); updated = now;
            if(tokens < 1.) { return reject(audit_record_status::rate_limited,statistics_.rate_limited); }
            auto entry_bytes{entry.size()};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
#endif
            { records.push_back(::std::move(entry)); }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            catch(...) { return reject(audit_record_status::allocation_failed,statistics_.allocation_failed); }
#endif
            tokens -= 1.;
            bytes+=entry_bytes;
            while(records.size()>maximum_records || bytes>maximum_bytes)
            { bytes -= records.front().size(); records.pop_front();increment(statistics_.overwritten);increment(suppressed); }
            increment(statistics_.accepted);last_status_=audit_record_status::retained;
            return true;
        }
        void drain(::std::vector<::std::string>& out, ::std::uint64_t& dropped)
        {
            // A failed output allocation must preserve every retained record
            // and the caller's previous output for a subsequent trusted retry.
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
#endif
            { ::std::vector<::std::string> candidate{records.begin(),records.end()};out.swap(candidate); }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            catch(...) { increment(statistics_.drain_failed);throw; }
#endif
            records.clear(); bytes = 0; dropped = suppressed;
        }
        ::std::uint64_t retained_bytes() const noexcept { return bytes; }
        ::std::size_t retained_records() const noexcept { return records.size(); }
        ::std::uint64_t dropped_records() const noexcept { return suppressed; }
        audit_record_status last_status() const noexcept { return last_status_; }
        audit_statistics statistics() const noexcept { return statistics_; }
    };
}
