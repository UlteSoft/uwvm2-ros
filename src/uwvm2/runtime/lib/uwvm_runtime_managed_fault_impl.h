#pragma once
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
bool runtime_managed_fault::raise(llvm_jit_trap_kind kind)
{
    // Only specified Guest traps, never an invariant/protocol/helper failure.
    switch(kind)
    {
        case llvm_jit_trap_kind::unreachable:
        case llvm_jit_trap_kind::invalid_conversion_to_integer:
        case llvm_jit_trap_kind::integer_divide_by_zero:
        case llvm_jit_trap_kind::integer_overflow:
        case llvm_jit_trap_kind::call_indirect_table_out_of_bounds:
        case llvm_jit_trap_kind::call_indirect_null_element:
        case llvm_jit_trap_kind::call_indirect_type_mismatch:
        case llvm_jit_trap_kind::memory_out_of_bounds:
        case llvm_jit_trap_kind::table_out_of_bounds:
        case llvm_jit_trap_kind::unaligned_atomic:
        case llvm_jit_trap_kind::atomic_wait_non_shared:
        case llvm_jit_trap_kind::null_reference:
        case llvm_jit_trap_kind::array_out_of_bounds:
        case llvm_jit_trap_kind::gc_allocation_failure:
        case llvm_jit_trap_kind::cast_failure: break;
        default: return false;
    }
    auto const epoch{current_runtime_generation()};
    auto const* participant{g_debug_pause_participant}; auto const* ledger{g_debug_activation_ledger};
    if(!catchable_ || enabled_epoch_.load(::std::memory_order_acquire) != epoch ||
       get_runtime_execution_entry_depth() != 1u || get_llvm_jit_generated_bridge_scope_depth() == 0u ||
       g_debug_observer_active || !g_runtime.debug_pause_control || participant == nullptr ||
       ledger == nullptr || !ledger->complete() || ledger->count() == 0u) { return false; }
    {
        runtime_state_publication_guard publication{};
        for(::std::size_t i{}; i < ledger->count(); ++i)
        {
            auto const& activation{ledger->data()[i]};
            if(activation.runtime_epoch != epoch || activation.module >= g_runtime.modules.size()) { return false; }
            auto const& rec{g_runtime.modules.index_unchecked(activation.module)};
            auto const* code{rec.llvm_jit_full_publication.get()};
            if(!rec.runtime_module || !code || !code->source ||
               code->debug_shutdown_abi_revision.load(::std::memory_order_acquire) != 1u) { return false; }
            auto const imports{rec.runtime_module->imported_function_vec_storage.size()};
            if(activation.function < imports) { return false; }
            auto const local{activation.function - imports};
            if(local >= rec.runtime_module->local_defined_function_vec_storage.size()) { return false; }
            auto generation{::std::uint_least64_t{1u}};
            if(!rec.llvm_jit_debug_full_entry_generations.empty())
            {
                if(local >= rec.llvm_jit_debug_full_entry_generations.size()) { return false; }
                generation = rec.llvm_jit_debug_full_entry_generations[local];
            }
            if(generation == 0u || generation != activation.function_generation) { return false; }
        }
    }
    trap_kind diagnostic{trap_kind::runtime_invariant_failure};
    switch(kind)
    {
        case llvm_jit_trap_kind::unreachable: diagnostic = trap_kind::unreachable; break;
        case llvm_jit_trap_kind::invalid_conversion_to_integer: diagnostic = trap_kind::invalid_conversion_to_integer; break;
        case llvm_jit_trap_kind::integer_divide_by_zero: diagnostic = trap_kind::integer_divide_by_zero; break;
        case llvm_jit_trap_kind::integer_overflow: diagnostic = trap_kind::integer_overflow; break;
        case llvm_jit_trap_kind::call_indirect_table_out_of_bounds: diagnostic = trap_kind::call_indirect_table_out_of_bounds; break;
        case llvm_jit_trap_kind::call_indirect_null_element: diagnostic = trap_kind::call_indirect_null_element; break;
        case llvm_jit_trap_kind::call_indirect_type_mismatch: diagnostic = trap_kind::call_indirect_type_mismatch; break;
        case llvm_jit_trap_kind::memory_out_of_bounds: diagnostic = trap_kind::memory_out_of_bounds; break;
        case llvm_jit_trap_kind::table_out_of_bounds: diagnostic = trap_kind::table_out_of_bounds; break;
        case llvm_jit_trap_kind::unaligned_atomic: diagnostic = trap_kind::unaligned_atomic; break;
        case llvm_jit_trap_kind::atomic_wait_non_shared: diagnostic = trap_kind::atomic_wait_non_shared; break;
        case llvm_jit_trap_kind::null_reference: diagnostic = trap_kind::null_reference; break;
        case llvm_jit_trap_kind::array_out_of_bounds: diagnostic = trap_kind::array_out_of_bounds; break;
        case llvm_jit_trap_kind::gc_allocation_failure: diagnostic = trap_kind::gc_allocation_failure; break;
        case llvm_jit_trap_kind::cast_failure: diagnostic = trap_kind::cast_failure; break;
        default: break;
    }
    // The DEV terminal observer borrows the still-live operand/activation
    // snapshot. Publish it before request_stop closes the pause domain and
    // before the managed signal unwinds those frames. RELEASE has no terminal
    // observer, and the observer independently authenticates the current leaf.
    // Continuing the terminal stop still follows managed fault termination.
    debug_observe_actual_trap(diagnostic);
    // Serialize this already validated Guest fault against the first authorized
    // process exit. An exit that won first must retain its explicit wait owners;
    // a late Guest trap cannot cancel them by closing the execution domain.
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    auto epoch_for_stop{epoch};
    if(!::uwvm2::utils::thread::process_exit::run_before_exit_request([](void* pointer) noexcept
    {
        fault_epoch_.store(*static_cast<decltype(epoch_for_stop)*>(pointer), ::std::memory_order_release);
        g_runtime.execution_domain.request_stop();
    }, &epoch_for_stop)) { ::uwvm2::utils::thread::process_exit::request_process_exit(0u); }
#else
    fault_epoch_.store(epoch, ::std::memory_order_release);
    g_runtime.execution_domain.request_stop();
#endif
    diagnostic_detail::fault(static_cast<::std::uint32_t>(kind), ledger);
    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
        u8"uwvm: [error] managed Guest trap; execution stopped (kind=", static_cast<unsigned>(kind), u8")");
    // Retain bounded, explicitly selected native stack diagnostics while the
    // genuine frames still exist; this does not expose a debugger capability.
    if(runtime_llvm_jit_unwind_call_stack_requested())
    {
        dump_call_stack_for_trap(diagnostic);
    }
    throw signal{epoch, participant->identifier()};
}
bool runtime_managed_fault::stop_after_unwind() noexcept
{
    auto const epoch{current_runtime_generation()};
    auto const* ledger{g_debug_activation_ledger};
    if(enabled_epoch_.load(::std::memory_order_acquire) != epoch ||
       get_runtime_execution_entry_depth() != 1u || !g_runtime.debug_pause_control ||
       g_debug_pause_participant == nullptr || ledger == nullptr || !ledger->valid() || ledger->count() != 0u ||
       ::uwvm2::runtime::gc::current_root_frames() != nullptr || g_debug_observer_active) { return false; }
    if(g_checkpoint_shadow_ledger)
    {
        if(g_checkpoint_shadow_ledger->failure() != ::uwvm2::runtime::checkpoint::status::ok &&
           !g_checkpoint_shadow_ledger->discard_failed_recording()) { return false; }
        if(g_checkpoint_shadow_ledger->size() != 0u) { return false; }
    }
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    auto epoch_for_stop{epoch};
    if(!::uwvm2::utils::thread::process_exit::run_before_exit_request([](void* pointer) noexcept
    {
        fault_epoch_.store(*static_cast<decltype(epoch_for_stop)*>(pointer), ::std::memory_order_release);
        g_runtime.execution_domain.request_stop();
    }, &epoch_for_stop)) { ::uwvm2::utils::thread::process_exit::request_process_exit(0u); }
#else
    fault_epoch_.store(epoch, ::std::memory_order_release);
    g_runtime.execution_domain.request_stop();
#endif
    return true;
}
bool runtime_managed_fault::accept_uncaught(::uwvm2::runtime::exception::guest_exception const& caught) noexcept
{
    // Only the typed, owning Wasm exception at a real outer native catch is
    // eligible. Tag identity must still be an actual current source member.
    // Unknown C++ exceptions and stale/foreign tags keep their original path.
    if(!caught.instance()) { return false; }
    auto const tag{resolve_guest_exception_tag_diagnostic(*caught.instance())};
    if(!tag.found || tag.module_id>=g_runtime.modules.size()) { return false; }
    auto const& record{g_runtime.modules.index_unchecked(tag.module_id)};
    auto const* code{record.llvm_jit_full_publication.get()};
    if(!record.runtime_module || !record.llvm_jit_ready || !code ||
       code->debug_source_runtime_epoch!=current_runtime_generation() ||
       !::uwvm2::uwvm::runtime::full::full_source_instance::has_canonical_owner(code->source) ||
       code->source->bound_initialized_file(tag.module_id,record.runtime_module)==nullptr) { return false; }
    // Native cleanup must have retired every activation, GC root and shadow
    // frame. This same private stop orders against the FIRST proc_exit claim,
    // so a late uncaught tag cannot cancel an already selected wait owner.
    if(!stop_after_unwind()) { return false; }
    diagnostic_detail::fault(0x10000u); // Typed uncaught Wasm exception; no payload retained.
    ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
        u8"uwvm: [error] uncaught Guest exception; execution stopped");
    return true; // No payload bytes or retained raw addresses enter diagnostics.
}
bool runtime_managed_fault::accept(signal const& caught) noexcept
{
    auto const* participant{g_debug_pause_participant}; auto const* ledger{g_debug_activation_ledger};
    if(caught.epoch_ != current_runtime_generation() || !faulted(caught.epoch_) ||
       participant == nullptr || participant->identifier() != caught.participant_ ||
       ledger == nullptr || !ledger->valid() || ledger->count() != 0u ||
       get_runtime_execution_entry_depth() != 1u || !runtime_execution_stop_requested_host_api() ||
       ::uwvm2::runtime::gc::current_root_frames() != nullptr || g_debug_observer_active) { return false; }
    if(g_checkpoint_shadow_ledger)
    {
        if(g_checkpoint_shadow_ledger->failure() != ::uwvm2::runtime::checkpoint::status::ok &&
           !g_checkpoint_shadow_ledger->discard_failed_recording()) { return false; }
        if(g_checkpoint_shadow_ledger->size() != 0u) { return false; }
    }
    return true;
}
#endif
extern "C++" UWVM_NOINLINE void llvm_jit_managed_runtime_trap(llvm_jit_trap_kind kind,
    ::std::uintptr_t frame, ::std::uintptr_t stack) UWVM_THROWS
{
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
    if(runtime_managed_fault::raise(kind)) { return; }
#endif
    llvm_jit_runtime_trap(kind, frame, stack);
}
extern "C++" UWVM_NOINLINE void llvm_jit_managed_memory_trap(::std::size_t index,
    ::std::uint_least64_t fixed, ::std::uint_least64_t offset, ::std::uint_least32_t overflow,
    ::std::uint_least64_t length, ::std::size_t width, ::std::uintptr_t frame, ::std::uintptr_t stack) UWVM_THROWS
{
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS)
    if(runtime_managed_fault::raise(llvm_jit_trap_kind::memory_out_of_bounds)) { return; }
#endif
    llvm_jit_memory_out_of_bounds_trap(index, fixed, offset, overflow, length, width, frame, stack);
}
