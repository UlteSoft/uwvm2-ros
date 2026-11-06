/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include <uwvm2/utils/control/host_diagnostics.h>
#ifndef UWVM_MODULE
#include <uwvm2/utils/thread/monotonic_clock.h>
#endif
#ifndef UWVM_MODULE
#include <array>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <uwvm2/utils/thread/process_exit.h>
#if defined(__linux__)
#include <cerrno>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>
#endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::environment
{
    inline constexpr ::std::uint64_t unlimited{UINT64_MAX};
    [[nodiscard]] inline ::std::chrono::steady_clock::time_point deadline_from_ns(::std::uint64_t timeout) noexcept
    {
        using clock = ::std::chrono::steady_clock;
        if(timeout == unlimited || timeout > static_cast<::std::uint64_t>(INT64_MAX)) { return clock::time_point::max(); }
        auto const now{::uwvm2::utils::host_clock::now()};
        auto const delta{::std::chrono::duration_cast<clock::duration>(::std::chrono::nanoseconds{static_cast<::std::int64_t>(timeout)})};
        if(now > clock::time_point::max() - delta) { return clock::time_point::max(); }
        return now + delta;
    }
    enum class budget_kind : unsigned { random, stdin_read, stdout_write, stderr_write, realtime, monotonic, process, thread, poll, signal, yield, filesystem_io, count };
    [[nodiscard]] inline constexpr budget_kind fd_io_budget(unsigned fd) noexcept
    { return fd == 0u ? budget_kind::stdin_read : fd == 1u ? budget_kind::stdout_write : fd == 2u ? budget_kind::stderr_write : budget_kind::filesystem_io; }
    inline void quota_diagnostic(budget_kind kind,::std::string_view field,bool wait) noexcept
    {
        if(!::uwvm2::utils::control::host_diagnostics::active)return;
#if defined(__cpp_exceptions)
        try
#endif
        {
            constexpr ::std::string_view names[]{"random","stdin","stdout","stderr","realtime","monotonic","process","thread","poll","signal","yield","filesystem"};
            ::std::string key{names[static_cast<unsigned>(kind)]};key+='.';key+=field;
            ::uwvm2::utils::control::host_diagnostics::limited(key,wait);
        }
#if defined(__cpp_exceptions)
        catch(...) {}
#endif
    }
    enum class limit_action : ::std::uint8_t { reject, wait };
    using exit_wait_action = ::uwvm2::utils::thread::process_exit::wait_action;
    enum class budget_failure : unsigned { none, exhausted, request_too_large, cancelled, waiting, clock_unavailable };
    struct operation_limit
    {
        ::std::uint64_t request_bytes{unlimited};
        ::std::uint64_t window_bytes{unlimited};
        ::std::uint64_t window_calls{unlimited};
        ::std::uint64_t total_bytes{unlimited};
        ::std::uint64_t total_calls{unlimited};
        ::std::uint64_t concurrent{unlimited};
        // request, window bytes, window calls, total bytes, total calls, concurrency.
        ::std::array<limit_action, 6u> actions{};
        ::std::array<exit_wait_action, 6u> exit_actions{};
        exit_wait_action native_wait_exit_action{};
    };
    struct resource_limits
    {
        ::std::array<operation_limit, static_cast<unsigned>(budget_kind::count)> operations{};
        ::std::array<::std::uint64_t, 4u> clock_granularity_ns{1u, 1u, 1u, 1u};
        ::std::uint64_t max_poll_subscriptions{unlimited};
        ::std::uint64_t max_poll_wait_ns{unlimited};
        ::std::uint64_t max_args_env_bytes{unlimited};
        ::std::uint64_t filesystem_base_rights{};
        ::std::uint64_t filesystem_inheriting_rights{};
    };
    // One stable process ledger is shared by modules, new environments and code
    // epochs. It is deliberately outside checkpoints and candidate state.
    struct budget_ledger
    {
        struct counter { ::std::uint64_t window_bytes{}, window_calls{}, total_bytes{}, total_calls{}, active{}; };
        ::std::mutex mutex{};
        ::std::chrono::steady_clock::time_point window_start{::uwvm2::utils::host_clock::now()};
        ::std::array<counter, static_cast<unsigned>(budget_kind::count)> counters{};
        ::std::atomic<::std::uint64_t> native_random_calls{};
        ::std::atomic<bool> cancelled{};
        ::std::condition_variable changed{};
#if defined(__linux__)
        struct waiter { int event{-1}; waiter* next{}; };
        waiter* waiters{};
        void notify_waiters_locked() noexcept
        {
            ::std::uint64_t const one{1u};
            for(auto* current{waiters}; current; current=current->next)
            {
                // Nonblocking eventfd. EAGAIN means a notification is already
                // pending; no resource/Guest callback executes under this lock.
                while(::write(current->event, &one, sizeof(one)) < 0 && errno == EINTR) {}
            }
        }
#endif
        void notify_waiters() noexcept
        {
#if defined(__linux__)
            { ::std::lock_guard lock{mutex}; notify_waiters_locked(); }
#endif
            changed.notify_all();
        }
        [[nodiscard]] budget_failure reserve_checked(budget_kind kind, operation_limit const& limit, ::std::uint64_t bytes, unsigned* blocked_fields=nullptr) noexcept
        {
            ::std::lock_guard lock{mutex};
            if(cancelled.load(::std::memory_order_acquire)) { return budget_failure::cancelled; }
            // No amount of window replenishment can admit a request larger than
            // its entire capacity. Do not turn that condition into an endless wait.
            if(bytes > limit.request_bytes || bytes > limit.window_bytes) {
                quota_diagnostic(kind,bytes>limit.request_bytes?"request-bytes":"window-bytes",false);
                return budget_failure::request_too_large;
            }
            auto const now{::uwvm2::utils::host_clock::now()};
            if(!::uwvm2::utils::host_clock::available()) { return budget_failure::clock_unavailable; }
            if(now - window_start >= ::std::chrono::seconds{1})
            { for(auto& item : counters) { item.window_bytes = item.window_calls = 0u; } window_start = now; }
            auto& item{counters[static_cast<unsigned>(kind)]};
            if(item.active == UINT64_MAX) { return budget_failure::exhausted; }
            ::std::array<bool, 6u> blocked{false,
                limit.window_bytes != unlimited && (item.window_bytes > limit.window_bytes || bytes > limit.window_bytes - item.window_bytes),
                limit.window_calls != unlimited && item.window_calls >= limit.window_calls,
                limit.total_bytes != unlimited && (item.total_bytes > limit.total_bytes || bytes > limit.total_bytes - item.total_bytes),
                limit.total_calls != unlimited && item.total_calls >= limit.total_calls,
                limit.concurrent != unlimited && item.active >= limit.concurrent};
            bool waiting{};
            for(unsigned i{}; i < blocked.size(); ++i)
            {
                if(!blocked[i]) { continue; }
                constexpr ::std::string_view fields[]{"request-bytes","window-bytes","window-calls","total-bytes","total-calls","concurrent"};
                bool const can_wait=i!=3u && i!=4u && limit.actions[i]==limit_action::wait &&
                    !(i==2u && limit.window_calls==0u) && !(i==5u && limit.concurrent==0u);
                quota_diagnostic(kind,fields[i],can_wait);
                // Persistent ceilings cannot replenish. Even an invalid native
                // policy cannot turn them into an unbounded impossible wait.
                if(i == 3u || i == 4u || limit.actions[i] != limit_action::wait ||
                   (i == 2u && limit.window_calls == 0u) || (i == 5u && limit.concurrent == 0u))
                { return budget_failure::exhausted; }
                waiting = true;
                if(blocked_fields) { *blocked_fields |= 1u << i; }
            }
            if(waiting) { return budget_failure::waiting; }
            // Unlimited is policy, not permission to wrap accounting. Saturation
            // preserves the fact that a later finite ceiling is exhausted.
            if(item.window_calls != UINT64_MAX) { ++item.window_calls; }
            if(item.total_calls != UINT64_MAX) { ++item.total_calls; }
            ++item.active;
            item.window_bytes = bytes > UINT64_MAX - item.window_bytes ? UINT64_MAX : item.window_bytes + bytes;
            item.total_bytes = bytes > UINT64_MAX - item.total_bytes ? UINT64_MAX : item.total_bytes + bytes;
            return budget_failure::none;
        }
        [[nodiscard]] bool reserve(budget_kind kind, operation_limit const& limit, ::std::uint64_t bytes) noexcept
        { return reserve_checked(kind, limit, bytes) == budget_failure::none; }
        void release(budget_kind kind) noexcept
        {
            { ::std::lock_guard lock{mutex}; --counters[static_cast<unsigned>(kind)].active; }
            notify_waiters();
        }
    };
    inline budget_ledger process_budget_ledger{};
    // Private native lexical scope. A Guest cannot install a callback or alter
    // the real execution lease. The callback observes actual stop/update fences.
    struct host_wait_scope
    {
        using cancellation_check = bool (*)(void*) noexcept;
        inline static thread_local host_wait_scope* current{};
        host_wait_scope* previous{current};
        ::uwvm2::utils::thread::process_exit::operation exit_owner{
            [](void*) noexcept { process_budget_ledger.notify_waiters(); }, nullptr};
        cancellation_check check{};
        void* context{};
        bool pending{};
        ::std::chrono::steady_clock::time_point wake_at{::std::chrono::steady_clock::time_point::max()};
        explicit host_wait_scope(cancellation_check callback, void* object) noexcept : check{callback}, context{object}
        { current = this; }
        ~host_wait_scope() { current = previous; }
        host_wait_scope(host_wait_scope const&) = delete;
        host_wait_scope& operator=(host_wait_scope const&) = delete;
        [[nodiscard]] bool cancelled() const noexcept
        { return exit_owner.cancelled() || process_budget_ledger.cancelled.load(::std::memory_order_acquire) || (check && check(context)); }
    };
    [[nodiscard]] inline bool host_wait_cancelled() noexcept
    {
        return process_budget_ledger.cancelled.load(::std::memory_order_acquire) ||
            (host_wait_scope::current && host_wait_scope::current->cancelled());
    }
    enum class quota_call_result { completed, cancelled, wait_unavailable };
#if defined(__linux__)
    // One eventfd per waiter makes notifications a broadcast; one Guest cannot
    // consume another waiter's concurrency/cancellation wakeup. Timerfds are
    // armed only for actual quota-window replenishment, never a polling sleep.
    class quota_native_wait
    {
        budget_ledger::waiter entry{};
        int timer{-1};
        bool registered{};
    public:
        quota_native_wait() = default;
        quota_native_wait(quota_native_wait const&) = delete;
        ~quota_native_wait()
        {
            if(registered)
            {
                ::std::lock_guard lock{process_budget_ledger.mutex};
                auto** item{&process_budget_ledger.waiters};
                while(*item && *item != &entry) { item=&(*item)->next; }
                if(*item) { *item=entry.next; }
            }
            if(timer >= 0) { ::close(timer); }
            if(entry.event >= 0) { ::close(entry.event); }
        }
        [[nodiscard]] bool ready() const noexcept { return registered; }
        [[nodiscard]] bool prepare() noexcept
        {
            entry.event=::eventfd(0u, EFD_CLOEXEC|EFD_NONBLOCK);
            if(entry.event < 0) { return false; }
            timer=::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
            if(timer < 0) { return false; }
            ::std::lock_guard lock{process_budget_ledger.mutex};
            entry.next=process_budget_ledger.waiters; process_budget_ledger.waiters=&entry; registered=true;
            return true;
        }
        [[nodiscard]] bool wait(::std::chrono::steady_clock::time_point deadline, int cancellation_fd, int native_fd=-1, short native_events=0) noexcept
        {
            if(!::uwvm2::utils::host_clock::arm_timer(timer, deadline)) { return false; }
            ::pollfd watched[4]{{entry.event,POLLIN,0},{timer,POLLIN,0},{cancellation_fd,POLLIN,0},{native_fd,native_events,0}};
            int result{};
            do { result=::poll(watched, 4, -1); } while(result < 0 && errno == EINTR);
            if(result < 0) { return false; }
            for(unsigned i{}; i < 3u; ++i)
            { if(watched[i].revents & (POLLERR|POLLNVAL)) { return false; } }
            for(unsigned i{}; i < 2u; ++i)
            {
                if(watched[i].revents & POLLIN)
                { ::std::uint64_t count{}; while(::read(watched[i].fd,&count,sizeof(count)) < 0 && errno == EINTR) {} }
            }
            return true;
        }
    };
#endif
    // Invoke only at the outer native provider boundary. Each unsuccessful
    // attempt has returned before waiting, so fd/memory/backend locks are gone.
    // A retry rechecks authority, handles and Guest memory and reserves exactly
    // once before any effect. No asynchronous completion or Guest reentry occurs.
    template<typename Invoke>
    [[nodiscard]] quota_call_result invoke_with_quota_wait(Invoke&& invoke,
        host_wait_scope::cancellation_check cancelled, void* context, [[maybe_unused]] int cancellation_fd=-1) noexcept
    {
        host_wait_scope scope{cancelled, context};
#if defined(__linux__)
        quota_native_wait native{};
#endif
        for(;;)
        {
            if(scope.cancelled()) { return quota_call_result::cancelled; }
            scope.pending = false;
            scope.wake_at = ::std::chrono::steady_clock::time_point::max();
            invoke();
            ::uwvm2::utils::control::host_diagnostics::flush_limits();
            if(!scope.pending) { return quota_call_result::completed; }
#if defined(__linux__)
            if(!native.ready())
            {
                if(!native.prepare()) { return quota_call_result::wait_unavailable; }
                // Register before retrying so a released slot cannot be lost
                // between the failed reservation and entering the kernel wait.
                continue;
            }
#endif
            if(scope.cancelled()) { return quota_call_result::cancelled; }
#if defined(__linux__)
            if(!native.wait(scope.wake_at, cancellation_fd)) { return quota_call_result::wait_unavailable; }
#else
            ::std::unique_lock lock{process_budget_ledger.mutex};
            process_budget_ledger.changed.wait_until(lock, scope.wake_at);
#endif
        }
    }
#if defined(__linux__)
    // Wait for actual native I/O readiness, with the same per-call exit owner.
    // The provider retries its nonblocking syscall after this returns; a
    // notification alone never claims bytes were read/written.
    [[nodiscard]] inline quota_call_result wait_for_native_io(int descriptor, short events,
        ::std::chrono::steady_clock::time_point deadline=::std::chrono::steady_clock::time_point::max(), int cancellation_fd=-1) noexcept
    {
        quota_native_wait native{};
        if(!native.prepare()) { return quota_call_result::wait_unavailable; }
        if(host_wait_cancelled()) { return quota_call_result::cancelled; }
        if(!native.wait(deadline, cancellation_fd, descriptor, events))
        { return quota_call_result::wait_unavailable; }
        return host_wait_cancelled() ? quota_call_result::cancelled : quota_call_result::completed;
    }
#endif
    struct budget_reservation
    {
        budget_ledger* ledger{};
        budget_kind kind{};
        budget_failure failure{budget_failure::exhausted};
        explicit budget_reservation(budget_kind requested, resource_limits const& limits, ::std::uint64_t bytes, bool applicable = true) noexcept : kind{requested}
        {
            if(!applicable) { return; }
            unsigned blocked_fields{};
            failure = host_wait_cancelled() ? budget_failure::cancelled :
                process_budget_ledger.reserve_checked(kind, limits.operations[static_cast<unsigned>(kind)], bytes, &blocked_fields);
            if(failure == budget_failure::none)
            {
                ledger = &process_budget_ledger;
                if(host_wait_scope::current && !host_wait_scope::current->exit_owner.select(
                    limits.operations[static_cast<unsigned>(kind)].native_wait_exit_action))
                { ledger->release(kind); ledger=nullptr; failure=budget_failure::cancelled; }
            }
            else if(failure == budget_failure::waiting && host_wait_scope::current)
            {
                auto& scope{*host_wait_scope::current};
                auto exit{exit_wait_action::cancel};
                for(unsigned i{}; i < 6u; ++i)
                { if((blocked_fields & (1u << i)) && limits.operations[static_cast<unsigned>(kind)].exit_actions[i] == exit_wait_action::wait) { exit=exit_wait_action::wait; } }
                if(!scope.exit_owner.select(exit)) { failure=budget_failure::cancelled; return; }
                scope.pending=true;
                // Read the real shared window under the accounting lock. If it
                // rolled while this scope returned, the timer fires immediately.
                auto const& limit{limits.operations[static_cast<unsigned>(kind)]};
                if(limit.window_bytes != unlimited || limit.window_calls != unlimited)
                {
                    ::std::lock_guard lock{process_budget_ledger.mutex};
                    scope.wake_at=process_budget_ledger.window_start + ::std::chrono::seconds{1};
                }
            }
        }
        budget_reservation(budget_reservation const&) = delete;
        budget_reservation& operator=(budget_reservation const&) = delete;
        ~budget_reservation() { if(ledger) { ledger->release(kind); } }
        explicit operator bool() const noexcept { return ledger != nullptr; }
        template<typename Error> [[nodiscard]] Error error() const noexcept
        {
            if(failure == budget_failure::cancelled) { return Error::ecanceled; }
            if(failure == budget_failure::clock_unavailable) { return Error::eio; }
            if(failure == budget_failure::request_too_large) { return Error::e2big; }
            return Error::eagain;
        }
    };
}
