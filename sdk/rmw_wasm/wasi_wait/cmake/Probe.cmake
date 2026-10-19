# SPDX-License-Identifier: Apache-2.0
include_guard(GLOBAL)
include(CheckCSourceCompiles)
function(uwvm_rmw_probe_wasi_wait output_directory)
 if(NOT IS_ABSOLUTE "${output_directory}")
  message(FATAL_ERROR "Actual generated feature directory must be absolute")
 endif()
 unset(UWVM_RMW_WASI_WAIT_P1_API)
 unset(UWVM_RMW_WASI_WAIT_P1_API CACHE)
 # A toolchain STATIC_LIBRARY shortcut is not a link proof. Function scope
 # restores the caller's original setting without changing its other checks.
 set(CMAKE_C_STANDARD 11)
 set(CMAKE_C_STANDARD_REQUIRED ON)
 set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)
 set(CMAKE_REQUIRED_LIBRARIES "")
 check_c_source_compiles([=[
#if !defined(__wasi__) || !defined(__wasm32__)
#error Genuine original wasm32 WASI Preview1 libc required
#endif
#include <wasi/api.h>
#include <stdint.h>
#include <stddef.h>
static __wasi_errno_t(*volatile real_poll)(__wasi_subscription_t const*,__wasi_event_t*,__wasi_size_t,__wasi_size_t*)=&__wasi_poll_oneoff;
_Static_assert(sizeof(__wasi_timestamp_t)==8&&sizeof(__wasi_userdata_t)==8,"actual nanosecond/userdata types");
_Static_assert(sizeof(__wasi_errno_t)==2,"full original WASI errno");
int main(void){__wasi_subscription_t s={0};__wasi_event_t e={0};__wasi_size_t n=0;
 s.userdata=1;s.u.tag=__WASI_EVENTTYPE_CLOCK;s.u.u.clock.id=__WASI_CLOCKID_MONOTONIC;
 s.u.u.clock.timeout=1;s.u.u.clock.precision=0;s.u.u.clock.flags=0;
 e.userdata=s.userdata;e.type=__WASI_EVENTTYPE_CLOCK;e.error=__WASI_ERRNO_SUCCESS;
 return !real_poll||n||e.type!=s.u.tag;}
]=] UWVM_RMW_WASI_WAIT_P1_API)
 configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uwvm_rmw_wasi_wait_features.h.in"
  "${output_directory}/uwvm_rmw_wasi_wait_features.h" @ONLY)
 set(UWVM_RMW_WASI_WAIT_FEATURE_INCLUDE_DIR "${output_directory}" PARENT_SCOPE)
 set(UWVM_RMW_WASI_WAIT_P1_API "${UWVM_RMW_WASI_WAIT_P1_API}" PARENT_SCOPE)
endfunction()
