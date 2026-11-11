/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <array>
# include <atomic>
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <span>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
# include "memtag.h"
# include "memory64_simd.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::uwvm_int::optable
{
    namespace memtag_details
    {
        namespace simd = ::uwvm2::runtime::compiler::shared::wasm1p1_simd_details;
        using simd_action_t = void(*)(native_memory_t const&,::std::byte*&,::std::uint64_t,::std::uint8_t) noexcept;
        template<::std::unsigned_integral Address,bool Shared,simd::simd_code Op>
        inline void simd_memory(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t offset,::std::uint8_t lane) noexcept
        {
            constexpr auto width{simd::simd_memory_access_size<Op>()};
            constexpr bool store{details::memory64_simd::store<Op>};
            constexpr bool consumes_vector{details::memory64_simd::consumes_vector<Op>};
            simd::wasm_v128 value{};
            if constexpr(consumes_vector) { value=pop<simd::wasm_v128>(stack); }
            auto const pointer{pop<Address>(stack)};
            auto const result{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(
                memory,pointer,offset,width,[&](::std::span<::std::byte> data) noexcept
                {
                    ::std::array<::std::byte,width> temporary{};
                    auto* buffer{data.data()};
                    if constexpr(Shared)
                    {
                        buffer=temporary.data();
                        if constexpr(!store)
                        {
                            for(::std::size_t index{};index!=width;++index)
                            { buffer[index]=::std::atomic_ref<::std::byte>(data[index]).load(::std::memory_order_relaxed); }
                        }
                    }
                    if constexpr(store)
                    {
                        simd::eval_memory_store<Op>(buffer,value,lane);
                        if constexpr(Shared)
                        {
                            for(::std::size_t index{};index!=width;++index)
                            { ::std::atomic_ref<::std::byte>(data[index]).store(buffer[index],::std::memory_order_relaxed); }
                        }
                    }
                    else { value=simd::eval_memory_load<Op>(buffer,value,lane); }
                })};
            if(!result) { access_failed(memory,pointer,offset,width,result); }
            if constexpr(!store) { push(stack,value); }
        }
    }
    template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_simd_dispatch(Type... args) UWVM_THROWS
    {
        args...[0]+=sizeof(uwvm_interpreter_opfunc_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const action{details::read_imm<memtag_details::simd_action_t>(args...[0])};
        auto const offset{details::read_imm<::std::uint64_t>(args...[0])};
        auto const lane{details::read_imm<::std::uint8_t>(args...[0])};
        action(*memory,args...[1],offset,lane);
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_simd_dispatch(Type&... args) UWVM_THROWS
    {
        args...[0]+=sizeof(uwvm_interpreter_opfunc_byref_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const action{details::read_imm<memtag_details::simd_action_t>(args...[0])};
        auto const offset{details::read_imm<::std::uint64_t>(args...[0])};
        auto const lane{details::read_imm<::std::uint8_t>(args...[0])};
        action(*memory,args...[1],offset,lane);
    }
    namespace translate
    {
        template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        inline constexpr auto get_uwvmint_memtag_simd_dispatch_from_tuple(::uwvm2::utils::container::tuple<Type...>) noexcept
        {
            if constexpr(Option.is_tail_call)
            { return static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_simd_dispatch<Option,Type...>); }
            else { return static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_simd_dispatch<Option,Type...>); }
        }
    }
}
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/pop_macros.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
