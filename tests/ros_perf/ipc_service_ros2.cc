#include "ipc_service.h"
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/trigger.hpp>
int main(int argc,char** argv)
{
    try
    {
        bool flood=argc==3&&std::string{argv[1]}=="--observe-flood";
        ros_probe::require(argc==2||flood,"usage: actual-gateway-path | --observe-flood resource");rclcpp::init(argc,argv);auto node=std::make_shared<rclcpp::Node>("uwvm_ipc_service_metrics");
        std::atomic<unsigned> calls{};auto group=node->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
        auto server=node->create_service<std_srvs::srv::Trigger>(flood?argv[2]:"/uwvm_test/trigger",[&](std::shared_ptr<std_srvs::srv::Trigger::Request>,std::shared_ptr<std_srvs::srv::Trigger::Response> reply){++calls;std::this_thread::sleep_for(std::chrono::milliseconds(5));reply->success=true;reply->message="ok";},rclcpp::ServicesQoS{},group);
        rclcpp::executors::MultiThreadedExecutor executor{rclcpp::ExecutorOptions{},4};executor.add_node(node);std::thread spin{[&]{executor.spin();}};
        try
        {
            if(flood){std::cout<<"READY actual generated Trigger counter\n"<<std::flush;std::this_thread::sleep_for(std::chrono::seconds(8));ros_probe::require(calls==2,"native service flood callbacks exactly2");std::cout<<"PASS actual native Trigger callbacks2 for100 malicious Guest requests\n"<<std::flush;}
            else{ros_perf::measure(ros_probe::core::version::ros2,argv[1],calls);}
        }
        catch(...){executor.cancel();spin.join();throw;}
        executor.cancel();spin.join();rclcpp::shutdown();return 0;
    }
    catch(std::exception const& error){std::cerr<<"FAIL "<<error.what()<<'\n';rclcpp::shutdown();return 1;}
}
