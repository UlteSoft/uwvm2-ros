// SPDX-License-Identifier: Apache-2.0
// Include INSIDE the real bridge AFTER profile_state/control/lifecycle exist.
public:
    [[nodiscard]] static bool configure_signal(threaded_signal_policy const& selected) noexcept
    {
        if(!signal_policy_admitted(selected)||current_host_!=nullptr)return false;
#if !defined(UWVM_IMPORT_WASI_WASIP1) || defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
        if(selected.read_fd!=0u)return false;
#endif
        ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};
        if(!registry.owns_lock()||control_owner().profile||
            control_owner().blocked_generation==current_runtime_generation())return false;
        return g_runtime.execution_domain.with_quiescent_configuration([&]()noexcept{control_owner().signal_policy=selected;});
    }
    [[nodiscard]] static ::std::shared_ptr<threaded_signal_sender const> signal_sender() noexcept
    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
        if(current_host_!=nullptr)return {};
        ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};if(!registry.owns_lock())return {};
        auto const& state{control_owner().profile};
        if(!state||state->joined||!state->native||!state->admitted||!state->signal||!state->signal_sender)return {};
        auto maintenance{g_runtime.execution_domain.try_maintenance_transition()};if(!maintenance)return {};
        auto generation{state->gate.try_enter()};
        if(!generation||!current_source(*state->admitted)||state->native->stop_requested_.load(::std::memory_order_acquire))return {};
        return state->signal_sender;
#else
        return {};
#endif
    }
    [[nodiscard]] static int push_signal(threaded_signal_sender const& sender,threaded_signal_event event) noexcept
    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
        if(current_host_!=nullptr)return EBUSY;
        ::std::shared_ptr<signal_owner> owner;::std::shared_ptr<group> native;
        ::uwvm2::utils::thread::execution_domain::lease generation;
        {
            ::std::unique_lock registry{control_owner().registry,::std::try_to_lock};if(!registry.owns_lock())return EBUSY;
            auto const& state{control_owner().profile};auto const actual{sender.observed_.lock()};
            if(!state||state->joined||state->signal_sender.get()!=&sender||!actual||!state->admitted||
                state->admitted.get()!=actual.get()||state->admitted.owner_before(actual)||actual.owner_before(state->admitted)||
                sender.generation_!=state->generation||!state->native||!state->signal)return ENOTCONN;
            // This real maintenance mutex is available during legitimate live
            // private execution; do NOT use the cold-only stop/reset exclusion.
            auto maintenance{g_runtime.execution_domain.try_maintenance_transition()};if(!maintenance)return EBUSY;
            generation=state->gate.try_enter();
            if(!generation||!current_source(*state->admitted)||state->native->stop_requested_.load(::std::memory_order_acquire))return ECANCELED;
            owner=state->signal;native=state->native;
        }
        // No registry/maintenance lock crosses a syscall. The real generation
        // lease and exact owner survive concurrent stop/retire; its short mutex
        // serializes the bounded atomic pipe write with true post-join cleanup.
        auto const actual=owner->push(event);
        // Terminal/zero-progress classification comes from this actual owned
        // write transaction, never a guessed global errno exemption or a
        // potentially busy later snapshot of another concurrent transaction.
        if(actual.terminal)native->request_stop({::uwvm2::runtime::wasm_threads::termination_kind::native_failure,1u,0u,0u,0u});
        return actual.error;
#else
        (void)sender;(void)event;return EOPNOTSUPP;
#endif
    }
    [[nodiscard]] static int observe_signal(threaded_signal_sender const& sender,threaded_signal_report&out) noexcept
    {
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
        ::std::shared_ptr<signal_owner> owner;
        {::std::unique_lock registry{control_owner().registry,::std::try_to_lock};if(!registry.owns_lock())return EBUSY;
         auto const& state{control_owner().profile};
         if(!state||state->signal_sender.get()!=&sender||sender.generation_!=state->generation||!state->signal)return ENOTCONN;
         owner=state->signal;}
        return owner->snapshot(out); // may observe this actual retired epoch
#else
        (void)sender;(void)out;return EOPNOTSUPP;
#endif
    }
    [[nodiscard]] static int signal_status(threaded_signal_report&out) noexcept
    {
        ::std::shared_ptr<signal_owner> owner;threaded_signal_report last{};
        {::std::unique_lock registry{control_owner().registry,::std::try_to_lock};if(!registry.owns_lock())return EBUSY;
         last=control_owner().last_signal_report;
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
         if(control_owner().profile)owner=control_owner().profile->signal;
#endif
        }
#if defined(UWVM_IMPORT_WASI_WASIP1) && !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1)
        if(owner)return owner->snapshot(out);
#endif
        out=last;return 0; // observation only; default DATA does not grant anything
    }
