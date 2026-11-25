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
        ::std::uint16_t available_{0xfffeu};

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

        [[nodiscard]] static constexpr tag_selection choose_rank(::std::uint16_t entropy, unsigned total, unsigned& rank) noexcept
        {
            if (!total) { return tag_selection::no_color; }
            ::std::uint32_t const product{static_cast<::std::uint32_t>(entropy) * total};
            unsigned const low{product & 0xffffu};
            if (low < total && low < 65536u % total) { return tag_selection::retry_entropy; }
            rank = product >> 16u;
            return tag_selection::selected;
        }

    public:
        constexpr explicit allocation_neighborhood(address_width, bool = false) noexcept {}

        constexpr void forbid(::std::uint8_t tag) noexcept
        { available_ &= static_cast<::std::uint16_t>(~(1u << (tag & 15u))); }

        template<::std::size_t Left, ::std::size_t Right=0>
        constexpr void forbid_tags(::std::span<::std::uint8_t const,Left> left,
                                   ::std::span<::std::uint8_t const,Right> right={}) noexcept
        {
            unsigned excluded{};
            for(auto tag : left) { excluded |= 1u << tag_class(tag); }
            for(auto tag : right) { excluded |= 1u << tag_class(tag); }
            available_ &= static_cast<::std::uint16_t>(~excluded);
        }

        [[nodiscard]] static constexpr unsigned tag_class(unsigned tag) noexcept { return tag & 15u; }
        [[nodiscard]] constexpr bool empty() const noexcept { return !available_; }

        [[nodiscard]] constexpr bool prefer(allocation_neighborhood const& subset) noexcept
        {
            auto const intersection{static_cast<::std::uint16_t>(available_ & subset.available_)};
            if(!intersection) { return false; }
            available_=intersection;
            return true;
        }

        [[nodiscard]] constexpr bool prefer_least_used(::std::span<::std::uint64_t const> counts) noexcept
        {
            if(counts.size()<16u) { return false; }
            ::std::uint16_t preferred{};
            ::std::uint64_t least{UINT64_MAX};
            for(unsigned bits{available_}; bits; bits &= bits-1u)
            {
                auto const bit{static_cast<unsigned>(::std::countr_zero(bits))};
                if(counts[bit]<least) { preferred=0; least=counts[bit]; }
                if(counts[bit]==least) { preferred |= static_cast<::std::uint16_t>(1u << bit); }
            }
            available_=preferred;
            return true;
        }

        [[nodiscard]] constexpr tag_selection select(::std::uint16_t entropy, ::std::uint8_t& out) const noexcept
        {
            auto const low{byte_entries_[static_cast<::std::uint8_t>(available_)]};
            auto const high{byte_entries_[static_cast<::std::uint8_t>(available_ >> 8u)]};
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
