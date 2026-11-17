// Same-process retained native continuations. No native stack, pointer, lock or
// unknown result is serialized. An archive can only select an independently
// retained operation at the exact original all-stop episode.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
extern "C++" {
    class llvm_jit_checkpoint_pending_snapshot final
    {
        friend llvm_jit_checkpoint_pending_capture llvm_jit_checkpoint_capture_pending_host_api(
            ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const&) noexcept;
        friend bool llvm_jit_checkpoint_validate_pending_host_api(
            ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const&,
            llvm_jit_checkpoint_pending_snapshot_owner const&) noexcept;
        ::std::weak_ptr<runtime_checkpoint_replay_session> session_{};
        ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket pause_{};
        ::std::uint64_t epoch_{},instructions_{};
#if defined(__unix__) || defined(__APPLE__)
        ::std::uint64_t process_{};
#endif
        ::std::size_t events_{};
        ::uwvm2::utils::container::std_vector<::std::uint64_t> pending_{};
    public:
        llvm_jit_checkpoint_pending_snapshot()=default;
    };
    static bool checkpoint_pending_cohort(runtime_checkpoint_replay_session const& session,
        ::uwvm2::utils::thread::cooperative_pause_domain::snapshot const& cohort) noexcept
    {
        using result=::uwvm2::utils::thread::cooperative_pause_result;
        if(cohort.result!=result::paused || cohort.participants.empty() || session.mode!=1u ||
           session.epoch!=current_runtime_generation() || session.retained_imports.size()!=session.participants.size())return false;
        ::std::size_t live{},pending{};
        for(::std::size_t n{};n<session.participants.size();++n)
        {
            if(session.exited[n])continue;
            ++live;
            auto row=::std::find_if(cohort.participants.begin(),cohort.participants.end(),
                [&](auto const& item){return item.id==session.participants[n];});
            if(row==cohort.participants.end() || row->location.code_generation!=session.epoch ||
               row->retained_host!=(session.retained_imports[n]!=0u))return false;
            if(row->retained_host)++pending;
        }
        return pending && live==cohort.participants.size();
    }
    extern "C++" llvm_jit_checkpoint_pending_capture llvm_jit_checkpoint_capture_pending_host_api(
        ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const& ticket) noexcept
    {
        namespace permission=::uwvm2::utils::control;
        namespace cp=::uwvm2::uwvm::debugger::checkpoint;
        if(!checkpoint_replay_original_process())return {};
        if(!permission::debug_permissions::full_allowed() ||
           permission::dump_permissions::permit(permission::dump_permissions::disclosure::full).status!=permission::dump_permissions::denial::none)return {};
        auto domain=g_runtime.debug_pause_control;auto session=checkpoint_replay_current();
        if(!domain || !session)return {};
        try
        {
            ::std::lock_guard lock{session->mutex};
            auto cohort=domain->capture(ticket);
            if(!checkpoint_pending_cohort(*session,cohort))return {};
            auto token=::uwvm2::utils::container::make_shared<llvm_jit_checkpoint_pending_snapshot>();
            token->session_=session;token->pause_=ticket;token->epoch_=session->epoch;
#if defined(__unix__) || defined(__APPLE__)
            token->process_=static_cast<::std::uint64_t>(::getpid());
#endif
            token->instructions_=session->instructions;token->events_=session->events.size();token->pending_=session->retained_imports;
            auto graph=::uwvm2::utils::container::make_shared<cp::state>(session->origin);
            graph->objects.insert(graph->objects.end(),session->events.begin(),session->events.end());
            if(cp::validate_graph(*graph)!=cp::error::none)return {};
            // Canonical owner comparison precedes dereferencing any supplied
            // opaque owner. Expired observations are reclaimed without a cap.
            ::std::erase_if(session->retained_snapshots,[](auto const& item){return item.expired();});
            session->retained_snapshots.push_back(token);
            return {::std::move(token),::std::move(graph)};
        }
        catch(...){return {};}
    }
    extern "C++" bool llvm_jit_checkpoint_validate_pending_host_api(
        ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const& ticket,
        llvm_jit_checkpoint_pending_snapshot_owner const& supplied) noexcept
    {
        if(!checkpoint_replay_original_process())return false;
        if(!::uwvm2::utils::control::debug_permissions::full_allowed())return false;
        auto domain=g_runtime.debug_pause_control;auto session=checkpoint_replay_current();
        if(!domain || !session || !supplied)return false;
        try
        {
            ::std::lock_guard lock{session->mutex};
            llvm_jit_checkpoint_pending_snapshot_owner actual{};
            for(auto const& item:session->retained_snapshots)
            {
                auto owner=item.lock();
                if(owner && owner.get()==supplied.get() && !owner.owner_before(supplied) && !supplied.owner_before(owner))
                {actual=::std::move(owner);break;}
            }
            if(!actual)return false;
#if defined(__unix__) || defined(__APPLE__)
            if(actual->process_!=static_cast<::std::uint64_t>(::getpid()))return false;
#endif
            if(actual->session_.lock()!=session || actual->epoch_!=session->epoch ||
               actual->instructions_!=session->instructions || actual->events_!=session->events.size() ||
               actual->pending_!=session->retained_imports || !domain->same_current_pause(ticket,actual->pause_))return false;
            return checkpoint_pending_cohort(*session,domain->capture(ticket));
        }
        catch(...){return false;}
    }
}
#else
extern "C++" llvm_jit_checkpoint_pending_capture llvm_jit_checkpoint_capture_pending_host_api(
    ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const&) noexcept{return {};}
extern "C++" bool llvm_jit_checkpoint_validate_pending_host_api(
    ::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket const&,
    llvm_jit_checkpoint_pending_snapshot_owner const&) noexcept{return false;}
#endif
