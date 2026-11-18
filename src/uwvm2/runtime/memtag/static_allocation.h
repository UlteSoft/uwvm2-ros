// Copyright (c) UlteSoft. Licensed under the APL-2.0 License. See LICENSE.
#pragma once
#ifndef UWVM_MODULE
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
    /// @brief Reserve the color of a validated static descriptor in its backing owner's directory.
    /// @details Called with exclusive ownership of an unpublished graph or private transition image.
    /// Read both the shadow and the directory, since reservations need not have reached the shadow.
    /// On failure the directory and output are unchanged. Entropy goes through the replay observer.
    template<typename ReadTag, typename Entropy>
    [[nodiscard]] inline bool reserve_static_extent(
        ::uwvm2::extension::memtag::managed_allocations& directory,
        ::uwvm2::extension::memtag::address_width width, bool physical,
        ::std::uint64_t count, ::std::uint64_t first, ::std::uint64_t last,
        ReadTag read, Entropy entropy, ::std::uint8_t& out) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        if (first > last || last >= count) { return false; }
        mt::allocation_neighborhood colors{width, physical};
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
        if (overlap) { return false; }
        for (unsigned attempt{}; attempt != 8u; ++attempt)
        {
            ::std::uint8_t high{}, low{}, tag{};
            if (!entropy(high) || !entropy(low)) { return false; }
            auto const word{static_cast<::std::uint16_t>((unsigned(high) << 8u) | unsigned(low))};
            auto const result{colors.select(word, tag)};
            if (result == mt::tag_selection::retry_entropy) { continue; }
            if (result != mt::tag_selection::selected || !directory.add_permanent(first, last, tag)) { return false; }
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
        ::std::uint64_t address, ::std::uint64_t bytes, ::std::uint8_t& tag) noexcept
    {
        namespace mt = ::uwvm2::extension::memtag;
        if (!bytes || address >= shadow.prepared_memory_bytes() || bytes > shadow.prepared_memory_bytes() - address) { return false; }
        return with_static_view(shadow, [&](auto view) noexcept
        {
            using address_type = decltype(view.insert(0, 0));
            if (!view.check(static_cast<address_type>(address), 0, bytes)) { return false; }
            return reserve_static_extent(shadow.allocation_directory(), shadow.width(), shadow.hardware_selected(),
                shadow.tag_count(), address / mt::granule_size, (address + bytes - 1u) / mt::granule_size,
                [&](::std::uint64_t index, ::std::uint8_t& value) noexcept
                {
                    auto const read{view.load(static_cast<address_type>(index * mt::granule_size))};
                    if (!read) { return false; }
                    value = read.allocation_tag;
                    return true;
                }, next_random_tag, tag);
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
