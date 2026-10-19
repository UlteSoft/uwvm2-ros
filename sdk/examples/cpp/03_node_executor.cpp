/** @file
 * @brief Layer3: typed Node-style publishers/subscriptions and a guest executor.
 * Grants: `pub` String topic.publish, `sub` String topic.subscribe; every native
 * endpoint/type/QoS setting belongs to host policy. Complete event quota <=128.
 * Node is a local lifetime context and must outlive resources. It is not rclcpp
 * or native node identity. Queue-empty11 is normal and callbacks run only when
 * the application calls spin_some. One pass does not guarantee DDS delivery.
 */
#include "example_support.hpp"
/** @brief Publish one typed value and run one bounded, explicitly invoked polling pass. */
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Node node{*ROSWASM_BACKEND(),"cpp-example"};
    auto pub=node.create_publisher<roswasmcpp::msg::String>("pub");
    if(pub.error()) return static_cast<int>(pub.error());
    auto sub=node.create_subscription<roswasmcpp::msg::String>("sub",128);
    if(sub.error()) return static_cast<int>(example_finish(pub,sub.error()));
    size_t received{};sub.on_message([&received](roswasmcpp::msg::String const& message){
        ++received; /* Copy message.data here if it must outlive this callback. */
        (void)message;
    });
    error=pub.publish({"hello"});
    roswasmcpp::Executor executor;auto registration=executor.add(sub);size_t delivered{};
    if(!registration&&!error) error=UWVM_ROS_INVALID;
    if(!error) error=executor.spin_some(1,delivered);executor.remove(registration);
    error=example_finish(sub,error);return static_cast<int>(example_finish(pub,error));
}
