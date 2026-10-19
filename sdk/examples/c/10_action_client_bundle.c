/** @file
 * @brief Runnable explicit four-role Fibonacci Action client and owned goal flow.
 * Required aliases: goal-send, goal-cancel-own, goal-feedback, goal-result.
 * Optional ROSWASM_EXAMPLE_CANCEL_ANY=1 adds only explicit goal-cancel-any.
 * CASE0 sends then receives one feedback/result; CASE1 cancels own then reads
 * one result; CASE2 requests stronger cancel-all (13 if not explicitly enabled);
 * CASE3 defers send then reads feedback/result; CASE4 sends then explicitly
 * cancels its nonzero selector through the stronger fifth role and reads result.
 * No default alias exists; default-empty Runtime returns13. Each request runs
 * once. This one-shot main reports pending11 then performs checked cleanup.
 * A long-lived application must retain its bundle and externally drive bounded
 * receives before closing; send/cancel are never automatically replayed.
 * REVOKE=1 deliberately revokes every acquired role instead of ordinary close.
 * See ADVANCED.md; a native matching Fibonacci server and full role policies
 * are external prerequisites, never inferred from this application compiling.
 */
#include "example_support.h"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#ifndef ROSWASM_EXAMPLE_CANCEL_ANY
#define ROSWASM_EXAMPLE_CANCEL_ANY 0
#endif
#ifndef ROSWASM_EXAMPLE_REVOKE
#define ROSWASM_EXAMPLE_REVOKE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 4
#error "ROSWASM_EXAMPLE_CASE must be 0..4"
#endif
typedef struct send_task {roswasmc_action_bundle* bundle;uint64_t goal;} send_task;
/** @brief Deferred work uses the same bounded synchronous send exactly once. */
static uint32_t send_once(void* opaque) {send_task* task=(send_task*)opaque;return roswasmc_action_bundle_send_goal(task->bundle,4,&task->goal).error;}
int main(void) {
    roswasmc_node node;roswasmc_action_bundle bundle={0};uint64_t goals[1],goal=0;unsigned char bytes[512];
    roswasmc_action_aliases aliases={{"goal-send",9},{"goal-cancel-own",15},{"goal-feedback",13},{"goal-result",11},{0,0}};
    uint32_t error,statuses[5]={0};roswasmc_result result;roswasmc_action_sequence sequence;int accepted=0;
    error=example_node(&node);if(error) return (int)error;
#if ROSWASM_EXAMPLE_CANCEL_ANY
    {roswasmc_string any={"goal-cancel-any",15};aliases.cancel_any=any;}
#endif
    error=roswasmc_node_create_action_bundle(&node,&bundle,&aliases,goals,1);
#if ROSWASM_EXAMPLE_CASE == 2
    if(!error) {result=roswasmc_action_bundle_cancel_all(&bundle,&accepted);error=result.error;if(!error&&!accepted) error=UWVM_ROS_CONFLICT;}
#else
    if(!error) {
#if ROSWASM_EXAMPLE_CASE == 3
        roswasmc_local_context context;roswasmc_executor executor={0};roswasmc_deferred_task slot[1];roswasmc_future future;send_task task={&bundle,0};size_t completed=0;
        roswasmc_local_context_init(&context);roswasmc_future_init(&future);
        error=roswasmc_executor_init(&executor,&context,slot,1);
        if(!error) error=roswasmc_executor_post(&executor,&future,send_once,&task,0,0);
        if(!error) error=roswasmc_executor_spin(&executor,&completed);
        if(!error&&completed!=1) error=UWVM_ROS_INVALID;
        if(!error) error=roswasmc_future_take(&future);if(!error) goal=task.goal;
        roswasmc_executor_cancel_pending(&executor);roswasmc_local_context_shutdown(&context);
#else
        result=roswasmc_action_bundle_send_goal(&bundle,4,&goal);error=result.error;
#endif
    }
    if(!error&&!roswasmc_action_bundle_owns(&bundle,goal)) error=UWVM_ROS_BAD_HANDLE;
    if(!error) {
#if ROSWASM_EXAMPLE_CASE == 1
        result=roswasmc_action_bundle_cancel_own(&bundle,goal,&accepted);error=result.error;if(!error&&!accepted) error=UWVM_ROS_CONFLICT;
#elif ROSWASM_EXAMPLE_CASE == 4
        result=roswasmc_action_bundle_cancel_any(&bundle,goal,&accepted);error=result.error;if(!error&&!accepted) error=UWVM_ROS_CONFLICT;
#else
        result=roswasmc_action_bundle_feedback(&bundle,goal,bytes,sizeof bytes,&sequence);error=result.error;
#endif
    }
    if(!error) {result=roswasmc_action_bundle_result(&bundle,goal,bytes,sizeof bytes,&sequence);error=result.error;}
    if(!error&&roswasmc_action_bundle_owns(&bundle,goal)) error=UWVM_ROS_CONFLICT;
#endif
    {roswasmc_cleanup_report cleanup=bundle.active?
        (ROSWASM_EXAMPLE_REVOKE?roswasmc_action_bundle_revoke_checked(&bundle,error,statuses,5):roswasmc_action_bundle_close_checked(&bundle,error,statuses,5)):
        bundle.rollback;
     return (int)(error?error:cleanup.cleanup_error);}
}
