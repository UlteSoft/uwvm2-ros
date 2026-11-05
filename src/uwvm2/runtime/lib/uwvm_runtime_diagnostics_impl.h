#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
namespace diagnostic_detail
{
    inline ::std::mutex mutex{};
    inline diagnostic_snapshot record{};
    inline ::std::atomic<bool> enabled{}, lost{}, update_in_progress{};
    inline ::std::atomic<::std::uint64_t> started{}, completed{}, active{};
    struct root_scope
    {
        bool counted{};
        root_scope() noexcept
        {
            counted = enabled.load(::std::memory_order_acquire) && get_runtime_execution_entry_depth() == 1u;
            if(counted) { started.fetch_add(1u, ::std::memory_order_relaxed); active.fetch_add(1u, ::std::memory_order_relaxed); }
        }
        ~root_scope()
        { if(counted) { completed.fetch_add(1u, ::std::memory_order_relaxed); active.fetch_sub(1u, ::std::memory_order_relaxed); } }
    };
    inline void fault(::std::uint32_t kind, details::debug_activation::ledger const* ledger = nullptr) noexcept
    {
        if(!enabled.load(::std::memory_order_acquire)) { return; }
        ::std::unique_lock lock{mutex, ::std::try_to_lock};
        if(!lock) { lost.store(true, ::std::memory_order_relaxed); return; }
        // Called only after the real managed-fault owner/type/generation checks.
        if(record.faulted) { return; }
        record.faulted = true; record.fault_kind = kind; record.active = false; ++record.sequence;
        if(ledger && ledger->complete())
        {
            auto const total{ledger->count()};
            record.stack_truncated = total > record.frames.size();
            record.frame_count = static_cast<::std::uint32_t>((::std::min)(total, record.frames.size()));
            for(::std::size_t i{}; i < record.frame_count; ++i)
            {
                auto const& frame{ledger->data()[total - 1u - i]};
                record.frames[i] = {frame.module, frame.function, frame.function_generation};
            }
        }
    }
    inline void update(::uwvm2::utils::update::facts const& facts) noexcept
    {
        if(!enabled.load(::std::memory_order_acquire)) { return; }
        ::std::unique_lock lock{mutex, ::std::try_to_lock};
        if(!lock) { lost.store(true, ::std::memory_order_relaxed); return; }
        record.update_managed = true; record.active = facts.active && !record.faulted; record.coordinator_connected = facts.coordinator_connected;
        record.update_phase = static_cast<::std::uint32_t>(facts.state);
        record.installed_epoch = facts.installed_epoch; record.authenticated_commit_epoch = facts.authenticated_commit_epoch;
        record.decision_sequence = facts.decision_sequence; record.installed_sha256 = facts.installed; ++record.sequence;
    }
}
extern "C++" bool runtime_diagnostic_initialize_host_api() noexcept
{
    if(!::uwvm2::utils::control::management::runtime_policy.release_sealed() || get_runtime_execution_entry_depth() != 0u ||
       diagnostic_detail::enabled.load(::std::memory_order_acquire)) { return false; }
    diagnostic_snapshot next{};
    auto const& main{::uwvm2::uwvm::wasm::storage::active_execute_wasm()};
    auto const& preloads{::uwvm2::uwvm::wasm::storage::active_preloaded_wasm()};
    if(!main.has_owned_source_image() || preloads.size() > 4095u) { return false; }
    next.runtime_generation = current_runtime_generation(); next.sequence = 1u; next.active = true;
    next.total_modules = static_cast<::std::uint32_t>(1u + preloads.size());
    next.module_count = (::std::min)(next.total_modules, static_cast<::std::uint32_t>(next.modules.size()));
    next.module_truncated = next.total_modules > next.module_count;
    for(::std::size_t i{}; i < next.module_count; ++i)
    {
        auto const& file{i == 0u ? main : preloads.index_unchecked(i - 1u)};
        if(!file.has_owned_source_image()) { return false; }
        auto& module{next.modules[i]}; module.bytes = file.source_size();
        if(!::uwvm2::utils::update::hash({reinterpret_cast<unsigned char const*>(file.source_cbegin()),file.source_size()},module.sha256)) { return false; }
    }
    next.installed_sha256 = next.modules[0].sha256;
    ::std::lock_guard lock{diagnostic_detail::mutex}; diagnostic_detail::record = next;
    diagnostic_detail::enabled.store(true, ::std::memory_order_release); return true;
}
extern "C++" bool runtime_diagnostic_snapshot_host_api(diagnostic_snapshot& out) noexcept
{
    if(!diagnostic_detail::enabled.load(::std::memory_order_acquire)) { return false; }
    ::std::unique_lock lock{diagnostic_detail::mutex, ::std::try_to_lock};
    if(!lock) { return false; }
    out = diagnostic_detail::record;
    out.update_in_progress = diagnostic_detail::update_in_progress.load(::std::memory_order_acquire);
    out.roots_started = diagnostic_detail::started.load(::std::memory_order_relaxed);
    out.roots_completed = diagnostic_detail::completed.load(::std::memory_order_relaxed);
    out.roots_active = diagnostic_detail::active.load(::std::memory_order_relaxed);
    out.collection_lost = diagnostic_detail::lost.load(::std::memory_order_relaxed);
    return true;
}

#else
namespace diagnostic_detail { struct root_scope {}; }
extern "C++" bool runtime_diagnostic_initialize_host_api() noexcept { return false; }
extern "C++" bool runtime_diagnostic_snapshot_host_api(diagnostic_snapshot&) noexcept { return false; }
#endif
