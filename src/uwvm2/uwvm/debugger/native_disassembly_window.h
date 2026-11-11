/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <cstddef>
# include <cstdint>
# include <limits>
# include <span>
# include "native_disassembly.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::debugger::native_disassembly
{
    inline constexpr ::std::size_t max_window_count{32u};
    struct window
    {
        bool available{};
        ::std::size_t count{};
        ::std::array<instruction, max_window_count> instructions{};
    };

    // Page sizes bound one response; offsets are the complete signed wire type.
    // Do not reserve an arbitrary prefix of the authenticated function.
    [[nodiscard]] inline constexpr bool add_window_offset(::std::int64_t left,
        ::std::int64_t right, ::std::int64_t& result) noexcept
    {
        if((right > 0 && left > INT64_MAX - right) ||
           (right < 0 && left < INT64_MIN - right)) { return false; }
        result = left + right;
        return true;
    }

    [[nodiscard]] inline constexpr ::std::uint64_t negative_magnitude(::std::int64_t value) noexcept
    { return static_cast<::std::uint64_t>(-(value + 1)) + 1u; }

    [[nodiscard]] inline constexpr ::std::size_t window_position(::std::size_t ordinal,
        ::std::int64_t first, ::std::size_t count) noexcept
    {
        if(first < 0)
        {
            auto const before{negative_magnitude(first)};
            if(before >= count || ordinal >= count - before) { return count; }
            return ordinal + static_cast<::std::size_t>(before);
        }
        auto const start{static_cast<::std::uint64_t>(first)};
        if(ordinal < start || ordinal - start >= count) { return count; }
        return static_cast<::std::size_t>(ordinal - start);
    }

    [[nodiscard]] inline constexpr bool past_window(::std::size_t ordinal,
        ::std::int64_t first, ::std::size_t count) noexcept
    {
        if(first < 0)
        {
            auto const before{negative_magnitude(first)};
            return before >= count || ordinal >= count - before;
        }
        auto const start{static_cast<::std::uint64_t>(first)};
        return ordinal >= start && ordinal - start >= count;
    }

    // This helper consumes OWNED copied bytes, not a publication or credential.
    // owner_begin/stop_pc are display positions; neither becomes a host pointer.
    // Only the caller's private runtime query can authenticate these arguments.
    // Decode FORWARD from the exact function entry: never guess an x86 boundary
    // by looking backwards at an arbitrary PC. Unknown boundaries fail closed.
    template<typename Decoder, typename SizeDecoder = Decoder>
    [[nodiscard]] inline window decode_window_with(Decoder& decoder,
        ::std::span<::std::uint8_t const> owned, ::std::uintptr_t owner_begin,
        ::std::uintptr_t stop_pc, ::std::int64_t byte_offset,
        ::std::int64_t instruction_offset, ::std::size_t requested,
        ::std::span<::std::uint8_t const> instruction_code = {}, SizeDecoder* sizes = nullptr) noexcept
    {
        window result{};
        if(!decoder || owner_begin == 0u || owned.empty() || owned.size() > PTRDIFF_MAX ||
           owned.size() > UINTPTR_MAX - owner_begin || stop_pc < owner_begin ||
           stop_pc - owner_begin >= owned.size() || requested == 0u || requested > max_window_count)
        { return result; }
        ::std::int64_t target{};
        auto const inside{add_window_offset(static_cast<::std::int64_t>(stop_pc - owner_begin), byte_offset, target) &&
            target >= 0 && static_cast<::std::uint64_t>(target) < owned.size()};
        // Only the private runtime supplies an ARM ELF mapping mask. It is
        // not code permission. Require complete aligned 0/1 regions; do not
        // infer a restart from undecodable bytes or arbitrary offsets.
        if(!instruction_code.empty())
        {
            if(instruction_code.size()!=owned.size() || owner_begin%4u!=0u ||
               owned.size()%4u!=0u || instruction_code[0u]!=1u ||
               instruction_code[stop_pc-owner_begin]!=1u) { return {}; }
            for(::std::size_t i{};i<instruction_code.size();++i)
            {
                if(instruction_code[i]>1u ||
                   (i!=0u && instruction_code[i]!=instruction_code[i-1u] && i%4u!=0u)) { return {}; }
            }
        }
        if constexpr(requires { decoder.fixed_instruction_bytes(); })
        {
            if(decoder.fixed_instruction_bytes() == 4u && instruction_code.empty())
            {
                // A64 has exact aligned four-byte slots. Paginate owned copied
                // bytes directly, including pages before the current stop; MC
                // still validates every returned slot. No host PC is read.
                if(owner_begin % 4u || stop_pc % 4u || owned.size() % 4u || byte_offset % 4)
                { return {}; }
                if(!inside) { result.available=true; result.count=requested; return result; }
                auto const anchor{target / 4};
                // Overflow describes an unavailable page, never a host read.
                ::std::int64_t first{};
                if(!add_window_offset(anchor, instruction_offset, first))
                { result.available=true; result.count=requested; return result; }
                for(::std::size_t index{}; index != requested; ++index)
                {
                    ::std::int64_t ordinal{};
                    if(!add_window_offset(first, static_cast<::std::int64_t>(index), ordinal)) { break; }
                    if(ordinal<0) { continue; }
                    if(static_cast<::std::uint64_t>(ordinal)>=owned.size()/4u) { break; }
                    auto const cursor{static_cast<::std::size_t>(ordinal)*4u};
                    auto const pc{owner_begin+cursor};
                    auto const decoded{decoder.decode(pc,{owned.data()+cursor,4u})};
                    if(!decoded || decoded.pc!=pc || decoded.size!=4u) { break; }
                    result.instructions[index]=decoded;
                }
                result.available=true;result.count=requested;return result;
            }
        }
        auto const scan{[&](::std::uintptr_t pc, ::std::span<::std::uint8_t const> bytes) noexcept -> instruction
        {
            if constexpr(requires { sizes->instruction_size(pc, bytes); })
            {
                if(sizes != nullptr)
                {
                    instruction row{}; row.pc = pc; row.size = sizes->instruction_size(pc, bytes);
                    return row;
                }
            }
            return decoder.decode(pc, bytes);
        }};
        auto const materialize{[&](instruction const& scanned, ::std::span<::std::uint8_t const> bytes) noexcept
        {
            if constexpr(requires { sizes->instruction_size(scanned.pc, bytes); })
            {
                if(sizes != nullptr)
                {
                    auto const shown{decoder.decode(scanned.pc, bytes)};
                    if(!shown || shown.pc != scanned.pc || shown.size != scanned.size) { return instruction{}; }
                    return shown;
                }
            }
            return scanned;
        }};
        if(inside && !instruction_code.empty() && instruction_code[static_cast<::std::size_t>(target)]!=1u)
        { return {}; } // A literal-pool byte never becomes a requested instruction.
        constexpr auto absent{(::std::numeric_limits<::std::size_t>::max)()};
        ::std::size_t cursor{}, total{}, target_index{absent};
        ::std::size_t region_end{instruction_code.empty() ? owned.size() : 0u};
        auto const advance_region{[&]() noexcept
        {
            if(!instruction_code.empty() && cursor==region_end)
            {
                while(cursor<owned.size() && instruction_code[cursor]==0u) { ++cursor; }
                region_end=cursor;
                while(region_end<owned.size() && instruction_code[region_end]==1u) { ++region_end; }
            }
            return cursor<owned.size();
        }};
        bool stop_seen{}, first_representable{};
        ::std::int64_t first{};
        // First prove the stop and byte target by forward decoding. Only scalar
        // indices are retained: no 8192-entry table and no allocation in this
        // noexcept policy. Unknown bytes never become a guessed new boundary.
        while(cursor < owned.size())
        {
            if(!advance_region()) { break; }
            // [owned.data ... cursor < owned.size) end
            // [safe] authenticate scalar extent BEFORE forming the suffix.
            auto const decoded{scan(owner_begin + cursor, {owned.data() + cursor, region_end - cursor})};
            if(!decoded || decoded.pc != owner_begin + cursor || decoded.size == 0u || decoded.size > region_end - cursor)
            {
                if(!stop_seen) { return {}; }
                break;
            }
            if(owner_begin + cursor == stop_pc) { stop_seen = true; }
            if(inside && cursor == static_cast<::std::size_t>(target))
            {
                target_index = total;
                first_representable = add_window_offset(static_cast<::std::int64_t>(target_index), instruction_offset, first);
            }
            if(instruction_offset >= 0 && target_index != absent && first_representable)
            {
                // Retain only the bounded forward page during this SAME
                // entry-to-stop proof. If the stop is still ahead, continue
                // proving it; no retained row alone makes the window available.
                auto const index{window_position(total, first, requested)};
                if(index < requested)
                {
                    auto const shown{materialize(decoded, {owned.data() + cursor, region_end - cursor})};
                    if(!shown) { return {}; }
                    result.instructions[static_cast<::std::size_t>(index)] = shown;
                }
            }
            ++total;
            cursor += decoded.size; // nonzero size <= remaining proves progress.
            if(stop_seen && ((!inside) || (target_index != absent &&
                (!first_representable || past_window(total, first, requested))))) { break; }
        }
        if(!stop_seen) { return {}; }
        result.available = true; result.count = requested;
        if(!inside) { return result; }
        if(target_index == absent)
        {
            // Inside an already decoded instruction is not a boundary; an
            // unknown suffix is only an unavailable filler, never resynchronized.
            if(static_cast<::std::size_t>(target) < cursor) { return {}; }
            return result;
        }
        if(!first_representable || past_window(0u, first, requested) ||
           (first >= 0 && static_cast<::std::uint64_t>(first) >= total)) { return result; }
        // Nonnegative pages were copied from the exact validated first
        // walk. Negative instruction offsets still use the bounded second
        // forward pass; no reverse decode, guessed boundary or table quota.
        if(instruction_offset >= 0) { return result; }
        // A second forward walk materializes at most one bounded page from the
        // known prefix. Negative instruction offsets are never a backwards read
        // or x86 resynchronization. Memory usage is constant for large functions.
        auto const prefix_end{cursor}; cursor = 0u;
        region_end=instruction_code.empty() ? owned.size() : 0u;
        ::std::size_t ordinal{};
        while(cursor < prefix_end && !past_window(ordinal, first, requested))
        {
            if(!advance_region() || cursor>=prefix_end) { break; }
            auto const decoded{scan(owner_begin + cursor, {owned.data() + cursor, region_end - cursor})};
            if(!decoded || decoded.pc != owner_begin + cursor || decoded.size == 0u || decoded.size > prefix_end - cursor || decoded.size > region_end - cursor)
            { return {}; }
            auto const index{window_position(ordinal, first, requested)};
            if(index < requested)
            {
                auto const shown{materialize(decoded, {owned.data() + cursor, region_end - cursor})};
                if(!shown) { return {}; }
                result.instructions[static_cast<::std::size_t>(index)] = shown;
            }
            ++ordinal; cursor += decoded.size;
        }
        return result;
    }
    [[nodiscard]] inline window decode_window(::std::span<::std::uint8_t const> owned,
        ::std::uintptr_t owner_begin, ::std::uintptr_t stop_pc, ::std::int64_t byte_offset,
        ::std::int64_t instruction_offset, ::std::size_t requested,
        ::std::span<::std::uint8_t const> instruction_code = {}) noexcept
    {
        decoder context{};
        return decode_window_with(context, owned, owner_begin, stop_pc, byte_offset, instruction_offset, requested, instruction_code);
    }

    // Policy over OWNED copied function bytes only. Never dereference the PC or
    // user offset. Forward decode proves variable-width instruction boundaries,
    // and instruction_code excludes literal pools on mapped-code targets.
    // All generated instructions in this authenticated JIT owner use the same
    // breakpoint/step policy, including compiler-generated GC/debug bridges.
    // The caller separately validates the actual parked native session and
    // public instruction projection; this copied result cannot authorize host
    // code, registers, data access, or a cross-owner execution transition.
    template<typename Decoder, typename Image>
    [[nodiscard]] inline instruction guest_breakpoint_with(Decoder& decoder, Image const& image,
        ::std::uint64_t offset) noexcept
    {
        if(offset > INT64_MAX || offset >= image.size || !image.complete_storage() ||
           image.owner_begin == 0u || image.owner_end <= image.owner_begin ||
           image.owner_end - image.owner_begin != image.size) { return {}; }
        auto const page{decode_window_with(decoder, {image.bytes.data(), image.size},
            image.owner_begin, image.owner_begin, static_cast<::std::int64_t>(offset), 0, 1u, image.instruction_code)};
        if(!page.available || page.count != 1u || !page.instructions[0u]) { return {}; }
        auto const candidate{page.instructions[0u]};
        if(candidate.pc != image.owner_begin + offset || candidate.size > image.size - offset) { return {}; }
        // guest_code is the source-opcode correlation mask. Requiring it
        // here rejects real owned JIT instructions which native stepping already
        // admits (notably GC/codegen bridges). It is not an executable-text map.
        // decode_window_with has proved the complete instruction in owned text.
        return candidate;
    }
}
