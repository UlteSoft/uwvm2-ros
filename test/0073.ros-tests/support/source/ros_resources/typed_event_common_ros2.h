#pragma once
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <rcl_interfaces/msg/parameter_type.hpp>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
#include <algorithm>
#include <cmath>
#include <limits>

namespace typed_event_probe
{
    inline std::string queue_name(std::string const& kind,std::string const& overflow)
    {
        auto name{"queue_"+kind+"_"+overflow};std::replace(name.begin(),name.end(),'-','_');return name;
    }
    inline ipc::parameter_value value(std::string const& kind,unsigned phase)
    {
        ipc::parameter_value result;result.type=scalar_kind(core::version::ros2,kind);
        result.bool_value=phase!=0;
        result.integer_value=phase==0?std::numeric_limits<std::int64_t>::min():
                             phase==1?9007199254740993LL:std::numeric_limits<std::int64_t>::max();
        result.double_value=phase==0?-0.0:phase==1?std::nextafter(1.0,2.0):1.25;
        return result;
    }
    inline rcl_interfaces::msg::Parameter native(std::string const& name,ipc::parameter_value const& value)
    {
        rcl_interfaces::msg::Parameter parameter;parameter.name=name;
        if(value.type==ipc::parameter_kind::boolean)
        { parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_BOOL;parameter.value.bool_value=value.bool_value; }
        else if(value.type==ipc::parameter_kind::int64)
        { parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER;parameter.value.integer_value=value.integer_value; }
        else if(value.type==ipc::parameter_kind::float64)
        { parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;parameter.value.double_value=value.double_value; }
        else { throw std::runtime_error{"unsupported native typed-event fixture value"}; }
        return parameter;
    }
    inline rcl_interfaces::msg::ParameterEvent event(std::string const& name,ipc::parameter_value const& value,
                                                   std::uint8_t kind,std::uint32_t nanoseconds)
    {
        rcl_interfaces::msg::ParameterEvent result;result.node="/uwvm_resources/native";
        result.stamp.sec=123;result.stamp.nanosec=nanoseconds;
        auto parameter{native(name,value)};
        (kind==1?result.new_parameters:kind==2?result.changed_parameters:result.deleted_parameters).push_back(std::move(parameter));
        return result;
    }
    inline std::vector<std::uint8_t> bytes(std::string const& name,ipc::parameter_value const& value,
                                         std::uint8_t kind,std::uint32_t nanoseconds)
    {
        std::vector<std::uint8_t> result;
        require(!ipc::encode_typed_parameter_event({123,nanoseconds,kind,"/uwvm_resources/native",name,value},result),
                "encode exact native typed-event fixture");return result;
    }
    inline core::capability_spec capability(std::string const& kind,std::string alias,std::string name="value")
    {
        auto cap{resource(core::version::ros2,std::move(alias),"param.subscribe-events","/uwvm_resources/native/"+name)};
        cap.type="builtin/param-event-"+kind+"-v1";cap.type_identity=std::string{ipc::parameter_event_identity(core::version::ros2,cap.type)};
        cap.limits["parameter-name"]=std::move(name);cap.limits["max-rate-hz"]="1000";cap.limits["burst"]="8";
        cap.limits["max-parameter-bytes"]="4096";
        cap.limits.insert({{"max-subscribers","1"},{"max-event-fields","1"},{"max-event-queue-depth","8"},
            {"max-event-queue-bytes","32768"},{"overflow-policy","drop-oldest"},{"qos.reliability","reliable"},
            {"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","32"},
            {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},
            {"qos.liveliness-lease-duration","infinite"}});
        if(kind=="int64") { cap.limits["parameter-min-value"]="-9223372036854775808";cap.limits["parameter-max-value"]="9223372036854775807"; }
        if(kind=="float64") { cap.limits["parameter-min-value"]="-2";cap.limits["parameter-max-value"]="2";cap.limits["validator"]="finite"; }
        return cap;
    }
}
