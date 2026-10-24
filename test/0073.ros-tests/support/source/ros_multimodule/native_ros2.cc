#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
int main(int argc,char** argv)
{
    try
    {
        if(argc<3||(argc-1)%2) { throw std::runtime_error("usage resource expected-text pairs"); }
        std::map<std::string,std::string> expected;for(int i{1};i<argc;i+=2) { if(!expected.emplace(argv[i],argv[i+1]).second) { throw std::runtime_error("duplicate observed resource"); } }
        int init_argc{1};rclcpp::init(init_argc,argv);auto node=std::make_shared<rclcpp::Node>("uwvm_five_module_native_ros2");
        std::map<std::string,unsigned> counts;std::mutex mutex;bool wrong{};std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> subscriptions;
        for(auto const& item:expected)
        {
            counts[item.first]=0;subscriptions.push_back(node->create_subscription<std_msgs::msg::String>(item.first,rclcpp::QoS{4}.reliable().transient_local(),[&,resource=item.first,value=item.second](std_msgs::msg::String::SharedPtr message) { std::lock_guard<std::mutex> guard{mutex};if(message->data!=value) { wrong=true; }++counts[resource]; }));
        }
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&] { executor.spin(); }};
        std::cout<<"READY five-module ROS2 native observer\n"<<std::flush;auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(25);bool complete{};
        while(std::chrono::steady_clock::now()<deadline)
        {
            { std::lock_guard<std::mutex> guard{mutex};complete=!wrong;for(auto const& item:counts) { complete=complete&&item.second>=2; }if(wrong||complete) { break; } }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));executor.cancel();spinner.join();rclcpp::shutdown();
        if(wrong||!complete) { throw std::runtime_error("every ROS2 module must publish only its exact native-observed content before deadline"); }
        for(auto const& item:counts) { std::cout<<"OBSERVED "<<item.first<<" count="<<item.second<<" exact="<<expected.at(item.first)<<'\n'; }
        std::cout<<"PASS actual five-module ROS2 exact resources and content\n";return 0;
    }
    catch(std::exception const& error) { std::cerr<<error.what()<<'\n';return 1; }
}
