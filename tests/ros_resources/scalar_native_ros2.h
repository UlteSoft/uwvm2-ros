#pragma once
#include "scalar_native_common.h"
inline void ros2_native_scalars(rclcpp::Node::SharedPtr const& node,char const* executable)
{
    for(auto const& kind:native_scalar_kinds(core::version::ros2))
    {
        auto initial{scalar_value(core::version::ros2,kind)};
        node->declare_parameter("scalar_"+kind,ros2_scalar_native(initial));
        rcl_interfaces::msg::ParameterDescriptor dynamic;dynamic.dynamic_typing=true;node->declare_parameter("scalar_dynamic_"+kind,ros2_scalar_native(initial),dynamic);
    }
    node->declare_parameter<std::string>("scalar_wrong","hello");
    std::atomic<unsigned> dynamic_writes{};
    auto observed{node->add_on_set_parameters_callback([&](std::vector<rclcpp::Parameter> const& parameters) {
        for(auto const& parameter:parameters) { if(parameter.get_name().rfind("scalar_dynamic_",0)==0) { ++dynamic_writes; } }
        rcl_interfaces::msg::SetParametersResult result;result.successful=true;return result;
    })};
    fixture f{core::version::ros2,executable,[](gateway::configuration& cfg) {
        native_scalar_caps(cfg,core::version::ros2);cfg.policy.connection["node-name"]="uwvm_scalar_gateway";
        for(auto const& kind:native_scalar_kinds(core::version::ros2))
        {
            add(cfg,native_scalar_cap(core::version::ros2,kind+"dynamicset","param.set","scalar_dynamic_"+kind,kind));
            add(cfg,native_scalar_cap(core::version::ros2,kind+"absentset","param.set","scalar_absent_"+kind,kind));
            auto local=[&](std::string alias,std::string op,std::string const& type) {
                auto cap{scalar_cap(core::version::ros2,std::move(alias),std::move(op),"/uwvm_test/uwvm_scalar_gateway/local_"+kind,type)};
                cap.limits["parameter-node"]="/uwvm_test/uwvm_scalar_gateway";cap.limits["parameter-name"]="local_"+kind;return cap;
            };
            for(auto const& op:{"declare","get","set","undeclare"}) { add(cfg,local(kind+"local"+op,std::string{"param."}+op,kind)); }
            auto other{kind=="bool"?"int64":"bool"};add(cfg,local(kind+"wrongundeclare","param.undeclare",other));add(cfg,local(kind+"wronglocalset","param.set",other));
            if(kind!="bool") { auto cap{local(kind+"boundundeclare","param.undeclare",kind)};cap.limits["parameter-min-value"]="0";cap.limits["parameter-max-value"]="1";add(cfg,std::move(cap)); }
        }
    }};ipc::client client;f.connect(client);
    auto read=[&](std::string const& kind) {
        auto native{node->get_parameter("scalar_"+kind)};auto value{scalar_value(core::version::ros2,kind)};
        if(kind=="bool") { require(native.get_type()==rclcpp::ParameterType::PARAMETER_BOOL,"ROS2 exact Boolean type");value.bool_value=native.as_bool(); }
        else if(kind=="int64") { require(native.get_type()==rclcpp::ParameterType::PARAMETER_INTEGER,"ROS2 exact signed int64 type");value.integer_value=native.as_int(); }
        else { require(native.get_type()==rclcpp::ParameterType::PARAMETER_DOUBLE,"ROS2 exact double type");value.double_value=native.as_double(); }
        return scalar_bytes(value);
    };
    native_scalar_roundtrips(client,core::version::ros2,read,[&]{auto native{node->get_parameter("scalar_wrong")};require(native.get_type()==rclcpp::ParameterType::PARAMETER_STRING&&native.as_string()=="hello","ROS2 wrong-type scalar denial leaves native String unchanged");});
    for(auto const& kind:native_scalar_kinds(core::version::ros2))
    {
        auto initial{scalar_value(core::version::ros2,kind)},changed{scalar_value(core::version::ros2,kind,true)};
        auto dynamic{client.call("param.set",kind+"dynamicset",scalar_bytes(changed),2500)};require(dynamic.error==95&&dynamic.payload.empty(),"remote scalar write refuses dynamically typed native parameter");
        require(node->get_parameter("scalar_dynamic_"+kind).get_parameter_value()==ros2_scalar_native(initial)&&dynamic_writes==0,"dynamic scalar refusal leaves exact native value unchanged without native setter callback");
        auto absent{client.call("param.set",kind+"absentset",scalar_bytes(changed),2500)};require(absent.error==95&&absent.payload.empty()&&!node->has_parameter("scalar_absent_"+kind),"remote scalar write refuses undeclared parameter without creating it");
        auto call=[&](std::string const& operation,std::string const& alias,std::vector<std::uint8_t> const& bytes,std::uint32_t error) {
            auto reply{client.call(operation,alias,bytes,2500)};require(reply.error==error&&(error||operation!="param.get"?reply.payload.empty():true),"local scalar exact error and bounded output");return reply;
        };
        auto retained=[&](ipc::parameter_value const& value) { require(call("param.get",kind+"localget",{},0).payload==scalar_bytes(value),"Gateway local native scalar kind/value retained"); };
        call("param.declare",kind+"localdeclare",scalar_bytes(initial),0);retained(initial);call("param.declare",kind+"localdeclare",scalar_bytes(changed),17);retained(initial);
        call("param.undeclare",kind+"wrongundeclare",{},95);retained(initial);
        if(kind!="bool") { call("param.undeclare",kind+"boundundeclare",{},13);retained(initial); }
        auto other{scalar_value(core::version::ros2,kind=="bool"?"int64":"bool",true)};call("param.set",kind+"wronglocalset",scalar_bytes(other),95);retained(initial);
        call("param.set",kind+"localset",scalar_bytes(changed),0);retained(changed);call("param.undeclare",kind+"localundeclare",{},0);call("param.get",kind+"localget",{},2);
    }
    client.close();std::cout<<"PASS ROS2 scalar static remote checks, dynamic/undeclared refusal without native setter mutation, typed local declare/get/set/undeclare, wrong-kind and out-of-range undeclare retains value\n";
}
