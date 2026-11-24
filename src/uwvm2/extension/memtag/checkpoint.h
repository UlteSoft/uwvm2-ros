// Complete allocation-tag images are portable DATA, never hardware addresses.
#pragma once
#ifndef UWVM_MODULE
#include <span>
#include <cstddef>
#include <cstdint>
#include "storage.h"
#include "tag_image.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    // Both passes run while the owner is stopped. No native node, link or
    // allocator metadata enters the portable image.
    [[nodiscard]] inline ::std::size_t checkpoint_extent_count(managed_allocations const& directory) noexcept
    {
        ::std::size_t count{};
        directory.visit(0u,UINT64_MAX,[&](allocation_extent const&) noexcept { ++count; });
        return count;
    }

    [[nodiscard]] inline bool copy_checkpoint_extents(managed_allocations const& directory, ::std::span<::std::byte> rows) noexcept
    {
        bool complete{true};
        directory.visit(0u,UINT64_MAX,[&](allocation_extent const& item) noexcept
        {
            if(rows.size() < checkpoint_extent_bytes) { complete=false; return; }
            encode_checkpoint_extent(rows.first<checkpoint_extent_bytes>(),item);
            rows=rows.subspan(checkpoint_extent_bytes);
        });
        return complete && rows.empty();
    }

    // The unpublished owner must already have its final software mode. No guest
    // or host access can run while restoring; hardware activation occurs later.
    [[nodiscard]] inline bool restore_checkpoint_tag_image(shadow_storage& storage,
        ::std::uint64_t memory_bytes, checkpoint_tag_image image) noexcept
    {
        auto const narrow{storage.width()==address_width::i32};
        if(!storage.enabled() || storage.hardware_active() || !storage.allocation_directory().empty() || memory_bytes>storage.prepared_memory_bytes() ||
           !image.valid(narrow?32u:64u,narrow?4u:8u,memory_bytes,storage.hardware_selected())) { return false; }
        managed_allocations staged;
        if(!image.visit_allocations([&](allocation_extent const& item) noexcept { return staged.add(item); }))
        { return false; } // All directory allocations precede the first shadow write.
        auto const tags{image.tags};
        auto apply=[&]<typename Address, bool Shared>() noexcept
        {
            auto view{storage.view<Address,Shared>(memory_bytes)};
            if(!view.bounds(Address{},0u,memory_bytes)) { return false; }
            for(::std::size_t first{};first<tags.size();)
            {
                auto end{first+1u};while(end<tags.size() && tags[end]==tags[first]) { ++end; }
                auto const offset{static_cast<::std::uint64_t>(first)*16u};
                auto const next{end==tags.size()?memory_bytes:static_cast<::std::uint64_t>(end)*16u};
                auto const pointer{view.insert(static_cast<Address>(offset),::std::to_integer<unsigned>(tags[first]))};
                if(!view.store(pointer,next-offset)) { return false; }
                first=end;
            }
            return true;
        };
        bool const applied{narrow ?
            (storage.shared()?apply.template operator()<::std::uint32_t,true>():apply.template operator()<::std::uint32_t,false>()) :
            (storage.shared()?apply.template operator()<::std::uint64_t,true>():apply.template operator()<::std::uint64_t,false>())};
        if(applied) { storage.allocation_directory().take(staged); }
        return applied;
    }

    [[nodiscard]] inline bool restore_tag_image(shadow_storage& storage,
        ::std::uint64_t memory_bytes, ::std::span<::std::byte const> tags) noexcept
    {
        return restore_checkpoint_tag_image(storage,memory_bytes,{tags,{}});
    }

    // The source initializer installed all permanent descriptors on their
    // canonical owners, including imports. Only their geometry is immutable:
    // a fresh instance of the same source can have different random colors.
    [[nodiscard]] inline bool matches_checkpoint_permanent_extents(
        shadow_storage const& source, checkpoint_tag_image image) noexcept
    {
        auto& directory{source.allocation_directory()};
        ::std::size_t remaining{};
        directory.visit(0u,UINT64_MAX,[&](allocation_extent const& item) noexcept { remaining += item.permanent; });
        return image.visit_allocations([&](allocation_extent const& item) noexcept
        {
            if(!item.permanent) { return true; }
            auto const* actual{directory.exact(item.first,item.last)};
            if(!remaining || !actual || !actual->permanent) { return false; }
            --remaining;
            return true;
        }) && !remaining;
    }
}
