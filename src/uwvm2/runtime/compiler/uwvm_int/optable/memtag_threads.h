/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <concepts>
# include <cstddef>
# include <cstdint>
# include <span>
# include <type_traits>
# include <utility>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/push_macros.h>
# include <uwvm2/uwvm/runtime/macro/push_macros.h>
# include <uwvm2/validation/standard/wasm3/atomic_immediate.h>
# include <uwvm2/runtime/memtag/wait.h>
# include "memtag.h"
# include "threads.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
UWVM_MODULE_EXPORT namespace uwvm2::runtime::compiler::uwvm_int::optable
{
    namespace memtag_details
    {
        template<::std::unsigned_integral Address,bool Shared,unsigned Opcode>
        inline void atomic_memory(native_memory_t const& memory,::std::byte*& stack,::std::uint64_t offset) noexcept
        {
            namespace atom=::uwvm2::validation::standard::wasm3;
            namespace operations=::uwvm2::runtime::compiler::shared::wasm_threads;
            constexpr auto descriptor{atom::describe_atomic_instruction(Opcode)};
            using kind=atom::atomic_instruction_kind;
            static_assert(descriptor.kind!=kind::invalid && descriptor.kind!=kind::fence);
            constexpr ::std::size_t bytes{1u<<descriptor.natural_alignment};
            using value_type=::std::conditional_t<descriptor.value_i64,::std::uint64_t,::std::uint32_t>;
            if constexpr(Opcode<=2u)
            {
                ::std::int64_t timeout{};
                if constexpr(Opcode!=0u) { timeout=pop<::std::int64_t>(stack); }
                auto const expected{pop<value_type>(stack)};
                auto const pointer{pop<Address>(stack)};
                auto const result{::uwvm2::runtime::memtag::wait_notify<Opcode,Address,Shared>(
                    memory,pointer,offset,expected,timeout)};
                if(!result.access) { access_failed(memory,pointer,offset,bytes,result.access); }
                namespace waiting=::uwvm2::runtime::wasm_threads;
                if(result.wait.status>waiting::wait_status::timed_out)
                {
                    if(result.wait.status==waiting::wait_status::unaligned) { details::atomic_unaligned_terminate(); }
                    if(result.wait.status==waiting::wait_status::out_of_bounds)
                    { access_failed(memory,pointer,offset,bytes,{.error=::uwvm2::extension::memtag::access_error::out_of_bounds,.address=result.access.address}); }
                    if(trap_atomic_wait_func!=nullptr) { trap_atomic_wait_func(static_cast<unsigned>(result.wait.status)); }
                    ::fast_io::fast_terminate();
                }
                push(stack,static_cast<::std::uint32_t>(Opcode==0u ? result.wait.notified : static_cast<unsigned>(result.wait.status)));
            }
            else
            {
                value_type input{},expected{},value{};
                if constexpr(descriptor.kind!=kind::load) { input=pop<value_type>(stack); }
                if constexpr(descriptor.kind==kind::compare_exchange) { expected=pop<value_type>(stack); }
                auto const pointer{pop<Address>(stack)};
                bool unaligned{};
                auto const checked{::uwvm2::runtime::memtag::with_checked_access<Address,Shared>(
                    memory,pointer,offset,bytes,[&](::std::span<::std::byte> data) noexcept
                    {
                        // Granule tags occupy high bits, so the address's low
                        // alignment bits are unchanged; no wrapped host pointer
                        // or guest address is used before the full range proof.
                        unaligned=((static_cast<::std::uint64_t>(pointer)+offset)&(bytes-1u))!=0u;
                        if(unaligned) { return; }
                        if constexpr(descriptor.kind==kind::load)
                        { value=static_cast<value_type>(operations::atomic_load_le<bytes>(data.data())); }
                        else if constexpr(descriptor.kind==kind::store)
                        { operations::atomic_store_le<bytes>(data.data(),input); }
                        else
                        {
                            constexpr auto operation{static_cast<operations::atomic_rmw_operation>(
                                static_cast<unsigned>(descriptor.kind)-static_cast<unsigned>(kind::add))};
                            value=static_cast<value_type>(operations::atomic_rmw_le<operation,bytes>(data.data(),input,expected));
                        }
                    })};
                if(!checked) { access_failed(memory,pointer,offset,bytes,checked); }
                if(unaligned) { details::atomic_unaligned_terminate(); }
                if constexpr(descriptor.has_result) { push(stack,value); }
            }
        }
        template<::std::unsigned_integral Address,bool Shared>
        [[nodiscard]] inline constexpr action_t atomic_action(unsigned opcode) noexcept
        {
            if(opcode<=2u)
            {
                constexpr action_t waiters[]{atomic_memory<Address,Shared,0>,atomic_memory<Address,Shared,1>,atomic_memory<Address,Shared,2>};
                return waiters[opcode];
            }
            if(opcode<0x10u || opcode>0x4eu) { return nullptr; }
            return [&]<::std::size_t... Index>(::std::index_sequence<Index...>) constexpr
            {
                constexpr action_t actions[]{atomic_memory<Address,Shared,static_cast<unsigned>(0x10u+Index)>...};
                return actions[opcode-0x10u];
            }(::std::make_index_sequence<63>{});
        }
    }
}
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/runtime/macro/pop_macros.h>
# include <uwvm2/runtime/compiler/uwvm_int/macro/pop_macros.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
