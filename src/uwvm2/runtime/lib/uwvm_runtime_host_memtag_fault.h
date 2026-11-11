// Private runtime include after trap diagnostics and host-fault record types.
#pragma once
template<bool Managed>
inline void deliver_host_memtag_fault(::uwvm2::runtime::memtag::host_fault const& fault) UWVM_THROWS
{
    if(!fault.pending) return;
#if defined(UWVM_RUNTIME_LLVM_JIT)
    if constexpr(Managed)
    if(get_llvm_jit_generated_bridge_scope_depth()!=0u)
    {
        llvm_jit_managed_memtag_trap(fault.memory_index,fault.pointer,fault.granule,
            fault.pointer_tag,fault.allocation_tag);
        return;
    }
#endif
    print_memtag_fault(fault.memory_index,fault.pointer,fault.granule,fault.pointer_tag,fault.allocation_tag);
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
    namespace crash=::uwvm2::runtime::diagnostic::crash;
    crash::memory_detail const detail{fault.memory_index,fault.pointer,fault.granule,fault.pointer_tag,fault.allocation_tag};
    report_guest_fault(trap_kind::memtag_violation,"host interface memory tag mismatch",0u,&detail);
#endif
    trap_fatal(trap_kind::memtag_violation);
}

// Only actual debug-selected foreign-call wrappers construct this scope.
// Each nesting level owns its fault, so a callback cannot move a failed
// Guest's attribution to a different outer activation. The wrapper calls
// finish only AFTER its real host island/admission/FP guards have left.
class deferred_builtin_memtag_fault final
{
    inline static thread_local deferred_builtin_memtag_fault* current_{};
    deferred_builtin_memtag_fault* previous_{current_};
    ::uwvm2::runtime::memtag::host_fault fault_{};
    bool attached_{true};
    void detach() noexcept
    {
        if(!attached_) return;
        if(current_!=this) ::fast_io::fast_terminate();
        current_=previous_;attached_=false;
    }
public:
    deferred_builtin_memtag_fault() noexcept {current_=this;}
    deferred_builtin_memtag_fault(deferred_builtin_memtag_fault const&)=delete;
    deferred_builtin_memtag_fault& operator=(deferred_builtin_memtag_fault const&)=delete;
    ~deferred_builtin_memtag_fault()
    {
        detach();
        // An unexpected native escape must not lose the FIRST tag failure
        // and later resume Guest. This noexcept fallback never grants unwind.
        if(fault_.pending) deliver_host_memtag_fault<false>(fault_);
    }
    [[nodiscard]] static bool retain(::uwvm2::runtime::memtag::host_fault const& fault) noexcept
    {
        if(current_==nullptr) return false;
        if(!current_->fault_.pending) current_->fault_=fault;
        return true;
    }
    void finish() UWVM_THROWS
    {
        detach();
        auto const fault{::std::exchange(fault_,{})};
        deliver_host_memtag_fault<true>(fault);
    }
};

template<bool Managed>
inline void finish_host_memtag_fault() UWVM_THROWS
{
    auto const fault{::uwvm2::runtime::memtag::consume_host_fault()};
    if(!fault.pending) [[likely]] return;
    if constexpr(Managed) if(deferred_builtin_memtag_fault::retain(fault)) return;
    deliver_host_memtag_fault<Managed>(fault);
}
