#pragma once
// Private cold completion policy. Call only after the genuine Guest unwind
// and the owning runtime's activation/GC/checkpoint validation have succeeded.
// crash_policy and process_exit declarations are supplied by the runtime TU.
namespace runtime_managed_fault_detail
{
    inline void finish_cli_fault_after_unwind() noexcept
    {
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        namespace crash = ::uwvm2::runtime::diagnostic::crash;
        // Unsealed native embedding retains its managed-fault return contract.
        // A sealed CLI policy explicitly selects process containment. Preserve
        // its first recorded category and the existing first-exit/wait owners.
        if(crash::process_policy.sealed() && crash::process_policy.selected() != crash::action::pause &&
           crash::first_fault.load(::std::memory_order_acquire) != 0u)
        {
            crash::crash_unless_stable_exit();
            ::uwvm2::utils::thread::process_exit::request_process_exit(crash::exit_status());
        }
#endif
    }
}
