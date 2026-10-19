#pragma once
#include "array_common.h"
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <rcl_interfaces/srv/describe_parameters.hpp>

namespace array_probe
{
    inline rclcpp::ParameterValue native(ipc::parameter_value const& value)
    {
        if(value.type==ipc::parameter_kind::boolean_array) { std::vector<bool> values;for(auto item:value.bool_array_value) { values.push_back(item!=0); }return rclcpp::ParameterValue{values}; }
        if(value.type==ipc::parameter_kind::int64_array) { return rclcpp::ParameterValue{value.int64_array_value}; }
        if(value.type==ipc::parameter_kind::float64_array) { return rclcpp::ParameterValue{value.double_array_value}; }
        if(value.type==ipc::parameter_kind::string_array) { return rclcpp::ParameterValue{value.string_array_value}; }
        if(value.type==ipc::parameter_kind::byte_array) { return rclcpp::ParameterValue{value.byte_array_value}; }
        throw std::runtime_error{"unsupported native ROS2 array fixture kind"};
    }
    inline ipc::parameter_value from_native(std::string const& kind,rclcpp::Parameter const& parameter)
    {
        auto result{value(core::version::ros2,kind,true)};
        if(kind=="bool") { require(parameter.get_type()==rclcpp::ParameterType::PARAMETER_BOOL_ARRAY,"native exact Bool array kind");for(auto item:parameter.as_bool_array()) { result.bool_array_value.push_back(item?1:0); } }
        else if(kind=="int64") { require(parameter.get_type()==rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY,"native exact int64 array kind");result.int64_array_value=parameter.as_integer_array(); }
        else if(kind=="float64") { require(parameter.get_type()==rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY,"native exact Float64 array kind");result.double_array_value=parameter.as_double_array(); }
        else if(kind=="string") { require(parameter.get_type()==rclcpp::ParameterType::PARAMETER_STRING_ARRAY,"native exact String array kind");result.string_array_value=parameter.as_string_array(); }
        else { require(parameter.get_type()==rclcpp::ParameterType::PARAMETER_BYTE_ARRAY,"native exact byte array kind");result.byte_array_value=parameter.as_byte_array(); }
        return result;
    }
}
inline int ros2_array_witness(int argc,char** argv)
{
    require(argc==3||argc==4,"array ROS2 witness requires kind and optional bad-native scenario");std::string mode{argv[1]},kind{argv[2]};
    require(mode=="--array-witness"||mode=="--array-observe-set"||mode=="--array-observe-empty"||mode=="--array-observe-denied"||mode=="--array-wrong-kind"||mode=="--array-dynamic-denied"||mode=="--array-bad-native","known native ROS2 array witness mode");
    auto changed{array_probe::value(core::version::ros2,kind)},initial{array_probe::value(core::version::ros2,kind,mode=="--array-observe-set")};
    if(mode=="--array-observe-empty") { changed=array_probe::value(core::version::ros2,kind,true); }
    bool wrong{mode=="--array-wrong-kind"},dynamic{mode=="--array-dynamic-denied"},bad{mode=="--array-bad-native"};
    std::string scenario{argc==4?argv[3]:""};require(bad?(argc==4):(argc==3),"array witness scenario arguments match mode");
    if(bad) { require(scenario!="mixed","generated ROS2 arrays cannot be heterogeneous");initial=array_probe::invalid(core::version::ros2,kind,scenario); }
    int ros_argc{1};rclcpp::init(ros_argc,argv);rclcpp::NodeOptions options;options.start_parameter_services(false);
    auto node{std::make_shared<rclcpp::Node>("native","/uwvm_resources",options)};
    rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.dynamic_typing=dynamic;
    node->declare_parameter("value",wrong?rclcpp::ParameterValue{std::string{"hello"}}:array_probe::native(initial),descriptor);
    auto writes{std::make_shared<std::atomic<unsigned>>()},reads{std::make_shared<std::atomic<unsigned>>()};
    auto get{node->create_service<rcl_interfaces::srv::GetParameters>("/uwvm_resources/native/get_parameters",[node,reads](std::shared_ptr<rcl_interfaces::srv::GetParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::GetParameters::Response> response) {
        ++*reads;for(auto const& name:request->names) { response->values.push_back(node->has_parameter(name)?node->get_parameter(name).get_parameter_value().to_value_msg():rcl_interfaces::msg::ParameterValue{}); }
    })};
    auto describe{node->create_service<rcl_interfaces::srv::DescribeParameters>("/uwvm_resources/native/describe_parameters",[node](std::shared_ptr<rcl_interfaces::srv::DescribeParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::DescribeParameters::Response> response) {
        for(auto const& name:request->names) { response->descriptors.push_back(node->describe_parameter(name)); }
    })};
    auto set{node->create_service<rcl_interfaces::srv::SetParameters>("/uwvm_resources/native/set_parameters",[node,writes](std::shared_ptr<rcl_interfaces::srv::SetParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::SetParameters::Response> response) {
        ++*writes;std::vector<rclcpp::Parameter> parameters;for(auto const& parameter:request->parameters) { parameters.push_back(rclcpp::Parameter::from_parameter_msg(parameter)); }response->results=node->set_parameters(parameters);
    })};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&] { executor.spin(); }};bool observed{};
    try
    {
        std::cout<<"READY array ROS2 kind="<<kind<<" mode="<<mode<<"\n"<<std::flush;
        auto until{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--array-observe-set"||mode=="--array-observe-empty"?30:5)};
        while(std::chrono::steady_clock::now()<until)
        {
            auto actual{node->get_parameter("value")};
            if(wrong) { require(actual.get_type()==rclcpp::ParameterType::PARAMETER_STRING&&actual.as_string()=="hello","array wrong-kind native scalar String unchanged"); }
            else
            {
                auto raw{array_probe::raw(array_probe::from_native(kind,actual))};
                if((mode=="--array-observe-set"||mode=="--array-observe-empty")&&raw==array_probe::raw(changed)&&*writes==1&&*reads>=3) { observed=true;break; }
                if(mode=="--array-observe-denied"||dynamic||bad||mode=="--array-witness") { require(raw==array_probe::raw(initial),"native array exact selected kind/element bits unchanged"); }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        if(mode=="--array-observe-set"||mode=="--array-observe-empty")
        {
            require(observed&&*writes==1&&*reads>=3,"actual array setter1 and all three generated reads observed");std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout<<"OBSERVED_ARRAY_SET native_set_requests="<<*writes<<" native_get_requests="<<*reads<<"\n";
        }
        else
        {
            require(*writes==0,"denied array operation sends zero generated SetParameters requests");
            if(mode=="--array-witness"||bad||wrong||dynamic) { require(*reads>=1,"actual generated array getter/preflight request reached native witness"); }
            std::cout<<"NO_UNAUTHORIZED_ARRAY_WRITE native_set_requests=0 native_get_requests="<<*reads<<"\n";
        }
    }
    catch(...) { executor.cancel();spinner.join();rclcpp::shutdown();throw; }
    executor.cancel();spinner.join();rclcpp::shutdown();std::cout<<"PASS native array parameter witness\n"<<std::flush;return 0;
}
