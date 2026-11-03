/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)            *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once

#if !defined(__cpp_exceptions) || !defined(__EXCEPTIONS)
# error "uwvm2-ros requires native C++ exception handling in every runtime consumer"
#endif

/**
 * @file paired_unwind.h
 * @brief Bind ROS native EH to the repository-owned LLVM runtime headers.
 * @details Xmake verifies the complete source inventory and builds libc++,
 * libc++abi and libunwind with the same compiler/target as the paired LLVM.
 * Qualified header names prevent a host GCC or Clang resource unwind.h from
 * silently changing this ABI. This adapter does not implement an unwinder.
 */
#if !defined(UWVM_ROS_PAIRED_RUNTIME_HEADERS) || UWVM_ROS_PAIRED_RUNTIME_HEADERS != 1
# error "uwvm2-ros requires repository-owned paired LLVM runtime headers and archives"
#endif
#if defined(__UNWIND_H__) && !defined(_LIBUNWIND_VERSION)
# error "An external unwind.h was included before the ROS paired provider"
#endif
#include <llvm-runtimes/upstream/libunwind/include/__libunwind_config.h>
#include <llvm-runtimes/upstream/libunwind/include/unwind.h>
#if _LIBUNWIND_VERSION != 230000
# error "The ROS unwind header does not match the pinned LLVM 23 runtime ABI"
#endif
