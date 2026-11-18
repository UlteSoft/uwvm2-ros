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
        inline void visit_directory(managed_allocations const& directory, ::std::uint64_t first,
                                   ::std::uint64_t last, Visitor&& visitor) noexcept
        {
            directory.visit(first >= reach ? first - reach : 0, last + reach, visitor);
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
        auto tag{shadow::extract(arguments[0])};
        if(operation == opcode::randomstore || operation == opcode::randomstorez)
        { tag = static_cast<::std::uint8_t>(random & shadow::maximum_tag); }
        else if(operation == opcode::hintstore || operation == opcode::hintstorez)
        { tag = static_cast<::std::uint8_t>((shadow::extract(arguments[2]) + arguments[3]) & shadow::maximum_tag); }
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

    /// @brief Check the parent span, select a legal color and publish under one owner lease.
    /// New allocations cover whole, currently untagged granules only. A reservation
    /// leaves backing bytes accessible through their original pointer until activation.
    /// Tagged pointers identify allocation records; they are not module credentials.
    template<::std::unsigned_integral Address, bool Shared>
    [[nodiscard]] inline operation_result<Address> transition_allocation(managed_allocations& directory,
        bool hardware, opcode operation, ::std::span<Address const> arguments, ::std::span<::std::byte> data,
        shadow_view<Address,Shared> view, ::std::uint16_t entropy, bool& retry) noexcept
    {
        using shadow = decltype(view);
        Address const input{arguments[0]}, length{arguments[1]};
        operation_result<Address> result{.value = input};
        auto const fail{[&](operation_error error) noexcept { result.error = error; return result; }};
        result.access = view.bounds(input, 0u, length);
        if(!result.access) { return fail(operation_error::memory_access); }
        auto const address{result.access.address};
        if(!length || (address % granule_size) || (length % granule_size))
        { return fail(operation_error::invalid_allocation); }
        auto const first{address / granule_size}, last{(address + length - 1) / granule_size};
        auto const input_tag{shadow::extract(input)};
        auto commit{[&](::std::uint8_t tag, bool zero) noexcept
        {
            Address const args[]{shadow::insert(input, tag), length};
            return execute_operation<true,Address,Shared>(zero ? opcode::storez : opcode::store, args,
                data, view, []() noexcept { return 0u; });
        }};
        if(operation == opcode::activate || operation == opcode::release)
        {
            auto* item{directory.exact(first, last)};
            if(!item || item->permanent || item->tag != input_tag || (operation == opcode::activate && item->active))
            { return fail(operation_error::invalid_allocation); }
            result.access = view.check(shadow::insert(input, item->active ? item->tag : item->parent), 0u, length);
            if(!result.access) { return fail(operation_error::memory_access); }
            if(operation == opcode::activate)
            {
                result = commit(item->tag, false);
                if(result) { item->active = true; }
                return result;
            }
            result.value = shadow::insert(input, item->parent);
            if(item->active) { result = commit(item->parent, false); }
            if(result) { directory.remove(*item); }
            return result;
        }
        if(input_tag) { return fail(operation_error::invalid_allocation); }
        result.access = view.check(input, 0u, length);
        if(!result.access) { return fail(operation_error::memory_access); }
        allocation_neighborhood colors{sizeof(Address) == 4 ? address_width::i32 : address_width::i64, hardware};
        bool overlaps{};
        allocation_details::visit_directory(directory, first, last, [&](managed_allocations::extent const& item) noexcept
        {
            if(item.first <= last && first <= item.last) { overlaps = true; }
            colors.forbid(item.tag);
        });
        if(overlaps) { return fail(operation_error::allocation_conflict); }
        auto const count{data.size() / granule_size + (data.size() % granule_size != 0)};
        if(!visit_allocation_neighbors(first, last, count, [&](::std::uint64_t index) noexcept
        {
            result.access = view.load(static_cast<Address>(index * granule_size));
            if(!result.access) { return false; }
            colors.forbid(result.access.allocation_tag);
            return true;
        })) { return fail(operation_error::memory_access); }
        ::std::uint8_t tag{};
        auto const selection{colors.select(entropy, tag)};
        if(selection == tag_selection::retry_entropy) { retry = true; return result; }
        if(selection == tag_selection::no_color) { return fail(operation_error::allocation_no_color); }
        bool const activate{operation != opcode::reserve};
        if(!directory.add({first, last, tag, input_tag, activate, false}))
        { return fail(operation_error::allocation_resource_limit); }
        result.value = shadow::insert(input, tag);
        if(activate)
        {
            result = commit(tag, operation == opcode::claimz);
            if(!result) { directory.remove(*directory.exact(first, last)); }
        }
        return result;
    }
}
