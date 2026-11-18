/** @file
 * @brief Bounded, optional host-only result audit for registered ROSv2 calls.
 * @details The operator installs scope ceilings before execution. All matching
 * rules charge the same record, including its newline, or none do. A completed
 * call keeps its real return value even when an audit budget, sink, or lock
 * refuses a record. No argument, payload, native identity or handle is copied.
 */
#pragma once
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <atomic>
#include <charconv>
#include <cstdint>
#include <limits>
#include <mutex>
#include <string_view>

namespace uwvm2::imported::ros::scene::call_audit
{
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

        /** @brief Admit one completed call and deliver a bounded ASCII record.
         * @details Caller and group are copied as hexadecimal bytes. A caller
         * longer than 255 bytes has an explicit truncation flag. Runtime code
         * supplies a registered operation label, the raw ABI result, a trusted
         * generation and monotonic timestamp. Sink failures keep the charge.
         */
        template<typename Sink>
        void observe(std::string_view group, std::string_view module, std::uint64_t generation,
                     std::string_view fn, std::uint32_t result, std::uint64_t now, Sink&& sink) noexcept
        {
            std::unique_lock lock{mutex, std::try_to_lock};
            if(!lock.owns_lock()) { increment(contention); return; }
            if(!now) { increment(clock_failures); return; }
            bool selected{};
            for(auto& r : rules)
            {
                if(!applies(r, group, module)) continue;
                selected = true;
                if(r.denied_only && result != 13) return;
            }
            if(!selected) return;
            if(sequence == UINT64_MAX || fn.empty() || fn.size() > 255 || group.size() > 255)
            { increment(limited); return; }
            for(auto ch : fn)
                if(!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                     (ch >= '0' && ch <= '9') || ch == '.' || ch == '_' || ch == '-'))
                { increment(limited); return; }
            // 1020 hex digits, 255 operation bytes and bounded integer fields.
            std::array<char,2048> line;
            char* pos = line.data();
            auto text = [&](std::string_view v) noexcept { for(char c : v) *pos++ = c; };
            auto number = [&](std::uint64_t n) noexcept { pos = std::to_chars(pos, line.data()+line.size(), n).ptr; };
            auto hex = [&](std::string_view v) noexcept {
                constexpr char digits[] = "0123456789abcdef";
                for(unsigned char c : v) { *pos++ = digits[c >> 4]; *pos++ = digits[c & 15]; }
            };
            text("{\"ros\":\"uwvm_ros_v2\",\"phase\":\"return\",\"sequence\":"); number(sequence+1);
            text(",\"generation\":"); number(generation); text(",\"steady_ns\":"); number(now);
            text(",\"caller_hex\":\""); hex(module.substr(0,255));
            text("\",\"caller_truncated\":"); text(module.size()>255 ? "true" : "false");
            text(",\"group_hex\":\""); hex(group); text("\",\"function\":\""); text(fn);
            text("\",\"result\":"); number(result);
            text(",\"limited\":"); number(limited.load(std::memory_order_relaxed));
            text(",\"busy\":"); number(contention.load(std::memory_order_relaxed));
            text(",\"clock_failed\":"); number(clock_failures.load(std::memory_order_relaxed));
            text(",\"sink_failed\":"); number(sink_failures.load(std::memory_order_relaxed)); text("}");
            std::string_view record{line.data(),static_cast<std::size_t>(pos-line.data())};
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
