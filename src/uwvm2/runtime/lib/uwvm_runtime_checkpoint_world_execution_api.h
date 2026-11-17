// Actual continuation entry for a published replacement. No saved native
// pointer or caller-created capture can enter this path.
#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
extern "C++" void checkpoint_join_published_workers_after_drain(
    ::std::shared_ptr<runtime_checkpoint_world_transaction> const& world) noexcept
{
    if(!world) { return; }
    for(auto const& thread:world->restored_threads_)
    {
        auto result=runtime_checkpoint_guest_worker_bridge::join_until(thread->worker,
            ::std::chrono::steady_clock::now()+::std::chrono::seconds{30});
        if(result.actual.status!=::fast_io::thread_join_status::joined) { ::fast_io::fast_terminate(); }
    }
    world->release_finished_root_carriers();
}
extern "C++" llvm_jit_checkpoint_continuation_status runtime_checkpoint_continuation_dispatcher::execute_restored_world(
    runtime_checkpoint_world_transaction& world, ::std::size_t index) noexcept
{
    using outcome=llvm_jit_checkpoint_continuation_status;
    namespace gc=::uwvm2::runtime::gc;
    using bridge=runtime_checkpoint_guest_worker_bridge;
    if(!world.published_ || index>=world.restored_threads_.size() || index>=world.prepared_threads_.size() ||
       world.publication_epoch_!=current_runtime_generation() ||
       !::uwvm2::uwvm::runtime::full::selected_full_source_matches_owner(world.prepared_source_) ||
       !same_owner(world.publication_host_,g_runtime.checkpoint_host_state) ||
       !same_owner(world.actual_prepared_profile_,g_runtime.checkpoint_profile) ||
       !same_owner(world.publication_control_,g_runtime.debug_pause_control)) { return outcome::stale_publication; }
    auto& thread=*world.restored_threads_[index];
    if(thread.frames.empty() || !bridge::current_restored_launch_state(thread.launch.get())) { return outcome::invalid_management_entry; }
    try
    {
        runtime_execution_entry_scope entry{runtime_execution_entry_scope::restored_try_entry{},::std::move(thread.startup)};
        if(!entry.restored_entry_admitted()) { return outcome::invalid_native_environment; }
        details::scoped_llvm_wasm_fp_environment fp{get_llvm_wasm_fp_environment_active_marker()};
        if(!fp.ready()) { return outcome::invalid_native_environment; }
        gc::scoped_cli_gc_execution managed{};
        runtime_gc_prepare_full_cohort();
        runtime_debug_execution_scope debugging{};entry.checkpoint_participant_admitted();
        if(!g_debug_pause_participant || !g_debug_activation_ledger || !g_checkpoint_shadow_ledger ||
           g_debug_activation_ledger->count()!=0u || g_checkpoint_shadow_ledger->size()!=0u ||
           !bridge::current_restored_worker(thread.worker,thread.launch.get(),g_debug_pause_participant->identifier()))
        { return outcome::invalid_native_environment; }
        // Input carriers are real relocated GC objects. Retain them before the
        // startup acknowledgment and throughout native packet materialization.
        runtime_checkpoint_world_transaction::prepared_world_root_scope roots{world.prepared_threads_[index],world.retained_roots_};
        if(!roots.install()) { return outcome::invalid_typed_packet; }
        using wait_batch=::uwvm2::utils::thread::keyed_wait_set::prepared_wait_batch;
        ::std::optional<wait_batch::completion_registration> wait_completion{};
        auto& pending_wait=world.prepared_threads_[index].pending_wait;
        if(pending_wait.batch)
        {
            auto const* execution=::uwvm2::runtime::wasm_threads::current_execution;
            if(!execution || !execution->pause)return outcome::invalid_native_environment;
            wait_completion.emplace(pending_wait.batch->bind_completion(pending_wait.index,
                {reinterpret_cast<void const*>(pending_wait.operation.owner),pending_wait.position},execution->pause,
                +[](void* context,::uwvm2::utils::thread::keyed_wait_result result) noexcept
                {static_cast<::uwvm2::runtime::wasm_threads::wait_pause_policy*>(context)->completed(result);}));
            if(!*wait_completion)return outcome::invalid_native_environment;
        }
        auto const baseline=gc::current_root_frames();
        {
            ::std::unique_lock lock{world.restored_mutex_};
            thread.participant=g_debug_pause_participant->identifier();thread.enrolled=true;
            world.restored_changed_.notify_all();
            world.restored_changed_.wait(lock,[&]{return world.restored_start_ || world.restored_cancel_;});
            if(world.restored_cancel_) { return outcome::execution_retired; }
        }
        llvm_jit_generated_bridge_scope_guard generated{get_llvm_jit_generated_bridge_scope_depth(),true};
        actual_resume_context context{thread.frames,world.publication_control_,world.actual_prepared_profile_,world.publication_epoch_};
        resume_context_scope active{context};
        struct wait_scope
        {
            checkpoint_restored_wait_state* previous{checkpoint_restored_wait};
            explicit wait_scope(checkpoint_restored_wait_state& value) noexcept{checkpoint_restored_wait=value.batch?::std::addressof(value):nullptr;}
            ~wait_scope(){checkpoint_restored_wait=previous;}
        } waiting{world.prepared_threads_[index].pending_wait};
        auto& root=thread.frames.front();
        if(!checked_live_values(root,root.logical)) { return outcome::invalid_typed_packet; }
        try
        {
            if(runtime_debug_shutdown_bridge::skip_cancelled_entry()) { return outcome::execution_retired; }
            runtime_debug_shutdown_bridge::full_entry_scope cancellation{};
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            runtime_managed_fault::boundary fault_boundary{};
#endif
            auto invoke=reinterpret_cast<native_entry>(root.address);
            invoke(root.landing,root.payload.data(),root.payload.size(),root.flags.data(),root.flags.size(),
                reinterpret_cast<::std::uintptr_t>(root.output.data()),root.output.size());
        }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        catch(runtime_managed_fault::signal const& signal)
        {
            // Resume-packet roots belong to this native entry, not generated
            // Wasm frames. Remove them after genuine generated unwind, before
            // the normal terminal boundary validates its empty root stack.
            roots.restore();
            if(!runtime_managed_fault::accept(signal)) { ::fast_io::fast_terminate(); }
            return outcome::execution_retired;
        }
#endif
        catch(runtime_debug_shutdown_bridge::signal const& signal)
        { roots.restore();if(!runtime_debug_shutdown_bridge::accept_at_full_entry(signal)) { ::fast_io::fast_terminate(); } return outcome::execution_retired; }
        catch(runtime_checkpoint_retirement_bridge::signal const& signal)
        { if(!runtime_checkpoint_retirement_bridge::accept_at_full_entry(signal)) { ::fast_io::fast_terminate(); } return outcome::execution_retired; }
        catch(bridge::guest_exit_signal const& signal)
        { if(!bridge::accept_guest_exit(signal,baseline)) { ::fast_io::fast_terminate(); } return outcome::continued; }
        catch(unsupported_child_result const&) { return outcome::invalid_typed_packet; }
        catch(::uwvm2::runtime::exception::guest_exception const& caught)
        {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
            roots.restore();
            if(runtime_managed_fault::accept_uncaught(caught)) { return outcome::execution_retired; }
#endif
            uncaught_guest_exception_fatal(::uwvm2::utils::container::u8string_view{
                root.module_name.data(),root.module_name.size()},root.logical.plan->get().function,caught);
        }
        if(!decode_actual_results(root) || !completed_native_entry(baseline)) { return outcome::invalid_typed_packet; }
        return outcome::continued;
    }
    catch(...) { return outcome::allocation_failed; }
}
#endif

extern "C++" llvm_jit_checkpoint_restored_join_result llvm_jit_checkpoint_join_restored_instance_host_api(
    ::std::uint_least64_t epoch, ::std::chrono::steady_clock::time_point deadline) noexcept
{
    using status=llvm_jit_checkpoint_restored_join_status;
    llvm_jit_checkpoint_restored_join_result out{};
    if(!::uwvm2::utils::control::debug_permissions::diagnostics_allowed())
    { out.status=status::invalid_context;return out; }
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
    if(get_runtime_execution_entry_depth()!=0u || get_runtime_state_publication_depth()!=0u ||
       get_llvm_jit_generated_bridge_scope_depth()!=0u || g_debug_observer_active)
    { out.status=status::invalid_context;return out; }
    // Hold the normal management exclusion while retaining the canonical world.
    // Physical join must never occur while holding a publication/GC lock.
    auto management=runtime_checkpoint_coherent_manager::try_legacy_transition_excluding_native_retirement();
    if(!management.owns_lock()) { out.status=status::pending;return out; }
    auto maintenance=g_runtime.execution_domain.try_maintenance_transition();
    if(!maintenance) { out.status=status::pending;return out; }
    auto world=g_runtime.checkpoint_world;
    if(!world || !world->published_ || epoch!=current_runtime_generation() || epoch!=world->publication_epoch_)
    { return out; }
    try
    {
        out.threads.reserve(world->restored_threads_.size());
        for(auto const& thread:world->restored_threads_)
        {
            auto joined=runtime_checkpoint_guest_worker_bridge::join_until(thread->worker,deadline);
            if(joined.actual.status!=::fast_io::thread_join_status::joined)
            { out.threads.clear();out.status=status::pending;return out; }
            ::std::lock_guard lock{world->restored_mutex_};
            if(!thread->finished) { out.threads.clear();out.status=status::pending;return out; }
            out.threads.push_back(thread->outcome);
        }
        world->release_finished_root_carriers();
        out.status=status::joined;
    }
    catch(...) { out.threads.clear();out.status=status::allocation_failed; }
#else
    static_cast<void>(epoch);static_cast<void>(deadline);
#endif
    return out;
}
