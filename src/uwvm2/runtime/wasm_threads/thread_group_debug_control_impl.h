// Included in wasm_threads AFTER the complete ThreadedGroup and compiler code.
// G/P are never nested. Only this actual Group owns/resumes/closes its P.
    inline bool ThreadedGroup::debug_stable_roster_locked(::std::size_t& count) const noexcept
    {
        count=0u;
        if(!debug_control_||!debug_live(1u)||!admission_->owns_actual_generation()||
           stop_requested_.load(::std::memory_order_acquire)||!accepting_||
           compiler_starting_||compiler_closing_||!compiler_created_||compiler_body_returned_||
           compiler_current_||compiler_head_!=SIZE_MAX||compiler_payload_reserved_!=0u||debug_control_->spawn_inflight!=0u)
            return false;
        for(::std::size_t i{};i!=slot_count_.load(::std::memory_order_acquire);++i)
        {
            auto const& job{compiler_jobs_[i]};
            if(job.phase==compiler_job_phase::queued||job.phase==compiler_job_phase::running||job.work||job.charged_bytes!=0u)
                return false;
            // Retained completed main tickets are legitimate; only actual
            // queued/running work and destruction/cleanup exclude B1.
            auto const& s{slots_[i]};if(s.phase==slot_phase::empty)continue;
            if(s.phase!=slot_phase::running||!s.native_created||!s.launch_published||
               !s.debug_registered||!s.work||!debug_image_matches(*s.work))return false;
            // A returned dynamic child is still owned until physical join, but
            // it is no longer a participant in the executable pause cohort.
            // The release/acquire body-return publication follows destruction
            // of the real P participant; metadata alone cannot claim departure.
            bool const departed=s.body_returned.load(::std::memory_order_acquire);
            if(departed)
            {
                if(s.debug_state!=debug_worker_state::body_left||s.debug_participant||s.debug_wait_policy||
                   s.debug_participant_id||s.debug_in_poll||s.debug_host_active||s.debug_host_retained)return false;
                auto actual_admission=s.work->retained_admission();
                if(actual_admission.get()!=admission_.get()||actual_admission.owner_before(admission_)||
                   admission_.owner_before(actual_admission))return false;
                continue;
            }
            if(s.debug_state!=debug_worker_state::native_entry||!s.debug_participant||!s.debug_wait_policy||
               !s.debug_participant_id)return false;
            auto actual_admission{s.work->retained_admission()};auto const& cell{s.debug_cell};
            if(actual_admission.get()!=admission_.get()||actual_admission.owner_before(admission_)||admission_.owner_before(actual_admission)||
               cell.participant!=s.debug_participant_id||cell.incarnation!=s.debug_incarnation||cell.native_tid!=s.debug_native_tid||
               cell.slot!=i||cell.source_generation!=admission_->source_generation()||
               cell.validation_epoch!=admission_->source_validation_epoch()||cell.backend!=s.work->debug_image()->backend())return false;
            if(s.debug_host_active!=s.debug_host_retained || cell.retained_host!=s.debug_host_retained)return false;
            ++count;
        }
        return count!=0u;
    }

    inline bool ThreadedGroup::debug_control_worker_enter(slot& actual,
        debug_pause_domain::participant& participant, wait_pause_policy& policy) noexcept
    {
        // enter()/owns_current_participant are P operations outside G. While
        // enrollment is pending the actual native slot fails stable preflight.
        if(!debug_control_||!participant||!debug_control_->pause.owns_current_participant(participant))return false;
        auto const id{participant.identifier()};
        ::std::lock_guard lock{mutex_};
        if(!debug_live(1u)||current_debug_slot_!=&actual||!actual.debug_registered||
           actual.debug_participant||debug_control_->phase!=debug_control_phase::idle||
           !::pthread_equal(actual.thread,::pthread_self()))return false;
        actual.debug_participant=&participant;actual.debug_participant_id=id;actual.debug_wait_policy=&policy;
        actual.debug_cell={id,actual.debug_incarnation,actual.debug_native_tid,admission_->source_generation(),
            admission_->source_validation_epoch(),actual.position,
            SIZE_MAX,SIZE_MAX,SIZE_MAX,UINT64_MAX,actual.work->debug_image()->backend(),debug_stop_site::native_boundary};
        return true;
    }
    inline void ThreadedGroup::debug_control_worker_leave(slot& actual) noexcept
    {
        if(!debug_control_)return;
        (void)debug_poll(actual,debug_stop_site::native_boundary);
        ::std::lock_guard lock{mutex_};
        if(actual.debug_in_poll||actual.debug_host_active||actual.debug_host_retained)::std::terminate();
        // Clear borrowed stack objects BEFORE their real destructors enter P.
        actual.debug_wait_policy=nullptr;actual.debug_participant=nullptr;actual.debug_participant_id=0u;
        changes_.notify_all();
    }

    inline bool ThreadedGroup::debug_poll(slot& actual, debug_stop_site site, ::std::size_t module,
        ::std::size_t function, ::std::uint64_t offset, ::std::size_t instruction) noexcept
    {
        if(!debug_control_)return !stop_requested_.load(::std::memory_order_acquire);
        auto& d{*debug_control_};
        ::std::uint64_t serial{};debug_pause_domain::participant* participant{};
        ::uwvm2::utils::thread::cooperative_pause_location location{};
        {
            ::std::unique_lock lock{mutex_};
            if(current_debug_slot_!=&actual||!actual.debug_participant||actual.debug_host_active||
               !::pthread_equal(actual.thread,::pthread_self()))return false;
            // arming closes admission BEFORE P exists; resuming persists until
            // every actual old P.poll call returned. No lost first request and
            // no stale S1 location carried into an immediately following S2.
            changes_.wait(lock,[&]
            {
                // G.closed/stop_requested publish BEFORE synchronous real stop
                // callbacks. Never return false to wait_suspendable until its
                // original arbitration can see the ACTUAL stop token. CV drops G.
                if(d.phase==debug_control_phase::closed||stop_requested_.load(::std::memory_order_acquire))
                    return cancellation_.stop_requested();
                return d.phase!=debug_control_phase::arming&&d.phase!=debug_control_phase::resuming;
            });
            if(d.phase==debug_control_phase::closed||cancellation_.stop_requested())return false;
            // Enrollment already fixed the identity/backend cell. Idle control
            // needs no last-PC collection or linear metadata scan. An armed
            // episode still proves the ACTUAL current site before entering P.
            if(d.phase!=debug_control_phase::armed)return true;
            auto cell{actual.debug_cell};cell.site=site;
            cell.module=module;cell.function=function;cell.instruction=instruction;cell.wasm_offset=offset;
            auto const& image{*actual.work->debug_image()};
            if(site==debug_stop_site::before_instruction||site==debug_stop_site::function_boundary)
            {
                bool proven{};
                for(auto const& row:image.functions())
                {
                    if(row.module!=module||row.function!=function||row.raw_adapter)continue;
                    if(site==debug_stop_site::function_boundary)
                    {proven=image.backend()==debug_backend::llvm&&offset==UINT64_MAX&&instruction==SIZE_MAX;break;}
                    if(!image.exact_logical_pc_map()||row.logical_begin>image.logical_pcs().size()||
                       row.logical_count>image.logical_pcs().size()-row.logical_begin)break;
                    for(::std::size_t i{};i!=row.logical_count;++i)
                    {
                        auto const& pc{image.logical_pcs()[row.logical_begin+i]};
                        if(pc.function==function&&pc.instruction==instruction&&pc.wasm_offset==offset){proven=true;break;}
                    }
                    break;
                }
                if(!proven)return false;
            }
            else {cell.instruction=SIZE_MAX;cell.wasm_offset=UINT64_MAX;}
            if(actual.debug_poll_sequence==UINT64_MAX)return false;
            ++actual.debug_poll_sequence;actual.debug_cell=cell;
            serial=d.serial;participant=actual.debug_participant;
            actual.debug_poll_serial=serial;actual.debug_in_poll=true;++d.polling;
            location={cell.slot+1u,cell.function,cell.wasm_offset,cell.source_generation};
        }
        // This exact participant is stack-owned by native_entry until AFTER
        // debug_control_worker_leave. No allocator/provider/shard/G lock lives.
        // Drain safety evidence before P marks this worker parked. A queued MTE
        // fault must not disappear behind an apparently coherent saved stop.
        actual.work->synchronize_debug_stop();
        if(auto* journal=current_thread_journal)journal->journal->release(*journal);
        participant->poll(location);
        {
            ::std::lock_guard lock{mutex_};
            if(!actual.debug_in_poll||actual.debug_poll_serial!=serial||d.polling==0u)::std::terminate();
            actual.debug_in_poll=false;--d.polling;
            if(d.phase==debug_control_phase::resuming&&d.domain_resumed&&d.polling==0u)d.phase=debug_control_phase::idle;
            changes_.notify_all();
        }
        // Ordinary resume can return this already-parked waiter while a
        // concurrent stop has published G.closed/stop_requested but not its
        // actual token. Only the genuine token may make this final refusal.
        // If still false, preserve original wait arbitration until its real
        // stop callback wakes W. Ordered P.close also follows that actual token.
        return !cancellation_.stop_requested();
    }
    inline bool ThreadedGroup::debug_suspend_wait(
        ::uwvm2::utils::thread::keyed_wait_set::suspension_borrow const&) noexcept
    {
        // Called ONLY by the installed typed execution scope, with its genuine
        // linked wait node and the shard unlocked. The original wait owns its
        // FIFO/deadline arbitration; this callback never inspects/relinks it.
        auto* actual{current_debug_slot_};auto* execution{current_execution};
        if(!actual||!actual->group||!execution||execution->pause!=actual->debug_wait_policy||
           ::std::addressof(execution->domain)!=::std::addressof(actual->group->waits_)||
           execution->cancellation!=actual->group->cancellation_.get_token())return false;
        return actual->group->debug_poll(*actual,debug_stop_site::atomic_wait);
    }
    inline bool ThreadedGroup::debug_host_enter(slot& actual,bool spawn) noexcept
    {
        if(!debug_control_)return true; // Preserve the original none dispatch path.
        for(;;)
        {
            if(!debug_poll(actual,debug_stop_site::host_boundary))return false;
            ::std::lock_guard lock{mutex_};auto& d{*debug_control_};
            if(d.phase==debug_control_phase::closed||stop_requested_.load(::std::memory_order_acquire))continue;
            if(d.phase!=debug_control_phase::idle)continue;
            if(actual.debug_host_active)return false;
            actual.debug_host_active=true;++d.host_inflight;if(spawn)++d.spawn_inflight;return true;
        }
    }
    inline bool ThreadedGroup::debug_retain_host(slot& actual,::std::size_t module,
        ::std::size_t function,::std::uint64_t offset) noexcept
    {
        if(!execution_journal_||!debug_control_||current_debug_slot_!=&actual)return false;
        debug_pause_domain::participant* participant{};
        {
            ::std::lock_guard lock{mutex_};
            if(!actual.debug_host_active||actual.debug_host_retained||!actual.debug_participant||actual.debug_in_poll)return false;
            participant=actual.debug_participant;
        }
        auto location=::uwvm2::utils::thread::cooperative_pause_location{actual.position+1u,function,offset,admission_->source_generation()};
        if(!participant->retain_host_operation(location))return false;
        {
            ::std::lock_guard lock{mutex_};
            actual.debug_host_retained=true;++debug_control_->retained_hosts;
            actual.debug_cell.module=module;actual.debug_cell.function=function;actual.debug_cell.wasm_offset=offset;
            actual.debug_cell.site=debug_stop_site::host_boundary;actual.debug_cell.instruction=SIZE_MAX;
            actual.debug_cell.retained_host=true;changes_.notify_all();
        }
        return true;
    }
    inline bool ThreadedGroup::debug_return_host(slot& actual,bool blocking) noexcept
    {
        if(!debug_control_||current_debug_slot_!=&actual)return false;
        debug_pause_domain::participant* participant{};
        {
            ::std::lock_guard lock{mutex_};
            if(!actual.debug_host_retained)return true;
            participant=actual.debug_participant;if(!participant)return false;
        }
        using result=::uwvm2::utils::thread::retained_host_return_result;
        bool success{};
        if(blocking)success=participant->return_from_host_operation();
        else
        {
            auto state=participant->try_return_from_host_operation();
            if(state==result::paused)return false;
            if(state==result::closed)success=participant->return_from_host_operation();
            else if(state==result::returned)success=true;
            else return false;
        }
        {
            ::std::lock_guard lock{mutex_};
            if(!actual.debug_host_retained||!debug_control_->retained_hosts)::std::terminate();
            actual.debug_host_retained=false;actual.debug_cell.retained_host=false;--debug_control_->retained_hosts;changes_.notify_all();
        }
        return success;
    }
    inline void ThreadedGroup::debug_host_leave(slot& actual,bool spawn) noexcept
    {
        if(!debug_control_)return;
        ::std::lock_guard lock{mutex_};auto& d{*debug_control_};
        if(actual.debug_host_retained||!actual.debug_host_active||d.host_inflight==0u||(spawn&&d.spawn_inflight==0u))::std::terminate();
        actual.debug_host_active=false;--d.host_inflight;if(spawn)--d.spawn_inflight;changes_.notify_all();
    }
    inline void ThreadedGroup::debug_resume(::std::uint64_t serial) noexcept
    {
        if(!debug_control_)return;auto& d{*debug_control_};debug_pause_domain::pause_ticket ticket;
        {
            ::std::lock_guard lock{mutex_};
            if(d.serial!=serial||(d.phase!=debug_control_phase::armed&&d.phase!=debug_control_phase::arming))return;
            d.phase=debug_control_phase::resuming;d.domain_resumed=false;
            d.advancing_participant=0u;d.advancing_sequence=0u;ticket=d.ticket;changes_.notify_all();
        }
        if(ticket)(void)d.pause.resume(ticket); // Stop may already have closed P.
        {
            ::std::lock_guard lock{mutex_};
            if(d.serial==serial&&d.phase==debug_control_phase::resuming)
            {d.domain_resumed=true;if(d.polling==0u)d.phase=debug_control_phase::idle;changes_.notify_all();}
        }
    }
    inline debug_stop_status ThreadedGroup::debug_stop_until(::std::uint64_t epoch,
        ::std::chrono::steady_clock::time_point deadline,threaded_debug_all_stop& output) noexcept
    {
        if(output.serial_!=0u)
        {
            if(output.owner_!=::std::this_thread::get_id())::std::terminate();
            return debug_stop_status::busy;
        }
        if(!debug_control_||epoch!=1u)return debug_stop_status::unavailable;
        if(deadline==::std::chrono::steady_clock::time_point::max()||deadline<=::std::chrono::steady_clock::now())
            return debug_stop_status::timeout;
        auto retained{canonical_owner_.lock()};if(!retained||retained.get()!=this)return debug_stop_status::stale;
        auto& d{*debug_control_};::std::uint64_t serial{};
        {
            ::std::unique_lock lock{mutex_,::std::try_to_lock};if(!lock.owns_lock())return debug_stop_status::busy;
            if(!debug_live(epoch)||d.phase==debug_control_phase::closed)return debug_stop_status::stale;
            if(current_debug_slot_||(compiler_created_&&::pthread_equal(compiler_thread_,::pthread_self()))||
               d.phase!=debug_control_phase::idle||d.polling!=0u||d.serial==UINT64_MAX)return debug_stop_status::busy;
            ::std::size_t count{};if(!debug_stable_roster_locked(count))return debug_stop_status::busy;
            d.expected=count;serial=++d.serial;d.phase=debug_control_phase::arming;
        }
        auto ticket{d.pause.request_pause()};
        bool armed{};
        {
            ::std::lock_guard lock{mutex_};
            if(ticket&&d.serial==serial&&d.phase==debug_control_phase::arming&&!stop_requested_.load(::std::memory_order_acquire))
            {d.ticket=ticket;d.phase=debug_control_phase::armed;armed=true;}
            changes_.notify_all();
        }
        if(!armed){debug_resume(serial);return debug_stop_status::stale;}
        auto const waited{d.pause.wait_until_paused(ticket,deadline)};
        if(waited!=::uwvm2::utils::thread::cooperative_pause_result::paused)
        {debug_resume(serial);return waited==::uwvm2::utils::thread::cooperative_pause_result::timeout?debug_stop_status::timeout:debug_stop_status::stale;}
        ::std::size_t observed{};
        // P copies only its own DATA into fixed scratch. No G/Host/Source lock,
        // allocation, observer, pointer escape or paused_transition in callback.
        bool const captured{d.pause.copy_stopped_observation(ticket,{d.scratch.get(),d.capacity},observed)};
        bool proven{},expired{};
        if(captured)
        {
            ::std::lock_guard lock{mutex_};::std::size_t count{};
            proven=d.phase==debug_control_phase::armed&&d.serial==serial&&debug_stable_roster_locked(count)&&
                count==d.expected&&observed==count&&d.polling+d.retained_hosts==count&&d.host_inflight==d.retained_hosts;
            for(::std::size_t i{};proven&&i!=slot_count_.load(::std::memory_order_acquire);++i)
            {
                auto const& s{slots_[i]};if(s.phase==slot_phase::empty||s.body_returned.load(::std::memory_order_acquire))continue;
                bool found{};
                if(!s.debug_host_retained&&(!s.debug_in_poll||s.debug_poll_serial!=serial)){proven=false;break;}
                for(::std::size_t p{};p!=observed;++p)
                {
                    auto const& parked{d.scratch[p]};auto const& cell{s.debug_cell};
                    if(parked.id==s.debug_participant_id&&parked.retained_host==s.debug_host_retained&&parked.location==::uwvm2::utils::thread::cooperative_pause_location{
                        i+1u,cell.function,cell.wasm_offset,cell.source_generation}){found=true;break;}
                }
                if(!found)proven=false;
            }
            if(proven&&::std::chrono::steady_clock::now()>=deadline){proven=false;expired=true;}
            if(proven)
            {
                output.group_=retained;output.admission_=admission_;output.serial_=serial;
                output.ticket_=ticket;output.owner_=::std::this_thread::get_id();
                output.native_process_=static_cast<::std::uint64_t>(::getpid());
            }
        }
        if(!proven){debug_resume(serial);return expired?debug_stop_status::timeout:debug_stop_status::busy;}
        return debug_stop_status::stopped;
    }
    inline debug_stop_status ThreadedGroup::debug_read_stopped(threaded_debug_all_stop const& guard,
        ::std::span<debug_stopped_worker> output,::std::size_t& needed) noexcept
    {
        needed=0u;
        // This local admission, and read()'s local Group owner, end on return.
        // Declare BEFORE G so no possibly-last strong owner dies under G/P.
        auto actual_admission{guard.admission_.lock()};
        if(!debug_control_||!actual_admission)return debug_stop_status::stale;
        ::std::lock_guard lock{mutex_};auto const& d{*debug_control_};
        if(guard.serial_==0u||guard.group_.owner_before(canonical_owner_)||canonical_owner_.owner_before(guard.group_)||
           actual_admission.get()!=admission_.get()||
           actual_admission.owner_before(admission_)||admission_.owner_before(actual_admission)||
           !guard.ticket_||guard.serial_!=d.serial||d.phase!=debug_control_phase::armed||!debug_live(1u))
            return debug_stop_status::stale;
        if(d.advancing_participant)return debug_stop_status::busy;
        ::std::size_t count{};
        if(!debug_stable_roster_locked(count)||count!=d.expected||d.polling+d.retained_hosts!=count||d.host_inflight!=d.retained_hosts)return debug_stop_status::stale;
        needed=count;if(output.size()<count)return debug_stop_status::insufficient_capacity;
        ::std::size_t n{};
        for(::std::size_t i{};i!=slot_count_.load(::std::memory_order_acquire);++i)
        {
            auto const& s{slots_[i]};if(s.phase==slot_phase::empty||s.body_returned.load(::std::memory_order_acquire))continue;
            if(!s.debug_host_retained&&(!s.debug_in_poll||s.debug_poll_serial!=d.serial)){needed=0u;return debug_stop_status::stale;}
            output[n++]=s.debug_cell;
        }
        if(!debug_live(1u)||stop_requested_.load(::std::memory_order_acquire)){needed=0u;return debug_stop_status::stale;}
        return debug_stop_status::stopped;
    }
    inline debug_stop_status ThreadedGroup::debug_read_retained(threaded_debug_all_stop const& guard,
        ::std::span<debug_retained_worker> output,::std::size_t& needed) noexcept
    {
        needed=0u;
        auto actual_admission=guard.admission_.lock();
        if(!debug_control_||!actual_admission)return debug_stop_status::stale;
        ::std::lock_guard lock{mutex_};auto const& d=*debug_control_;
        if(!guard.serial_||guard.group_.owner_before(canonical_owner_)||canonical_owner_.owner_before(guard.group_)||
           actual_admission.get()!=admission_.get()||actual_admission.owner_before(admission_)||
           admission_.owner_before(actual_admission)||!guard.ticket_||guard.serial_!=d.serial||
           d.phase!=debug_control_phase::armed||!debug_live(1u))return debug_stop_status::stale;
        if(d.advancing_participant)return debug_stop_status::busy;
        ::std::size_t live{};
        if(!debug_stable_roster_locked(live)||live!=d.expected||d.polling+d.retained_hosts!=live||d.host_inflight!=d.retained_hosts)
            return debug_stop_status::stale;
        auto const count=slot_count_.load(::std::memory_order_acquire);
        for(::std::size_t i{};i<count;++i)
        {
            auto const& worker=slots_[i];if(worker.phase==slot_phase::empty)continue;
            if(!worker.body_returned.load(::std::memory_order_acquire)&&
               (!worker.debug_host_retained&&(!worker.debug_in_poll||worker.debug_poll_serial!=d.serial)))return debug_stop_status::stale;
            ++needed;
        }
        if(output.size()<needed)return debug_stop_status::insufficient_capacity;
        ::std::size_t index{};
        for(::std::size_t i{};i<count;++i)
        {
            auto const& worker=slots_[i];if(worker.phase==slot_phase::empty)continue;
            output[index++]={worker.tid,worker.argument,worker.debug_incarnation,
                admission_->source_generation(),admission_->source_validation_epoch(),i,
                worker.work->debug_image()->source_module(),worker.work->debug_image()->backend(),
                worker.initial_main,worker.body_returned.load(::std::memory_order_acquire)};
        }
        if(!debug_live(1u)||stop_requested_.load(::std::memory_order_acquire))
        {needed=0u;return debug_stop_status::stale;}
        return debug_stop_status::stopped;
    }
    inline debug_stop_status ThreadedGroup::debug_advance_until(threaded_debug_all_stop const& guard,
        ::std::uint64_t participant,::std::chrono::steady_clock::time_point deadline) noexcept
    {
        if(!participant)return debug_stop_status::unavailable;
        if(deadline==::std::chrono::steady_clock::time_point::max()||deadline<=::std::chrono::steady_clock::now())
            return debug_stop_status::timeout;
        auto admitted=guard.admission_.lock();
        if(!debug_control_||!admitted)return debug_stop_status::stale;
        auto& d=*debug_control_;bool release{};::std::uint64_t sequence{};
        {
            ::std::lock_guard lock{mutex_};
            if(!guard.serial_||guard.group_.owner_before(canonical_owner_)||canonical_owner_.owner_before(guard.group_)||
               admitted.get()!=admission_.get()||admitted.owner_before(admission_)||admission_.owner_before(admitted)||
               !guard.ticket_||guard.serial_!=d.serial||d.phase!=debug_control_phase::armed||!debug_live(1u))
                return debug_stop_status::stale;
            if(d.advancing_participant)
            {
                if(d.advancing_participant!=participant)return debug_stop_status::busy;
                sequence=d.advancing_sequence; // Join the prior transfer; never release it twice.
            }
            else
            {
                ::std::size_t count{};
                if(!debug_stable_roster_locked(count)||count!=d.expected||d.polling+d.retained_hosts!=count||d.host_inflight!=d.retained_hosts)
                    return debug_stop_status::stale;
                slot const* selected{};
                for(::std::size_t i{};i!=slot_count_.load(::std::memory_order_acquire);++i)
                {
                    auto const& candidate=slots_[i];
                    if(candidate.phase!=slot_phase::empty&&candidate.debug_participant_id==participant)
                    {selected=&candidate;break;}
                }
                if(!selected)return debug_stop_status::unavailable;
                // A blocked native provider/wait is NOT an executable opcode.
                // Completing its replay needs its own recorded phase transition.
                if(!selected->debug_in_poll||selected->debug_poll_serial!=d.serial||
                   selected->debug_cell.site!=debug_stop_site::before_instruction||
                   selected->debug_poll_sequence==UINT64_MAX)return debug_stop_status::unavailable;
                if(::std::chrono::steady_clock::now()>=deadline)return debug_stop_status::timeout;
                sequence=selected->debug_poll_sequence;
                d.advancing_sequence=sequence;d.advancing_participant=participant;release=true;
            }
        }
        // P is entered with no G/provider/source lock. Mark the observation
        // unavailable BEFORE releasing the worker: read() cannot return old PCs.
        if(release&&!d.pause.release_one_to_next_poll(guard.ticket_,participant))
        {
            ::std::lock_guard lock{mutex_};
            if(d.serial==guard.serial_&&d.advancing_participant==participant&&d.advancing_sequence==sequence)
            {d.advancing_participant=0u;d.advancing_sequence=0u;}
            return debug_stop_status::stale;
        }
        auto waited=d.pause.wait_until_paused(guard.ticket_,deadline);
        if(waited!=::uwvm2::utils::thread::cooperative_pause_result::paused)
            return waited==::uwvm2::utils::thread::cooperative_pause_result::timeout?
                debug_stop_status::timeout:debug_stop_status::stale;
        ::std::size_t observed{};
        bool captured=d.pause.copy_stopped_observation(guard.ticket_,{d.scratch.get(),d.capacity},observed);
        if(!captured)return debug_stop_status::stale;
        {
            ::std::lock_guard lock{mutex_};::std::size_t count{};bool advanced{};
            if(d.serial!=guard.serial_||d.phase!=debug_control_phase::armed||d.advancing_participant!=participant||
               d.advancing_sequence!=sequence||!debug_stable_roster_locked(count)||count!=d.expected||
               observed!=count||d.polling+d.retained_hosts!=count||d.host_inflight!=d.retained_hosts)return debug_stop_status::stale;
            for(::std::size_t i{};i!=slot_count_.load(::std::memory_order_acquire);++i)
            {
                auto const& worker=slots_[i];if(worker.phase==slot_phase::empty||worker.body_returned.load(::std::memory_order_acquire))continue;
                if(!worker.debug_host_retained&&(!worker.debug_in_poll||worker.debug_poll_serial!=d.serial))return debug_stop_status::stale;
                auto const& cell=worker.debug_cell;bool proven{};
                for(::std::size_t j{};j!=observed;++j)
                    if(d.scratch[j].id==worker.debug_participant_id&&d.scratch[j].retained_host==worker.debug_host_retained&&d.scratch[j].location==
                       ::uwvm2::utils::thread::cooperative_pause_location{i+1u,cell.function,cell.wasm_offset,cell.source_generation})
                    {proven=true;break;}
                if(!proven)return debug_stop_status::stale;
                if(worker.debug_participant_id==participant)advanced=worker.debug_poll_sequence>sequence;
            }
            if(!advanced)return debug_stop_status::stale;
            if(::std::chrono::steady_clock::now()>=deadline)return debug_stop_status::timeout;
            d.advancing_participant=0u;d.advancing_sequence=0u;
        }
        return debug_stop_status::stopped;
    }
    inline threaded_debug_all_stop::~threaded_debug_all_stop(){reset();}
    inline void threaded_debug_all_stop::reset() noexcept
    {
        if(serial_==0u)return;
        // fork preserves mutex/weak-owner bytes, not the genuine native group.
        // Discard a copied observer without touching inherited G/P locks or
        // attempting to resume the parent's still-paused execution.
        if(native_process_!=static_cast<::std::uint64_t>(::getpid()))
        {group_.reset();admission_.reset();serial_=native_process_=0u;ticket_={};owner_={};return;}
        if(owner_!=::std::this_thread::get_id())::std::terminate();
        auto observed_group{group_};auto observed_admission{admission_};auto const serial{serial_};
        // Clear this issued guard FIRST. Expired observers only retire identity;
        // they do not recreate a Group/P or retain an execution-generation lease.
        group_.reset();admission_.reset();serial_=native_process_=0u;ticket_={};owner_={};
        auto actual{observed_group.lock()};auto admitted{observed_admission.lock()};
        if(actual&&admitted&&!observed_group.owner_before(actual->canonical_owner_)&&
           !actual->canonical_owner_.owner_before(observed_group)&&admitted.get()==actual->admission_.get()&&
           !admitted.owner_before(actual->admission_)&&!actual->admission_.owner_before(admitted))
            actual->debug_resume(serial); // Private G serial/phase check; stale never releases a later episode.
        // All local strong owners die here, outside G/P/registry, before callers
        // can enter a public synchronous provider-generation drain.
    }
    inline debug_stop_status threaded_debug_all_stop::read(::std::span<debug_stopped_worker> output,::std::size_t& needed) const noexcept
    {
        needed=0u;if(serial_==0u)return debug_stop_status::unavailable;
        if(native_process_!=static_cast<::std::uint64_t>(::getpid()))return debug_stop_status::stale;
        if(owner_!=::std::this_thread::get_id())::std::terminate();
        auto actual{group_.lock()};
        return actual?actual->debug_read_stopped(*this,output,needed):debug_stop_status::stale;
        // The genuine operation-local owner is destroyed before returning. An
        // issued expired guard stays stale and keeps its same-thread contract.
    }
    inline debug_stop_status threaded_debug_all_stop::read_retained(
        ::std::span<debug_retained_worker> output,::std::size_t& needed) const noexcept
    {
        needed=0u;if(!serial_)return debug_stop_status::unavailable;
        if(native_process_!=static_cast<::std::uint64_t>(::getpid()))return debug_stop_status::stale;
        if(owner_!=::std::this_thread::get_id())::std::terminate();
        auto actual=group_.lock();
        return actual?actual->debug_read_retained(*this,output,needed):debug_stop_status::stale;
    }
    inline debug_stop_status threaded_debug_all_stop::advance_until(::std::uint64_t participant,
        ::std::chrono::steady_clock::time_point deadline) noexcept
    {
        if(!serial_)return debug_stop_status::unavailable;
        if(native_process_!=static_cast<::std::uint64_t>(::getpid()))return debug_stop_status::stale;
        if(owner_!=::std::this_thread::get_id())::std::terminate();
        auto actual=group_.lock();
        return actual?actual->debug_advance_until(*this,participant,deadline):debug_stop_status::stale;
    }
    inline debug_stop_status threaded_debug_control_session::stop_until(::std::chrono::steady_clock::time_point deadline,
        threaded_debug_all_stop& output) const noexcept
    {
        if(output.serial_!=0u)
        {
            if(output.native_process_!=static_cast<::std::uint64_t>(::getpid()))return debug_stop_status::stale;
            if(output.owner_!=::std::this_thread::get_id())::std::terminate();
            return debug_stop_status::busy; // Even an expired issued output needs reset first.
        }
        if(!epoch_)return debug_stop_status::unavailable;
        if(native_process_!=static_cast<::std::uint64_t>(::getpid()))return debug_stop_status::stale;
        auto actual{group_.lock()};return actual?actual->debug_stop_until(epoch_,deadline,output):debug_stop_status::stale;
    }
