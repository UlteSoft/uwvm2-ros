// Copyright (c) UlteSoft. Licensed under the APL-2.0 License. See LICENSE.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <bit>
# include <cstddef>
# include <cstdint>
# include <span>
# include "instruction.h"
# include "software_x86.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    inline constexpr unsigned allocation_gap_bytes{64u};

    /// @brief Compare disjoint, inclusive granule ranges on one backing memory.
    /// Adjacent granules have zero bytes between them. Subtraction in address
    /// order also handles the largest index without endpoint wraparound.
    [[nodiscard]] inline constexpr bool allocations_near(::std::uint64_t first_a, ::std::uint64_t last_a,
                                                         ::std::uint64_t first_b, ::std::uint64_t last_b) noexcept
    {
        constexpr auto distance{1u + allocation_gap_bytes / granule_size};
        if (first_a > last_a || first_b > last_b) { return false; }
        if (last_a < first_b) { return first_b - last_a <= distance; }
        return last_b < first_a && first_a - last_b <= distance;
    }

    /// @brief Visit exactly those external granules whose intervening gap
    /// does not exceed the exclusion radius. The caller validates the range
    /// and supplies its backing owner's actual granule count.
    template<typename Visitor>
    [[nodiscard]] inline constexpr bool visit_allocation_neighbors(
        ::std::uint64_t first, ::std::uint64_t last, ::std::uint64_t count, Visitor&& visit) noexcept
    {
        if (last >= count || first > last) { return false; }
        constexpr unsigned reach{1u + allocation_gap_bytes / granule_size};
        for (unsigned d{1u}; d <= reach; ++d)
        {
            if (first >= d && !visit(first - d)) { return false; }
            if (d < count - last && !visit(last + d)) { return false; }
        }
        return true;
    }

    enum class tag_selection { selected, retry_entropy, no_color };

    /// @brief Select uniformly over legal allocation tags using one uniform
    /// 16-bit entropy word. The caller supplies exclusions from the same locked
    /// owner snapshot in which the reservation or allocation will be published.
    class allocation_neighborhood
    {
        using word = ::std::uint64_t;
        ::std::array<word, 4> available_{};
        bool wide_{};
        bool physical_{};

        // Each entry holds eight three-bit positions and a population in its top byte.
        static constexpr auto byte_entries_ = []() constexpr noexcept
        {
            ::std::array<::std::uint32_t,256> positions{};
            for(unsigned byte{}; byte < positions.size(); ++byte)
            {
                unsigned shift{};
                for(unsigned bit{}; bit != 8u; ++bit)
                {
                    if(byte & (1u << bit)) { positions[byte] |= bit << shift; shift += 3u; }
                }
                positions[byte] |= static_cast<::std::uint32_t>(::std::popcount(byte)) << 24u;
            }
            return positions;
        }();

        [[nodiscard]] static constexpr unsigned byte_position(::std::uint32_t entry, unsigned rank) noexcept
        { return (entry >> (3u * rank)) & 7u; }

        [[nodiscard]] static constexpr unsigned select_bit(word bits, unsigned rank) noexcept
        {
            if(bits == ~word{}) { return rank; }
            // Count the set bits in each byte, then sum from the least significant byte.
            constexpr word ones{UINT64_C(0x0101010101010101)};
            constexpr word high_bits{ones * 0x80u};
            word counts{bits - ((bits >> 1u) & (ones * 0x55u))};
            counts = (counts & (ones * 0x33u)) + ((counts >> 2u) & (ones * 0x33u));
            word const prefixes{((counts + (counts >> 4u)) & (ones * 0x0fu)) * ones};
            // Each prefix is at most 64. Setting the high bits before subtracting
            // prevents borrows between bytes and marks every prefix greater than rank.
            word const reached{((prefixes | high_bits) - (rank + 1u) * ones) & high_bits};
            unsigned const shift{static_cast<unsigned>(::std::countr_zero(reached)) & ~7u};
            rank -= static_cast<::std::uint8_t>((prefixes << 8u) >> shift);
            return shift + byte_position(byte_entries_[static_cast<::std::uint8_t>(bits >> shift)], rank);
        }

        [[nodiscard]] static constexpr tag_selection choose_rank(::std::uint16_t entropy, unsigned total, unsigned& rank) noexcept
        {
            if (!total) { return tag_selection::no_color; }
            ::std::uint32_t const product{static_cast<::std::uint32_t>(entropy) * total};
            unsigned const low{product & 0xffffu};
            if (low < total && low < 65536u % total) { return tag_selection::retry_entropy; }
            rank = product >> 16u;
            return tag_selection::selected;
        }

        template<auto BitPosition>
        [[nodiscard,gnu::always_inline]] constexpr tag_selection select_wide(::std::uint16_t entropy, ::std::uint8_t& out) const noexcept
        {
            unsigned const first{static_cast<unsigned>(::std::popcount(available_[0]))};
            unsigned const middle{first + static_cast<unsigned>(::std::popcount(available_[1]))};
            unsigned const third{middle + static_cast<unsigned>(::std::popcount(available_[2]))};
            unsigned const total{third + static_cast<unsigned>(::std::popcount(available_[3]))};
            unsigned rank{};
            auto const status{choose_rank(entropy, total, rank)};
            if(status != tag_selection::selected) { return status; }

            // Locate the ranked word by its exclusive end in ascending tag order.
            bool const upper{rank >= middle};
            unsigned const split{upper ? third : first};
            bool const second{rank >= split};
            unsigned const index{2u * static_cast<unsigned>(upper) + static_cast<unsigned>(second)};
            rank -= second ? split : (upper ? middle : 0u);
            out = static_cast<::std::uint8_t>(64u * index + BitPosition(available_[index], rank));
            return tag_selection::selected;
        }

#if (defined(__x86_64__) || defined(__i386__)) && defined(__linux__) && defined(__GNUC__) && __STDC_HOSTED__
        [[nodiscard,gnu::target("bmi2,popcnt")]] static unsigned select_deposited_bit(word bits, unsigned rank) noexcept
        {
            // Depositing a single set bit places it at the requested available position.
# if defined(__x86_64__)
            auto const selected{static_cast<word>(__builtin_ia32_pdep_di(word{1} << rank, bits))};
            return static_cast<unsigned>(::std::countr_zero(selected));
# else
            // The 32-bit host has PDEP r32. Select the ranked half of the word first.
            auto const low{static_cast<::std::uint32_t>(bits)};
            auto const low_count{static_cast<unsigned>(::std::popcount(low))};
            bool const upper{rank >= low_count};
            unsigned const offset{upper ? 32u : 0u};
            auto const half{static_cast<::std::uint32_t>(bits >> offset)};
            if(upper) { rank -= low_count; }
            auto const selected{__builtin_ia32_pdep_si(1u << rank, half)};
            return offset + static_cast<unsigned>(::std::countr_zero(selected));
# endif
        }

        [[nodiscard,gnu::target("bmi2,popcnt")]] tag_selection select_wide_bmi2(::std::uint16_t entropy, ::std::uint8_t& out) const noexcept
        { return select_wide<select_deposited_bit>(entropy, out); }
#endif

    public:
        constexpr explicit allocation_neighborhood(address_width width, bool physical_four_bits = false) noexcept
            : wide_{width == address_width::i64}, physical_{physical_four_bits && wide_}
        {
            if (wide_) { available_.fill(~word{}); }
            else { available_[0] = 0xffffu; }
            forbid(0u);
        }

        /// @brief Exclude a neighbor, reserved allocation or backing tag.
        /// With physical MTE selected, all encodings with the same low nibble
        /// are excluded as well. Zero always remains unavailable.
        constexpr void forbid(::std::uint8_t tag) noexcept
        {
            if (physical_)
            {
                word const mask{~(UINT64_C(0x0001000100010001) << (tag & 15u))};
                for (auto& entry : available_) { entry &= mask; }
            }
            else
            {
                unsigned const value{wide_ ? unsigned(tag) : unsigned(tag & 15u)};
                available_[value >> 6u] &= ~(word{1} << (value & 63u));
            }
        }

        template<::std::size_t Left, ::std::size_t Right=0>
        [[gnu::always_inline]] constexpr void forbid_tags(::std::span<::std::uint8_t const,Left> left,
                                                          ::std::span<::std::uint8_t const,Right> right={}) noexcept
        {
            if(wide_)
            {
                for(auto tag : left) { forbid(tag); }
                for(auto tag : right) { forbid(tag); }
            }
            else
            {
                unsigned mask{};
                for(auto tag : left) { mask |= 1u << (tag & 15u); }
                for(auto tag : right) { mask |= 1u << (tag & 15u); }
                available_[0] &= ~static_cast<word>(mask);
            }
        }

        /// @brief Identify the tag as seen by this owner's effective comparison.
        [[nodiscard]] constexpr unsigned tag_class(unsigned tag) const noexcept
        { return wide_ && !physical_ ? tag : tag & 15u; }

        [[nodiscard]] constexpr bool empty() const noexcept
        { return !(available_[0] | available_[1] | available_[2] | available_[3]); }

        /// @brief Narrow the legal set if it intersects another set from the same owner.
        [[nodiscard]] constexpr bool prefer(allocation_neighborhood const& subset) noexcept
        {
            auto intersection{available_};
            for (unsigned i{}; i != intersection.size(); ++i) { intersection[i] &= subset.available_[i]; }
            if (!(intersection[0] | intersection[1] | intersection[2] | intersection[3])) { return false; }
            available_ = intersection;
            return true;
        }

        /// @brief Keep the legal colors shared by the fewest permanent extents.
        /// @details The startup planner supplies owner-local counts. Hard
        /// exclusions are applied first, and dynamic allocations skip this step.
        [[nodiscard]] constexpr bool prefer_least_used(::std::span<::std::uint64_t const> counts) noexcept
        {
            if (counts.size() <= tag_class(255u)) { return false; }
            ::std::array<word, 4> preferred{};
            ::std::uint64_t least{UINT64_MAX};
            unsigned const words{wide_ && !physical_ ? 4u : 1u};
            for (unsigned i{}; i != words; ++i)
            {
                for (word bits{physical_ ? available_[i] & 0xffffu : available_[i]}; bits; bits &= bits - 1u)
                {
                    auto const bit{static_cast<unsigned>(::std::countr_zero(bits))};
                    auto const count{counts[64u * i + bit]};
                    if (count < least)
                    {
                        preferred = {};
                        least = count;
                    }
                    if (count == least) { preferred[i] |= word{1} << bit; }
                }
            }
            if (physical_) { preferred.fill(preferred[0] * UINT64_C(0x0001000100010001)); }
            available_ = preferred;
            return true;
        }

        /// @brief Produce a tag only on success. Reject the incomplete entropy
        /// bucket to give every legal tag exactly the same number of preimages.
        [[nodiscard]] constexpr tag_selection select(::std::uint16_t entropy, ::std::uint8_t& out) const noexcept
        {
            if(wide_)
            {
#if (defined(__x86_64__) || defined(__i386__)) && defined(__linux__) && defined(__GNUC__) && __STDC_HOSTED__
                if(!__builtin_is_constant_evaluated() && software_x86::host_features.bmi2_popcnt)
                { return select_wide_bmi2(entropy, out); }
#endif
                return select_wide<select_bit>(entropy, out);
            }
            auto const low{byte_entries_[static_cast<::std::uint8_t>(available_[0])]};
            auto const high{byte_entries_[static_cast<::std::uint8_t>(available_[0] >> 8u)]};
            unsigned const lower_count{low >> 24u};
            unsigned rank{};
            auto const status{choose_rank(entropy, lower_count + (high >> 24u), rank)};
            if(status != tag_selection::selected) { return status; }
            bool const upper{rank >= lower_count};
            if(upper) { rank -= lower_count; }
            out = static_cast<::std::uint8_t>((upper ? 8u : 0u) + byte_position(upper ? high : low, rank));
            return tag_selection::selected;
        }
    };
}
