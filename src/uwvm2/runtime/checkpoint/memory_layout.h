// The final memory-record word preserves the guest pointer representation.
#pragma once
#ifndef UWVM_MODULE
# include <array>
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

    // The saved syntax and caller checks bind executable behavior. The receiving
    // host chooses the physical backend for each restored memory owner.
    [[nodiscard]] inline constexpr bool compatible_module_words(::std::array<::std::uint64_t, 8u> saved,
        ::std::array<::std::uint64_t, 8u> current) noexcept
    {
        saved[4u] &= 1u;
        current[4u] &= 1u;
        return saved == current;
    }

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
        if(word > 0xffffu || (width != 32u && width != 64u)) { return false; }
        if(!tag_bits) { return !start && ::uwvm2::extension::adm::valid(width == 64u, k, false); }
        return tag_bits == 4u && ::uwvm2::extension::adm::valid_tag_field(width == 64u, k, start);
    }
}
