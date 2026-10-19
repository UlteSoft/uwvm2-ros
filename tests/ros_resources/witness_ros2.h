#pragma once
#include "scalar_witness_ros2.h"
#include "typed_event_witness_ros2.h"
#include "array_witness_ros2.h"
inline int ros2_resource_witness(int argc,char** argv)
{
    if(std::string{argv[1]}=="--typed-events") { return ros2_typed_event_witness(argc,argv); }
    if(std::string{argv[1]}.rfind("--scalar-",0)==0) { return ros2_scalar_witness(argc,argv); }
    if(std::string{argv[1]}.rfind("--array-",0)==0) { return ros2_array_witness(argc,argv); }
    std::string mode{argv[1]};int ros_argc{1};rclcpp::init(ros_argc,argv);
    rclcpp::NodeOptions options;if(mode=="--observe-read-denied") { options.start_parameter_services(false); }
    auto node{std::make_shared<rclcpp::Node>("native","/uwvm_resources",options)};
    node->declare_parameter<std::string>("value",mode=="--observe-set"?"before":"hello");node->declare_parameter<std::string>("other","hidden");
    auto publisher{node->create_publisher<std_msgs::msg::String>("/uwvm_resources/topic",rclcpp::QoS{2})};
    std::atomic<unsigned> reads{};
    rclcpp::Service<rcl_interfaces::srv::GetParameters>::SharedPtr read_service;
    if(mode=="--observe-read-denied") { read_service=node->create_service<rcl_interfaces::srv::GetParameters>("/uwvm_resources/native/get_parameters",[&](std::shared_ptr<rcl_interfaces::srv::GetParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::GetParameters::Response> response) {
        ++reads;for(auto const& name:request->names) { auto parameter=node->get_parameter(name);response->values.push_back(parameter.get_parameter_value().to_value_msg()); }
    }); }
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&] { executor.spin(); }};
    std::cout<<"READY resource witness ROS2\n"<<std::flush;
    bool observed{};
    try
    {
        auto until{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--observe-denied"||mode=="--observe-read-denied"?5:30)};
        while(std::chrono::steady_clock::now()<until)
        {
            auto value{node->get_parameter("value").as_string()};
            if(mode=="--observe-set"&&value=="hello") { observed=true;std::cout<<"OBSERVED_SET\n"<<std::flush;break; }
            if(mode=="--observe-denied") { require(value=="hello","denied Wasm parameter write has no native side effect"); }
            if(mode=="--events") { node->set_parameter(rclcpp::Parameter{"value","selected event"}); }
            std::this_thread::sleep_for(std::chrono::milliseconds(mode=="--events"?100:20));
        }
    }
    catch(...) { executor.cancel();spinner.join();rclcpp::shutdown();throw; }
    executor.cancel();spinner.join();rclcpp::shutdown();
    if(mode=="--observe-set") { require(observed,"native Wasm set observed before deadline"); }
    if(mode=="--observe-denied") { std::cout<<"NO_UNAUTHORIZED_WRITE\n"<<std::flush; }
    if(mode=="--observe-read-denied") { require(reads==0,"denied Wasm parameter read causes zero generated native GetParameters requests");std::cout<<"NO_UNAUTHORIZED_READ native_get_requests=0\n"<<std::flush; }
    (void)argc;return 0;
}
