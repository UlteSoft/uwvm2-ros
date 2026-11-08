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
    // The unpublished owner must already have its final software mode. No guest
    // or host access can run while restoring; hardware activation occurs later.
    [[nodiscard]] inline bool restore_tag_image(shadow_storage& storage,
        ::std::uint64_t memory_bytes, ::std::span<::std::byte const> tags) noexcept
    {
        if(!storage.enabled() || storage.hardware_active() || memory_bytes>storage.prepared_memory_bytes() ||
           !valid_tag_image(storage.width()==address_width::i32?32u:64u,
               storage.width()==address_width::i32?4u:8u,memory_bytes,tags)) { return false; }
        auto apply=[&]<typename Address, bool Shared>() noexcept
        {
            auto view{storage.view<Address,Shared>(memory_bytes)};
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
        return storage.width()==address_width::i32 ?
            (storage.shared()?apply.template operator()<::std::uint32_t,true>():apply.template operator()<::std::uint32_t,false>()) :
            (storage.shared()?apply.template operator()<::std::uint64_t,true>():apply.template operator()<::std::uint64_t,false>());
    }
}
