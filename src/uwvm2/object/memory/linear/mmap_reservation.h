// Virtual address space reserved by a linear memory, including its inaccessible tail.
#pragma once
#include <algorithm>
#include <atomic>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>

namespace uwvm2::object::memory::linear::mmap_reservation
{
    using bytes_type = ::std::uint_least64_t;
    inline constexpr unsigned host_bits{::std::numeric_limits<::std::uintptr_t>::digits};
    inline constexpr bytes_type maximum_bytes{UINT64_MAX};
    inline constexpr bytes_type full_wasm32_window{bytes_type{1} << 33u};
    inline constexpr bytes_type max_access_size{64u};
    inline constexpr bytes_type tagged_wasm32_data_limit{bytes_type{1} << 28u};
    inline constexpr bytes_type tagged_wasm64_span_limit{bytes_type{1} << 48u};
    [[nodiscard]] inline constexpr bytes_type hard_limit(unsigned pointer_bits) noexcept
    { return pointer_bits >= 64u ? maximum_bytes : bytes_type{1} << pointer_bits; }
    [[nodiscard]] inline constexpr bytes_type default_pool(unsigned pointer_bits) noexcept
    { return pointer_bits >= 64u ? bytes_type{16} << 40u : pointer_bits >= 32u ? bytes_type{2} << 30u : hard_limit(pointer_bits); }

    // A pool can be the entire 2^64-byte address space. A single mapping and
    // all native lengths must still fit size_t. No arithmetic wraps to zero.
    struct byte_budget
    {
        bytes_type bytes{};
        bool full_address64{};

        [[nodiscard]] constexpr bool take(bytes_type value, bytes_type count = 1u) noexcept
        {
            if(!value || !count) { return true; }
            auto available{bytes};
            if(full_address64) { available = maximum_bytes - (value - 1u); --count; }
            if(count && value > available / count) { return false; }
            bytes = available - value * count;
            full_address64 = false;
            return true;
        }
    };

    struct layout
    {
        bytes_type data{};     // Prefix eligible for memory.grow, in complete Wasm pages.
        bytes_type reserved{}; // Complete OS-page-rounded mapping starting at the Wasm base.
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return reserved != 0u; }
    };

    [[nodiscard]] inline constexpr bytes_type round_up(bytes_type value, bytes_type alignment) noexcept
    {
        if(!::std::has_single_bit(alignment) || value > maximum_bytes - (alignment - 1u)) { return 0u; }
        return (value + (alignment - 1u)) & ~(alignment - 1u);
    }

    [[nodiscard]] inline constexpr bytes_type outer_guard(bytes_type wasm_page, bytes_type os_page) noexcept
    {
        if(!::std::has_single_bit(wasm_page) || wasm_page > maximum_bytes - max_access_size) { return 0u; }
        return round_up(wasm_page + max_access_size, os_page);
    }

    struct owner_range
    {
        bytes_type minimum_span{};
        bytes_type maximum_span{};
        bytes_type maximum_data{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return minimum_span != 0u; }
        [[nodiscard]] constexpr bytes_type span(bytes_type share) const noexcept
        { return (::std::clamp)(share, minimum_span, maximum_span); }
    };

    // Compare page counts before multiplying. A valid memory64 declaration
    // may describe 2^64 bytes, which is one more than a u64 byte count.
    [[nodiscard]] inline constexpr owner_range bounds(unsigned pointer_bits, bool address64, bytes_type minimum_pages,
        bytes_type maximum_pages, bytes_type wasm_page, bytes_type os_page, bytes_type const* manual_data = nullptr, bool tagged = false) noexcept
    {
        auto const tail{outer_guard(wasm_page, os_page)};
        if(!tail) { return {}; }
        auto const native{pointer_bits >= 64u ? maximum_bytes : hard_limit(pointer_bits) - 1u};
        auto const largest_span{(tagged && address64 ? (::std::min)(native, tagged_wasm64_span_limit) : native) & ~(os_page - 1u)};
        if(tail > largest_span || (pointer_bits >= 64u && full_wasm32_window > largest_span - tail)) { return {}; }
        bool const fixed_wasm32{pointer_bits >= 64u && !address64};
        auto domain{fixed_wasm32 ? full_wasm32_window / 2u : largest_span - tail};
        if(tagged && !address64) { domain = (::std::min)(domain, tagged_wasm32_data_limit); }
        auto const domain_pages{domain / wasm_page};
        auto const largest_pages{(::std::min)(maximum_pages, domain_pages)};
        if(minimum_pages > largest_pages) { return {}; }
        auto const minimum_data{minimum_pages * wasm_page};
        auto maximum_data{largest_pages * wasm_page};
        if(manual_data)
        {
            auto const value{*manual_data};
            if(fixed_wasm32 || !value || (address64 && !::std::has_single_bit(value)) || value < minimum_data) { return {}; }
            // A declared maximum clips a valid fixed request. If the resulting
            // data plus its guard cannot fit a native mapping, refuse it.
            if(maximum_pages <= maximum_bytes / wasm_page) { maximum_data = (::std::min)(value, maximum_pages * wasm_page); }
            else { maximum_data = value; }
            if(maximum_data > domain) { return {}; }
        }
        auto const floor{pointer_bits >= 64u ? full_wasm32_window + tail : tail};
        if(fixed_wasm32) { return {floor, floor, maximum_data}; }
        auto span_for = [=](bytes_type data) noexcept
        {
            // data <= largest_span - tail; tail >= wasm_page + max_access_size.
            return (::std::max)(floor, round_up(data + wasm_page + max_access_size, os_page));
        };
        auto const small{span_for(manual_data ? maximum_data : minimum_data)};
        auto const large{!manual_data && maximum_pages >= domain_pages ? largest_span : span_for(maximum_data)};
        return {small, large, maximum_data};
    }

    [[nodiscard]] inline constexpr layout make_layout(unsigned pointer_bits, bool address64, bytes_type cap,
                                                     bytes_type maximum_pages, bytes_type wasm_page, bytes_type os_page) noexcept
    {
        auto const range{bounds(pointer_bits, address64, 0u, maximum_pages, wasm_page, os_page)};
        if(!range || cap > hard_limit(pointer_bits) || cap < range.minimum_span) { return {}; }
        auto const reserved{range.span(cap & ~(os_page - 1u))};
        auto const tail{outer_guard(wasm_page, os_page)};
        return {(::std::min)(range.maximum_data, (reserved - tail) / wasm_page * wasm_page), reserved};
    }

    // Find the largest common power-of-two share that satisfies every owner's
    // minimum and reuses budget released by finite maxima and exact overrides.
    // Fixed-size arithmetic avoids an auxiliary allocation proportional to the graph.
    struct census
    {
        struct candidate
        {
            byte_budget remaining{};
            bool fits{true};
        };
        bytes_type participants{};
        candidate shares[65]{}; // Exponents 0..63 and the complete 64-bit domain.
        explicit constexpr census(byte_budget pool) noexcept
        { for(auto& s: shares) { s.remaining = pool; } }

        constexpr void add(owner_range const& range, bool participates, bytes_type count = 1u) noexcept
        {
            if(!count) { return; }
            if(participates) { participants += count; } // Actual native declaration census; future owners never increment it.
            for(unsigned i{}; i != 65u; ++i)
            {
                auto& s{shares[i]};
                if(s.fits)
                {
                    auto const share{i == 64u ? maximum_bytes : bytes_type{1} << i};
                    s.fits = s.remaining.take(range.span(share), count);
                }
            }
        }
        [[nodiscard]] constexpr bool fits_minimum() const noexcept { return shares[0].fits; }
        [[nodiscard]] constexpr bytes_type common_share() const noexcept
        {
            if(shares[64].fits) { return maximum_bytes; }
            for(unsigned i{64u}; i != 0u; --i)
            { if(shares[i - 1u].fits) { return bytes_type{1} << (i - 1u); } }
            return 0u;
        }
    };

    // A replaced generation, a private thread instance and a cold restore can
    // all coexist. Charge the owner until its actual unmap completes. This cold
    // lock also works on hosts without atomic 64-bit arithmetic instructions.
    // Launch configuration is sealed before concurrent owners can be created.
    inline bytes_type pool_bytes{default_pool(host_bits)};
    inline bool pool_full_address_space{};
    inline ::std::size_t fixed_participants{};
    inline ::std::atomic_flag accounting_lock{};
    inline byte_budget live_usage{};
    inline bytes_type live_participants{};
    struct accounting_guard
    {
        accounting_guard() noexcept { while(accounting_lock.test_and_set(::std::memory_order_acquire)) {} }
        ~accounting_guard() { accounting_lock.clear(::std::memory_order_release); }
    };
    struct pool_snapshot
    {
        byte_budget remaining{};
        bytes_type participants{};
        bool valid{};
    };
    [[nodiscard]] inline pool_snapshot pool_without_live_owners_unlocked() noexcept
    {
        byte_budget remaining{pool_bytes, pool_full_address_space};
        if((remaining.full_address64 && host_bits < 64u) || (!remaining.full_address64 && remaining.bytes > hard_limit(host_bits)))
        { return {}; }
        if(live_usage.full_address64) { return {{}, live_participants, remaining.full_address64}; }
        if(!remaining.take(live_usage.bytes)) { return {}; }
        return {remaining, live_participants, true};
    }
    [[nodiscard]] inline pool_snapshot available_pool() noexcept
    {
        accounting_guard guard{};
        return pool_without_live_owners_unlocked();
    }
    [[nodiscard]] inline bool acquire(bytes_type bytes, bool participates) noexcept
    {
        accounting_guard guard{};
        auto snapshot{pool_without_live_owners_unlocked()};
        if(!bytes || !snapshot.valid || !snapshot.remaining.take(bytes) ||
           (participates && fixed_participants && live_participants >= fixed_participants)) { return false; }
        if(bytes <= maximum_bytes - live_usage.bytes) { live_usage.bytes += bytes; }
        else { live_usage = {0u, true}; } // Addition fits the configured 2^64-byte pool exactly.
        if(participates) { ++live_participants; }
        return true;
    }
    inline void release(bytes_type bytes, bool participates) noexcept
    {
        accounting_guard guard{};
        if(!live_usage.take(bytes) || (participates && !live_participants)) { ::std::abort(); }
        if(participates) { --live_participants; }
    }
}
