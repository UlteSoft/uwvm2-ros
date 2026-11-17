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
# include <utility>
# include <uwvm2/utils/macro/push_macros.h>
# if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
#  include <uwvm2/utils/thread/stop_token.h>
#  include "execution_journal.h"
# endif
# include <uwvm2/utils/thread/impl.h>
# include <uwvm2/object/memory/linear/impl.h>
# include <uwvm2/runtime/compiler/shared/wasm_threads.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::wasm_threads
{
    // Only 0/1/2 are guest results. Other outcomes must enter the backend's
    // diagnostic/trap path after all wait-registry and allocation locks release.
    enum class wait_status : unsigned
    { notified, not_equal, timed_out, unaligned, out_of_bounds, not_shared, cancelled, unavailable, too_many_waiters };
    struct wait_result { wait_status status{}; ::std::size_t memory_length{}; ::std::uint_least32_t notified{}; };

#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    // VM-owned notification domain; stable native memory identity plus offset
    // preserves aliases between Wasm modules, including across allocator growth.
    // A VM reset cancels and drains execution leases before freeing memories.
    // The wait set may then be reused; no old waiter can observe a recycled key.
    struct wait_domain { ::uwvm2::utils::thread::keyed_wait_set waiters{}; };
    // HOST-only binding. The real execution scope retains the participant,
    // controller and callback for the entire linked wait; guest operands cannot
    // supply these pointers. No ordinary execution installs this policy.
    struct wait_pause_policy
    {
        ::uwvm2::utils::thread::cooperative_pause_domain& control;
        ::uwvm2::utils::thread::cooperative_pause_domain::participant const& participant;
        bool (*on_suspend)(::uwvm2::utils::thread::keyed_wait_set::suspension_borrow const&) noexcept;
        void (*on_linked)() noexcept{};
        void (*on_completed)(::std::uint64_t,::uwvm2::utils::thread::keyed_wait_result) noexcept{};
        void linked() const noexcept
        {
            if(auto* current=current_thread_journal;current&&current->waiting)current->journal->release(*current);
            if(on_linked)on_linked();
        }
        void completed(::uwvm2::utils::thread::keyed_wait_result result) const noexcept
        {if(on_completed)on_completed(participant.identifier(),result);}
        [[nodiscard]] auto register_waker(void* context, void (*wake)(void*) noexcept) const noexcept
        { return participant.register_pause_waker(context, wake); }
        [[nodiscard]] bool requested() const noexcept { return control.wait_interrupt_requested(); }
        [[nodiscard]] bool suspend(::uwvm2::utils::thread::keyed_wait_set::suspension_borrow const& borrow) noexcept
        { return on_suspend != nullptr && on_suspend(borrow) && !control.is_closed(); }
    };
    struct execution_context
    {
        wait_domain& domain;
        ::uwvm2::utils::thread::stop_token cancellation;
        wait_pause_policy* pause{};
    };
    inline constinit thread_local execution_context* current_execution{};

    // Only host entry installs this scope; no guest operation can choose a domain
    // or cancellation token. Nested callbacks restore their previous host scope.
    class execution_scope
    {
        execution_context context;
        execution_context* previous;
    public:
        execution_scope(wait_domain& domain, ::uwvm2::utils::thread::stop_token cancellation, wait_pause_policy* pause=nullptr) noexcept
            : context{domain, ::std::move(cancellation),pause}, previous{current_execution}
        {
            // [live scope.context] remains alive through the entire host entry.
            // [safe              ]
            // ^^ current_execution borrows this host-owned context.
            current_execution = ::std::addressof(context);
        }
        execution_scope(execution_scope const&) = delete;
        execution_scope& operator=(execution_scope const&) = delete;
        ~execution_scope()
        {
            current_execution = previous;
            // [outer live context] or null; restore before this context is destroyed.
        }
    };
#endif

    template<typename Memory>
    [[nodiscard]] inline constexpr bool is_shared(Memory const& memory) noexcept
    {
        if constexpr(requires { memory.sequentially_consistent_size; }) { return memory.sequentially_consistent_size; }
        else { return false; }
    }

    namespace details
    {
        // Explicit bounds are intentional for wait/notify, including mmap: a
        // hardware fault must never escape while holding the wait shard mutex.
        // This helper is not used by ordinary memory loads/stores.
        template<typename Memory, typename Function>
        [[nodiscard]] inline bool access(Memory const& memory, ::std::uint_least64_t offset,
                                         ::std::size_t bytes, ::std::size_t& length_out, Function&& function) noexcept
        {
            auto checked{[&](::std::byte* begin, ::std::size_t length) noexcept
            {
                length_out = length;
                if(begin == nullptr || bytes > length || offset > length - bytes) { return false; }
                // [live memory ... offset: bytes ...] end
                // [safe                            ]
                //                  ^^ pointer formed only after the whole range is proven.
                auto const pointer{begin + static_cast<::std::size_t>(offset)};
                ::std::forward<Function>(function)(pointer);
                return true;
            }};
            if constexpr(Memory::can_mmap)
            { return checked(memory.memory_begin, memory.memory_length_p->load(::std::memory_order_acquire)); }
            else if constexpr(Memory::support_multi_thread)
            {
#if __cpp_lib_atomic_wait >= 201907L
                // Pin ONLY the value comparison/bounds snapshot, never the wait.
                ::uwvm2::object::memory::linear::memory_operation_guard_t pin{memory.growing_flag_p, memory.active_ops_p};
                return checked(memory.memory_begin, memory.memory_length);
#else
                static_assert(!Memory::support_multi_thread);
#endif
            }
            else { return checked(memory.memory_begin, memory.memory_length); }
        }
    }

#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    // A terminal trap/exit closes the journal while other real workers may
    // still be inside wait/notify. This is cancellation, not a broken wait
    // service: reporting unavailable here would turn a handled Guest fault
    // into a second, internal VM crash in a different worker.
    [[nodiscard]] inline wait_result interrupted_journal_wait(thread_journal_context const& context) noexcept
    {
        if(context.cancellation.stop_requested())return {wait_status::cancelled};
        // Another worker may have published refusal but still be delivering
        // the native stop callback. Do not promote that window to a VM fault.
        if(context.journal)
        {
            auto const status=context.journal->observe().status;
            if(status==journal_status::diverged||status==journal_status::cancelled||status==journal_status::allocation_failure)
                return {wait_status::cancelled};
        }
        return {wait_status::unavailable};
    }

    // Shared by ordinary and tagged atomic waits. In particular, a tagged
    // infinite wait must release its execution turn AFTER its genuine native
    // wait node is linked, just like an untagged wait. Replay consumes the same
    // checked comparison and completion without sleeping on a new native node.
    template<::std::size_t Bytes>
    class wait_journal_phase final
    {
        thread_journal_context* context_{current_thread_journal};
        bool replay_{context_&&context_->journal->replaying()};
    public:
        [[nodiscard]] bool active() const noexcept { return context_!=nullptr; }
        [[nodiscard]] bool replaying() const noexcept { return replay_; }
        [[nodiscard]] wait_result interrupted() const noexcept
        {return context_?interrupted_journal_wait(*context_):wait_result{wait_status::unavailable};}
        template<typename Read>
        [[nodiscard]] bool begin(::std::uint64_t offset,::std::uint64_t expected,
            ::std::int64_t timeout_ns,Read&& read) noexcept
        {
            if(!context_)return true;
            try
            {
                auto event=journal_at(journal_kind::wait_begin);event.a=Bytes;event.b=offset;event.c=expected;
                for(unsigned n{};n<8u;++n)event.input.push_back(static_cast<::std::byte>(static_cast<::std::uint64_t>(timeout_ns)>>(n*8u)));
                if(!context_->journal->select(*context_,::std::move(event),false))return false;
                ::std::array<::std::byte,Bytes> comparison{};
                if(!::std::forward<Read>(read)(comparison)||!context_->journal->verify(*context_,comparison))
                {if(!context_->cancellation.stop_requested())context_->journal->refuse();return false;}
                context_->waiting=true;return true;
            }
            catch(...){context_->journal->refuse();return false;}
        }
        [[nodiscard]] wait_result finish(wait_result value) noexcept
        {
            if(!context_)return value;
            if(replay_)context_->journal->release(*context_);
            auto event=journal_at(journal_kind::wait_end);
            if(!context_->journal->select(*context_,::std::move(event)))return interrupted();
            ::std::array<::std::byte,16> bytes{};
            auto put=[&](unsigned at,::std::uint64_t v){for(unsigned n{};n<8u;++n)bytes[at+n]=static_cast<::std::byte>(v>>(8u*n));};
            put(0u,static_cast<unsigned>(value.status));put(8u,value.memory_length);
            if(!context_->journal->value(*context_,bytes))return interrupted();
            if(replay_)
            {
                auto get=[&](unsigned at){::std::uint64_t v{};for(unsigned n{};n<8u;++n)v|=::std::to_integer<::std::uint64_t>(bytes[at+n])<<(8u*n);return v;};
                auto status=get(0u),length=get(8u);
                if(status>static_cast<unsigned>(wait_status::too_many_waiters)||length>SIZE_MAX)
                {context_->journal->refuse();return {wait_status::unavailable};}
                value.status=static_cast<wait_status>(status);value.memory_length=static_cast<::std::size_t>(length);
            }
            return value;
        }
        wait_journal_phase()=default;
        wait_journal_phase(wait_journal_phase const&)=delete;
        ~wait_journal_phase(){if(context_)context_->waiting=false;}
    };
#endif

    template<::std::size_t Bytes, typename Memory>
        requires(Bytes == 4uz || Bytes == 8uz)
    [[nodiscard]] inline wait_result memory_wait(Memory const& memory, bool shared, ::std::uint_least64_t offset,
                                                 ::std::uint64_t expected, ::std::int_least64_t timeout_ns)
    {
        if(offset & (Bytes - 1uz)) { return {wait_status::unaligned}; }
        if(!shared) { return {wait_status::not_shared}; }
        wait_result result{};
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
        auto const execution{current_execution}; // host scope remains alive while guest is suspended
        if(execution == nullptr) { return {wait_status::unavailable}; }
        auto const matches{[&]
        {
            bool matches{};
            bool const bounded{details::access(memory, offset, Bytes, result.memory_length, [&](::std::byte* pointer) noexcept
            {
                auto const loaded{::uwvm2::runtime::compiler::shared::wasm_threads::atomic_load_le<Bytes>(pointer)};
                if constexpr(Bytes == 4uz) { matches = loaded == static_cast<::std::uint32_t>(expected); }
                else { matches = loaded == expected; }
            })};
            if(!bounded) { result.status = wait_status::out_of_bounds; }
            return bounded && matches;
        }};
        wait_journal_phase<Bytes> journal;
        // Preserve the standard bounds trap before opening a wait event.
        if(journal.active()&&!details::access(memory,offset,Bytes,result.memory_length,[](::std::byte*) noexcept{}))
            return {wait_status::out_of_bounds,result.memory_length};
        if(!journal.begin(offset,expected,timeout_ns,[&](auto& comparison) noexcept
        {
            return details::access(memory,offset,Bytes,result.memory_length,[&](::std::byte* p) noexcept
            {
                auto const loaded=::uwvm2::runtime::compiler::shared::wasm_threads::atomic_load_le<Bytes>(p);
                for(::std::size_t i{};i<Bytes;++i)comparison[i]=static_cast<::std::byte>(loaded>>(8u*i));
            });
        }))return journal.interrupted();
        if(journal.replaying())return journal.finish(result);
        auto const outcome{[&]
        {
            if(execution->pause != nullptr)
            {
                // Compare/enqueue once. Suspension drops the shard lock but
                // retains this exact node, FIFO position and original deadline.
                return execution->domain.waiters.wait_suspendable({::std::addressof(memory), offset},
                    timeout_ns, matches, *execution->pause, execution->cancellation, UINT32_MAX).outcome;
            }
            return execution->domain.waiters.wait({::std::addressof(memory), offset}, timeout_ns,
                matches, execution->cancellation, UINT32_MAX);
        }()};
        if(result.status == wait_status::out_of_bounds) { return result; }
        using outcome_type = ::uwvm2::utils::thread::keyed_wait_result;
        switch(outcome)
        {
            case outcome_type::notified: result.status=wait_status::notified; break;
            case outcome_type::not_equal: result.status=wait_status::not_equal; break;
            case outcome_type::timed_out: result.status=wait_status::timed_out; break;
            case outcome_type::cancelled: case outcome_type::closed: result.status=wait_status::cancelled; break;
            case outcome_type::too_many_waiters: result.status=wait_status::too_many_waiters; break;
        }
#else
        result.status=wait_status::unavailable;
#endif
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
        return journal.finish(result);
#else
        return result;
#endif
    }

    template<typename Memory>
    [[nodiscard]] inline wait_result memory_notify(Memory const& memory, ::std::uint_least64_t offset, ::std::uint_least32_t count)
    {
        if(offset & 3u) { return {wait_status::unaligned}; }
        wait_result result{};
        if(!details::access(memory, offset, 4uz, result.memory_length, [](::std::byte*) noexcept {}))
        { result.status=wait_status::out_of_bounds; return result; }
        // Notify is SC even when count is zero or no agent is waiting. The same
        // shard mutex linearizes queue publication/notification with the SC wait load.
        ::std::atomic_thread_fence(::std::memory_order_seq_cst);
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
        auto const execution{current_execution};
        if(execution == nullptr) { return {wait_status::unavailable}; }
        auto* const recording=current_thread_journal;
        bool const replay=recording&&recording->journal->replaying();
        if(recording)
        {
            auto event=journal_at(journal_kind::notify);event.a=offset;event.b=count;
            if(!recording->journal->select(*recording,::std::move(event),false))return interrupted_journal_wait(*recording);
        }
        if(!replay)result.notified=static_cast<::std::uint_least32_t>(execution->domain.waiters.notify({::std::addressof(memory),offset},count));
        if(recording)
        {
            ::std::array<::std::byte,4> value{};
            for(unsigned n{};n<4u;++n)value[n]=static_cast<::std::byte>(result.notified>>(8u*n));
            if(!recording->journal->value(*recording,value))return interrupted_journal_wait(*recording);
            if(replay)
            {
                result.notified=0u;for(unsigned n{};n<4u;++n)result.notified|=::std::to_integer<::std::uint32_t>(value[n])<<(8u*n);
                if(result.notified>count){recording->journal->refuse();return {wait_status::unavailable};}
            }
        }
#endif
        return result;
    }
    // The full memory64 sum must be checked before choosing a wait key. A
    // wrapped low address must never compare, register or wake a different key.
    // The VM host-entry lease owns memory across suspension; this helper never
    // retains an allocator pin or a pointer into its movable byte allocation.
    template<unsigned Operation, typename Memory>
    [[nodiscard]] inline wait_result memory_wait_notify64(Memory const& memory, ::std::uint64_t static_offset,
        ::std::uint64_t address, ::std::uint64_t expected, ::std::int_least64_t timeout_ns)
    {
        static_assert(Operation <= 2u);
        constexpr ::std::size_t bytes{Operation == 2u ? 8uz : 4uz};
        auto const effective{address + static_offset};
        if(effective < address) [[unlikely]]
        {
            if(effective & (bytes - 1uz)) { return {wait_status::unaligned}; }
            wait_result result{wait_status::out_of_bounds};
            // Borrow only the length snapshot, with a bounded zero-width access.
            // No wait-shard lock or byte pointer survives this call.
            (void)details::access(memory, 0u, 0uz, result.memory_length, [](::std::byte*) noexcept {});
            return result;
        }
        if constexpr(Operation == 0u) { return memory_notify(memory, effective, static_cast<::std::uint32_t>(expected)); }
        else { return memory_wait<bytes>(memory, is_shared(memory), effective, expected, timeout_ns); }
    }
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
