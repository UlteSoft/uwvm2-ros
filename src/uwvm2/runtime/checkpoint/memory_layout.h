// The final memory-record word preserves the guest pointer representation.
#pragma once
#ifndef UWVM_MODULE
# include <cstdint>
# include <uwvm2/extension/adm/types.h>
# include <uwvm2/extension/memtag/execution_policy.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::checkpoint::memory_layout
{
    [[nodiscard]] inline constexpr ::std::uint64_t module_policy(::uwvm2::extension::memtag::module_memory_policy policy) noexcept
    { return (static_cast<::std::uint64_t>(policy.backend) << 1u) | !policy.check_tags; }

    [[nodiscard]] inline constexpr unsigned ignored_bits(::std::uint64_t word) noexcept { return static_cast<unsigned>(word & 0xffu); }
    [[nodiscard]] inline constexpr unsigned tag_start(::std::uint64_t word) noexcept { return static_cast<unsigned>((word >> 8u) & 0xffu); }

    template<typename Shadow>
    [[nodiscard]] inline constexpr ::std::uint64_t encode(Shadow const& shadow) noexcept
    {
        return shadow.ignored_bits() | (shadow.enabled() ? static_cast<::std::uint64_t>(shadow.tag_start_bit()) << 8u : 0u);
    }
    [[nodiscard]] inline constexpr bool valid(::std::uint64_t word, ::std::uint64_t width, ::std::uint64_t tag_bits) noexcept
    {
        auto const k{ignored_bits(word)}, start{tag_start(word)};
        if(word > 0xffffu || (width != 32u && width != 64u) || !::uwvm2::extension::adm::valid(width == 64u, k, tag_bits != 0u)) { return false; }
        if(!tag_bits) { return !start; }
        return tag_bits == 4u && start >= width - k &&
            (start == width - 4u || (width == 64u && (start == 56u || start == 57u)));
    }
    [[nodiscard]] inline constexpr ::uwvm2::extension::memtag::owner_backend backend(::std::uint64_t word) noexcept
    {
        using type = ::uwvm2::extension::memtag::owner_backend;
        auto const start{tag_start(word)};
        return start == 56u ? type::arm_mte : start == 57u ? type::x86_mtt : type::software;
    }
}
