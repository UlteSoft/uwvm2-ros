#pragma once
#include <array>
#include <atomic>
#include <limits>
#include <string>
#include <thread>
#include <vector>
#include <fast_io.h>
#include "../ros_resources/common.h"
#include "../../../../../sdk/origin/common/codec.hpp"

// This fixture's policy and expected bytes are independent of the Gateway's
// projection helper. All aliases require explicit activation or mutation; the
// fixture does not infer authority from another parameter or event grant.
namespace local_event_probe
{
    using namespace ros_resources_probe;
    inline constexpr char gateway_name[]{"/local_parameter_events/gateway"};
    inline constexpr char namespace_name[]{"/local_parameter_events"};
    inline constexpr unsigned domain_id{203};
    inline constexpr std::array<char const*,9> kinds{{"bool","int64","float64","string","byte-array","bool-array","int64-array","float64-array","string-array"}};
    inline std::string name_of(std::string_view kind)
    { auto name{fast_io::concat_std("value_",kind)};std::replace(name.begin(),name.end(),'-','_');return name; }
    inline ipc::parameter_value sample(std::string_view kind,unsigned phase=0)
    {
        ipc::parameter_value value;value.type=ipc::parameter_type(core::version::ros2,fast_io::concat_std("builtin/",kind,"-v1"));
        value.bool_value=phase!=0;value.integer_value=phase?9007199254740993LL:-7LL;value.double_value=phase?1.25:-0.5;
        value.string_value=phase?"changed \xe4\xb8\xad":"before";
        value.byte_array_value={0,127,static_cast<std::uint8_t>(phase?255:128)};value.bool_array_value={0,1,static_cast<std::uint8_t>(phase!=0)};
        value.int64_array_value={-7,phase?9007199254740993LL:8LL};value.double_array_value={-0.0,phase?1.25:-0.5};
        value.string_array_value={"",phase?"\xe4\xb8\xad":"before"};return value;
    }
    inline void typed_limits(core::capability_spec& cap,ipc::parameter_kind kind)
    {
        if(ipc::array_parameter(kind)) { cap.limits["max-parameter-elements"]="8"; }
        if(ipc::float64_parameter(kind)) { cap.limits["parameter-min-value"]="-2";cap.limits["parameter-max-value"]="2";cap.limits["validator"]="finite"; }
        else if(ipc::numeric_parameter(kind)) { cap.limits["parameter-min-value"]="-9223372036854775808";cap.limits["parameter-max-value"]="9223372036854775807"; }
    }
    inline core::capability_spec parameter_cap(std::string_view kind,std::string alias,std::string operation,std::string name)
    {
        auto cap{resource(core::version::ros2,std::move(alias),std::move(operation),fast_io::concat_std(gateway_name,"/",name))};
        cap.type=fast_io::concat_std("builtin/",kind,"-v1");cap.type_identity=std::string{ipc::parameter_identity(core::version::ros2,cap.type)};
        cap.limits["parameter-node"]=gateway_name;cap.limits["parameter-name"]=std::move(name);cap.limits["max-parameter-bytes"]="8192";
        cap.limits["max-rate-hz"]="10000";cap.limits["max-bytes-per-second"]="1073741824";
        typed_limits(cap,ipc::parameter_type(core::version::ros2,cap.type));gateway::validate(core::version::ros2,cap);return cap;
    }
    inline core::capability_spec event_cap(std::string_view kind,std::string alias,std::string name)
    {
        auto cap{resource(core::version::ros2,std::move(alias),"param.subscribe-events",fast_io::concat_std(gateway_name,"/",name))};
        cap.type=fast_io::concat_std("builtin/param-event-",kind,"-v1");cap.type_identity=fast_io::concat_std("builtin:param-event-",kind,"-v1");
        cap.limits["parameter-node"]=gateway_name;cap.limits["parameter-name"]=std::move(name);cap.limits["max-parameter-bytes"]="8192";
        cap.limits["max-rate-hz"]="1000000";cap.limits["burst"]="32";cap.limits["max-bytes-per-second"]="1073741824";
        cap.limits.insert({{"max-subscribers","1"},{"max-event-fields","1"},{"max-event-queue-depth","32"},{"max-event-queue-bytes","262144"},
            {"overflow-policy","reject"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","32"},
            {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
        typed_limits(cap,ipc::parameter_event_type(core::version::ros2,cap.type));gateway::validate(core::version::ros2,cap);return cap;
    }
    inline std::vector<std::uint8_t> request(ipc::parameter_value const& value)
    { std::vector<std::uint8_t> bytes;require(!ipc::encode_parameter(value,bytes),"fixture value encoded");return bytes; }
    inline std::vector<std::uint8_t> expected(std::string const& name,ipc::parameter_value const& value,unsigned kind,unsigned seconds,unsigned nanoseconds,bool deletion_only=false)
    {
        uwvm_ros::typed_parameter_event event;event.seconds=seconds;event.nanoseconds=nanoseconds;event.kind=kind;event.node=gateway_name;event.name=name;
        event.tag=deletion_only?uwvm_ros::parameter_event_value_tag::deletion:static_cast<uwvm_ros::parameter_event_value_tag>(ipc::parameter_event_tag(value.type));
        event.bool_value=value.bool_value;event.integer_value=value.integer_value;event.double_value=value.double_value;event.string_value=value.string_value;
        event.byte_array_value=value.byte_array_value;event.bool_array_value=value.bool_array_value;event.integer_array_value=value.int64_array_value;
        event.double_array_value=value.double_array_value;event.string_array_value=value.string_array_value;return uwvm_ros::encode(event);
    }
    inline std::vector<std::uint8_t> receive(ipc::client& client,std::string const& alias,std::string const& name,ipc::parameter_value const& value,unsigned kind,bool deletion_only=false)
    {
        auto result{client.call("event.poll",alias,{},2000)};require(!result.error,"committed local event immediately available");
        uwvm_ros::typed_parameter_event decoded;require(uwvm_ros::decode(result.payload,decoded),"SDK decodes whole local event");
        require(result.payload==expected(name,value,kind,decoded.seconds,decoded.nanoseconds,deletion_only),"entire local event equals independent SDK oracle");
        auto duplicate{client.call("event.poll",alias,{},1000)};require(duplicate.error==11&&duplicate.payload.empty(),"one mutation produces precisely one local event");return result.payload;
    }
    inline void empty(ipc::client& client,std::string const& alias)
    { auto result{client.call("event.poll",alias,{},1000)};require(result.error==11&&result.payload.empty(),"unselected or rejected listener has no disclosure"); }
}
