/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#include "protocol.h"

namespace uwvm2::utils::update
{
    // The signed scope vector is exact LE64 data. Untagged entries retain their
    // v1 local-function meaning. Bit 63 selects a mutable reset; bit 62 selects
    // a fresh immutable binding and explicitly authorized whole-function closure.
    inline constexpr ::std::uint64_t immutable_global_tag{::std::uint64_t{1} << 62u};
    inline constexpr ::std::uint64_t scalar_tag_mask{immutable_global_tag | (::std::uint64_t{1} << 63u)};
    inline constexpr ::std::uint64_t global_reset_tag{::std::uint64_t{1} << 63u};
    inline constexpr ::std::size_t maximum_scalar_globals{4096u};
    struct scalar_reset_record
    {
        ::std::uint64_t index{};
        unsigned char type{}, width{};
        // Wasm little-endian raw values, including signed zero. Runtime compares
        // actual current bits to old_bits after drain before accepting this reset.
        ::std::array<unsigned char, 8> old_bits{}, new_bits{};
        bool immutable{};
    };
    namespace scalar_detail
    {
        struct value
        {
            unsigned char type{}, width{}, mutability{};
            ::std::array<unsigned char, 8> bits{};
        };
        inline bool payload(::std::span<unsigned char const> bytes, section const& s,
                            ::std::span<unsigned char const>& result) noexcept
        {
            if(s.offset >= bytes.size() || s.size > bytes.size() - s.offset || s.size < 2u) { return false; }
            auto pos{s.offset + 1u}; ::std::uint32_t length{};
            if(!u32(bytes.first(s.offset + s.size), pos, length) || length != s.offset + s.size - pos) { return false; }
            result = bytes.subspan(pos, length); return true;
        }
        inline bool signed_integer(::std::span<unsigned char const> bytes, ::std::size_t& pos,
                                   unsigned bits, ::std::uint64_t& value) noexcept
        {
            value = 0u; auto const maximum{(bits + 6u) / 7u};
            for(unsigned n{}; n < maximum; ++n)
            {
                if(pos == bytes.size()) { return false; }
                auto const byte{bytes[pos++]}; auto const low{static_cast<unsigned>(byte & 127u)};
                if(n + 1u == maximum)
                {
                    unsigned const used{bits - n * 7u}, sign{1u << (used - 1u)};
                    unsigned const unused_mask{127u & ~((1u << used) - 1u)};
                    if((low & unused_mask) != ((low & sign) ? unused_mask : 0u)) { return false; }
                }
                value |= static_cast<::std::uint64_t>(low) << (n * 7u);
                if((byte & 128u) == 0u)
                {
                    auto const consumed{(n + 1u) * 7u};
                    if(consumed < bits && (byte & 64u)) { value |= (~::std::uint64_t{}) << consumed; }
                    if(bits == 32u) { value &= UINT32_MAX; }
                    return true;
                }
            }
            return false;
        }
        inline bool globals(::std::span<unsigned char const> bytes, ::std::vector<value>& values)
        {
            ::std::size_t pos{}; ::std::uint32_t count{};
            if(!u32(bytes, pos, count) || count > maximum_scalar_globals) { return false; }
            values.clear(); values.reserve(count);
            for(::std::uint32_t i{}; i < count; ++i)
            {
                if(bytes.size() - pos < 3u) { return false; }
                value v{}; v.type = bytes[pos++]; v.mutability = bytes[pos++];
                if(v.mutability > 1u) { return false; }
                unsigned char opcode{};
                switch(v.type)
                {
                    case 0x7fu: v.width = 4u; opcode = 0x41u; break;
                    case 0x7eu: v.width = 8u; opcode = 0x42u; break;
                    case 0x7du: v.width = 4u; opcode = 0x43u; break;
                    case 0x7cu: v.width = 8u; opcode = 0x44u; break;
                    default: return false; // No reference carriers or extended initializers.
                }
                if(bytes[pos++] != opcode) { return false; }
                ::std::uint64_t raw{};
                if(opcode == 0x41u || opcode == 0x42u)
                { if(!signed_integer(bytes, pos, v.width * 8u, raw)) { return false; } }
                else
                {
                    if(v.width > bytes.size() - pos) { return false; }
                    for(unsigned j{}; j < v.width; ++j) { raw |= static_cast<::std::uint64_t>(bytes[pos++]) << (j * 8u); }
                    // Configuration values must be finite. Bitwise decoding
                    // avoids evaluating signalling NaNs on the host.
                    if((v.width == 4u && (raw & 0x7f800000u) == 0x7f800000u) ||
                       (v.width == 8u && (raw & 0x7ff0000000000000ull) == 0x7ff0000000000000ull)) { return false; }
                }
                if(pos == bytes.size() || bytes[pos++] != 0x0bu) { return false; }
                for(unsigned j{}; j < v.width; ++j) { v.bits[j] = static_cast<unsigned char>(raw >> (j * 8u)); }
                values.push_back(v);
            }
            return pos == bytes.size();
        }
        inline bool empty_counted_section(::std::span<unsigned char const> bytes) noexcept
        { ::std::size_t pos{}; ::std::uint32_t count{}; return u32(bytes, pos, count) && count == 0u && pos == bytes.size(); }
        inline bool scalar_type(unsigned char type) noexcept
        { return type == 0x7fu || type == 0x7eu || type == 0x7du || type == 0x7cu; }
        // Closed aliases: only numeric function signatures/locals and no global
        // exports. No table, start or initializer can retain an old binding.
        inline bool closed_immutable_aliases(::std::span<unsigned char const> module,
            ::std::span<section const> sections, ::std::span<function_body const> bodies) noexcept
        {
            for(auto const& section : sections)
            {
                ::std::span<unsigned char const> bytes{};
                if(!payload(module, section, bytes)) { return false; }
                ::std::size_t pos{}; ::std::uint32_t count{};
                if(section.id == 8u) { return false; }
                if(section.id == 4u || section.id == 13u)
                { if(!empty_counted_section(bytes)) { return false; } }
                if(section.id == 1u)
                {
                    if(!u32(bytes, pos, count) || count > 1000000u) { return false; }
                    for(::std::uint32_t i{}; i < count; ++i)
                    {
                        if(pos == bytes.size() || bytes[pos++] != 0x60u) { return false; }
                        for(unsigned side{}; side < 2u; ++side)
                        {
                            ::std::uint32_t arity{};
                            if(!u32(bytes, pos, arity) || arity > bytes.size() - pos) { return false; }
                            for(::std::uint32_t j{}; j < arity; ++j) { if(!scalar_type(bytes[pos++])) { return false; } }
                        }
                    }
                    if(pos != bytes.size()) { return false; }
                }
                if(section.id == 7u)
                {
                    if(!u32(bytes, pos, count) || count > 1000000u) { return false; }
                    for(::std::uint32_t i{}; i < count; ++i)
                    {
                        ::std::uint32_t length{}, index{};
                        if(!u32(bytes, pos, length) || length >= bytes.size() - pos) { return false; }
                        pos += length;
                        auto const kind{bytes[pos++]};
                        if(kind != 0u && kind != 2u) { return false; }
                        if(!u32(bytes, pos, index)) { return false; }
                    }
                    if(pos != bytes.size()) { return false; }
                }
            }
            for(auto const& body : bodies)
            {
                auto const bytes{module.subspan(body.offset, body.size)};
                ::std::size_t pos{}; ::std::uint32_t groups{};
                if(!u32(bytes, pos, groups) || groups > bytes.size() - pos) { return false; }
                for(::std::uint32_t i{}; i < groups; ++i)
                {
                    ::std::uint32_t count{};
                    if(!u32(bytes, pos, count) || pos == bytes.size() || !scalar_type(bytes[pos++])) { return false; }
                }
            }
            return true;
        }

    }

    inline status compare_scope(::std::span<unsigned char const> base, ::std::span<unsigned char const> target,
        ::std::span<::std::uint64_t const> scopes, ::std::vector<function_body>& bodies,
        ::std::vector<scalar_reset_record>& reset_records)
    {
        reset_records.clear(); bodies.clear();
        if(scopes.empty() || scopes.size() > maximum_functions) { return status::invalid_input; }
        for(::std::size_t i{1}; i < scopes.size(); ++i)
        { if(scopes[i] <= scopes[i-1u]) { return status::invalid_input; } }
        auto const split_scope{::std::lower_bound(scopes.begin(), scopes.end(), immutable_global_tag)};
        auto const function_count{static_cast<::std::size_t>(split_scope - scopes.begin())};
        if(split_scope == scopes.end()) { return function_scope(base, target, scopes, bodies); }
        ::std::vector<section> old_sections, new_sections;
        ::std::vector<function_body> old_bodies;
        if(!split(base, old_sections, old_bodies) || !split(target, new_sections, bodies) ||
           old_sections.size() != new_sections.size() || old_bodies.size() != bodies.size()) { return status::validation_failed; }
        if(function_count && scopes[function_count - 1u] >= bodies.size()) { return status::invalid_input; }
        ::std::vector<scalar_detail::value> old_globals, new_globals; bool found_globals{};
        for(::std::size_t i{}; i < old_sections.size(); ++i)
        {
            auto const& old{old_sections[i]}; auto const& next{new_sections[i]};
            if(old.id != next.id) { return status::scope_mismatch; }
            if(old.id != 6u && old.id != 10u && (old.size != next.size ||
               ::std::memcmp(base.data() + old.offset, target.data() + next.offset, old.size) != 0)) { return status::scope_mismatch; }
            if(old.id == 6u)
            {
                ::std::span<unsigned char const> a{}, b{};
                if(found_globals || !scalar_detail::payload(base, old, a) || !scalar_detail::payload(target, next, b) ||
                   !scalar_detail::globals(a, old_globals) || !scalar_detail::globals(b, new_globals)) { return status::unsupported; }
                found_globals = true;
            }
            // No imported authority or active/passive initialization resources
            // can derive state from a reset global in this conservative subset.
            if(old.id == 2u || old.id == 9u || old.id == 11u || old.id == 12u)
            {
                ::std::span<unsigned char const> data{};
                if(!scalar_detail::payload(base, old, data) || !scalar_detail::empty_counted_section(data)) { return status::unsupported; }
            }
        }
        if(!found_globals || old_globals.size() != new_globals.size()) { return status::scope_mismatch; }
        bool has_immutable{};
        for(auto it{split_scope}; it != scopes.end(); ++it)
        {
            auto const tag{*it & scalar_tag_mask}, index{*it & ~scalar_tag_mask};
            if((tag != global_reset_tag && tag != immutable_global_tag) || index >= new_globals.size()) { return status::invalid_input; }
            if((new_globals[index].mutability == 0u) != (tag == immutable_global_tag)) { return status::scope_mismatch; }
            has_immutable = has_immutable || tag == immutable_global_tag;
        }
        if(has_immutable)
        {
            // This deliberately never grows signed scope automatically. Every
            // callable body must be selected, including unchanged callers.
            if(function_count != bodies.size()) { return status::scope_mismatch; }
            for(::std::size_t i{}; i < function_count; ++i) { if(scopes[i] != i) { return status::scope_mismatch; } }
            if(!scalar_detail::closed_immutable_aliases(base, old_sections, old_bodies) ||
               !scalar_detail::closed_immutable_aliases(target, new_sections, bodies)) { return status::unsupported; }
        }
        bool changed{};
        for(::std::size_t i{}; i < new_globals.size(); ++i)
        {
            auto const& old{old_globals[i]}; auto const& next{new_globals[i]};
            if(old.type != next.type || old.mutability != next.mutability) { return status::scope_mismatch; }
            bool const immutable{next.mutability == 0u};
            bool const selected{::std::binary_search(split_scope, scopes.end(), (immutable ? immutable_global_tag : global_reset_tag) | i)};
            bool const differs{old.bits != next.bits};
            if(differs && !selected) { return status::scope_mismatch; }
            if(selected)
            {
                reset_records.push_back({i, next.type, next.width, old.bits, next.bits, immutable});
            }
            changed = changed || differs;
        }
        for(::std::size_t i{}; i < bodies.size(); ++i)
        {
            bool const differs{old_bodies[i].size != bodies[i].size ||
                ::std::memcmp(base.data() + old_bodies[i].offset, target.data() + bodies[i].offset, old_bodies[i].size) != 0};
            if(differs && !::std::binary_search(scopes.begin(), split_scope, i)) { return status::scope_mismatch; }
            changed = changed || differs;
        }
        return changed ? status::ok : status::invalid_input;
    }
}
