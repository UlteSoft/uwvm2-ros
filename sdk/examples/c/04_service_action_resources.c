/** @file
 * @brief Runnable typed C services/Fibonacci client application.
 * Compile ROSWASM_EXAMPLE_CASE=0 Trigger,1 SetBool,2 send-goal,3 send+feedback,
 *4 send+result,5 send+cancel-own,6 cancel-any(all). Default0 performs a real
 * service.call attempt; default-empty Runtime denial13 remains a failure.
 * See ADVANCED.md for aliases, complete policies and native prerequisites.
 * Every bounded RPC runs once.11 is pending;110 permits no automatic replay.
 * Keep the sender alive while using its goal ID through separately granted roles.
 */
#include "example_support.h"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 6
#error "ROSWASM_EXAMPLE_CASE must be 0..6"
#endif
/** @brief Reusable bounded Trigger call; response borrows the caller's buffer. */
ROSWASM_EXAMPLE_EXPORT("trigger_once")
uint32_t trigger_once(roswasmc_capability const* client,roswasmc_service_response* response,unsigned char bytes[512]) {
    return roswasmc_trigger_call(client,bytes,512,response);
}
/** @brief Acquire only selected roles, execute once, and inspect every close. */
int main(void) {
    roswasmc_node node;roswasmc_capability first,second;unsigned char bytes[512];uint32_t error;
    roswasmc_service_response response;roswasmc_action_sequence sequence;uint64_t goal=0;int accepted=0;
    error=example_node(&node);if(error) return (int)error;
    roswasmc_capability_init(&first,&node.context);roswasmc_capability_init(&second,&node.context);
#if ROSWASM_EXAMPLE_CASE == 0 || ROSWASM_EXAMPLE_CASE == 1
    {roswasmc_string alias={ROSWASM_EXAMPLE_CASE==0?"trigger-call":"set-bool-call",ROSWASM_EXAMPLE_CASE==0?12:13};
     error=roswasmc_node_create_client(&node,&first,alias);}
    if(!error) error=ROSWASM_EXAMPLE_CASE==0?trigger_once(&first,&response,bytes):roswasmc_set_bool_call(&first,1,bytes,sizeof bytes,&response);
#else
#if ROSWASM_EXAMPLE_CASE != 6
    {roswasmc_string alias={"goal-send",9};error=roswasmc_node_create_action_client(&node,&first,alias,ROSWASMC_ACTION_SEND_GOAL);}
    if(!error) error=roswasmc_fibonacci_send_goal(&first,4,&goal);
#else
    error=0;
#endif
#if ROSWASM_EXAMPLE_CASE != 2
    if(!error) {
        roswasmc_operation operation=ROSWASM_EXAMPLE_CASE==3?ROSWASMC_ACTION_RECEIVE_FEEDBACK:ROSWASM_EXAMPLE_CASE==4?ROSWASMC_ACTION_RECEIVE_RESULT:
            ROSWASM_EXAMPLE_CASE==5?ROSWASMC_ACTION_CANCEL_OWN_GOAL:ROSWASMC_ACTION_CANCEL_ANY_GOAL;
        roswasmc_string alias={ROSWASM_EXAMPLE_CASE==3?"goal-feedback":ROSWASM_EXAMPLE_CASE==4?"goal-result":ROSWASM_EXAMPLE_CASE==5?"goal-cancel-own":"goal-cancel-any",
            ROSWASM_EXAMPLE_CASE==3?13:ROSWASM_EXAMPLE_CASE==4?11:15};
        error=roswasmc_node_create_action_client(&node,&second,alias,operation);
        if(!error) error=ROSWASM_EXAMPLE_CASE<=4?roswasmc_action_receive(&second,operation,goal,bytes,sizeof bytes,&sequence):
            roswasmc_action_cancel(&second,operation,goal,&accepted);
    }
#endif
#endif
    return (int)example_finish_pair(&first,&second,error);
}
