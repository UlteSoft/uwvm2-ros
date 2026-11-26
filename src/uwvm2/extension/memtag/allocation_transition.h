// Copyright (c) UlteSoft. Licensed under the APL-2.0 License. See LICENSE.
#pragma once
#ifndef UWVM_MODULE
# include <concepts>
# include <cstddef>
# include <cstdint>
# include <span>
# include "allocation_neighborhood.h"
# include "managed_allocations.h"
# include "operations.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    namespace allocation_details
    {
        // All arguments are validated, inclusive linear granule indices. A
        // representable linear memory ends well before UINT64_MAX granules.
        inline constexpr ::std::uint64_t reach{1u + allocation_gap_bytes / granule_size};
        template<typename Visitor>
        inline auto visit_directory(managed_allocations const& directory, ::std::uint64_t first,
                                    ::std::uint64_t last, Visitor&& visitor) noexcept
        {
            return directory.visit_near(first >= reach ? first - reach : 0, last + reach, visitor);
        }

        template<::std::unsigned_integral Address, bool Shared>
        [[nodiscard]] inline bool forbid_backing_neighbors(allocation_neighborhood& colors,
            shadow_view<Address,Shared> const& view, ::std::uint64_t first, ::std::uint64_t last,
            ::std::uint64_t count, access_result& access) noexcept
        {
            if constexpr(!Shared)
            {
                if(last < count)
                {
                    auto const lo{first < reach ? 0u : first-reach};
                    auto const hi{count-1u-last < reach ? count-1u : last+reach};
                    auto const tags{view.private_tag_window(lo,hi)};
                    if(!tags.empty())
                    {
                        auto const left{static_cast<::std::size_t>(first-lo)};
                        auto const right{static_cast<::std::size_t>(hi-last)};
                        // Most allocations have all five neighbors on both sides.
                        // Fixed-size spans let the optimizer unroll the tag reads.
                        if(left == reach && right == reach)
                        {
                            colors.forbid_tags(tags.template first<reach>(),tags.template last<reach>());
                        }
                        else
                        {
                            colors.forbid_tags(tags.first(left),tags.last(right));
                        }
                        // Match the scalar visitation order's final load even
                        // when returning an unactivated reservation or a retry.
                        if(left > right) { access={.address=lo*granule_size,.allocation_tag=tags.front()}; }
                        else if(right) { access={.address=hi*granule_size,.allocation_tag=tags.back()}; }
                        return true;
                    }
                }
            }
            // Shared tags retain individual relaxed atomic loads. A short
            // snapshot reports the first failed load in original probe order.
            return visit_allocation_neighbors(first,last,count,[&](::std::uint64_t index) noexcept
            {
                access=view.load(static_cast<Address>(index*granule_size));
                if(!access) { return false; }
                colors.forbid(access.allocation_tag);
                return true;
            });
        }
    }

    /// @brief Validate a legacy tag write against registered mutable and permanent extents.
    /// Caller holds the backing owner's exclusive tag lease and relocation pin.
    /// Empty, invalid and out-of-bounds writes retain their original kernel validation.
    template<::std::unsigned_integral Address, bool Shared>
    [[nodiscard]] inline bool allows_raw_write(managed_allocations const& directory, bool hardware,
        opcode operation, ::std::span<Address const> arguments, shadow_view<Address,Shared> view,
        ::std::uint8_t random) noexcept
    {
        if(directory.empty() || arguments[1] == 0) { return true; }
        auto const proof{view.bounds(arguments[0], 0, arguments[1])};
        if(!proof) { return true; }
        using shadow = decltype(view);
        auto tag{view.pointer_tag(arguments[0])};
        if(operation == opcode::randomstore || operation == opcode::randomstorez)
        { tag = static_cast<::std::uint8_t>(random & shadow::maximum_tag); }
        else if(operation == opcode::hintstore || operation == opcode::hintstorez)
        { tag = static_cast<::std::uint8_t>((view.pointer_tag(arguments[2]) + arguments[3]) & shadow::maximum_tag); }
        else if(operation == opcode::untagstore || operation == opcode::untagstorez) { tag = 0; }

        auto const first{proof.address / granule_size};
        auto const last{(proof.address + arguments[1] - 1) / granule_size};
        bool allowed{true};
        allocation_details::visit_directory(directory, first, last, [&](managed_allocations::extent const& item) noexcept
        {
            if((item.first <= last && first <= item.last) || item.tag == tag ||
                (hardware && (item.tag & 15u) == (tag & 15u))) { allowed = false; }
        });
        return allowed;
    }

    namespace allocation_details
    {
        [[nodiscard]] inline bool matches_issued(managed_allocations::extent const* item, ::std::uint8_t tag) noexcept
        { return item && !item->permanent && item->tag == tag; }

        [[nodiscard]] inline bool forbid_extent(allocation_neighborhood& colors, managed_allocations::extent const& item,
                                                ::std::uint64_t first, ::std::uint64_t last) noexcept
        {
            colors.forbid(item.tag);
            return item.first <= last && first <= item.last;
        }

        // A short software transition either commits in full or leaves both the
        // owner and the entropy stream untouched for the general transition.
        // try_entropy consumes its buffered word only when selection succeeds.
        template<opcode Operation, ::std::unsigned_integral Address, typename TryEntropy, typename View = shadow_view<Address,false>>
        [[nodiscard,gnu::always_inline]] inline bool try_short_private_transition(managed_allocations& directory,
            Address input, Address length, ::std::span<::std::byte> data, View const& view,
            TryEntropy&& try_entropy, Address& value) noexcept
        {
            static_assert(is_allocation_operation(Operation));
            auto const tags{view.short_software_allocation(input,length)};
            auto const address{static_cast<::std::uint64_t>(view.canonical_address(input))};
            if(!tags || address+length > data.size()) { return false; }
            auto const first{address / granule_size}, last{first + length / granule_size - 1u};
            if constexpr(Operation == opcode::activate || Operation == opcode::release)
            {
                auto const found{directory.locate_indexed(first,last)};
                auto* item{found.get()};
                if(!matches_issued(item,view.pointer_tag(input))) { return false; }
                if constexpr(Operation == opcode::activate)
                {
                    if(item->active || !tags.matches(item->parent)) { return false; }
                    tags.store(item->tag);
                    item->active=true;
                    value=input;
                }
                else
                {
                    if(!item->active || !tags.matches(item->tag)) { return false; }
                    value=view.with_tag(input,item->parent);
                    tags.store(item->parent);
                    directory.remove(found);
                }
            }
            else
            {
                if(view.pointer_tag(input) || !tags.matches(0u) || first < reach) { return false; }
                auto const lo{first-reach}, hi{last+reach};
                if(hi > (data.size()-1u)/granule_size) { return false; }
                auto const neighbors{view.private_tag_window(lo,hi)};
                if(neighbors.empty()) { return false; }
                allocation_neighborhood colors{sizeof(Address) == 4u ? address_width::i32 : address_width::i64};
                bool overlaps{};
                auto const page{directory.visit_indexed_window<reach>(first,last,[&](managed_allocations::extent const& item) noexcept
                { overlaps |= forbid_extent(colors,item,first,last); })};
                if(!page || overlaps) { return false; }
                colors.forbid_tags(neighbors.template first<reach>(),neighbors.template last<reach>());
                ::std::uint8_t tag{};
                if(!try_entropy([&] [[gnu::always_inline]] (::std::uint16_t word) noexcept { return colors.select(word,tag) == tag_selection::selected; }))
                { return false; }
                // The proven page already owns this slot. Nothing below can
                // allocate, fail, call outside the snapshot or request entropy.
                directory.add_to_page(page,{first,last,tag,0u,Operation != opcode::reserve,false});
                if constexpr(Operation != opcode::reserve) { tags.store(tag); }
                if constexpr(Operation == opcode::claimz)
                { operation_details::zero_memory<false>(data.data()+static_cast<::std::size_t>(address),static_cast<::std::size_t>(length)); }
                value=view.with_tag(input,tag);
            }
            return true;
        }

        // An instruction selected during native lowering can pass an integral_constant
        // here. The ordinary interpreter passes its decoded opcode to the same kernel.
        // A false entropy callback requests a refill outside the current owner snapshot.
        template<typename Operation, ::std::unsigned_integral Address, bool Shared, typename Entropy>
        [[nodiscard,gnu::always_inline]] inline operation_result<Address> transition(managed_allocations& directory,
            bool hardware, Operation operation, Address input, Address length, ::std::span<::std::byte> data,
            shadow_view<Address,Shared> const& view, Entropy&& get_entropy, bool& retry) noexcept
        {
            operation_result<Address> result{.value = input};
            auto const fail{[&](operation_error error) noexcept { result.error = error; return result; }};
            auto const range{view.allocation(input,length)};
            result.access = range.bounds();
            if(!result.access) { return fail(operation_error::memory_access); }
            auto const address{result.access.address};
            if(!length || (address % granule_size) || (length % granule_size))
            { return fail(operation_error::invalid_allocation); }
            ::std::uint16_t entropy{};
            if(operation != opcode::activate && operation != opcode::release && !get_entropy(entropy))
            { return result; }
            auto const first{address / granule_size}, last{(address + length - 1) / granule_size};
            auto const input_tag{view.pointer_tag(input)};
            auto commit{[&] [[gnu::always_inline]] (::std::uint8_t tag) noexcept
            {
                Address const tagged{view.with_tag(input,tag)};
                operation_result<Address> stored{.value=tagged};
                // A successful shadow proof puts the exclusive end within the
                // address capacity. The data span can still be shorter.
                if(address + length > data.size())
                { stored.access={.error=access_error::out_of_bounds,.address=address}; }
                else if constexpr(Shared) { stored.access=view.store(tagged,length); }
                else { stored.access=range.store(tag); }
                if(!stored.access) { stored.error=operation_error::memory_access; }
                return stored;
            }};
            if(operation == opcode::activate || operation == opcode::release)
            {
                auto const found{directory.locate(first,last)};
                auto* item{found.get()};
                if(!matches_issued(item,input_tag) || (operation == opcode::activate && item->active))
                { return fail(operation_error::invalid_allocation); }
                result.access = range.check(item->active ? item->tag : item->parent);
                if(!result.access) { return fail(operation_error::memory_access); }
                if(operation == opcode::activate)
                {
                    result = commit(item->tag);
                    if(result) { item->active = true; }
                    return result;
                }
                result.value = view.with_tag(input, item->parent);
                if(item->active) { result = commit(item->parent); }
                if(result) { directory.remove(found); }
                return result;
            }
            if(input_tag) { return fail(operation_error::invalid_allocation); }
            result.access = range.check(input_tag);
            if(!result.access) { return fail(operation_error::memory_access); }
            allocation_neighborhood colors{sizeof(Address) == 4 ? address_width::i32 : address_width::i64, hardware};
            bool overlaps{};
            auto const page{allocation_details::visit_directory(directory, first, last, [&](managed_allocations::extent const& item) noexcept
            {
                overlaps |= forbid_extent(colors,item,first,last);
            })};
            if(overlaps) { return fail(operation_error::allocation_conflict); }
            auto const count{data.size() / granule_size + (data.size() % granule_size != 0)};
            if(!allocation_details::forbid_backing_neighbors(colors,view,first,last,count,result.access))
            { return fail(operation_error::memory_access); }
            ::std::uint8_t tag{};
            auto const selection{colors.select(entropy, tag)};
            if(selection == tag_selection::retry_entropy) { retry = true; return result; }
            if(selection == tag_selection::no_color) { return fail(operation_error::allocation_no_color); }
            bool const activate{operation != opcode::reserve};
            managed_allocations::extent const item{first,last,tag,input_tag,activate,false};
            bool const added{Shared ? directory.add(item) : directory.add_private(item,page)};
            if(!added)
            { return fail(operation_error::allocation_resource_limit); }
            result.value = view.with_tag(input, tag);
            if(activate)
            {
                result=commit(tag);
                if(result && operation == opcode::claimz)
                {
                    operation_details::zero_memory<Shared>(view.native_pointer(data.data(),result.access),static_cast<::std::size_t>(length));
                }
                if(!result) { directory.remove(directory.locate(first,last)); }
            }
            return result;
        }
    }

    /// @brief Apply a decoded lifecycle operation with entropy already drawn by the caller.
    template<::std::unsigned_integral Address, bool Shared>
    [[nodiscard]] inline operation_result<Address> transition_allocation(managed_allocations& directory,
        bool hardware, opcode operation, ::std::span<Address const> arguments, ::std::span<::std::byte> data,
        shadow_view<Address,Shared> view, ::std::uint16_t entropy, bool& retry) noexcept
    {
        return allocation_details::transition(directory,hardware,operation,arguments[0],arguments[1],data,view,
            [entropy](::std::uint16_t& word) noexcept { word=entropy; return true; },retry);
    }
}
