// Private runtime journal lifetime; no guest entry points.
#pragma once
extern "C++"
{
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    // Only the authentic selected ROS-v1 import bridge installs a producer on
    // this native thread. The host API keeps actual provenance and range checks.
    // A scope serializes one guest-memory operation, never an arbitrary provider
    // callback or its native lock/wait. No pointer is accepted from a snapshot.
    class checkpoint_replay_host_memory_scope final
    {
        void* actual_{};
    public:
        checkpoint_replay_host_memory_scope(unsigned,::std::size_t,::std::uint64_t,::std::size_t) noexcept;
        ~checkpoint_replay_host_memory_scope();
        checkpoint_replay_host_memory_scope(checkpoint_replay_host_memory_scope const&)=delete;
        checkpoint_replay_host_memory_scope& operator=(checkpoint_replay_host_memory_scope const&)=delete;
        [[nodiscard]] bool finish(bool) noexcept;
    };
#endif
    struct runtime_checkpoint_replay_session;
    inline ::std::atomic<bool> checkpoint_replay_selected{};
    inline ::std::mutex checkpoint_replay_mutex{};
    inline ::std::shared_ptr<runtime_checkpoint_replay_session> checkpoint_replay_session{};
    void checkpoint_replay_probe_opcode(::uwvm2::utils::thread::cooperative_pause_location,::std::uint64_t) noexcept;
    void checkpoint_replay_enter_opcode(::uwvm2::utils::thread::cooperative_pause_location,::std::uint64_t) noexcept;
    void checkpoint_replay_reset() noexcept;
    void checkpoint_replay_leave_worker(::std::uint64_t) noexcept;
    void checkpoint_replay_wait_registered() noexcept;
    void checkpoint_replay_wait_completed(::std::uint64_t,::uwvm2::utils::thread::keyed_wait_result) noexcept;
    // Stack-only evidence of the actual debug-instrumented wait operation.
    // It is never populated from a debugger request or serialized pointer.
    struct checkpoint_wait_operation
    {
        ::std::uintptr_t owner{};
        unsigned operation{}, bits{};
        ::std::uint64_t offset{}, address{}, expected{};
        ::std::int64_t timeout{};
    };
    struct checkpoint_restored_wait_state
    {
        checkpoint_wait_operation operation{};
        ::uwvm2::utils::thread::keyed_wait_set::prepared_wait_batch* batch{};
        ::std::size_t index{};
        ::std::uint64_t position{};
        ::uwvm2::utils::thread::cooperative_pause_location location{};
        bool consumed{};
    };
    inline thread_local checkpoint_restored_wait_state* checkpoint_restored_wait{};
    // Only an authenticated replay wait can install this stack-borrowed queue
    // state. Complete census refreshes intermediate stops after the full cohort
    // parks; endpoints use their independently matched final queue record.
    inline thread_local ::uwvm2::utils::thread::keyed_wait_set::suspension_snapshot const* checkpoint_virtual_wait{};
    inline thread_local checkpoint_wait_operation const* checkpoint_actual_wait{};
    inline thread_local ::uwvm2::utils::thread::keyed_wait_set::suspension_borrow const* checkpoint_actual_wait_borrow{};

}
