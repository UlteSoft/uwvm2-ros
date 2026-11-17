// Included inside the actual runtime namespace AFTER the resolved import cache,
// raw-entry dispatch, canonical full publications and checkpoint TLS are defined.
// Full compiler selection uses the retained resumable profile only. Ordinary
// and observation-only engines keep their prior generated bridge calls.
// Effects classification never mints a host-operation, world-stop, asset-registry,
// checkpoint publication or executable-restore capability.
#pragma once

#if defined(UWVM_RUNTIME_LLVM_JIT)

namespace details
{
    extern "C++" void llvm_jit_checkpoint_call_raw_from_generated_wasm_abi_bridge(
        ::std::uintptr_t, ::std::uintptr_t, ::std::uintptr_t, ::std::size_t,
        ::std::uintptr_t, ::std::size_t) UWVM_THROWS;
    extern "C++" void llvm_jit_checkpoint_raw_target_abi_bridge(
        ::std::uintptr_t, ::std::uintptr_t, ::std::uintptr_t, ::std::size_t,
        ::std::uintptr_t, ::std::size_t) UWVM_THROWS;
}

#if defined(UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL) && UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL == 1
extern "C++" void uwvm2test_checkpoint_before_native_effect(unsigned, bool) noexcept;
#endif

#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
namespace
{
    enum class checkpoint_resolved_effect : unsigned char
    { defined_wasm, unadapted_host, invalid_actual_owner, checkpoint_builtin_host };

    [[nodiscard]] bool checkpoint_effect_actual_full_owner(::std::size_t module_id, bool require_checkpoint = true) noexcept
    {
        if(module_id >= g_runtime.modules.size() || (require_checkpoint && !g_runtime.checkpoint_profile)) { return false; }
        // [actual generation-pinned module records ... module_id ... count] end
        // [safe                                                        ] check
        // membership before indexing; no caller pointer is dereferenced here.
        auto const& record{g_runtime.modules.index_unchecked(module_id)};
        auto const publication{record.llvm_jit_full_publication.get()};
        auto const module{record.runtime_module};
        if(module == nullptr || publication == nullptr || !record.llvm_jit_ready ||
           !publication->engine || !publication->context || publication->plan) { return false; }
        if(require_checkpoint && (!publication->checkpoint_profile || publication->checkpoint_profile != g_runtime.checkpoint_profile ||
           publication->checkpoint_profile.owner_before(g_runtime.checkpoint_profile) ||
           g_runtime.checkpoint_profile.owner_before(publication->checkpoint_profile))) { return false; }
        if(!require_checkpoint && (!g_runtime.debug_pause_control ||
           publication->debug_source_runtime_epoch != current_runtime_generation() ||
           publication->debug_shutdown_abi_revision.load(::std::memory_order_acquire) != 1u)) { return false; }
        auto const& source{publication->source};
        if(!::uwvm2::uwvm::runtime::full::full_source_instance::has_canonical_owner(source) ||
           !source->initialized_from_actual_state()) { return false; }
        auto const member{source->registry().find(record.module_name)};
        return member != source->registry().end() && ::std::addressof(member->second) == module;
    }

    [[nodiscard]] checkpoint_resolved_effect checkpoint_effect_actual_cached_leaf(
        cached_import_target const& target, bool require_checkpoint = true,
        ::uwvm2::uwvm::runtime::full::builtin_checkpoint_function_data* builtin = nullptr) noexcept
    {
        // Only an actual cache member may reach this helper. The cache owns the
        // final leaf; an import name, user enum or replay-adapter boolean is not
        // accepted. No arbitrary host callback currently has a replay adapter.
        // A genuine stable builtin has a current-state adapter, independently
        // of its unavailable external-effect replay history.
        switch(target.k)
        {
            case cached_import_target::kind::local_imported:
            {
                if(!require_checkpoint || target.frame.module_id != target.origin_module_id ||
                   !checkpoint_effect_actual_full_owner(target.origin_module_id))
                { return checkpoint_resolved_effect::unadapted_host; }
                auto const& record{g_runtime.modules.index_unchecked(target.origin_module_id)};
                auto const module{record.runtime_module};
                if(target.frame.function_index >= module->imported_function_vec_storage.size())
                { return checkpoint_resolved_effect::invalid_actual_owner; }
                auto const& original{module->imported_function_vec_storage.index_unchecked(target.frame.function_index)};
                auto const leaf{resolve_import_leaf_assuming_initialized(::std::addressof(original))};
                auto const location{details::find_runtime_imported_function_location(g_runtime.modules,leaf)};
                if(!location.found || !checkpoint_effect_actual_full_owner(location.module_id))
                { return checkpoint_resolved_effect::invalid_actual_owner; }
                auto const& provider{g_runtime.modules.index_unchecked(location.module_id)};
                // Locate the genuine final import in its owning module before
                // reading it. Forwarding an import does not change the caller's
                // independently selected WASI/ROS environment in normal dispatch.
                auto const& final{provider.runtime_module->imported_function_vec_storage.index_unchecked(location.function_index)};
                using link = ::uwvm2::uwvm::runtime::storage::imported_function_link_kind;
                if(final.link_kind != link::local_imported ||
                   final.target.local_imported.module_ptr != target.u.local_imported.module_ptr ||
                   final.target.local_imported.index != target.u.local_imported.index ||
                   target.reference_params || target.reference_results)
                { return checkpoint_resolved_effect::unadapted_host; }
                ::uwvm2::uwvm::runtime::full::builtin_checkpoint_function_data data{};
                auto const& source{provider.llvm_jit_full_publication->source};
                if(!source->actual_checkpoint_builtin_function(location.module_id,current_runtime_generation(),
                    provider.runtime_module,location.function_index,data))
                { return checkpoint_resolved_effect::unadapted_host; }
                // Canonical loader identity, concrete original wrapper, native
                // vector membership and source epoch are checked above. No
                // import spelling, file flag or guest-supplied pointer suffices.
                if(builtin)*builtin=data;
                return checkpoint_resolved_effect::checkpoint_builtin_host;
            }
            case cached_import_target::kind::dl:
                return checkpoint_resolved_effect::unadapted_host;
            case cached_import_target::kind::defined:
            {
                if(!checkpoint_effect_actual_full_owner(target.frame.module_id,require_checkpoint))
                { return checkpoint_resolved_effect::invalid_actual_owner; }
                auto const module{g_runtime.modules.index_unchecked(target.frame.module_id).runtime_module};
                auto const imports{module->imported_function_vec_storage.size()};
                if(target.frame.function_index < imports ||
                   target.frame.function_index - imports >= module->local_defined_function_vec_storage.size())
                { return checkpoint_resolved_effect::invalid_actual_owner; }
                // [actual provider local functions ... bounded local_index] end
                // [safe                                                  ] the
                // non-import prefix and local extent were checked before taking
                // an address; equality never dereferences target.runtime_func.
                auto const& local{module->local_defined_function_vec_storage.index_unchecked(target.frame.function_index - imports)};
                return ::std::addressof(local) == target.u.defined.runtime_func ?
                    checkpoint_resolved_effect::defined_wasm : checkpoint_resolved_effect::invalid_actual_owner;
            }
        }
        return checkpoint_resolved_effect::invalid_actual_owner;
    }

    [[nodiscard]] checkpoint_resolved_effect checkpoint_effect_actual_import(
        ::std::uintptr_t module_address, ::std::uintptr_t function_index,
        ::std::size_t parameter_bytes, ::std::size_t result_bytes, bool require_checkpoint = true, cached_import_target const** selected = nullptr) noexcept
    {
        // [comparison-only native address] no object permission from arithmetic
        // [safe                          ] resolve actual registry membership
        // BEFORE any module read. The execution lease pins this generation.
        auto const candidate{reinterpret_cast<runtime_module_storage_t const*>(module_address)};
        auto const module_id{find_runtime_module_id_from_storage_ptr(candidate)};
        if(!checkpoint_effect_actual_full_owner(module_id,require_checkpoint))
        { return checkpoint_resolved_effect::invalid_actual_owner; }
        auto const module{g_runtime.modules.index_unchecked(module_id).runtime_module};
        auto const imports{module->imported_function_vec_storage.size()};
        if(function_index >= imports)
        {
            return function_index - imports < module->local_defined_function_vec_storage.size() ?
                checkpoint_resolved_effect::defined_wasm : checkpoint_resolved_effect::invalid_actual_owner;
        }
        if(module_id >= g_import_call_cache.size()) { return checkpoint_resolved_effect::invalid_actual_owner; }
        auto const& cache{g_import_call_cache.index_unchecked(module_id)};
        if(function_index >= cache.size()) { return checkpoint_resolved_effect::invalid_actual_owner; }
        // [actual resolved cache ... bounded imported index] end
        // [safe                                           ] both dimensions
        // checked before indexing; ABI extents match the actual final leaf.
        auto const& target{cache.index_unchecked(function_index)};
        if(target.origin_module_id != module_id || target.param_bytes != parameter_bytes || target.result_bytes != result_bytes)
        { return checkpoint_resolved_effect::invalid_actual_owner; }
        if(selected)*selected=::std::addressof(target);
        return checkpoint_effect_actual_cached_leaf(target,require_checkpoint);
    }

    [[nodiscard]] checkpoint_resolved_effect checkpoint_effect_actual_raw_target(
        ::std::uintptr_t entry_address, ::std::uintptr_t context_address,
        ::std::size_t parameter_bytes, ::std::size_t result_bytes, bool require_checkpoint = true, cached_import_target const** selected = nullptr) noexcept
    {
        if(entry_address == reinterpret_cast<::std::uintptr_t>(llvm_jit_raw_call_cached_import_entry))
        {
            for(::std::size_t module_id{}; module_id != g_import_call_cache.size(); ++module_id)
            {
                auto const& cache{g_import_call_cache.index_unchecked(module_id)};
                auto const count{cache.size()};
                if(count == 0u || count > UINTPTR_MAX / sizeof(cached_import_target)) { continue; }
                // [actual contiguous cache allocation] count complete records
                // [safe                              ] inspect a native address
                // range without advancing/dereferencing the supplied context.
                auto const begin{reinterpret_cast<::std::uintptr_t>(cache.data())};
                auto const bytes{count * sizeof(cached_import_target)};
                if(begin > UINTPTR_MAX - bytes || context_address < begin || context_address - begin >= bytes) { continue; }
                auto const offset{context_address - begin};
                if(offset % sizeof(cached_import_target) != 0u || !checkpoint_effect_actual_full_owner(module_id,require_checkpoint))
                { return checkpoint_resolved_effect::invalid_actual_owner; }
                auto const index{offset / sizeof(cached_import_target)};
                // [actual cache0 ... exact index ... count] cache_end
                // [safe                                  ] offset<bytes and
                // exact record alignment BEFORE indexing. Never read an
                // interior/one-past pointer or a partial guessed native record.
                auto const& target{cache.index_unchecked(index)};
                if(target.origin_module_id != module_id || target.param_bytes != parameter_bytes || target.result_bytes != result_bytes)
                { return checkpoint_resolved_effect::invalid_actual_owner; }
                if(selected)*selected=::std::addressof(target);
        return checkpoint_effect_actual_cached_leaf(target,require_checkpoint);
            }
            return checkpoint_resolved_effect::invalid_actual_owner;
        }
        // An eager full raw wrapper's context is zero. Checkpoint full engines
        // do not admit lazy/tiered/native-import replacement entry conventions.
        // Old retained addresses not in this current publication conservatively
        // decline replay; this is not permission to invoke a recovered pointer.
        if(context_address == 0u)
        {
            for(::std::size_t module_id{}; module_id != g_runtime.modules.size(); ++module_id)
            {
                if(!checkpoint_effect_actual_full_owner(module_id,require_checkpoint)) { continue; }
                auto const& entries{g_runtime.modules.index_unchecked(module_id).llvm_jit_local_raw_entry_addresses};
                for(auto const actual : entries)
                { if(actual != 0u && actual == entry_address) { return checkpoint_resolved_effect::defined_wasm; } }
            }
        }
        return checkpoint_resolved_effect::invalid_actual_owner;
    }

    [[nodiscard]] bool checkpoint_record_actual_effect(checkpoint_resolved_effect effect) noexcept
    {
        namespace cp = ::uwvm2::runtime::checkpoint;
        auto const shadow{g_checkpoint_shadow_ledger};
        auto const actual_lease{get_runtime_execution_lease()};
        // All pointers are actual scope-owned TLS borrows. Their construction
        // precedes generated entry; no address or bool from guest/file DATA is
        // accepted. A retired host-tail frame may have an empty valid ledger.
        if(shadow == nullptr || actual_lease == nullptr || !*actual_lease || actual_lease->stop_requested() ||
           g_debug_observer_active || g_debug_pause_participant == nullptr || !g_runtime.debug_pause_control ||
           !g_runtime.checkpoint_profile ||
           g_runtime.checkpoint_profile->purpose() != cp::compilation_purpose::resumable ||
           !g_runtime.checkpoint_host_enabled.load(::std::memory_order_acquire) ||
           g_debug_activation_ledger == nullptr || !g_debug_activation_ledger->valid())
        { return false; }
        switch(effect)
        {
            case checkpoint_resolved_effect::defined_wasm: return true;
            case checkpoint_resolved_effect::checkpoint_builtin_host:
                // Retain the history gap BEFORE calling the provider. Genuine
                // host admission/island scopes still exclude in-flight captures;
                // only a fresh, complete post-return stop can save live state.
                // Unknown providers keep the stronger sticky poison below.
                shadow->note_unrecorded_builtin_effect(); return true;
            case checkpoint_resolved_effect::unadapted_host:
                // Sticky BEFORE the actual provider. Later host returns,
                // exceptions and reentry cannot turn this into replay authority.
                shadow->poison(cp::status::non_replayable_import); return true;
            case checkpoint_resolved_effect::invalid_actual_owner:
                shadow->poison(cp::status::invalid_activation); return false;
        }
        shadow->poison(cp::status::invalid_activation); return false;
    }
#include "uwvm_runtime_checkpoint_execution_replay_import.h"
#if defined(UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL) && UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL == 1
    // Private test build only. The actual selected bridge invokes this AFTER
    // successful real foreign-operation admission and BEFORE provider entry.
    // Production builds contain neither this probe nor its call.
    void checkpoint_effect_before_actual_native_provider(bool const admitted) noexcept
    {
        uwvm2test_checkpoint_before_native_effect(
            static_cast<unsigned>(g_checkpoint_shadow_ledger->failure()), admitted);
    }
#endif

}
#endif

#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
extern "C++"
{
    checkpoint_replay_host_memory_scope::checkpoint_replay_host_memory_scope(unsigned operation,
        ::std::size_t memory,::std::uint64_t offset,::std::size_t size) noexcept
    {
        if(auto* actual=checkpoint_replay_import::active_memory_producer())
        {actual_=actual;actual->enter_memory(operation,memory,offset,size);}
    }
    bool checkpoint_replay_host_memory_scope::finish(bool accepted) noexcept
    {
        if(actual_)
        {
            auto* actual=checkpoint_replay_import::active_memory_producer();
            if(actual!=actual_)::fast_io::fast_terminate();
            actual->leave_memory(accepted);actual_=nullptr;
        }
        return accepted;
    }
    checkpoint_replay_host_memory_scope::~checkpoint_replay_host_memory_scope()
    {if(actual_)::fast_io::fast_terminate();}
}
#endif

extern "C++" void details::llvm_jit_debug_classified_import_abi_bridge(
    ::std::uintptr_t module, ::std::uintptr_t function, ::std::uintptr_t result,
    ::std::size_t result_bytes, ::std::uintptr_t parameters, ::std::size_t parameter_bytes) UWVM_THROWS
{
    if(get_llvm_jit_generated_bridge_scope_depth()==0u) { ::fast_io::fast_terminate(); }
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    cached_import_target const* replay_target{};
    auto const effect{checkpoint_effect_actual_import(module,function,parameter_bytes,result_bytes,
        checkpoint_replay_selected.load(::std::memory_order_acquire),::std::addressof(replay_target))};
    if(effect==checkpoint_resolved_effect::invalid_actual_owner) { ::fast_io::fast_terminate(); }
    if(effect==checkpoint_resolved_effect::defined_wasm)
    {
        details::llvm_jit_call_raw_from_generated_wasm_abi_bridge(
            module,function,result,result_bytes,parameters,parameter_bytes);
        return;
    }
#endif
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    checkpoint_replay_import journal{replay_target,parameters,parameter_bytes,result,result_bytes};
    if(journal.replayed())return;
#endif
    deferred_builtin_memtag_fault fault{};
    {
        details::llvm_jit_debug_host_scope scope{};
        details::llvm_jit_call_raw_from_generated_wasm_abi_bridge(
            module,function,result,result_bytes,parameters,parameter_bytes);
    }
    fault.finish();
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    journal.finish();
#endif
}
extern "C++" void details::llvm_jit_debug_classified_raw_target_abi_bridge(
    ::std::uintptr_t entry_address, ::std::uintptr_t context, ::std::uintptr_t result,
    ::std::size_t result_bytes, ::std::uintptr_t parameters, ::std::size_t parameter_bytes) UWVM_THROWS
{
    if(entry_address==0u || get_llvm_jit_generated_bridge_scope_depth()==0u) { ::fast_io::fast_terminate(); }
    using entry_type=void(UWVM2_RUNTIME_LLVM_JIT_RAW_ENTRY_PTR_ABI*)(
        ::std::uintptr_t,::std::uintptr_t,::std::size_t,::std::uintptr_t,::std::size_t);
    auto const entry{reinterpret_cast<entry_type>(entry_address)};
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    cached_import_target const* replay_target{};
    auto const effect{checkpoint_effect_actual_raw_target(entry_address,context,parameter_bytes,result_bytes,
        checkpoint_replay_selected.load(::std::memory_order_acquire),::std::addressof(replay_target))};
    if(effect==checkpoint_resolved_effect::invalid_actual_owner) { ::fast_io::fast_terminate(); }
    if(effect==checkpoint_resolved_effect::defined_wasm)
    { entry(context,result,result_bytes,parameters,parameter_bytes);return; }
#endif
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    checkpoint_replay_import journal{replay_target,parameters,parameter_bytes,result,result_bytes};
    if(journal.replayed())return;
#endif
    deferred_builtin_memtag_fault fault{};
    {
        details::llvm_jit_debug_host_scope scope{};
        entry(context,result,result_bytes,parameters,parameter_bytes);
    }
    fault.finish();
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    journal.finish();
#endif
}

extern "C++" void details::llvm_jit_checkpoint_call_raw_from_generated_wasm_abi_bridge(
    ::std::uintptr_t module_address, ::std::uintptr_t function_index,
    ::std::uintptr_t result, ::std::size_t result_bytes,
    ::std::uintptr_t parameters, ::std::size_t parameter_bytes) UWVM_THROWS
{
    if(get_llvm_jit_generated_bridge_scope_depth() == 0u) [[unlikely]] { ::fast_io::fast_terminate(); }
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    cached_import_target const* replay_target{};
    auto const effect{checkpoint_effect_actual_import(module_address,function_index,parameter_bytes,result_bytes,true,::std::addressof(replay_target))};
    if(!checkpoint_record_actual_effect(effect)) [[unlikely]] { ::fast_io::fast_terminate(); }
    if(effect != checkpoint_resolved_effect::defined_wasm)
    {
        checkpoint_replay_import journal{replay_target,parameters,parameter_bytes,result,result_bytes};
        if(journal.replayed())return;
        // Private retained entry/generation/profile/control issues the counted
        // scope. The classification enum does NOT admit a host operation, and
        // a typed-tail caller need not have a fabricated top activation here.
        deferred_builtin_memtag_fault fault{};
        {
        runtime_checkpoint_host_bridge::foreign_operation_scope host_operation{};
        if(!host_operation.admitted()) [[unlikely]] { ::fast_io::fast_terminate(); }
#if defined(UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL) && UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL == 1
        checkpoint_effect_before_actual_native_provider(host_operation.admitted());
#endif
        // This selected bridge is an internal control symbol: suspend only
        // the actual native leaf. A callback's nested guest activation must
        // retain the genuine foreign-host island/incomplete-stack distinction.
        details::llvm_jit_debug_host_scope debug_host_operation{};
        details::llvm_jit_call_raw_from_generated_wasm_abi_bridge(
            module_address, function_index, result, result_bytes, parameters, parameter_bytes);
        } // actual provider/island/admission guards have all left
        fault.finish();
        journal.finish();
        return;
    }
#endif
    // The original dispatch retains ABI checks, Wasm FP/depth suspension,
    // genuine host-provider lifetime and native exception propagation. A real
    // defined Wasm target does not acquire a foreign host-operation scope.
    // Neither the effect marker nor the private gate claims a coherent world.
    details::llvm_jit_call_raw_from_generated_wasm_abi_bridge(
        module_address, function_index, result, result_bytes, parameters, parameter_bytes);
}

extern "C++" void details::llvm_jit_checkpoint_raw_target_abi_bridge(
    ::std::uintptr_t entry_address, ::std::uintptr_t context,
    ::std::uintptr_t result, ::std::size_t result_bytes,
    ::std::uintptr_t parameters, ::std::size_t parameter_bytes) UWVM_THROWS
{
    if(entry_address == 0u || get_llvm_jit_generated_bridge_scope_depth() == 0u) [[unlikely]]
    { ::fast_io::fast_terminate(); }
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    cached_import_target const* replay_target{};
    auto const effect{checkpoint_effect_actual_raw_target(entry_address,context,parameter_bytes,result_bytes,true,::std::addressof(replay_target))};
    if(!checkpoint_record_actual_effect(effect)) [[unlikely]] { ::fast_io::fast_terminate(); }
    if(effect == checkpoint_resolved_effect::defined_wasm)
    {
        using entry_type = void(UWVM2_RUNTIME_LLVM_JIT_RAW_ENTRY_PTR_ABI*)(
            ::std::uintptr_t, ::std::uintptr_t, ::std::size_t, ::std::uintptr_t, ::std::size_t);
        // [actual checked full publication/cache raw entry] retained by lease
        // [safe                                          ] keep the exact raw
        // convention. No host island erases a genuine Wasm caller identity.
        // This address came from actual native target resolution, never a file.
        auto const entry{reinterpret_cast<entry_type>(entry_address)};
        entry(context, result, result_bytes, parameters, parameter_bytes); return;
    }
#endif
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    checkpoint_replay_import journal{replay_target,parameters,parameter_bytes,result,result_bytes};
    if(journal.replayed())return;
#endif
    // An unadapted/unknown host keeps the existing qualified forwarding scope.
    // Recording was invalidated before this possible native side effect.
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    runtime_checkpoint_host_bridge::foreign_operation_scope host_operation{};
    if(!host_operation.admitted()) [[unlikely]] { ::fast_io::fast_terminate(); }
#if defined(UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL) && UWVM2_TEST_CHECKPOINT_HOST_EFFECT_PRECALL == 1
    checkpoint_effect_before_actual_native_provider(host_operation.admitted());
#endif
#endif
    // The checked raw leaf above is already authenticated. Invoke it once;
    // routing through the observation wrapper would journal this call twice.
    using entry_type=void(UWVM2_RUNTIME_LLVM_JIT_RAW_ENTRY_PTR_ABI*)(
        ::std::uintptr_t,::std::uintptr_t,::std::size_t,::std::uintptr_t,::std::size_t);
    auto const entry=reinterpret_cast<entry_type>(entry_address);
    deferred_builtin_memtag_fault fault{};
    {
        details::llvm_jit_debug_host_scope debug_host_operation{};
        entry(context,result,result_bytes,parameters,parameter_bytes);
    }
    fault.finish();
#if defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
    journal.finish();
#endif
}
#endif
