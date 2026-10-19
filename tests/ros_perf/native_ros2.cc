#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <thread>
int main(int argc,char** argv)
{
    try
    {
        if(argc!=5){throw std::runtime_error("usage: --publish|--observe count payload-bytes topic");}
        std::string mode{argv[1]},topic{argv[4]};unsigned count{static_cast<unsigned>(std::stoul(argv[2]))},bytes{static_cast<unsigned>(std::stoul(argv[3]))};
        if((mode!="--publish"&&mode!="--observe"&&mode!="--emit")||!count||count>200||bytes<16||bytes>4096){throw std::runtime_error("bounded workload");}
        rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp::Node>(mode=="--emit"?"uwvm_ros2_perf_emitter":"uwvm_ros2_perf_observer")};ros_perf::samples samples{bytes,count};std::string callback_error;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub;if(mode!="--emit"){sub=node->create_subscription<std_msgs::msg::String>(topic,rclcpp::QoS{32}.reliable(),[&](std_msgs::msg::String::ConstSharedPtr m){try{samples.receive(m->data);}catch(std::exception const& e){callback_error=e.what();}});}
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub;if(mode!="--observe"){pub=node->create_publisher<std_msgs::msg::String>(topic,rclcpp::QoS{32}.reliable());}
        std::cout<<"READY\n"<<std::flush;
        if(pub){for(unsigned i{};i!=200&&!pub->get_subscription_count();++i){rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(10));}if(!pub->get_subscription_count()){throw std::runtime_error("native discovery timeout");}for(unsigned i{};i!=count;++i){std_msgs::msg::String m;m.data=ros_perf::payload(bytes);pub->publish(m);rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(10));}}
        if(mode=="--emit"){std::this_thread::sleep_for(std::chrono::seconds(1));rclcpp::shutdown();std::cout<<"PASS native ROS2 separate-process emission\n";return 0;}
        auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds(10)};
        while(!samples.done()&&std::chrono::steady_clock::now()<deadline){rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(1));}
        if(!callback_error.empty()){throw std::runtime_error(callback_error);}samples.report();rclcpp::shutdown();return 0;
    }
    catch(std::exception const& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}
}
