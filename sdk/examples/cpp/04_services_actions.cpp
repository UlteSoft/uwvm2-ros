/** @file
 * @brief Runnable typed C++ Trigger/SetBool/Fibonacci client application.
 * Compile ROSWASM_EXAMPLE_CASE=0 Trigger,1 SetBool,2 send,3 send+feedback,
 *4 send+result,5 send+cancel-own,6 cancel-any(all). Every case performs its
 * actual Node factory and one bounded operation. Default denial13 and pending11
 * remain numeric failure/idle results, never successful delivery assertions.
 * Host policy fixes type/deadline/QoS/quotas; see ../c/ADVANCED.md.
 */
#include "example_support.hpp"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 6
#error "ROSWASM_EXAMPLE_CASE must be 0..6"
#endif
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Context context{*ROSWASM_BACKEND()};roswasmcpp::Node node{context,"cpp-client"};
#if ROSWASM_EXAMPLE_CASE == 0
    auto client=node.create_client<roswasmcpp::srv::Trigger>("trigger-call",512);error=client.error();
    roswasmcpp::srv::Trigger::Response response{};if(!error) error=client.call({},response);
    return static_cast<int>(example_finish(client,error));
#elif ROSWASM_EXAMPLE_CASE == 1
    auto client=node.create_client<roswasmcpp::srv::SetBool>("set-bool-call",512);error=client.error();
    roswasmcpp::srv::SetBool::Response response{};if(!error) error=client.call({true},response);
    return static_cast<int>(example_finish(client,error));
#elif ROSWASM_EXAMPLE_CASE == 6
    auto cancel=node.create_action_client("goal-cancel-any",ROSWASMC_ACTION_CANCEL_ANY_GOAL,1);error=cancel.error();bool accepted{};
    if(!error) error=cancel.cancel_all(accepted);return static_cast<int>(example_finish(cancel,error));
#else
    auto sender=node.create_action_client("goal-send",ROSWASMC_ACTION_SEND_GOAL,8);error=sender.error();uint64_t goal{};
    if(!error) error=sender.send_goal(4,goal);
#if ROSWASM_EXAMPLE_CASE != 2
    if(!error) {
        constexpr auto operation=ROSWASM_EXAMPLE_CASE==3?ROSWASMC_ACTION_RECEIVE_FEEDBACK:ROSWASM_EXAMPLE_CASE==4?ROSWASMC_ACTION_RECEIVE_RESULT:ROSWASMC_ACTION_CANCEL_OWN_GOAL;
        auto reader=node.create_action_client(ROSWASM_EXAMPLE_CASE==3?"goal-feedback":ROSWASM_EXAMPLE_CASE==4?"goal-result":"goal-cancel-own",operation,512);
        error=reader.error();roswasmcpp::wire::action_sequence sequence{};bool accepted{};
        if(!error) {if constexpr(ROSWASM_EXAMPLE_CASE==5) error=reader.cancel(goal,accepted);else error=reader.receive(goal,sequence);}
        error=example_finish(reader,error);
    }
#endif
    return static_cast<int>(example_finish(sender,error));
#endif
}
