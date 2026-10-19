# SPDX-License-Identifier: Apache-2.0
# Explicit SOURCE-RECOMPILE toolchain for original ROS client libraries.
# This file grants no ROS, filesystem, Clock, debugger or management authority.
# Use the actual independently admitted SDK33 distribution, not facade headers.
include_guard(GLOBAL)
foreach(_role UWVM_WASI_SDK33_PREFIX UWVM_CROSS_PREFIX)
  if(NOT DEFINED ${_role} OR NOT IS_ABSOLUTE "${${_role}}" OR NOT IS_DIRECTORY "${${_role}}")
    message(FATAL_ERROR "Actual absolute admitted ${_role} directory is required")
  endif()
endforeach()
foreach(_role UWVM_SHARED_INITIAL_BYTES UWVM_SHARED_MAXIMUM_BYTES UWVM_MAIN_STACK_BYTES)
  if(NOT DEFINED ${_role} OR NOT "${${_role}}" MATCHES "^[1-9][0-9]*$" OR
     "${${_role}}" STRGREATER "9999999999")
    message(FATAL_ERROR "Explicit finite numeric ${_role} is required")
  endif()
  # Refuse excessively long input before CMake's signed integer arithmetic.
  string(LENGTH "${${_role}}" _digits)
  if(_digits GREATER 10)
    message(FATAL_ERROR "${_role} exceeds the wasm32 resource range")
  endif()
  math(EXPR _alignment "${${_role}} % 65536")
  if(NOT _alignment EQUAL 0 OR ${_role} GREATER 2147483648)
    message(FATAL_ERROR "${_role} must be page aligned and at most 2 GiB")
  endif()
endforeach()
if(UWVM_SHARED_INITIAL_BYTES GREATER UWVM_SHARED_MAXIMUM_BYTES OR
   UWVM_MAIN_STACK_BYTES GREATER_EQUAL UWVM_SHARED_INITIAL_BYTES)
  message(FATAL_ERROR "Main stack, initial and maximum shared memory are inconsistent")
endif()
# The fixed toolchain body is official SDK33 source, not a string name or a
# compiler-version certificate. Root separately admits every actual tool/sysroot.
set(_pthread "${UWVM_WASI_SDK33_PREFIX}/share/cmake/wasi-sdk-pthread.cmake")
if(NOT EXISTS "${_pthread}" OR IS_DIRECTORY "${_pthread}")
  message(FATAL_ERROR "Actual SDK33 installed pthread toolchain is missing")
endif()
file(SHA256 "${_pthread}" _pthread_sha)
if(NOT _pthread_sha STREQUAL "c71f5fdc85acdd6864d125427cacab94345f5549b5a75a1819ee84334a8916c3")
  message(FATAL_ERROR "Installed pthread toolchain differs from fixed SDK33 source")
endif()
foreach(_tool clang clang++ llvm-ar llvm-ranlib)
  if(NOT EXISTS "${UWVM_WASI_SDK33_PREFIX}/bin/${_tool}" OR
     IS_DIRECTORY "${UWVM_WASI_SDK33_PREFIX}/bin/${_tool}")
    message(FATAL_ERROR "Actual SDK33 tool is missing: ${_tool}")
  endif()
endforeach()
set(_sysroot "${UWVM_WASI_SDK33_PREFIX}/share/wasi-sysroot")
foreach(_lane include/wasm32-wasi-threads/eh/c++/v1 lib/wasm32-wasi-threads/eh)
  if(NOT IS_DIRECTORY "${_sysroot}/${_lane}")
    message(FATAL_ERROR "Actual SDK33 threaded dual-EH sysroot lane is missing: ${_lane}")
  endif()
endforeach()
# Original toolchain reads this explicit prefix; it never inherits HOME or PATH
# as an alternate SDK. Retain its genuine -pthread and import/export-memory.
set(WASI_SDK_PREFIX "${UWVM_WASI_SDK33_PREFIX}")
include("${_pthread}")
set(CMAKE_SYSROOT "${_sysroot}")
# SDK33's pinned documentation requires ALL of these for standard Wasm EH.
# In particular linking also needs -fwasm-exceptions to select the real EH STL.
set(_eh "-fwasm-exceptions -mllvm -wasm-use-legacy-eh=false")
string(APPEND CMAKE_C_FLAGS " ${_eh}")
string(APPEND CMAKE_CXX_FLAGS " ${_eh}")
string(APPEND CMAKE_EXE_LINKER_FLAGS
  " ${_eh} -lunwind -Wl,--initial-memory=${UWVM_SHARED_INITIAL_BYTES} -Wl,--max-memory=${UWVM_SHARED_MAXIMUM_BYTES} -Wl,-z,stack-size=${UWVM_MAIN_STACK_BYTES}")
set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
set(CMAKE_FIND_ROOT_PATH "${_sysroot};${UWVM_CROSS_PREFIX}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
  UWVM_WASI_SDK33_PREFIX UWVM_CROSS_PREFIX UWVM_SHARED_INITIAL_BYTES
  UWVM_SHARED_MAXIMUM_BYTES UWVM_MAIN_STACK_BYTES)
# A successful CMake compiler check is not qualification. The actual signal
# executable link contract and produced Wasm imports/shared memory/TLS/EH must
# still be independently admitted; the original project hook remains closed
# until its genuine threaded/default-control source composition is complete.
