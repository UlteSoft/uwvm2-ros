/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/utils/thread/monotonic_clock.h>
#endif

#include <algorithm>
#include <iterator>
#include <chrono>
#include <deque>
#include <vector>
#include <limits>
#include "message.h"
#include "synchronous_wait.h"
#include <cmath>

namespace uwvm2::imported::ros
{
    enum class audit_record_status : unsigned char { retained, unconfigured, rate_limited, oversized, allocation_failed, sequence_exhausted, capacity_exhausted, cancelled, clock_unavailable };
    struct audit_statistics
    {
        ::std::uint64_t attempted{}, accepted{}, overwritten{}, rate_limited{}, oversized{}, unconfigured{}, allocation_failed{}, sequence_exhausted{}, drain_failed{}, capacity_exhausted{}, cancelled{}, clock_unavailable{};
        bool saturated{};
    };
    // Optional per-capability audit quotas. Rejected records increment a
    // visible reason counter; already retained records are never evicted.
    // The trusted host drains records; guests have no handle to this sink.
    class bounded_audit
    {
        ::std::deque<::std::string> records{};
        ::std::uint64_t maximum_records{}, maximum_bytes{}, bytes{}, suppressed{};
        double rate{};
        ::std::uint64_t burst{},tokens{};
        long double fraction{};
        bool wait_rate{},finish_rate{};
        audit_statistics statistics_{};
        audit_record_status last_status_{audit_record_status::unconfigured};
        bool configured_{};
        bool limited_rate_{};
        ::std::chrono::steady_clock::time_point updated{::uwvm2::utils::host_clock::now()};
        void increment(::std::uint64_t& value) noexcept
        { if(value!=::std::numeric_limits<::std::uint64_t>::max()) { ++value; } else { statistics_.saturated=true; } }
        bool reject(audit_record_status status,::std::uint64_t& reason) noexcept
        { last_status_=status;increment(reason);increment(suppressed);return false; }
    public:
        result configure(capability_spec const& cap)
        {
            ::std::uint64_t burst_integer{quota_value(cap,"audit-burst")},
                candidate_records{quota_value(cap,"audit-max-records")},candidate_bytes{quota_value(cap,"audit-max-bytes")};
            double candidate_rate{};
            auto const rate_field{cap.limits.find("audit-max-rate-hz")};
            limited_rate_=rate_field!=cap.limits.end() && rate_field->second!="unlimited";
            if(limited_rate_ && ((rate_field->second!="0" && !parse_positive_double(rate_field->second,candidate_rate)) || burst_integer==unlimited_quota))
            { return {errc::invalid_argument,"audit rate and burst must both be explicitly finite"}; }
            rate=candidate_rate;maximum_records=candidate_records;maximum_bytes=candidate_bytes;
            wait_rate=quota_wait_enabled(cap,"audit-max-rate-hz");finish_rate=quota_wait_on_exit(cap,"audit-max-rate-hz")||quota_wait_on_exit(cap,"audit-burst");
            records.clear(); bytes = suppressed = 0; updated = ::uwvm2::utils::host_clock::now();
            if(!::uwvm2::utils::host_clock::available()) { configured_=false;return {errc::io_error,"monotonic clock unavailable"}; }
            burst=burst_integer;tokens=burst;fraction=0;statistics_={};configured_=true;last_status_=audit_record_status::retained;
            return {};
        }
        void record_allocation_failure() noexcept
        {
            increment(statistics_.attempted);
            (void)reject(audit_record_status::allocation_failed,statistics_.allocation_failed);
        }
        void record_extent_failure() noexcept
        {
            increment(statistics_.attempted);
            (void)reject(audit_record_status::oversized,statistics_.oversized);
        }
        bool record(::std::string entry) noexcept
        {
            if(statistics_.attempted==::std::numeric_limits<::std::uint64_t>::max())
            { return reject(audit_record_status::sequence_exhausted,statistics_.sequence_exhausted); }
            increment(statistics_.attempted);
            if(!configured_) { return reject(audit_record_status::unconfigured,statistics_.unconfigured); }
            if(entry.size()>maximum_bytes) { return reject(audit_record_status::oversized,statistics_.oversized); }
            if(records.size()>=maximum_records || bytes>maximum_bytes || entry.size()>maximum_bytes-bytes)
            { return reject(audit_record_status::capacity_exhausted,statistics_.capacity_exhausted); }
            struct restore_native_policy
            {
                bool previous{synchronous_wait_context::native_exit_policy()};
                ~restore_native_policy() { (void)synchronous_wait_context::select_native_exit(previous); }
            } native_policy;
            ::std::unique_ptr<synchronous_quota_wait> fallback;
            synchronous_quota_wait* waiter{};
            while(limited_rate_)
            {
                auto now{::uwvm2::utils::host_clock::now()};
                if(!::uwvm2::utils::host_clock::available())
                { return reject(audit_record_status::clock_unavailable,statistics_.clock_unavailable); }
                auto credit{::std::chrono::duration<long double>(now-updated).count()*rate+fraction};
                auto const room{burst-tokens};
                if(credit>=static_cast<long double>(room)) { tokens=burst;fraction=0; }
                else
                {
                    auto whole{static_cast<::std::uint64_t>((::std::max)(0.L,credit))};
                    tokens+=whole;fraction=credit-whole;
                }
                updated=now;
                if(tokens) { break; }
                if(!wait_rate || !rate || !burst)
                { return reject(audit_record_status::rate_limited,statistics_.rate_limited); }
                if(!waiter)
                {
                    if(synchronous_wait_context::present()) { waiter=synchronous_wait_context::waiter(); }
                    else { fallback.reset(new(::std::nothrow) synchronous_quota_wait{});waiter=fallback.get(); }
                    if(!waiter) { return reject(audit_record_status::allocation_failed,statistics_.allocation_failed); }
                }
                if(!waiter->select_exit(finish_rate))
                { return reject(audit_record_status::cancelled,statistics_.cancelled); }
                auto deadline{::std::chrono::steady_clock::time_point::max()};
                auto seconds{(1-fraction)/rate};
                if(seconds<::std::chrono::duration<long double>(deadline-now).count())
                { deadline=now+::std::chrono::nanoseconds(static_cast<::std::int64_t>((::std::max)(1.L,::std::ceil(seconds*1000000000.L)))); }
                auto const outcome{waiter->until(deadline)};
                if(outcome!=quota_wait_result::ready)
                { return reject(audit_record_status::cancelled,statistics_.cancelled); }
            }
            auto entry_bytes{entry.size()};
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            try
#endif
            { records.push_back(::std::move(entry)); }
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
            catch(...) { return reject(audit_record_status::allocation_failed,statistics_.allocation_failed); }
#endif
            if(limited_rate_) { --tokens; }
            bytes+=entry_bytes;
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
