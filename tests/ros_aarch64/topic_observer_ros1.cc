// Test-owned discovery gate; the original SDK native probe is unchanged.
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <boost/function.hpp>
#include <atomic>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <unistd.h>

int main(int argc, char** argv)
{
    try
    {
        if(argc != 5 || std::string{argv[1]} != "--observe" || std::string{argv[4]} != "String")
            throw std::runtime_error("usage observer --observe resource hello String");
        std::string resource{argv[2]}, expected{argv[3]};
        auto name = "uwvm_arm_multi_memory_observer_" + std::to_string(::getpid());
        ros::init(argc, argv, name, ros::init_options::NoSigintHandler | ros::init_options::NoRosout);
        ros::NodeHandle node;
        std::atomic<unsigned> received{};
        std::atomic<bool> wrong{};
        boost::function<void(std_msgs::String::ConstPtr const&)> callback{
            [&](std_msgs::String::ConstPtr const& message)
            {
                if(message->data != expected) wrong = true;
                else ++received;
            }};
        auto subscriber = node.subscribe<std_msgs::String>(resource, 4, callback);
        ros::AsyncSpinner spinner{1}; spinner.start();
        // Same 12-second observer budget as the original --observe witness.
        auto end = std::chrono::steady_clock::now() + std::chrono::seconds(12);
        std::cout << "READY_SUBSCRIBER pid=" << ::getpid() << " node=" << ros::this_node::getName()
                  << " resource=" << resource << "\n" << std::flush;
        while(std::chrono::steady_clock::now() < end && subscriber.getNumPublishers() != 1)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(subscriber.getNumPublishers() != 1) throw std::runtime_error("actual ROS1 TCPROS publisher match deadline");
        std::cout << "READY_MATCHED pid=" << ::getpid() << " actual_publishers=1 resource=" << resource
                  << "\n" << std::flush;
        while(std::chrono::steady_clock::now() < end && !wrong)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // Original --observe requires received && !wrong. The volatile first
        // send creates the endpoint and may precede discovery; send two is gated.
        if(!received || received > 2 || wrong) throw std::runtime_error("actual native ROS1 exact content actual=" + std::to_string(received.load()));
        spinner.stop(); ros::shutdown();
        std::cout << "PASS actual ROS1 exact String hello native_count=" << received << " wrong_count=0\n";
        return 0;
    }
    catch(std::exception const& error) { std::cerr << "FAIL " << error.what() << "\n"; return 1; }
}
