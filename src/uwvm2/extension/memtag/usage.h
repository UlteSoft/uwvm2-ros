// Metadata collector used only during authoritative typed validation.
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <span>
# include "encoding.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    // Conservative fast filter only: arbitrary immediates can match. A true
    // result always requires authoritative typed validation before enabling an
    // owner. Every valid canonical or padded MTE subopcode necessarily matches.
    [[nodiscard]] inline constexpr bool may_contain_memtag(::std::span<::std::byte const> code) noexcept
    {
        for(::std::size_t i{};i<code.size();++i)
        {
            // A conservative FF match also catches padded and malformed
            // instruction words. Typed validation alone can admit the owner.
            if(code[i]==::std::byte{opcode_prefix}) { return true; }
            // Obsolete FC MTE must also reach authoritative validation, even
            // in an unused function. This is rejection detection, not support.
            if(code[i]!=::std::byte{0xfc}) { continue; }
            auto const* cursor{code.data()+i+1u};
            ::std::uint_least32_t operation{};
            if(encoding_details::read_u32(cursor,code.data()+code.size(),operation) &&
               find_instruction(operation)!=nullptr) { return true; }
        }
        return false;
    }

    /// A replacement function body is checkpoint DATA and has not yet been
    /// parsed with its replacement local declarations. Until a typed replacement
    /// is available, refuse any body containing a complete managed instruction
    /// word. This may conservatively refuse matching bytes in an immediate.
    [[nodiscard]] inline constexpr bool may_require_allocation_directory(::std::span<::std::byte const> bytes) noexcept
    {
        for(::std::size_t index{}; index < bytes.size(); ++index)
        {
            if(bytes[index] != ::std::byte{opcode_prefix}) { continue; }
            auto cursor{bytes.data() + index + 1u};
            auto const instruction{scan_opcode(cursor, bytes.data() + bytes.size())};
            if(instruction.error == encoding_error::none &&
               is_allocation_operation(static_cast<opcode>(instruction.subopcode))) { return true; }
        }
        return false;
    }

    struct memory_usage
    {
        bool used{};
        address_width width{};
        bool has_undefined_placeholder{};
    };
    // Scratch DATA, not a memory-enable permit. The caller must discard the
    // entire plan if any function or global descriptor fails validation.
    struct usage_sink
    {
        static constexpr bool retains_operations{false};
        ::std::span<memory_usage> memories{};
        bool valid{true};
        bool classify_only{};
        bool uses_allocation_directory{};
        inline constexpr void observe_memtag(::std::uint32_t index, opcode instruction, address_width width) noexcept
        {
            uses_allocation_directory |= is_allocation_operation(instruction);
            if(classify_only) { return; } // The typed validator already resolved the memory and operand widths.
            if(index>=memories.size()) {valid=false;return;}
            auto& entry{memories[index]};
            if(entry.used && entry.width!=width) {valid=false;return;}
            entry.used=true;entry.width=width;
            entry.has_undefined_placeholder |= instruction==opcode::add;
        }
    };
}
