// Test-owned discovery gate; the original SDK native probe is unchanged.
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <csignal>
#include <set>
#include <unistd.h>

int main(int argc, char** argv)
{
    try
    {
        if(argc == 6 && std::string{argv[1]} == "--discover-subscription")
        {
            std::string resource{argv[2]}, expected_node{argv[3]}, expected_type{argv[4]}, seen{argv[5]};
            rclcpp::init(argc, argv);
            auto node = std::make_shared<rclcpp::Node>("uwvm_arm_reader_query_" + std::to_string(::getpid()));
            auto end = std::chrono::steady_clock::now() + std::chrono::seconds(6);
            std::set<std::string> diagnosed;
            while(std::chrono::steady_clock::now() < end)
            {
                rclcpp::spin_some(node);
                for(auto const& reader : node->get_subscriptions_info_by_topic(resource))
                {
                    std::string name = reader.node_namespace();
                    if(name != "/") name += "/";
                    name += reader.node_name();
                    auto const& qos = reader.qos_profile().get_rmw_qos_profile();
                    if(name != expected_node || reader.topic_type() != expected_type) continue;
                    std::string gid;
                    static constexpr char digits[] = "0123456789abcdef";
                    for(auto byte : reader.endpoint_gid()) { gid += digits[byte >> 4]; gid += digits[byte & 15]; }
                    if(diagnosed.insert(gid).second)
                        std::cout << "API_READER_QOS resource=" << resource << " node=" << name
                                  << " type=" << expected_type << " reader_gid=" << gid
                                  << " reliability=" << static_cast<int>(qos.reliability)
                                  << " durability=" << static_cast<int>(qos.durability)
                                  << " history=" << static_cast<int>(qos.history)
                                  << " depth=" << qos.depth << "\n" << std::flush;
                    // FastDDS RTPS graph discovery does not carry history/depth.
                    // Keep that absence explicit. A reported known value must
                    // still match the pinned original observer's KeepLast4.
                    bool unavailable = qos.history == RMW_QOS_POLICY_HISTORY_UNKNOWN && qos.depth == 0;
                    bool known_exact = qos.history == RMW_QOS_POLICY_HISTORY_KEEP_LAST && qos.depth == 4;
                    if(qos.reliability != RMW_QOS_POLICY_RELIABILITY_RELIABLE ||
                       qos.durability != RMW_QOS_POLICY_DURABILITY_VOLATILE ||
                       (!unavailable && !known_exact)) continue;
                    if(gid.find_first_not_of('0') == std::string::npos || ("," + seen + ",").find("," + gid + ",") != std::string::npos) continue;
                    std::cout << "READY_SUBSCRIPTION_GRAPH resource=" << resource << " node=" << name
                              << " type=" << expected_type << " reader_gid=" << gid
                              << " reliable=1 volatile=1 history=" << static_cast<int>(qos.history)
                              << " history_policy=" << (unavailable ? "unknown" : "keep-last")
                              << " depth=" << qos.depth
                              << " history_depth_scope=" << (unavailable ? "remote-unavailable" : "remote-reported")
                              << " pid=" << ::getpid() << "\n" << std::flush;
                    // A task-owned observation barrier ensures the short helper
                    // itself has exact live ELF/QEMU/cgroup evidence before exit.
                    ::raise(SIGSTOP);
                    rclcpp::shutdown(); return 0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            throw std::runtime_error("actual DDS exact new reader graph discovery deadline");
        }
        if(argc != 5 || std::string{argv[1]} != "--observe" || std::string{argv[4]} != "String")
            throw std::runtime_error("usage observer --observe resource hello String");
        std::string resource{argv[2]}, expected{argv[3]};
        rclcpp::init(argc, argv);
        auto node = std::make_shared<rclcpp::Node>("uwvm_arm_multi_memory_observer_" + std::to_string(::getpid()));
        unsigned received{}; bool wrong{};
        auto subscriber = node->create_subscription<std_msgs::msg::String>(resource, rclcpp::QoS{4}.reliable(),
            [&](std_msgs::msg::String::ConstSharedPtr const& message)
            {
                if(message->data != expected) wrong = true;
                else ++received;
            });
        // Same 12-second observer budget as the original --observe witness.
        auto end = std::chrono::steady_clock::now() + std::chrono::seconds(12);
        std::cout << "READY_SUBSCRIBER pid=" << ::getpid() << " node=" << node->get_fully_qualified_name()
                  << " resource=" << resource << "\n" << std::flush;
        bool matched{};
        while(std::chrono::steady_clock::now() < end)
        {
            rclcpp::spin_some(node);
            auto writers = node->get_publishers_info_by_topic(resource);
            if(writers.size() == 1 && subscriber->get_publisher_count() == 1 &&
               writers[0].node_name() == "uwvm_ros2_wasm_network" &&
               writers[0].node_namespace() == "/uwvm_ros_test" &&
               writers[0].topic_type() == "std_msgs/msg/String")
            {
                auto const& gid = writers[0].endpoint_gid();
                std::cout << "READY_MATCHED pid=" << ::getpid()
                          << " actual_publishers=1 node=/uwvm_ros_test/uwvm_ros2_wasm_network"
                          << " resource=" << resource << " writer_gid=";
                static constexpr char digits[] = "0123456789abcdef";
                for(auto byte : gid) std::cout << digits[byte >> 4] << digits[byte & 15];
                std::cout << "\n" << std::flush;
                matched = true; break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if(!matched) throw std::runtime_error("actual ROS2 reliable DDS exact publisher match deadline");
        while(std::chrono::steady_clock::now() < end && !wrong)
        {
            rclcpp::spin_some(node);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        // Original --observe requires received && !wrong. The volatile first
        // send creates the endpoint and may precede discovery; send two is gated.
        if(!received || received > 2 || wrong) throw std::runtime_error("actual native ROS2 exact content actual=" + std::to_string(received));
        rclcpp::shutdown();
        std::cout << "PASS actual ROS2 exact String hello native_count=" << received << " wrong_count=0\n";
        return 0;
    }
    catch(std::exception const& error) { std::cerr << "FAIL " << error.what() << "\n"; return 1; }
}
