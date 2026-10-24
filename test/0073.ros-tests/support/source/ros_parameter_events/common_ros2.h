#pragma once
#include <rclcpp/rclcpp.hpp>
#include <fast_io.h>
#include "../ros_resources/common.h"
using namespace ros_resources_probe;
#include "../ros_resources/typed_event_witness_ros2.h"
#include "../../../../../sdk/origin/common/codec.hpp"

namespace expanded_event_probe {
inline constexpr char node_name[]{"/typed_events/native"};
inline constexpr std::array<char const*,6> kinds{{"string","byte-array","bool-array","int64-array","float64-array","string-array"}};
inline unsigned tag(std::string_view kind) {
    for(unsigned i=0;i<kinds.size();++i) if(kind==kinds[i]) return i+4;
    throw std::runtime_error{"unsupported expanded event fixture kind"};
}
inline ipc::parameter_value value(std::string_view kind,unsigned phase=1) {
    ipc::parameter_value result;result.type=ipc::parameter_event_kind(tag(kind));
    if(!phase) return result;
    // Canonical String permits NUL; FastCDR rejects it on this native transport.
    // The SDK unit and the separate native NUL diagnostic retain that boundary.
    result.string_value="\xe4\xb8\xadvalue";
    result.byte_array_value={0,127,128,255};result.bool_array_value={0,1,0};
    result.int64_array_value={INT64_MIN,9007199254740993LL,INT64_MAX};
    result.double_array_value={-0.0,std::nextafter(1.,2.),std::numeric_limits<double>::min()};
    result.string_array_value={"","\xe4\xb8\xad","tail"};if(phase==2) result.int64_array_value={9007199254740993LL};return result;
}
inline rclcpp::ParameterValue native(ipc::parameter_value const& value) {
    switch(value.type) {
    case ipc::parameter_kind::string:return rclcpp::ParameterValue{value.string_value};
    case ipc::parameter_kind::byte_array:return rclcpp::ParameterValue{value.byte_array_value};
    case ipc::parameter_kind::boolean_array: {
        std::vector<bool> result;for(auto item:value.bool_array_value) { require(item<=1,"native BoolArray fixture canonical");result.push_back(item!=0); }return rclcpp::ParameterValue{result};
    }
    case ipc::parameter_kind::int64_array:return rclcpp::ParameterValue{value.int64_array_value};
    case ipc::parameter_kind::float64_array:return rclcpp::ParameterValue{value.double_array_value};
    case ipc::parameter_kind::string_array:return rclcpp::ParameterValue{value.string_array_value};
    default:throw std::runtime_error{"unsupported expanded native value"};
    }
}
inline rcl_interfaces::msg::ParameterEvent event(std::string const& name,ipc::parameter_value const& value,unsigned kind,unsigned stamp) {
    rcl_interfaces::msg::ParameterEvent result;result.node=node_name;result.stamp.sec=123;result.stamp.nanosec=stamp;
    rcl_interfaces::msg::Parameter parameter;parameter.name=name;parameter.value=native(value).to_value_msg();
    require(parameter.value.type==ipc::parameter_event_tag(value.type),"native ROS2 ParameterType agrees with closed wire tag");
    (kind==1?result.new_parameters:kind==2?result.changed_parameters:result.deleted_parameters).push_back(std::move(parameter));return result;
}
inline std::vector<std::uint8_t> bytes(std::string const& name,ipc::parameter_value const& value,unsigned kind,unsigned stamp) {
    // An independently implemented SDK encoder supplies the expected envelope.
    uwvm_ros::typed_parameter_event expected;expected.seconds=123;expected.nanoseconds=stamp;expected.kind=kind;
    expected.node=node_name;expected.name=name;expected.tag=static_cast<uwvm_ros::parameter_event_value_tag>(ipc::parameter_event_tag(value.type));
    expected.string_value=value.string_value;expected.byte_array_value=value.byte_array_value;expected.bool_array_value=value.bool_array_value;
    expected.integer_array_value=value.int64_array_value;expected.double_array_value=value.double_array_value;expected.string_array_value=value.string_array_value;
    auto result=uwvm_ros::encode(expected);require(!result.empty(),"independent SDK exact expanded event fixture");return result;
}
inline core::capability_spec capability(std::string const& kind,std::string alias,std::string name) {
    auto result=resource(core::version::ros2,std::move(alias),"param.subscribe-events",fast_io::concat_std(node_name,"/",name));
    result.type=fast_io::concat_std("builtin/param-event-",kind,"-v1");result.type_identity=fast_io::concat_std("builtin:param-event-",kind,"-v1");
    result.limits["parameter-node"]=node_name;result.limits["parameter-name"]=std::move(name);
    result.limits["max-rate-hz"]="1000";result.limits["burst"]="32";result.limits["max-parameter-bytes"]="4096";
    result.limits.insert({{"max-subscribers","1"},{"max-event-fields","1"},{"max-event-queue-depth","8"},
        {"max-event-queue-bytes","32768"},{"overflow-policy","reject"},{"qos.reliability","reliable"},
        {"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","32"},
        {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
    if(kind!="string") result.limits["max-parameter-elements"]="4";
    if(kind=="int64-array") { result.limits["parameter-min-value"]="-9223372036854775808";result.limits["parameter-max-value"]="9223372036854775807"; }
    if(kind=="float64-array") { result.limits["parameter-min-value"]="-2";result.limits["parameter-max-value"]="2";result.limits["validator"]="finite"; }
    return result;
}
inline std::vector<std::string> scenarios(std::string_view kind) {
    std::vector<std::string> result{"wrong-node","wrong-name","wrong-kind","not-set-delete","stamp","fields"};
    if(kind!="string") result.push_back("elements");
    if(kind=="string"||kind=="string-array") result.push_back("utf8");
    if(kind=="int64-array"||kind=="float64-array") result.push_back("bounds");
    if(kind=="float64-array") result.push_back("nonfinite");return result;
}
inline rcl_interfaces::msg::ParameterEvent rejected(std::string const& kind,std::string const& name,std::string const& scenario,unsigned event_kind=2) {
    auto selected=value(kind);auto result=event(name,selected,scenario=="not-set-delete"?3:event_kind,80);
    auto& parameter=(result.new_parameters.empty()?(result.changed_parameters.empty()?result.deleted_parameters:result.changed_parameters):result.new_parameters).front();
    if(scenario=="wrong-node") result.node="/typed_events/other";
    else if(scenario=="wrong-name") parameter.name="unselected";
    else if(scenario=="wrong-kind") { parameter.value.type=1;parameter.value.bool_value=true; }
    else if(scenario=="not-set-delete") parameter.value.type=0;
    else if(scenario=="stamp") result.stamp.nanosec=1000000000;
    else if(scenario=="fields") result.new_parameters.push_back(event("unselected",selected,1,80).new_parameters.front());
    else if(scenario=="utf8") { parameter.value.string_value="\x80";parameter.value.string_array_value={"valid","\x80"}; }
    else if(scenario=="nul") parameter.value.string_array_value={"valid",std::string{"a\0b",3}};
    else if(scenario=="elements") {
        parameter.value.byte_array_value={0,1,2,3,4};parameter.value.bool_array_value={false,true,false,true,false};
        parameter.value.integer_array_value={0,1,2,3,4};parameter.value.double_array_value={0,1,0,1,0};parameter.value.string_array_value={"","","","",""};
    }
    else if(scenario=="bounds") { parameter.value.integer_array_value={0,9007199254740996LL};parameter.value.double_array_value={0.5,3.}; }
    else if(scenario=="nonfinite") parameter.value.double_array_value={0.5,std::numeric_limits<double>::infinity()};
    else throw std::runtime_error{"unknown expanded event rejection scenario"};return result;
}
}
