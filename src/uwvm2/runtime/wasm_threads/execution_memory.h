#pragma once
#ifndef UWVM_MODULE
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <span>
#include <uwvm2/extension/memtag/storage.h>
#endif
namespace uwvm2::runtime::wasm_threads
{
    // Native owner only. Caller holds a complete instruction/memory phase,
    // an actual all-stop, or the pre-publication initializer lease.
    template<typename Memory>
    [[nodiscard]] inline ::std::size_t threaded_memory_length(Memory const& memory) noexcept
    {
        if constexpr(requires{memory.memory_length_p;})
            return memory.memory_length_p?memory.memory_length_p->load(::std::memory_order_acquire):0u;
        else return memory.memory_length;
    }
    template<typename Memory,typename Transfer>
    [[nodiscard]] inline bool visit_threaded_memory(Memory const& memory,Transfer transfer) noexcept
    {
        namespace mt=::uwvm2::extension::memtag;
        auto const length=threaded_memory_length(memory);
        if(!length)return true;
        if(!memory.memory_begin)return false;
        auto const& tags=memory.memtag_shadow;
        if(!tags.hardware_active())return transfer(memory.memory_begin,0u,length);
        if(!tags.enabled()||tags.hardware_base()!=memory.memory_begin||length>tags.prepared_memory_bytes())return false;
        auto visit=[&]<typename Address,bool Shared>() noexcept
        {
            auto view=tags.template view<Address,Shared>(length);
            for(::std::size_t offset{};offset<length;)
            {
                auto tag=view.load(static_cast<Address>(offset));if(!tag)return false;
                auto end=offset+(::std::min)(::std::size_t{16},length-offset);
                while(end<length)
                {
                    auto next=view.load(static_cast<Address>(end));if(!next)return false;
                    if(next.allocation_tag!=tag.allocation_tag)break;
                    end+=(::std::min)(::std::size_t{16},length-end);
                }
                if(!transfer(mt::hardware_mte::tagged_pointer(memory.memory_begin+offset,tag.allocation_tag),offset,end-offset))return false;
                offset=end;
            }
            return true;
        };
        return tags.width()==mt::address_width::i64?
            (tags.shared()?visit.template operator()<::std::uint64_t,true>():visit.template operator()<::std::uint64_t,false>()):
            (tags.shared()?visit.template operator()<::std::uint32_t,true>():visit.template operator()<::std::uint32_t,false>());
    }
    template<typename Memory>
    [[nodiscard]] inline bool copy_threaded_memory(Memory const& memory,::std::span<::std::byte> image,bool write) noexcept
    {
        if(image.size()!=threaded_memory_length(memory))return false;
        return visit_threaded_memory(memory,[&](::std::byte* physical,::std::size_t offset,::std::size_t length) noexcept
        {
            for(::std::size_t i{};i<length;++i)
            {
                ::std::atomic_ref<::std::byte> cell{physical[i]};
                if(write)cell.store(image[offset+i],::std::memory_order_relaxed);
                else image[offset+i]=cell.load(::std::memory_order_relaxed);
            }
            return true;
        });
    }
}
