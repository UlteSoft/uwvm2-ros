#pragma once
#include "array_witness_ros1.h"

inline void ros1_native_arrays(ros::NodeHandle& node,char const* executable)
{
    using namespace array_probe;
    for(auto const& kind:kinds(core::version::ros1))
    {
        for(std::string const profile:{"value","count","bytes","bound","invalid"})
        {
            if(profile=="bound"&&kind=="bool") { continue; }
            auto resident{value(core::version::ros1,kind)};
            if(profile=="invalid"&&kind=="float64") { resident=invalid(core::version::ros1,kind,"nonfinite"); }
            node.setParam("/uwvm_resources/array_"+profile+"_"+kind,xml(resident));
        }
        node.setParam("/uwvm_resources/array_wrong_"+kind,std::string{"hello"});
        node.setParam("/uwvm_resources/array_mixed_"+kind,mixed(kind));
        node.deleteParam("/uwvm_resources/array_created_"+kind);
    }
    fixture session{core::version::ros1,executable,[](gateway::configuration& config) {
        caps(config,core::version::ros1);
        for(auto const& kind:kinds(core::version::ros1)) { for(auto const* operation:{"get","set"}) { add(config,cap(core::version::ros1,kind+"create"+operation,std::string{"param."}+operation,"array_created_"+kind,kind)); } }
    }};ipc::client client;session.connect(client);
    auto native_read=[&](std::string const& kind,std::string const& profile) {
        XmlRpc::XmlRpcValue resident;require(node.getParam("/uwvm_resources/array_"+profile+"_"+kind,resident),"actual ROS1 array fixture resource exists");
        if(profile=="wrong") { require(resident.getType()==XmlRpc::XmlRpcValue::TypeString&&static_cast<std::string>(resident)=="hello","wrong native ROS1 kind remains scalar String"); }
        if(profile=="wrong"||profile=="mixed") { auto text{resident.toXml()};return std::vector<std::uint8_t>(text.begin(),text.end()); }
        return raw(from_xml(kind,resident));
    };
    roundtrips(client,core::version::ros1,native_read,[]{return 0u;});
    for(auto const& kind:kinds(core::version::ros1))
    {
        auto name{"/uwvm_resources/array_created_"+kind};require(!node.hasParam(name),"ROS1 typed array creation starts absent");
        auto request{client.call("param.set",kind+"createset",bytes(value(core::version::ros1,kind)),2500)};
        require(!request.error&&request.payload.empty(),"explicit ROS1 typed array set creates missing resource");
        XmlRpc::XmlRpcValue resident;require(node.getParam(name,resident)&&raw(from_xml(kind,resident))==raw(value(core::version::ros1,kind)),"actual ROS1 created Array and every element have exact selected native kind/value");
        auto fetched{client.call("param.get",kind+"createget",{},2500)};require(!fetched.error&&fetched.payload==bytes(value(core::version::ros1,kind)),"created ROS1 typed array canonical roundtrip exact");
        node.deleteParam(name);
    }
    client.close();
    for(auto const& kind:kinds(core::version::ros1)) { for(auto const* profile:{"value","wrong","count","bytes","mixed","bound","invalid"}) { node.deleteParam("/uwvm_resources/array_"+std::string{profile}+"_"+kind); } }
    std::cout<<"PASS ROS1 typed array creation and heterogeneous-element rejection; native empty Array has no element-kind metadata\n";
}
