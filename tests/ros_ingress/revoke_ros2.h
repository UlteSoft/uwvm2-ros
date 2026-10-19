#pragma once
#include "../ros_ipc/probe_common.h"
#include "revoke_observer.h"
inline int ros_revoke_race_witness(int argc, char** argv)
{
    ::rclcpp::init(argc, argv); auto node{std::make_shared<::rclcpp::Node>("uwvm_ros2_revoke_race_witness")};
    ros_revoke_observer observer{argv[2]};
    auto subscriber{node->create_subscription<std_msgs::msg::String>("/uwvm_ros_test/revoke_race", ::rclcpp::QoS{256}.reliable(),
        [&](std::shared_ptr<std_msgs::msg::String const> value) { observer.receive(value->data); })};
    unsigned stable{}; auto deadline{std::chrono::steady_clock::now() + std::chrono::seconds{20}};
    ::ros_probe::print_fixture(STDOUT_FILENO,"READY real ROS2 revocation race observer\n");
    while(std::chrono::steady_clock::now() < deadline)
    {
        ::rclcpp::spin_some(node);
        stable = observer.complete() ? stable + 1 : 0;
        if(stable == 30)
        {
            ::rclcpp::shutdown();
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS2 admitted=",observer.messages.size()," unique-received=",observer.messages.size()," native-count-frozen=30 samples\n");
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    ::rclcpp::shutdown(); throw std::runtime_error{"native ROS2 revocation race count did not settle exactly to admitted operations"};
}
