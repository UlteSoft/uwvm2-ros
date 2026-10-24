#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
using namespace ros_resources_probe;
#include "witness_ros2.h"
#include "scalar_native_ros2.h"
#include "typed_event_native_ros2.h"
#include "array_native_ros2.h"
int main(int argc,char** argv)
{
    try
    {
        if(argc>=2&&std::string{argv[1]}.rfind("--",0)==0) { return ros2_resource_witness(argc,argv); }
        require(argc==2,"usage native-ros2-resources gateway"); ::setenv("ROS_DOMAIN_ID","75",1); ::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1); ::setenv("ROS_SECURITY_ENABLE","false",1);
        rclcpp::init(argc,argv); auto node{std::make_shared<rclcpp::Node>("native","/uwvm_resources")}; node->declare_parameter<std::string>("value","before"); node->declare_parameter<std::string>("other","hidden");
        auto pub{node->create_publisher<std_msgs::msg::String>("/uwvm_resources/topic",rclcpp::QoS{2})};
        auto outside{node->create_publisher<std_msgs::msg::String>("/uwvm_resources_bad/secret",rclcpp::QoS{2})};
        rclcpp::executors::SingleThreadedExecutor executor; executor.add_node(node); std::thread spinner{[&] { executor.spin(); }};
        try
        {
            fixture f{core::version::ros2,argv[1],[](gateway::configuration& cfg) {
                cfg.policy.connection["domain-id"]="75";
                cfg.policy.capabilities.clear();
                for(auto op: {"get","set","describe"}) { auto cap{resource(core::version::ros2,op,std::string{"param."}+op,"/uwvm_resources/native/value")};cap.limits["qos.depth"]="6";add(cfg,std::move(cap)); }
                auto list{resource(core::version::ros2,"list","param.list","/uwvm_resources/native")}; list.limits["allowed-name-prefix"]="/uwvm_resources/native/value";list.limits["qos.depth"]="6"; add(cfg,std::move(list));
                for(auto name: {"topics","nodes","services"}) { add(cfg,resource(core::version::ros2,name,std::string{"graph.list-"}+name,"/uwvm_resources")); }
                add(cfg,resource(core::version::ros2,"inspectTopic","graph.inspect-topic","/uwvm_resources/topic"));
                add(cfg,resource(core::version::ros2,"inspectNode","graph.inspect-node","/uwvm_resources/native"));
                add(cfg,resource(core::version::ros2,"inspectService","graph.inspect-service","/uwvm_resources/native/get_parameters"));
                auto events{resource(core::version::ros2,"events","param.subscribe-events","/uwvm_resources/native/value")};
                events.type="rcl_interfaces/msg/ParameterEvent";events.type_identity="RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204";
                events.limits.insert({{"max-subscribers","1"},{"max-event-fields","8"},{"max-event-queue-depth","2"},{"max-event-queue-bytes","131072"},{"overflow-policy","drop-oldest"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","32"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
                add(cfg,std::move(events));
                for(auto op: {"declare","undeclare"})
                { auto cap{resource(core::version::ros2,op,std::string{"param."}+op,"/uwvm_test/uwvm_ros2_gateway_probe/value")}; cap.limits["parameter-node"]="/uwvm_test/uwvm_ros2_gateway_probe"; add(cfg,std::move(cap)); }
            }};
            ipc::client client; f.connect(client);
            require(text(client.call("param.get","get",{},2500))=="before","real ROS2 native parameter service get");
            require(!client.call("param.set","set",string("after"),2500).error,"ROS2 string parameter set request accepted"); require(node->get_parameter("value").as_string()=="after","native ROS2 witness confirms actual parameter side effect");
            require(client.call("param.set","get",string("unauthorized"),2500).error==13,"get cannot mutate ROS2 parameter"); require(node->get_parameter("value").as_string()=="after","unauthorized operation causes no real ROS2 side effect");
            require(client.call("param.set","absent",string("unauthorized"),2500).error==13,"unknown ROS2 parameter alias denied"); require(node->get_parameter("value").as_string()=="after","missing grant causes no real parameter side effect");
            auto descriptor{text(client.call("param.describe","describe",{},2500))}; require(descriptor.find("name=value\n")!=std::string::npos&&descriptor.find("type=4\n")!=std::string::npos,"actual ROS2 descriptor returned for exact named string parameter");
            auto names{text(client.call("param.list","list",{},2500))}; require(names=="/uwvm_resources/native/value\n","ROS2 parameter list applies exact resource and explicit prefix filtering");
            bool discovered{};
            for(unsigned n{};n!=100;++n)
            {
                auto topics{text(client.call("graph.list-topics","topics",{},2500))};
                require(topics.find("uwvm_resources_bad")==std::string::npos,"DDS graph excludes neighboring prefix namespace");
                if(topics.find("/uwvm_resources/topic\tstd_msgs/msg/String\n")!=std::string::npos) { discovered=true; break; }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            require(discovered,"actual DDS graph topic/type discovered");
            auto graph_ready=[&](std::string const& operation,std::string const& alias,std::string const& row) {
                for(unsigned n{};n!=100;++n) { auto result{client.call(operation,alias,{},2500)}; if(!result.error&&text(result).find(row)!=std::string::npos) { return true; } std::this_thread::sleep_for(std::chrono::milliseconds(20)); } return false;
            };
            require(graph_ready("graph.list-nodes","nodes","/uwvm_resources/native\n"),"native DDS node graph");
            require(graph_ready("graph.list-services","services","/uwvm_resources/native/get_parameters\trcl_interfaces/srv/GetParameters\n"),"actual DDS graph generated parameter service type");
            require(text(client.call("graph.inspect-topic","inspectTopic",{},2500))=="/uwvm_resources/topic\tstd_msgs/msg/String\n","DDS inspect topic exact resource and type");
            require(text(client.call("graph.inspect-node","inspectNode",{},2500))=="/uwvm_resources/native\n","DDS inspect node exact resource");
            require(text(client.call("graph.inspect-service","inspectService",{},2500))=="/uwvm_resources/native/get_parameters\trcl_interfaces/srv/GetParameters\n","DDS inspect service exact generated type");
            require(client.call("param.subscribe-events","absent",{},2500).error==13,"unknown event subscription has no authority");
            require(!client.call("param.subscribe-events","events",{},2500).error,"explicit filtered ParameterEvent subscription created");
            bool received{};
            for(unsigned n{};n!=100;++n)
            {
                node->set_parameter(rclcpp::Parameter{"value","selected event"});std::this_thread::sleep_for(std::chrono::milliseconds(20));
                auto event{client.call("event.poll","events",{},2500)};
                if(!event.error) { ipc::parameter_event parsed; require(ipc::decode_parameter_event(event.payload,parsed)&&parsed.node=="/uwvm_resources/native"&&parsed.name=="value"&&parsed.value=="selected event"&&parsed.kind==2,"actual selected canonical ParameterEvent content");received=true;break; }
                require(event.error==11,"empty ParameterEvent queue returns bounded EAGAIN");
            }
            require(received,"real ParameterEvent delivered after native mutation");
            while(!client.call("event.poll","events",{},2500).error) {}
            node->set_parameter(rclcpp::Parameter{"other","hidden event"});std::this_thread::sleep_for(std::chrono::milliseconds(100));
            require(client.call("event.poll","events",{},2500).error==11,"neighbor parameter filtered before guest event queue");
            require(!client.call("capability.revoke","events",{},1000).error,"ParameterEvent subscription revoked");
            node->set_parameter(rclcpp::Parameter{"value","revoked event"});std::this_thread::sleep_for(std::chrono::milliseconds(50));
            require(client.call("event.poll","events",{},2500).error==125,"revoked event queue cannot deliver subsequent native events");
            require(!client.call("param.declare","declare",string("local value"),2500).error,"explicit native gateway parameter declaration"); require(client.call("param.declare","declare",string("again"),2500).error==17,"duplicate declaration refused");
            require(!client.call("param.undeclare","undeclare",{},2500).error,"independently granted native gateway undeclare"); require(client.call("param.undeclare","undeclare",{},2500).error==2,"undeclared parameter no longer exists");
            {
                fixture denied{core::version::ros2,argv[1],[](gateway::configuration& cfg){cfg.policy.connection["domain-id"]="75";cfg.policy.connection["node-name"]="uwvm_incompatible_parameter_qos";cfg.policy.capabilities.clear();auto cap{resource(core::version::ros2,"set","param.set","/uwvm_resources/native/value")};cap.limits["qos.reliability"]="best-effort";cap.limits["timeout-ms"]="300";add(cfg,std::move(cap));}};
                ipc::client request;denied.connect(request);auto before{node->get_parameter("value").as_string()};auto reply{request.call("param.set","set",string("qos-denied"),1000)};
                require(reply.error==110&&reply.payload.empty(),"explicit incompatible parameter request QoS times out without response");std::this_thread::sleep_for(std::chrono::milliseconds(100));require(node->get_parameter("value").as_string()==before,"incompatible parameter QoS leaves actual native value unchanged");request.close();
            }
            require(!client.call("capability.revoke","get",{},1000).error,"get revoked"); require(client.call("param.get","get",{},2500).error==125,"revoked ROS2 get delivers no sensitive value");
            client.close();ros2_native_scalars(node,argv[1]);ros2_native_typed_events(node,argv[1]);ros2_native_arrays(node,argv[1]);std::cout<<"PASS actual ROS2 remote get/set/describe/list with explicit depth6 and request/response eight-field actual QoS checks, incompatible QoS no parameter mutation, explicit local declare/undeclare without DDS, filtered DDS graph and denied writes\n";
        }
        catch(...) { executor.cancel(); spinner.join(); rclcpp::shutdown(); throw; }
        executor.cancel(); spinner.join(); rclcpp::shutdown(); return 0;
    }
    catch(std::exception const& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
