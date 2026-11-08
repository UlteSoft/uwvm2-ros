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
# include <span>
# include <type_traits>
# include <uwvm2/utils/macro/push_macros.h>
# include "instruction.h"
# include "shadow.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    enum class operation_error : unsigned { none, unknown_opcode, wrong_arity, undefined_placeholder, memory_access };
    template<::std::unsigned_integral Address>
    struct operation_result
    {
        Address value{};
        operation_error error{};
        access_result access{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return error == operation_error::none; }
    };

    // Shared int/JIT semantic kernel. The backend pins the memory owner before
    // entry and releases all guards before converting a returned error to a
    // guest trap. No guest exception may unwind through an owner mutex here.
    // Tag execution is a template policy so the disabled specialization never
    // reads a shadow, masks a data address, or invokes the tag generator.
    template<bool Enabled, ::std::unsigned_integral Address, bool Shared, typename Random>
    [[nodiscard]] inline operation_result<Address> execute_operation(
        opcode operation, ::std::span<Address const> arguments, ::std::span<::std::byte> memory,
        shadow_view<Address, Shared> shadow, Random&& random_tag) noexcept
    {
        using view = shadow_view<Address, Shared>;
        static_assert(!Enabled || ::std::is_nothrow_invocable_v<Random&>);
        operation_result<Address> result{};
        auto const* descriptor{find_instruction(static_cast<::std::uint32_t>(operation))};
        if(descriptor == nullptr) { result.error = operation_error::unknown_opcode; return result; }
        if(arguments.size() != descriptor->operand_count)
        { result.error = operation_error::wrong_arity; return result; }
        // The published author presentation calls add a placeholder and neither
        // pinned implementation defines its arithmetic. Never invent a formula.
        if(operation == opcode::add)
        { result.error = operation_error::undefined_placeholder; return result; }
        if(operation == opcode::status)
        {
            result.value = Enabled ? static_cast<Address>(runtime_status::software_full) : Address{};
            return result;
        }
        if(operation == opcode::tagbits)
        { result.value = Enabled ? static_cast<Address>(view::TagBits) : Address{}; return result; }
        if(operation == opcode::startbit)
        {
            result.value = static_cast<Address>(Enabled ? view::start_bit : ::std::numeric_limits<Address>::digits);
            return result;
        }
        // The descriptor and exact arity proof establish argument zero here.
        auto const input{arguments[0]};
        result.value = input;
        if(operation == opcode::sub)
        {
            if constexpr(Enabled) { result.value = static_cast<Address>(view::untag(input) - view::untag(arguments[1])); }
            else { result.value = static_cast<Address>(input - arguments[1]); }
            return result;
        }
        if(operation == opcode::extract)
        {
            if constexpr(Enabled) { result.value = static_cast<Address>(view::extract(input)); }
            else { result.value = 0u; }
            return result;
        }
        if(operation == opcode::insert)
        {
            if constexpr(Enabled) { result.value = view::insert(input, arguments[1]); }
            return result;
        }
        if(operation == opcode::copy)
        {
            if constexpr(Enabled) { result.value = view::insert(input, view::extract(arguments[1])); }
            return result;
        }
        if(operation == opcode::untag)
        {
            if constexpr(Enabled) { result.value = view::untag(input); }
            return result;
        }
        if(operation == opcode::load)
        {
            if constexpr(Enabled)
            {
                auto const address{static_cast<::std::uint64_t>(view::untag(input))};
                if(address >= memory.size())
                {
                    result.error = operation_error::memory_access;
                    result.access = {.error = access_error::out_of_bounds, .address = address};
                    return result;
                }
                result.access = shadow.load(input);
                if(!result.access) { result.error = operation_error::memory_access; return result; }
                result.value = view::insert(input, result.access.allocation_tag);
            }
            return result;
        }
        if(operation == opcode::random)
        {
            if constexpr(Enabled) { result.value = view::insert(input, static_cast<Address>(random_tag())); }
            return result;
        }
        if(operation == opcode::hint)
        {
            if constexpr(Enabled)
            { result.value = view::insert(input, static_cast<Address>(view::extract(arguments[1]) + arguments[2])); }
            return result;
        }

        if constexpr(!Enabled)
        {
            if(!descriptor->zeroes_memory) { return result; }
        }

        // The remaining defined instructions have an address and byte count.
        auto const bytes{static_cast<::std::uint64_t>(arguments[1])};
        auto const address{static_cast<::std::uint64_t>(Enabled ? view::untag(input) : input)};
        // Bound BOTH the actual data snapshot and the shadow snapshot before any
        // zeroing/tag mutation. Callers cannot accidentally authorize a larger
        // shadow view than the data span supplied to this operation.
        if(address > memory.size() || bytes > memory.size() - address)
        {
            result.error = operation_error::memory_access;
            result.access = {.error = access_error::out_of_bounds, .address = address};
            return result;
        }
        if constexpr(Enabled)
        {
            result.access = shadow.bounds(input, 0u, bytes);
            if(!result.access) { result.error = operation_error::memory_access; return result; }
            if(operation == opcode::untagstore || operation == opcode::untagstorez)
            { result.value = view::untag(input); }
            else if(operation == opcode::randomstore || operation == opcode::randomstorez)
            { result.value = view::insert(input, static_cast<Address>(random_tag())); }
            else if(operation == opcode::hintstore || operation == opcode::hintstorez)
            { result.value = view::insert(input, static_cast<Address>(view::extract(arguments[2]) + arguments[3])); }
            result.access = shadow.store(result.value, bytes);
            if(!result.access) { result.error = operation_error::memory_access; return result; }
        }
        if(descriptor->zeroes_memory && bytes != 0u)
        {
            auto const start{static_cast<::std::size_t>(address)};
            auto const size{static_cast<::std::size_t>(bytes)};
            auto* destination{memory.data() + start};
            if constexpr(Enabled) { destination = shadow.native_pointer(memory.data(),result.access); }
            if constexpr(Shared)
            {
                for(::std::size_t index{}; index != size; ++index)
                { ::std::atomic_ref<::std::byte>(destination[index]).store(::std::byte{}, ::std::memory_order_relaxed); }
            }
            else { ::std::memset(destination, 0, size); }
        }
        return result;
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
