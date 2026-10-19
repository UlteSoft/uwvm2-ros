/** @file
 * @brief Allocation-free, explicitly granted Fibonacci Action client bundle.
 *
 * A bundle acquires four independent send/cancel-own/feedback/result aliases.
 * Cancel-any/all is a fifth, optional explicit authority. No alias, node, clock,
 * thread, permission or native Action type is inferred. The caller supplies a
 * stable context, zero-initialized bundle and fixed goal-ID storage. Do not copy,
 * move, overwrite or concurrently mutate a live bundle/context/goal table.
 *
 * Calls are synchronous and execute at most once. Errors11/90/107/110 never
 * trigger replay, resize, implicit cancellation or rollback of admitted native
 * effects. An opaque goal ID is local bookkeeping, never native authority.
 * The Runtime independently validates Instance/backend/grant/goal ownership.
 */
#ifndef ROSWASMC_ACTION_BUNDLE_H
#define ROSWASMC_ACTION_BUNDLE_H
#include "codec.h"
#include "session.h"

/** @brief Explicit role aliases in acquisition order; no default values.
 * Four required views are nonempty/distinct. Optional cancel_any is exactly
 * {NULL,0} when absent, otherwise a distinct valid alias. Native resource/type,
 * identity, timeout, goal/queue/byte/rate and channel QoS remain host policy.
 */
typedef struct roswasmc_action_aliases {
    roswasmc_string send,cancel_own,feedback,result,cancel_any;
} roswasmc_action_aliases;

/** @brief Unique logical owner with caller-provided bounded goal storage.
 * cap roles0..4 are send, own-cancel, feedback, result, any-cancel respectively.
 * error is initialization/retired-state status, not a sticky operation error.
 * rollback reports primary/first cleanup failure, counts and every role status.
 * goals[0..count) contains unique nonzero IDs. The remaining storage is unused.
 * Initialize this object to zero before first init; reinitialize only retired
 * bundles. Context/backend and table must outlive all calls and checked cleanup.
 */
typedef struct roswasmc_action_bundle {
    roswasmc_capability cap[5];
    uint64_t* goals;
    size_t capacity,count,roles;
    int active;
    uint32_t error;
    roswasmc_cleanup_report rollback;
    uint32_t rollback_status[5];
} roswasmc_action_bundle;

/** @brief Internal checked ordinary-object range; this does not validate Wasm
 * memory authority. Caller buffers must still be actual live C objects.
 */
static inline int roswasmc_action_bundle_range(void const* pointer,size_t size) {
    return (!size)|| (pointer && size<=UINTPTR_MAX && (uintptr_t)pointer<=UINTPTR_MAX-size);
}
/** @brief Internal range intersection without adding unchecked pointer offsets. */
static inline int roswasmc_action_bundle_overlap(void const* left,size_t left_size,void const* right,size_t right_size) {
    uintptr_t a=(uintptr_t)left,b=(uintptr_t)right;
    if(!left_size||!right_size) return 0;
    if(!roswasmc_action_bundle_range(left,left_size)||!roswasmc_action_bundle_range(right,right_size)) return 1;
    return a<b+right_size&&b<a+left_size;
}
/** @brief Internal policy-alias grammar, consumed completely before any open. */
static inline int roswasmc_action_bundle_alias(roswasmc_string alias) {
    size_t i;if(!alias.data||!alias.size||alias.size>255||!roswasmc_action_bundle_range(alias.data,alias.size)) return 0;
    if((alias.size==1&&alias.data[0]=='.')||(alias.size==2&&alias.data[0]=='.'&&alias.data[1]=='.')) return 0;
    for(i=0;i<alias.size;++i) {
        unsigned char c=(unsigned char)alias.data[i];
        if(!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9')||c=='_'||c=='.'||c=='-')) return 0;
    }return 1;
}
/** @brief Internal exact alias equality; five bounded views require no allocation. */
static inline int roswasmc_action_bundle_same_alias(roswasmc_string left,roswasmc_string right) {
    size_t i;if(left.size!=right.size) return 0;
    for(i=0;i<left.size;++i) if(left.data[i]!=right.data[i]) return 0;return 1;
}
/** @brief Internal request result for a local refusal, with no fabricated length. */
static inline roswasmc_result roswasmc_action_bundle_failure(uint32_t error) {
    roswasmc_result result={error,0,0};return result;
}
/** @brief Check a live initialized group before touching a native operation. */
static inline uint32_t roswasmc_action_bundle_check(roswasmc_action_bundle const* bundle) {
    if(!bundle||!bundle->active) return UWVM_ROS_BAD_HANDLE;
    if(bundle->error) return bundle->error;
    if(!bundle->goals||!bundle->capacity||bundle->capacity>1024u||bundle->count>bundle->capacity||
       (bundle->roles!=4&&bundle->roles!=5)) return UWVM_ROS_INVALID;
    return 0;
}
/** @brief Inspect tracked ownership locally, without importing or granting rights. */
static inline int roswasmc_action_bundle_owns(roswasmc_action_bundle const* bundle,uint64_t goal) {
    size_t i;if(!goal||roswasmc_action_bundle_check(bundle)) return 0;
    for(i=0;i<bundle->count;++i) if(bundle->goals[i]==goal) return 1;return 0;
}
/** @brief Internal destination validation: caller outputs must not overwrite
 * the bundle, stable context/backend or goal table. Zero-length raw output is
 * allowed; the host can report90 and an observed required length.
 */
static inline int roswasmc_action_bundle_destination(roswasmc_action_bundle const* bundle,void const* output,size_t size) {
    roswasmc_context const* context=bundle->cap[0].context;
    return roswasmc_action_bundle_range(output,size)&&
        !roswasmc_action_bundle_overlap(output,size,bundle,sizeof(*bundle))&&
        !roswasmc_action_bundle_overlap(output,size,bundle->goals,bundle->capacity*sizeof(uint64_t))&&
        (!context||(!roswasmc_action_bundle_overlap(output,size,context,sizeof(*context))&&
         !roswasmc_action_bundle_overlap(output,size,context->backend,sizeof(*context->backend))));
}

/** @brief Validate all roles/aliases/storage before opening any capability.
 * Required operation registrations and ABI1 are checked up front. Capacity is
 * explicit1..1024 goal slots (at most8KiB), a local bound that cannot increase
 * host quotas. Linear membership scans are bounded by this caller-selected limit.
 * Invalid input/support errors open nothing. A failed later open closes every
 * successful earlier owner in reverse order and records all rollback statuses.
 * Successful init opens4 or5 aliases; it does not send/advertise a goal. A live
 * bundle cannot be reinitialized: Conflict17 preserves its existing ownership.
 */
static inline uint32_t roswasmc_action_bundle_init(roswasmc_context const* context,
    roswasmc_action_bundle* bundle,roswasmc_action_aliases const* aliases,uint64_t* goals,size_t capacity) {
    roswasmc_string views[5];roswasmc_operation const operations[5]={ROSWASMC_ACTION_SEND_GOAL,
        ROSWASMC_ACTION_CANCEL_OWN_GOAL,ROSWASMC_ACTION_RECEIVE_FEEDBACK,ROSWASMC_ACTION_RECEIVE_RESULT,
        ROSWASMC_ACTION_CANCEL_ANY_GOAL};size_t i,j,roles=4;uint32_t error=0;
    if(!bundle) return UWVM_ROS_INVALID;if(bundle->active) return UWVM_ROS_CONFLICT;
    if(!context||!context->backend||!aliases||
       roswasmc_action_bundle_overlap(context,sizeof(*context),bundle,sizeof(*bundle))||
       roswasmc_action_bundle_overlap(context->backend,sizeof(*context->backend),bundle,sizeof(*bundle))||
       roswasmc_action_bundle_overlap(aliases,sizeof(*aliases),bundle,sizeof(*bundle))||
       !goals||!capacity||capacity>1024u||
       capacity>SIZE_MAX/sizeof(uint64_t)||!roswasmc_action_bundle_range(goals,capacity*sizeof(uint64_t))||
       roswasmc_action_bundle_overlap(goals,capacity*sizeof(uint64_t),bundle,sizeof(*bundle))||
       roswasmc_action_bundle_overlap(goals,capacity*sizeof(uint64_t),context,sizeof(*context))||
       roswasmc_action_bundle_overlap(goals,capacity*sizeof(uint64_t),context->backend,sizeof(*context->backend))) error=UWVM_ROS_INVALID;
    if(!error) {
        views[0]=aliases->send;views[1]=aliases->cancel_own;views[2]=aliases->feedback;views[3]=aliases->result;views[4]=aliases->cancel_any;
        if(views[4].data||views[4].size) roles=5;
        for(i=0;i<roles&&!error;++i) {
            if(!roswasmc_action_bundle_alias(views[i])||
               roswasmc_action_bundle_overlap(views[i].data,views[i].size,bundle,sizeof(*bundle))) error=UWVM_ROS_INVALID;
            for(j=0;j<i&&!error;++j) if(roswasmc_action_bundle_same_alias(views[i],views[j])) error=UWVM_ROS_INVALID;
        }
    }
    if(!error) {
        if((context->backend->version!=1&&context->backend->version!=2)||!context->backend->open||
           !context->backend->close||!context->backend->abi_version) error=UWVM_ROS_UNSUPPORTED;
        for(i=0;i<roles&&!error;++i) error=roswasmc_check_operation(context,operations[i]);
        if(!error&&context->backend->abi_version()!=1) error=UWVM_ROS_UNSUPPORTED;
    }
    bundle->goals=goals;bundle->capacity=capacity;bundle->count=0;bundle->roles=roles;bundle->active=0;bundle->error=error;
    bundle->rollback=roswasmc_cleanup_init(error);
    for(i=0;i<5;++i) {roswasmc_capability_init(bundle->cap+i,context);bundle->rollback_status[i]=0;}
    if(error) return error;
    for(i=0;i<roles;++i) {
        error=roswasmc_open(bundle->cap+i,views[i].data,views[i].size);
        if(!error) for(j=0;j<i;++j) if(bundle->cap[j].handle==bundle->cap[i].handle) {
            /* A malformed duplicate reply is not a second independent owner.
             * Retire its duplicate slot; rollback the original native ID once. */
            bundle->cap[i].handle=0;error=UWVM_ROS_CONFLICT;break;
        }
        if(error) break;
    }
    if(error) {
        bundle->error=error;bundle->rollback.primary_error=error;
        for(i=roles;i>0;--i) bundle->rollback_status[i-1]=roswasmc_cleanup_owner(bundle->cap+i-1,&bundle->rollback);
        return error;
    }
    bundle->active=1;return 0;
}

/** @brief Send one bounded Fibonacci order and transactionally track its returned ID.
 * A full local table returns90 before native dispatch. Success requires one
 * nonzero8-byte ID not already tracked; duplicate reply IDs are17. Caller goal
 * output/table/count are unchanged on any failure, including admitted timeouts.
 * The result preserves the import's written/unwritten length distinction.
 */
static inline roswasmc_result roswasmc_action_bundle_send_goal(roswasmc_action_bundle* bundle,
    int32_t order,uint64_t* goal) {
    unsigned char request[4],response[8];size_t length;uint64_t next;uint32_t error=roswasmc_action_bundle_check(bundle);roswasmc_result result;
    if(error) return roswasmc_action_bundle_failure(error);
    if(!goal||!roswasmc_action_bundle_destination(bundle,goal,sizeof(*goal))||order<0||order>46) return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
    if(bundle->count==bundle->capacity) return roswasmc_action_bundle_failure(UWVM_ROS_TOO_LARGE);
    error=roswasmc_encode_fibonacci_order(order,request,sizeof(request),&length);if(error) return roswasmc_action_bundle_failure(error);
    result=roswasmc_action_send_goal(bundle->cap,request,length,response,sizeof(response));if(result.error) return result;
    if(!result.has_length||result.length!=8||(next=uwvm_ros_load_u64(response))==0) {result.error=UWVM_ROS_INVALID;return result;}
    if(roswasmc_action_bundle_owns(bundle,next)) {result.error=UWVM_ROS_CONFLICT;return result;}
    bundle->goals[bundle->count++]=next;*goal=next;return result;
}
/** @brief Internal cancellation with a checked canonical Bool output.
 * Cancellation never removes local goals: terminal result reception does that.
 */
static inline roswasmc_result roswasmc_action_bundle_cancel_impl(roswasmc_action_bundle const* bundle,
    size_t role,roswasmc_operation operation,uint64_t goal,int* accepted) {
    unsigned char request[8],response[1];size_t length;int next;roswasmc_result result;uint32_t error;
    if(!accepted||!roswasmc_action_bundle_destination(bundle,accepted,sizeof(*accepted))) return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
    error=roswasmc_encode_u64(goal,request,sizeof(request),&length);if(error) return roswasmc_action_bundle_failure(error);
    result=roswasmc_request(bundle->cap+role,operation,request,length,response,sizeof(response));if(result.error) return result;
    if(!result.has_length||result.length!=1||roswasmc_decode_bool(response,1,&next)) {result.error=UWVM_ROS_INVALID;return result;}
    *accepted=next;return result;
}
/** @brief Cancel only a locally tracked goal using the independent own-cancel alias.
 * Unknown/stale goals return9 before request; errors11/110 keep ownership.
 */
static inline roswasmc_result roswasmc_action_bundle_cancel_own(roswasmc_action_bundle const* bundle,
    uint64_t goal,int* accepted) {
    uint32_t error=roswasmc_action_bundle_check(bundle);if(error) return roswasmc_action_bundle_failure(error);
    if(!roswasmc_action_bundle_owns(bundle,goal)) return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);
    return roswasmc_action_bundle_cancel_impl(bundle,1,ROSWASMC_ACTION_CANCEL_OWN_GOAL,goal,accepted);
}
/** @brief Cancel one nonzero selector only with the explicit fifth alias.
 * Absence returns13 before validating caller outputs or native dispatch. A zero
 * selector is22: cancel_all is the separate, intentional all-goals operation.
 * This stronger authority may target a goal outside the local ownership table;
 * the Runtime still validates whether that exact goal exists and policy permits
 * cancellation. No local goal is removed, even on an accepted native response.
 */
static inline roswasmc_result roswasmc_action_bundle_cancel_any(roswasmc_action_bundle const* bundle,
    uint64_t goal,int* accepted) {
    uint32_t error=roswasmc_action_bundle_check(bundle);if(error) return roswasmc_action_bundle_failure(error);
    if(bundle->roles!=5) return roswasmc_action_bundle_failure(UWVM_ROS_DENIED);
    if(!goal) return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
    return roswasmc_action_bundle_cancel_impl(bundle,4,ROSWASMC_ACTION_CANCEL_ANY_GOAL,goal,accepted);
}
/** @brief Cancel all only with an explicitly acquired fifth cancel-any alias.
 * Absence returns13 before native request; no optional alias is invented. This
 * stronger operation is destructive and may affect other owners according to
 * host policy. Successful cancellation still leaves local IDs until results.
 */
static inline roswasmc_result roswasmc_action_bundle_cancel_all(roswasmc_action_bundle const* bundle,int* accepted) {
    uint32_t error=roswasmc_action_bundle_check(bundle);if(error) return roswasmc_action_bundle_failure(error);
    if(bundle->roles!=5) return roswasmc_action_bundle_failure(UWVM_ROS_DENIED);
    return roswasmc_action_bundle_cancel_impl(bundle,4,ROSWASMC_ACTION_CANCEL_ANY_GOAL,0,accepted);
}
/** @brief Internal copied-wire receive with exact ID and terminal-kind checks.
 * sequence is assigned only after complete validation and borrows output bytes.
 * Output storage must not overlap sequence, the bundle, context/backend or IDs.
 */
static inline roswasmc_result roswasmc_action_bundle_receive(roswasmc_action_bundle const* bundle,
    uint64_t goal,int terminal,void* bytes,size_t capacity,roswasmc_action_sequence* sequence) {
    unsigned char request[8];size_t length;uint32_t error=roswasmc_action_bundle_check(bundle);roswasmc_result result;roswasmc_action_sequence next;
    if(error) return roswasmc_action_bundle_failure(error);
    if(!roswasmc_action_bundle_owns(bundle,goal)) return roswasmc_action_bundle_failure(UWVM_ROS_BAD_HANDLE);
    if(!sequence||capacity>4193786u||!roswasmc_action_bundle_destination(bundle,bytes,capacity)||
       !roswasmc_action_bundle_destination(bundle,sequence,sizeof(*sequence))||
       roswasmc_action_bundle_overlap(bytes,capacity,sequence,sizeof(*sequence))) return roswasmc_action_bundle_failure(UWVM_ROS_INVALID);
    error=roswasmc_encode_u64(goal,request,sizeof(request),&length);if(error) return roswasmc_action_bundle_failure(error);
    result=roswasmc_request(bundle->cap+(terminal?3:2),terminal?ROSWASMC_ACTION_RECEIVE_RESULT:ROSWASMC_ACTION_RECEIVE_FEEDBACK,request,length,bytes,capacity);
    if(result.error) return result;
    if(!result.has_length||result.length>capacity||roswasmc_decode_action_sequence(bytes,(size_t)result.length,terminal,&next)||
       next.goal!=goal||(terminal&&(next.kind<2||next.kind>4))) {result.error=UWVM_ROS_INVALID;return result;}
    *sequence=next;return result;
}
/** @brief Receive one feedback envelope for an owned ID, retaining all goal state.
 * Again11/timeout110 and error90 length observations are not replayed. A native
 * event may already be consumed on90; no resize/retry or implicit polling loop.
 */
static inline roswasmc_result roswasmc_action_bundle_feedback(roswasmc_action_bundle const* bundle,
    uint64_t goal,void* bytes,size_t capacity,roswasmc_action_sequence* sequence) {
    return roswasmc_action_bundle_receive(bundle,goal,0,bytes,capacity,sequence);
}
/** @brief Receive one successful terminal result and then retire that local ID.
 * Malformed/mismatched/idle/error replies preserve the table and caller sequence.
 * A successful terminal2..4 removes by swap, so ID-table order is unspecified.
 */
static inline roswasmc_result roswasmc_action_bundle_result(roswasmc_action_bundle* bundle,
    uint64_t goal,void* bytes,size_t capacity,roswasmc_action_sequence* sequence) {
    roswasmc_result result=roswasmc_action_bundle_receive(bundle,goal,1,bytes,capacity,sequence);size_t i;
    if(!result.error) for(i=0;i<bundle->count;++i) if(bundle->goals[i]==goal) {
        --bundle->count;bundle->goals[i]=bundle->goals[bundle->count];bundle->goals[bundle->count]=0;break;
    }return result;
}
/** @brief Internal checked reverse cleanup; initialized storage cannot be copied.
 * Optional statuses needs roles entries and is filled in acquisition order.
 * An undersized/overlapping destination refuses before any close or mutation.
 */
static inline roswasmc_cleanup_report roswasmc_action_bundle_cleanup(roswasmc_action_bundle* bundle,
    uint32_t primary,uint32_t* statuses,size_t status_capacity,int revoke) {
    roswasmc_cleanup_report report=roswasmc_cleanup_init(primary);size_t i;uint32_t code;
    if(!bundle) {report.cleanup_error=UWVM_ROS_INVALID;return report;}
    if(!bundle->active) {report.cleanup_error=UWVM_ROS_BAD_HANDLE;return report;}
    code=roswasmc_action_bundle_check(bundle);
    if(code||(statuses&&(status_capacity<bundle->roles||!roswasmc_action_bundle_destination(bundle,statuses,bundle->roles*sizeof(*statuses))))) {
        report.cleanup_error=code?code:(uint32_t)UWVM_ROS_INVALID;return report;
    }
    bundle->active=0;bundle->error=UWVM_ROS_BAD_HANDLE;
    for(i=0;i<bundle->count;++i) bundle->goals[i]=0;bundle->count=0;
    for(i=bundle->roles;i>0;--i) {
        code=0;
        if(bundle->cap[i-1].handle) {
            code=revoke?roswasmc_revoke(bundle->cap+i-1):roswasmc_close(bundle->cap+i-1);
            ++report.attempted;if(code) {++report.failed;if(!report.cleanup_error) report.cleanup_error=code;}
        }
        if(statuses) statuses[i-1]=code;
    }return report;
}
/** @brief Close all live roles in reverse once, preserving primary and all cleanup
 * statuses. Goals are cleared even after107/110. Closing does not cancel native
 * work or revoke an alias; repeated cleanup returns9 without any replay.
 */
static inline roswasmc_cleanup_report roswasmc_action_bundle_close_checked(roswasmc_action_bundle* bundle,
    uint32_t primary,uint32_t* statuses,size_t status_capacity) {
    return roswasmc_action_bundle_cleanup(bundle,primary,statuses,status_capacity,0);
}
/** @brief Explicit destructive alias revocation of every live role in reverse.
 * This can invalidate sibling handles in host policy; it is never automatic
 * rollback/Drop/timeout recovery. Each local owner is retired once even on
 * notification failure. Goals clear; reopening revoked aliases remains125.
 */
static inline roswasmc_cleanup_report roswasmc_action_bundle_revoke_checked(roswasmc_action_bundle* bundle,
    uint32_t primary,uint32_t* statuses,size_t status_capacity) {
    return roswasmc_action_bundle_cleanup(bundle,primary,statuses,status_capacity,1);
}
#endif
