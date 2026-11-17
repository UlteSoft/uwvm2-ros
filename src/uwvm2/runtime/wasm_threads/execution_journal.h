// Host-owned deterministic execution history for private WASI Threads worlds.
// Wire data carries no native addresses, handles or execution authority.
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <mutex>
#include <span>
#include <utility>
#include <vector>
#include <uwvm2/utils/thread/stop_token.h>
#if defined(__linux__)
#include <unistd.h>
#endif
#endif
namespace uwvm2::runtime::wasm_threads
{
    enum class journal_kind : ::std::uint32_t
    { origin=1, enter, instruction, leave, spawn, host_begin, memory_read, memory_write, host_end,
      wait_begin, wait_end, notify, entropy, exit_begin, exit_end, grow, trap, table_grow, endpoint };
    // Stable Guest fault classes, independent of interpreter/JIT internal enums.
    enum class journal_fault : ::std::uint32_t { unreachable=120u, execution=121u, memtag=122u };
    enum class journal_status : unsigned { idle, recording, replaying, endpoint, diverged, cancelled, allocation_failure };
    struct journal_event
    {
        journal_kind kind{};
        ::std::uint64_t worker{},module{},function{},offset{},a{},b{},c{};
        ::uwvm2::utils::container::std_vector<::std::byte> input{},output{};
        [[nodiscard]] bool matches(journal_event const& other) const noexcept
        { return kind==other.kind && worker==other.worker && module==other.module && function==other.function &&
            offset==other.offset && a==other.a && b==other.b && c==other.c && input==other.input; }
    };
    struct threaded_module_image
    {
        ::std::uint64_t identifier{};
        ::uwvm2::utils::container::std_vector<::std::byte> name,wasm;
    };
    struct threaded_execution_history
    {
        ::uwvm2::utils::container::std_vector<journal_event> events;
        // A completed terminal main has no native continuation. A coherent
        // prefix ends before an instruction; playback remains stopped there.
        bool terminal{},retained_native{};
        ::std::uint64_t replay_until{}; // 0: complete known interval; otherwise an exact event-prefix checkpoint.
    };
    struct journal_observation
    {
        journal_status status{journal_status::idle};
        ::std::uint64_t events{},cursor{},running{};
    };
    class threaded_execution_journal;
    struct thread_journal_context
    {
        threaded_execution_journal* journal{};
        ::std::uint64_t worker{};
        ::std::size_t selected{SIZE_MAX};
        journal_event location{};
        // The live debugger uses private-world indices. Only wire events use
        // the stable, source-name ordered module identity below.
        ::std::span<::std::uint64_t const> logical_modules{};
        [[nodiscard]] ::std::uint64_t logical_module(::std::uint64_t index) const noexcept
        {return index<logical_modules.size()?logical_modules[static_cast<::std::size_t>(index)]:index;}
        ::uwvm2::utils::thread::stop_token cancellation{};
        // Called only outside the journal mutex. The authentic worker polls its
        // own private pause domain, never an address supplied by the recording.
        void* native{};
        bool (*poll)(void*) noexcept{};
        bool host{},waiting{},has_location{};
        thread_journal_context* queued_next{}; bool queued{};
        bool (*guest_exiting)() noexcept{};
        bool (*return_host)(void*) noexcept{};
        bool (*retain_host)(void*) noexcept{};
    };
    inline constinit thread_local thread_journal_context* current_thread_journal{};
    class thread_journal_scope final
    {
        thread_journal_context* previous_{};
    public:
        explicit thread_journal_scope(thread_journal_context& current) noexcept:
            previous_{current_thread_journal}{current_thread_journal=&current;}
        ~thread_journal_scope(){current_thread_journal=previous_;}
        thread_journal_scope(thread_journal_scope const&)=delete;
    };
    [[nodiscard]] inline bool valid_execution_history(threaded_execution_history const& history) noexcept
    {
        if(history.events.size()<2u||history.events.front().kind!=journal_kind::origin||history.events.front().worker!=1u||
           history.replay_until>history.events.size()||history.events.front().input.size()!=64u||history.events.front().output.size()>PTRDIFF_MAX||(history.terminal&&history.retained_native))return false;
        try
        {
            struct worker {::std::uint64_t id{},tid{};unsigned phase{};}; // 0 runnable, 1 native import, 2 wait, 3 left
            ::uwvm2::utils::container::std_vector<worker> workers;
            struct birth {::std::uint64_t tid{},argument{};bool entered{};};
            ::uwvm2::utils::container::std_vector<birth> births;
            bool main_left{};
            for(::std::size_t index=1u;index<history.events.size();++index)
            {
                auto const& event=history.events[index];
                if(!event.worker||static_cast<unsigned>(event.kind)<2u||static_cast<unsigned>(event.kind)>static_cast<unsigned>(journal_kind::endpoint)||
                   event.input.size()>PTRDIFF_MAX||event.output.size()>PTRDIFF_MAX)return false;
                auto word=[&](unsigned start,unsigned count){::std::uint64_t value{};
                    for(unsigned n{};n<count;++n)value|=::std::to_integer<::std::uint64_t>(event.output[start+n])<<(8u*n);return value;};
                switch(event.kind)
                {
                    case journal_kind::instruction:
                        if(!event.input.empty()||!event.output.empty()||event.a||event.b||event.c)return false;break;
                    case journal_kind::spawn:
                        if(!event.input.empty()||event.output.size()!=4u||event.a>UINT32_MAX||event.c)return false;break;
                    case journal_kind::host_begin:
                        if(!event.output.empty()||(event.b>>1u)>UINT32_MAX)return false;break;
                    case journal_kind::host_end:
                        if(!event.input.empty()||event.output.size()!=event.a||event.b||event.c)return false;break;
                    case journal_kind::memory_read:case journal_kind::memory_write:
                    {
                        bool const write=event.kind==journal_kind::memory_write;
                        if(!event.input.empty()||event.c>(SIZE_MAX-1u)/(write?2u:1u)||
                           event.output.size()!=1u+event.c*(write?2u:1u)||event.output[0]> ::std::byte{1})return false;
                        break;
                    }
                    case journal_kind::wait_begin:
                        if((event.a!=4u&&event.a!=8u)||event.input.size()!=8u||event.output.size()!=event.a)return false;break;
                    case journal_kind::wait_end:
                        if(!event.input.empty()||event.output.size()!=16u||event.a||event.b||event.c)return false;break;
                    case journal_kind::notify:
                        if(!event.input.empty()||event.output.size()!=4u||event.b>UINT32_MAX||word(0,4)>event.b||event.c)return false;break;
                    case journal_kind::entropy:
                        if(!event.input.empty()||event.output.size()!=2u||event.output[0]> ::std::byte{1}||event.a||event.b||event.c)return false;break;
                    case journal_kind::enter:
                        if(!event.output.empty()||event.module||event.function||event.offset||event.b>UINT32_MAX)return false;break;
                    case journal_kind::leave:
                        if(!event.input.empty()||event.b||event.c)return false;break;
                    case journal_kind::exit_begin:
                        if(!event.input.empty()||event.output.size()!=4u||event.a>UINT32_MAX||event.b||event.c)return false;break;
                    case journal_kind::exit_end:
                        if(!event.input.empty()||!event.output.empty()||event.a>UINT32_MAX||event.b||event.c)return false;break;
                    case journal_kind::trap:
                        if(!event.input.empty()||!event.output.empty()||event.a<120u||event.a>122u||event.b||event.c)return false;break;
                    case journal_kind::table_grow:
                        if(!event.input.empty()||event.output.size()!=8u||event.a>UINT32_MAX||
                           (word(0,8)!=UINT64_MAX&&word(0,8)!=event.c)){return false;}
                        break;
                    case journal_kind::grow:
                        if(!event.input.empty()||event.output.size()!=8u||
                           (word(0,8)!=UINT64_MAX&&word(0,8)!=event.b))return false;
                        break;
                    default:return false;
                }
                auto found=::std::find_if(workers.begin(),workers.end(),[&](auto const& w){return w.id==event.worker;});
                if(event.kind==journal_kind::enter)
                {
                    if(found!=workers.end()||main_left||event.a>=(1u<<29u)||
                       (workers.empty()?(event.worker!=1u||event.a!=0u):event.a==0u))return false;
                    for(auto const& w:workers)if(w.tid==event.a&&w.phase!=3u)return false;
                    if(!workers.empty())
                    {
                        if(!event.input.empty())return false;
                        auto created=::std::find_if(births.begin(),births.end(),[&](auto const& b)
                            {return !b.entered&&b.tid==event.a&&b.argument==event.b;});
                        if(created==births.end())return false;
                        created->entered=true;
                    }
                    workers.push_back({event.worker,event.a,0u});continue;
                }
                if(found==workers.end()||found->phase==3u)return false;
                switch(event.kind)
                {
                    case journal_kind::spawn:
                    {
                        if(found->phase||main_left)return false;
                        auto bits=static_cast<::std::uint32_t>(word(0,4));auto tid=static_cast<::std::int32_t>(bits);
                        if(tid==0||tid< -1||tid>=(1<<29))return false;
                        if(tid>0)
                        {
                            for(auto const& b:births)if(!b.entered&&b.tid==bits)return false;
                            for(auto const& w:workers)if(w.tid==bits&&w.phase!=3u)return false;
                            births.push_back({bits,event.a,false});
                        }
                        break;
                    }
                    case journal_kind::instruction:case journal_kind::notify:case journal_kind::entropy:case journal_kind::grow:case journal_kind::table_grow:
                        if(found->phase||main_left)return false;break;
                    case journal_kind::host_begin:if(found->phase||main_left)return false;found->phase=1u;break;
                    case journal_kind::memory_read:case journal_kind::memory_write:
                        if(found->phase!=1u)return false;break;
                    case journal_kind::host_end:if(found->phase!=1u)return false;found->phase=0u;break;
                    case journal_kind::wait_begin:if(found->phase||main_left)return false;found->phase=2u;break;
                    case journal_kind::wait_end:if(found->phase!=2u)return false;found->phase=0u;break;
                    case journal_kind::exit_begin:if(found->phase||main_left)return false;found->phase=4u;break;
                    case journal_kind::exit_end:if(found->phase!=4u||main_left)return false;found->phase=3u;main_left=true;break;
                    case journal_kind::trap:
                        if(main_left)return false;found->phase=3u;main_left=true;break;
                    case journal_kind::leave:
                        if(found->phase||event.a!=found->tid)return false;found->phase=3u;if(found->tid==0u)main_left=true;break;
                    default:return false;
                }
            }
            bool unknown{};for(auto const& w:workers)unknown|=w.phase==1u||w.phase==4u;
            return main_left==history.terminal&&((!main_left&&unknown)==history.retained_native);
        }
        catch(...){return false;}
    }
    class threaded_execution_journal final
    {
        mutable ::std::mutex mutex_{};
        ::std::condition_variable changed_{};
        threaded_execution_history history_{};
        // Mode is immutable even when delivery fails. A concurrent refusal
        // must never make a replaying import/entropy adapter think it is a
        // recording and fall through to a real native provider.
        bool const playback_{};
        bool entry_held_{};
        journal_status mode_{journal_status::recording};
        ::std::size_t cursor_{};
        ::std::uint64_t running_{};
        ::std::size_t limit_{};
        thread_journal_context *queue_head_{}, *queue_tail_{};
        void enqueue_locked(thread_journal_context& value) noexcept
        {
            if(value.queued)return;value.queued=true;value.queued_next=nullptr;
            if(queue_tail_)queue_tail_->queued_next=&value;else queue_head_=&value;
            queue_tail_=&value;
        }
        void dequeue_locked(thread_journal_context& value) noexcept
        {
            if(!value.queued)return;
            thread_journal_context* previous{};
            for(auto* item=queue_head_;item;item=item->queued_next)
            {
                if(item!=&value){previous=item;continue;}
                if(previous)previous->queued_next=item->queued_next;else queue_head_=item->queued_next;
                if(queue_tail_==item)queue_tail_=previous;
                value.queued=false;value.queued_next=nullptr;changed_.notify_all();return;
            }
        }
#if defined(__linux__)
        decltype(::getpid()) const process_{::getpid()};
#endif
        [[nodiscard]] bool original_process() const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        ::std::weak_ptr<void> failure_owner_{};
        void (*failure_handler_)(void*) noexcept{};
        bool failure_notified_{};
        void fail_locked(journal_status status) noexcept
        {if(!failed_locked())mode_=status;running_=0u;changed_.notify_all();}
        void notify_failure() noexcept
        {
            ::std::shared_ptr<void> owner;void (*handler)(void*) noexcept{};
            {
                ::std::lock_guard lock{mutex_};
                if(!failed_locked()||failure_notified_)return;
                failure_notified_=true;owner=failure_owner_.lock();handler=failure_handler_;
            }
            // Real cancellation may acquire a wait-queue mutex. A waiter can
            // release its journal turn while that same queue mutex is held.
            // Never invoke stop callbacks with the journal mutex held.
            if(owner&&handler)handler(owner.get());
        }
        struct failure_notification
        {
            threaded_execution_journal& owner;
            ~failure_notification(){owner.notify_failure();}
        };
        [[nodiscard]] bool failed_locked() const noexcept
        {return mode_==journal_status::diverged || mode_==journal_status::cancelled || mode_==journal_status::allocation_failure;}
    public:
        threaded_execution_journal()=default;
        explicit threaded_execution_journal(bool hold_entry) noexcept:entry_held_{hold_entry}{}
        explicit threaded_execution_journal(threaded_execution_history history):history_{::std::move(history)},playback_{true},mode_{journal_status::replaying},
            limit_{history_.replay_until?static_cast<::std::size_t>(history_.replay_until):history_.events.size()}{}
        threaded_execution_journal(threaded_execution_journal const&)=delete;
        [[nodiscard]] bool bind_failure_owner(::std::shared_ptr<void> const& owner,void(*handler)(void*) noexcept) noexcept
        {
            if(!original_process()||!owner||!handler)return false;::std::lock_guard lock{mutex_};
            if(running_||cursor_||failure_handler_)return false;failure_owner_=owner;failure_handler_=handler;return true;
        }
        [[nodiscard]] bool replaying() const noexcept
        {return original_process()&&playback_;}
        // Cold terminal inspection only: no counter or scan is added to the
        // ordinary instruction path. A virtual replay has no native operations.
        [[nodiscard]] bool has_unresolved_native_imports() const noexcept
        {
            if(!original_process()||playback_)return false;
            ::std::lock_guard lock{mutex_};::std::size_t pending{};
            for(auto const& event:history_.events)
            {
                if(event.kind==journal_kind::host_begin||event.kind==journal_kind::exit_begin)++pending;
                else if(event.kind==journal_kind::host_end||event.kind==journal_kind::exit_end)
                {if(!pending)return true;--pending;}
            }
            return pending!=0u;
        }
        [[nodiscard]] journal_observation observe() const noexcept
        {if(!original_process())return {journal_status::diverged};::std::lock_guard lock{mutex_};return {mode_,history_.events.size(),cursor_,running_};}
        [[nodiscard]] bool origin(::std::span<::std::byte const> identity,::std::span<::std::byte> allocation_tags={}) noexcept
        {
            if(!original_process())return false;
            try
            {
                ::std::lock_guard lock{mutex_};
                if(running_||cursor_||failed_locked())return false;
                journal_event event{};event.kind=journal_kind::origin;event.worker=1u;event.input.assign(identity.begin(),identity.end());
                if(mode_==journal_status::recording)
                {if(!history_.events.empty())return false;event.output.assign(allocation_tags.begin(),allocation_tags.end());history_.events.push_back(::std::move(event));return true;}
                if(!valid_execution_history(history_)||!history_.events[0].matches(event)||history_.events[0].output.size()!=allocation_tags.size())return false;
                if(!allocation_tags.empty())::std::copy(history_.events[0].output.begin(),history_.events[0].output.end(),allocation_tags.begin());
                cursor_=1u;return true;
            }
            catch(...){return false;}
        }
        [[nodiscard]] bool continue_recorded_interval() noexcept
        {
            if(!original_process())return false;::std::lock_guard lock{mutex_};
            if(!playback_ && entry_held_ && !running_ && !failed_locked())
            {entry_held_=false;changed_.notify_all();return true;}
            if(running_||mode_!=journal_status::endpoint||cursor_>=history_.events.size())return false;
            limit_=history_.events.size();mode_=journal_status::replaying;changed_.notify_all();return true;
        }
        void cancel() noexcept
        {if(!original_process())return;failure_notification notify{*this};::std::lock_guard lock{mutex_};if(!failed_locked())fail_locked(journal_status::cancelled);}
        void refuse() noexcept
        {if(!original_process())return;failure_notification notify{*this};::std::lock_guard lock{mutex_};fail_locked(journal_status::diverged);}
        void release(thread_journal_context& actual) noexcept
        {
            if(!original_process())return;
            ::std::lock_guard lock{mutex_};
            if(running_==actual.worker){running_=0u;changed_.notify_all();}
            actual.selected=SIZE_MAX;
        }
        [[nodiscard]] bool select(thread_journal_context& actual,journal_event event,bool relinquish=true) noexcept
        {
            if(!original_process()||actual.journal!=this||!actual.worker)return false;
            event.worker=actual.worker;
            failure_notification notify{*this};
            try
            {
                ::std::unique_lock lock{mutex_};
                // A genuine recording stop can hand the turn to another
                // worker between an opcode and its nested spawn/wait/entropy
                // event. Replay must reproduce that hand-off even when this
                // sub-operation normally keeps its opcode's turn. Keeping it
                // while waiting for the other recorded worker deadlocks both.
                bool const replay_handoff=mode_==journal_status::replaying&&cursor_<limit_&&
                    history_.events[cursor_].worker!=actual.worker;
                if((relinquish||replay_handoff)&&running_==actual.worker){running_=0u;changed_.notify_all();}
                actual.selected=SIZE_MAX;
                if(mode_==journal_status::recording&&running_!=actual.worker)enqueue_locked(actual);
                for(;;)
                {
                    if(failed_locked()||actual.cancellation.stop_requested()){dequeue_locked(actual);return false;}
                    // The archive's worker identifier is logical data. Bind it
                    // only at an authentic new ChildWork entry with the exact
                    // recorded TID/start argument/entry packet; never to a raw
                    // native thread ID or a caller-supplied native address.
                    if(mode_==journal_status::replaying&&event.kind==journal_kind::enter&&cursor_<limit_)
                    {
                        auto const& saved=history_.events[cursor_];
                        if(saved.kind==journal_kind::enter)
                        {
                            auto expected=event;expected.worker=saved.worker;
                            if(saved.matches(expected)){actual.worker=saved.worker;event.worker=saved.worker;}
                            else if(saved.a==event.a)
                            {fail_locked(journal_status::diverged);return false;}
                        }
                    }
                    bool const exiting=!actual.host&&actual.guest_exiting&&actual.guest_exiting();
                    if(exiting)dequeue_locked(actual);
                    bool eligible=!exiting&&!(entry_held_&&event.kind==journal_kind::instruction)&&((mode_==journal_status::recording?(running_==actual.worker||queue_head_==&actual):
                        false)||
                        (cursor_<limit_&&history_.events[cursor_].worker==actual.worker));
                    if(!exiting&&(!running_||running_==actual.worker) &&
                       (mode_!=journal_status::recording||eligible) &&
                       (!eligible||!actual.host||!actual.return_host||actual.return_host(actual.native)))
                    {
                        if(mode_==journal_status::recording)
                        {
                            history_.events.push_back(::std::move(event));actual.selected=history_.events.size()-1u;dequeue_locked(actual);
                            running_=actual.worker;return true;
                        }
                        if(cursor_==limit_)
                        {
                            // No unrecorded instruction or external call runs
                            // after a finite prefix. The native caller can pause
                            // and inspect, or retire, this reconstructed world.
                            mode_=journal_status::endpoint;
                        }
                        else if(history_.events[cursor_].worker==actual.worker)
                        {
                            if(!history_.events[cursor_].matches(event))
                            {fail_locked(journal_status::diverged);return false;}
                            actual.selected=cursor_++;running_=actual.worker;return true;
                        }
                    }
                    // Cancellation and debugger control are native capabilities,
                    // not history fields. Poll without the journal lock, including
                    // at a completed replay prefix; this keeps all-stop usable.
                    lock.unlock();bool const live=!actual.poll||actual.poll(actual.native);lock.lock();
                    if(!live){dequeue_locked(actual);return false;}
                    changed_.wait_for(lock,::std::chrono::milliseconds{2});
                }
            }
            catch(...){::std::lock_guard lock{mutex_};dequeue_locked(actual);fail_locked(journal_status::allocation_failure);return false;}
        }
        [[nodiscard]] bool value(thread_journal_context& actual,::std::span<::std::byte> output) noexcept
        {
            if(!original_process())return false;
            failure_notification notify{*this};
            try
            {
                ::std::lock_guard lock{mutex_};
                if(failed_locked()||running_!=actual.worker||actual.selected>=history_.events.size())return false;
                auto& saved=history_.events[actual.selected].output;
                if(mode_==journal_status::recording){saved.assign(output.begin(),output.end());return true;}
                if(saved.size()!=output.size()){fail_locked(journal_status::diverged);return false;}
                if(!output.empty())::std::memcpy(output.data(),saved.data(),output.size());return true;
            }
            catch(...){::std::lock_guard lock{mutex_};fail_locked(journal_status::allocation_failure);return false;}
        }
        [[nodiscard]] bool verify(thread_journal_context& actual,::std::span<::std::byte const> output) noexcept
        {
            if(!original_process())return false;
            failure_notification notify{*this};
            try
            {
                ::std::lock_guard lock{mutex_};
                if(failed_locked()||running_!=actual.worker||actual.selected>=history_.events.size())return false;
                auto& saved=history_.events[actual.selected].output;
                if(mode_==journal_status::recording){saved.assign(output.begin(),output.end());return true;}
                if(saved.size()==output.size()&&::std::equal(output.begin(),output.end(),saved.begin()))return true;
                fail_locked(journal_status::diverged);return false;
            }
            catch(...){::std::lock_guard lock{mutex_};fail_locked(journal_status::allocation_failure);return false;}
        }
        [[nodiscard]] bool capture(threaded_execution_history& out,bool terminal,bool retained=false) const noexcept
        {
            if(!original_process())return false;
            try
            {
                ::std::lock_guard lock{mutex_};
                if(running_||failed_locked())return false;
                auto copy=history_;
                if(mode_==journal_status::recording)
                {
                    copy.terminal=terminal;
                    // A participant retained while waiting to ADMIT host_begin
                    // has no unknown external result. Only admitted, unmatched
                    // host/exit events require the original process capability.
                    ::std::size_t pending{};
                    for(auto const& event:copy.events)
                    {
                        if(event.kind==journal_kind::host_begin||event.kind==journal_kind::exit_begin)++pending;
                        else if(event.kind==journal_kind::host_end||event.kind==journal_kind::exit_end)
                        {if(!pending)return false;--pending;}
                    }
                    copy.retained_native=!terminal&&pending!=0u;
                    if(copy.retained_native&&!retained)return false;
                }
                else copy.replay_until=cursor_;
                if(!valid_execution_history(copy))return false;out=::std::move(copy);return true;
            }
            catch(...){return false;}
        }
        // Only a bound, authentic host-import adapter consumes memory/result
        // phases. This operation never obtains authority from a recorded address.
        [[nodiscard]] bool peek(thread_journal_context& actual,journal_event& out) noexcept
        {
            if(!original_process())return false;
            failure_notification notify{*this};
            try
            {
                ::std::unique_lock lock{mutex_};
                if(running_==actual.worker){running_=0u;changed_.notify_all();}
                actual.selected=SIZE_MAX;
                for(;;)
                {
                    if(failed_locked()||actual.cancellation.stop_requested())return false;
                    if(!running_&&cursor_<limit_&&history_.events[cursor_].worker==actual.worker)
                    {out=history_.events[cursor_];return true;}
                    if(cursor_==limit_)mode_=journal_status::endpoint;
                    lock.unlock();bool const live=!actual.poll||actual.poll(actual.native);lock.lock();
                    if(!live)return false;
                    changed_.wait_for(lock,::std::chrono::milliseconds{2});
                }
            }
            catch(...){::std::lock_guard lock{mutex_};fail_locked(journal_status::allocation_failure);return false;}
        }
    };
    [[nodiscard]] inline journal_event journal_at(journal_kind kind) noexcept
    {
        journal_event result{};result.kind=kind;
        if(auto const* actual=current_thread_journal)
        {result.worker=actual->worker;result.module=actual->logical_module(actual->location.module);result.function=actual->location.function;result.offset=actual->location.offset;}
        return result;
    }
    // memory.grow can fail because of current host allocation pressure. Its
    // recorded -1 must not become success merely because replay has more RAM.
    // A recorded success still requires a genuine checked native allocation;
    // failure today stops replay instead of inventing a writable memory extent.
    template<typename Grow>
    [[nodiscard]] inline ::std::uint64_t journal_memory_grow(::std::uint64_t delta,
        ::std::uint64_t old_pages,::std::uint64_t maximum,Grow&& actual) noexcept
    {
        auto* context=current_thread_journal;if(!context)return actual();
        auto& journal=*context->journal;
        auto event=journal_at(journal_kind::grow);event.a=delta;event.b=old_pages;event.c=maximum;
        if(!journal.select(*context,::std::move(event),false))return UINT64_MAX;
        ::std::array<::std::byte,8> bytes{};
        bool const replay=journal.replaying();
        if(replay)
        {
            if(!journal.value(*context,bytes))return UINT64_MAX;
            ::std::uint64_t recorded{};
            for(unsigned n{};n<8u;++n)recorded|=::std::to_integer<::std::uint64_t>(bytes[n])<<(8u*n);
            if(recorded==UINT64_MAX)return recorded;
        }
        ::std::uint64_t const result=actual();
        for(unsigned n{};n<8u;++n)bytes[n]=static_cast<::std::byte>(result>>(8u*n));
        if(!journal.verify(*context,bytes))return UINT64_MAX;
        return result;
    }
    // Table allocation has the same nondeterministic host-failure boundary as
    // memory.grow. Preserve a recorded failure without retrying allocation, and
    // require genuine storage for a recorded success. Indices/results stay u64
    // even on 32-bit hosts; only the checked table operation may narrow them.
    template<typename Grow>
    [[nodiscard]] inline ::std::uint64_t journal_table_grow(::std::uint32_t table,
        ::std::uint64_t delta,::std::uint64_t old_size,Grow&& actual) noexcept
    {
        auto* context=current_thread_journal;if(!context)return actual();
        auto& journal=*context->journal;auto event=journal_at(journal_kind::table_grow);
        event.a=table;event.b=delta;event.c=old_size;
        if(!journal.select(*context,::std::move(event),false))return UINT64_MAX;
        ::std::array<::std::byte,8> bytes{};
        if(journal.replaying())
        {
            if(!journal.value(*context,bytes))return UINT64_MAX;
            ::std::uint64_t recorded{};
            for(unsigned n{};n<8u;++n)recorded|=::std::to_integer<::std::uint64_t>(bytes[n])<<(8u*n);
            if(recorded==UINT64_MAX)return recorded;
        }
        auto const result=actual();
        for(unsigned n{};n<8u;++n)bytes[n]=static_cast<::std::byte>(result>>(8u*n));
        return journal.verify(*context,bytes)?result:UINT64_MAX;
    }
    // Canonical payload for the unified archive. The outer archive supplies
    // provenance/disclosure/checksum; this decoder independently bounds every
    // allocation by the physical input. Decoding never configures execution.
    [[nodiscard]] inline bool encode_execution_history(threaded_execution_history const& history,
        ::uwvm2::utils::container::std_vector<::std::byte>& output) noexcept
    {
        if(!valid_execution_history(history))return false;
        try
        {
            ::uwvm2::utils::container::std_vector<::std::byte> bytes;
            auto put=[&](::std::uint64_t value){for(unsigned n{};n<8u;++n)bytes.push_back(static_cast<::std::byte>(value>>(8u*n)));};
            put(0x00374c4e4a485455ull);put((history.terminal?1u:0u)|(history.retained_native?2u:0u));put(history.events.size());put(history.replay_until);
            for(auto const& e:history.events)
            {
                put(static_cast<unsigned>(e.kind));put(e.worker);put(e.module);put(e.function);put(e.offset);put(e.a);put(e.b);put(e.c);
                put(e.input.size());put(e.output.size());
                bytes.insert(bytes.end(),e.input.begin(),e.input.end());bytes.insert(bytes.end(),e.output.begin(),e.output.end());
            }
            output=::std::move(bytes);return true;
        }
        catch(...){return false;}
    }
    [[nodiscard]] inline bool decode_execution_history(::std::span<::std::byte const> bytes,
        threaded_execution_history& output) noexcept
    {
        try
        {
            ::std::size_t position{};
            auto get=[&](::std::uint64_t& out){if(bytes.size()-position<8u)return false;out=0u;
                for(unsigned n{};n<8u;++n)out|=::std::to_integer<::std::uint64_t>(bytes[position++])<<(8u*n);return true;};
            ::std::uint64_t magic{},flags{},count{},target{};
            if(!get(magic)||magic!=0x00374c4e4a485455ull||!get(flags)||flags>3u||!get(count)||!get(target)||target>count||count>(bytes.size()-position)/80u)return false;
            threaded_execution_history decoded{};decoded.terminal=(flags&1u)!=0u;decoded.retained_native=(flags&2u)!=0u;decoded.replay_until=target;
            decoded.events.reserve(static_cast<::std::size_t>(count));
            for(::std::uint64_t i{};i<count;++i)
            {
                journal_event e{};::std::uint64_t kind{},input{},output_bytes{};
                if(!get(kind)||kind<1u||kind>static_cast<unsigned>(journal_kind::endpoint)||!get(e.worker)||!get(e.module)||!get(e.function)||
                   !get(e.offset)||!get(e.a)||!get(e.b)||!get(e.c)||!get(input)||!get(output_bytes)||
                   input>bytes.size()-position||output_bytes>bytes.size()-position-input)return false;
                e.kind=static_cast<journal_kind>(kind);
                e.input.assign(bytes.begin()+position,bytes.begin()+position+static_cast<::std::size_t>(input));position+=static_cast<::std::size_t>(input);
                e.output.assign(bytes.begin()+position,bytes.begin()+position+static_cast<::std::size_t>(output_bytes));position+=static_cast<::std::size_t>(output_bytes);
                decoded.events.push_back(::std::move(e));
            }
            if(position!=bytes.size()||!valid_execution_history(decoded))return false;
            output=::std::move(decoded);return true;
        }
        catch(...){return false;}
    }
    // A recording phase surrounds the real guest-memory operation, outside any
    // tag/grow lease. Native waits and provider locks do not own a Wasm turn.
    class thread_journal_memory_phase final
    {
        thread_journal_context* context_{};
        ::uwvm2::utils::container::std_vector<::std::byte> bytes_{};
        bool write_{},entered_{},finished_{};
    public:
        thread_journal_memory_phase(::std::size_t index,::std::uint64_t address,::std::size_t count,bool write) noexcept:
            context_{current_thread_journal},write_{write}
        {
            if(!context_){entered_=true;return;}
            try
            {
                auto event=journal_at(write?journal_kind::memory_write:journal_kind::memory_read);
                event.a=index==SIZE_MAX?UINT64_MAX:index;event.b=address;event.c=count;
                if(count>(SIZE_MAX-1u)/(write?2u:1u))return;
                bytes_.resize(1u+count*(write?2u:1u));
                entered_=context_->host&&context_->journal->select(*context_,::std::move(event));
            }
            catch(...){context_->journal->refuse();}
        }
        [[nodiscard]] bool entered() const noexcept{return entered_;}
        [[nodiscard]] bool active() const noexcept{return context_!=nullptr;}
        void before(::std::span<::std::byte const> value) noexcept
        {if(context_&&entered_&&write_&&!value.empty()&&value.size()==(bytes_.size()-1u)/2u)::std::memcpy(bytes_.data()+1u,value.data(),value.size());}
        [[nodiscard]] bool finish(bool success,::std::span<::std::byte const> value) noexcept
        {
            if(!context_)return success;
            if(!entered_||finished_)return false;finished_=true;
            auto count=(bytes_.size()-1u)/(write_?2u:1u);
            if(success&&value.size()!=count){context_->journal->refuse();return false;}
            bytes_[0]=static_cast<::std::byte>(success);
            if(success&&!value.empty())::std::memcpy(bytes_.data()+1u+(write_?count:0u),value.data(),value.size());
            bool const accepted=context_->journal->verify(*context_,bytes_);
            bool const retained=!context_->retain_host||context_->retain_host(context_->native);
            context_->journal->release(*context_);
            return success&&accepted&&retained;
        }
        ~thread_journal_memory_phase()
        {
            if(context_&&!finished_)
            {
                if(entered_)static_cast<void>(finish(false,{}));
                else if(!context_->cancellation.stop_requested())context_->journal->refuse();
            }
        }
    };
}
