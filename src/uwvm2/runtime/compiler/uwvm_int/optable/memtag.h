/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <atomic>
# include <bit>
# include <concepts>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <span>
# include <limits>
# include <type_traits>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
# include <uwvm2/runtime/memtag/impl.h>
# include "define.h"
# include "storage.h"
# include "memory64.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::uwvm_int::optable
{
    namespace memtag_details
    {
        using native_memory_t = ::uwvm2::object::memory::linear::native_memory_t;
        using action_t = void(*)(native_memory_t const&, ::std::byte*&, ::std::uint64_t) noexcept;

        template<typename Value>
        inline Value pop(::std::byte*& stack) noexcept
        {
            // [validated operand prefix][complete Value] <- stack
            // The typed emitter materializes all cached operands before entry.
            stack -= sizeof(Value);
            Value value{};
            ::std::memcpy(::std::addressof(value),stack,sizeof(Value));
            return value;
        }
        template<typename Value>
        inline void push(::std::byte*& stack,Value const& value) noexcept
        {
            // [validated operand prefix][reserved result slot] frame end
            ::std::memcpy(stack,::std::addressof(value),sizeof(Value));
            stack += sizeof(Value);
        }
        [[noreturn]] UWVM_NOINLINE UWVM_GNU_COLD inline void operation_failed(
            unsigned bridge, unsigned operation) noexcept
        {
            if(trap_memtag_operation_func != nullptr) { trap_memtag_operation_func(bridge,operation); }
            ::fast_io::fast_terminate(); // Missing/returning trap callback is a host invariant violation.
        }
        [[noreturn]] UWVM_NOINLINE UWVM_GNU_COLD inline void access_failed(
            native_memory_t const& memory, ::std::uint64_t raw, ::std::uint64_t offset, ::std::size_t bytes,
            ::uwvm2::extension::memtag::access_result const& failure) noexcept
        {
            namespace mt = ::uwvm2::extension::memtag;
            if(failure.error==mt::access_error::tag_mismatch)
            {
                if(trap_memtag_func != nullptr)
                { trap_memtag_func(memory.diagnostic_owner_memory_index,raw,failure.mismatch_granule,
                                   failure.pointer_tag,failure.allocation_tag); }
                ::fast_io::fast_terminate();
            }
            if(failure.error!=mt::access_error::out_of_bounds)
            { operation_failed(static_cast<unsigned>(::uwvm2::runtime::memtag::bridge_error::invalid_owner),
                               static_cast<unsigned>(mt::operation_error::memory_access)); }
            // Diagnostic snapshot only, after the operation pin has released.
            auto const memory_bytes{static_cast<::std::size_t>(memory.get_page_size()) << memory.custom_page_size_log2};
            details::memory_oob_terminate(memory,offset,{.offset=failure.address,.offset_65_bit=false},memory_bytes,bytes);
        }
        template<bool Enabled, ::std::unsigned_integral Address, bool Shared>
        inline void instruction(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t immediate) noexcept
        {
            namespace mt = ::uwvm2::extension::memtag;
            auto const operation{static_cast<mt::opcode>(immediate)};
            auto const* descriptor{mt::find_instruction(static_cast<::std::uint32_t>(operation))};
            if(descriptor==nullptr)
            { operation_failed(0u,static_cast<unsigned>(mt::operation_error::unknown_opcode)); }
            ::std::array<Address,4u> values{};
            for(auto count{descriptor->operand_count};count!=0u;--count)
            { values[count-1u]=pop<Address>(stack); }
            auto const result{::uwvm2::runtime::memtag::execute<Enabled,Address,Shared>(
                memory,operation,{values.data(),descriptor->operand_count})};
            if(!result)
            {
                if(result.operation.error==mt::operation_error::memory_access)
                {
                    auto const bytes{descriptor->operand_count>1u ? static_cast<::std::uint64_t>(values[1]) : 1u};
                    constexpr auto maximum{::std::numeric_limits<::std::size_t>::max()};
                    access_failed(memory,values[0],0u,bytes>maximum ? maximum : static_cast<::std::size_t>(bytes),
                                  result.operation.access);
                }
                operation_failed(static_cast<unsigned>(result.error),static_cast<unsigned>(result.operation.error));
            }
            if(descriptor->result_count!=0u) { push(stack,result.operation.value); }
        }

        // Separate tagged scalar handlers preserve the ordinary memory opfuncs.
        // No tag-mode branch is inserted into disabled/uninstrumented execution.
        template<::std::unsigned_integral Address, bool Shared, typename Value, ::std::size_t Bytes, bool Signed, bool Store>
        inline void scalar(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t offset) noexcept
        {
            static_assert(Bytes>=1u && Bytes<=sizeof(Value) && sizeof(Value)<=16u);
            constexpr bool integer{::std::is_integral_v<Value>};
            Value value{};
            if constexpr(Store)
            {
                // Copy floating representations directly; an x87 return ABI can
                // otherwise quiet signalling NaNs while merely transporting bits.
                stack-=sizeof(Value);::std::memcpy(::std::addressof(value),stack,sizeof(Value));
            }
            auto const pointer{pop<Address>(stack)};
            auto const result{::uwvm2::runtime::memtag::with_checked_access<Address,Shared>(
                memory,pointer,offset,Bytes,[&](::std::span<::std::byte> data) noexcept
                {
                    ::std::array<::std::byte,Bytes> temporary{};
                    auto* buffer{data.data()};
                    if constexpr(Shared)
                    {
                        buffer=temporary.data();
                        if constexpr(!Store)
                        {
                            for(::std::size_t index{};index!=Bytes;++index)
                            { buffer[index]=::std::atomic_ref<::std::byte>(data[index]).load(::std::memory_order_relaxed); }
                        }
                    }
                    if constexpr(Store)
                    {
                        if constexpr(integer) { details::memory64::store_integer<Bytes>(buffer,value); }
                        else
                        {
                            static_assert(Bytes==sizeof(Value));
                            if constexpr(sizeof(Value)==4u) { details::store_f32_le(buffer,value); }
                            else if constexpr(sizeof(Value)==8u) { details::store_f64_le(buffer,value); }
                            else { ::std::memcpy(buffer,::std::addressof(value),Bytes); }
                        }
                        if constexpr(Shared)
                        {
                            for(::std::size_t index{};index!=Bytes;++index)
                            { ::std::atomic_ref<::std::byte>(data[index]).store(buffer[index],::std::memory_order_relaxed); }
                        }
                    }
                    else
                    {
                        if constexpr(integer) { value=details::memory64::load_integer<Value,Bytes,Signed>(buffer); }
                        else
                        {
                            static_assert(Bytes==sizeof(Value));
                            if constexpr(sizeof(Value)==4u) { details::load_f32_le(buffer,value); }
                            else if constexpr(sizeof(Value)==8u) { details::load_f64_le(buffer,value); }
                            else { ::std::memcpy(::std::addressof(value),buffer,Bytes); }
                        }
                    }
                })};
            if(!result) { access_failed(memory,pointer,offset,Bytes,result); }
            if constexpr(!Store) { push(stack,value); }
        }
    }

    template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_dispatch(Type... args) UWVM_THROWS
    {
        args...[0] += sizeof(uwvm_interpreter_opfunc_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const action{details::read_imm<memtag_details::action_t>(args...[0])};
        auto const immediate{details::read_imm<::std::uint64_t>(args...[0])};
        action(*memory,args...[1],immediate);
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_dispatch(Type&... args) UWVM_THROWS
    {
        args...[0] += sizeof(uwvm_interpreter_opfunc_byref_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const action{details::read_imm<memtag_details::action_t>(args...[0])};
        auto const immediate{details::read_imm<::std::uint64_t>(args...[0])};
        action(*memory,args...[1],immediate);
    }
    namespace translate
    {
        template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        inline constexpr auto get_uwvmint_memtag_dispatch_from_tuple(::uwvm2::utils::container::tuple<Type...>) noexcept
        {
            if constexpr(Option.is_tail_call)
            { return static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_dispatch<Option,Type...>); }
            else
            { return static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_dispatch<Option,Type...>); }
        }
    }
}
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/pop_macros.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
