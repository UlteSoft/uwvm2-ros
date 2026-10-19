#include "common.h"
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/parameter_descriptor.hpp>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <cstdio>
#include <utility>

using namespace ros_resources_probe;
namespace
{
    unsigned checks{};
    void check(bool ok,char const* message) { ++checks;require(ok,message); }
    std::string parameter_path(std::string const& node,std::string name)
    { std::replace(name.begin(),name.end(),'.','/');return node+"/"+name; }
    core::capability_spec parameter(std::string alias,std::string operation,std::string const& node,
        std::string const& name,std::size_t maximum,std::string const& prefix={})
    {
        bool list{operation=="param.list"};
        auto cap{resource(core::version::ros2,std::move(alias),operation,list?node:parameter_path(node,name))};
        cap.limits["parameter-node"]=node;cap.limits["parameter-name"]=list?"not-applicable":name;
        cap.limits["max-parameter-bytes"]=std::to_string(maximum);
        if(list) { cap.limits["allowed-name-prefix"]=prefix;cap.limits["list-depth"]="2"; }
        if(cap.limits.count("qos.depth")) { cap.limits["qos.depth"]="6"; }
        return cap;
    }
    core::capability_spec graph(std::string alias,std::string operation,std::string const& resource_name,
        std::size_t maximum,std::string const& prefix)
    {
        auto cap{resource(core::version::ros2,std::move(alias),std::move(operation),resource_name)};
        cap.limits["max-graph-result-bytes"]=std::to_string(maximum);cap.limits["allowed-name-prefix"]=prefix;return cap;
    }
    void exact(ipc::call_result const& result,std::string const& expected)
    {
        check(!result.error,"bounded native result succeeds");
        check(result.payload==string(expected),"complete canonical wire equals independent exact oracle");
        check(result.payload.size()==4+expected.size()&&ipc::get(result.payload.data(),4)==expected.size(),"wire quota includes exact LE length envelope");
    }
    void refused(ipc::call_result const& result,std::uint32_t error)
    { check(result.error==error,"bounded native refusal exact code");check(result.payload.empty(),"bounded native refusal has no partial payload"); }
    bool contains(ipc::call_result const& result,std::string const& row)
    { return !result.error&&result.payload.size()>=4&&text(result).find(row)!=std::string::npos; }
}
int main(int argc,char** argv)
{
    try
    {
        check(argc==2,"usage bounded-native-ros2 gateway");
        ::setenv("ROS_DOMAIN_ID","75",1);::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1);::setenv("ROS_SECURITY_ENABLE","false",1);
        rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("native","/uwvm_resources")};
        std::string const remote{"/uwvm_resources/native"},local{"/uwvm_test/uwvm_ros2_gateway_probe"};
        std::string const graph_prefix{"/uwvm_resources/bounded"},neighbor{"/uwvm_resources/bounded_bad/secret"};
        std::string const descriptor_text{"name=descriptor_value\ntype=4\nread_only=false\ndescription=metadata\nconstraints=unit"};
        std::string const local_descriptor_text{"name=bounded.a\ntype=4\nread_only=false\ndescription=\nconstraints="};
        std::string const remote_rows{remote+"/bounded/a\n"+remote+"/bounded/z\n"};
        std::string const local_rows{local+"/bounded/a\n"+local+"/bounded/z\n"};
        std::string const graph_rows{graph_prefix+"/a\tstd_msgs/msg/String\n"+graph_prefix+"/z\tstd_msgs/msg/String\n"};
        rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.description="metadata";descriptor.additional_constraints="unit";
        node->declare_parameter<std::string>("descriptor_value","normal",descriptor);
        rcl_interfaces::msg::ParameterDescriptor oversized;oversized.description.assign(128*1024,'x');
        node->declare_parameter<std::string>("oversized_descriptor","normal",oversized);
        node->declare_parameter<std::string>("bounded.z","z");node->declare_parameter<std::string>("bounded.a","a");
        node->declare_parameter<std::string>("bounded_bad.secret","hidden");
        // Keep actual DDS endpoints alive, including the excluded neighbor.
        auto z{node->create_publisher<std_msgs::msg::String>(graph_prefix+"/z",rclcpp::QoS{2})};
        auto a{node->create_publisher<std_msgs::msg::String>(graph_prefix+"/a",rclcpp::QoS{2})};
        auto outside{node->create_publisher<std_msgs::msg::String>(neighbor,rclcpp::QoS{2})};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&] { executor.spin(); }};
        try
        {
            fixture f{core::version::ros2,argv[1],[&](gateway::configuration& cfg) {
                cfg.policy.connection["domain-id"]="75";cfg.policy.connection["node-name"]="uwvm_ros2_gateway_probe";cfg.policy.capabilities.clear();
                add(cfg,parameter("describeBig","param.describe",remote,"oversized_descriptor",64));
                add(cfg,parameter("describeExact","param.describe",remote,"descriptor_value",4+descriptor_text.size()));
                add(cfg,parameter("describeShort","param.describe",remote,"descriptor_value",3+descriptor_text.size()));
                add(cfg,parameter("remoteListExact","param.list",remote,"",4+remote_rows.size(),remote+"/bounded"));
                add(cfg,parameter("remoteListShort","param.list",remote,"",3+remote_rows.size(),remote+"/bounded"));
                for(auto const& name:{"bounded.z","bounded.a","bounded_bad.secret"})
                {
                    auto alias{name==std::string{"bounded.z"}?"localZ":name==std::string{"bounded.a"}?"localA":"localNeighbor"};
                    add(cfg,parameter(alias,"param.declare",local,name,64));
                }
                add(cfg,parameter("localGetNeighbor","param.get",local,"bounded_bad.secret",64));
                add(cfg,parameter("localDescribeExact","param.describe",local,"bounded.a",4+local_descriptor_text.size()));
                add(cfg,parameter("localDescribeShort","param.describe",local,"bounded.a",3+local_descriptor_text.size()));
                add(cfg,parameter("localListExact","param.list",local,"",4+local_rows.size(),local+"/bounded"));
                add(cfg,parameter("localListShort","param.list",local,"",3+local_rows.size(),local+"/bounded"));
                add(cfg,graph("graphDiscovery","graph.list-topics","/uwvm_resources",65536,"/uwvm_resources"));
                add(cfg,graph("graphExact","graph.list-topics",graph_prefix,4+graph_rows.size(),graph_prefix));
                add(cfg,graph("graphShort","graph.list-topics",graph_prefix,3+graph_rows.size(),graph_prefix));
                // This limit fits the name and newline but excludes its real
                // generated type column, exercising single-row preflight.
                add(cfg,graph("graphTypeShort","graph.inspect-topic",graph_prefix+"/a",4+(graph_prefix+"/a").size()+1,graph_prefix));
                add(cfg,graph("inspectMissing","graph.inspect-topic",graph_prefix+"/missing",64,graph_prefix));
            }};
            ipc::client client;f.connect(client);
            // Require the excluded neighbor to be present in the Gateway's
            // unrestricted graph before claiming prefix filtering was tested.
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds{5}};bool discovered{};
            std::string const neighbor_row{neighbor+"\tstd_msgs/msg/String\n"};
            std::string const a_row{graph_prefix+"/a\tstd_msgs/msg/String\n"},z_row{graph_prefix+"/z\tstd_msgs/msg/String\n"};
            while(std::chrono::steady_clock::now()<until)
            {
                auto observed{client.call("graph.list-topics","graphDiscovery",{},1000)};
                if(contains(observed,a_row)&&contains(observed,z_row)&&contains(observed,neighbor_row)) { discovered=true;break; }
                check(!observed.error||observed.error==110||observed.error==11,"discovery only bounded transient pending");
                std::this_thread::sleep_for(std::chrono::milliseconds{50});
            }
            check(discovered,"Gateway discovers both native publishers and neighboring prefix within five seconds");
            refused(client.call("param.describe","describeBig",{},2500),90);
            exact(client.call("param.describe","describeExact",{},2500),descriptor_text);
            refused(client.call("param.describe","describeShort",{},2500),90);
            check(node->get_parameter("bounded_bad.secret").as_string()=="hidden","excluded remote parameter actually exists unchanged");
            exact(client.call("param.list","remoteListExact",{},2500),remote_rows);
            refused(client.call("param.list","remoteListShort",{},2500),90);
            for(auto const& item:{std::pair<char const*,char const*>{"localZ","z"},{"localA","a"},{"localNeighbor","hidden"}})
            {
                auto declared{client.call("param.declare",item.first,string(item.second),2500)};
                check(!declared.error&&declared.payload.empty(),"explicit local declaration has empty success payload");
            }
            exact(client.call("param.get","localGetNeighbor",{},2500),"hidden");
            exact(client.call("param.describe","localDescribeExact",{},2500),local_descriptor_text);
            refused(client.call("param.describe","localDescribeShort",{},2500),90);
            exact(client.call("param.list","localListExact",{},2500),local_rows);
            refused(client.call("param.list","localListShort",{},2500),90);
            auto filtered{client.call("graph.list-topics","graphExact",{},2500)};exact(filtered,graph_rows);
            check(text(filtered).find("bounded_bad")==std::string::npos,"neighbor absent from exact-budget scoped graph");
            refused(client.call("graph.list-topics","graphShort",{},2500),90);
            refused(client.call("graph.inspect-topic","graphTypeShort",{},2500),90);
            refused(client.call("graph.inspect-topic","inspectMissing",{},2500),2);
            client.close();
            std::printf("PASS actual ROS2 bounded metadata %u checks: descriptor, remote/local list, sorted filtered DDS graph, exact quotas and empty refusals\n",checks);
        }
        catch(...) { executor.cancel();spinner.join();rclcpp::shutdown();throw; }
        executor.cancel();spinner.join();rclcpp::shutdown();return 0;
    }
    catch(std::exception const& error) { std::fprintf(stderr,"FAIL bounded native ROS2: %s\n",error.what());return 1; }
}
