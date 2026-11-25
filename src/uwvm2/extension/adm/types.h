// WasmADM is a per-memory type property. Zero ignored bits is ordinary memory.
#pragma once
#ifndef UWVM_MODULE
# include <cstdint>
# include <limits>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::adm
{
    inline constexpr unsigned memory_type_flag{0x10u};

    template<typename Memory>
    [[nodiscard]] inline constexpr unsigned ignored_bits(Memory const& memory) noexcept
    {
        if constexpr(requires { memory.adm_bits; }) { return memory.adm_bits; }
        else { return 0u; } // The MVP memory type has no extensions.
    }

    template<typename Memory>
    [[nodiscard]] inline constexpr unsigned page_shift(Memory const& memory) noexcept
    {
        if constexpr(requires { memory.page_size_log2; }) { return memory.page_size_log2; }
        else { return 16u; }
    }

    [[nodiscard]] inline constexpr bool valid(bool address64, unsigned k, bool tagged) noexcept
    { return k < (address64 ? 64u : 32u) && (!tagged || k >= 4u); }

    // Full unsigned arithmetic proves every masked base and every byte of a
    // fixed-width access stays within this extent, including its static memarg.
    [[nodiscard]] inline constexpr bool masked_access_fits(::std::uint_least64_t mask,
        ::std::uint_least64_t offset, ::std::uint_least64_t bytes, ::std::uint_least64_t extent) noexcept
    { return bytes && offset <= extent && bytes <= extent - offset && mask <= extent - offset - bytes; }

    // Count complete pages without ever forming 2^64 in a 64-bit byte count.
    [[nodiscard]] inline constexpr ::std::uint_least64_t maximum_pages(
        bool address64, unsigned k, bool tagged, unsigned page_shift) noexcept
    {
        if(!valid(address64, k, tagged)) { return 0u; }
        unsigned bits{(address64 ? 64u : 32u) - k};
        if(address64 && tagged && bits > 48u) { bits = 48u; }
        if(page_shift > bits) { return 0u; }
        unsigned const exponent{bits - page_shift};
        // The page count returned by memory.size must fit its index type.
        if(!address64 && exponent == 32u) { return 0xffff'ffffu; }
        if(exponent == 64u) { return (::std::numeric_limits<::std::uint_least64_t>::max)(); }
        return ::std::uint_least64_t{1u} << exponent;
    }
}
