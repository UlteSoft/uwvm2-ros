#pragma once
#include "../ros_ipc/probe_common.h"
#include "revoke_observer.h"
inline int ros_revoke_race_witness(int argc, char** argv)
{
    ::ros::init(argc, argv, "uwvm_ros1_revoke_race_witness"); ::ros::NodeHandle node;
    ros_revoke_observer observer{argv[2]};
    boost::function<void(boost::shared_ptr<std_msgs::String const> const&)> callback{
        [&](boost::shared_ptr<std_msgs::String const> const& value) { observer.receive(value->data); }};
    auto subscriber{node.subscribe<std_msgs::String>("/uwvm_ros_test/revoke_race", 256, callback)};
    unsigned stable{}; auto deadline{std::chrono::steady_clock::now() + std::chrono::seconds{20}};
    ::ros_probe::print_fixture(STDOUT_FILENO,"READY real ROS1 revocation race observer\n");
    while(std::chrono::steady_clock::now() < deadline)
    {
        ::ros::spinOnce();
        stable = observer.complete() ? stable + 1 : 0;
        if(stable == 30)
        {
            ::ros::shutdown();
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS1 admitted=",observer.messages.size()," unique-received=",observer.messages.size()," native-count-frozen=30 samples\n");
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    ::ros::shutdown(); throw std::runtime_error{"native ROS1 revocation race count did not settle exactly to admitted operations"};
}
