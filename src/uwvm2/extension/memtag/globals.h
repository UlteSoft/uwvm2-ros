/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.   *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <algorithm>
# include <cstddef>
# include <cstdint>
# include <span>
# include <uwvm2/utils/macro/push_macros.h>
# include "instruction.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    enum class globals_error : unsigned
    {
        none, truncated, integer_overflow, unsupported_version,
        impossible_count, memory_out_of_bounds, trailing_bytes, invalid_memory_index
    };
    struct globals_result
    {
        globals_error error{};
        ::std::size_t error_offset{};
        ::std::size_t descriptors_offset{};
        ::std::uint64_t descriptor_count{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return error == globals_error::none; }
    };

    namespace details
    {
        // A bounded ULEB64: never reads beyond the owned immutable custom
        // section and never shifts a malformed terminal payload by >=64.
        [[nodiscard]] inline constexpr globals_error read_global_uleb(
            ::std::span<::std::byte const> bytes, ::std::size_t& cursor, ::std::uint64_t& result) noexcept
        {
            result = 0u;
            for(unsigned index{}; index != 10u; ++index)
            {
                if(cursor == bytes.size()) { return globals_error::truncated; }
                auto const byte{::std::to_integer<unsigned>(bytes[cursor++])};
                if(index == 9u && byte > 1u) { return globals_error::integer_overflow; }
                result |= static_cast<::std::uint64_t>(byte & 0x7fu) << (index * 7u);
                if((byte & 0x80u) == 0u) { return globals_error::none; }
            }
            return globals_error::integer_overflow;
        }

        [[nodiscard]] inline constexpr globals_error read_global_memory_index(
            ::std::span<::std::byte const> bytes, ::std::size_t& cursor, ::std::uint32_t& result) noexcept
        {
            result = 0u;
            for(unsigned index{}; index != 5u; ++index)
            {
                if(cursor == bytes.size()) { return globals_error::truncated; }
                auto const byte{::std::to_integer<unsigned>(bytes[cursor++])};
                if(index == 4u && byte > 15u) { return globals_error::integer_overflow; }
                result |= static_cast<::std::uint32_t>(byte & 0x7fu) << (index * 7u);
                if((byte & 0x80u) == 0u) { return globals_error::none; }
            }
            return globals_error::integer_overflow;
        }
    }

    // The lookup is read-only: it maps a module-local memory index to the
    // canonical owner's current byte length and returns false for an invalid
    // index or a provider without a Wasm-owned allocation. This format never
    // treats a segment index or a GC type index as a linear-memory index.
    template<typename MemoryBytes>
    [[nodiscard]] inline constexpr globals_result validate_linear_globals(
        ::std::span<::std::byte const> bytes, MemoryBytes&& memory_bytes) noexcept
    {
        globals_result result{};
        ::std::size_t cursor{};
        ::std::uint64_t version{};
        auto read{[&](::std::uint64_t& value) constexpr noexcept
        {
            result.error_offset = cursor;
            result.error = details::read_global_uleb(bytes, cursor, value);
            return result.error == globals_error::none;
        }};
        if(!read(version)) { return result; }
        if(version != linear_globals_section_version)
        { result.error = globals_error::unsupported_version; return result; }
        if(!read(result.descriptor_count)) { return result; }
        result.descriptors_offset = cursor;
        if(result.descriptor_count > (bytes.size() - cursor) / 3u)
        { result.error = globals_error::impossible_count; return result; }
        for(::std::uint64_t index{}; index != result.descriptor_count; ++index)
        {
            ::std::uint32_t memory_index{};
            ::std::uint64_t address{}, length{}, owner_bytes{};
            auto const start{cursor};
            result.error_offset = cursor;
            result.error = details::read_global_memory_index(bytes, cursor, memory_index);
            if(result.error != globals_error::none || !read(address) || !read(length)) { return result; }
            if(!memory_bytes(memory_index, owner_bytes))
            { result.error = globals_error::invalid_memory_index; result.error_offset = start; return result; }
            if(address > owner_bytes || length > owner_bytes - address)
            { result.error = globals_error::memory_out_of_bounds; result.error_offset = start; return result; }
        }
        if(cursor != bytes.size())
        { result.error = globals_error::trailing_bytes; result.error_offset = cursor; }
        return result;
    }

    template<typename MemoryBytes, typename Visitor>
    inline constexpr globals_result visit_linear_globals(::std::span<::std::byte const> bytes,
                                                          MemoryBytes&& memory_bytes, Visitor&& visitor)
    {
        auto const validated{validate_linear_globals(bytes, memory_bytes)};
        if(!validated) { return validated; }
        auto cursor{validated.descriptors_offset};
        for(::std::uint64_t index{}; index != validated.descriptor_count; ++index)
        {
            // All reads have been proved before the first visitor invocation.
            ::std::uint32_t memory_index{};
            ::std::uint64_t address{}, length{};
            (void)details::read_global_memory_index(bytes, cursor, memory_index);
            (void)details::read_global_uleb(bytes, cursor, address);
            (void)details::read_global_uleb(bytes, cursor, length);
            visitor(linear_global_descriptor{memory_index, address, length});
        }
        return validated;
    }

    // Admission scratch only: these identities name canonical native owners,
    // not module memory indices. Empty descriptors are omitted. Both ends are
    // inclusive granule indices, formed only after the full byte-range proof.
    struct global_tag_range
    {
        ::std::uintptr_t owner{};
        ::std::uint64_t first_granule{}, last_granule{};
    };

    // Independently assigning tags to one shared granule is forbidden,
    // including byte-disjoint ranges, imported aliases and cross-module plans.
    // Sorting scratch metadata avoids a quadratic rejection path.
    [[nodiscard]] inline constexpr bool validate_global_tag_ranges(::std::span<global_tag_range> ranges) noexcept
    {
        if(ranges.empty()) { return true; }
        ::std::sort(ranges.begin(), ranges.end(), [](auto const& left, auto const& right) constexpr noexcept
        {
            if(left.owner != right.owner) { return left.owner < right.owner; }
            return left.first_granule < right.first_granule;
        });
        ::std::uintptr_t owner{};
        ::std::uint64_t last{};
        bool have_owner{};
        for(auto const& range:ranges)
        {
            if(range.first_granule > range.last_granule) { return false; }
            if(have_owner && range.owner == owner && range.first_granule <= last) { return false; }
            owner=range.owner;last=range.last_granule;have_owner=true;
        }
        return true;
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
