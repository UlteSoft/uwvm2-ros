#pragma once
// Both translation and execution must agree on this capability. The product
// links the matching LLVM libunwind on these Linux ABIs; other builds retain
// explicit bounds until their fault-context delivery is implemented.
#if defined(__linux__) && defined(UWVM_ROS_PAIRED_RUNTIME_HEADERS) && \
    (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__) || defined(__arm__))
# define UWVM2_RUNTIME_HAS_LINUX_MMAP_FAULT_LANDING 1
#else
# define UWVM2_RUNTIME_HAS_LINUX_MMAP_FAULT_LANDING 0
#endif
