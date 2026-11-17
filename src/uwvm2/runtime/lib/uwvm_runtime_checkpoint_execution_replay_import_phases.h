// Included inside checkpoint_replay_import. A ROS-v1 provider holds no
// scheduler turn while acquiring native locks or waiting for a peer. Each real
// host memory access acquires a turn and records an indivisible transaction.
// Replay consumes these transactions; it never enters the native provider.
    void acquire_phase(::std::unique_lock<::std::mutex>& lock,bool replay)
    {
        for(;;)
        {
            session_->consume_ready();
            if(session_->mode!=(replay?3u:1u) || session_->epoch!=current_runtime_generation() ||
               !g_debug_pause_participant || g_debug_pause_participant->identifier()!=participant_ ||
               session_->logical_thread(participant_)!=logical_)reject(error::replay_diverged);
            if(!replay ? session_->recording_turn(logical_) :
                (!session_->running && session_->cursor<session_->events.size() &&
                 session_->events[session_->cursor].words[2]==logical_))
            {
                if(!replay && retained_host_)
                {
                    // The next guest turn is not reserved while global pause
                    // retains the native result. In particular a different
                    // worker released for one step must still be able to run.
                    // Under the journal lock, atomically leave retention only
                    // if the domain is running, then own the memory phase.
                    using returned=::uwvm2::utils::thread::retained_host_return_result;
                    auto const status=g_debug_pause_participant->try_return_from_host_operation();
                    if(status==returned::paused)
                    {
                        session_->changed.notify_all();
                        session_->changed.wait_for(lock,::std::chrono::milliseconds{1});
                        continue;
                    }
                    if(status!=returned::returned)reject(error::replay_diverged);
                    retained_host_=false;
                }
                session_->running=participant_;
                return;
            }
            if(replay)checkpoint_replay_import_schedule_poll(*session_,lock);
            else
            {
                // A live provider may own native locks and has no serializable
                // continuation here. Only the virtual replay branch can park.
                session_->changed.wait_for(lock,::std::chrono::milliseconds{1});
            }
        }
    }
    void release_phase() noexcept
    {
        if(session_->running!=participant_)reject(error::replay_diverged);
        if(session_->mode==1u)
        {
            if(retained_host_ || !g_debug_wait_point.valid || !g_debug_pause_participant ||
               !g_debug_pause_participant->retain_host_operation(g_debug_wait_point.location))
                reject(error::replay_diverged);
            retained_host_=true;
        }
        session_->running=0u;session_->changed.notify_all();
    }
    void save_before()
    {
        before_.clear();before_.reserve(memories_.size());
        for(auto const& memory:memories_)before_.emplace_back(memory.bytes.begin(),memory.bytes.end());
    }
    void encode_memory(object& event)
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        ::uwvm2::utils::container::std_vector<image> before{},after{};
        event.links.clear();
        for(::std::size_t i{};i<memories_.size();++i)
        {
            before.push_back({memories_[i].memory,before_[i]});after.push_back({memories_[i].memory,memories_[i].bytes});
            event.links.push_back(memories_[i].memory);
        }
        auto checked=cp::encode_replay_memory_effect(before,after,event.bytes);
        if(checked!=error::none)reject(checked);
    }
    void apply_memory(object const& event,bool immutable=false)
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        if(event.links.size()!=memories_.size())reject(error::replay_diverged);
        for(::std::size_t i{};i<memories_.size();++i)
            if(event.links[i]!=memories_[i].memory)reject(error::replay_diverged);
        ::uwvm2::utils::container::std_vector<cp::replay_memory_detail::write> writes{};
        auto checked=cp::replay_memory_detail::prepare(event.bytes,memories_,writes);
        if(checked!=error::none)reject(checked);
        if(immutable && !writes.empty())reject(error::replay_diverged);
        for(auto const& write:writes)::std::memcpy(write.target.data(),write.value.data(),write.value.size());
        if(!writes.empty())for(auto& owner:owners_)if(!copy_memory(owner,true))reject(error::replay_diverged);
    }
    void copy_results(object const& event)
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        ::std::size_t length{},index{};
        for(::std::size_t n{};n<target_->signature().results.size;++n)
        {
            auto type=target_->signature().results.at(n);auto count=width(type);
            if(!count || index>=event.values.size() || event.values[index].type.kind!=static_cast<cp::value_kind>(0x80u-static_cast<unsigned>(type)) ||
               length>result_bytes_ || count>result_bytes_-length)reject(error::incompatible_type);
            length+=count;++index;
        }
        if(length!=result_bytes_ || index!=event.values.size())reject(error::incompatible_type);
        auto output=reinterpret_cast<::std::byte*>(results_);::std::size_t offset{};index=0u;
        for(::std::size_t n{};n<target_->signature().results.size;++n)
        {
            auto count=width(target_->signature().results.at(n));auto const& value=event.values[index++];
            if(count==4u){auto scalar=static_cast<::std::uint32_t>(value.low_bits);::std::memcpy(output+offset,&scalar,4u);}
            else {::std::memcpy(output+offset,&value.low_bits,8u);if(count==16u)::std::memcpy(output+offset+8u,&value.high_bits,8u);}
            offset+=count;
        }
    }
    void begin_phased(::std::unique_lock<::std::mutex>& lock,bool replay)
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;namespace ex=cp::execution_replay;
        if(!replay)
        {
            save_before();encode_memory(event_);begin_sequence_=event_.words[0];
            session_->append(event_);active_=this;
            if(logical_>session_->retained_imports.size() || session_->retained_imports[logical_-1u])reject(error::replay_diverged);
            session_->retained_imports[logical_-1u]=begin_sequence_;
            namespace reports=::uwvm2::runtime::memtag::report;
            if(reports::current_observer)reject(error::non_replayable_import);
            report_observer_={this,[](void* context,reports::observation const& report) noexcept
            {
                auto* producer=static_cast<checkpoint_replay_import*>(context);
                if(active_!=producer)::fast_io::fast_terminate();
                producer->record_host_report(report);
            }};
            reports::current_observer=&report_observer_;
            if(wasi_phased_)
            {
                namespace host=::uwvm2::runtime::checkpoint::host_memory_phase;
                if(host::current)reject(error::replay_diverged);
                wasi_consumer_={this,
                    [](void* context,bool write) noexcept
                    {
                        auto* producer=static_cast<checkpoint_replay_import*>(context);
                        if(active_!=producer)::fast_io::fast_terminate();
                        producer->enter_memory(write?2u:1u,0u,0u,0u);
                    },
                    [](void* context) noexcept
                    {
                        auto* producer=static_cast<checkpoint_replay_import*>(context);
                        if(active_!=producer)::fast_io::fast_terminate();
                        producer->leave_memory(true);
                    }};
                host::current=&wasi_consumer_;
            }
            release_phase();return;
        }
        if(session_->cursor>=session_->events.size())reject(error::replay_diverged);
        auto const& begin=session_->events[session_->cursor];
        if(begin.flags!=ex::import_begin_kind || begin.words[1]!=event_.words[1] || begin.words[2]!=logical_ ||
           begin.words[3]!=event_.words[3] || begin.words[4]!=event_.words[4] || begin.words[5] ||
           begin.values!=event_.values)reject(error::replay_diverged);
        apply_memory(begin,true);begin_sequence_=begin.words[0];session_->consume();release_phase();
        for(;;)
        {
            acquire_phase(lock,true);
            auto const& phase=session_->events[session_->cursor];
            if(phase.words[1]!=session_->instructions || phase.words[2]!=logical_ || phase.words[3]!=event_.words[3])reject(error::replay_diverged);
            if(phase.flags==ex::tags_kind)
            {
                if(!collect_memories())reject(error::replay_diverged);
                tag_guard(true);release_phase();continue;
            }
            if(phase.words[7]!=begin_sequence_)reject(error::replay_diverged);
            if(phase.flags==ex::import_memtag_report_kind)
            {
                if(!::uwvm2::extension::memtag::logging_requested())reject(error::unavailable_capability);
                auto v=[&](::std::size_t n){return phase.values[n].low_bits;};
                namespace mt=::uwvm2::extension::memtag;
                mt::access_result report{mt::access_error::tag_mismatch,v(0),v(1),
                    static_cast<::std::uint8_t>(v(7)),static_cast<::std::uint8_t>(v(8))};
                auto const module=v(4)==UINT64_MAX?UINT64_MAX:session_->actual_modules[v(4)];
                // Reproduce the diagnostic only. Neither the entropy source nor
                // a WASI/ROS native provider is reentered. The recorded source
                // module is rebound to its authenticated current logical owner.
                ::uwvm2::runtime::memtag::report::violation(report,v(2),v(3),module,v(5),v(6),v(9)!=0u);
                session_->consume();release_phase();continue;
            }
            if(phase.flags==ex::import_finish_kind)
            {
                copy_results(phase);session_->consume();completed_=replayed_=true;return;
            }
            if(phase.flags!=ex::import_memory_kind || !collect_memories())reject(error::replay_diverged);
            apply_memory(phase,phase.values[0].low_bits!=2u || !phase.values[4].low_bits);session_->consume();release_phase();
        }
    }
    void finish_phased()
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        ::std::unique_lock lock{session_->mutex};
        try
        {
            if(active_!=this || accessing_)reject(error::replay_diverged);
            acquire_phase(lock,false);object finish{};finish.kind=cp::object_kind::event;finish.flags=cp::execution_replay::import_finish_kind;
            if(!read_values(target_->signature().results,results_,result_bytes_,finish.values))reject(error::incompatible_type);
            finish.words={session_->events.size()+1u,session_->instructions,logical_,event_.words[3],0u,finish.values.size(),cp::execution_replay::schema,begin_sequence_};
            if(wasi_phased_)
            {
                auto& current=::uwvm2::runtime::checkpoint::host_memory_phase::current;
                if(current!=&wasi_consumer_)reject(error::replay_diverged);
                current=nullptr;
            }
            auto& reports=::uwvm2::runtime::memtag::report::current_observer;
            if(reports!=&report_observer_)reject(error::replay_diverged);
            reports=nullptr;
            session_->append(::std::move(finish));completed_=true;active_=nullptr;
            if(session_->retained_imports[logical_-1u]!=begin_sequence_)reject(error::replay_diverged);
            session_->retained_imports[logical_-1u]=0u;
            // Return delivery owns this turn until the next actual opcode.
        }
        catch(...){reject(error::limit_exceeded);}
    }
public:
    void record_host_report(::uwvm2::runtime::memtag::report::observation const& report) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        ::std::unique_lock lock{session_->mutex};
        try
        {
            if(active_!=this || !phased_ || session_->mode!=1u ||
               !::uwvm2::extension::memtag::logging_requested() ||
               report.access.error!=::uwvm2::extension::memtag::access_error::tag_mismatch)
                reject(error::replay_diverged);
            // Preflight can diagnose outside a memory transaction. Serialize the
            // detection with actual instruction turns before appending the event;
            // otherwise a different worker's entropy event could overtake it.
            bool const own_turn=!accessing_;
            if(own_turn)acquire_phase(lock,false);
            else if(session_->running!=participant_)reject(error::replay_diverged);
            auto const module=report.module==UINT64_MAX?UINT64_MAX:session_->logical_module(report.module);
            if((report.module!=UINT64_MAX && module==SIZE_MAX) ||
               (report.function!=UINT64_MAX && report.function>UINT32_MAX) ||
               (report.offset!=UINT64_MAX && report.offset>UINT32_MAX))reject(error::non_replayable_import);
            object event{};event.kind=cp::object_kind::event;event.flags=cp::execution_replay::import_memtag_report_kind;
            event.words={session_->events.size()+1u,session_->instructions,logical_,event_.words[3],10u,0u,
                cp::execution_replay::schema,begin_sequence_};
            for(::std::uint64_t number:{report.access.address,report.access.mismatch_granule,report.raw,report.memory,
                module,report.function,report.offset,static_cast<::std::uint64_t>(report.access.pointer_tag),
                static_cast<::std::uint64_t>(report.access.allocation_tag),static_cast<::std::uint64_t>(report.asynchronous)})
            {value scalar{};scalar.type.kind=cp::value_kind::i64;scalar.low_bits=number;event.values.push_back(scalar);}
            session_->append(::std::move(event));
            if(own_turn)release_phase();
        }
        catch(...){reject(error::limit_exceeded);}
    }
    [[nodiscard]] static checkpoint_replay_import* active_memory_producer() noexcept{return active_;}
    void enter_memory(unsigned operation,::std::size_t memory,::std::uint64_t offset,::std::size_t size) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        ::std::unique_lock lock{session_->mutex};
        try
        {
            if(active_!=this || accessing_ || !phased_ || operation>2u)reject(error::replay_diverged);
            acquire_phase(lock,false);if(!collect_memories())reject(error::replay_diverged);
            tag_guard(false);save_before();access_event_={};access_event_.kind=cp::object_kind::event;access_event_.flags=cp::execution_replay::import_memory_kind;
            access_event_.words={0u,0u,logical_,event_.words[3],4u,1u,cp::execution_replay::schema,begin_sequence_};
            for(auto bits:{static_cast<::std::uint64_t>(operation),static_cast<::std::uint64_t>(memory),offset,static_cast<::std::uint64_t>(size)})
            {value scalar{};scalar.type.kind=cp::value_kind::i64;scalar.low_bits=bits;access_event_.values.push_back(scalar);}
            accessing_=true;
        }
        catch(...){reject(error::limit_exceeded);}
    }
    void leave_memory(bool accepted) noexcept
    {
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        ::std::lock_guard lock{session_->mutex};
        try
        {
            if(active_!=this || !accessing_ || session_->mode!=1u || session_->running!=participant_)reject(error::replay_diverged);
            for(auto& owner:owners_)if(!copy_memory(owner,false))reject(error::replay_diverged);
            if(access_event_.values[0].low_bits!=2u || !accepted)
                for(::std::size_t i{};i<owners_.size();++i)
                    if(before_[i]!=owners_[i].image)reject(error::non_replayable_import);
            encode_memory(access_event_);value result{};result.type.kind=cp::value_kind::i64;result.low_bits=accepted;access_event_.values.push_back(result);
            access_event_.words[0]=session_->events.size()+1u;access_event_.words[1]=session_->instructions;
            session_->append(::std::move(access_event_));accessing_=false;release_phase();
        }
        catch(...){reject(error::limit_exceeded);}
    }
private:
