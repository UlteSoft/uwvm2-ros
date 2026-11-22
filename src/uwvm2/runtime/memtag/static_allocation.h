// Copyright (c) UlteSoft. Licensed under the APL-2.0 License. See LICENSE.
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <cstddef>
# include <cstdint>
# include <span>
# include <uwvm2/extension/memtag/allocation_neighborhood.h>
# include <uwvm2/extension/memtag/storage.h>
# include "random.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    /// @brief Track permanent colors while preparing one unpublished owner.
    /// @details Cache the globally least-used set. A successful reservation
    /// removes its class from this set. Rescan only when it becomes empty or
    /// every globally least-used color is excluded by a hard local constraint.
    template<::std::size_t Classes = 256>
    class static_color_usage
    {
        static_assert(Classes == 16 || Classes == 256);
        using palette_type = ::uwvm2::extension::memtag::allocation_neighborhood;
        using width_type = ::uwvm2::extension::memtag::address_width;
        palette_type palette_{width_type::i32};
        palette_type least_{palette_};
        ::std::array<::std::uint64_t,Classes> counts_{};

    public:
        constexpr static_color_usage() noexcept = default;
        constexpr static_color_usage(width_type width, bool physical) noexcept requires(Classes == 256)
            : palette_{width, physical} {}

        [[nodiscard]] constexpr palette_type legal_colors() const noexcept { return palette_; }
        [[nodiscard]] constexpr auto const& counts() const noexcept { return counts_; }

        constexpr void record(::std::uint8_t tag) noexcept
        {
            ++counts_[palette_.tag_class(tag)];
            least_.forbid(tag);
        }

        [[nodiscard]] constexpr bool prefer_least_used(palette_type& legal) noexcept
        {
            if (least_.empty())
            {
                least_ = palette_;
                if (!least_.prefer_least_used(counts_)) { return false; }
            }
            return legal.prefer(least_) || legal.prefer_least_used(counts_);
        }
    };

    /// @brief Reserve the color of a validated static descriptor in its backing owner's directory.
    /// @details Called with exclusive ownership of an unpublished graph or private transition image.
    /// Counts cover permanent extents on this owner only and are shared by its imported aliases.
    /// Read both the shadow and the directory, since reservations need not have reached the shadow.
    /// On failure the directory and output are unchanged. Entropy goes through the replay observer.
    template<::std::size_t Classes, typename ReadTag, typename Entropy>
    [[nodiscard]] inline bool reserve_static_extent(
        ::uwvm2::extension::memtag::managed_allocations& directory,
        ::std::uint64_t count, ::std::uint64_t first, ::std::uint64_t last,
        ReadTag read, Entropy entropy, static_color_usage<Classes>& static_colors, ::std::uint8_t& out) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        if (first > last || last >= count) { return false; }
        auto colors{static_colors.legal_colors()};
        if (!mt::visit_allocation_neighbors(first, last, count, [&](::std::uint64_t index) noexcept
        {
            ::std::uint8_t tag{};
            if (!read(index, tag)) { return false; }
            colors.forbid(tag);
            return true;
        })) { return false; }
        constexpr ::std::uint64_t reach{1u + mt::allocation_gap_bytes / mt::granule_size};
        auto const lo{first >= reach ? first - reach : 0u};
        auto const hi{count - 1u - last >= reach ? last + reach : count - 1u};
        bool overlap{};
        directory.visit(lo, hi, [&](auto const& allocation) noexcept
        {
            if (first <= allocation.last && allocation.first <= last) { overlap = true; }
            colors.forbid(allocation.tag);
        });
        if (overlap || !static_colors.prefer_least_used(colors)) { return false; }
        for (unsigned attempt{}; attempt != 8u; ++attempt)
        {
            ::std::uint8_t high{}, low{}, tag{};
            if (!entropy(high) || !entropy(low)) { return false; }
            auto const word{static_cast<::std::uint16_t>((unsigned(high) << 8u) | unsigned(low))};
            auto const result{colors.select(word, tag)};
            if (result == mt::tag_selection::retry_entropy) { continue; }
            if (result != mt::tag_selection::selected || !directory.add_permanent(first, last, tag)) { return false; }
            static_colors.record(tag);
            out = tag;
            return true;
        }
        return false;
    }

    /// @brief Bind staged reset records to a complete, validated memory32 migration image.
    /// @details Only immutable global descriptors have a representation in this format.
    /// Check the entire image first so a rejected row cannot partially recolor the directory.
    [[nodiscard]] inline bool bind_migrated_static_image(
        ::uwvm2::extension::memtag::managed_allocations& directory,
        ::std::span<unsigned char const> image) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        bool valid{true};
        directory.visit(0, UINT64_MAX, [&](auto const& item) noexcept
        {
            if (!valid) { return; }
            if (!item.permanent || !item.active || item.last >= image.size()) { valid=false; return; }
            auto const tag{image[item.first]};
            if (!tag || tag > 15u) { valid=false; return; }
            for (auto i{item.first}; i <= item.last; ++i)
            { if (image[i] != tag) { valid=false; return; } }
            valid=mt::visit_allocation_neighbors(item.first,item.last,image.size(),
                [&](::std::uint64_t i) noexcept { return image[i]!=tag; });
        });
        if (!valid) { return false; }
        directory.visit(0, UINT64_MAX, [&](auto const& item) noexcept
        { directory.exact(item.first,item.last)->tag=image[item.first]; });
        return true;
    }

    /// @brief Dispatch an initializer operation to the shadow matching this memory's actual geometry.
    template<typename Action>
    [[nodiscard]] inline bool with_static_view(::uwvm2::extension::memtag::shadow_storage const& shadow,
                                               Action action) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        if (!shadow.enabled()) { return false; }
        auto const bytes{shadow.prepared_memory_bytes()};
        if (shadow.width() == mt::address_width::i32)
        {
            return shadow.shared() ? action(shadow.view<::std::uint32_t,true>(bytes)) :
                                     action(shadow.view<::std::uint32_t,false>(bytes));
        }
        return shadow.shared() ? action(shadow.view<::std::uint64_t,true>(bytes)) :
                                 action(shadow.view<::std::uint64_t,false>(bytes));
    }

    /// @brief Reserve an entire granule extent before publishing any guest-visible tag.
    [[nodiscard]] inline bool stage_static_allocation(::uwvm2::extension::memtag::shadow_storage const& shadow,
        ::std::uint64_t address, ::std::uint64_t bytes, static_color_usage<>& static_colors, ::std::uint8_t& tag) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        if (!bytes || address >= shadow.prepared_memory_bytes() || bytes > shadow.prepared_memory_bytes() - address) { return false; }
        return with_static_view(shadow, [&](auto view) noexcept
        {
            using address_type = decltype(view.insert(0, 0));
            if (!view.check(static_cast<address_type>(address), 0, bytes)) { return false; }
            return reserve_static_extent(shadow.allocation_directory(),
                shadow.tag_count(), address / mt::granule_size, (address + bytes - 1u) / mt::granule_size,
                [&](::std::uint64_t index, ::std::uint8_t& value) noexcept
                {
                    auto const read{view.load(static_cast<address_type>(index * mt::granule_size))};
                    if (!read) { return false; }
                    value = read.allocation_tag;
                    return true;
                }, next_random_tag, static_colors, tag);
        });
    }

    /// @brief Publish (or roll back to zero) after validation, before any guest can run.
    [[nodiscard]] inline bool write_static_allocation(::uwvm2::extension::memtag::shadow_storage const& shadow,
        ::std::uint64_t address, ::std::uint64_t bytes, ::std::uint8_t tag) noexcept
    {
        return with_static_view(shadow, [&](auto view) noexcept
        {
            using address_type = decltype(view.insert(0, 0));
            return static_cast<bool>(view.store(view.insert(static_cast<address_type>(address), tag), bytes));
        });
    }
}
