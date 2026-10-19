/** @file
 * @brief Allocation-free session ownership and inspectable cleanup for C11/C++.
 *
 * A session is a guest-local lifetime organizer over an explicitly selected
 * backend. It never creates a native node, inherits ambient permissions, reads
 * a clock, retries a timeout, or grants an operation. Supply stable owner
 * storage; the session/context/storage must outlive every borrowed owner.
 */
#ifndef ROSWASMC_SESSION_H
#define ROSWASMC_SESSION_H
#include "roswasmc.h"
/** @brief Explicit local dispatch context, independent of native ROS authority. */
typedef struct roswasmc_local_context {int active;} roswasmc_local_context;
/** @brief Start local dispatch without opening a native node or alias. */
static inline void roswasmc_local_context_init(roswasmc_local_context* context) {if(context) context->active=1;}
/** @brief Stop associated local Session/Executor scheduling, granting/revoking nothing.
 * Existing native owners must still be explicitly closed; shutdown never sends
 * a native lifecycle transition, acquires a thread or cancels an admitted RPC.
 */
static inline void roswasmc_local_context_shutdown(roswasmc_local_context* context) {if(context) context->active=0;}
/** @brief Separate application/setup failure from first cleanup failure.
 * All cleanup attempts run even after an error. Owners are retired locally
 * after one attempt; failure107/110 must never cause an automatic close replay.
 */
typedef struct roswasmc_cleanup_report {
    uint32_t primary_error,cleanup_error;
    size_t attempted,failed;
} roswasmc_cleanup_report;
/** @brief Start an empty report. This performs no imports. */
static inline roswasmc_cleanup_report roswasmc_cleanup_init(uint32_t primary_error) {
    roswasmc_cleanup_report result={primary_error,0,0,0};return result;
}
/** @brief Close one live owner and accumulate its independent cleanup status.
 * Empty owners are skipped. Use roswasmc_close directly to test an empty/stale
 * wrapper's9 result. The return value is the attempted close's fresh ABI code.
 */
static inline uint32_t roswasmc_cleanup_owner(roswasmc_capability* cap,roswasmc_cleanup_report* report) {
    uint32_t error;if(!cap||!cap->handle) return 0;
    error=roswasmc_close(cap);
    if(report) {++report->attempted;if(error) {++report->failed;if(!report->cleanup_error) report->cleanup_error=error;}}
    return error;
}
/** @brief Stable caller-buffer ownership arena; no resource opens during init.
 * Slots returned by session_open stay at fixed addresses until session_close.
 * Do not manually copy, move or overwrite live slots. Individual explicit close
 * is allowed; session cleanup skips its already-retired slot. Reinitialize only
 * after closing the previous session. No destructor exists in C.
 */
typedef struct roswasmc_session {
    roswasmc_context context;
    roswasmc_capability* owners;
    size_t capacity,count;
    roswasmc_local_context const* lifecycle;
} roswasmc_session;
/** @brief Initialize a fresh session over fixed storage, granting no authority.
 * The caller must not call this on a live initialized session. A zero-capacity
 * session is valid and refuses allocation before any host import.
 */
static inline uint32_t roswasmc_session_init(roswasmc_session* session,roswasmc_backend const* backend,
    roswasmc_capability* owners,size_t capacity) {
    if(!session||!backend||(!owners&&capacity)) return UWVM_ROS_INVALID;
    session->context.backend=backend;session->owners=owners;session->capacity=capacity;session->count=0;session->lifecycle=0;return 0;
}
/** @brief Initialize a session associated with explicit controlled local shutdown. */
static inline uint32_t roswasmc_session_init_context(roswasmc_session* session,roswasmc_backend const* backend,
    roswasmc_local_context const* context,roswasmc_capability* owners,size_t capacity) {
    uint32_t error;if(!context) return UWVM_ROS_INVALID;error=roswasmc_session_init(session,backend,owners,capacity);
    if(!error) session->lifecycle=context;return error;
}
/** @brief Check the explicit imported ABI version. Value1 succeeds; no alias opens.
 * A missing version import or a different ABI is unsupported95, not proof of a
 * functioning runtime. Backend.version selects ROS1/ROS2 and is not ABI version.
 */
static inline uint32_t roswasmc_session_check_abi(roswasmc_session const* session) {
    if(!session||!session->context.backend) return UWVM_ROS_INVALID;
    if(session->lifecycle&&!session->lifecycle->active) return UWVM_ROS_REVOKED;
    if(!session->context.backend->abi_version) return UWVM_ROS_UNSUPPORTED;
    return session->context.backend->abi_version()==1?0:UWVM_ROS_UNSUPPORTED;
}
/** @brief Open exactly one alias into the next stable slot, transactionally.
 * On error, count and *owner are unchanged. The returned pointer is borrowed
 * from the session; caller must never free it or transfer it to another backend.
 * Capacity exhaustion returns90 before calling the host and confers no rights.
 */
static inline uint32_t roswasmc_session_open(roswasmc_session* session,char const* alias,size_t length,
    roswasmc_capability** owner) {
    roswasmc_capability* next;uint32_t error;
    if(!session||!owner||!session->context.backend||(!session->owners&&session->capacity)) return UWVM_ROS_INVALID;
    if(session->lifecycle&&!session->lifecycle->active) return UWVM_ROS_REVOKED;
    if(session->count>=session->capacity) return UWVM_ROS_TOO_LARGE;
    next=session->owners+session->count;roswasmc_capability_init(next,&session->context);
    error=roswasmc_open(next,alias,length);if(error) return error;
    ++session->count;*owner=next;return 0;
}
/** @brief Check backend registration before opening a selected request alias.
 * A registered method may still be denied13, revoked125 or unsupported95 by
 * the native adapter. Neither return95 nor return13 is a functional success.
 */
static inline uint32_t roswasmc_session_open_for(roswasmc_session* session,roswasmc_operation operation,
    char const* alias,size_t length,roswasmc_capability** owner) {
    uint32_t error;if(!session) return UWVM_ROS_INVALID;
    if(session->lifecycle&&!session->lifecycle->active) return UWVM_ROS_REVOKED;
    error=roswasmc_check_operation(&session->context,operation);
    return error?error:roswasmc_session_open(session,alias,length,owner);
}
/** @brief Close every live slot in reverse acquisition order, once each.
 * primary_error is retained separately. Optional statuses receives one status
 * per acquisition-order slot (zero for already-retired slots); provide count
 * entries. An undersized statuses buffer refuses before closing any owner.
 * All attempted owners retire even when cleanup fails. count becomes zero.
 */
static inline roswasmc_cleanup_report roswasmc_session_close(roswasmc_session* session,uint32_t primary_error,
    uint32_t* statuses,size_t status_capacity) {
    roswasmc_cleanup_report report=roswasmc_cleanup_init(primary_error);size_t remaining;
    if(!session||!session->context.backend||(!session->owners&&session->capacity)||
        session->count>session->capacity||(statuses&&status_capacity<session->count)) {
        report.cleanup_error=UWVM_ROS_INVALID;return report;
    }
    remaining=session->count;
    while(remaining) {size_t index=--remaining;uint32_t code=roswasmc_cleanup_owner(session->owners+index,&report);if(statuses) statuses[index]=code;}
    session->count=0;return report;
}
#endif
