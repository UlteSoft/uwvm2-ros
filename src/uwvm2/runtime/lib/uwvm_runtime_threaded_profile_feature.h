#pragma once
// Availability of genuine Host source, never an execution or ROS permission.
#if defined(__linux__) && defined(UWVM_USE_THREAD_LOCAL) && defined(UWVM_UTILS_HAS_FAST_IO_NATIVE_THREAD) && defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE) && (defined(UWVM_RUNTIME_UWVM_INTERPRETER) || defined(UWVM_RUNTIME_LLVM_JIT))
# define UWVM2_HAS_THREADED_PROFILE_HOST_API 1
#else
# define UWVM2_HAS_THREADED_PROFILE_HOST_API 0
#endif
