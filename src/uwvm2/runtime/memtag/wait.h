/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cstddef>
# include <cstdint>
# include <memory>
# include <span>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/runtime/wasm_threads/impl.h>
# include <uwvm2/runtime/compiler/shared/wasm_threads.h>
# include "bridge.h"
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    struct wait_execution_result
    {
        ::uwvm2::runtime::wasm_threads::wait_result wait{};
        ::uwvm2::extension::memtag::access_result access{};
    };
    // The tag check and SC comparison share the same owner snapshot, inside
    // the existing wait shard. Neither a native byte pointer nor an allocator
    // relocation pin survives the comparison or the suspension.
    template<unsigned Operation, ::std::unsigned_integral Address, bool Shared, typename Memory>
    [[nodiscard]] inline wait_execution_result wait_notify(
        Memory const& memory, Address pointer, ::std::uint64_t offset,
        ::std::uint64_t expected, ::std::int64_t timeout_ns) noexcept
    {
        static_assert(Operation<=2u);
        deferred::poll(true);
        namespace waiting=::uwvm2::runtime::wasm_threads;
        constexpr ::std::size_t bytes{Operation==2u ? 8u : 4u};
        wait_execution_result result{};
        auto const checked{check_access<Address,Shared>(memory,pointer,offset,bytes)};
        result.access=checked;
        if(!checked) { return result; }
        if((checked.address&(bytes-1u))!=0u) { result.wait.status=waiting::wait_status::unaligned;return result; }
        if constexpr(Operation==0u)
        {
            result.wait=waiting::memory_notify(memory,checked.address,static_cast<::std::uint32_t>(expected));
            return result;
        }
        if(!waiting::is_shared(memory)) { result.wait.status=waiting::wait_status::not_shared;return result; }
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
        auto const execution{waiting::current_execution};
        if(execution==nullptr) { result.wait.status=waiting::wait_status::unavailable;return result; }
        auto const matches{[&]() noexcept
        {
            bool equal{};
            result.access=with_checked_access<Address,Shared>(memory,pointer,offset,bytes,
                [&](::std::span<::std::byte> data) noexcept
                {
                    auto const loaded{::uwvm2::runtime::compiler::shared::wasm_threads::atomic_load_le<bytes>(data.data())};
                    if constexpr(bytes==4u) { equal=loaded==static_cast<::std::uint32_t>(expected); }
                    else { equal=loaded==expected; }
                });
            return static_cast<bool>(result.access)&&equal;
        }};
        auto const outcome{[&]
        {
            if(execution->pause!=nullptr)
            {
                return execution->domain.waiters.wait_suspendable(
                    {::std::addressof(memory),checked.address},timeout_ns,matches,*execution->pause,
                    execution->cancellation,UINT32_MAX).outcome;
            }
            return execution->domain.waiters.wait(
                {::std::addressof(memory),checked.address},timeout_ns,matches,execution->cancellation,UINT32_MAX);
        }()};
        if(!result.access) { return result; }
        using outcome_type=::uwvm2::utils::thread::keyed_wait_result;
        switch(outcome)
        {
            case outcome_type::notified:result.wait.status=waiting::wait_status::notified;break;
            case outcome_type::not_equal:result.wait.status=waiting::wait_status::not_equal;break;
            case outcome_type::timed_out:result.wait.status=waiting::wait_status::timed_out;break;
            case outcome_type::cancelled:case outcome_type::closed:result.wait.status=waiting::wait_status::cancelled;break;
            case outcome_type::too_many_waiters:result.wait.status=waiting::wait_status::too_many_waiters;break;
        }
#else
        result.wait.status=waiting::wait_status::unavailable;
#endif
        return result;
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
