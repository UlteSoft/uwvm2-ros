#pragma once
/* Test-only, architecture-neutral barrier. Force-include only in the Worker
 * main translation unit and the Gateway executable. Existing native witnesses
 * already have their own pre-init barrier and must not include this header.
 * SIGSTOP is observed by the real kernel (including user-QEMU); no Runtime,
 * import, ROS object or fake identity is substituted. Root resumes the exact
 * retained birth through its pidfd after matching the executable's real maps.
 */
#if !defined(__linux__)
#error "The multiarchitecture evidence lane requires the actual Linux pidfd/procfs host"
#endif
#include <signal.h>
namespace uwvm_multiarch_test
{
    struct startup_barrier
    {
        startup_barrier() noexcept { if(::raise(SIGSTOP)!=0) { __builtin_trap(); } }
    };
    inline startup_barrier before_main{};
}
