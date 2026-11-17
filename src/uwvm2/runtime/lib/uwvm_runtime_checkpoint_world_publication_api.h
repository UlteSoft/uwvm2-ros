// Sole issuer of replacement-world publication. Saved data never calls this
// directly: the canonical native operation supplies actual drain and OS join.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
extern "C++" void runtime_checkpoint_coherent_manager::finish_published_failure_until(
    native_retirement_state& actual, ::uwvm2::utils::thread::execution_domain::maintenance_transition const& maintenance) noexcept
{
    using outcome=llvm_jit_checkpoint_native_retirement_status;
    auto world=actual.candidate;
    if(!actual.publication_committed || !world || !world->published_ || world!=g_runtime.checkpoint_world ||
       world->publication_epoch_!=current_runtime_generation()) { actual.result.status=outcome::failed_closed;return; }
    // Ordinary admission has never opened. Wake every enrolled worker only
    // into cancellation; incomplete startup leases cannot keep reset blocked.
    { ::std::lock_guard lock{world->restored_mutex_};world->restored_cancel_=true; }
    world->restored_changed_.notify_all();
    if(!g_runtime.execution_domain.request_stop_without_callbacks_owned_transition(maintenance))
    { actual.result.status=outcome::failed_closed;return; }
    if(!world->restored_workers_released_)
    {
        for(auto& thread:world->restored_threads_) { thread->startup.reset(); }
        world->cancel_unpublished_workers();
    }
    for(auto const& thread:world->restored_threads_)
    {
        auto joined=runtime_checkpoint_guest_worker_bridge::join_until(thread->worker,actual.deadline);
        if(joined.actual.status!=::fast_io::thread_join_status::joined)
        { actual.result.status=outcome::pending_native_join;return; }
    }
    if(!g_runtime.execution_domain.drain_owned_transition_until(maintenance,actual.deadline))
    { actual.result.status=outcome::pending_execution;return; }
    actual.result.status=outcome::restored_startup_failed;
    actual.result.runtime_epoch=world->publication_epoch_;
    actual.result.candidate_world_retained=false;
    // Keep the selected world pinned in g_runtime for diagnosis and explicit
    // host reset. No old frame is resumed, no partial new cohort is admitted.
}
extern "C++" llvm_jit_checkpoint_native_retirement_result runtime_checkpoint_coherent_manager::publish_prepared_instance(
    llvm_jit_checkpoint_native_retirement_owner const& supplied, ::std::chrono::steady_clock::time_point deadline) noexcept
{
    using execution=::uwvm2::utils::thread::execution_domain;
    using outcome=llvm_jit_checkpoint_native_retirement_status;
    llvm_jit_checkpoint_native_retirement_result out{};
    if(!native_retirement_context_allowed()) { out.status=outcome::invalid_context;return out; }
    if(deadline<=::std::chrono::steady_clock::now()) { out.status=outcome::invalid_deadline;return out; }
    ::std::unique_lock lock{native_retirement_mutex_,::std::try_to_lock};
    if(!lock.owns_lock()) { out.status=outcome::busy;return out; }
    if(!native_retirement_ || !same_owner(native_retirement_->token,supplied) || !native_retirement_->prepare_candidate)
    { out.status=outcome::stale_owner;return out; }
    auto actual=native_retirement_;actual->deadline=deadline;
    try
    {
        auto maintenance=g_runtime.execution_domain.try_maintenance_transition();
        if(!maintenance) { out=actual->result;out.status=outcome::busy;return out; }
        finish_native_retirement_until(*actual,maintenance);
        if(actual->result.status!=outcome::prepared_world_ready_closed)
        { out=actual->result;release_completed_native_retirement(lock,actual);return out; }
        auto world=actual->candidate;
        if(!world || !world->publication_ready() || !cancel_actual_candidate_workers(world))
        { actual->result.status=outcome::failed_closed;return actual->result; }
        actual->result.private_closed_workers_cancelled_and_joined=true;
        auto drain=g_runtime.execution_domain.drain_prepared_generation_until(maintenance,actual->generation,deadline);
        if(drain.status!=execution::prepared_drain_status::drained || !drain.actual)
        { actual->result.status=outcome::failed_closed;return actual->result; }
        // Old code, source and worker objects remain strongly retained until
        // publication and new-worker enrollment finish. No destruction in commit.
        ::std::optional<::uwvm2::runtime::gc::managed_entry_admission::exclusive_lease> exclusive{};
        bool published{};
        auto status=g_runtime.execution_domain.publish_prepared_drained_reset(maintenance,actual->generation,drain.actual,
            [&](auto const&) noexcept
            {
                exclusive.emplace(::uwvm2::runtime::gc::runtime_gc_entry_admission.try_exclusive(0u));
                if(!*exclusive) { return false; }
                runtime_state_publication_guard publication{};
                if(!world->publication_ready()) { return false; }
                g_runtime.gc_collection.reset_after_execution_drain();
                if(!world->publish_world_after_actual_drain()) { ::fast_io::fast_terminate(); }
                world->retired_world_=::std::move(g_runtime.checkpoint_world);
                g_runtime.checkpoint_world=world;actual->publication_committed=true;published=true;return true;
            });
        if(status!=execution::prepared_reset_result::published_closed || !published)
        { actual->result.status=outcome::failed_closed;return actual->result; }
        // Release the old admission gate only AFTER real retirement and the
        // atomic source/code/state publication; it never admits old execution.
        auto released=retirement::release_after_actual_execution_drain(actual->request);
        if(released==retirement::drain_result::invalid_request) { ::fast_io::fast_terminate(); }
        actual->result.all_frames_signalled=released==retirement::drain_result::all_frames_signalled;
        bool collection_ready=true;
        if(world->collection_requested_)
        {
            collection_ready=g_runtime.gc_collection.request_cli(
                ::uwvm2::runtime::gc::published_initializer_serial.load(::std::memory_order_acquire)) &&
                !g_runtime.gc_collection.disabled();
        }
        world->release_retired_world_after_join();
        g_runtime.published_runtime_state=make_runtime_state_signature();
        exclusive.reset(); // New worker GC entry requires actual exclusion release.
        if(!collection_ready)
        {
            finish_published_failure_until(*actual,maintenance);
            out=actual->result;release_completed_native_retirement(lock,actual);return out;
        }
        // Participants must enroll before requesting a pause: a requested
        // pause deliberately closes normal participant admission. They are
        // still behind restored_start_, so no guest instruction can run yet.
        if(!world->enroll_restored_workers(actual->generation,deadline))
        {
            finish_published_failure_until(*actual,maintenance);
            out=actual->result;release_completed_native_retirement(lock,actual);return out;
        }
        if(world->pause_after_restore_)
        {
            world->publication_pause_=world->publication_control_->request_pause();
            if(!world->publication_pause_ || !world->publication_observer_.on_restored_pause(
                world->publication_observer_.context.get(),world->publication_control_.get(),
                world->publication_pause_,world->publication_epoch_))
            {
                finish_published_failure_until(*actual,maintenance);
                out=actual->result;release_completed_native_retirement(lock,actual);return out;
            }
        }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        runtime_managed_fault::enable(world->publication_epoch_);
#endif
        if(!g_runtime.execution_domain.open_published_startup_generation(maintenance,actual->generation,
                [&]() noexcept {return world->release_enrolled_workers();}))
        {
            finish_published_failure_until(*actual,maintenance);
            out=actual->result;release_completed_native_retirement(lock,actual);return out;
        }
        actual->result.status=outcome::restored_execution_started;
        actual->result.runtime_epoch=world->publication_epoch_;
        actual->result.candidate_world_retained=false;
        out=actual->result;
    }
    catch(...)
    {
        actual->result.status=outcome::failed_closed;
        if(actual->publication_committed)
        {
            auto maintenance=g_runtime.execution_domain.try_maintenance_transition();
            if(maintenance) { finish_published_failure_until(*actual,maintenance); }
        }
        out=actual->result;
    }
    release_completed_native_retirement(lock,actual);return out;
}
#endif
extern "C++" llvm_jit_checkpoint_native_retirement_result llvm_jit_checkpoint_publish_prepared_instance_host_api(
    llvm_jit_checkpoint_native_retirement_owner const& supplied, ::std::chrono::steady_clock::time_point deadline) noexcept
{
    if(!::uwvm2::utils::control::debug_permissions::full_allowed())
    { llvm_jit_checkpoint_native_retirement_result out{};out.status=llvm_jit_checkpoint_native_retirement_status::invalid_context;return out; }
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    return runtime_checkpoint_coherent_manager::publish_prepared_instance(supplied,deadline);
#else
    static_cast<void>(supplied);static_cast<void>(deadline);return {};
#endif
}
