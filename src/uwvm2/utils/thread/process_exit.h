/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <mutex>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::utils::thread::process_exit
{
    enum class wait_action : ::std::uint8_t { cancel, wait };
    class operation;
    [[noreturn]] inline void request_process_exit(::std::uint32_t code) noexcept;
    inline ::std::mutex mutex{};
    inline ::std::condition_variable completed{};
    inline ::std::atomic<bool> requested{};
    inline ::std::uint32_t first_code{};
    inline ::std::atomic<bool> exit_claimed{};
    inline ::std::size_t fault_transitions{};
    inline operation* operations{};
    // Each real native call owns its registration until all of its providers,
    // pins and effect guards have returned. There is no Guest-accessible setter.
    class operation
    {
        friend void request_process_exit(::std::uint32_t) noexcept;
        friend void notify_operations() noexcept;
        operation* next{};
        operation* previous_current{};
        inline static thread_local operation* current{};
        void (*wake)(void*) noexcept{};
        void* context{};
        bool hold{}, excluded{};
    public:
        operation(void (*notify)(void*) noexcept, void* object) noexcept : wake{notify}, context{object}
        {
            ::std::lock_guard lock{mutex};
            next=operations; operations=this; previous_current=current; current=this;
        }
        operation(operation const&) = delete;
        ~operation()
        {
            ::std::lock_guard lock{mutex};
            auto** item{&operations};
            while(*item && *item != this) { item=&(*item)->next; }
            if(*item) { *item=next; }
            current=previous_current; completed.notify_all();
        }
        [[nodiscard]] bool select(wait_action action) noexcept
        {
            ::std::lock_guard lock{mutex};
            if(exit_claimed.load(::std::memory_order_relaxed)) { return hold && !excluded; }
            hold=action == wait_action::wait; return true;
        }
        [[nodiscard]] bool cancelled() const noexcept
        {
            if(!exit_claimed.load(::std::memory_order_acquire)) { return false; }
            ::std::lock_guard lock{mutex}; return !hold || excluded;
        }
    };
    // A native lifecycle stop can wake pending providers without changing any
    // user quota, requesting process exit or reporting that work has drained.
    inline void notify_operations() noexcept
    {
        ::std::lock_guard lock{mutex};
        for(auto* item{operations};item;item=item->next)
        { if(item->wake) { item->wake(item->context); } }
    }
    // Linearize a validated Guest-fault stop against proc_exit. Execute the
    // real stop outside this mutex: domain close notifies native waiters and
    // may itself enter notify_operations(). proc_exit waits for an already
    // reserved fault transition before publishing its request.
    inline bool run_before_exit_request(void (*stop)(void*) noexcept,void* context) noexcept
    {
        {
            ::std::lock_guard lock{mutex};
            if(exit_claimed.load(::std::memory_order_relaxed)) { return false; }
            ++fault_transitions;
        }
        stop(context);
        {
            ::std::lock_guard lock{mutex};
            --fault_transitions; completed.notify_all();
        }
        return true;
    }
    // Called after genuine proc_exit authorization or by the trusted runtime
    // stopping execution for a Guest fault; never exposed as an unchecked import.
    // First code wins. Every concurrent caller joins the same process shutdown; none may
    // bypass a pending wait owner by issuing a second proc_exit.
    [[noreturn]] inline void request_process_exit(::std::uint32_t code) noexcept
    {
        ::std::unique_lock lock{mutex};
        if(!exit_claimed.load(::std::memory_order_relaxed)) { first_code=code; exit_claimed.store(true,::std::memory_order_release); }
        completed.wait(lock, [] { return fault_transitions==0u; });
        requested.store(true, ::std::memory_order_release);
        // A proc_exit caller is not a pending message. Exclude all of its
        // lexical native scopes so nested imports cannot wait on themselves.
        for(auto* item{operation::current}; item; item=item->previous_current)
        { item->excluded=true; }
        for(auto* item{operations}; item; item=item->next)
        { if(item->wake) { item->wake(item->context); } }
        completed.notify_all();
        completed.wait(lock, []
        {
            for(auto* item{operations}; item; item=item->next)
            { if(item->hold && !item->excluded) { return false; } }
            return true;
        });
        auto const result{static_cast<int>(first_code & 255u)};
        lock.unlock();
        ::std::_Exit(result);
    }
}
