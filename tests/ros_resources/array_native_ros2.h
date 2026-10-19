#pragma once
#include "array_witness_ros2.h"

inline void ros2_native_arrays(rclcpp::Node::SharedPtr const& node,char const* executable)
{
    using namespace array_probe;
    for(auto const& kind:kinds(core::version::ros2))
    {
        for(std::string const profile:{"value","count","bytes","bound","invalid"})
        {
            if(profile=="bound"&&(kind=="bool"||kind=="string"||kind=="byte")) { continue; }
            auto resident{value(core::version::ros2,kind)};
            if(profile=="invalid"&&kind=="float64") { resident=invalid(core::version::ros2,kind,"nonfinite"); }
            if(profile=="invalid"&&kind=="string") { resident.string_array_value={std::string{"\xc0\xaf",2}}; }
            node->declare_parameter("array_"+profile+"_"+kind,native(resident));
        }
        node->declare_parameter("array_wrong_"+kind,std::string{"hello"});
        rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.dynamic_typing=true;
        node->declare_parameter("array_dynamic_"+kind,native(value(core::version::ros2,kind)),descriptor);
    }
    auto writes{std::make_shared<std::atomic<unsigned>>()};
    auto callbacks{node->add_on_set_parameters_callback([writes](std::vector<rclcpp::Parameter> const& parameters) {
        for(auto const& parameter:parameters) { if(parameter.get_name().rfind("array_",0)==0) { ++*writes; } }
        rcl_interfaces::msg::SetParametersResult reply;reply.successful=true;return reply;
    })};
    fixture session{core::version::ros2,executable,[](gateway::configuration& config) {
        caps(config,core::version::ros2);
        auto add_local=[&](core::capability_spec c) {
            if(c.operation=="param.declare"||c.operation=="param.undeclare")
            {
                for(auto it{c.limits.begin()};it!=c.limits.end();)
                { if(it->first.rfind("qos.",0)==0) { it=c.limits.erase(it); }else { ++it; } }
            }
            add(config,std::move(c));
        };
        for(auto const& kind:kinds(core::version::ros2))
        {
            add(config,cap(core::version::ros2,kind+"dynamicset","param.set","array_dynamic_"+kind,kind));
            add(config,cap(core::version::ros2,kind+"absentset","param.set","array_absent_"+kind,kind));
            for(std::string const profile:{"value","count","bytes","bound"})
            {
                if(profile=="bound"&&(kind=="bool"||kind=="string"||kind=="byte")) { continue; }
                for(auto const* operation:{"declare","get","set","undeclare"})
                {
                    auto name{"array_local_"+profile+"_"+kind};auto c{cap(core::version::ros2,kind+"local"+profile+operation,std::string{"param."}+operation,name,kind)};
                    c.resource="/uwvm_test/uwvm_array_gateway/"+name;c.limits["parameter-node"]="/uwvm_test/uwvm_array_gateway";
                    if(std::string{operation}=="undeclare")
                    {
                        if(profile=="count") { c.limits["max-parameter-elements"]="1"; }
                        if(profile=="bytes") { c.limits["max-parameter-bytes"]="4"; }
                        if(profile=="bound") { c.limits["parameter-min-value"]="-1";c.limits["parameter-max-value"]="1"; }
                    }
                    add_local(std::move(c));
                }
            }
            for(auto const* operation:{"set","undeclare"})
            {
                auto other{kind=="bool"?"int64":"bool"};auto name{"array_local_value_"+kind};auto c{cap(core::version::ros2,kind+"localwrong"+operation,std::string{"param."}+operation,name,other)};
                c.resource="/uwvm_test/uwvm_array_gateway/"+name;c.limits["parameter-node"]="/uwvm_test/uwvm_array_gateway";add_local(std::move(c));
            }
        }
    }};ipc::client client;session.connect(client);
    auto native_read=[&](std::string const& kind,std::string const& profile) {
        auto resident{node->get_parameter("array_"+profile+"_"+kind)};
        if(profile=="wrong") { require(resident.get_type()==rclcpp::ParameterType::PARAMETER_STRING&&resident.as_string()=="hello","wrong native array kind remains scalar String");return string(resident.as_string()); }
        return raw(from_native(kind,resident));
    };
    roundtrips(client,core::version::ros2,native_read,[writes]{return writes->load();});
    for(auto const& kind:kinds(core::version::ros2))
    {
        auto before{writes->load()};auto request{bytes(value(core::version::ros2,kind))};
        expect(client,"param.set",kind+"dynamicset",request,95);require(raw(from_native(kind,node->get_parameter("array_dynamic_"+kind)))==raw(value(core::version::ros2,kind))&&*writes==before,"dynamic array refusal preserves native exact value and setter callback count");
        expect(client,"param.set",kind+"absentset",request,95);require(!node->has_parameter("array_absent_"+kind)&&*writes==before,"absent array refusal creates no parameter or native setter callback");
        for(std::string const profile:{"value","count","bytes","bound"})
        {
            if(profile=="bound"&&(kind=="bool"||kind=="string"||kind=="byte")) { continue; }
            auto alias{kind+"local"+profile};
            auto call=[&](std::string const& operation,std::vector<std::uint8_t> const& input) { auto reply{client.call("param."+operation,alias+operation,input,2500)};require(!reply.error&&(operation=="get"||reply.payload.empty()),"actual Gateway-local array operation succeeds with bounded output");return reply; };
            call("declare",request);require(call("get",{}).payload==request,"Gateway-local native array declaration exact resident type/elements");
            expect(client,"param.declare",alias+"declare",bytes(value(core::version::ros2,kind,true)),17);
            if(profile=="value")
            {
                expect(client,"param.undeclare",kind+"localwrongundeclare",{},95);
                expect(client,"param.set",kind+"localwrongset",bytes(value(core::version::ros2,kind=="bool"?"int64":"bool")),95);
            }
            else { expect(client,"param.undeclare",alias+"undeclare",{},profile=="bound"?13:90); }
            require(call("get",{}).payload==request,"failed typed local undeclare/set leaves exact native array resident");
            call("set",bytes(value(core::version::ros2,kind,true)));require(call("get",{}).payload==bytes(value(core::version::ros2,kind,true)),"Gateway-local empty typed array retains selected native kind");
            call("undeclare",{});expect(client,"param.get",alias+"get",{},2);
        }
    }
    client.close();
    std::cout<<"PASS ROS2 official array static remote preconditions, dynamic/absent refusal without setter callback, typed local declare/get/set/undeclare and count/byte/range/type guards retain denied resident values\n";
}
