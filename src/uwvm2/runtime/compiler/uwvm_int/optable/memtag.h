/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <uwvm2/runtime/compiler/shared/interpreter_adm_guard.h>
#include <array>
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
        enum class scalar_policy : unsigned { enforcing, synchronous_log, software_async, generic, adm, adm16_live };
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
        template<bool Enabled, ::std::unsigned_integral Address, bool Shared, bool UntaggedOwner=false>
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
            auto const result{[&]() noexcept
            {
                if constexpr(UntaggedOwner)
                {
                    static_assert(!Enabled);
                    return ::uwvm2::runtime::memtag::execute_untagged<Address,Shared>(
                        memory,*descriptor,{values.data(),descriptor->operand_count});
                }
                else { return ::uwvm2::runtime::memtag::execute<Enabled,Address,Shared>(
                    memory,operation,{values.data(),descriptor->operand_count}); }
            }()};
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

        template<typename Value, ::std::size_t Bytes, bool Signed, bool Store>
        inline void scalar_bytes(::std::byte* buffer, Value& value) noexcept
        {
            if constexpr(::std::is_integral_v<Value>)
            {
                if constexpr(Store) { details::memory64::store_integer<Bytes>(buffer,value); }
                else { value=details::memory64::load_integer<Value,Bytes,Signed>(buffer); }
            }
            else
            {
                static_assert(Bytes==sizeof(Value));
                if constexpr(Store && Bytes==4u) { details::store_f32_le(buffer,value); }
                else if constexpr(Store && Bytes==8u) { details::store_f64_le(buffer,value); }
                else if constexpr(!Store && Bytes==4u) { details::load_f32_le(buffer,value); }
                else if constexpr(!Store && Bytes==8u) { details::load_f64_le(buffer,value); }
                else if constexpr(Store) { ::std::memcpy(buffer,::std::addressof(value),Bytes); }
                else { ::std::memcpy(::std::addressof(value),buffer,Bytes); }
            }
        }

#if defined(UWVM_SUPPORT_MMAP)
        enum class adm_scalar_bounds : unsigned { unavailable, live_length, declared_minimum, full_guard };
        inline constexpr auto wasm64_adm16_mask{
            ::uwvm2::extension::memtag::adm_address_mask(::uwvm2::extension::memtag::address_width::i64,16u)};

        // Emitted for a private, stable mapping after proving mask+offset+Bytes
        // representable. A varying sub-OS-page tail always needs its byte length
        // unless the complete reachable access is below the declared minimum.
        template<typename Value, ::std::size_t Bytes, bool Signed, bool Store, bool Wasm64Adm16=false>
        UWVM_ALWAYS_INLINE inline void adm_scalar_access(adm_scalar_bounds bounds, native_memory_t const& memory,
            ::std::uint64_t raw, ::std::uint64_t offset, Value& value) noexcept
        {
            auto const mask{Wasm64Adm16 ? wasm64_adm16_mask : memory.memtag_shadow.address_mask()};
            auto const effective{(raw & mask)+offset};
            if(bounds == adm_scalar_bounds::live_length)
            {
                auto const length{memory.memory_length_p->load(::std::memory_order_acquire)};
                if(effective+Bytes > length) [[unlikely]]
                { details::memory_oob_terminate(memory,offset,{.offset=effective,.offset_65_bit=false},length,Bytes); }
            }
            // The translated range fits either the live data or the qualified,
            // immutable mmap reservation, including the final accessed byte.
            auto* begin{memory.memory_begin+static_cast<::std::size_t>(effective)};
#if UWVM2_HAS_INT_ADM_GUARD && defined(UWVM_RUNTIME_LLVM_JIT)
            if(bounds == adm_scalar_bounds::full_guard)
            {
                namespace guard=::uwvm2::runtime::compiler::shared::int_adm_guard;
                ::std::uint64_t bits{};
                auto* encoded{reinterpret_cast<::std::byte*>(::std::addressof(bits))};
                if constexpr(Store)
                {
                    if constexpr(Bytes>1u)
                    {
                        auto const page{::std::uint64_t{1} << memory.custom_page_size_log2};
                        if((effective&(page-1u)) > page-Bytes)
                        { static_cast<void>(guard::load<1u>(begin+Bytes-1u)); }
                    }
                    scalar_bytes<Value,Bytes,Signed,true>(encoded,value);
                    guard::store<Bytes>(begin,bits);
                }
                else
                {
                    bits=guard::load<Bytes>(begin);
                    scalar_bytes<Value,Bytes,Signed,false>(encoded,value);
                }
                return;
            }
#endif
            scalar_bytes<Value,Bytes,Signed,Store>(begin,value);
        }

        template<adm_scalar_bounds Bounds, ::std::unsigned_integral Address, typename Value, ::std::size_t Bytes, bool Signed, bool Store>
        inline void adm_scalar(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t offset) noexcept
        {
            Value value{};
            if constexpr(Store)
            {
                // Moving a floating payload through an x87 return value may quiet a signalling NaN.
                stack -= sizeof(Value);
                ::std::memcpy(::std::addressof(value),stack,sizeof(Value));
            }
            auto const raw{pop<Address>(stack)};
            adm_scalar_access<Value,Bytes,Signed,Store>(Bounds,memory,raw,offset,value);
            if constexpr(!Store) { push(stack,value); }
        }
#endif

        // Separate tagged scalar handlers preserve the ordinary memory opfuncs.
        // No tag-mode branch is inserted into disabled/uninstrumented execution.
        template<::std::unsigned_integral Address, bool Shared, typename Value, ::std::size_t Bytes, bool Signed, bool Store>
        inline void scalar(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t offset) noexcept
        {
            static_assert(Bytes>=1u && Bytes<=sizeof(Value) && sizeof(Value)<=16u);
            Value value{};
            if constexpr(Store)
            {
                // Copy floating representations directly; an x87 return ABI can
                // otherwise quiet signalling NaNs while merely transporting bits.
                stack-=sizeof(Value);::std::memcpy(::std::addressof(value),stack,sizeof(Value));
            }
            auto const pointer{pop<Address>(stack)};
            auto const result{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(
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
                    scalar_bytes<Value,Bytes,Signed,Store>(buffer,value);
                    if constexpr(Shared && Store)
                    {
                        for(::std::size_t index{};index!=Bytes;++index)
                        { ::std::atomic_ref<::std::byte>(data[index]).store(buffer[index],::std::memory_order_relaxed); }
                    }
                })};
            if(!result) { access_failed(memory,pointer,offset,Bytes,result); }
            if constexpr(!Store) { push(stack,value); }
        }

        // The i686 translator binds this materialized-stack action to a private
        // integer access under the immutable software synchronous/crash policy.
        template<::std::unsigned_integral Address, typename Value, ::std::size_t Bytes, bool Signed, bool Store>
        inline void scalar_enforcing(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t offset) noexcept
        {
            static_assert(::std::same_as<Value,details::wasm_i32> || ::std::same_as<Value,details::wasm_i64>);
            static_assert(Bytes>=1u && Bytes<=sizeof(Value));
            Value value{};
            if constexpr(Store) { value=pop<Value>(stack); }
            auto const pointer{pop<Address>(stack)};
            auto const result{::uwvm2::runtime::memtag::with_access_policy<Address,false,false,true>(
                memory,pointer,offset,Bytes,[&](::std::span<::std::byte> data) noexcept
                {
                    if constexpr(Store) { details::memory64::store_integer<Bytes>(data.data(),value); }
                    else { value=details::memory64::load_integer<Value,Bytes,Signed>(data.data()); }
                })};
            if(!result) { access_failed(memory,pointer,offset,Bytes,result); }
            if constexpr(!Store) { push(stack,value); }
        }

        template<scalar_policy Policy, ::std::unsigned_integral Address, typename Value, ::std::size_t Bytes, bool Signed, bool Store>
        UWVM_ALWAYS_INLINE inline constexpr void checked_integer_access(native_memory_t const& memory, Address raw,
            ::std::uint64_t offset, Value& value, ::std::byte const*& ip) noexcept
        {
            static_assert(Policy != scalar_policy::adm && Policy != scalar_policy::adm16_live);
            ::std::byte const* encoded_origin{};
            if constexpr(Policy==scalar_policy::synchronous_log || Policy==scalar_policy::software_async)
            {
                // [module][function][Wasm offset][successor] in the translated instruction.
                encoded_origin=ip;
                ip+=3u*sizeof(::std::uint64_t);
            }
            auto access{[&](::std::span<::std::byte> data) noexcept
            {
                scalar_bytes<Value,Bytes,Signed,Store>(data.data(),value);
            }};
            auto const result{[&]() noexcept
            {
                if constexpr(Policy==scalar_policy::enforcing)
                { return ::uwvm2::runtime::memtag::with_access_policy<Address,false,false,true>(
                    memory,raw,offset,Bytes,access); }
                else if constexpr(Policy==scalar_policy::synchronous_log)
                { return ::uwvm2::runtime::memtag::with_access_policy<Address,false,false,false,true>(
                    memory,raw,offset,Bytes,access,encoded_origin); }
                else if constexpr(Policy==scalar_policy::software_async)
                { return ::uwvm2::runtime::memtag::with_access_policy<Address,false,true,false,true>(
                    memory,raw,offset,Bytes,access,encoded_origin); }
                else { return ::uwvm2::runtime::memtag::with_guest_access<Address,false>(
                    memory,raw,offset,Bytes,access); }
            }()};
            // The checked-access bridge has released every pin before reporting.
            if(!result) { access_failed(memory,raw,offset,Bytes,result); }
        }

        // Both ADM and checked MTE access use the typed operand layout selected by the translator.
        template<::std::unsigned_integral Address, typename Value, ::std::size_t Bytes, bool Signed, bool Store,
                 memtag_details::scalar_policy Policy, uwvm_interpreter_translate_option_t Option, ::std::size_t ValuePos, ::std::size_t AddressPos,
                 uwvm_int_stack_top_type... Type>
        UWVM_ALWAYS_INLINE inline constexpr void execute_scalar_cached(Type&... args) UWVM_THROWS
        {
            using wasm_address = ::std::conditional_t<sizeof(Address)==4u,details::wasm_i32,details::wasm_i64>;
            static_assert(::std::same_as<Value,details::wasm_i32> || ::std::same_as<Value,details::wasm_i64>);
            static_assert(Bytes>=1u && Bytes<=sizeof(Value));
            constexpr auto value_begin{details::stacktop_range_begin_pos<Option,Value>()};
            constexpr auto value_end{details::stacktop_range_end_pos<Option,Value>()};
            constexpr auto address_begin{details::stacktop_range_begin_pos<Option,wasm_address>()};
            constexpr auto address_end{details::stacktop_range_end_pos<Option,wasm_address>()};
            constexpr bool merged{value_begin!=value_end && value_begin==address_begin && value_end==address_end};

            // [handler][owner][u64 offset][policy-specific immediates][successor]
            // All slots belong to this translated instruction or its successor.
            if constexpr(Option.is_tail_call) { args...[0]+=sizeof(uwvm_interpreter_opfunc_t<Type...>); }
            else { args...[0]+=sizeof(uwvm_interpreter_opfunc_byref_t<Type...>); }
            auto const memory{details::read_imm<native_memory_t*>(args...[0])};
            auto const offset{details::read_imm<::std::uint64_t>(args...[0])};
            Value value{};
            if constexpr(Store)
            { value=get_curr_val_from_operand_stack_top<Option,Value,ValuePos>(args...); }
            wasm_address address{};
            if constexpr(Store && merged)
            {
                static_assert(ValuePos==AddressPos);
                if constexpr(address_end-address_begin>=2uz)
                {
                    constexpr auto position{details::ring_next_pos(AddressPos,address_begin,address_end)};
                    address=get_curr_val_from_operand_stack_top<Option,wasm_address,position>(args...);
                }
                else
                {
                    // A one-slot merged ring contains only the store value;
                    // its address was spilled by the normal typed emitter.
                    address=get_curr_val_from_operand_stack_cache<wasm_address>(args...);
                }
            }
            else { address=get_curr_val_from_operand_stack_top<Option,wasm_address,AddressPos>(args...); }
            auto const raw{static_cast<Address>(address)};
#if defined(UWVM_SUPPORT_MMAP)
            if constexpr(Policy==scalar_policy::adm16_live)
            {
                static_assert(::std::same_as<Address,::std::uint64_t>);
                adm_scalar_access<Value,Bytes,Signed,Store,true>(adm_scalar_bounds::live_length,*memory,raw,offset,value);
            }
            else if constexpr(Policy==scalar_policy::adm)
            {
                // Other ADM geometries share one handler per pair of register positions.
                auto const bounds{details::read_imm<adm_scalar_bounds>(args...[0])};
                adm_scalar_access<Value,Bytes,Signed,Store>(bounds,*memory,raw,offset,value);
            }
            else
#endif
            {
                checked_integer_access<Policy,Address,Value,Bytes,Signed,Store>(*memory,raw,offset,value,args...[0]);
            }
            if constexpr(!Store)
            {
                if constexpr(value_begin!=value_end)
                {
                    if constexpr(merged)
                    {
                        static_assert(ValuePos==AddressPos);
                        details::set_curr_val_to_stacktop_cache<Option,Value,ValuePos>(value,args...);
                    }
                    else
                    {
                        constexpr auto position{details::ring_prev_pos(ValuePos,value_begin,value_end)};
                        details::set_curr_val_to_stacktop_cache<Option,Value,position>(value,args...);
                    }
                }
                else
                {
                    // [older operands][reserved integer result] frame end.
                    ::std::memcpy(args...[1],::std::addressof(value),sizeof(value));
                    args...[1]+=sizeof(value);
                }
            }
        }
    }


    template<::std::unsigned_integral Address, typename Value, ::std::size_t Bytes, bool Signed, bool Store,
             memtag_details::scalar_policy Policy, uwvm_interpreter_translate_option_t Option, ::std::size_t ValuePos, ::std::size_t AddressPos,
             uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_scalar_cached(Type... args) UWVM_THROWS
    {
        memtag_details::execute_scalar_cached<Address,Value,Bytes,Signed,Store,Policy,Option,ValuePos,AddressPos>(args...);
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<::std::unsigned_integral Address, typename Value, ::std::size_t Bytes, bool Signed, bool Store,
             memtag_details::scalar_policy Policy, uwvm_interpreter_translate_option_t Option, ::std::size_t ValuePos, ::std::size_t AddressPos,
             uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_scalar_cached(Type&... args) UWVM_THROWS
    { memtag_details::execute_scalar_cached<Address,Value,Bytes,Signed,Store,Policy,Option,ValuePos,AddressPos>(args...); }

    template<uwvm_interpreter_translate_option_t Option, bool CheckTags, uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_dispatch(Type... args) UWVM_THROWS
    {
        args...[0] += sizeof(uwvm_interpreter_opfunc_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const action{details::read_imm<memtag_details::action_t>(args...[0])};
        auto const immediate{details::read_imm<::std::uint64_t>(args...[0])};
        { ::uwvm2::extension::memtag::scoped_access_checks checks{CheckTags}; action(*memory,args...[1],immediate); }
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<uwvm_interpreter_translate_option_t Option, bool CheckTags, uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_dispatch(Type&... args) UWVM_THROWS
    {
        args...[0] += sizeof(uwvm_interpreter_opfunc_byref_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const action{details::read_imm<memtag_details::action_t>(args...[0])};
        auto const immediate{details::read_imm<::std::uint64_t>(args...[0])};
        { ::uwvm2::extension::memtag::scoped_access_checks checks{CheckTags}; action(*memory,args...[1],immediate); }
    }
    template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_boundary(Type... args) UWVM_THROWS
    {
        args...[0] += sizeof(uwvm_interpreter_opfunc_t<Type...>);
        auto const module{details::read_imm<::std::uint64_t>(args...[0])};
        auto const function{details::read_imm<::std::uint64_t>(args...[0])};
        auto const offset{details::read_imm<::std::uint64_t>(args...[0])};
        auto const flags{details::read_imm<unsigned>(args...[0])};
        ::uwvm2::runtime::memtag::deferred::boundary(module,function,offset,flags);
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_boundary(Type&... args) UWVM_THROWS
    {
        args...[0] += sizeof(uwvm_interpreter_opfunc_byref_t<Type...>);
        auto const module{details::read_imm<::std::uint64_t>(args...[0])};
        auto const function{details::read_imm<::std::uint64_t>(args...[0])};
        auto const offset{details::read_imm<::std::uint64_t>(args...[0])};
        auto const flags{details::read_imm<unsigned>(args...[0])};
        ::uwvm2::runtime::memtag::deferred::boundary(module,function,offset,flags);
    }
    // Software-only pure-control boundary: evidence already carries its source.
    // Host/GC/atomic and hardware-async sites retain the complete boundary op.
    template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_poll(Type... args) UWVM_THROWS
    {
        args...[0]+=sizeof(uwvm_interpreter_opfunc_t<Type...>);
        ::uwvm2::runtime::memtag::deferred::poll(true);
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_poll(Type&... args) UWVM_THROWS
    {
        args...[0]+=sizeof(uwvm_interpreter_opfunc_byref_t<Type...>);
        ::uwvm2::runtime::memtag::deferred::poll(true);
    }
    namespace translate
    {

        namespace details::memtag_scalar
        {
            template<typename Address,typename Value,::std::size_t Bytes,bool Signed,bool Store,memtag_details::scalar_policy Policy>
            struct operation
            {
                template<uwvm_interpreter_translate_option_t O,::std::size_t ValuePos,::std::size_t AddrPos,uwvm_int_stack_top_type... T>
                inline static constexpr uwvm_interpreter_opfunc_t<T...> choose() noexcept
                { return uwvmint_memtag_scalar_cached<Address,Value,Bytes,Signed,Store,Policy,O,ValuePos,AddrPos,T...>; }
                struct one
                {
                    template<uwvm_interpreter_translate_option_t O,::std::size_t Pos,uwvm_int_stack_top_type... T>
                    inline static constexpr uwvm_interpreter_opfunc_t<T...> fptr() noexcept
                    { return operation::template choose<O,Pos,Pos,T...>(); }
                };
                struct two
                {
                    template<uwvm_interpreter_translate_option_t O,::std::size_t ValuePos,::std::size_t AddrPos,uwvm_int_stack_top_type... T>
                    inline static constexpr uwvm_interpreter_opfunc_t<T...> fptr() noexcept
                    { return operation::template choose<O,ValuePos,AddrPos,T...>(); }
                };
                struct value_only
                {
                    template<uwvm_interpreter_translate_option_t O,::std::size_t Pos,uwvm_int_stack_top_type... T>
                    inline static constexpr uwvm_interpreter_opfunc_t<T...> fptr() noexcept
                    { return operation::template choose<O,Pos,0uz,T...>(); }
                };
                struct address_only
                {
                    template<uwvm_interpreter_translate_option_t O,::std::size_t Pos,uwvm_int_stack_top_type... T>
                    inline static constexpr uwvm_interpreter_opfunc_t<T...> fptr() noexcept
                    { return operation::template choose<O,0uz,Pos,T...>(); }
                };
            };
        }

        template<::std::unsigned_integral Address,typename Value,::std::size_t Bytes,bool Signed,bool Store,memtag_details::scalar_policy Policy,
                 uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        [[nodiscard]] inline constexpr auto get_uwvmint_memtag_scalar_cached_from_tuple(
            uwvm_interpreter_stacktop_currpos_t const& positions,details::op_details::native_memory_t const&,
            ::uwvm2::utils::container::tuple<Type...> const&) noexcept
        {
            namespace op=details::op_details;
            using wasm_address=::std::conditional_t<sizeof(Address)==4u,op::wasm_i32,op::wasm_i64>;
            if constexpr(!Option.is_tail_call)
            { return static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(
                uwvmint_memtag_scalar_cached<Address,Value,Bytes,Signed,Store,Policy,Option,0uz,0uz,Type...>); }
            else
            {
                using operations=details::memtag_scalar::operation<Address,Value,Bytes,Signed,Store,Policy>;
                constexpr auto value_begin{op::stacktop_range_begin_pos<Option,Value>()};
                constexpr auto value_end{op::stacktop_range_end_pos<Option,Value>()};
                constexpr auto address_begin{op::stacktop_range_begin_pos<Option,wasm_address>()};
                constexpr auto address_end{op::stacktop_range_end_pos<Option,wasm_address>()};
                // Bounds and tag policy were selected by the emitter. Only the register positions vary here.
                if constexpr(Store)
                { return details::select_binary_fptr<Option,Value,wasm_address,value_begin,value_end,address_begin,address_end,
                    typename operations::one,typename operations::two,typename operations::address_only,Type...>(positions); }
                else
                { return details::select_unary_fptr<Option,wasm_address,Value,address_begin,address_end,value_begin,value_end,
                    typename operations::one,typename operations::two,typename operations::value_only,Type...>(positions); }
            }
        }

        template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        inline constexpr auto get_uwvmint_memtag_dispatch_from_tuple(::uwvm2::utils::container::tuple<Type...>, bool check_tags=true) noexcept
        {
            if constexpr(Option.is_tail_call)
            { return check_tags ? static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_dispatch<Option,true,Type...>) :
                     static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_dispatch<Option,false,Type...>); }
            else
            { return check_tags ? static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_dispatch<Option,true,Type...>) :
                     static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_dispatch<Option,false,Type...>); }
        }
        template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        inline constexpr auto get_uwvmint_memtag_poll_from_tuple(::uwvm2::utils::container::tuple<Type...>) noexcept
        {
            if constexpr(Option.is_tail_call)
            { return static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_poll<Option,Type...>); }
            else
            { return static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_poll<Option,Type...>); }
        }
        template<uwvm_interpreter_translate_option_t Option, uwvm_int_stack_top_type... Type>
        inline constexpr auto get_uwvmint_memtag_boundary_from_tuple(::uwvm2::utils::container::tuple<Type...>) noexcept
        {
            if constexpr(Option.is_tail_call)
            { return static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_boundary<Option,Type...>); }
            else
            { return static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_boundary<Option,Type...>); }
        }
    }
}
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/pop_macros.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
