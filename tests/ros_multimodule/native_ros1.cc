#include <ros/ros.h>
#include <std_msgs/String.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <set>
#include <string>
#include <thread>
#include <vector>
int main(int argc,char** argv)
{
    try
    {
        if(argc<3||(argc-1)%2) { throw std::runtime_error("usage resource expected-text pairs"); }
        std::map<std::string,std::set<std::string>> expected;for(int i{1};i<argc;i+=2) { if(!expected[argv[i]].insert(argv[i+1]).second) { throw std::runtime_error("duplicate observed resource/content pair"); } }
        int init_argc{1};ros::init(init_argc,argv,"uwvm_five_module_native_ros1",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;
        std::map<std::pair<std::string,std::string>,unsigned> counts;std::mutex mutex;bool wrong{};std::vector<ros::Subscriber> subscriptions;
        for(auto const& item:expected)
        {
            for(auto const& value:item.second) { counts[{item.first,value}]=0; }
            boost::function<void(boost::shared_ptr<std_msgs::String const> const&)> callback{[&,resource=item.first,values=item.second](auto const& message) { std::lock_guard<std::mutex> guard{mutex};if(!values.count(message->data)) { wrong=true; }else { ++counts[{resource,message->data}]; } }};
            subscriptions.push_back(node.subscribe<std_msgs::String>(item.first,4,callback));
        }
        ros::AsyncSpinner spinner{2};spinner.start();std::cout<<"READY five-module ROS1 native observer\n"<<std::flush;
        auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(25);bool complete{};
        while(std::chrono::steady_clock::now()<deadline)
        {
            { std::lock_guard<std::mutex> guard{mutex};complete=!wrong;for(auto const& item:counts) { complete=complete&&item.second>=2; }if(wrong||complete) { break; } }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));spinner.stop();ros::shutdown();
        if(wrong||!complete) { throw std::runtime_error("every ROS1 module must publish only its exact native-observed content before deadline"); }
        for(auto const& item:counts) { std::cout<<"OBSERVED "<<item.first.first<<" count="<<item.second<<" exact="<<item.first.second<<'\n'; }
        std::cout<<"PASS actual five-module ROS1 exact resources and content\n";return 0;
    }
    catch(std::exception const& error) { std::cerr<<error.what()<<'\n';return 1; }
}
