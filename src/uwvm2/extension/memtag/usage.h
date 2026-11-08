// Metadata collector used only during authoritative typed validation.
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <span>
# include "instruction.h"
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
            if(code[i]!=::std::byte{0xfc}) { continue; }
            ::std::uint32_t value{};
            for(unsigned n{};n!=5u && n+1u<code.size()-i;++n)
            {
                auto const next{::std::to_integer<unsigned>(code[i+1u+n])};
                if(n==4u && next>15u) { break; }
                value|=static_cast<::std::uint32_t>(next&127u)<<(7u*n);
                if((next&128u)==0u)
                {
                    if(find_instruction(value)!=nullptr) { return true; }
                    break;
                }
            }
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
        inline constexpr void observe_memtag(::std::uint32_t index, opcode instruction, address_width width) noexcept
        {
            if(index>=memories.size()) {valid=false;return;}
            auto& entry{memories[index]};
            if(entry.used && entry.width!=width) {valid=false;return;}
            entry.used=true;entry.width=width;
            entry.has_undefined_placeholder |= instruction==opcode::add;
        }
    };
}
