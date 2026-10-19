// Included inside ThreadedGroup after compiler state. G is mutex_; P is this
// group's independent domain. No function enters P while holding G.
        using debug_pause_domain = ::uwvm2::utils::thread::cooperative_pause_domain;
        enum class debug_control_phase : unsigned { idle, arming, armed, resuming, closed };
        struct debug_control_state final
        {
            debug_pause_domain pause;
            ::std::unique_ptr<debug_pause_domain::stopped_participant[]> scratch;
            ::std::size_t capacity{}, expected{}, polling{}, host_inflight{}, spawn_inflight{};
            ::std::uint64_t serial{};
            debug_control_phase phase{debug_control_phase::idle};
            debug_pause_domain::pause_ticket ticket;
            bool domain_resumed{true};
            explicit debug_control_state(::std::size_t count)
                : pause{count}, scratch{new debug_pause_domain::stopped_participant[count]}, capacity{count} {}
        };
        ::std::unique_ptr<debug_control_state> debug_control_;
        [[nodiscard]] bool debug_stable_roster_locked(::std::size_t&) const noexcept;
        [[nodiscard]] bool debug_control_worker_enter(slot&, debug_pause_domain::participant&, wait_pause_policy&) noexcept;
        void debug_control_worker_leave(slot&) noexcept;
        [[nodiscard]] bool debug_poll(slot&, debug_stop_site, ::std::size_t module=SIZE_MAX,
            ::std::size_t function=SIZE_MAX, ::std::uint64_t offset=UINT64_MAX,
            ::std::size_t instruction=SIZE_MAX) noexcept;
        [[nodiscard]] static bool debug_suspend_wait(::uwvm2::utils::thread::keyed_wait_set::suspension_borrow const&) noexcept;
        [[nodiscard]] bool debug_host_enter(slot&, bool spawn) noexcept;
        void debug_host_leave(slot&, bool spawn) noexcept;
        void debug_resume(::std::uint64_t) noexcept;
        [[nodiscard]] debug_stop_status debug_stop_until(::std::uint64_t,
            ::std::chrono::steady_clock::time_point, threaded_debug_all_stop&) noexcept;
        [[nodiscard]] debug_stop_status debug_read_stopped(threaded_debug_all_stop const&,
            ::std::span<debug_stopped_worker>, ::std::size_t&) noexcept;
