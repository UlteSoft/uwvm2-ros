/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <uwvm2/extension/adm/types.h>
# include <uwvm2/extension/memtag/instruction.h>
# include "memory_immediate.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::validation::standard::wasm3
{
    // Core 3.0 binary/types#limits and valid/types#{memory,table}-types.
    // The threads extension adds bit 1 only for memories, with a mandatory max.
    // Resource/host-size limits are instantiation concerns; validation preserves
    // all 64 bits, including the legal memory64 endpoint of 2^48 pages.
    enum class storage_address_type : unsigned { i32, i64 };
    enum class address_limits_kind : unsigned { memory, table };
    enum class address_limits_error : unsigned
    {
        ok, missing_flags, invalid_flags, address64_disabled, threads_disabled,
        shared_without_maximum, minimum_encoding, maximum_encoding,
        maximum_below_minimum, limit_out_of_range, adm_disabled, memtag_disabled, invalid_adm_bits, custom_pages_disabled, invalid_page_size
    };
    struct address_limits
    {
        ::std::uint_least64_t min{};
        ::std::uint_least64_t max{0xffff'ffff'ffff'ffffull};
        bool present_max{};
        storage_address_type address_type{};
        bool shared{};
        ::std::uint_least8_t adm_bits{};
        bool memtag{};
        ::std::uint_least8_t page_size_log2{16u};
    };
    struct address_limits_result
    {
        address_limits limits{};
        address_limits_error error{};
        // Byte offset for a diagnostic; no borrowed pointer escapes on failure.
        ::std::size_t error_offset{};
    };

    [[nodiscard]] inline constexpr ::std::uint_least64_t address_limit_maximum(
        address_limits_kind kind, storage_address_type address_type, unsigned adm_bits = 0u, bool memtag = false, unsigned page_shift = 16u) noexcept
    {
        if(kind == address_limits_kind::memory)
        { return ::uwvm2::extension::adm::maximum_pages(address_type == storage_address_type::i64, adm_bits, memtag, page_shift); }
        return address_type == storage_address_type::i64 ? 0xffff'ffff'ffff'ffffull : 0xffff'ffffull;
    }

    [[nodiscard]] inline constexpr address_limits_result scan_address_limits(
        ::std::byte const*& cursor, ::std::byte const* end, address_limits_kind kind,
        bool address64_enabled, bool threads_enabled, bool wide_encoding = true, bool adm_enabled = false, bool memtag_enabled = false, bool custom_pages_enabled = false) noexcept
    {
        address_limits_result result{};
        // limits ... (end)
        // unsafe (could be end)
        // ^^ next borrows cursor's bounded range; cursor commits only on success.
        auto next{cursor};
        if(next == end) { result.error = address_limits_error::missing_flags; return result; }
        auto const flags{::std::to_integer<unsigned>(*next)};
        constexpr auto adm_flag{::uwvm2::extension::adm::memory_type_flag};
        constexpr auto memtag_flag{::uwvm2::extension::memtag::memory_type_flag};
        auto const allowed_flags{kind == address_limits_kind::memory ? (15u | adm_flag | memtag_flag) : 5u};
        if((flags & ~allowed_flags) != 0u)
        { result.error = address_limits_error::invalid_flags; return result; }
        auto& limits{result.limits};
        limits.present_max = (flags & 1u) != 0u;
        limits.address_type = (flags & 4u) ? storage_address_type::i64 : storage_address_type::i32;
        limits.shared = (flags & 2u) != 0u;
        limits.memtag = (flags & memtag_flag) != 0u;
        if((flags & 8u) && !custom_pages_enabled)
        { result.error = address_limits_error::custom_pages_disabled; return result; }
        if((flags & adm_flag) && !adm_enabled)
        { result.error = address_limits_error::adm_disabled; return result; }
        if(limits.memtag && !memtag_enabled)
        { result.error = address_limits_error::memtag_disabled; return result; }
        if(limits.shared && !limits.present_max)
        { result.error = address_limits_error::shared_without_maximum; return result; }
        if(limits.address_type == storage_address_type::i64 && !address64_enabled)
        { result.error = address_limits_error::address64_disabled; return result; }
        if(limits.shared && !threads_enabled)
        { result.error = address_limits_error::threads_disabled; return result; }
        ++next;
        if(flags & adm_flag)
        {
            result.error_offset = 1u;
            if(next == end) { result.error = address_limits_error::invalid_adm_bits; return result; }
            limits.adm_bits = ::std::to_integer<::std::uint_least8_t>(*next++);
        }
        if(!::uwvm2::extension::adm::valid(limits.address_type == storage_address_type::i64, limits.adm_bits, limits.memtag))
        { result.error = address_limits_error::invalid_adm_bits; return result; }
        // [flags k?] min max? ... (end). Both header bytes were checked before advancing.
        auto const scan{[&](::std::uint_least64_t& value) constexpr noexcept
        {
            // [consumed] limit ... (end)
            // [safe    ] unsafe (could be end); bounded scanner advances next only
            // on a complete integer, and never dereferences end.
            if(wide_encoding || limits.address_type == storage_address_type::i64)
            { return scan_memory_unsigned(next, end, value); }
            ::std::uint_least32_t narrow{};
            if(!scan_memory_unsigned(next, end, narrow)) { return false; }
            // [consumed limit] ... (end)
            // [safe          ] unsafe (could be end)
            //                  ^^ next after the bounded u32 scan.
            value = narrow;
            return true;
        }};
        result.error_offset = static_cast<::std::size_t>(next - cursor);
        if(!scan(limits.min)) { result.error = address_limits_error::minimum_encoding; return result; }
        // [flags min] max? ... (end)
        // [safe     ] unsafe (could be end)
        //             ^^ next; minimum was consumed without truncation.
        if(limits.present_max)
        {
            result.error_offset = static_cast<::std::size_t>(next - cursor);
            if(!scan(limits.max)) { result.error = address_limits_error::maximum_encoding; return result; }
            // [flags min max] ... (end)
            // [safe         ] unsafe (could be end)
            //                 ^^ next; maximum was consumed without truncation.
            if(limits.max < limits.min)
            { result.error = address_limits_error::maximum_below_minimum; return result; }
        }
        // Custom page sizes append a u32 LEB exponent to the complete limits.
        if(flags & 8u)
        {
            ::std::uint_least32_t exponent{};
            result.error_offset = static_cast<::std::size_t>(next - cursor);
            if(!scan_memory_unsigned(next, end, exponent) || (exponent != 0u && exponent != 16u))
            { result.error = address_limits_error::invalid_page_size; return result; }
            limits.page_size_log2 = static_cast<::std::uint_least8_t>(exponent);
        }
        auto const maximum{address_limit_maximum(kind, limits.address_type, limits.adm_bits, limits.memtag, limits.page_size_log2)};
        if(limits.min > maximum || (limits.present_max && limits.max > maximum))
        { result.error = address_limits_error::limit_out_of_range; return result; }
        // [validated flags/min/max bytes] next ... end
        // [safe                         ] unsafe (possibly one-past)
        // ^^ cursor: still at the original limits start; next was bounded by scan.
        cursor = next;
        // [flags min max?] next ... (end)
        // [safe          ] unsafe (could be end)
        //                  ^^ cursor: commit the complete validated encoding.
        result.error_offset = 0uz;
        return result;
    }

    // Core 3 memory/table import matching. These are declared limits, without
    // implementation resource caps. Absent actual max cannot satisfy a required max.
    [[nodiscard]] inline constexpr bool address_limits_match(address_limits const& expected,
                                                              address_limits const& actual) noexcept
    {
        return expected.address_type == actual.address_type && expected.shared == actual.shared &&
            expected.adm_bits == actual.adm_bits && expected.memtag == actual.memtag && expected.page_size_log2 == actual.page_size_log2 &&
            actual.min >= expected.min && (!expected.present_max || (actual.present_max && actual.max <= expected.max));
    }
    // memory.copy/table.copy use destination and source address types separately;
    // their length is i64 only when BOTH storage objects have i64 addresses.
    [[nodiscard]] inline constexpr storage_address_type copy_length_address_type(
        storage_address_type destination, storage_address_type source) noexcept
    {
        return destination == storage_address_type::i64 && source == storage_address_type::i64 ?
            storage_address_type::i64 : storage_address_type::i32;
    }
}
