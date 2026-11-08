// Included in the private runtime namespace after trap names/runtime state.
#pragma once
namespace
{
    [[nodiscard]] inline bool crash_policy_guest_fault(trap_kind kind) noexcept
    {
        switch(kind)
        {
            case trap_kind::unreachable: case trap_kind::invalid_conversion_to_integer:
            case trap_kind::integer_divide_by_zero: case trap_kind::integer_overflow:
            case trap_kind::call_indirect_table_out_of_bounds: case trap_kind::call_indirect_null_element:
            case trap_kind::call_indirect_type_mismatch: case trap_kind::table_out_of_bounds:
            case trap_kind::memory_out_of_bounds: case trap_kind::unaligned_atomic:
            case trap_kind::atomic_wait_non_shared: case trap_kind::null_reference:
            case trap_kind::array_out_of_bounds: case trap_kind::gc_allocation_failure:
            case trap_kind::cast_failure: case trap_kind::memtag_violation:
            case trap_kind::uncatched_int_tag: return true;
            default: return false;
        }
    }

    [[nodiscard]] inline auto crash_category(trap_kind kind) noexcept
    {
        namespace crash=::uwvm2::runtime::diagnostic::crash;
        if(kind==trap_kind::unreachable)return crash::category::unreachable;
        if(kind==trap_kind::memtag_violation)return crash::category::memtag;
        return crash_policy_guest_fault(kind)?crash::category::execution:crash::category::internal;
    }
    inline void apply_guest_crash_policy(trap_kind kind,bool after_debug=false) noexcept
    {
        namespace crash=::uwvm2::runtime::diagnostic::crash;
        if(get_runtime_execution_entry_depth()==0u) return;
        auto const name=trap_kind_name(kind);
        // trap_kind_name returns static literals, never a Guest string.
        crash::report(crash_category(kind),reinterpret_cast<char const*>(name.data()),
            ::uwvm2::uwvm::runtime::runtime_mode::global_runtime_compiler==
                ::uwvm2::uwvm::runtime::runtime_mode::runtime_compiler_t::uwvm_interpreter_only ? 1u:2u);
        if(crash::process_policy.selected()!=crash::action::pause)return;
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD)
        // Use an already-authorized controller's genuine terminal stop when
        // available. Never create a controller or reopen RELEASE permission.
        if(g_runtime.debug_pause_control && !after_debug)return;
        if(crash::stop_state.load(::std::memory_order_acquire)==2u)return;
#else
        (void)after_debug;
#endif
        crash::pause_process();
    }
}
