#pragma once
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <rcl_interfaces/srv/describe_parameters.hpp>
inline ::rclcpp::ParameterValue ros2_scalar_native(ipc::parameter_value const& value)
{
    if(value.type==ipc::parameter_kind::boolean) { return ::rclcpp::ParameterValue{value.bool_value}; }
    if(value.type==ipc::parameter_kind::int64) { return ::rclcpp::ParameterValue{value.integer_value}; }
    return ::rclcpp::ParameterValue{value.double_value};
}
inline int ros2_scalar_witness(int argc,char** argv)
{
    require(argc==3,"scalar ROS2 witness requires a kind");std::string mode{argv[1]},kind{argv[2]};
    require(mode=="--scalar-witness"||mode=="--scalar-observe-set"||mode=="--scalar-observe-denied"||mode=="--scalar-wrong-kind"||mode=="--scalar-dynamic-denied","known scalar ROS2 witness mode");
    auto initial{scalar_value(core::version::ros2,kind)},changed{scalar_value(core::version::ros2,kind,true)};
    int ros_argc{1};rclcpp::init(ros_argc,argv);rclcpp::NodeOptions options;options.start_parameter_services(false);
    auto node{std::make_shared<rclcpp::Node>("native","/uwvm_resources",options)};
    bool wrong{mode=="--scalar-wrong-kind"},dynamic{mode=="--scalar-dynamic-denied"};
    rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.dynamic_typing=dynamic;
    node->declare_parameter("value",wrong?rclcpp::ParameterValue{std::string{"hello"}}:ros2_scalar_native(initial),descriptor);
    auto writes{std::make_shared<std::atomic<unsigned>>()};
    auto reads{std::make_shared<std::atomic<unsigned>>()};
    auto get{node->create_service<rcl_interfaces::srv::GetParameters>("/uwvm_resources/native/get_parameters",[node,reads](std::shared_ptr<rcl_interfaces::srv::GetParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::GetParameters::Response> response) {
        ++*reads;
        for(auto const& name:request->names) { response->values.push_back(node->has_parameter(name)?node->get_parameter(name).get_parameter_value().to_value_msg():rcl_interfaces::msg::ParameterValue{}); }
    })};
    auto describe{node->create_service<rcl_interfaces::srv::DescribeParameters>("/uwvm_resources/native/describe_parameters",[node](std::shared_ptr<rcl_interfaces::srv::DescribeParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::DescribeParameters::Response> response) {
        for(auto const& name:request->names) { response->descriptors.push_back(node->describe_parameter(name)); }
    })};
    auto set{node->create_service<rcl_interfaces::srv::SetParameters>("/uwvm_resources/native/set_parameters",[node,writes](std::shared_ptr<rcl_interfaces::srv::SetParameters::Request> request,std::shared_ptr<rcl_interfaces::srv::SetParameters::Response> response) {
        ++*writes;std::vector<rclcpp::Parameter> values;for(auto const& parameter:request->parameters) { values.push_back(rclcpp::Parameter::from_parameter_msg(parameter)); }response->results=node->set_parameters(values);
    })};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&] { executor.spin(); }};
    bool observed{};
    try
    {
        std::cout<<"READY scalar ROS2 "<<kind<<'\n'<<std::flush;
        auto until{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--scalar-witness"||mode=="--scalar-observe-set"?30:5)};
        while(std::chrono::steady_clock::now()<until)
        {
            auto native{node->get_parameter("value")};
            if(wrong) { require(native.get_type()==rclcpp::ParameterType::PARAMETER_STRING&&native.as_string()=="hello","wrong-kind native String remains unchanged"); }
            else
            {
                auto value{initial};
                if(value.type==ipc::parameter_kind::boolean) { require(native.get_type()==rclcpp::ParameterType::PARAMETER_BOOL,"native Bool kind retained");value.bool_value=native.as_bool(); }
                else if(value.type==ipc::parameter_kind::int64) { require(native.get_type()==rclcpp::ParameterType::PARAMETER_INTEGER,"native int64 kind retained");value.integer_value=native.as_int(); }
                else { require(native.get_type()==rclcpp::ParameterType::PARAMETER_DOUBLE,"native double kind retained");value.double_value=native.as_double(); }
                auto bytes{scalar_bytes(value)};
                // The roundtrip Guest still needs its final get after mutation:
                // initial read, scalar setter preflight, then final read.
                if(mode=="--scalar-observe-set"&&bytes==scalar_bytes(changed)&&*writes==1&&*reads>=3) { observed=true;break; }
                if(mode=="--scalar-observe-denied"||dynamic) { require(bytes==scalar_bytes(initial),"denied scalar write leaves native kind and exact value unchanged"); }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        if(mode=="--scalar-observe-set")
        {
            require(observed&&*writes==1&&*reads>=3,"native scalar roundtrip received one exact setter and all three reads");
            // The read count increments when the callback starts. Keep the
            // executor active briefly so its generated response can be sent;
            // cancel/join below then finishes any callback already executing.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout<<"OBSERVED_SCALAR_SET native_set_requests="<<*writes<<" native_get_requests="<<*reads<<'\n';
        }
        if(mode=="--scalar-observe-denied"||wrong||dynamic) { require(*writes==0,"denied scalar write sends zero generated SetParameters requests");std::cout<<"NO_UNAUTHORIZED_SCALAR_WRITE native_set_requests=0\n"; }
    }
    catch(...) { executor.cancel();spinner.join();rclcpp::shutdown();throw; }
    executor.cancel();spinner.join();rclcpp::shutdown();return 0;
}
