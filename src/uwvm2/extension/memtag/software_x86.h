// Copyright (c) UlteSoft. Licensed under the APL-2.0 License. See LICENSE.
#pragma once
#ifndef UWVM_MODULE
# include <bit>
# include <cstddef>
# include <cstdint>
# if defined(__linux__) && (defined(__x86_64__) || defined(__i386__)) && defined(__GNUC__) && __STDC_HOSTED__
#  include <cpuid.h>
#  include <immintrin.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag::software_x86
{
    struct features
    {
        bool avx2{}, avx512bw{}, bmi2_popcnt{};
    };

    // CPUID.1:ECX.XSAVE, OSXSAVE and AVX. XGETBV must itself be gated.
    [[nodiscard]] inline constexpr bool can_read_vector_state(unsigned leaf1_ecx) noexcept
    {
        constexpr unsigned required{(1u << 26u) | (1u << 27u) | (1u << 28u)};
        return (leaf1_ecx & required) == required;
    }

    [[nodiscard]] inline constexpr features decode_features(unsigned leaf1_ecx, unsigned leaf7_ebx, unsigned xcr0) noexcept
    {
        features result{};
        // PDEP and POPCNT use integer registers and need no extended OS state.
        result.bmi2_popcnt = (leaf7_ebx & (1u << 8u)) && (leaf1_ecx & (1u << 23u));
        if(!can_read_vector_state(leaf1_ecx) || (xcr0 & 0x06u) != 0x06u) { return result; }
        result.avx2 = (leaf7_ebx & (1u << 5u)) != 0u;
        // The 64-byte kernel uses AVX-512F and BW with full 512-bit vectors.
        // Its target also enables AVX2. Require XMM/YMM, opmask and both ZMM states.
        constexpr unsigned foundation_and_bw{(1u << 16u) | (1u << 30u)};
        result.avx512bw = result.avx2 && (leaf7_ebx & foundation_and_bw) == foundation_and_bw &&
                          (xcr0 & 0xe6u) == 0xe6u;
        return result;
    }

#if defined(__linux__) && (defined(__x86_64__) || defined(__i386__)) && defined(__GNUC__) && __STDC_HOSTED__
    [[nodiscard]] inline features query_features() noexcept
    {
        if(__get_cpuid_max(0u, nullptr) < 7u) { return {}; }
        unsigned eax{}, ebx{}, ecx{}, edx{};
        __cpuid_count(1u, 0u, eax, ebx, ecx, edx);
        unsigned const leaf1_ecx{ecx};
        __cpuid_count(7u, 0u, eax, ebx, ecx, edx);
        unsigned xcr0{};
        if(can_read_vector_state(leaf1_ecx))
        {
            asm volatile("xgetbv" : "=a"(xcr0), "=d"(edx) : "c"(0u));
        }
        return decode_features(leaf1_ecx, ebx, xcr0);
    }

    inline features const host_features{query_features()};
    inline constexpr bool has_vector_kernels{true};

    // Consume complete, unaligned vectors from this pinned private shadow. On
    // a difference the lowest set bit identifies the first failing lane. The
    // caller checks any unconsumed tail and obtains the actual tag for reports.
    [[nodiscard, gnu::target("avx2"), gnu::noinline]] inline ::std::size_t matching_avx2_prefix(
        ::std::uint8_t const* tags, ::std::size_t count, ::std::uint8_t expected) noexcept
    {
        auto const wanted{_mm256_set1_epi8(static_cast<char>(expected))};
        ::std::size_t index{};
        for(; count - index >= 32u; index += 32u)
        {
            auto const bytes{_mm256_loadu_si256(reinterpret_cast<__m256i_u const*>(tags + index))};
            auto const equal{_mm256_cmpeq_epi8(bytes, wanted)};
            auto const different{~static_cast<::std::uint32_t>(_mm256_movemask_epi8(equal))};
            if(different) { return index + static_cast<unsigned>(::std::countr_zero(different)); }
        }
        return index;
    }

    [[nodiscard, gnu::target("avx2,avx512f,avx512bw"), gnu::noinline]] inline ::std::size_t matching_avx512bw_prefix(
        ::std::uint8_t const* tags, ::std::size_t count, ::std::uint8_t expected) noexcept
    {
        auto const wanted{_mm512_set1_epi8(static_cast<char>(expected))};
        ::std::size_t index{};
        for(; count - index >= 64u; index += 64u)
        {
            auto const bytes{_mm512_loadu_si512(tags + index)};
            auto const different{static_cast<::std::uint64_t>(_mm512_cmpneq_epi8_mask(bytes, wanted))};
            if(different) { return index + static_cast<unsigned>(::std::countr_zero(different)); }
        }
        return index;
    }

    [[nodiscard]] inline ::std::size_t matching_vector_prefix(
        ::std::uint8_t const* tags, ::std::size_t count, ::std::uint8_t expected) noexcept
    {
        if(count >= 64u && host_features.avx512bw) { return matching_avx512bw_prefix(tags, count, expected); }
        if(count >= 32u && host_features.avx2) { return matching_avx2_prefix(tags, count, expected); }
        return 0u;
    }
#else
    inline constexpr features host_features{};
    inline constexpr bool has_vector_kernels{};
    [[nodiscard]] inline constexpr ::std::size_t matching_vector_prefix(
        ::std::uint8_t const*, ::std::size_t, ::std::uint8_t) noexcept { return 0u; }
#endif
}
