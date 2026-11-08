/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.   *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <concepts>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <limits>
# include <type_traits>
# include <uwvm2/utils/macro/push_macros.h>
# include "hardware_mte.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    enum class access_error : unsigned
    {
        none,
        memory_not_representable,
        out_of_bounds,
        shadow_out_of_bounds,
        tag_mismatch
    };

    struct access_result
    {
        access_error error{};
        ::std::uint64_t address{};
        ::std::uint64_t mismatch_granule{};
        ::std::uint8_t pointer_tag{};
        ::std::uint8_t allocation_tag{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return error == access_error::none; }
    };

    // A borrowed view, never a process-global tag map. The owner must keep both
    // the linear-memory snapshot and its shadow alive for the whole operation.
    // Imported aliases borrow the same owner; independent memories never share
    // a shadow. Growth publishes initialized shadow before the enlarged length.
    //
    // Shared=true uses atomic byte reads/writes to avoid a native data race on
    // allocation tags. A software precheck and the later data access are not
    // atomic with respect to concurrent retagging: this differs from a hardware
    // check coupled to the physical access. Allocator retirement/reuse requires
    // separate synchronization. An observed mismatch ALWAYS rejects the access.
    template<::std::unsigned_integral Address, bool Shared = false>
    class shadow_view
    {
        static_assert(::std::numeric_limits<Address>::digits == 32 || ::std::numeric_limits<Address>::digits == 64);
    public:
        static constexpr unsigned TagBits{::std::numeric_limits<Address>::digits == 32 ? 4u : 8u};
        using tag_type = ::std::conditional_t<Shared, ::std::atomic<::std::uint8_t>, ::std::uint8_t>;
        static constexpr unsigned start_bit{::std::numeric_limits<Address>::digits == 32 ? 28u : 56u};
        static constexpr Address address_mask{(Address{1u} << start_bit) - Address{1u}};
        static constexpr Address tag_mask{((Address{1u} << TagBits) - Address{1u}) << start_bit};
        static constexpr ::std::uint64_t address_capacity{::std::uint64_t{1u} << start_bit};
        static constexpr ::std::uint8_t maximum_tag{static_cast<::std::uint8_t>((1u << TagBits) - 1u)};

        constexpr shadow_view(tag_type* tags, ::std::size_t tag_count, ::std::uint64_t memory_bytes,
                              ::std::byte* hardware_base = nullptr) noexcept
            : tags_{tags}, count_{tag_count}, memory_bytes_{memory_bytes}
#if defined(__linux__) && defined(__aarch64__)
            , hardware_base_{hardware_base}
#endif
        { static_cast<void>(hardware_base); }

        // Only after a successful bounds/tag proof, while the owner is pinned.
        [[nodiscard]] ::std::byte* native_pointer(::std::byte* base, access_result const& proof) const noexcept
        {
            auto* const pointer{base + static_cast<::std::size_t>(proof.address)};
#if defined(__linux__) && defined(__aarch64__)
            if(hardware_base_ != nullptr) { return hardware_mte::tagged_pointer(pointer, proof.pointer_tag); }
#endif
            return pointer;
        }

        [[nodiscard]] static constexpr Address untag(Address pointer) noexcept { return pointer & address_mask; }
        [[nodiscard]] static constexpr ::std::uint8_t extract(Address pointer) noexcept
        { return static_cast<::std::uint8_t>((pointer & tag_mask) >> start_bit); }
        [[nodiscard]] static constexpr Address insert(Address pointer, Address tag) noexcept
        { return static_cast<Address>(untag(pointer) | ((tag << start_bit) & tag_mask)); }

        // No pointer arithmetic or memory effect precedes the complete range
        // proof. Offset and length retain all 64 bits, including on ISA32.
        [[nodiscard]] constexpr access_result bounds(Address pointer, ::std::uint64_t offset,
                                                      ::std::uint64_t bytes) const noexcept
        {
            access_result result{.pointer_tag = extract(pointer)};
            if(memory_bytes_ > address_capacity)
            { result.error = access_error::memory_not_representable; return result; }
            auto const base{static_cast<::std::uint64_t>(untag(pointer))};
            if(offset > address_capacity - base)
            { result.error = access_error::out_of_bounds; return result; }
            result.address = base + offset;
            if(result.address > memory_bytes_ || bytes > memory_bytes_ - result.address)
            { result.error = access_error::out_of_bounds; return result; }
            if(bytes != 0u)
            {
                auto const last{(result.address + bytes - 1u) >> 4u};
                if(tags_ == nullptr || last >= count_)
                { result.error = access_error::shadow_out_of_bounds; }
            }
            return result;
        }

        [[nodiscard]] access_result check(Address pointer, ::std::uint64_t offset,
                                           ::std::uint64_t bytes) const noexcept
        {
            auto result{bounds(pointer, offset, bytes)};
            if(!result || bytes == 0u) { return result; }
            auto index{static_cast<::std::size_t>(result.address >> 4u)};
            auto const end{static_cast<::std::size_t>(((result.address + bytes - 1u) >> 4u) + 1u)};
            if constexpr(!Shared)
            {
                // Keep block-loop setup out of constant-width scalar accesses.
                // Small/unaligned ranges still take the complete byte-tag loop
                // below. This threshold changes no checked range or result.
                if(bytes >= sizeof(::std::uint64_t) * 16u)
                {
                    // Eight granules per comparison; memcpy avoids alignment
                    // and strict-aliasing UB on all five hosts. Resolve a failed
                    // block below to its exact first mismatching granule.
                    auto const repeated{::std::uint64_t{0x0101010101010101u} * result.pointer_tag};
                    while(end - index >= sizeof(::std::uint64_t))
                    {
                        ::std::uint64_t word{};
                        ::std::memcpy(&word, tags_ + index, sizeof word);
                        if(word != repeated) { break; }
                        index += sizeof word;
                    }
                }
            }
            for(; index != end; ++index)
            {
                auto const actual{read(index)};
                if(actual != result.pointer_tag)
                {
                    result.error = access_error::tag_mismatch;
                    result.mismatch_granule = static_cast<::std::uint64_t>(index) << 4u;
                    result.allocation_tag = actual;
                    return result;
                }
            }
            return result;
        }

        // Tag stores intentionally change the allocation tag; they require the
        // complete memory/shadow bounds proof, not a match with the old tag.
        // No partial update is permitted when any part of the range is invalid.
        [[nodiscard]] access_result store(Address pointer, ::std::uint64_t bytes) const noexcept
        {
            auto result{bounds(pointer, 0u, bytes)};
            if(!result || bytes == 0u) { return result; }
            auto const first{static_cast<::std::size_t>(result.address >> 4u)};
            auto const end{static_cast<::std::size_t>(((result.address + bytes - 1u) >> 4u) + 1u)};
#if defined(__linux__) && defined(__aarch64__)
            if(hardware_base_ != nullptr)
            {
                for(auto index{first}; index != end; ++index)
                { hardware_mte::store_tag(hardware_base_ + index * 16u, result.pointer_tag); }
                hardware_mte::finish_tag_writes();
            }
#endif
            if constexpr(Shared)
            {
                for(auto index{first}; index != end; ++index)
                { tags_[index].store(result.pointer_tag, ::std::memory_order_relaxed); }
            }
            else { ::std::memset(tags_ + first, result.pointer_tag, end - first); }
            return result;
        }

        [[nodiscard]] access_result load(Address pointer) const noexcept
        {
            auto result{bounds(pointer, 0u, 1u)};
            if(result) { result.allocation_tag = read(static_cast<::std::size_t>(result.address >> 4u)); }
            return result;
        }

    private:
        [[nodiscard]] ::std::uint8_t read(::std::size_t index) const noexcept
        {
            if constexpr(Shared) { return tags_[index].load(::std::memory_order_relaxed); }
            else { return tags_[index]; }
        }
        tag_type* tags_{};
        ::std::size_t count_{};
        ::std::uint64_t memory_bytes_{};
#if defined(__linux__) && defined(__aarch64__)
        ::std::byte* hardware_base_{};
#endif
    };
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
