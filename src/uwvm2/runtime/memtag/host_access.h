// Checked host buffers retain the complete guest pointer until range/tag proof.
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <limits>
# include <span>
# include <type_traits>
# include <utility>
# include "bridge.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    // The callback runs while the actual owner is pinned. No raw host pointer
    // may escape it. The same complete-range proof precedes reads and writes.
    template<typename Memory,typename Function>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result with_host_access(
        Memory const& memory,::std::uint64_t raw_pointer,::std::uint64_t bytes,Function&& function) noexcept
    {
        namespace mt=::uwvm2::extension::memtag;
        static_assert(::std::is_nothrow_invocable_r_v<void,Function,::std::span<::std::byte>>);
        if(!memory.memtag_shadow.enabled())
        {
            return details::with_snapshot(memory,[&](::std::span<::std::byte> data) noexcept
            {
                if(raw_pointer>data.size() || bytes>data.size()-raw_pointer)
                { return mt::access_result{.error=mt::access_error::out_of_bounds,.address=raw_pointer}; }
                ::std::forward<Function>(function)(data.subspan(
                    static_cast<::std::size_t>(raw_pointer),static_cast<::std::size_t>(bytes)));
                return mt::access_result{.address=raw_pointer};
            });
        }
        auto dispatch{[&]<typename Address>() noexcept
        {
            if(raw_pointer>(::std::numeric_limits<Address>::max)())
            { return mt::access_result{.error=mt::access_error::out_of_bounds,.address=raw_pointer}; }
            if(memory.memtag_shadow.shared())
            { return with_checked_access<Address,true>(memory,static_cast<Address>(raw_pointer),0u,bytes,::std::forward<Function>(function)); }
            return with_checked_access<Address,false>(memory,static_cast<Address>(raw_pointer),0u,bytes,::std::forward<Function>(function));
        }};
        if(memory.memtag_shadow.width()==mt::address_width::i32)
        { return dispatch.template operator()<::std::uint32_t>(); }
        if(memory.memtag_shadow.width()==mt::address_width::i64)
        { return dispatch.template operator()<::std::uint64_t>(); }
        return {.error=mt::access_error::memory_not_representable};
    }

    template<typename Memory>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result check_host_access(
        Memory const& memory,::std::uint64_t raw_pointer,::std::uint64_t bytes) noexcept
    { return with_host_access(memory,raw_pointer,bytes,[](::std::span<::std::byte>) noexcept {}); }

    // Shared guest bytes use atomic byte transport, like shared interpreter
    // accesses. This is not a promise of a multi-byte atomic snapshot.
    template<bool Write,typename Memory>
    [[nodiscard]] inline ::uwvm2::extension::memtag::access_result copy_host_access(
        Memory const& memory,::std::uint64_t raw_pointer,void* host,::std::size_t bytes,bool declared_shared=false) noexcept
    {
        if(bytes && host==nullptr)
        { return {.error=::uwvm2::extension::memtag::access_error::out_of_bounds}; }
        return with_host_access(memory,raw_pointer,bytes,[&](::std::span<::std::byte> data) noexcept
        {
            if(data.empty()) { return; }
            if(declared_shared || memory.memtag_shadow.shared())
            {
                auto* native{static_cast<::std::byte*>(host)};
                for(::std::size_t index{};index!=data.size();++index)
                {
                    ::std::atomic_ref<::std::byte> cell{data[index]};
                    if constexpr(Write) { cell.store(native[index],::std::memory_order_relaxed); }
                    else { native[index]=cell.load(::std::memory_order_relaxed); }
                }
            }
            else if constexpr(Write) { ::std::memcpy(data.data(),host,data.size()); }
            else { ::std::memcpy(host,data.data(),data.size()); }
        });
    }
}
