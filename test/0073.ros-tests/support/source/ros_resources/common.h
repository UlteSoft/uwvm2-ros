#pragma once
#include "../ros_ipc/probe_common.h"
#include <uwvm2/imported/ros/ipc/parameter.h>
namespace ros_resources_probe
{
    using namespace ros_probe;
    inline core::capability_spec resource(core::version v,std::string alias,std::string operation,std::string name)
    {
        bool graph{operation.rfind("graph.",0)==0};
        core::capability_spec c{alias,operation,name,graph?"builtin/graph-list-v1":"builtin/string-v1",graph?"builtin:graph-list-v1":"builtin:param-string-v1",{}};
        c.limits={{"timeout-ms","2000"},{"max-inflight","2"},{"max-rate-hz","100"},{"burst","20"},{"max-bytes-per-second","1048576"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","20"},{"audit-max-records","1000"},{"audit-max-bytes","1048576"}};
        if(graph) { c.limits.emplace("max-graph-result-bytes","65536"); c.limits.emplace("allowed-name-prefix","/uwvm_resources"); }
        else
        {
            c.limits.emplace("max-parameter-bytes","65536"); c.limits.emplace("validator","none");
            if(operation=="param.search"||operation=="param.list") { c.limits.emplace("allowed-name-prefix","/uwvm_resources"); }
            if(v==core::version::ros2)
            {
                c.limits.emplace("parameter-node","/uwvm_resources/native"); c.limits.emplace("parameter-name",operation=="param.list"?"not-applicable":"value");
                if(operation=="param.list") { c.limits.emplace("list-depth","2"); }
                if(operation!="param.declare"&&operation!="param.undeclare"&&operation!="param.subscribe-events") { service_qos(c); }
            }
        }
        return c;
    }
    inline std::vector<std::uint8_t> string(std::string const& text)
    { std::vector<std::uint8_t> bytes; ipc::put(bytes,text.size(),4); bytes.insert(bytes.end(),text.begin(),text.end()); return bytes; }
    inline std::string text(ipc::call_result const& response)
    { require(!response.error&&response.payload.size()>=4&&ipc::get(response.payload.data(),4)==response.payload.size()-4,"canonical bounded resource response"); return std::string(reinterpret_cast<char const*>(response.payload.data()+4),response.payload.size()-4); }
    inline void add(gateway::configuration& config,core::capability_spec cap)
    { auto alias{cap.alias}; require(config.policy.capabilities.emplace(std::move(alias),std::move(cap)).second,"unique resource capability alias"); }
    inline ipc::parameter_kind scalar_kind(core::version v,std::string const& name)
    {
        auto kind{ipc::parameter_type(v,"builtin/"+name+"-v1")};
        require(kind==ipc::parameter_kind::boolean||ipc::numeric_parameter(kind),"supported native scalar fixture kind");return kind;
    }
    inline ipc::parameter_value scalar_value(core::version v,std::string const& name,bool changed=false)
    {
        ipc::parameter_value value;value.type=scalar_kind(v,name);
        value.bool_value=changed;value.integer_value=changed?(v==core::version::ros1?-123456789LL:9007199254740993LL):-7;
        value.double_value=changed?1.25:-0.5;return value;
    }
    inline std::vector<std::uint8_t> scalar_bytes(ipc::parameter_value const& value)
    { std::vector<std::uint8_t> bytes;require(!ipc::encode_parameter(value,bytes),"encode native scalar fixture");return bytes; }
    inline core::capability_spec scalar_cap(core::version v,std::string alias,std::string operation,std::string name,std::string const& kind)
    {
        auto cap{resource(v,std::move(alias),std::move(operation),std::move(name))};cap.type="builtin/"+kind+"-v1";cap.type_identity=std::string{ipc::parameter_identity(v,cap.type)};
        cap.limits["max-parameter-bytes"]=std::to_string(ipc::parameter_width(scalar_kind(v,kind)));
        if(kind=="int32") { cap.limits["parameter-min-value"]="-2147483648";cap.limits["parameter-max-value"]="2147483647"; }
        if(kind=="int64") { cap.limits["parameter-min-value"]="-9223372036854775808";cap.limits["parameter-max-value"]="9223372036854775807"; }
        if(kind=="float64") { cap.limits["parameter-min-value"]="-1.7976931348623157e308";cap.limits["parameter-max-value"]="1.7976931348623157e308";cap.limits["validator"]="finite"; }
        return cap;
    }
}
