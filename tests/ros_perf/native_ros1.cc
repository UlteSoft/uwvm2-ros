#include "common.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <thread>
int main(int argc,char** argv)
{
    try
    {
        if(argc!=5){throw std::runtime_error("usage: --publish|--observe count payload-bytes topic");}
        std::string mode{argv[1]},topic{argv[4]};unsigned count{static_cast<unsigned>(std::stoul(argv[2]))},bytes{static_cast<unsigned>(std::stoul(argv[3]))};
        if((mode!="--publish"&&mode!="--observe"&&mode!="--emit")||!count||count>200||bytes<16||bytes>4096){throw std::runtime_error("bounded workload");}
        ros::init(argc,argv,mode=="--emit"?"uwvm_ros1_perf_emitter":"uwvm_ros1_perf_observer",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;ros_perf::samples samples{bytes,count};
        std::string callback_error;std::mutex error_mutex;
        auto sub{mode!="--emit"?node.subscribe<std_msgs::String>(topic,32,[&](std_msgs::String::ConstPtr const& m){try{samples.receive(m->data);}catch(std::exception const& e){std::lock_guard<std::mutex> lock{error_mutex};callback_error=e.what();}}):ros::Subscriber{}};
        auto pub{mode!="--observe"?node.advertise<std_msgs::String>(topic,32,false):ros::Publisher{}};
        ros::AsyncSpinner spinner{1};spinner.start();std::cout<<"READY\n"<<std::flush;
        if(pub){for(unsigned i{};i!=200&&!pub.getNumSubscribers();++i){std::this_thread::sleep_for(std::chrono::milliseconds(10));}if(!pub.getNumSubscribers()){throw std::runtime_error("native discovery timeout");}for(unsigned i{};i!=count;++i){std_msgs::String m;m.data=ros_perf::payload(bytes);pub.publish(m);std::this_thread::sleep_for(std::chrono::milliseconds(10));}}
        if(mode=="--emit"){std::this_thread::sleep_for(std::chrono::seconds(1));spinner.stop();ros::shutdown();std::cout<<"PASS native ROS1 separate-process emission\n";return 0;}
        auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds(10)};
        while(!samples.done()&&std::chrono::steady_clock::now()<deadline){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
        spinner.stop();{std::lock_guard<std::mutex> lock{error_mutex};if(!callback_error.empty()){throw std::runtime_error(callback_error);}}samples.report();ros::shutdown();return 0;
    }
    catch(std::exception const& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}
}
