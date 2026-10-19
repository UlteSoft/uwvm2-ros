#include "ipc_service.h"
#include <ros/ros.h>
#include <std_srvs/Trigger.h>
int main(int argc,char** argv)
{
    try
    {
        bool flood=argc==3&&std::string{argv[1]}=="--observe-flood";
        ros_probe::require(argc==2||flood,"usage: actual-gateway-path | --observe-flood resource");
        ros::init(argc,argv,"uwvm_ipc_service_metrics",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;
        std::atomic<unsigned> calls{};auto server=node.advertiseService<std_srvs::Trigger::Request,std_srvs::Trigger::Response>(flood?argv[2]:"/uwvm_test/trigger",[&](auto&,auto& reply){++calls;std::this_thread::sleep_for(std::chrono::milliseconds(5));reply.success=true;reply.message="ok";return true;});
        ros::AsyncSpinner spinner{4};spinner.start();
        if(flood){std::cout<<"READY actual generated Trigger counter\n"<<std::flush;std::this_thread::sleep_for(std::chrono::seconds(8));ros_probe::require(calls==2,"native service flood callbacks exactly2");std::cout<<"PASS actual native Trigger callbacks2 for100 malicious Guest requests\n"<<std::flush;}
        else{ros_perf::measure(ros_probe::core::version::ros1,argv[1],calls);}
        spinner.stop();ros::shutdown();return 0;
    }
    catch(std::exception const& error){std::cerr<<"FAIL "<<error.what()<<'\n';ros::shutdown();return 1;}
}
