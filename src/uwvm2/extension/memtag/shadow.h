/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.   *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <bit>
# include <concepts>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <limits>
# include <span>
# include <type_traits>
# include <uwvm2/utils/macro/push_macros.h>
# include "hardware_mte.h"
# include "instruction.h"
# include "software_x86.h"
# include <uwvm2/utils/mutex/rw_spin_lock.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    // Bridge calls execute under the accessing module's policy, including imports.
    inline thread_local bool access_checks_enabled{true};
    class scoped_access_checks
    {
        bool previous_;
    public:
        explicit scoped_access_checks(bool enabled) noexcept : previous_{access_checks_enabled} { access_checks_enabled=enabled; }
        scoped_access_checks(scoped_access_checks const&) = delete;
        scoped_access_checks& operator=(scoped_access_checks const&) = delete;
        ~scoped_access_checks() { access_checks_enabled=previous_; }
    };

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
    // Owner-created shared views serialize tag writes with complete checked
    // accesses. A standalone check is still only a snapshot: callers must keep
    // the owner's access lease through the actual byte operation. Bare views
    // constructed over a raw tag array have no owner lifetime or locking proof.
    template<::std::unsigned_integral Address, bool Shared = false>
    class shadow_view
    {
        static_assert(::std::numeric_limits<Address>::digits == 32 || ::std::numeric_limits<Address>::digits == 64);
    public:
        static constexpr unsigned TagBits{4u};
        using tag_type = ::std::conditional_t<Shared, ::std::atomic<::std::uint8_t>, ::std::uint8_t>;
        static constexpr unsigned start_bit{::std::numeric_limits<Address>::digits - TagBits};
        static constexpr Address address_mask{(Address{1u} << start_bit) - Address{1u}};
        static constexpr Address tag_mask{((Address{1u} << TagBits) - Address{1u}) << start_bit};
        static constexpr ::std::uint64_t address_capacity{::std::uint64_t{1u} << start_bit};
        static constexpr ::std::uint8_t maximum_tag{static_cast<::std::uint8_t>((1u << TagBits) - 1u)};

        constexpr shadow_view(tag_type* tags, ::std::size_t tag_count, ::std::uint64_t memory_bytes,
                              ::std::byte* hardware_base = nullptr,
                              ::uwvm2::utils::mutex::rwlock_t* tag_write_gate = nullptr,
                              Address effective_mask = address_mask, unsigned tag_start = start_bit,
                              bool has_tags = true) noexcept
            : tags_{tags}, count_{tag_count}, memory_bytes_{memory_bytes}, tag_write_gate_{tag_write_gate}, effective_mask_{effective_mask}, tag_start_{tag_start}, has_tags_{has_tags}
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

        [[nodiscard]] constexpr Address canonical_address(Address pointer) const noexcept { return pointer & effective_mask_; }
        [[nodiscard]] constexpr unsigned tag_start_bit() const noexcept { return tag_start_; }
        [[nodiscard]] constexpr ::std::uint8_t pointer_tag(Address pointer) const noexcept
        { return static_cast<::std::uint8_t>((pointer >> tag_start_) & maximum_tag); }
        [[nodiscard]] constexpr Address clear_tag(Address pointer) const noexcept
        { return pointer & ~(static_cast<Address>(maximum_tag) << tag_start_); }
        [[nodiscard]] constexpr Address with_tag(Address pointer, Address tag) const noexcept
        { return clear_tag(pointer) | ((tag & maximum_tag) << tag_start_); }
        [[nodiscard]] constexpr runtime_status status() const noexcept
        {
#if defined(__linux__) && defined(__aarch64__)
            if(hardware_base_) { return runtime_status::arm_mte; }
#endif
            return runtime_status::software_full;
        }

        // No pointer arithmetic or memory effect precedes the complete range
        // proof. Offset and length retain all 64 bits, including on ISA32.
        [[nodiscard]] constexpr access_result bounds(Address pointer, ::std::uint64_t offset,
                                                      ::std::uint64_t bytes) const noexcept
        {
            access_result result{.pointer_tag = pointer_tag(pointer)};
            if(memory_bytes_ && memory_bytes_ - 1u > effective_mask_)
            { result.error = access_error::memory_not_representable; return result; }
            auto const base{static_cast<::std::uint64_t>(canonical_address(pointer))};
            if(offset > UINT64_MAX - base)
            { result.error = access_error::out_of_bounds; return result; }
            result.address = base + offset;
            if(result.address > memory_bytes_ || bytes > memory_bytes_ - result.address)
            { result.error = access_error::out_of_bounds; return result; }
            if(has_tags_ && bytes != 0u)
            {
                auto const last{(result.address + bytes - 1u) >> 4u};
                if(tags_ == nullptr || last >= count_)
                { result.error = access_error::shadow_out_of_bounds; }
            }
            return result;
        }

        [[nodiscard]] access_result check(Address pointer, ::std::uint64_t offset,
                                           ::std::uint64_t bytes, ::std::uint8_t comparison_mask=0xffu) const noexcept
        {
            auto result{bounds(pointer, offset, bytes)};
            if(!result || bytes == 0u || !has_tags_ || !access_checks_enabled || comparison_mask==0u) { return result; }
            return check_tags(result,bytes,comparison_mask);
        }

    private:
        [[nodiscard]] static access_result mismatch(access_result result, ::std::size_t index,
                                                     ::std::uint8_t actual) noexcept
        {
            result.error = access_error::tag_mismatch;
            result.mismatch_granule = static_cast<::std::uint64_t>(index) << 4u;
            result.allocation_tag = actual;
            return result;
        }

        // The XOR word is nonzero. Find the earliest differing byte in memory order.
        template<typename Word>
        [[nodiscard]] static unsigned first_different_byte(Word difference) noexcept
        {
            if constexpr(::std::endian::native == ::std::endian::little) { return ::std::countr_zero(difference) / 8u; }
            else if constexpr(::std::endian::native == ::std::endian::big) { return ::std::countl_zero(difference) / 8u; }
            else
            {
                unsigned char bytes[sizeof(Word)];
                ::std::memcpy(bytes,&difference,sizeof bytes);
                unsigned index{};
                while(!bytes[index]) { ++index; }
                return index;
            }
        }

        // First and last words cover between one and two word widths of tags.
        template<typename Word>
        struct pair_difference { Word first, last; };

        template<typename Word>
        [[nodiscard,gnu::always_inline]] static pair_difference<Word> compare_pair(::std::uint8_t const* tags, ::std::size_t count,
                                                                                 ::std::uint8_t expected) noexcept
        {
            Word first{},last{};
            ::std::memcpy(&first,tags,sizeof(Word));
            ::std::memcpy(&last,tags+count-sizeof(Word),sizeof(Word));
            auto const repeated{static_cast<Word>((::std::numeric_limits<Word>::max()/255u)*expected)};
            return {static_cast<Word>(first ^ repeated),static_cast<Word>(last ^ repeated)};
        }
        template<typename Word>
        [[nodiscard,gnu::always_inline]] static ::std::size_t matching_pair(::std::uint8_t const* tags, ::std::size_t count,
                                                                           ::std::uint8_t expected) noexcept
        {
            auto const [first,last]{compare_pair<Word>(tags,count,expected)};
            if((first | last) == 0u) { return count; }
            return first ? first_different_byte(first) : count-sizeof(Word)+first_different_byte(last);
        }

    public:
        // A complete, pinned interval of up to two words of software tags.
        // First and last words may overlap, but never reach outside the interval.
        class short_allocation_tags
        {
            friend class shadow_view;
            ::std::uint8_t* tags_;
            ::std::size_t count_;
            static constexpr unsigned halfword_pair_count{2u * sizeof(::std::uint16_t)};

            short_allocation_tags(::std::uint8_t* tags, ::std::size_t count) noexcept : tags_{tags},count_{count} {}
            template<typename Word>
            [[nodiscard]] bool matches_pair(::std::uint8_t expected) const noexcept
            {
                auto const [first,last]{compare_pair<Word>(tags_,count_,expected)};
                return (first | last) == 0u;
            }
            template<typename Word>
            void store_pair(::std::uint8_t tag) const noexcept
            {
                auto const repeated{static_cast<Word>((::std::numeric_limits<Word>::max()/255u)*tag)};
                ::std::memcpy(tags_,&repeated,sizeof repeated);
                ::std::memcpy(tags_+count_-sizeof repeated,&repeated,sizeof repeated);
            }

        public:
            static constexpr unsigned maximum_count{2u * sizeof(::std::uint32_t)};
            [[nodiscard]] static constexpr bool supports(Address input, Address bytes) noexcept
            { return bytes && bytes <= granule_size*maximum_count && !((input | bytes) & (granule_size-1u)); }
            // Each supplied shadow byte backs one complete committed data granule.
            [[nodiscard]] static short_allocation_tags in_committed_prefix(
                ::std::span<::std::uint8_t> prefix, Address address, Address bytes) noexcept
            {
                if(!supports(address,bytes)) { return {nullptr,0u}; }
                auto const first{address/granule_size}, count{bytes/granule_size};
                if(first+count>prefix.size()) { return {nullptr,0u}; }
                return {prefix.data()+static_cast<::std::size_t>(first),static_cast<::std::size_t>(count)};
            }
            [[nodiscard]] explicit operator bool() const noexcept { return tags_ != nullptr; }
            [[nodiscard]] bool matches(::std::uint8_t expected) const noexcept
            {
                if(count_ == 1u) { return *tags_ == expected; }
                if(count_ <= halfword_pair_count) { return matches_pair<::std::uint16_t>(expected); }
                return matches_pair<::std::uint32_t>(expected);
            }
            void store(::std::uint8_t tag) const noexcept
            {
                if(count_ == 1u) { *tags_=tag; }
                else if(count_ <= halfword_pair_count) { store_pair<::std::uint16_t>(tag); }
                else { store_pair<::std::uint32_t>(tag); }
            }
        };

        [[nodiscard]] short_allocation_tags short_software_allocation(Address input, Address bytes) const noexcept requires (!Shared)
        {
            if(!short_allocation_tags::supports(input,bytes)) { return {nullptr,0u}; }
#if defined(__linux__) && defined(__aarch64__)
            if(hardware_base_) { return {nullptr,0u}; }
#endif
            auto const proof{bounds(input,0u,bytes)};
            if(!proof) { return {nullptr,0u}; }
            return {tags_+static_cast<::std::size_t>(proof.address >> 4u),static_cast<::std::size_t>(bytes >> 4u)};
        }

    private:
        // A 32-tag block uses four consecutive words. For 16..31 tags the
        // same bounded loads cover both ends. Earlier words win in the overlap.
        [[nodiscard,gnu::always_inline]] static ::std::size_t matching_four_words(
            ::std::uint8_t const* tags, ::std::size_t count, ::std::uint64_t repeated) noexcept
        {
            ::std::uint64_t words[4];
            ::std::memcpy(words,tags,16u);
            ::std::memcpy(words+2u,tags+count-16u,16u);
            if(((words[0]^repeated) | (words[1]^repeated) | (words[2]^repeated) | (words[3]^repeated)) != 0u) [[unlikely]]
            {
                for(unsigned i{}; i != 4u; ++i)
                {
                    if(auto difference=words[i]^repeated)
                    {
                        auto const offset{i < 2u ? i*8u : count-(4u-i)*8u};
                        return offset + first_different_byte(difference);
                    }
                }
            }
            return count;
        }

        // In shadow tags (16 guest bytes each), from native allocation and access sweeps.
        static constexpr ::std::size_t allocation_vector_tags{768u}, access_vector_tags{1536u};

        // The caller proved a nonempty range of private, full-byte tags.
        [[nodiscard,gnu::always_inline]] access_result check_allocation_tags(access_result result, ::std::size_t first,
            ::std::size_t count) const noexcept requires (!Shared)
        {
            auto const* tags{tags_+first};
            auto const expected{result.pointer_tag};
            ::std::size_t matched{};
            if(count <= short_allocation_tags::halfword_pair_count)
            {
                matched = count == 1u ? static_cast<::std::size_t>(*tags == expected) :
                                       matching_pair<::std::uint16_t>(tags,count,expected);
                if(matched != count) [[unlikely]] { return mismatch(result,first+matched,tags[matched]); }
                return result;
            }
            if(count >= 16u)
            {
                auto const initial{tags[0]};
                if(initial != expected) [[unlikely]] { return mismatch(result,first,initial); }
                if constexpr(software_x86::has_vector_kernels)
                {
                    if(count >= allocation_vector_tags && software_x86::host_features.avx2)
                    {
                        matched = software_x86::matching_vector_prefix(tags,count,expected);
                        if(matched == count) { return result; }
                        // The prefix ends at a known bad lane or the first
                        // unchecked tail byte. Inspect just that byte to decide.
                        auto const actual{tags[matched]};
                        if(actual != expected) [[unlikely]] { return mismatch(result,first+matched,actual); }
                        ++matched;
                    }
                }
                auto const repeated{UINT64_C(0x0101010101010101) * expected};
                while(count-matched >= 32u)
                {
                    auto const step{matching_four_words(tags+matched,32u,repeated)};
                    matched += step;
                    if(step != 32u) [[unlikely]] { return mismatch(result,first+matched,tags[matched]); }
                }
                if(count-matched >= 16u)
                {
                    matched += matching_four_words(tags+matched,count-matched,repeated);
                    if(matched != count) [[unlikely]] { return mismatch(result,first+matched,tags[matched]); }
                    return result;
                }
            }
            auto const tail{count-matched};
            if(tail == 1u) { matched += tags[matched] == expected; }
            else if(tail >= 8u) { matched += matching_pair<::std::uint64_t>(tags+matched,tail,expected); }
            else if(tail >= 4u) { matched += matching_pair<::std::uint32_t>(tags+matched,tail,expected); }
            else if(tail >= 2u) { matched += matching_pair<::std::uint16_t>(tags+matched,tail,expected); }
            if(matched != count) [[unlikely]] { return mismatch(result,first+matched,tags[matched]); }
            return result;
        }

        // Out-of-line vector dispatch returns only a tag index. Keep the
        // ordinary access bounds proof and error reporting in the caller.
        [[nodiscard,gnu::noinline]] static ::std::size_t matching_long_access(
            ::std::uint8_t const* tags, ::std::size_t count, ::std::uint8_t expected) noexcept
        {
            auto matched{software_x86::matching_vector_prefix(tags,count,expected)};
            while(matched != count && tags[matched] == expected) { ++matched; }
            return matched;
        }

        [[nodiscard,gnu::always_inline]] access_result check_tags(access_result result,::std::uint64_t bytes,
                                                                  ::std::uint8_t comparison_mask) const noexcept
        {
            auto index{static_cast<::std::size_t>(result.address >> 4u)};
            auto const end{static_cast<::std::size_t>(((result.address + bytes - 1u) >> 4u) + 1u)};
            if constexpr(!Shared)
            {
                // Keep block-loop setup out of constant-width scalar accesses.
                if(comparison_mask == 0xffu && bytes >= sizeof(::std::uint64_t) * 16u)
                {
                    auto const expected{result.pointer_tag};
                    if(tags_[index] != expected) [[unlikely]] { return mismatch(result,index,tags_[index]); }
                    if constexpr(software_x86::has_vector_kernels)
                    {
                        if(end-index >= access_vector_tags && software_x86::host_features.avx2)
                        {
                            index += matching_long_access(tags_+index,end-index,expected);
                            if(index != end) { return mismatch(result,index,tags_[index]); }
                            return result;
                        }
                    }
                    // Compare complete blocks in the same access that proved
                    // their bounds. Only the failing block needs finer resolution.
                    auto const repeated{UINT64_C(0x0101010101010101) * expected};
                    while(end-index >= 32u)
                    {
                        ::std::uint64_t words[4];
                        ::std::memcpy(words,tags_+index,sizeof words);
                        if(((words[0]^repeated) | (words[1]^repeated) |
                            (words[2]^repeated) | (words[3]^repeated)) != 0u) { break; }
                        index += 32u;
                    }
                    while(end-index >= sizeof(::std::uint64_t))
                    {
                        ::std::uint64_t word;
                        ::std::memcpy(&word,tags_+index,sizeof word);
                        if(auto difference=word^repeated)
                        {
                            index += first_different_byte(difference);
                            return mismatch(result,index,tags_[index]);
                        }
                        index += sizeof word;
                    }
                }
            }
            for(; index != end; ++index)
            {
                auto const actual{read(index)};
                if((actual&comparison_mask)!=(result.pointer_tag&comparison_mask))
                { return mismatch(result,index,actual); }
            }
            return result;
        }

    public:
        // Tag stores intentionally change the allocation tag; they require the
        // complete memory/shadow bounds proof, not a match with the old tag.
        // No partial update is permitted when any part of the range is invalid.
        [[nodiscard]] access_result store(Address pointer, ::std::uint64_t bytes) const noexcept
        {
            if constexpr(Shared)
            {
                if(tag_write_gate_ != nullptr)
                {
                    ::uwvm2::utils::mutex::rw_fair_unique_guard_t held{*tag_write_gate_};
                    return store_under_lease(pointer, bytes);
                }
            }
            return store_under_lease(pointer, bytes);
        }

    private:
        [[nodiscard]] access_result store_under_lease(Address pointer, ::std::uint64_t bytes) const noexcept
        {
            auto result{bounds(pointer, 0u, bytes)};
            if(!result || bytes == 0u) { return result; }
            write_tags(result,bytes);
            return result;
        }

        template<bool Allocation = false>
        void write_tags(access_result const& result,::std::uint64_t bytes) const noexcept
        {
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
            else
            {
                auto const count{end-first};
                if constexpr(Allocation)
                {
                    if(count <= short_allocation_tags::halfword_pair_count)
                    {
                        short_allocation_tags{tags_+first,count}.store(result.pointer_tag);
                        return;
                    }
                }
                ::std::memset(tags_+first,result.pointer_tag,count);
            }
        }

    public:
        // Bind a lifecycle instruction's bounds proof to its actual view and
        // length. Recoloring the pointer preserves both interval endpoints.
        class allocation_range
        {
            friend class shadow_view;
            shadow_view const& view_;
            ::std::uint64_t bytes_;
            access_result proof_;

            allocation_range(shadow_view const& view, Address input, ::std::uint64_t bytes) noexcept
                : view_{view},bytes_{bytes},proof_{view.bounds(input,0u,bytes)} {}

        public:
            [[nodiscard]] access_result const& bounds() const noexcept { return proof_; }

            [[nodiscard,gnu::always_inline]] access_result check(::std::uint8_t expected) const noexcept
            {
                auto result{proof_};
                if(!result || !bytes_) { return result; }
                result.pointer_tag=expected;
                if constexpr(!Shared)
                {
                    auto const first{static_cast<::std::size_t>(result.address >> 4u)};
                    auto const last{static_cast<::std::size_t>((result.address+bytes_-1u) >> 4u)};
                    return view_.check_allocation_tags(result,first,last-first+1u);
                }
                else { return view_.check_tags(result,bytes_,0xffu); }
            }
            [[nodiscard]] access_result store(::std::uint8_t tag) const noexcept requires (!Shared)
            {
                auto result{proof_};
                if(!result || !bytes_) { return result; }
                result.pointer_tag=tag;
                view_.template write_tags<true>(result,bytes_);
                return result;
            }
        };

        [[nodiscard]] allocation_range allocation(Address input,::std::uint64_t bytes) const & noexcept
        { return {*this,input,bytes}; }
        allocation_range allocation(Address,::std::uint64_t) const && = delete;

        // Borrowed for the owner's pinned private operation. Both endpoints
        // must map to actual data and shadow granules before forming the span.
        [[nodiscard,gnu::always_inline]] ::std::span<::std::uint8_t const> private_tag_window(
            ::std::uint64_t first, ::std::uint64_t last) const noexcept requires (!Shared)
        {
            if(first > last || !tags_ || last >= count_ || !memory_bytes_ ||
               memory_bytes_ - 1u > effective_mask_ || last > (memory_bytes_-1u)/16u) { return {}; }
            return {tags_+static_cast<::std::size_t>(first),static_cast<::std::size_t>(last-first+1u)};
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
        ::uwvm2::utils::mutex::rwlock_t* tag_write_gate_{};
        Address effective_mask_{address_mask};
        unsigned tag_start_{start_bit};
        bool has_tags_{true};
#if defined(__linux__) && defined(__aarch64__)
        ::std::byte* hardware_base_{};
#endif
    };
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
