/** Original native-process owner of the complete industrial protocol witness.
 * No Wasm process or physical controller is represented by this executable.
 * The second stopped checkpoint intentionally leaves the actual production
 * Gateway alive for its original parent's kernel map_files inspection.
 */
#include "native_common.h"
#include "cases.h"
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <spawn.h>
#include <sys/wait.h>
extern char** environ;

static void checkpoint()
{ assert(::raise(SIGSTOP)==0);char permit{};assert(::read(STDIN_FILENO,&permit,1)==1&&permit=='I'); }
class gateway_process
{
    pid_t pid_{};
public:
    gateway_process(char const* executable,std::string const& configuration)
    {
        char* arguments[]{const_cast<char*>(executable),const_cast<char*>(configuration.c_str()),nullptr};
        assert(::posix_spawn(&pid_,executable,nullptr,nullptr,arguments,environ)==0&&pid_>0);
    }
    ~gateway_process(){stop();}
    void stop()
    {
        if(!pid_)return;
        assert(::kill(pid_,SIGTERM)==0);int status{};auto until=Clock::now()+10s;
        for(;;)
        {
            auto observed=::waitpid(pid_,&status,WNOHANG);if(observed==pid_)break;
            assert(observed==0&&Clock::now()<until);std::this_thread::sleep_for(2ms);
        }
        assert(WIFEXITED(status)&&WEXITSTATUS(status)==0);pid_=0;
    }
};

int main(int argc,char** argv)
{
    if(argc==3&&std::string_view{argv[1]}=="--credential-probe")
    {ipc::client client;return client.connect(argv[2],2,::geteuid(),1000)==0?1:0;}
    if(argc!=4||std::string_view{argv[1]}!="--trusted-native-fixture")return 13;
    checkpoint();std::string directory=argv[3];
    auto empty=gw::load(directory+"/empty.conf"),granted=gw::load(directory+"/granted.conf");
    assert(empty.policy.capabilities.empty()&&!granted.policy.capabilities.empty());
    assert(empty.worker_pid==static_cast<unsigned>(::getpid())&&granted.worker_pid==empty.worker_pid);
    rclcpp::init(0,nullptr);rclcpp::NodeOptions options;
    options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    auto node=std::make_shared<rclcpp::Node>("original_industrial_native_peer","/uwvm_industrial",options);
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    std::thread spinning{[&]{executor.spin();}};
    struct executor_guard
    {rclcpp::executors::SingleThreadedExecutor& executor;std::thread& thread;
     ~executor_guard(){executor.cancel();thread.join();rclcpp::shutdown();}} guard{executor,spinning};
    {
        gateway_process process{argv[2],directory+"/empty.conf"};ipc::client client;connect(client,empty);
        assert(call(client,"service.call","ungranted").error==13);bind(client,empty);
        for(auto operation:{"topic.publish","topic.subscribe","service.call","service.advertise",
                           "action.send-goal","action.advertise","action.debug","action.update"})
            assert(call(client,operation,"ungranted").error==13);
        eventually([&]{auto names=node->get_node_names();return std::find(names.begin(),names.end(),"/uwvm_industrial/gateway_empty")!=names.end();});
        auto endpoint_free=[&]{
            return node->get_node_graph_interface()->get_publisher_names_and_types_by_node("gateway_empty","/uwvm_industrial").empty()&&
                   node->get_node_graph_interface()->get_subscriber_names_and_types_by_node("gateway_empty","/uwvm_industrial").empty()&&
                   node->get_service_names_and_types_by_node("gateway_empty","/uwvm_industrial").empty();};
        assert(endpoint_free());
        auto injection=node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::ParameterEventsQoS());
        rcl_interfaces::msg::ParameterEvent event;event.node="/uwvm_industrial/gateway_empty";
        rcl_interfaces::msg::Parameter parameter;parameter.name="use_sim_time";
        parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_BOOL;parameter.value.bool_value=true;
        event.changed_parameters.push_back(parameter);
        for(unsigned i{};i!=3;++i){injection->publish(event);std::this_thread::sleep_for(30ms);}
        assert(endpoint_free()&&node->count_subscribers("/clock")==0);client.close();process.stop();
        fast_io::io::println("PASS zero-grant-production-gateway no-implicit-endpoints foreign-clock-refused");
    }
    gateway_process process{argv[2],directory+"/granted.conf"};ipc::client client;connect(client,granted);
    assert(call(client,"topic.publish","absent").error==13);
    auto wrong=ros::serialize_manifest(granted.policy);assert(!wrong.empty());wrong.back()^=1;
    assert(call(client,"session.bind","target",wrong).error==13);bind(client,granted);
    assert(call(client,"session.bind","target",ros::serialize_manifest(granted.policy)).error==13);
    assert(call(client,"service.call","absent").error==13);
    assert(call(client,"action.debug","absent").error==13&&call(client,"action.update","absent").error==13);
    pid_t probe{};char* arguments[]{argv[0],const_cast<char*>("--credential-probe"),const_cast<char*>(granted.socket.c_str()),nullptr};
    assert(::posix_spawn(&probe,argv[0],nullptr,nullptr,arguments,environ)==0);int status{};
    assert(::waitpid(probe,&status,0)==probe&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    ipc::client foreign;connect(foreign,granted);bind(foreign,granted);
    context ctx{node,client,foreign};run_cases(ctx);
    fast_io::io::println("PASS complete-industrial-native-route messages=",ctx.messages," services=",ctx.services,
        " actions=",ctx.actions," topic-modes=",ctx.topic_cases," service-modes=",ctx.service_cases," action-modes=",ctx.action_cases);
    checkpoint();foreign.close();client.close();process.stop();
    eventually([&]{auto names=node->get_node_names();return std::find(names.begin(),names.end(),"/uwvm_industrial/gateway_granted")==names.end();});
    fast_io::io::println("PASS complete-session-and-production-gateway-retirement");
}
