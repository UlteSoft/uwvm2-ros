/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.   *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
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
        impossible_count, memory_out_of_bounds, trailing_bytes
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
    }

    // LLD's memtag.globals version 1 always describes memory zero. The caller
    // supplies that owner's current byte length, not a module-local memory
    // vector index or a declared maximum. No allocation or tag mutation occurs.
    [[nodiscard]] inline constexpr globals_result validate_globals(
        ::std::span<::std::byte const> bytes, ::std::uint64_t memory_bytes) noexcept
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
        if(version != globals_section_version) { result.error = globals_error::unsupported_version; return result; }
        if(!read(result.descriptor_count)) { return result; }
        result.descriptors_offset = cursor;
        // Each descriptor needs at least two bytes. This input-derived bound
        // also proves native-size representability without an arbitrary quota.
        if(result.descriptor_count > (bytes.size() - cursor) / 2u)
        { result.error = globals_error::impossible_count; return result; }
        for(::std::uint64_t index{}; index != result.descriptor_count; ++index)
        {
            ::std::uint64_t address{}, length{};
            auto const start{cursor};
            if(!read(address) || !read(length)) { return result; }
            if(address > memory_bytes || length > memory_bytes - address)
            {
                result.error = globals_error::memory_out_of_bounds;
                result.error_offset = start;
                return result;
            }
        }
        if(cursor != bytes.size())
        { result.error = globals_error::trailing_bytes; result.error_offset = cursor; }
        return result;
    }

    // The entire immutable section is proved before the first visitor call.
    // In particular a malformed last descriptor cannot partially initialize
    // allocation tags. The visitor owns tag selection and allocator locking.
    template<typename Visitor>
    inline constexpr globals_result visit_globals(::std::span<::std::byte const> bytes,
                                                    ::std::uint64_t memory_bytes, Visitor&& visitor)
    {
        auto const validated{validate_globals(bytes, memory_bytes)};
        if(!validated) { return validated; }
        auto cursor{validated.descriptors_offset};
        for(::std::uint64_t index{}; index != validated.descriptor_count; ++index)
        {
            global_descriptor descriptor{};
            // The immutable source and complete first pass prove both reads.
            (void)details::read_global_uleb(bytes, cursor, descriptor.address);
            (void)details::read_global_uleb(bytes, cursor, descriptor.byte_length);
            visitor(descriptor);
        }
        return validated;
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
