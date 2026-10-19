/** @file
 * @brief Runnable Node-style four-role Fibonacci client and local Future flow.
 * CASE0 send/feedback/result; CASE1 send/cancel-own/result; CASE2 explicit
 * cancel-any/all; CASE3 deferred send then feedback/result; CASE4 sends then
 * cancels a nonzero selector through the independent any-role and reads result.
 * CANCEL_ANY=1 explicitly supplies the stronger fifth alias; without it CASE2
 * returns13 before cancel dispatch. REVOKE=1 explicitly revokes the role group.
 * Every grant/QoS/timeout/native prerequisite is listed in ../c/ADVANCED.md.
 * This one-shot main reports pending11 then checks cleanup. A retained app must
 * keep the bundle alive and externally drive bounded receives before closing;
 * no send/cancel retry, implicit clock or sleep is provided.
 */
#include "example_support.hpp"
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
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Context context{*ROSWASM_BACKEND()};roswasmcpp::Node node{context,"cpp-action-bundle"};
    roswasmcpp::ActionClientAliases aliases{"goal-send","goal-cancel-own","goal-feedback","goal-result"};
#if ROSWASM_EXAMPLE_CANCEL_ANY
    aliases.cancel_any="goal-cancel-any";
#endif
    auto client=node.create_action_client(aliases,1,512);error=client.error();if(error) return static_cast<int>(error);
    uint64_t goal=0;bool accepted=false;roswasmcpp::wire::action_sequence sequence;
#if ROSWASM_EXAMPLE_CASE == 2
    error=client.cancel_all(accepted);if(!error&&!accepted) error=UWVM_ROS_CONFLICT;
#else
#if ROSWASM_EXAMPLE_CASE == 3
    roswasmcpp::Executor executor{context,1};auto future=client.async_send_goal(executor,4);size_t delivered=0;
    error=executor.spin_some(0,delivered);if(!error) error=future.take(goal);executor.cancel_pending();
#else
    error=client.send_goal(4,goal);
#endif
    if(!error&&!client.owns_goal(goal)) error=UWVM_ROS_BAD_HANDLE;
    if(!error) {
#if ROSWASM_EXAMPLE_CASE == 1
        error=client.cancel_own(goal,accepted);if(!error&&!accepted) error=UWVM_ROS_CONFLICT;
#elif ROSWASM_EXAMPLE_CASE == 4
        error=client.cancel_any(goal,accepted);if(!error&&!accepted) error=UWVM_ROS_CONFLICT;
#else
        error=client.receive_feedback(goal,sequence);
#endif
    }
    if(!error) error=client.receive_result(goal,sequence);
    if(!error&&client.owns_goal(goal)) error=UWVM_ROS_CONFLICT;
#endif
    uint32_t statuses[5]{};auto cleanup=ROSWASM_EXAMPLE_REVOKE?client.revoke_checked(statuses,error):client.close_checked(statuses,error);
    context.shutdown();return static_cast<int>(error?error:cleanup.cleanup_error);
}
