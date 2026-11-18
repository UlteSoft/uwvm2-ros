/** @file
 * @brief Bounded, optional host-only result audit for ROSv2 returns and import visibility refusals.
 * @details The operator installs scope ceilings before execution. All matching
 * rules charge the same record, including its newline, or none do. A completed
 * call keeps its real return value even when an audit budget, sink, or lock
 * refuses a record. No argument, payload, native identity or handle is copied.
 */
#pragma once
#include "call_audit_statistics.h"
#include <uwvm2/utils/container/standard.h>
#include "service_observation_audit.h"
#include <array>
#include <atomic>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <limits>
#include <mutex>
#include <string_view>

namespace uwvm2::imported::ros::scene::call_audit
{
    enum class phase { returned, hidden_import };
    enum class scope { global, group, single };
    struct ceiling
    {
        std::uint64_t records{}, bytes{}, rate_hz{}, burst{};
    };
    struct rule
    {
        scope target{};
        ::uwvm2::utils::container::std_string label;
        bool denied_only{};
        ceiling limits;
        std::uint64_t records{}, bytes{}, credit{}, updated{};
    };
    inline constexpr std::uint64_t second = 1000000000;
    inline void increment(std::atomic<std::uint64_t>& value) noexcept
    {
        auto n = value.load(std::memory_order_relaxed);
        while(n != UINT64_MAX && !value.compare_exchange_weak(n, n + 1, std::memory_order_relaxed)) {}
    }
    class journal
    {
        mutable std::mutex mutex;
        ::uwvm2::utils::container::std_vector<rule> rules;
        std::atomic<bool> enabled{};
        std::uint64_t sequence{};

        static bool applies(rule const& r, std::string_view group, std::string_view module) noexcept
        {
            return r.target == scope::global || (r.target == scope::group ? group : module) == r.label;
        }
        static void refill(rule& r, std::uint64_t now) noexcept
        {
            if(now <= r.updated) return;
            auto room = r.limits.burst * second - r.credit;
            auto elapsed = now - r.updated;
            if(r.limits.rate_hz)
                r.credit += elapsed > room / r.limits.rate_hz ? room : elapsed * r.limits.rate_hz;
            r.updated = now;
        }
    public:
        std::atomic<std::uint64_t> contention{}, limited{}, sink_failures{}, clock_failures{};
        bool active() const noexcept { return enabled.load(std::memory_order_acquire); }

        /** @brief Copy current counters without recording, refilling or draining.
         * @details This operation never waits for a producer or sink. Failures
         * occurring concurrently outside the journal lock appear on this or
         * a later read. The caller must report a contended read as unavailable.
         */
        bool inspect(statistics& output) const noexcept
        {
            std::unique_lock lock{mutex, std::try_to_lock};
            if(!lock.owns_lock()) return false;
            output = {active(), sequence, limited.load(std::memory_order_relaxed),
                      contention.load(std::memory_order_relaxed),
                      clock_failures.load(std::memory_order_relaxed),
                      sink_failures.load(std::memory_order_relaxed)};
            return true;
        }
        /** @brief Visit configured ceilings. Counters and rate-window state stay private. */
        template<class Visitor> bool visit_policy(Visitor visit)const
        {
            std::lock_guard lock{mutex};
            for(auto const& r:rules)if(!visit(r.target,r.label,r.denied_only,r.limits))return false;
            return true;
        }
        /** @brief Register one finite allowance for an exact trusted scope.
         * @details Repeated scope/label pairs are rejected. A zero ceiling
         * disables that part of the selection. One billion nanocredits is one
         * event. Refills saturate before multiplication can overflow.
         */
        bool add(scope target, std::string_view label, bool denied_only, ceiling limits)
        {
            if(limits.rate_hz > second || limits.burst > second) return false;
            std::lock_guard lock{mutex};
            for(auto const& r : rules) if(r.target == target && r.label == label) return false;
            rules.push_back({target, ::uwvm2::utils::container::std_string(label), denied_only,
                             limits, 0, 0, limits.burst * second, 0});
            enabled.store(true, std::memory_order_release);
            return true;
        }

        /** @brief Admit one call result or hidden import and deliver a bounded ASCII record.
         * @details Caller and group are copied as hexadecimal bytes. A caller
         * longer than 255 bytes has an explicit truncation flag. Runtime code
         * supplies a registered operation label, the raw ABI result, a trusted
         * generation and monotonic timestamp. Hidden imports precede runtime activation.
         * They record a null result and generation zero. Unregistered names with
         * unsafe bytes use a bounded hexadecimal prefix and an exact byte count.
         * Sink failures keep the charge.
         */
        template<typename Sink>
        void observe(std::string_view group, std::string_view module, std::uint64_t generation,
                     std::string_view fn, std::uint32_t result, std::uint64_t now, Sink&& sink, phase point = phase::returned) noexcept
        {
            std::unique_lock lock{mutex, std::try_to_lock};
            if(!lock.owns_lock()) { increment(contention); return; }
            if(!now) { increment(clock_failures); return; }
            bool selected{};
            for(auto& r : rules)
            {
                if(!applies(r, group, module)) continue;
                selected = true;
                // Include failed authority, invalid handles and stale generations.
                if(r.denied_only && point == phase::returned && result != 13 && result != 9 && result != 116) return;
            }
            if(!selected) return;
            bool named = !fn.empty() && fn.size() <= 255;
            if(named) for(auto ch : fn)
            {
                if(!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                     (ch >= '0' && ch <= '9') || ch == '.' || ch == '_' || ch == '-'))
                { named = false; break; }
            }
            if(sequence == UINT64_MAX || group.size() > 255 || (!named && point == phase::returned))
            { increment(limited); return; }
            // 1020 hex digits, 255 operation bytes and bounded integer fields.
            std::array<char,2816> line;
            std::size_t used{};
            bool fits=true;
            auto text = [&](std::string_view v) noexcept {
                if(v.size()>line.size()-used) { fits=false; return; }
                std::memcpy(line.data()+used,v.data(),v.size());
                used+=v.size();
            };
            auto number = [&](std::uint64_t n) noexcept {
                std::array<char,20> digits;
                auto value=std::to_chars(digits.data(),digits.data()+digits.size(),n);
                if(value.ec!=std::errc{}) {fits=false;return;}
                text({digits.data(),static_cast<std::size_t>(value.ptr-digits.data())});
            };
            auto hex = [&](std::string_view v) noexcept {
                constexpr char digits[] = "0123456789abcdef";
                if(v.size()>(line.size()-used)/2) {fits=false;return;}
                for(unsigned char c : v) { line[used++]=digits[c>>4]; line[used++]=digits[c&15]; }
            };
            text("{\"ros\":\"uwvm_ros_v2\",\"phase\":");
            text(point == phase::returned ? "\"return\"" : "\"link\"");
            text(",\"sequence\":"); number(sequence+1);
            auto pair=point==phase::returned && fn=="service.available"?service_observation_audit::current:nullptr;
            if(pair&&pair->observer_length&&pair->source_length)
            {
                text(",\"observer_hex\":\"");hex({pair->observer.data(),pair->observer_length});
                text("\",\"source_hex\":\"");hex({pair->source.data(),pair->source_length});
                using loss=service_observation_audit::withdrawal;
                text("\",\"authority_lost\":\"");
                text(pair->lost==loss::observer?"observer":pair->lost==loss::source?"source":pair->lost==loss::both?"both":"none");text("\"");
            }
            text(",\"generation\":"); number(generation); text(",\"steady_ns\":"); number(now);
            text(",\"caller_hex\":\""); hex(module.substr(0,255));
            text("\",\"caller_truncated\":"); text(module.size()>255 ? "true" : "false");
            text(",\"group_hex\":\""); hex(group); text("\",\"function\":");
            if(named) { text("\""); text(fn); text("\""); }
            else
            {
                text("null,\"function_hex\":\""); hex(fn.substr(0, 128));
                text("\",\"function_bytes\":"); number(fn.size());
                text(",\"function_truncated\":"); text(fn.size() > 128 ? "true" : "false");
            }
            if(point == phase::hidden_import) text(",\"reason\":\"hidden\",\"result\":null");
            else {text(",\"result\":"); number(result);}
            text(",\"limited\":"); number(limited.load(std::memory_order_relaxed));
            text(",\"busy\":"); number(contention.load(std::memory_order_relaxed));
            text(",\"clock_failed\":"); number(clock_failures.load(std::memory_order_relaxed));
            text(",\"sink_failed\":"); number(sink_failures.load(std::memory_order_relaxed)); text("}");
            if(!fits) { increment(limited); return; }
            std::string_view record{line.data(),used};
            auto bytes = static_cast<std::uint64_t>(record.size()+1);
            for(auto& r : rules)
            {
                if(!applies(r,group,module)) continue;
                refill(r,now);
                if(r.records >= r.limits.records || bytes > r.limits.bytes-r.bytes ||
                   !r.limits.rate_hz || r.credit < second)
                { increment(limited); return; }
            }
            for(auto& r : rules)
            {
                if(!applies(r,group,module)) continue;
                ++r.records; r.bytes += bytes; r.credit -= second;
            }
            ++sequence;
            // A slow or failed sink must not hold either authority or audit locks.
            lock.unlock();
            if(!sink(record)) increment(sink_failures);
        }
    };
}
