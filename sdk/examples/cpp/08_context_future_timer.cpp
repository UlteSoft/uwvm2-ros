/** @file
 * @brief Executable Context/Session/Node/Future/CallbackGroup/Timer example.
 * Default mode exercises only local scheduling, not native ROS delivery. Set
 * ROSWASM_EXAMPLE_DEFER_RPC=1 to defer an explicitly granted trigger-call once.
 * The Future preserves the bounded synchronous RPC's actual status. Nothing
 * retries after timeout, silently acquires rights or automatically starts time.
 */
#include "example_support.hpp"
#ifndef ROSWASM_EXAMPLE_DEFER_RPC
#define ROSWASM_EXAMPLE_DEFER_RPC 0
#endif
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Context context{*ROSWASM_BACKEND()};roswasmcpp::Session session{context,1};
    error=session.check_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Node node{context,"cpp-scheduler"};roswasmcpp::Executor executor{context,1};roswasmcpp::CallbackGroup group;
    unsigned completed{},ticks{};size_t delivered{};
#if ROSWASM_EXAMPLE_DEFER_RPC
    auto client=node.create_client<roswasmcpp::srv::Trigger>("trigger-call",512);error=client.error();
    if(!error) {
        auto future=client.async_call(executor,{},[&](auto response){++completed;(void)response;},&group);
        error=executor.spin_some(0,delivered);roswasmcpp::srv::Trigger::Response response;
        if(!error) error=future.take(response);
    }
    error=example_finish(client,error);
#else
    unsigned calls{};
    auto future=executor.post<unsigned>([&]{++calls;return roswasmcpp::DeferredResult<unsigned>{0,42};},[&](auto value){if(!value.error&&value.value==42) ++completed;},&group);
    if(calls) error=UWVM_ROS_INVALID;if(!error) error=executor.spin_some(0,delivered);
    unsigned result{};if(!error) error=future.take(result);if(!error&&(result!=42||calls!=1)) error=UWVM_ROS_INVALID;
#endif
    if(!error&&completed!=1) error=UWVM_ROS_INVALID;
    roswasmcpp::Timer timer{10,100,[&]{++ticks;},&group,&context};
    auto registration=error?0:executor.add_timer(timer);if(!error&&!registration) error=UWVM_ROS_INVALID;
    if(!error) error=executor.spin_some_at(109,1,delivered);if(!error&&ticks) error=UWVM_ROS_INVALID;
    if(!error) error=executor.spin_some_at(110,1,delivered);if(!error&&(ticks!=1||delivered!=1)) error=UWVM_ROS_INVALID;
    executor.remove(registration);executor.shutdown();auto cleanup=session.close({},error);
    return static_cast<int>(error?error:cleanup.cleanup_error);
}
