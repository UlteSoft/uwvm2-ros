/* SPDX-License-Identifier: Apache-2.0 */
#ifndef UWVM_RMW_C_INTERNAL_H
#define UWVM_RMW_C_INTERNAL_H
#include <uwvm_rmw_c/provider.h>
#include <uwvm_rmw_platform.h>
#include <rmw/error_handling.h>
#include <stdatomic.h>
#include <string.h>
#define UWVM_RMW_C_GUARD_LIMIT (UWVM_RMW_MAX_CONTEXTS*UWVM_RMW_MAX_OWNERS)
#define UWVM_RMW_C_WAIT_LIMIT (UWVM_RMW_MAX_CONTEXTS*32u)
typedef struct uwvm_rmw_c_guard {
 rmw_guard_condition_t public_handle;
 uwvm_rmw_context*context;uint64_t context_instance;rcutils_allocator_t allocator;
 _Atomic unsigned triggered;int live;
#if UWVM_RMW_ENABLE_THREADED_PROFILE
 size_t pins; /* registry lock protects actual short trigger + wait borrowers */
 int wait_borrowed;
#endif
} uwvm_rmw_c_guard;
uwvm_rmw_c_profile const*uwvm_rmw_c_configuration(void);
void uwvm_rmw_c_mark_started(void);
uint32_t uwvm_rmw_c_endpoint_valid(uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*,size_t qos_count);
uint32_t uwvm_rmw_c_endpoint_inspect(uwvm_rmw_c_native_endpoint const*,roswasmc_capability const*,roswasmc_capability const*);
uwvm_rmw_c_guard*uwvm_rmw_c_guard_find(void const*);
uint32_t uwvm_rmw_c_guard_ready(uwvm_rmw_c_guard*,int consume);
/* Readiness may consume one transport record into an already owned canonical
 * queue. It never decodes into application storage or invokes an upper callback.
 * The corresponding take later makes the complete owned copy exactly once. */
uint32_t uwvm_rmw_c_subscription_ready(uwvm_rmw_resource*,int*);
uint32_t uwvm_rmw_c_client_ready(uwvm_rmw_resource*,int*);
uint32_t uwvm_rmw_c_service_ready(uwvm_rmw_resource*,int*);
#if UWVM_RMW_ENABLE_THREADED_PROFILE
uint32_t uwvm_rmw_c_guard_pin(uwvm_rmw_c_guard*);
void uwvm_rmw_c_guard_unpin(uwvm_rmw_c_guard*);
void uwvm_rmw_c_guard_drain(uwvm_rmw_context*);
void uwvm_rmw_c_wait_drain(uwvm_rmw_context*);
#endif
#endif
