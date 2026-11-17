/** @file
 * @brief Transfer ROSv2 frames under complete guest-memory access proofs.
 * @details Prefix and payload are VM-owned synchronous borrows. The complete
 * destination is checked before either part is written, including all memory
 * tags, and the backend remains pinned through both byte-atomic copies. No
 * pointer escapes, scratch payload is created, or authority is cached. This
 * helper grants no access: only the authenticated exchange owner may call it.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/runtime/memtag/host_access.h>
#endif
namespace uwvm2::runtime::lib::exchange_memory_details
{
    /** @brief Copy an admitted input to host storage and overwrite its guest extent.
     * @details The complete range and every tag are checked before the first
     * read or write. The backend stays pinned through separate byte-atomic
     * copy and zero passes. This is not a whole-message snapshot:
     * concurrent guest stores remain untrusted, as with separate copy/erase.
     * Destination is synchronous, VM-owned storage disjoint from guest memory.
     * The trusted deployment may explicitly set clear_after_copy=false; that
     * performs the same checked copy but leaves source bytes resident. This
     * weakens data-remanence protection only and supplies no extra authority.
     * Refusal changes neither buffer. On success with default policy every
     * source byte has been overwritten before return. No raw pointer escapes, authority
     * is acquired, allocation occurs or guest/provider callback executes.
     */
    template<typename Memory>
    [[nodiscard]] inline bool read_input(Memory const& memory,::std::uint64_t offset,
        void* destination,::std::size_t size,bool clear_after_copy=true) noexcept
    {
        if(size&&!destination)return false;
        return static_cast<bool>(::uwvm2::runtime::memtag::with_host_access(memory,offset,size,
            [destination,clear_after_copy](::std::span<::std::byte> source) noexcept
            {
                auto* output=static_cast<::std::byte*>(destination);
                for(::std::size_t i{};i!=source.size();++i)
                    output[i]=::std::atomic_ref<::std::byte>{source[i]}.load(::std::memory_order_relaxed);
                // Interleaving each zero with the next input load creates a
                // slow mixed load/store loop on the qualified x86 host. Keep
                // the same pin and complete proof, but finish copying before
                // the independent erase pass. Do not widen shared byte access.
                if(clear_after_copy)for(::std::size_t i{};i!=source.size();++i)
                    ::std::atomic_ref<::std::byte>{source[i]}.store(::std::byte{},::std::memory_order_relaxed);
            }));
    }
    /** @brief Copy two native parts into one checked shared-memory frame.
     * @details Invalid host buffers, size overflow, destination bounds or tags
     * reject before any write. A zero-sized part need not have storage. Each
     * shared byte uses a relaxed atomic store; this is not whole-frame atomic
     * publication. The existing router owns delivery, length and erasure policy.
     */
    template<typename Memory>
    [[nodiscard]] inline bool write_frame(Memory const& memory,::std::uint64_t offset,
        void const* prefix,::std::size_t prefix_size,void const* payload,::std::size_t payload_size) noexcept
    {
        if((prefix_size&&!prefix)||(payload_size&&!payload)||
            payload_size>(::std::numeric_limits<::std::size_t>::max)()-prefix_size)return false;
        return static_cast<bool>(::uwvm2::runtime::memtag::with_host_access(memory,offset,prefix_size+payload_size,
            [prefix,prefix_size,payload](::std::span<::std::byte> destination) noexcept
            {
                auto const* header=static_cast<::std::byte const*>(prefix);
                auto const* body=static_cast<::std::byte const*>(payload);
                // The entire destination has already passed bounds and tag
                // validation. Keep both proved extents in local spans: byte
                // stores may alias captured scalar references in the C++
                // abstract machine, otherwise forcing reloads on every byte.
                // No restrict assumption or wider non-atomic store is needed.
                auto const header_destination=destination.first(prefix_size);
                auto const body_destination=destination.subspan(prefix_size);
                for(::std::size_t i{};i!=header_destination.size();++i)
                    ::std::atomic_ref<::std::byte>{header_destination[i]}.store(header[i],::std::memory_order_relaxed);
                for(::std::size_t i{};i!=body_destination.size();++i)
                    ::std::atomic_ref<::std::byte>{body_destination[i]}.store(body[i],::std::memory_order_relaxed);
            }));
    }
}
