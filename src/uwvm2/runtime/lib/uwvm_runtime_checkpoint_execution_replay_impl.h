// Private real execution journal, selected only for resumable debug code.
#pragma once
#include <uwvm2/utils/container/standard.h>
#ifndef UWVM_MODULE
# include <uwvm2/utils/thread/monotonic_clock.h>
#endif
extern "C++"
{
    // A fork copies the journal and mutex bytes, but not its native workers.
    // Check this immutable process identity before touching inherited locks or
    // shared owners. An exec initializes a fresh identity and empty journal.
#if defined(__unix__) || defined(__APPLE__)
    inline auto const checkpoint_replay_native_process{::getpid()};
#endif
    [[nodiscard]] inline bool checkpoint_replay_original_process() noexcept
    {
#if defined(__unix__) || defined(__APPLE__)
        return checkpoint_replay_native_process==::getpid();
#else
        return true;
#endif
    }
    struct runtime_checkpoint_replay_session
    {
        ::std::mutex mutex{};
        ::uwvm2::uwvm::debugger::checkpoint::state origin{};
        ::uwvm2::utils::container::std_vector<::uwvm2::uwvm::debugger::checkpoint::object> events{};
        ::uwvm2::utils::container::std_vector<::std::size_t> actual_modules{};
        ::std::uint64_t epoch{},thread{},instructions{},running{};
        ::std::condition_variable changed{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> participants{};
        // Begin-event identity of the actual retained native operation. Only
        // audited phased providers populate this; a zero cell is ordinary Wasm.
        ::uwvm2::utils::container::std_vector<::std::uint64_t> retained_imports{};
        ::uwvm2::utils::container::std_vector<::std::weak_ptr<llvm_jit_checkpoint_pending_snapshot const>> retained_snapshots{};
        ::uwvm2::utils::container::std_vector<bool> exited{},pending_origin{};
        // Recording serializes Wasm instructions and audited host-memory phases.
        // Native mutex acquisition is not fair: an eager worker could otherwise
        // starve every peer and create millions of unrelated journal events
        // before the host can reach a requested multi-worker stop.
        ::uwvm2::utils::container::std_vector<::std::uint64_t> turn_requests{};
        ::std::uint64_t next_turn{1u};
        [[nodiscard]] bool recording_turn(::std::uint64_t logical) noexcept
        {
            if(!logical || logical>participants.size())return false;
            if(turn_requests.size()!=participants.size())return false;
            auto& request=turn_requests[logical-1u];
            if(!request)
            {
                if(next_turn==UINT64_MAX)
                {
                    // This is a host sequence, never serialized authority. A
                    // complete empty queue can reset it without changing order.
                    if(::std::any_of(turn_requests.begin(),turn_requests.end(),[](auto n){return n!=0u;}))
                    {fail(::uwvm2::uwvm::debugger::checkpoint::error::limit_exceeded);return false;}
                    next_turn=1u;
                }
                request=next_turn++;
            }
            if(running)return false;
            for(auto other:turn_requests)if(other && other<request)return false;
            request=0u;return true;
        }
        struct wait_state
        {
            ::std::uint64_t memory{},position{},sequence{},deadline{UINT64_MAX},order{};
            unsigned operation{},phase{}; // 0 inactive, 1 pending, 2 notified, 3 timed out, 4 unequal
            bool ready{};
        };
        ::uwvm2::utils::container::std_vector<wait_state> waits{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> times{};
        ::std::uint64_t elapsed{},clock_origin{},wait_order{};
        ::std::size_t cursor{},history_index{SIZE_MAX};
        unsigned mode{1u};
        ::uwvm2::uwvm::debugger::checkpoint::error failure{};
        using location=::uwvm2::utils::thread::cooperative_pause_location;
        using cp_object=::uwvm2::uwvm::debugger::checkpoint::object;
        void initialize_waits()
        {
            namespace cp=::uwvm2::uwvm::debugger::checkpoint;
            waits.resize(participants.size());turn_requests.resize(participants.size());retained_imports.resize(participants.size());
            for(auto const& thread:origin.objects)if(thread.kind==cp::object_kind::thread && thread.flags==2u)
            {
                auto const& saved=origin.objects[thread.links.back()-1u];auto& wait=waits[thread.words[0]-1u];
                wait.memory=saved.links[0];wait.position=saved.words[0];wait.sequence=UINT64_MAX;
                wait.operation=saved.words[1]==4u?1u:2u;wait.phase=static_cast<unsigned>(saved.flags)+1u;
                wait.ready=saved.flags!=0u;wait.deadline=saved.words[3];wait.order=saved.words[4];
                if(!wait.ready)wait_order=(::std::max)(wait_order,wait.order+1u);
            }
            if(mode==1u)
            {
                auto sample=::uwvm2::utils::host_clock::now();
                if(!::uwvm2::utils::host_clock::available())throw ::uwvm2::uwvm::debugger::checkpoint::error::unavailable_capability;
                clock_origin=static_cast<::std::uint64_t>(::std::chrono::duration_cast<::std::chrono::nanoseconds>(sample.time_since_epoch()).count());
            }
        }
        void append(cp_object event)
        {
            auto sample=::uwvm2::utils::host_clock::now();
            auto stamp=static_cast<::std::uint64_t>(::std::chrono::duration_cast<::std::chrono::nanoseconds>(sample.time_since_epoch()).count());
            if(!::uwvm2::utils::host_clock::available() || stamp<clock_origin || stamp-clock_origin<elapsed || stamp-clock_origin>INT64_MAX)
                throw ::uwvm2::uwvm::debugger::checkpoint::error::unavailable_capability;
            elapsed=stamp-clock_origin;
            for(unsigned n{};n<8u;++n)event.bytes.push_back(static_cast<::std::byte>(elapsed>>(8u*n)));
            events.push_back(::std::move(event));
        }
        void consume() noexcept {elapsed=times[cursor];++cursor;}
        void consume_ready() noexcept
        {
            namespace cp=::uwvm2::uwvm::debugger::checkpoint;
            while(mode==3u && cursor<events.size() &&
                  (events[cursor].flags==cp::execution_replay::wait_ready_kind || events[cursor].flags==cp::execution_replay::environment_kind))
            {
                if(events[cursor].flags==cp::execution_replay::environment_kind)
                {history_index=cursor;consume();continue;}
                auto const& event=events[cursor];auto& wait=waits[event.words[2]-1u];
                if(!wait.phase || wait.ready || wait.sequence!=event.words[7] || wait.operation!=event.words[3])
                {fail(cp::error::replay_diverged);return;}
                wait.ready=true;auto result=event.values[0].low_bits;
                wait.phase=result==0u?2u:result==2u?3u:4u;
                consume();changed.notify_all();
            }
        }
        [[nodiscard]] bool inspect_wait(::std::uint64_t logical,
            ::uwvm2::utils::thread::keyed_wait_set::suspension_snapshot& snapshot) const noexcept
        {
            if(!logical || logical>waits.size())return false;
            auto const& wait=waits[logical-1u];if(!wait.phase || wait.phase==4u)return false;
            snapshot={};snapshot.position=wait.position;snapshot.notified=wait.phase==2u;snapshot.timed_out=wait.phase==3u;
            snapshot.remaining_nanoseconds=wait.deadline==UINT64_MAX?-1:
                wait.deadline>elapsed?static_cast<::std::int64_t>(wait.deadline-elapsed):0;
            if(snapshot.timed_out)snapshot.remaining_nanoseconds=0;
            if(snapshot.notified || snapshot.timed_out)snapshot.pending_predecessors=SIZE_MAX;
            else for(auto const& other:waits)
                if(other.phase==1u && other.memory==wait.memory && other.position==wait.position && other.order<wait.order)
                    ++snapshot.pending_predecessors;
            return true;
        }
        [[nodiscard]] ::std::size_t logical_module(::std::size_t actual) const noexcept
        {
            auto it=::std::find(actual_modules.begin(),actual_modules.end(),actual);
            return it==actual_modules.end()?SIZE_MAX:static_cast<::std::size_t>(it-actual_modules.begin());
        }
        void fail(::uwvm2::uwvm::debugger::checkpoint::error error) noexcept
        {mode=5u;failure=error;changed.notify_all();}
        [[nodiscard]] bool same_instruction(cp_object const& event,location point) const noexcept
        {
            return event.words[3]==point.function && event.words[6]==logical_module(point.code_unit) && event.words[7]==point.offset;
        }
        [[nodiscard]] cp_object instruction(location point,::std::uint64_t logical_thread,bool endpoint=false) const
        {
            namespace ex=::uwvm2::uwvm::debugger::checkpoint::execution_replay;
            cp_object event{};event.kind=::uwvm2::uwvm::debugger::checkpoint::object_kind::event;
            event.flags=endpoint?ex::endpoint_kind:ex::instruction_kind;
            event.words={events.size()+1u,instructions+1u,logical_thread,point.function,0u,0u,logical_module(point.code_unit),point.offset};
            return event;
        }
        [[nodiscard]] ::std::uint64_t logical_thread(::std::uint64_t participant) const noexcept
        {
            auto found=::std::find(participants.begin(),participants.end(),participant);
            return participant && found!=participants.end()?static_cast<::std::uint64_t>(found-participants.begin())+1u:0u;
        }
        [[nodiscard]] bool actual_worker(location point,::std::uint64_t id) noexcept
        {
            auto logical=logical_thread(id);
            if(point.code_generation!=epoch || logical_module(point.code_unit)==SIZE_MAX || !logical || exited[logical-1u])
            {fail(::uwvm2::uwvm::debugger::checkpoint::error::replay_diverged);return false;}
            return true;
        }
        [[nodiscard]] bool endpoint(location point,::std::uint64_t id) const noexcept
        {
            auto logical=logical_thread(id);
            for(auto it=events.begin()+cursor;it!=events.end();++it)
                if((it->flags==::uwvm2::uwvm::debugger::checkpoint::execution_replay::endpoint_kind ||
                    it->flags==::uwvm2::uwvm::debugger::checkpoint::execution_replay::wait_endpoint_kind) && it->words[2]==logical)
                    return same_instruction(*it,point);
            return false;
        }

    };
    // A management capture may append environment/endpoint records and then
    // fail while copying or validating the frozen graph. Keep the journal
    // locked through publication, and roll back the append-only suffix on
    // every rejection/exception. Ordinary guest execution never uses this guard.
    class runtime_checkpoint_replay_append_transaction
    {
        ::std::shared_ptr<runtime_checkpoint_replay_session> session_{};
        ::std::unique_lock<::std::mutex> lock_{};
        ::std::size_t count_{}, history_{};
        ::std::uint64_t elapsed_{};
        unsigned mode_{};
        bool published_{};
    public:
        runtime_checkpoint_replay_append_transaction() noexcept = default;
        runtime_checkpoint_replay_append_transaction(runtime_checkpoint_replay_append_transaction const&) = delete;
        runtime_checkpoint_replay_append_transaction& operator=(runtime_checkpoint_replay_append_transaction const&) = delete;
        void bind(::std::shared_ptr<runtime_checkpoint_replay_session> session)
        {
            if(session_ || !session) { ::fast_io::fast_terminate(); }
            ::std::unique_lock lock{session->mutex};
            count_=session->events.size(); history_=session->history_index;
            elapsed_=session->elapsed; mode_=session->mode;
            session_=::std::move(session); lock_=::std::move(lock);
        }
        void publish() noexcept { published_=true; }
        ~runtime_checkpoint_replay_append_transaction() noexcept
        {
            if(!session_ || published_) { return; }
            if(session_->events.size()<count_) { ::fast_io::fast_terminate(); }
            while(session_->events.size()!=count_) { session_->events.pop_back(); }
            session_->history_index=history_; session_->elapsed=elapsed_; session_->mode=mode_;
        }
    };
    [[noreturn]] inline void checkpoint_replay_refuse(runtime_checkpoint_replay_session const& session) noexcept
    {
        namespace diagnostic=::uwvm2::runtime::diagnostic::crash;
        diagnostic::text_record text{};
        text.append("uwvm: [error] execution replay refused; exit=125 state-error=");
        text.number(static_cast<unsigned>(session.failure));
        text.append(" instruction=");text.number(session.instructions);
        text.append(" event=");text.number(session.cursor);
        text.append("; no unrecorded host call was dispatched.\n");
        static_cast<void>(diagnostic::write_stderr(text.bytes,text.size));
        // Linux fast_exit is SYS_exit, which abandons this worker's locks.
        // The rejection must close the entire process on every host platform.
        ::std::_Exit(125);
    }
    inline ::std::shared_ptr<runtime_checkpoint_replay_session> checkpoint_replay_current() noexcept
    {
        if(!checkpoint_replay_selected.load(::std::memory_order_acquire))return {};
        if(!checkpoint_replay_original_process())return {};
        ::std::lock_guard lock{checkpoint_replay_mutex};return checkpoint_replay_session;
    }
    void checkpoint_replay_reset() noexcept
    {
        ::std::lock_guard lock{checkpoint_replay_mutex};checkpoint_replay_selected.store(false,::std::memory_order_release);
        checkpoint_replay_session.reset();
    }
    inline void checkpoint_replay_install(::std::shared_ptr<runtime_checkpoint_replay_session> session) noexcept
    {
        ::std::lock_guard lock{checkpoint_replay_mutex};checkpoint_replay_session=::std::move(session);
        checkpoint_replay_selected.store(static_cast<bool>(checkpoint_replay_session),::std::memory_order_release);
    }
    void checkpoint_replay_wait_completed(::std::uint64_t participant,::uwvm2::utils::thread::keyed_wait_result result) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        using outcome=::uwvm2::utils::thread::keyed_wait_result;
        auto current=checkpoint_replay_current();if(!current)return;
        ::std::lock_guard lock{current->mutex};if(current->mode!=1u)return;
        auto logical=current->logical_thread(participant);if(!logical)return;
        auto& wait=current->waits[logical-1u];
        // An already completed wait captured in the origin has no second native
        // completion event. Its delivery remains separately ordered by wait_end.
        if(wait.ready)return;
        if(!wait.phase || !wait.operation || (result!=outcome::notified && result!=outcome::timed_out && result!=outcome::not_equal))
        {current->fail(cp::error::replay_diverged);checkpoint_replay_refuse(*current);}
        try
        {
            auto value=result==outcome::notified?0u:result==outcome::timed_out?2u:1u;
            cp::object event{};event.kind=cp::object_kind::event;event.flags=cp::execution_replay::wait_ready_kind;
            event.words={current->events.size()+1u,current->instructions,logical,wait.operation,0u,1u,cp::execution_replay::schema,wait.sequence};
            cp::value scalar{};scalar.type.kind=cp::value_kind::i32;scalar.low_bits=value;event.values.push_back(scalar);
            current->append(::std::move(event));wait.ready=true;wait.phase=value==0u?2u:value==2u?3u:4u;
            current->changed.notify_all();
        }
        catch(...){current->fail(cp::error::limit_exceeded);checkpoint_replay_refuse(*current);}
    }
    llvm_jit_checkpoint_replay_status llvm_jit_checkpoint_replay_status_host_api() noexcept
    {
        if(!::uwvm2::utils::control::debug_permissions::inspection_allowed())return {};
        auto current=checkpoint_replay_current();if(!current)return {};
        ::std::lock_guard lock{current->mutex};
        return {current->mode,current->instructions,current->events.size(),current->cursor,current->failure};
    }
    llvm_jit_checkpoint_replay_history llvm_jit_checkpoint_replay_history_host_api() noexcept
    {
        llvm_jit_checkpoint_replay_history result{};
        namespace permissions=::uwvm2::utils::control;
        if(!permissions::debug_permissions::inspection_allowed() ||
           permissions::dump_permissions::permit(permissions::dump_permissions::disclosure::full).status!=permissions::dump_permissions::denial::none)return result;
        auto current=checkpoint_replay_current();if(!current)return result;
        ::std::lock_guard lock{current->mutex};
        if(current->history_index>=current->events.size())return result;
        auto const& event=current->events[current->history_index];
        if(event.flags!=::uwvm2::uwvm::debugger::checkpoint::execution_replay::environment_kind)return result;
        try
        {
            auto size=event.bytes.size();
            if(current->mode==1u || current->mode==2u){if(size<8u)return result;size-=8u;}
            result.archive.assign(event.bytes.begin(),event.bytes.begin()+size);
            result.epoch=current->epoch;result.sequence=event.words[0];result.instruction=event.words[1];
        }
        catch(...){return {};}
        return result;
    }
    // Wait outside the journal mutex and keep the genuine generated packet
    // parkable. A scheduler wait must never prevent the debugger's full cohort
    // stop or old-world retirement. No VM/helper PC is fabricated as guest code.
    inline void checkpoint_replay_schedule_poll(runtime_checkpoint_replay_session& current,
        ::std::unique_lock<::std::mutex>& lock,bool before_opcode=true) noexcept
    {
        lock.unlock();
        if(before_opcode && g_debug_pause_participant && g_debug_wait_point.valid)
        {
            auto const point=g_debug_wait_point;auto const participant=g_debug_pause_participant;
            participant->poll(point.location,[&]() noexcept
            {
                {
                    // This authentic before-park callback runs only when the
                    // actual pause domain is parking this worker. Its pending
                    // turn must not block a different worker released for one
                    // debugger step. Rejoin the fair queue after resumption.
                    // Never drop a request merely because a mutex wait occurs.
                    ::std::lock_guard journal_lock{current.mutex};
                    if(current.mode==1u)
                    {
                        auto const logical=current.logical_thread(participant->identifier());
                        if(!logical || logical>current.turn_requests.size() ||
                           current.running==participant->identifier())
                        {current.fail(::uwvm2::uwvm::debugger::checkpoint::error::replay_diverged);checkpoint_replay_refuse(current);}
                        current.turn_requests[logical-1u]=0u;current.changed.notify_all();
                    }
                }
                debug_capture_actual_park(participant,point.location,point.local_snapshot,point.captured_local_count,false);
            });
        }
        lock.lock();
        current.consume_ready();
        current.changed.wait_for(lock,::std::chrono::milliseconds{1});
    }
    // A virtual import waits only on recorded phases. Unlike a live native
    // provider it owns no provider lock, foreign-host scope or native I/O.
    // Let its authentic participant join a requested stop while another worker
    // owns the next journal turn. The location remains the actual caller's
    // Wasm call; never expose this C++ helper as a JIT assembly landing.
    inline void checkpoint_replay_import_schedule_poll(runtime_checkpoint_replay_session& current,
        ::std::unique_lock<::std::mutex>& lock) noexcept
    {
        lock.unlock();
        auto const point=g_debug_wait_point;auto const participant=g_debug_pause_participant;
        auto const ledger=g_debug_activation_ledger;
        if(point.valid && participant && ledger && ledger->complete() && ledger->count() &&
           get_llvm_jit_generated_bridge_scope_depth()!=0u && !g_debug_observer_active &&
           g_debug_native_safe_point_site.return_pc==0u)
        {
            auto const& leaf=ledger->data()[ledger->count()-1u];
            if(point.incarnation && point.incarnation==leaf.incarnation &&
               leaf.module==point.location.code_unit && leaf.function==point.location.function &&
               leaf.runtime_epoch==point.location.code_generation)
                participant->poll(point.location,[&]() noexcept
                {
                    // This is a suspended operation, not a before-opcode
                    // continuation. With no authenticated atomic wait owner,
                    // ordinary executable capture must remain unavailable;
                    // replay may resume the SAME virtual call after the stop.
                    debug_capture_actual_park(participant,point.location,
                        point.local_snapshot,point.captured_local_count,true);
                });
        }
        lock.lock();current.consume_ready();
        current.changed.wait_for(lock,::std::chrono::milliseconds{1});
    }
    void checkpoint_replay_probe_opcode(::uwvm2::utils::thread::cooperative_pause_location point,::std::uint64_t participant) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;namespace ex=cp::execution_replay;
        auto current=checkpoint_replay_current();if(!current)return;
        ::std::lock_guard lock{current->mutex};
        if(current->running==participant){current->running=0u;current->changed.notify_all();}
        current->consume_ready();
        if(current->mode!=3u)return;
        if(!current->actual_worker(point,participant))return;
        if(current->pending_origin[current->logical_thread(participant)-1u])return;
        if(current->cursor==current->events.size()){current->fail(cp::error::replay_diverged);return;}
        auto const& next=current->events[current->cursor];
        if(next.flags==ex::endpoint_kind || next.flags==ex::wait_endpoint_kind)
        {
            if(!current->endpoint(point,participant)){current->fail(cp::error::replay_diverged);return;}
            current->mode=4u;current->changed.notify_all();
        }
        // Another worker owns the next recorded turn. Check our instruction
        // only after that turn is granted, not against its source coordinates.
        else if(next.words[2]==current->logical_thread(participant) &&
                (next.flags!=ex::instruction_kind || !current->same_instruction(next,point)))
            current->fail(cp::error::replay_diverged);
    }
    void checkpoint_replay_enter_opcode(::uwvm2::utils::thread::cooperative_pause_location point,::std::uint64_t participant) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;namespace ex=cp::execution_replay;
        auto current=checkpoint_replay_current();if(!current)return;
        ::std::unique_lock lock{current->mutex};
        for(;;)
        {
            current->consume_ready();
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
            if(runtime_checkpoint_retirement_bridge::management_blocked())return;
#endif
            if(current->mode==2u)return;
            if(!current->actual_worker(point,participant) || current->mode==5u)checkpoint_replay_refuse(*current);
            // This opcode had already compared and entered its native wait at
            // the checkpoint. It has no new instruction/begin event to consume.
            if(current->pending_origin[current->logical_thread(participant)-1u])return;
            if(current->mode==4u || (current->mode==3u && current->cursor<current->events.size() &&
                (current->events[current->cursor].flags==ex::endpoint_kind || current->events[current->cursor].flags==ex::wait_endpoint_kind)))
            {
                if(!current->endpoint(point,participant)){current->fail(cp::error::replay_diverged);checkpoint_replay_refuse(*current);}
                current->mode=4u;current->changed.notify_all();
                lock.unlock();
                auto const& observer=g_runtime.debug_observer;
                if(observer.on_safe_point && !g_debug_observer_active)
                {
                    g_debug_observer_active=true;
                    observer.on_safe_point(observer.context.get(),participant,point);
                    g_debug_observer_active=false;
                }
                lock.lock();checkpoint_replay_schedule_poll(*current,lock);continue;
            }
            if(current->mode==1u ? current->recording_turn(current->logical_thread(participant)) :
                (!current->running && current->cursor<current->events.size() &&
                 current->events[current->cursor].words[2]==current->logical_thread(participant)))break;
            checkpoint_replay_schedule_poll(*current,lock);
        }
        try
        {
            if(current->mode==1u){current->append(current->instruction(point,current->logical_thread(participant)));++current->instructions;}
            else if(current->mode==3u)
            {
                if(current->cursor==current->events.size() || current->events[current->cursor].flags!=ex::instruction_kind ||
                   !current->same_instruction(current->events[current->cursor],point))
                {current->fail(cp::error::replay_diverged);checkpoint_replay_refuse(*current);}
                current->consume();++current->instructions;
            }
            current->running=participant;
        }
        catch(...){current->fail(cp::error::limit_exceeded);checkpoint_replay_refuse(*current);}
    }
    void checkpoint_replay_leave_worker(::std::uint64_t participant) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        auto current=checkpoint_replay_current();if(!current)return;
        ::std::unique_lock lock{current->mutex};
        if(current->running==participant){current->running=0u;current->changed.notify_all();}
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
        if(runtime_checkpoint_retirement_bridge::management_blocked())return;
#endif
        if(current->epoch!=current_runtime_generation() || current->mode==2u || current->mode==4u)return;
        auto logical=current->logical_thread(participant);if(!logical)return;
        try
        {
            cp::object event{};event.kind=cp::object_kind::event;event.flags=cp::execution_replay::thread_exit_kind;
            event.words={current->events.size()+1u,current->instructions,logical,0u,0u,0u,cp::execution_replay::schema,0u};
            if(current->mode==1u)current->append(::std::move(event));
            else if(current->mode==3u)
            {
                while(current->cursor<current->events.size() && current->events[current->cursor].words[2]!=logical)
                    checkpoint_replay_schedule_poll(*current,lock,false);
                if(current->cursor==current->events.size() || current->events[current->cursor].flags!=event.flags)
                {current->fail(cp::error::replay_diverged);checkpoint_replay_refuse(*current);}
                current->consume();
            }
            else checkpoint_replay_refuse(*current);
            current->exited[logical-1u]=true;current->changed.notify_all();
        }
        catch(...){current->fail(cp::error::limit_exceeded);checkpoint_replay_refuse(*current);}
    }
    // Stable memory identity is recovered from current owned modules. Journal
    // bytes and guest operands never name a host allocation or waiter domain.
    inline ::uwvm2::uwvm::debugger::checkpoint::object_id checkpoint_replay_memory_identity(
        runtime_checkpoint_replay_session const& current,::std::uintptr_t owner,unsigned bits) noexcept
    {
        for(::std::size_t dense{};dense<current.actual_modules.size();++dense)
        {
            auto const actual=current.actual_modules[dense];if(actual>=g_runtime.modules.size())return 0u;
            auto const* module=g_runtime.modules.index_unchecked(actual).runtime_module;if(!module)return 0u;
            auto const& instance=current.origin.objects[current.origin.root_instances[dense]-1u];
            auto const begin=1u+instance.words[0]+instance.words[1]+module->imported_memory_vec_storage.size();
            for(::std::size_t local{};local<module->local_defined_memory_vec_storage.size();++local)
            {
                auto const& memory=module->local_defined_memory_vec_storage.index_unchecked(local).memory;
                if(reinterpret_cast<::std::uintptr_t>(::std::addressof(memory))!=owner)continue;
                auto const id=instance.links[begin+local];
                return current.origin.objects[id-1u].words[0]==bits?id:0u;
            }
        }
        return 0u;
    }
    inline thread_local runtime_checkpoint_replay_session* checkpoint_replay_linked_wait{};
    // Capture the bytes actually compared by this wait and its allocation tag
    // while this worker still owns the instruction turn. A replay must not
    // return a saved wait result for different memory/tag state. The address is
    // resolved by native owner identity, never dereferenced from journal DATA.
    // Pending-origin waits are already queued and deliberately do not compare
    // again: another worker may have changed their word after enqueueing.
    inline bool checkpoint_replay_wait_guard(runtime_checkpoint_replay_session const& current,
        checkpoint_wait_operation const& actual,::uwvm2::utils::container::std_vector<::std::byte>& output)
    {
        namespace mt=::uwvm2::extension::memtag;
        for(auto id:current.actual_modules)
        {
            if(id>=g_runtime.modules.size())return false;
            auto const* module=g_runtime.modules.index_unchecked(id).runtime_module;if(!module)return false;
            for(auto const& local:module->local_defined_memory_vec_storage)
            {
                auto const& memory=local.memory;
                if(reinterpret_cast<::std::uintptr_t>(::std::addressof(memory))!=actual.owner)continue;
                auto const length=[]<typename M>(M const& m)->::std::uint64_t
                {
                    if constexpr(M::can_mmap)return m.memory_length_p?m.memory_length_p->load(::std::memory_order_acquire):0u;
                    else return m.memory_length;
                }(memory);
                auto const width=actual.operation==2u?8u:4u;
                bool const tagged=memory.memtag_shadow.enabled();
                auto address=actual.address;
                if(actual.bits==32u && address>UINT32_MAX)return false;
                if(tagged)
                {
                    if((memory.memtag_shadow.width()==mt::address_width::i64?64u:32u)!=actual.bits)return false;
                    address&=actual.bits==32u?mt::shadow_view<::std::uint32_t>::address_mask:mt::shadow_view<::std::uint64_t>::address_mask;
                }
                if(actual.offset>UINT64_MAX-address)return false;
                address+=actual.offset;
                if((address&(width-1u)) || address>length || width>length-address || !memory.memory_begin)return false;
                ::std::uint64_t tag{};
                if(tagged)
                {
                    auto load=[&]<typename Address,bool Shared>() noexcept
                    {return memory.memtag_shadow.template view<Address,Shared>(length).load(static_cast<Address>(address));};
                    auto const checked=actual.bits==64u?
                        (memory.memtag_shadow.shared()?load.template operator()<::std::uint64_t,true>():load.template operator()<::std::uint64_t,false>()):
                        (memory.memtag_shadow.shared()?load.template operator()<::std::uint32_t,true>():load.template operator()<::std::uint32_t,false>());
                    if(!checked)return false;
                    tag=1u|(::std::uint64_t{checked.allocation_tag}<<8u);
                }
                auto* pointer=reinterpret_cast<::std::byte*>(memory.memory_begin)+static_cast<::std::size_t>(address);
                if(memory.memtag_shadow.hardware_active())pointer=mt::hardware_mte::tagged_pointer(pointer,static_cast<::std::uint8_t>(tag>>8u));
                auto const value=width==8u?::uwvm2::runtime::compiler::shared::wasm_threads::atomic_load_le<8u>(pointer):
                    ::uwvm2::runtime::compiler::shared::wasm_threads::atomic_load_le<4u>(pointer);
                output.reserve(32u);
                for(auto word:{length,address,value,tag})for(unsigned byte{};byte<8u;++byte)
                    output.push_back(static_cast<::std::byte>(word>>(byte*8u)));
                return true;
            }
        }
        return false;
    }
    void checkpoint_replay_wait_registered() noexcept
    {
        auto* current=checkpoint_replay_linked_wait;if(!current)return;
        ::std::lock_guard lock{current->mutex};
        if(g_debug_pause_participant && current->mode==1u)
        {
            auto logical=current->logical_thread(g_debug_pause_participant->identifier());
            if(logical && current->pending_origin[logical-1u])return; // Already pre-linked; never release another worker's turn.
        }
        if(!g_debug_pause_participant || current->running!=g_debug_pause_participant->identifier() || current->mode!=1u)
        {current->fail(::uwvm2::uwvm::debugger::checkpoint::error::replay_diverged);checkpoint_replay_refuse(*current);}
        current->running=0u;current->changed.notify_all();
    }
    inline void checkpoint_replay_poll_wait(runtime_checkpoint_replay_session& current,
        ::std::unique_lock<::std::mutex>& lock,::std::uint64_t logical,checkpoint_wait_operation const& actual) noexcept
    {
        auto const& wait=current.waits[logical-1u];
        if(!actual.operation || !wait.phase || wait.phase==4u)
        {checkpoint_replay_schedule_poll(current,lock,false);return;}
        ::uwvm2::utils::thread::keyed_wait_set::suspension_snapshot snapshot{};
        if(!current.inspect_wait(logical,snapshot))checkpoint_replay_refuse(current);
        // This is the genuine still-active wait opcode and its independently
        // matched queue state, not a fabricated helper/program counter. A new
        // dump at this intermediate stop can restore the pending/completed wait.
        auto const* previous=checkpoint_virtual_wait;checkpoint_virtual_wait=::std::addressof(snapshot);
        auto const point=g_debug_wait_point;auto const participant=g_debug_pause_participant;
        lock.unlock();
        participant->poll(point.location,[&]() noexcept
            {debug_capture_actual_park(participant,point.location,point.local_snapshot,point.captured_local_count,true);});
        checkpoint_virtual_wait=previous;lock.lock();
        current.consume_ready();
        current.changed.wait_for(lock,::std::chrono::milliseconds{1});
    }
    // Invoked only by the complete census after its genuine all-participant
    // pause/root/publication transaction. A before-park DATA copy can predate
    // another worker's last ready event; recopy from the now-stable journal.
    [[nodiscard]] inline bool checkpoint_replay_refresh_virtual_wait(::std::uint64_t participant,
        checkpoint_wait_operation const& operation,::std::uint64_t generation,
        ::uwvm2::utils::thread::keyed_wait_set::suspension_snapshot& snapshot) noexcept
    {
        auto current=checkpoint_replay_current();if(!current)return true;
        ::std::lock_guard lock{current->mutex};
        if(current->mode!=3u)return current->mode!=5u;
        auto logical=current->logical_thread(participant);
        if(current->epoch!=generation || !logical || current->waits[logical-1u].operation!=operation.operation ||
           current->waits[logical-1u].memory!=checkpoint_replay_memory_identity(*current,operation.owner,operation.bits))return false;
        return current->inspect_wait(logical,snapshot);
    }
    inline void checkpoint_replay_park_wait_endpoint(runtime_checkpoint_replay_session& current,
        ::std::unique_lock<::std::mutex>& lock,::std::uint64_t logical,checkpoint_wait_operation const& actual) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        auto refuse=[&]() noexcept{current.fail(cp::error::replay_diverged);checkpoint_replay_refuse(current);};
        auto found=::std::find_if(current.events.begin()+current.cursor,current.events.end(),[&](auto const& event)
            {return event.flags==cp::execution_replay::wait_endpoint_kind && event.words[2]==logical;});
        if(found==current.events.end() || !current.same_instruction(*found,g_debug_wait_point.location) ||
            found->links.size()!=1u || found->values.size()!=9u ||
            found->links[0]!=checkpoint_replay_memory_identity(current,actual.owner,actual.bits))refuse();
        auto value=[&](::std::size_t n){return found->values[n].low_bits;};
        if(value(1)!=(actual.operation==1u?4u:8u) || value(2)!=actual.expected || value(5)!=actual.address ||
            value(6)!=actual.offset || value(7)!=actual.bits)refuse();
        ::uwvm2::utils::thread::keyed_wait_set::suspension_snapshot queued{};
        if(!current.inspect_wait(logical,queued) || value(0)!=queued.position || value(4)!=queued.pending_predecessors ||
           value(8)!=(queued.timed_out?2u:queued.notified?1u:0u))refuse();
        try
        {
            ::uwvm2::utils::container::std_vector<::std::byte> guard{};
            if(!checkpoint_replay_wait_guard(current,actual,guard) || guard!=found->bytes)refuse();
        }
        catch(...){current.fail(cp::error::limit_exceeded);checkpoint_replay_refuse(current);}
        ::uwvm2::utils::thread::keyed_wait_set::suspension_snapshot snapshot{};
        snapshot.position=value(0);snapshot.remaining_nanoseconds=static_cast<::std::int64_t>(value(3));
        snapshot.pending_predecessors=value(4);snapshot.notified=value(8)==1u;snapshot.timed_out=value(8)==2u;
        current.mode=4u;current.running=0u;current.changed.notify_all();
        auto const* previous=checkpoint_virtual_wait;checkpoint_virtual_wait=::std::addressof(snapshot);
        lock.unlock();
        auto const& observer=g_runtime.debug_observer;
        auto const point=g_debug_wait_point;auto const participant=g_debug_pause_participant;
        if(observer.on_safe_point && !g_debug_observer_active)
        {
            g_debug_observer_active=true;
            observer.on_safe_point(observer.context.get(),participant->identifier(),point.location);
            g_debug_observer_active=false;
        }
        participant->poll(point.location,[&]() noexcept
            {debug_capture_actual_park(participant,point.location,point.local_snapshot,point.captured_local_count,true);});
        checkpoint_virtual_wait=previous;lock.lock();
        current.consume_ready();
        current.changed.wait_for(lock,::std::chrono::milliseconds{1});
    }
    ::std::uint64_t llvm_jit_checkpoint_wait(::std::uintptr_t owner,unsigned operation,unsigned bits,
        ::std::uint64_t offset,::std::uint64_t address,::std::uint64_t expected,::std::int64_t timeout,
        void* context,::std::uint64_t (*invoke)(void*)) UWVM_THROWS
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;namespace ex=cp::execution_replay;
        checkpoint_wait_operation actual{owner,operation,bits,offset,address,expected,timeout};
        struct operation_scope
        {
            checkpoint_wait_operation const* previous{checkpoint_actual_wait};
            explicit operation_scope(checkpoint_wait_operation const& value) noexcept{checkpoint_actual_wait=::std::addressof(value);}
            ~operation_scope(){checkpoint_actual_wait=previous;}
        } operation_lifetime{actual};
        auto native_wait=[&]() -> ::std::uint64_t
        {
            auto retire_after_wait=[]()
            {
                if(!runtime_checkpoint_retirement_bridge::management_blocked())return;
                auto const point=g_debug_wait_point;
                runtime_checkpoint_retirement_bridge::poll_current_after_park(point.incarnation,
                    point.location.code_unit,point.location.function,point.location.offset);
            };
            auto* pending=checkpoint_restored_wait;
            if(!pending || pending->consumed)
            {auto value=invoke(context);retire_after_wait();return value;}
            auto const& saved=pending->operation;
            auto const* execution=::uwvm2::runtime::wasm_threads::current_execution;
            if(!execution || !execution->pause || !pending->batch || !g_debug_wait_point.valid ||
                saved.owner!=owner || saved.operation!=operation || saved.bits!=bits || saved.offset!=offset ||
                saved.address!=address || saved.expected!=expected ||
                pending->location.code_unit!=g_debug_wait_point.location.code_unit ||
                pending->location.function!=g_debug_wait_point.location.function || pending->location.offset!=g_debug_wait_point.location.offset)
                ::std::_Exit(125);
            pending->consumed=true;
            execution->pause->linked();
            auto const result=pending->batch->consume(pending->index,{reinterpret_cast<void const*>(owner),pending->position},
                *execution->pause,execution->cancellation);
            retire_after_wait();
            using outcome=::uwvm2::utils::thread::keyed_wait_result;
            if(result.outcome==outcome::notified)return 0u;
            if(result.outcome==outcome::timed_out)return 2u;
            details::llvm_jit_debug_cancelled_wait_abi_bridge();
            llvm_jit_managed_runtime_trap(llvm_jit_trap_kind::atomic_wait_cancelled,0u,0u);
            ::fast_io::fast_terminate();
        };
        auto current=checkpoint_replay_current();bool completed_before_record{};::std::uint64_t result{};
        if(!current)
        {
            result=native_wait();current=checkpoint_replay_current();
            if(!current)return result;
            completed_before_record=true;
        }
        ::std::unique_lock lock{current->mutex};
        current->consume_ready();
        if(current->mode==2u){lock.unlock();return completed_before_record?result:native_wait();}
        auto refuse=[&]() noexcept -> void
        {current->fail(cp::error::replay_diverged);checkpoint_replay_refuse(*current);};
        if(!g_debug_pause_participant || !g_debug_wait_point.valid || checkpoint_replay_linked_wait || operation>2u ||
           (bits!=32u && bits!=64u) || !invoke || !context ||
           !current->actual_worker(g_debug_wait_point.location,g_debug_pause_participant->identifier()))refuse();
        auto const participant=g_debug_pause_participant->identifier();
        auto const logical=current->logical_thread(participant);
        bool const resumed=current->pending_origin[logical-1u];
        if((completed_before_record && !resumed) || (!resumed && current->running!=participant))refuse();
        cp::object begin{};begin.kind=cp::object_kind::event;begin.flags=ex::wait_begin_kind;
        begin.words={current->events.size()+1u,current->instructions,logical,operation,4u,0u,ex::schema,bits};
        try
        {
            auto const memory=checkpoint_replay_memory_identity(*current,owner,bits);if(!memory)refuse();
            begin.links.push_back(memory);
            for(auto value:{offset,address,expected,static_cast<::std::uint64_t>(timeout)})
            {cp::value item{};item.type.kind=cp::value_kind::i64;item.low_bits=value;begin.values.push_back(item);}
            bool const replay=current->mode==3u || current->mode==4u;::std::uint64_t sequence{};
            if(resumed)sequence=UINT64_MAX;
            else
            {
                if(!checkpoint_replay_wait_guard(*current,actual,begin.bytes))
                {
                    if(replay)refuse();
                    // Preserve the genuine Wasm bounds/alignment trap during
                    // recording. This incomplete history cannot be saved.
                    current->fail(cp::error::non_replayable_import);lock.unlock();return native_wait();
                }
                if(replay)
                {
                    if(current->cursor>=current->events.size())refuse();
                    auto const& saved=current->events[current->cursor];
                    if(saved.flags!=begin.flags || saved.words[1]!=begin.words[1] || saved.words[2]!=begin.words[2] ||
                       saved.words[3]!=begin.words[3] || saved.words[7]!=begin.words[7] || saved.links!=begin.links ||
                       saved.values!=begin.values || saved.bytes!=begin.bytes)refuse();
                    sequence=saved.words[0];current->consume();current->running=0u;current->changed.notify_all();
                }
                else
                {if(current->mode!=1u)refuse();sequence=begin.words[0];current->append(::std::move(begin));}
                auto& wait=current->waits[logical-1u];wait={};wait.operation=operation;wait.sequence=sequence;
                if(operation)
                {
                    wait.phase=1u;wait.memory=memory;wait.position=address;
                    auto const& saved=current->events[static_cast<::std::size_t>(sequence-1u)];
                    wait.position=0u;
                    for(unsigned n{};n<8u;++n)wait.position|=::std::uint64_t{::std::to_integer<unsigned>(saved.bytes[8u+n])}<<(8u*n);
                    if(current->wait_order==UINT64_MAX)refuse();wait.order=current->wait_order++;
                    if(timeout>=0)wait.deadline=current->elapsed+static_cast<::std::uint64_t>(timeout);
                }
            }
            if(replay && checkpoint_restored_wait && resumed)checkpoint_restored_wait->consumed=true;
            if(!replay && !completed_before_record)
            {
                struct linked_scope
                {
                    explicit linked_scope(runtime_checkpoint_replay_session* value) noexcept{checkpoint_replay_linked_wait=value;}
                    ~linked_scope(){checkpoint_replay_linked_wait=nullptr;}
                } active{current.get()};
                lock.unlock();
                try {result=native_wait();}
                catch(...){lock.lock();current->fail(cp::error::non_replayable_import);throw;}
                lock.lock();
            }
            for(;;)
            {
                current->consume_ready();
                if(runtime_checkpoint_retirement_bridge::management_blocked())
                {
                    // Retire at the captured wait opcode, BEFORE returning a
                    // result or advancing the generated operand/shadow state.
                    auto const point=g_debug_wait_point;lock.unlock();
                    runtime_checkpoint_retirement_bridge::poll_current_after_park(point.incarnation,
                        point.location.code_unit,point.location.function,point.location.offset);
                    lock.lock();continue;
                }
                if(!replay && current->mode==2u)return result;
                if(replay && (current->mode==4u || (current->cursor<current->events.size() &&
                    (current->events[current->cursor].flags==ex::endpoint_kind || current->events[current->cursor].flags==ex::wait_endpoint_kind))))
                {checkpoint_replay_park_wait_endpoint(*current,lock,logical,actual);continue;}
                if(current->mode!=1u && current->mode!=3u)refuse();
                if((!current->running || current->running==participant) && (!replay ||
                    (current->cursor<current->events.size() && current->events[current->cursor].words[2]==logical)))break;
                checkpoint_replay_poll_wait(*current,lock,logical,actual);
            }
            cp::object end{};end.kind=cp::object_kind::event;end.flags=ex::wait_end_kind;
            end.words={current->events.size()+1u,current->instructions,logical,operation,0u,1u,ex::schema,sequence};
            if(replay)
            {
                if(current->cursor>=current->events.size())refuse();
                auto const& saved=current->events[current->cursor];
                if(saved.flags!=end.flags || saved.words[1]!=end.words[1] || saved.words[3]!=operation ||
                   saved.words[7]!=sequence || saved.values.size()!=1u || saved.values[0].type.kind!=cp::value_kind::i32)refuse();
                result=saved.values[0].low_bits;current->consume();
            }
            else
            {
                if(result>UINT32_MAX || (operation!=0u && result>2u))refuse();
                cp::value value{};value.type.kind=cp::value_kind::i32;value.low_bits=result;
                end.values.push_back(value);current->append(::std::move(end));
            }
            if(operation && (!current->waits[logical-1u].ready ||
                current->waits[logical-1u].phase!=(result==0u?2u:result==2u?3u:4u)))refuse();
            current->pending_origin[logical-1u]=false;
            current->waits[logical-1u]={};
            current->running=participant;current->changed.notify_all();return result;
        }
        catch(...)
        {if(!lock.owns_lock())lock.lock();current->fail(cp::error::non_replayable_import);throw;}
    }
    bool llvm_jit_checkpoint_memtag_entropy(::std::uintptr_t owner,unsigned operation,
        unsigned address_bits,::std::uint8_t& value) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        namespace mt=::uwvm2::extension::memtag;
        auto current=checkpoint_replay_current();
        if(!current)return ::uwvm2::runtime::memtag::next_random_tag(value);
        ::std::lock_guard lock{current->mutex};
        current->consume_ready();
        if(current->mode==2u)return ::uwvm2::runtime::memtag::next_random_tag(value);
        auto refuse=[&]() noexcept -> void
        {current->fail(cp::error::replay_diverged);checkpoint_replay_refuse(*current);};
        if((current->mode!=1u && current->mode!=3u) || !g_debug_pause_participant ||
           !g_debug_wait_point.valid || !current->actual_worker(g_debug_wait_point.location,g_debug_pause_participant->identifier()) ||
           (address_bits!=32u && address_bits!=64u) ||
           (operation!=static_cast<unsigned>(mt::opcode::random) &&
            operation!=static_cast<unsigned>(mt::opcode::randomstore) &&
            operation!=static_cast<unsigned>(mt::opcode::randomstorez) &&
            operation!=static_cast<unsigned>(mt::opcode::claim) && operation!=static_cast<unsigned>(mt::opcode::claimz) &&
            operation!=static_cast<unsigned>(mt::opcode::reserve)))refuse();
        try
        {
            cp::object_id memory_id{};
            for(::std::size_t dense{};dense<current->actual_modules.size();++dense)
            {
                auto actual=current->actual_modules[dense];if(actual>=g_runtime.modules.size())refuse();
                auto const* module=g_runtime.modules.index_unchecked(actual).runtime_module;if(!module)refuse();
                auto const& instance=current->origin.objects[current->origin.root_instances[dense]-1u];
                auto const first=1u+instance.words[0]+instance.words[1]+module->imported_memory_vec_storage.size();
                for(::std::size_t local{};local<module->local_defined_memory_vec_storage.size();++local)
                {
                    auto const& memory=module->local_defined_memory_vec_storage.index_unchecked(local).memory;
                    if(reinterpret_cast<::std::uintptr_t>(::std::addressof(memory))!=owner)continue;
                    if(!memory.memtag_shadow.enabled() ||
                       (memory.memtag_shadow.width()==mt::address_width::i64?64u:32u)!=address_bits)refuse();
                    memory_id=instance.links[first+local];
                }
            }
            if(!memory_id)refuse();
            cp::object event{};event.kind=cp::object_kind::event;event.flags=cp::execution_replay::entropy_kind;
            event.words={current->events.size()+1u,current->instructions,current->logical_thread(g_debug_pause_participant->identifier()),operation,0u,0u,cp::execution_replay::schema,address_bits};
            event.links.push_back(memory_id);
            if(current->mode==3u)
            {
                if(current->cursor>=current->events.size())refuse();
                auto const& saved=current->events[current->cursor];
                if(saved.flags!=event.flags || saved.words[1]!=event.words[1] || saved.words[2]!=event.words[2] ||
                   saved.words[3]!=event.words[3] || saved.words[7]!=event.words[7] || saved.links!=event.links || saved.bytes.size()!=2u)refuse();
                value=::std::to_integer<::std::uint8_t>(saved.bytes[1]);current->consume();
                return saved.bytes[0]!=::std::byte{};
            }
            bool const success=::uwvm2::runtime::memtag::next_random_tag(value);
            event.bytes={static_cast<::std::byte>(success),static_cast<::std::byte>(value)};
            current->append(::std::move(event));return success;
        }
        catch(...){current->fail(cp::error::limit_exceeded);checkpoint_replay_refuse(*current);}
    }

}
