// Private world member fragment. All storage and code ownership is prepared
// while the old cohort is still stopped. Only the coherent manager, after real
// execution drain and OS-thread join, may publish this replacement.
#pragma once
#include <uwvm2/utils/container/standard.h>
::std::uint_least64_t publication_epoch_{};
::std::shared_ptr<::uwvm2::utils::thread::cooperative_pause_domain> publication_control_{};
runtime_checkpoint_host_bridge::owner publication_host_{};
decltype(g_runtime.module_name_to_id) publication_names_{};
decltype(g_runtime.llvm_jit_code_ranges) publication_ranges_{};
decltype(g_runtime.llvm_jit_native_function_ranges) publication_functions_{};
decltype(g_runtime.llvm_jit_unwind_entries) publication_unwind_{};
::uwvm2::runtime::exception::diagnostic_symbols_ref publication_exception_symbols_{};
::uwvm2::utils::container::std_vector<store_owner> publication_previous_stores_{};
bool publication_prepared_{}, published_{}, collection_requested_{};
bool pause_after_restore_{};
::std::shared_ptr<runtime_checkpoint_replay_session> publication_replay_{};
llvm_jit_debug_observer publication_observer_{};
::uwvm2::utils::thread::cooperative_pause_domain::pause_ticket publication_pause_{};

// The coherent manager calls preparation inside the pause domain's locked
// cohort callback. That callback already proves its live, exact stop; querying
// is_closed() here would recursively lock the same nonrecursive mutex. Keep
// candidate checks independent of pause-domain lock acquisition. Publication
// separately requires actual native retirement and generation ownership.
[[nodiscard]] bool prepare_world_publication()
{
    if(publication_prepared_ || !preflight_private_handed_off_full_code() ||
       !g_runtime.debug_pause_control ||
       source_initializer_observed_epoch_ == UINT64_MAX) { return false; }
    if(!charge_native(g_runtime.modules.size(),sizeof(store_owner))) { return false; }
    publication_previous_stores_.reserve(g_runtime.modules.size());
    for(auto const& record:g_runtime.modules)
    {
        if(!record.runtime_module || !record.runtime_module->gc_store) { return false; }
        publication_previous_stores_.push_back(record.runtime_module->gc_store);
    }
    if(!staged_gc_.previous_cohort_matches(publication_previous_stores_)) { return false; }
    publication_epoch_=source_initializer_observed_epoch_+1u;
    publication_control_=g_runtime.debug_pause_control;
    publication_observer_=g_runtime.debug_observer;
    if(pause_after_restore_ && (!publication_observer_.context || !publication_observer_.on_restored_pause))return false;
    using bridge=runtime_checkpoint_host_bridge;
    publication_host_=bridge::prepare_world_state(actual_prepared_profile_,publication_control_,publication_epoch_);
    collection_requested_=g_runtime.gc_collection.roots_requested();
    if(!charge_native(modules_.size(),sizeof(decltype(publication_names_)::value_type))) { return false; }
    publication_names_.reserve(modules_.size());
    for(auto const& binding:modules_)
    { if(!publication_names_.emplace(binding.file->module_name,binding.new_actual_id).second) { return false; } }
    // Copy diagnostic names against the NEW dense module roster. Keeping the
    // old roster could attribute an exception to another preload after restore.
    namespace exn=::uwvm2::runtime::exception;
    if(!charge_native(modules_.size(),sizeof(exn::diagnostic_module_symbols))) { return false; }
    ::uwvm2::utils::container::std_vector<exn::diagnostic_module_symbols> symbols{};symbols.reserve(modules_.size());
    for(auto owner:actual_dense_module_owners_)
    {
        if(owner>=modules_.size()) { return false; }
        auto const& binding=modules_[owner];
        auto const imported=binding.actual->imported_function_vec_storage.size();
        auto const local=binding.actual->local_defined_function_vec_storage.size();
        if(local>SIZE_MAX-imported || !charge_native(binding.file->module_name.size(),sizeof(char8_t))) { return false; }
        exn::diagnostic_module_symbols entry{.function_count=imported+local};
        entry.name.assign(binding.file->module_name.data(),binding.file->module_name.size());
        auto const& names=binding.file->wasm_custom_name.function_name;
        if(!charge_native(names.size(),sizeof(exn::diagnostic_function_symbol))) { return false; }
        entry.functions.reserve(names.size());
        for(auto const& [index,name]:names)
        {
            if(index>=entry.function_count || name.empty()) { continue; }
            if(!charge_native(name.size(),sizeof(char8_t))) { return false; }
            exn::diagnostic_function_symbol function{.function_index=index};
            function.name.assign(name.data(),name.size());entry.functions.push_back(::std::move(function));
        }
        ::std::sort(entry.functions.begin(),entry.functions.end(),[](auto const& a,auto const& b)
            {return a.function_index<b.function_index;});
        symbols.push_back(::std::move(entry));
    }
    publication_exception_symbols_=exn::diagnostic_symbols::make(::std::move(symbols));
    if(!publication_exception_symbols_) { return false; }
    ::std::size_t range_count{},function_count{},unwind_count{};
    for(auto const& engine:engines_)
    {
        if(!engine || !engine->ranges_ || engine->ranges_->pending.size()>SIZE_MAX-range_count ||
           engine->ranges_->pending_functions.size()>SIZE_MAX-function_count ||
           engine->entries_.size()>(SIZE_MAX-unwind_count)/4u) { return false; }
        range_count+=engine->ranges_->pending.size();function_count+=engine->ranges_->pending_functions.size();
        unwind_count+=engine->entries_.size()*4u;
    }
    if(!charge_native(range_count,sizeof(llvm_jit_code_range)) || !charge_native(function_count,sizeof(llvm_jit_code_range)) ||
       !charge_native(unwind_count,sizeof(llvm_jit_unwind_entry))) { return false; }
    publication_ranges_.reserve(range_count);publication_functions_.reserve(function_count);publication_unwind_.reserve(unwind_count);
    auto add_range=[](auto& target,::std::uintptr_t begin,::std::uintptr_t size)->bool
    {
        if(begin==0u || size==0u || size>UINTPTR_MAX-begin) { return false; }
        target.push_back(llvm_jit_code_range{begin,begin+size});return true;
    };
    for(::std::size_t dense{};dense<engines_.size();++dense)
    {
        auto const& engine=*engines_[dense];auto const& ranges=*engine.ranges_;
        if(ranges.observation_failed_ || ranges.committed_) { return false; }
        for(auto const& r:ranges.pending) { if(!add_range(publication_ranges_,r.begin,r.size)) { return false; } }
        for(auto const& r:ranges.pending_functions) { if(!add_range(publication_functions_,r.begin,r.size)) { return false; } }
        auto const& record=indirect_dispatch_modules_.index_unchecked(dense);
        auto const imported=record.runtime_module->imported_function_vec_storage.size();
        for(::std::size_t local{};local<engine.entries_.size();++local)
        {
            auto const& entry=engine.entries_[local];
            auto add=[&](::std::uintptr_t pointer,bool raw,bool wrapper)
            {
                auto const address=details::native_function_code_address(pointer);
                publication_unwind_.push_back(llvm_jit_unwind_entry{address,0u,dense,imported+local,raw,wrapper});
            };
            // The resumed body is a distinct physical Wasm frame. Register
            // its compiler-owned extent as well as the ordinary body; raw ABI
            // adapters are wrappers and must not duplicate the logical frame.
            add(entry.typed,false,false);add(entry.raw,true,true);
            add(entry.resume_typed,false,false);add(entry.resume_raw,true,true);
        }
    }
    auto normalize=[](auto& ranges,bool merge)
    {
        ::std::sort(ranges.begin(),ranges.end(),[](auto const& a,auto const& b){ return a.begin<b.begin; });
        ::std::size_t count{};
        for(auto const item:ranges)
        {
            if(count!=0u && (merge ? item.begin<=ranges.index_unchecked(count-1u).end : item.begin==ranges.index_unchecked(count-1u).begin))
            {
                auto& previous=ranges.index_unchecked(count-1u);
                previous.end=merge?(::std::max)(previous.end,item.end):(::std::min)(previous.end,item.end);
            }
            else { ranges.index_unchecked(count++)=item; }
        }
        ranges.resize(count);
    };
    normalize(publication_ranges_,true);normalize(publication_functions_,false);
    ::std::sort(publication_unwind_.begin(),publication_unwind_.end(),[](auto const& a,auto const& b){ return a.address<b.address; });
    for(auto& entry:publication_unwind_)
    {
        if(entry.address==0u) { return false; }
        auto const it=::std::lower_bound(publication_functions_.begin(),publication_functions_.end(),entry.address,
            [](auto const& r,auto address){ return r.begin<address; });
        if(it==publication_functions_.end() || it->begin!=entry.address) { return false; }
        entry.end=it->end;
    }
    publication_prepared_=true;return true;
}

// Revalidate compiler-observed executable extents, expected plans, and actual
// callable slots before adopting a staged native observer. No saved address or
// next-symbol estimate can grant an assembly debugger range.
[[nodiscard]] bool preflight_publication_native_owners() const noexcept
{
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
    for(::std::size_t dense{};dense<engines_.size();++dense)
    {
        auto const& record=indirect_dispatch_modules_.index_unchecked(dense);
        auto const* observer=record.llvm_jit_full_publication->actual_native_endpoints.get();
        auto const count=record.runtime_module->local_defined_function_vec_storage.size();
        auto const imported=record.runtime_module->imported_function_vec_storage.size();
        if(count==0u) { if(observer!=nullptr) { return false; } continue; }
        if(!private_native_endpoint_owner_matches(*engines_[dense],record,record.llvm_jit_full_publication.get()) ||
           observer->engine_->hasError() || observer->objects_.empty() || observer->expected_.size()!=observer->seen_.size()) { return false; }
        for(auto seen:observer->seen_) { if(seen!=1u) { return false; } }
        for(auto const& expected:observer->expected_)
        {
            if(expected.module!=dense || expected.function<imported || expected.function-imported>=count) { return false; }
            auto const local=expected.function-imported;
            auto const& plan=record.llvm_jit_compiled.local_funcs.index_unchecked(local).checkpoint_plan;
            if(expected.generation!=record.llvm_jit_debug_full_entry_generations[local] || expected.plan.get()!=plan.get() ||
               expected.plan.owner_before(plan) || plan.owner_before(expected.plan)) { return false; }
        }
        ::std::uintptr_t last{};
        for(auto const& body:observer->bodies_)
        {
            if(body.code_begin==0u || body.code_end<=body.code_begin || body.code_begin<last) { return false; }
            last=body.code_end;
            if(body.role!=1u && body.role!=2u) { continue; }
            if(body.expected_index>=observer->expected_.size()) { return false; }
            auto const& expected=observer->expected_[body.expected_index];
            if(expected.function<imported || expected.function-imported>=count) { return false; }
            auto const local=expected.function-imported;
            if(body.role==1u && record.llvm_jit_local_entry_addresses.index_unchecked(local)!=body.callable_entry) { return false; }
        }
    }
#endif
    return true;
}

[[nodiscard]] bool publication_ready() const noexcept
{
    return publication_prepared_ && !published_ && publication_epoch_!=0u &&
        publication_epoch_-1u==current_runtime_generation() && publication_host_ && publication_control_ &&
        publication_control_.get()==g_runtime.debug_pause_control.get() &&
        preflight_private_handed_off_full_code() && preflight_publication_native_owners() &&
        staged_gc_.previous_cohort_matches(publication_previous_stores_) && current_ros_handoff();
}

// Called ONLY with the coherent manager's real drained generation, physical
// worker join, exclusive GC entry and runtime publication lock. All refusal
// checks precede mutation; everything after them is a nonallocating owner move.
[[nodiscard]] bool publish_world_after_actual_drain() noexcept
{
    namespace gc=::uwvm2::runtime::gc;
    namespace storage=::uwvm2::uwvm::wasm::storage;
    namespace full=::uwvm2::uwvm::runtime::full;
    if(get_runtime_state_publication_depth()!=1u || !publication_ready()) { return false; }
    if(!publish_current_ros_handoff()) { return false; }
    staged_gc_.retire_previous_cohort_after_drain(publication_previous_stores_);
    staged_gc_.publish_preflighted();
    ::std::swap(storage::all_module,prepared_source_->checkpoint_declarations_);
    ::std::swap(storage::all_module_export,prepared_source_->checkpoint_exports_);
    full::details::selected_full_source=prepared_source_;
    storage::details::selected_execute_wasm=::std::addressof(prepared_source_->file_);
    storage::details::selected_preloaded_wasm=::std::addressof(prepared_source_->preload_files_);
    storage::details::selected_execute_wasm_initialized=false;
    ::uwvm2::uwvm::runtime::storage::details::selected_runtime_registry=::std::addressof(prepared_source_->registry_);
    auto const serial=gc::begin_owned_initializer();
    for(auto& binding:modules_)
    { binding.actual->gc_collection_phase.publish_initialized(serial);binding.actual->gc_collection_phase.publish_active_segments(true); }
    gc::publish_owned_initializer(serial);
    if(!prepared_source_->seal_actual_initializer()) { ::fast_io::fast_terminate(); }
    ::std::swap(g_runtime.modules,indirect_dispatch_modules_);
    ::std::swap(g_runtime.module_name_to_id,publication_names_);
    ::std::swap(g_runtime.defined_func_cache,defined_dispatch_bindings_);
    ::std::swap(g_runtime.defined_func_ptr_ranges,defined_dispatch_pointer_ranges_);
    ::std::swap(g_import_call_cache,import_dispatch_bindings_);
    ::std::swap(g_runtime.llvm_jit_code_ranges,publication_ranges_);
    ::std::swap(g_runtime.llvm_jit_native_function_ranges,publication_functions_);
    ::std::swap(g_runtime.llvm_jit_unwind_entries,publication_unwind_);
    ::std::swap(g_runtime.exception_diagnostic_symbols,publication_exception_symbols_);
#if !defined(UWVM_DISABLE_LOCAL_IMPORTED_WASIP1) && defined(UWVM_IMPORT_WASI_WASIP1)
    ::std::swap(g_wasip1_runtime_module_context_cache,wasip1_dispatch_contexts_);
#endif
    advance_runtime_generation();
    if(current_runtime_generation()!=publication_epoch_) { ::fast_io::fast_terminate(); }
    for(::std::size_t dense{};dense<g_runtime.modules.size();++dense)
    {
        auto& record=g_runtime.modules.index_unchecked(dense);auto& publication=*record.llvm_jit_full_publication;
        if(!prepared_source_->bind_actual_compiled_module(dense,record.runtime_module) ||
           !prepared_source_->record_actual_full_validation(dense,publication_epoch_,record.runtime_module)) { ::fast_io::fast_terminate(); }
        publication.runtime_epoch=publication_epoch_;publication.debug_source_runtime_epoch=publication_epoch_;
        record.llvm_jit_debug_source_fused_epoch=publication_epoch_;record.llvm_jit_precise_gc_roots_emitted=true;
        engines_[dense]->ranges_->commit([](::std::uintptr_t,::std::uintptr_t) noexcept {});
        publication.native_provenance=engines_[dense]->ranges_->take_debug_full_capture(publication_epoch_);
#if defined(UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2) && UWVM_EXPERIMENTAL_NATIVE_OWNER_TABLE_V2 == 1
        if(auto* observer=publication.actual_native_endpoints.get())
        {
            // The complete fixed observer graph was checked before mutation.
            // Recheck its new selected source/epoch before sealing it as LIVE.
            if(observer->source_->actual_validated_file(dense,publication_epoch_,record.runtime_module)==nullptr ||
               observer->engine_!=publication.engine.get() || observer->code_publication_!=::std::addressof(publication))
            { ::fast_io::fast_terminate(); }
            observer->runtime_epoch_=publication_epoch_;observer->staged_only_=false;observer->sealed_=true;
        }
#endif
        if(!checkpoint_root_generation_publication_bridge::seal_actual_full(record)) { ::fast_io::fast_terminate(); }
        record.llvm_jit_ready=true;
    }
    g_runtime.checkpoint_profile=actual_prepared_profile_;
    g_runtime.checkpoint_host_state=publication_host_;
    g_runtime.checkpoint_host_enabled.store(true,::std::memory_order_release);
    g_runtime.published_runtime_state=make_runtime_state_signature();
    g_runtime.bridges_initialized.store(true,::std::memory_order_release);
    g_runtime.compiled_all.store(true,::std::memory_order_release);
    if(publication_replay_)publication_replay_->epoch=publication_epoch_;
    // Retain the private owner until enrolled native participants have been
    // bound to their saved logical IDs before the startup gate opens.
    checkpoint_replay_install(publication_replay_);
    published_=true;return true;
}
