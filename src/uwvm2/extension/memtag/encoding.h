/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include "instruction.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    // Private FF ULEB32((version << 24) | (domain << 16) | operation)
    //            ULEB32(memoryidx). Core 3 reserves FF against allocation to a
    // standard opcode/prefix; that does not register this private wire format.
    // Consume exactly the instruction word. Ordinary validators still resolve
    // and type-check memoryidx, operands, feature gates, and canonical owners.

    enum class encoding_error : unsigned char
    {
        none, malformed_integer, unsupported_domain,
        unsupported_version, unknown_operation, reserved_operation
    };
    struct decoded_opcode
    {
        ::std::uint_least32_t subopcode{};
        encoding_error error{};
    };
    namespace encoding_details
    {
        // The caller proves [current,end) belongs to the same immutable source
        // buffer. Each byte is read only after current != end; padded ULEB32 is
        // accepted but its fifth byte may contain only the low four bits.
        [[nodiscard]] inline constexpr bool read_u32(::std::byte const*& current,
            ::std::byte const* end, ::std::uint_least32_t& result) noexcept
        {
            result = 0u;
            for(unsigned index{}; index != 5u; ++index)
            {
                if(current == end) { return false; }
                auto const byte{::std::to_integer<unsigned>(*current)};
                ++current; // The preceding check proved this one-byte advance.
                if(index == 4u && byte > 15u) { return false; }
                result |= static_cast<::std::uint_least32_t>(byte & 127u) << (7u * index);
                if((byte & 128u) == 0u) { return true; }
            }
            return false;
        }
    }
    [[nodiscard]] inline constexpr decoded_opcode scan_opcode(
        ::std::byte const*& current, ::std::byte const* end) noexcept
    {
        // Entry is AFTER the checked 0xff byte. Failure never commits a cursor
        // advance; unknown namespaces never fall through to standard FC ops.
        auto* cursor{current};
        ::std::uint_least32_t word{};
        if(!encoding_details::read_u32(cursor, end, word))
        { return {0u, encoding_error::malformed_integer}; }
        auto const version{word >> 24u};
        auto const domain{(word >> 16u) & 0xffu};
        auto const operation{word & 0xffffu};
        if(version != encoding_version) { return {0u, encoding_error::unsupported_version}; }
        if(domain != linear_encoding_domain) { return {0u, encoding_error::unsupported_domain}; }
        if(find_instruction(operation) == nullptr) { return {0u, encoding_error::unknown_operation}; }
        if(operation == static_cast<::std::uint_least32_t>(opcode::add))
        { return {0u, encoding_error::reserved_operation}; }
        // Every read above was bounded by the same source image. This is the
        // only caller-cursor commit, at the following memoryidx immediate.
        current = cursor;
        return {operation, encoding_error::none};
    }
}
