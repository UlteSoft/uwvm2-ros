#pragma once
#include "../ros_ipc/probe_common.h"

inline int ros_trap_cleanup_witness(int argc, char** argv)
{
    ::ros::init(argc, argv, "uwvm_ros1_trap_cleanup_witness");
    ::ros::NodeHandle node;
    std::atomic<bool> received{}, wrong{};
    boost::function<void(boost::shared_ptr<std_msgs::String const> const&)> callback{
        [&](boost::shared_ptr<std_msgs::String const> const& value) {
            if(value->data == "hello") { received = true; } else { wrong = true; }
        }};
    auto subscriber{node.subscribe<std_msgs::String>("/uwvm_ros_test/output", 4, callback)};
    auto publisher{node.advertise<std_msgs::String>("/uwvm_ros_test/input", 4, false)};
    ::ros::AsyncSpinner spinner{2}; spinner.start();
    std_msgs::String sample; sample.data = "hello";
    bool appeared{}; unsigned absent_samples{};
    auto deadline{std::chrono::steady_clock::now() + std::chrono::seconds{20}};
    ::ros_probe::print_fixture(STDOUT_FILENO,"READY real ROS1 trap cleanup witness\n");
    while(std::chrono::steady_clock::now() < deadline)
    {
        publisher.publish(sample);
        bool const pub_present{subscriber.getNumPublishers() != 0};
        bool const sub_present{publisher.getNumSubscribers() != 0};
        bool const service_present{::ros::service::exists("/uwvm_ros_test/service_trigger", false)};
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
                spinner.stop(); ::ros::shutdown();
                ::ros_probe::print_fixture(STDOUT_FILENO,"PASS trap cleanup actual ROS1 publisher subscriber service absent for ten native graph samples\n");
                return 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    spinner.stop(); ::ros::shutdown();
    ros_probe::require(appeared, "all actual ROS1 resources appeared before Wasm trap");
    throw std::runtime_error{"actual ROS1 resources survived worker trap deadline"};
}
