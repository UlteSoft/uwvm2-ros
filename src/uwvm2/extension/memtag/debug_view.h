// Read allocation tags only while the actual native memory owner is pinned.
#pragma once
#ifndef UWVM_MODULE
#include <span>
#include <cstring>
#include <algorithm>
#include "debug_types.h"
#include "storage.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    [[nodiscard]] inline debug_tag_view copy_debug_tags(shadow_storage const& storage,
        ::std::uint64_t memory_bytes, ::std::uint64_t offset, ::std::uint64_t bytes,
        ::std::span<::std::uint8_t> destination) noexcept
    {
        debug_tag_view result{.status=debug_tag_status::invalid_range};
        // The snapshot's actual current length is authoritative. Shadow capacity
        // may be larger after a failed native grow; it never grants extra bytes.
        if(offset > memory_bytes || bytes > memory_bytes-offset) { return result; }
        result.memory_bytes=memory_bytes; result.first_granule=offset/16u;
        if(!storage.enabled()) { result.status=debug_tag_status::disabled; return result; }
        auto const width{storage.width()};
        auto const capacity{::std::uint64_t{1u} << (width==address_width::i32 ? 28u : 56u)};
        if(memory_bytes>capacity || memory_bytes>storage.prepared_memory_bytes()) { return result; }
        auto const count{bytes==0u ? 0u : (offset+bytes-1u)/16u-result.first_granule+1u};
        if(count>destination.size() || (count!=0u && destination.data()==nullptr)) { return result; }
        auto read=[&]<typename Address, bool Shared>() noexcept
        {
            auto const view{storage.view<Address,Shared>(memory_bytes)};
            for(::std::size_t index{};index!=static_cast<::std::size_t>(count);++index)
            {
                // The complete actual byte range above proves these granule
                // starts are representable and before the current memory end.
                auto const tag{view.load(static_cast<Address>((result.first_granule+index)*16u))};
                if(!tag) { return false; }
                destination[index]=tag.allocation_tag;
            }
            return true;
        };
        bool const copied{width==address_width::i32 ?
            (storage.shared() ? read.template operator()<::std::uint32_t,true>() : read.template operator()<::std::uint32_t,false>()) :
            (storage.shared() ? read.template operator()<::std::uint64_t,true>() : read.template operator()<::std::uint64_t,false>())};
        if(!copied) { return result; }
        result.status=debug_tag_status::available;result.count=static_cast<::std::size_t>(count);
        result.tag_bits=width==address_width::i32 ? 4u : 8u;
        return result;
    }
    // A debugger holds a real owner/snapshot and complete stop. It may inspect
    // mixed allocation tags without presenting a guest pointer tag. This is
    // deliberately separate from normal checked guest access.
    [[nodiscard]] inline bool copy_debug_memory(shadow_storage const& storage,
        ::std::span<::std::byte> memory, ::std::uint64_t offset,
        void* buffer, ::std::size_t bytes, bool write = false) noexcept
    {
        if(offset > memory.size() || bytes > memory.size()-offset ||
           (bytes != 0u && (memory.data() == nullptr || buffer == nullptr))) { return false; }
        if(bytes == 0u) { return true; }
        auto const start{static_cast<::std::size_t>(offset)};
        auto* const detached{static_cast<::std::byte*>(buffer)};
        auto copy=[&](::std::byte* physical, ::std::size_t copied, ::std::size_t count) noexcept
        {
            if(write) { ::std::memcpy(physical,detached+copied,count); }
            else { ::std::memcpy(detached+copied,physical,count); }
        };
        if(!storage.hardware_active())
        {
            copy(memory.data()+start,0u,bytes);
            return true;
        }
        // Debugger/collector workers can be distinct from Guest execution threads.
        if(!hardware_mte::enable_current_thread_sync()) { return false; }
        // Prove the complete extent before the first write. Spare prepared
        // storage is not permission to inspect beyond this pinned snapshot.
        if(!storage.enabled() || storage.hardware_base()!=memory.data() ||
           memory.size()>storage.prepared_memory_bytes()) { return false; }
        auto const width{storage.width()};
        auto const capacity{::std::uint64_t{1u} << (width==address_width::i32 ? 28u : 56u)};
        if(memory.size()>capacity) { return false; }
        auto visit=[&]<typename Address, bool Shared>() noexcept
        {
            auto const view{storage.view<Address,Shared>(memory.size())};
            // Validate all tag locations before mutation; the owner pin and
            // complete stopped cohort keep this shadow stable through copying.
            for(::std::size_t done{}; done!=bytes;)
            {
                auto const address{start+done};
                auto const count{(::std::min)(bytes-done,16u-address%16u)};
                if(!view.load(static_cast<Address>(address))) { return false; }
                done+=count;
            }
            for(::std::size_t done{}; done!=bytes;)
            {
                auto const address{start+done};
                auto const count{(::std::min)(bytes-done,16u-address%16u)};
                auto const tag{view.load(static_cast<Address>(address))};
                copy(hardware_mte::tagged_pointer(memory.data()+address,tag.allocation_tag),done,count);
                done+=count;
            }
            return true;
        };
        return width==address_width::i32 ?
            (storage.shared() ? visit.template operator()<::std::uint32_t,true>() : visit.template operator()<::std::uint32_t,false>()) :
            (storage.shared() ? visit.template operator()<::std::uint64_t,true>() : visit.template operator()<::std::uint64_t,false>());
    }

    // Source locations can contain tagged guest pointers. Raw diagnostic
    // addresses are retained by the caller; only this host copy uses canonical
    // offsets. Disabled MTE retains the exact ordinary Wasm address semantics.
    [[nodiscard]] inline bool debug_canonical_address(shadow_storage const& storage,
        ::std::uint64_t raw, ::std::uint64_t& canonical) noexcept
    {
        canonical=raw;
        if(!storage.enabled()) { return true; }
        if(storage.width()==address_width::i32)
        {
            if(raw>UINT32_MAX) { return false; }
            canonical=raw&UINT64_C(0x0fffffff);
        }
        else { canonical=raw&UINT64_C(0x00ffffffffffffff); }
        return true;
    }

}
