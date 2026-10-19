#pragma once
#include "../ros_ipc/probe_common.h"

inline int ros_trap_cleanup_witness(int argc, char** argv)
{
    ::rclcpp::init(argc, argv);
    auto node{std::make_shared<::rclcpp::Node>("uwvm_ros2_trap_cleanup_witness")};
    std::atomic<bool> received{}, wrong{};
    auto subscriber{node->create_subscription<std_msgs::msg::String>("/uwvm_ros_test/output", ::rclcpp::QoS{4}.reliable(),
        [&](std::shared_ptr<std_msgs::msg::String const> value) {
            if(value->data == "hello") { received = true; } else { wrong = true; }
        })};
    auto publisher{node->create_publisher<std_msgs::msg::String>("/uwvm_ros_test/input", ::rclcpp::QoS{4}.reliable())};
    std_msgs::msg::String sample; sample.data = "hello";
    bool appeared{}; unsigned absent_samples{};
    auto deadline{std::chrono::steady_clock::now() + std::chrono::seconds{20}};
    ::ros_probe::print_fixture(STDOUT_FILENO,"READY real ROS2 trap cleanup witness\n");
    while(std::chrono::steady_clock::now() < deadline)
    {
        ::rclcpp::spin_some(node); publisher->publish(sample);
        bool const pub_present{node->count_publishers("/uwvm_ros_test/output") != 0};
        bool const sub_present{node->count_subscribers("/uwvm_ros_test/input") != 0};
        bool const service_present{node->get_service_names_and_types().count("/uwvm_ros_test/service_trigger") != 0};
        ros_probe::require(!wrong, "trap publisher emitted exact native String content");
        if(!appeared && received && pub_present && sub_present && service_present)
        {
            appeared = true;
            ::ros_probe::print_fixture(STDOUT_FILENO,"OBSERVED_RESOURCES publisher subscriber Trigger service plus exact hello\n");
        }
        if(appeared)
        {
            absent_samples = !pub_present && !sub_present && !service_present ? absent_samples + 1 : 0;
            if(absent_samples == 10)
            {
                ::rclcpp::shutdown();
                ::ros_probe::print_fixture(STDOUT_FILENO,"PASS trap cleanup actual ROS2 publisher subscriber service absent for ten native graph samples\n");
                return 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    ::rclcpp::shutdown();
    ros_probe::require(appeared, "all actual ROS2 resources appeared before Wasm trap");
    throw std::runtime_error{"actual ROS2 resources survived worker trap deadline"};
}
