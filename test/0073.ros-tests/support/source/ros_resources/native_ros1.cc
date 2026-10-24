#include "common.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_srvs/Trigger.h>
using namespace ros_resources_probe;
#include "witness_ros1.h"
#include "scalar_native_ros1.h"
#include "array_native_ros1.h"
int main(int argc,char** argv)
{
    try
    {
        if(argc>=2&&std::string{argv[1]}.rfind("--",0)==0) { return ros1_resource_witness(argc,argv); }
        require(argc==2,"usage native-ros1-resources gateway");
        ros::M_string remapping; remapping["__ns"]="/uwvm_resources";
        ros::init(remapping,"native",ros::init_options::NoSigintHandler|ros::init_options::NoRosout); ros::NodeHandle node; ros::AsyncSpinner spinner{2}; spinner.start();
        node.setParam("/uwvm_resources/value",std::string{"before"}); node.setParam("/uwvm_resources_bad/secret",std::string{"hidden"});
        auto pub{node.advertise<std_msgs::String>("/uwvm_resources/topic",2,false)};
        auto outside{node.advertise<std_msgs::String>("/uwvm_resources_bad/secret",2,false)};
        boost::function<bool(std_srvs::Trigger::Request&,std_srvs::Trigger::Response&)> callback{[](auto&,auto& response) { response.success=true; return true; }};
        auto service{node.advertiseService("/uwvm_resources/trigger",callback)};
        fixture f{core::version::ros1,argv[1],[](gateway::configuration& cfg) {
            cfg.policy.capabilities.clear();
            for(auto op: {"get","set","delete","exists","search"}) { add(cfg,resource(core::version::ros1,op,std::string{"param."}+op,"/uwvm_resources/value")); }
            add(cfg,resource(core::version::ros1,"list","param.list","/uwvm_resources"));
            for(auto name: {"topics","nodes","services"}) { add(cfg,resource(core::version::ros1,name,std::string{"graph.list-"}+name,"/uwvm_resources")); }
            add(cfg,resource(core::version::ros1,"inspectTopic","graph.inspect-topic","/uwvm_resources/topic"));
            add(cfg,resource(core::version::ros1,"inspectNode","graph.inspect-node","/uwvm_resources/native"));
            add(cfg,resource(core::version::ros1,"inspectService","graph.inspect-service","/uwvm_resources/trigger"));
        }};
        ipc::client client; f.connect(client);
        require(text(client.call("param.get","get",{},2500))=="before","actual ROS1 Master get exact string");
        require(!client.call("param.set","set",string("after"),2500).error,"ROS1 set request accepted"); std::string actual; require(node.getParam("/uwvm_resources/value",actual)&&actual=="after","native ROS1 witness observes set side effect");
        require(client.call("param.set","get",string("unauthorized"),2500).error==13,"get capability cannot grant set"); require(node.getParam("/uwvm_resources/value",actual)&&actual=="after","operation substitution has no actual parameter side effect");
        require(client.call("param.set","missing",string("unauthorized"),2500).error==13,"missing parameter capability denied"); require(node.getParam("/uwvm_resources/value",actual)&&actual=="after","missing authorization has no actual parameter side effect");
        auto exists{client.call("param.exists","exists",{},2500)}; require(!exists.error&&exists.payload==std::vector<std::uint8_t>{1},"exists is separate exact resource authorization");
        require(text(client.call("param.search","search",{},2500))=="/uwvm_resources/value","search returns only explicitly granted fully qualified parameter");
        auto names{text(client.call("param.list","list",{},2500))}; require(names.find("/uwvm_resources/value\n")!=std::string::npos&&names.find("uwvm_resources_bad")==std::string::npos,"parameter list respects segment prefix and hides neighboring namespace");
        auto topics{text(client.call("graph.list-topics","topics",{},2500))}; require(topics.find("/uwvm_resources/topic\tstd_msgs/String\n")!=std::string::npos&&topics.find("uwvm_resources_bad")==std::string::npos,"actual Master topic graph exact type filtered by segment prefix");
        auto services{text(client.call("graph.list-services","services",{},2500))}; require(services.find("/uwvm_resources/trigger\tnot-applicable\n")!=std::string::npos,"actual Master service graph marks unavailable type explicitly");
        auto nodes{text(client.call("graph.list-nodes","nodes",{},2500))}; require(nodes.find("/uwvm_resources/native\n")!=std::string::npos,"actual Master graph native node discovered");
        require(text(client.call("graph.inspect-topic","inspectTopic",{},2500))=="/uwvm_resources/topic\tstd_msgs/String\n","inspect topic is exact resource snapshot");
        require(text(client.call("graph.inspect-node","inspectNode",{},2500))=="/uwvm_resources/native\n","inspect node is exact native node snapshot");
        require(text(client.call("graph.inspect-service","inspectService",{},2500))=="/uwvm_resources/trigger\tnot-applicable\n","inspect service marks Master unavailable type explicitly");
        require(!client.call("param.delete","delete",{},2500).error&&!node.hasParam("/uwvm_resources/value"),"delete native ROS1 witness sees parameter removed");
        require(!client.call("capability.revoke","get",{},1000).error,"get capability revoked"); require(client.call("param.get","get",{},2500).error==125,"revoked get returns no sensitive data");
        node.deleteParam("/uwvm_resources_bad/secret"); client.close();ros1_native_scalars(node,argv[1]);ros1_native_arrays(node,argv[1]);spinner.stop(); ros::shutdown();
        std::cout<<"PASS actual ROS1 parameter get/set/delete/exists/search/list, filtered graph topics/nodes/services, independent capabilities and no unauthorized write side effect\n"; return 0;
    }
    catch(std::exception const& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
