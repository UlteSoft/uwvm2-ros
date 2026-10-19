/** Bounded native-only distribution smoke witness.
 *
 * Two independently created pub/sub entities exchange one exact String after graph
 * matching. This tests the selected native SDK/RMW; it does not instantiate a
 * Gateway, Worker or Wasm Guest and does not qualify their capability boundary.
 * ROS1 shares one process/node; ROS2 creates two nodes. ROS1 requires an
 * explicitly provisioned loopback Master. The orchestration
 * profile owns the domain/Master selection and the outer command deadline.
 * Diagnostic I/O uses the checked C++17 native_io shim because old ROS toolchains
 * cannot parse the vendored C++20 fast_io concepts. No iostream is used.
 */
#include "basic_features.h"
#include "native_io.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>
#if UWVM_ROS_BACKEND == 1
#include <ros/ros.h>
#include <std_msgs/String.h>
#elif UWVM_ROS_BACKEND == 2
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#else
#error "Select exactly one ROS backend"
#endif

namespace
{
namespace io = uwvm2::ros_gateway::native_io;
constexpr std::string_view payload{"uwvm-native-distribution-exact-string"};
void checked(bool condition, char const* message)
{ if(!condition) { throw std::runtime_error(message); } }
struct counters
{
    unsigned received{}, mismatches{};
    void accept(std::string const& text)
    { ++received; if(std::string_view{text.data(), text.size()} != payload || received != 1) { ++mismatches; } }
};
}

int main(int argc, char** argv)
{
    try
    {
        counters counts;
        io::fixed_text<96> topic_text;
        checked(topic_text.append("/uwvm_distro_basic/p") && topic_text.number(::getpid()), "topic bound");
        std::string topic{topic_text.view()};
#if UWVM_ROS_BACKEND == 1
        ros::init(argc, argv, "uwvm_distribution_basic", ros::init_options::NoSigintHandler);
        checked(ros::master::check(), "explicit loopback Master is unavailable");
        ros::NodeHandle publisher_node, subscriber_node;
        auto callback = [&counts](std_msgs::String::ConstPtr const& message)
        { counts.accept(message->data); };
        auto subscriber = subscriber_node.subscribe<std_msgs::String>(topic, 4, callback);
        auto publisher = publisher_node.advertise<std_msgs::String>(topic, 4, false);
        auto spin = [] { ros::spinOnce(); };
        auto matched = [&] { return publisher.getNumSubscribers() == 1 && subscriber.getNumPublishers() == 1; };
#else
        rclcpp::init(argc, argv);
        auto selected_rmw = std::getenv("RMW_IMPLEMENTATION");
        auto actual_rmw = rmw_get_implementation_identifier();
        checked(selected_rmw && actual_rmw && std::string_view{selected_rmw} == actual_rmw,
                "actual RMW differs from explicit profile");
        auto publisher_node = std::make_shared<rclcpp::Node>("uwvm_distribution_publisher");
        auto subscriber_node = std::make_shared<rclcpp::Node>("uwvm_distribution_subscriber");
        auto callback = [&counts](std_msgs::msg::String::SharedPtr message)
        { counts.accept(message->data); };
#if UWVM_BASIC_HAVE_QOS
        auto qos = rclcpp::QoS(4).reliable().durability_volatile();
        auto publisher = publisher_node->create_publisher<std_msgs::msg::String>(topic, qos);
        auto subscriber = subscriber_node->create_subscription<std_msgs::msg::String>(topic, qos, callback);
#else
        // Ardent/Crystal public headers expose the callback/profile ordering.
        // The selected SDK compile probe chooses this branch, not ROS_DISTRO.
        auto qos = rmw_qos_profile_default; qos.depth = 4;
        auto publisher = publisher_node->create_publisher<std_msgs::msg::String>(topic, qos);
        auto subscriber = subscriber_node->create_subscription<std_msgs::msg::String>(topic, callback, qos);
#endif
        rclcpp::executors::SingleThreadedExecutor executor;
        executor.add_node(publisher_node); executor.add_node(subscriber_node);
        auto spin = [&] { executor.spin_some(); };
        auto matched = [&] { return publisher_node->count_subscribers(topic) == 1 &&
                                   subscriber_node->count_publishers(topic) == 1; };
#endif
        auto until = std::chrono::steady_clock::now() + std::chrono::seconds(10);
        while(!matched() && std::chrono::steady_clock::now() < until)
        { spin(); std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
        checked(matched(), "bounded native discovery did not match exactly one peer");
#if UWVM_ROS_BACKEND == 1
        std_msgs::String message; message.data.assign(payload.data(), payload.size());
#else
        std_msgs::msg::String message; message.data.assign(payload.data(), payload.size());
#endif
#if UWVM_ROS_BACKEND == 1
        publisher.publish(message);
#else
        publisher->publish(message);
#endif
        while(counts.received == 0 && std::chrono::steady_clock::now() < until)
        { spin(); std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
        checked(counts.received == 1 && counts.mismatches == 0, "exact native payload not received");
        auto quiet = std::chrono::steady_clock::now() + std::chrono::seconds(2);
        unsigned samples{};
        while(std::chrono::steady_clock::now() < quiet)
        { spin(); ++samples; std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
        checked(samples >= 30 && counts.received == 1 && counts.mismatches == 0, "unexpected duplicate native effect");
#if UWVM_ROS_BACKEND == 1
        publisher.shutdown(); subscriber.shutdown(); ros::shutdown();
#else
        executor.remove_node(subscriber_node); executor.remove_node(publisher_node);
        subscriber.reset(); publisher.reset(); subscriber_node.reset(); publisher_node.reset();
        rclcpp::shutdown();
#endif
        checked(io::write_all(STDOUT_FILENO,
                "PASS_ROS_DISTRO_NATIVE_TOPIC sent=1 received=1 mismatches=0\n"), "short diagnostic write");
        return 0;
    }
    catch(std::exception const&)
    {
        (void)io::write_all(STDERR_FILENO, "FAIL_ROS_DISTRO_NATIVE_TOPIC\n");
        return 1;
    }
}
