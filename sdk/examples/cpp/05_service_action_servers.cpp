/** @file
 * @brief Runnable caller-driven typed service/Action server, one bounded pass.
 * Compile ROSWASM_EXAMPLE_CASE=0 Trigger,1 Fibonacci,2 SetBool. Independent
 * advertise/respond grants are acquired before transactional activation.
 * This short application polls once then checks both closes. Idle returns11,
 * so exit0 requires one checked callback/response, not merely an empty queue.
 * Retained applications keep server+registration alive across externally paced
 * passes. No thread/time/sleep/discovery loop is implicitly acquired.
 * See ../c/ADVANCED.md for policies, all40 Action QoS fields and native clients.
 */
#include "example_support.hpp"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 2
#error "ROSWASM_EXAMPLE_CASE must be 0..2"
#endif
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Context context{*ROSWASM_BACKEND()};roswasmcpp::Node node{context,"cpp-server"};roswasmcpp::Executor executor{context};
#if ROSWASM_EXAMPLE_CASE == 1
    auto server=node.create_action_server("fib-advertise","fib-respond",12);error=server.error();uint32_t response_error{};
    if(!error) server.on_goal([&](roswasmcpp::FibonacciGoal const& goal){
        // Compute only the separately policy-bounded order0..12; no chosen ID.
        if(goal.order<0||goal.order>12) {response_error=UWVM_ROS_INVALID;return;}
        std::vector<int32_t> sequence;for(int32_t n=0;n<goal.order;++n) sequence.push_back(n<2?n:sequence[n-1]+sequence[n-2]);
        response_error=server.respond({goal.id,2,std::move(sequence)});
    });
#elif ROSWASM_EXAMPLE_CASE == 2
    auto server=node.create_service<roswasmcpp::srv::SetBool>("set-bool-advertise","set-bool-respond",64);error=server.error();
    if(!error) server.on_request([](roswasmcpp::srv::SetBool::Request const& request){return roswasmcpp::srv::SetBool::Response{request.data,"guest SetBool handler"};});
#else
    auto server=node.create_service<roswasmcpp::srv::Trigger>("trigger-advertise","trigger-respond",64);error=server.error();
    if(!error) server.on_request([](roswasmcpp::srv::Trigger::Request const&){return roswasmcpp::srv::Trigger::Response{true,"guest Trigger handler"};});
#endif
    size_t delivered{};size_t registration{};
    if(!error) {registration=executor.add(server);if(!registration) error=UWVM_ROS_INVALID;}
    if(!error) error=executor.spin_some(1,delivered);executor.remove(registration);
#if ROSWASM_EXAMPLE_CASE == 1
    if(!error) error=response_error;
#endif
    if(!error&&!delivered) error=UWVM_ROS_AGAIN;
    return static_cast<int>(example_finish_server(server,error));
}
