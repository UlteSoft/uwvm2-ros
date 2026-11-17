// Private world member fragment. Native workers are allocated before old-world
// retirement. Their real execution leases are obtained only after publication.
#pragma once
#include <uwvm2/utils/allocator/fast_io_strict/object.h>
#include <uwvm2/utils/container/unique.h>
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/container/shared.h>
using continuation=runtime_checkpoint_continuation_dispatcher;
using worker_bridge=runtime_checkpoint_guest_worker_bridge;
using execution_domain=::uwvm2::utils::thread::execution_domain;
struct restored_launch
{
    ::std::weak_ptr<runtime_checkpoint_world_transaction> world{};
    ::std::size_t index{};
};
struct restored_thread
{
    ::std::shared_ptr<restored_launch> launch{};
    worker_bridge::mutable_owner worker{};
    ::uwvm2::utils::container::std_vector<continuation::prepared_frame> frames{};
    execution_domain::lease startup{};
    llvm_jit_checkpoint_continuation_status outcome{llvm_jit_checkpoint_continuation_status::invalid_native_environment};
    ::std::uint_least64_t participant{};
    ::std::uint_least32_t exit_code{};
    bool enrolled{}, finished{};
};
::uwvm2::utils::container::std_vector<::uwvm2::utils::container::std_unique_ptr<restored_thread>> restored_threads_{};
::std::mutex restored_mutex_{};
::std::condition_variable restored_changed_{};
bool restored_start_{},restored_cancel_{},restored_workers_released_{};
::std::shared_ptr<runtime_checkpoint_world_transaction> retired_world_{};

// Management-only cleanup, AFTER the complete old cohort physically joined
// and publication replaced every callable binding. None of these swapped old
// allocations is needed by the new continuation packets.
void release_retired_world_after_join() noexcept
{
    if(!published_) { ::fast_io::fast_terminate(); }
    defined_dispatch_bindings_.clear();defined_dispatch_pointer_ranges_.clear();
    import_dispatch_bindings_.clear();
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1)
    wasip1_dispatch_contexts_.clear();
#endif
    publication_names_.clear();publication_ranges_.clear();publication_functions_.clear();publication_unwind_.clear();
    publication_exception_symbols_.reset();
    indirect_dispatch_modules_.clear();
    prepared_source_->checkpoint_declarations_.clear();prepared_source_->checkpoint_exports_.clear();
    retired_world_.reset();publication_previous_stores_.clear();
}
void release_finished_root_carriers() noexcept
{
    // Called only after physical joins of EVERY restored worker. A status bit
    // or a logical return alone is insufficient: root scopes are native TLS.
    for(auto& thread:restored_threads_) { thread->frames.clear(); }
    prepared_threads_.clear();
    if(prepared_waits_)prepared_waits_->abort_unconsumed();
    retained_roots_.clear();retained_root_owners_.clear();
}


static void restored_body(void* opaque) noexcept
{
    auto& launch=*static_cast<restored_launch*>(opaque);
    auto world=launch.world.lock();
    if(!world || launch.index>=world->restored_threads_.size() ||
       world->restored_threads_[launch.index]->launch.get()!=::std::addressof(launch) ||
       !worker_bridge::current_restored_launch_state(opaque)) { ::fast_io::fast_terminate(); }
    auto result=continuation::execute_restored_world(*world,launch.index);
    bool complete{};::std::uint_least32_t code{};
    {
        ::std::lock_guard lock{world->restored_mutex_};
        auto& thread=*world->restored_threads_[launch.index];thread.outcome=result;
        thread.exit_code=worker_bridge::completion_code();thread.finished=true;
        complete=true;
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        bool const fault=runtime_managed_fault::faulted(world->publication_epoch_);
#else
        constexpr bool fault=false;
#endif
        for(auto const& current:world->restored_threads_)
        {
            if(!current->finished || (!fault && current->outcome==llvm_jit_checkpoint_continuation_status::execution_retired))
            {complete=false;break;}
            if(!code)code=current->exit_code;
            if(current->outcome!=llvm_jit_checkpoint_continuation_status::continued)code=121u;
        }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        if(fault)code=::uwvm2::runtime::diagnostic::crash::exit_status();
#endif
    }
    world->restored_changed_.notify_all();
    auto const& observer=world->publication_observer_;
    if(complete && observer.context && observer.on_restored_completion)
        observer.on_restored_completion(observer.context.get(),world->publication_control_.get(),world->publication_epoch_,code);
}

[[nodiscard]] bool prepare_restored_workers(::std::shared_ptr<runtime_checkpoint_world_transaction> const& self)
{
    if(self.get()!=this || !publication_ready() || !restored_threads_.empty()) { return false; }
    if(!charge_native(prepared_threads_.size(),sizeof(restored_thread)+sizeof(restored_launch)+
        sizeof(llvm_jit_debug_guest_worker)+sizeof(::fast_io::native_thread)+sizeof(::uwvm2::utils::container::std_unique_ptr<restored_thread>))) { return false; }
    restored_threads_.reserve(prepared_threads_.size());
    for(::std::size_t index{};index<prepared_threads_.size();++index)
    {
        auto thread=::uwvm2::utils::container::make_unique<restored_thread>();
        thread->launch=::uwvm2::utils::container::make_shared<restored_launch>();thread->launch->world=self;thread->launch->index=index;
        auto const& input=prepared_threads_[index].frames;
        if(input.empty() || !charge_native(input.size(),sizeof(continuation::prepared_frame))) { return false; }
        thread->frames.resize(input.size());
        for(::std::size_t n{};n<input.size();++n)
        {
            auto const& from=input[n];auto& to=thread->frames[n];
            if(from.owner>=modules_.size() || !from.plan || from.site==0u || from.site>from.plan->get().sites.size() ||
               !charge_native(from.payload.size(),1u) || !charge_native(from.output.size(),1u) || !charge_native(from.flags.size(),1u) ||
               !charge_native(from.values.size(),sizeof(frame_checkpoint)) ||
               !charge_native(from.result_types.size(),sizeof(frame_checkpoint)+sizeof(reference)+2u*sizeof(::std::size_t))) { return false; }
            to.logical.plan=from.plan;to.logical.site=from.site;to.logical.materialized=true;to.logical.values=from.values;
            to.child_projection=from.child_projection;to.address=from.entry;to.landing=from.site;
            to.ros_cross_module_ancestry=from.ros_cross_module_ancestry;
            to.payload=from.payload;to.output=from.output;to.flags=from.flags;
            to.result_offsets=from.result_offsets;to.result_widths=from.result_widths;
            to.declared_store=modules_[from.owner].actual->gc_store;
            to.module_name=modules_[from.owner].file->module_name;
            to.result_roots.resize(from.result_types.size());to.returned.resize(from.result_types.size());
            for(::std::size_t k{};k<from.result_types.size();++k) { to.returned[k].declaration={from.result_types[k],true}; }
        }
        using worker=llvm_jit_debug_guest_worker;
        thread->worker=worker_bridge::mutable_owner{::uwvm2::utils::container::adopt_shared(::uwvm2::utils::allocator::fast_io_strict::object<worker>([&](worker* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) worker{publication_control_,prepared_source_,actual_prepared_profile_,
            publication_host_,publication_epoch_,thread->launch,restored_body,worker::origin::restored_world}; }))};
        auto actual=thread->worker;
        auto const slot=worker_bridge::register_actual_worker(actual);
        // Keep cancellation ownership before OS creation. If allocation throws,
        // every previously created worker still waits behind its private gate.
        restored_threads_.push_back(::std::move(thread));
        try { actual->native_.reset(::uwvm2::utils::allocator::fast_io_strict::object<::fast_io::native_thread>([&](::fast_io::native_thread* uwvm_allocated) { return ::new(static_cast<void*>(uwvm_allocated)) ::fast_io::native_thread{[actual]() noexcept { worker_bridge::actual_worker_body(actual); }}; })); }
        catch(...) { worker_bridge::unregister_unstarted_worker(actual,slot);restored_threads_.back()->worker.reset();throw; }
    }
    return restored_threads_.size()==prepared_threads_.size();
}
void cancel_unpublished_workers() noexcept
{
    if(restored_workers_released_) { return; }
    for(auto const& thread:restored_threads_)
    {
        if(!thread->worker || !thread->worker->native_) { continue; }
        worker_bridge::release_publication(thread->worker,true);
    }
    for(auto const& thread:restored_threads_)
    {
        if(!thread->worker || !thread->worker->native_) { continue; }
        // These bodies only return; no guest/provider callback can block them.
        auto joined=worker_bridge::join_actual(thread->worker,::std::chrono::steady_clock::now()+::std::chrono::seconds{30});
        if(joined.actual.status!=::fast_io::thread_join_status::joined) { ::fast_io::fast_terminate(); }
    }
}
[[nodiscard]] bool enroll_restored_workers(execution_domain::prepared_generation const& generation,
    ::std::chrono::steady_clock::time_point deadline) noexcept
{
    if(!published_ || restored_workers_released_ || restored_threads_.size()!=prepared_threads_.size()) { return false; }
    // Obtain ALL startup leases before releasing even one native worker.
    for(auto& thread:restored_threads_)
    {
        thread->startup=g_runtime.execution_domain.try_enter_owned_published_startup_generation(generation);
        if(!thread->startup)
        {
            for(auto& prepared:restored_threads_) { prepared->startup.reset(); }
            cancel_unpublished_workers();
            return false;
        }
    }
    restored_workers_released_=true;
    for(auto& thread:restored_threads_) { worker_bridge::release_publication(thread->worker); }
    ::std::unique_lock lock{restored_mutex_};
    auto complete=[&]() noexcept
    { for(auto const& thread:restored_threads_) { if(!thread->enrolled && !thread->finished) { return false; } } return true; };
    if(!restored_changed_.wait_until(lock,deadline,complete)) { return false; }
    for(auto const& thread:restored_threads_)
    { if(!thread->enrolled || thread->finished || thread->participant==0u) { return false; } }
    if(publication_replay_)
    {
        ::std::lock_guard journal_lock{publication_replay_->mutex};
        if(publication_replay_->participants.size()!=restored_threads_.size())return false;
        for(::std::size_t index{};index<restored_threads_.size();++index)
            publication_replay_->participants[index]=restored_threads_[index]->participant;
    }
    return true;
}
[[nodiscard]] bool release_enrolled_workers() noexcept
{
    ::std::lock_guard lock{restored_mutex_};
    if(!published_ || !restored_workers_released_ || restored_start_ || restored_cancel_) { return false; }
    for(auto const& thread:restored_threads_)
    { if(!thread->enrolled || thread->finished || thread->participant==0u) { return false; } }
    if(prepared_waits_)prepared_waits_->start_deadlines();
    restored_start_=true;restored_changed_.notify_all();return true;
}
