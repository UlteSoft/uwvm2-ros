#pragma once
#include "scalar_witness_ros1.h"
#include "array_witness_ros1.h"
inline int ros1_resource_witness(int argc,char** argv)
{
    if(std::string{argv[1]}.rfind("--scalar-",0)==0) { return ros1_scalar_witness(argc,argv); }
    if(std::string{argv[1]}.rfind("--array-",0)==0) { return ros1_array_witness(argc,argv); }
    std::string mode{argv[1]};ros::M_string remapping;remapping["__ns"]="/uwvm_resources";
    ros::init(remapping,"native",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;ros::AsyncSpinner spinner{2};spinner.start();
    node.setParam("/uwvm_resources/value",std::string{mode=="--observe-set"?"before":"hello"});
    auto publisher{node.advertise<std_msgs::String>("/uwvm_resources/topic",2,false)};
    boost::function<bool(std_srvs::Trigger::Request&,std_srvs::Trigger::Response&)> callback{[](std_srvs::Trigger::Request&,std_srvs::Trigger::Response& response) { response.success=true;response.message="resource witness";return true; }};
    auto service{node.advertiseService("/uwvm_resources/trigger",callback)};
    std::cout<<"READY resource witness ROS1\n"<<std::flush;
    auto until{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--observe-denied"?5:30)};
    bool observed{};
    while(std::chrono::steady_clock::now()<until)
    {
        std::string value;bool exists{node.getParam("/uwvm_resources/value",value)};
        if(mode=="--observe-delete"&&!exists) { observed=true;std::cout<<"OBSERVED_DELETE\n"<<std::flush;break; }
        require(exists,"native resource witness value retained");
        if(mode=="--observe-set"&&value=="hello") { observed=true;std::cout<<"OBSERVED_SET\n"<<std::flush;break; }
        if(mode=="--observe-denied") { require(value=="hello","denied Wasm parameter write has no native side effect"); }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    spinner.stop();ros::shutdown();
    if(mode=="--observe-set") { require(observed,"native Wasm set observed before deadline"); }
    if(mode=="--observe-delete") { require(observed,"native Wasm delete observed before deadline"); }
    if(mode=="--observe-denied") { std::cout<<"NO_UNAUTHORIZED_WRITE\n"<<std::flush; }
    (void)argc;return 0;
}
