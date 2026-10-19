#pragma once
#include "scalar_native_common.h"
inline void ros1_native_scalars(ros::NodeHandle& node,char const* executable)
{
    for(auto const& kind:native_scalar_kinds(core::version::ros1))
    {
        auto initial{scalar_value(core::version::ros1,kind)};auto name{"/uwvm_resources/scalar_"+kind};
        if(kind=="bool") { node.setParam(name,initial.bool_value); }
        else if(kind=="int32") { node.setParam(name,static_cast<int>(initial.integer_value)); }
        else { node.setParam(name,initial.double_value); }
    }
    node.setParam("/uwvm_resources/scalar_wrong",std::string{"hello"});
    for(auto const& kind:native_scalar_kinds(core::version::ros1)) { node.deleteParam("/uwvm_resources/scalar_created_"+kind); }
    fixture f{core::version::ros1,executable,[](gateway::configuration& cfg){
        native_scalar_caps(cfg,core::version::ros1);
        for(auto const& kind:native_scalar_kinds(core::version::ros1)) { for(auto const& op:{"get","set"}) { add(cfg,native_scalar_cap(core::version::ros1,kind+"create"+op,std::string{"param."}+op,"scalar_created_"+kind,kind)); } }
    }};ipc::client client;f.connect(client);
    auto read=[&](std::string const& kind) {
        XmlRpc::XmlRpcValue native;require(node.getParam("/uwvm_resources/scalar_"+kind,native),"ROS1 native scalar exists");auto value{scalar_value(core::version::ros1,kind)};
        if(kind=="bool") { require(native.getType()==XmlRpc::XmlRpcValue::TypeBoolean,"ROS1 exact Boolean type");value.bool_value=static_cast<bool>(native); }
        else if(kind=="int32") { require(native.getType()==XmlRpc::XmlRpcValue::TypeInt,"ROS1 exact signed int32 type");value.integer_value=static_cast<int>(native); }
        else { require(native.getType()==XmlRpc::XmlRpcValue::TypeDouble,"ROS1 exact double type");value.double_value=static_cast<double>(native); }
        return scalar_bytes(value);
    };
    native_scalar_roundtrips(client,core::version::ros1,read,[&]{XmlRpc::XmlRpcValue native;require(node.getParam("/uwvm_resources/scalar_wrong",native)&&native.getType()==XmlRpc::XmlRpcValue::TypeString&&static_cast<std::string>(native)=="hello","ROS1 wrong-type scalar denial leaves native String unchanged");});
    for(auto const& kind:native_scalar_kinds(core::version::ros1))
    {
        auto name{"/uwvm_resources/scalar_created_"+kind};require(!node.hasParam(name),"explicit ROS1 scalar creation resource initially absent");auto value{scalar_value(core::version::ros1,kind,true)};
        auto written{client.call("param.set",kind+"createset",scalar_bytes(value),2500)};require(!written.error&&written.payload.empty(),"typed ROS1 set creates missing parameter");
        auto reply{client.call("param.get",kind+"createget",{},2500)};require(!reply.error&&reply.payload==scalar_bytes(value),"created ROS1 scalar canonical roundtrip exact");
        XmlRpc::XmlRpcValue native;require(node.getParam(name,native),"native Master now contains explicitly created scalar");
        if(kind=="bool") { require(native.getType()==XmlRpc::XmlRpcValue::TypeBoolean&&static_cast<bool>(native)==value.bool_value,"created native Bool exact"); }
        else if(kind=="int32") { require(native.getType()==XmlRpc::XmlRpcValue::TypeInt&&static_cast<int>(native)==value.integer_value,"created native int32 exact"); }
        else { require(native.getType()==XmlRpc::XmlRpcValue::TypeDouble&&static_cast<double>(native)==value.double_value,"created native float64 exact"); }
        node.deleteParam(name);
    }
    client.close();for(auto const& kind:native_scalar_kinds(core::version::ros1)) { node.deleteParam("/uwvm_resources/scalar_"+kind); }node.deleteParam("/uwvm_resources/scalar_wrong");
}
