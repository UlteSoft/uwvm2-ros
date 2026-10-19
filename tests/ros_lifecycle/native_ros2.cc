#include "../ros_ipc/probe_common.h"
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <lifecycle_msgs/msg/state.hpp>
using namespace ros_probe;
namespace
{
    core::capability_spec capability(std::string alias,std::string operation,std::string transition="")
    {
        bool get{operation=="lifecycle.get-state"};
        core::capability_spec cap{alias,operation,"/uwvm_lifecycle_witness",get?"lifecycle_msgs/srv/GetState":"lifecycle_msgs/srv/ChangeState",get?"RIHS01_800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795":"RIHS01_356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5",{}};
        cap.limits={{"timeout-ms","1000"},{"max-inflight","1"},{"max-request-bytes","64"},{"max-response-bytes","1024"},{"max-rate-hz","100"},{"burst","10"},{"max-bytes-per-second","65536"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};
        service_qos(cap);cap.limits["qos.depth"]="6";
        if(!get) { cap.limits["allowed-transition"]=transition; }return cap;
    }
}
int main(int argc,char** argv)
{
    try
    {
        if(argc==2&&(std::string{argv[1]}=="--serve"||std::string{argv[1]}=="--observe-configure"||std::string{argv[1]}=="--observe-denied"))
        {
            std::string mode{argv[1]};
            rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp_lifecycle::LifecycleNode>("uwvm_lifecycle_witness")};
            rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node->get_node_base_interface());
            std::cout<<"READY\n"<<std::flush;
            bool configured{};auto end{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--observe-denied"?5:12)};
            while(std::chrono::steady_clock::now()<end)
            {
                executor.spin_some();auto state{node->get_current_state().id()};
                if(mode=="--observe-denied"){require(state==lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED,"denied Wasm lifecycle request caused no native state change");}
                if(mode=="--observe-configure"&&state==lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE){configured=true;break;}
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if(mode=="--observe-configure"){require(configured,"Wasm configure changed actual native lifecycle state to inactive");}
            rclcpp::shutdown();std::cout<<"PASS actual ROS2 Lifecycle external witness served\n";return 0;
        }
        require(argc==2,"usage: lifecycle probe gateway-executable");
        ::setenv("ROS_DOMAIN_ID","72",1);::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1);
        rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp_lifecycle::LifecycleNode>("uwvm_lifecycle_witness")};
        rclcpp::executors::MultiThreadedExecutor executor;executor.add_node(node->get_node_base_interface());std::thread spin{[&]{executor.spin();}};
        struct joiner { rclcpp::Executor& executor;std::thread& thread;~joiner(){executor.cancel();thread.join();rclcpp::shutdown();} } cleanup{executor,spin};
        fixture f{core::version::ros2,argv[1],[](gateway::configuration& c)
        {
            c.policy.capabilities.clear();auto get{capability("state","lifecycle.get-state")};c.policy.capabilities.emplace(get.alias,std::move(get));
            for(auto transition:{"configure","activate","deactivate","cleanup","shutdown"}) { auto cap{capability(transition,"lifecycle.change-state",transition)};c.policy.capabilities.emplace(cap.alias,std::move(cap)); }
        }};
        ipc::client client;f.connect(client);
        auto check{[&](std::uint8_t expected)
        {auto reply{client.call("lifecycle.get-state","state",{},1000)};require(!reply.error&&reply.payload.size()>=5&&reply.payload[0]==expected&&ipc::get(reply.payload.data()+1,4)==reply.payload.size()-5,"actual lifecycle state response");}};
        check(lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
        {
            fixture denied{core::version::ros2,argv[1],[](gateway::configuration& cfg){cfg.policy.connection["node-name"]="uwvm_incompatible_lifecycle_qos";cfg.policy.capabilities.clear();auto cap{capability("configure","lifecycle.change-state","configure")};cap.limits["qos.reliability"]="best-effort";cap.limits["timeout-ms"]="300";cfg.policy.capabilities.emplace(cap.alias,std::move(cap));}};
            ipc::client request;denied.connect(request);auto reply{request.call("lifecycle.change-state","configure",{},1000)};
            require(reply.error==110&&reply.payload.empty(),"explicit incompatible Lifecycle request QoS times out without response");std::this_thread::sleep_for(std::chrono::milliseconds(100));require(node->get_current_state().id()==lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED,"incompatible Lifecycle QoS leaves actual native state unchanged");request.close();
        }
        require(client.call("lifecycle.change-state","missing",{},1000).error==13,"ungranted lifecycle transition denied");
        require(client.call("lifecycle.change-state","configure",{3},1000).error==90,"Guest cannot replace configured transition beyond canonical empty request");
        check(lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
        auto change{[&](std::string const& alias){auto reply{client.call("lifecycle.change-state",alias,{},1000)};require(!reply.error&&reply.payload==std::vector<std::uint8_t>{1},"explicit lifecycle transition "+alias);}};
        change("configure");check(lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
        change("activate");check(lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
        change("deactivate");change("cleanup");check(lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
        change("shutdown");check(lifecycle_msgs::msg::State::PRIMARY_STATE_FINALIZED);
        std::cout<<"PASS ROS2 real Lifecycle explicit depth6 and request/response actual eight-field QoS, incompatible reliability has no native state change, get-state/all explicit transitions\n";return 0;
    }
    catch(std::exception const& e){std::cerr<<e.what()<<'\n';return 1;}
}
