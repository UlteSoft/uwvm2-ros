/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
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
# include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
# include <uwvm2/uwvm/runtime/storage/impl.h>
# include <uwvm2/runtime/wasm_threads/impl.h>
# include "memtag.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::uwvm_int::optable
{
    namespace memtag_details
    {
        using bulk_action_t=void(*)(native_memory_t const&,void const*,::std::byte*&) noexcept;
        [[nodiscard]] inline constexpr ::std::size_t diagnostic_length(::std::uint64_t bytes) noexcept
        {
            constexpr auto maximum{::std::numeric_limits<::std::size_t>::max()};
            return bytes>maximum ? maximum : static_cast<::std::size_t>(bytes);
        }
        template<::std::unsigned_integral Address,bool Shared>
        inline void fill(native_memory_t const& memory,void const*,::std::byte*& stack) noexcept
        {
            auto const bytes{pop<Address>(stack)};
            auto const value{static_cast<::std::byte>(pop<::std::uint32_t>(stack))};
            auto const destination{pop<Address>(stack)};
            auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(
                memory,destination,0u,bytes,[&](::std::span<::std::byte> data) noexcept
                {
                    if constexpr(Shared)
                    { for(auto& byte:data) { ::std::atomic_ref<::std::byte>(byte).store(value,::std::memory_order_relaxed); } }
                    else if(!data.empty()) { ::std::memset(data.data(),static_cast<int>(value),data.size()); }
                })};
            if(!checked) { access_failed(memory,destination,0u,diagnostic_length(bytes),checked); }
        }
        template<::std::unsigned_integral Destination,::std::unsigned_integral Source>
        inline void copy(native_memory_t const& destination_memory,void const* source_owner,::std::byte*& stack) noexcept
        {
            auto const& source_memory{*static_cast<native_memory_t const*>(source_owner)};
            using length_type=::std::conditional_t<sizeof(Destination)==8u && sizeof(Source)==8u,::std::uint64_t,::std::uint32_t>;
            auto const bytes{pop<length_type>(stack)};
            auto const source{pop<Source>(stack)};
            auto const destination{pop<Destination>(stack)};
            auto const checked{::uwvm2::runtime::memtag::with_guest_copy(
                destination_memory,source_memory,destination,source,bytes,
                [&](::std::span<::std::byte> destination_data,::std::span<::std::byte> source_data) noexcept
                {
                    if(destination_data.empty()) { return; }
                    bool const destination_shared{::uwvm2::runtime::wasm_threads::is_shared(destination_memory)};
                    bool const source_shared{::uwvm2::runtime::wasm_threads::is_shared(source_memory)};
                    if(!destination_shared && !source_shared)
                    { ::std::memmove(destination_data.data(),source_data.data(),destination_data.size());return; }
                    auto transfer{[&](::std::size_t index) noexcept
                    {
                        auto const value{source_shared ? ::std::atomic_ref<::std::byte>(source_data[index]).load(::std::memory_order_relaxed) :
                                                         source_data[index]};
                        if(destination_shared)
                        { ::std::atomic_ref<::std::byte>(destination_data[index]).store(value,::std::memory_order_relaxed); }
                        else { destination_data[index]=value; }
                    }};
                    auto const dst{reinterpret_cast<::std::uintptr_t>(destination_data.data())};
                    auto const src{reinterpret_cast<::std::uintptr_t>(source_data.data())};
                    if(dst>src && dst-src<destination_data.size())
                    { for(auto index{destination_data.size()};index!=0u;) { transfer(--index); } }
                    else { for(::std::size_t index{};index!=destination_data.size();++index) { transfer(index); } }
                })};
            if(!checked)
            { access_failed(checked.source_failed ? source_memory : destination_memory,
                checked.source_failed ? static_cast<::std::uint64_t>(source) : static_cast<::std::uint64_t>(destination),
                0u,diagnostic_length(bytes),checked.access); }
        }
        template<::std::unsigned_integral Address,bool Shared>
        inline void init(native_memory_t const& memory,void const* data_owner,::std::byte*& stack) noexcept
        {
            using storage=::uwvm2::uwvm::runtime::storage::local_defined_data_storage_t;
            auto const& data{*static_cast<storage const*>(data_owner)};
            auto const bytes{pop<::std::uint32_t>(stack)};
            auto const source{pop<::std::uint32_t>(stack)};
            auto const destination{pop<Address>(stack)};
            auto const payload{::uwvm2::uwvm::runtime::storage::load_wasm_data_segment_payload(data.data)};
            if((payload.byte_begin==nullptr)!=(payload.byte_end==nullptr)) { ::fast_io::fast_terminate(); }
            auto const source_size{payload.byte_begin==nullptr ? 0u : static_cast<::std::size_t>(payload.byte_end-payload.byte_begin)};
            if(bytes>source_size || source>source_size-bytes)
            { details::memory_oob_terminate(memory,0u,{.offset=source,.offset_65_bit=false},source_size,bytes); }
            auto const checked{::uwvm2::runtime::memtag::with_guest_access<Address,Shared>(
                memory,destination,0u,bytes,[&](::std::span<::std::byte> destination_data) noexcept
                {
                    if(destination_data.empty()) { return; }
                    // Segment payload bytes belong to the immutable module and
                    // remain alive under the enclosing execution lease.
                    auto const begin{payload.byte_begin+source};
                    if constexpr(Shared)
                    {
                        for(::std::size_t index{};index!=destination_data.size();++index)
                        { ::std::atomic_ref<::std::byte>(destination_data[index]).store(begin[index],::std::memory_order_relaxed); }
                    }
                    else { ::std::memcpy(destination_data.data(),begin,destination_data.size()); }
                })};
            if(!checked) { access_failed(memory,destination,0u,bytes,checked); }
        }
    }
    template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        requires(Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_bulk_dispatch(Type... args) UWVM_THROWS
    {
        args...[0]+=sizeof(uwvm_interpreter_opfunc_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const other{details::read_imm<void const*>(args...[0])};
        auto const action{details::read_imm<memtag_details::bulk_action_t>(args...[0])};
        action(*memory,other,args...[1]);
        uwvm_interpreter_opfunc_t<Type...> next{};
        ::std::memcpy(::std::addressof(next),args...[0],sizeof(next));
        UWVM_MUSTTAIL return next(args...);
    }
    template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        requires(!Option.is_tail_call)
    UWVM_INTERPRETER_OPFUNC_HOT_MACRO inline constexpr void uwvmint_memtag_bulk_dispatch(Type&... args) UWVM_THROWS
    {
        args...[0]+=sizeof(uwvm_interpreter_opfunc_byref_t<Type...>);
        auto const memory{details::read_imm<memtag_details::native_memory_t*>(args...[0])};
        auto const other{details::read_imm<void const*>(args...[0])};
        auto const action{details::read_imm<memtag_details::bulk_action_t>(args...[0])};
        action(*memory,other,args...[1]);
    }
    namespace translate
    {
        template<uwvm_interpreter_translate_option_t Option,uwvm_int_stack_top_type... Type>
        inline constexpr auto get_uwvmint_memtag_bulk_dispatch_from_tuple(::uwvm2::utils::container::tuple<Type...>) noexcept
        {
            if constexpr(Option.is_tail_call)
            { return static_cast<uwvm_interpreter_opfunc_t<Type...>>(uwvmint_memtag_bulk_dispatch<Option,Type...>); }
            else { return static_cast<uwvm_interpreter_opfunc_byref_t<Type...>>(uwvmint_memtag_bulk_dispatch<Option,Type...>); }
        }
    }
}
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/pop_macros.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
