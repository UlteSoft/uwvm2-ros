// Copyright (c) UlteSoft. Licensed under the APL-2.0 License. See LICENSE.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <bit>
# include <cstdint>
# include <span>
# include "instruction.h"
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

        [[nodiscard]] static constexpr unsigned select_bit(word bits, unsigned rank) noexcept
        {
            // Binary select on a nonempty 64-bit word. Each step keeps the
            // half containing the requested set bit. The shifts are < 64.
            unsigned offset{};
            for (unsigned half{32u}; half; half >>= 1u)
            {
                word const lower{bits & ((word{1} << half) - 1u)};
                unsigned const count{static_cast<unsigned>(::std::popcount(lower))};
                if (rank < count) { bits = lower; }
                else { rank -= count; bits >>= half; offset += half; }
            }
            return offset;
        }

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
            unsigned const words{wide_ ? 4u : 1u};
            ::std::array<unsigned, 4> counts{};
            unsigned total{};
            for (unsigned i{}; i != words; ++i)
            {
                counts[i] = static_cast<unsigned>(::std::popcount(available_[i]));
                total += counts[i];
            }
            if (!total) { return tag_selection::no_color; }
            ::std::uint32_t const product{static_cast<::std::uint32_t>(entropy) * total};
            unsigned const low{product & 0xffffu};
            if (low < total && low < 65536u % total) { return tag_selection::retry_entropy; }
            unsigned rank{product >> 16u};
            for (unsigned i{}; i != words; ++i)
            {
                if (rank < counts[i])
                {
                    out = static_cast<::std::uint8_t>(64u * i + select_bit(available_[i], rank));
                    return tag_selection::selected;
                }
                rank -= counts[i];
            }
            return tag_selection::no_color;
        }
    };
}
