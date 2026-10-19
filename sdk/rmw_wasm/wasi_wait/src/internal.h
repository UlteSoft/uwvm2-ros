/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_RMW_WASI_WAIT_INTERNAL_H
#define UWVM_RMW_WASI_WAIT_INTERNAL_H
#if __has_include("uwvm_rmw_wasi_wait_features.h")
#include "uwvm_rmw_wasi_wait_features.h"
#endif
#ifndef UWVM_RMW_WASI_WAIT_P1_API
#define UWVM_RMW_WASI_WAIT_P1_API 0
#endif
#if UWVM_RMW_WASI_WAIT_P1_API
/* Only the ACTUAL selected original header may define these ABI types/imports.
 * There is no native handwritten ABI, weak Host symbol or injected syscall. */
#include <wasi/api.h>
#include <stdint.h>
static inline uint32_t uwvm_rmw_wasi_wait_errno(__wasi_errno_t error){
 switch(error){
 case __WASI_ERRNO_SUCCESS:return 0;
 case __WASI_ERRNO_ACCES:case __WASI_ERRNO_PERM:case __WASI_ERRNO_NOTCAPABLE:return 13;
 case __WASI_ERRNO_INVAL:return 22;case __WASI_ERRNO_NOMEM:return 12;
 case __WASI_ERRNO_NOSYS:case __WASI_ERRNO_NOTSUP:return 95;
 case __WASI_ERRNO_INTR:return 4;case __WASI_ERRNO_CANCELED:return 125;
 case __WASI_ERRNO_TIMEDOUT:return 110;case __WASI_ERRNO_OVERFLOW:return 90;
 default:return 5;
 }
}
/* CLOCK events explicitly IGNORE fd_readwrite and padding in the original ABI.
 * Inspect only defined fields. A pure DATA contract uses this same function,
 * never a fake Host callback. Nonzero full WASI errno can never become0. */
static inline uint32_t uwvm_rmw_wasi_wait_validate(__wasi_errno_t call_errno,
 __wasi_size_t count,__wasi_event_t const*event,__wasi_userdata_t expected){
 if(call_errno)return uwvm_rmw_wasi_wait_errno(call_errno);
 if(count!=1||!event||!expected||event->type!=__WASI_EVENTTYPE_CLOCK||event->userdata!=expected)return 22;
 return uwvm_rmw_wasi_wait_errno(event->error);
}
#endif
#endif
