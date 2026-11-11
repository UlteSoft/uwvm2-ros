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
    inline void report_guest_fault(trap_kind kind,char const* reason,unsigned backend,
        ::uwvm2::runtime::diagnostic::crash::memory_detail const* memory=nullptr) noexcept
    {
        ::std::uint64_t module{UINT64_MAX},function{UINT64_MAX};
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
        if(backend==1u)
        {
            auto const& frames=get_call_stack().frames;
            if(!frames.empty()){module=frames.back().module_id;function=frames.back().function_index;}
        }
#endif
#if defined(UWVM_RUNTIME_LLVM_JIT)
        if(backend==2u)
        {
# if defined(UWVM_USE_THREAD_LOCAL)
            auto const pc=llvm_jit_trap_return_address;
# else
            auto const pc=get_llvm_jit_trap_return_address();
# endif
            auto const resolved=resolve_llvm_jit_unwind_entry(pc==0u ? 0u : pc-1u);
            if(resolved.entry && !resolved.entry->wrapper_entry)
            {module=resolved.entry->module_id;function=resolved.entry->function_index;}
# if UWVM2_RUNTIME_LLVM_JIT_HAS_UNWIND_H_BACKTRACE
            else
            {
                // Some checked bulk-memory helpers report through the memory
                // provider and have no direct generated trap return PC. Recover
                // the genuine innermost Wasm frame only on this cold fault path;
                // successful memory operations need no provenance publication.
                struct location {::std::uint64_t* module; ::std::uint64_t* function;};
                location found{&module, &function};
                static_cast<void>(::uwvm2::runtime::compiler::llvm_jit::native_unwind_abi::backtrace_noexcept(
                    +[](_Unwind_Context* context, void* opaque) noexcept -> _Unwind_Reason_Code
                    {
                        int before{};
                        auto ip{static_cast<::std::uintptr_t>(
                            ::uwvm2::runtime::compiler::llvm_jit::native_unwind_abi::get_ip_info_noexcept(context, &before))};
                        if(ip != 0u && !before) { --ip; }
                        auto const entry{resolve_llvm_jit_unwind_entry(ip)};
                        if(entry.entry == nullptr || entry.entry->wrapper_entry) { return _URC_NO_REASON; }
                        auto* output{static_cast<location*>(opaque)};
                        *output->module = entry.entry->module_id;
                        *output->function = entry.entry->function_index;
                        return _URC_END_OF_STACK;
                    }, &found));
            }
# endif
        }
#endif
        // A native offset is not a Wasm byte offset. No instruction coordinate
        // is manufactured when the real producer did not retain that mapping.
        ::uwvm2::runtime::diagnostic::crash::report(crash_category(kind),reason,backend,module,function,UINT64_MAX,memory);
    }
    inline void apply_guest_crash_policy(trap_kind kind,bool after_debug=false) noexcept
    {
        namespace crash=::uwvm2::runtime::diagnostic::crash;
        if(get_runtime_execution_entry_depth()==0u) return;
        auto const name=trap_kind_name(kind);
        // trap_kind_name returns static literals, never a Guest string.
        unsigned backend_id{2u};
#if defined(UWVM_RUNTIME_UWVM_INTERPRETER)
        if(::uwvm2::uwvm::runtime::runtime_mode::global_runtime_compiler==
            ::uwvm2::uwvm::runtime::runtime_mode::runtime_compiler_t::uwvm_interpreter_only) { backend_id=1u; }
#endif
        report_guest_fault(kind,reinterpret_cast<char const*>(name.data()),backend_id);
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
