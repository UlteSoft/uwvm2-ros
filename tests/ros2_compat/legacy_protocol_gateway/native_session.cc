/** Explicit trusted-process integration witness for an actual production
 * Gateway and an independently running original ROS2 SDK peer.
 *
 * The external owner must establish an isolated loopback-only network namespace,
 * bound every process to its hard memory cgroup, provide exact source/proof and
 * executable pins, and prepare two immutable configuration files while this
 * original child is stopped at its initial checkpoint. This witness neither
 * grants Guest authority nor represents an actual Wasm Runtime execution.
 *
 * Both directions use the production canonical codecs and actual native SDK
 * Topics, Services and Actions. Native callbacks independently check field
 * values. The second stopped checkpoint leaves the production Gateway alive so
 * its original parent, cgroup, namespace and kernel-mapped ELF bytes can be
 * captured before deterministic session/process teardown.
 */
#ifdef NDEBUG
#error "The actual native integration assertions must remain enabled"
#endif
#include "gateways/common/config.h"
#include <uwvm2/imported/ros/common/manifest.h>
#include UWVM_ROS_CUSTOM_ROS2_HEADER
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <uwvm_source_proof_msgs/msg/child.hpp>
#include <uwvm_source_proof_msgs/srv/transform.hpp>
#include <uwvm_source_proof_msgs/action/traverse.hpp>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <atomic>
#include <cassert>
#include <csignal>
#include <future>
#include <mutex>
#include <spawn.h>
#include <sys/wait.h>
#include <thread>
extern char** environ;
namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace ros=uwvm2::imported::ros;
namespace codecs=uwvm_custom_bounded_native;
using Child=uwvm_source_proof_msgs::msg::Child;
using Transform=uwvm_source_proof_msgs::srv::Transform;
using Traverse=uwvm_source_proof_msgs::action::Traverse;
using ChildCodec=codecs::Ros2Uwvm_source_proof_msgsMsgChildCodec;
using RequestCodec=codecs::Ros2Uwvm_source_proof_msgsSrvTransform_RequestCodec;
using ResponseCodec=codecs::Ros2Uwvm_source_proof_msgsSrvTransform_ResponseCodec;
using GoalCodec=codecs::Ros2Uwvm_source_proof_msgsActionTraverse_GoalCodec;
using ResultCodec=codecs::Ros2Uwvm_source_proof_msgsActionTraverse_ResultCodec;
using FeedbackCodec=codecs::Ros2Uwvm_source_proof_msgsActionTraverse_FeedbackCodec;
using Clock=std::chrono::steady_clock;
using namespace std::chrono_literals;

static void checkpoint()
{
    assert(::raise(SIGSTOP)==0);char permit{};
    assert(::read(STDIN_FILENO,&permit,1)==1&&permit=='I');
}
static void note(char const* text)
{ assert(gw::native_io::write_audit_text(text)); }
template<class Predicate>static void eventually(Predicate predicate)
{
    auto until=Clock::now()+8s;
    while(!predicate()){assert(Clock::now()<until);std::this_thread::sleep_for(2ms);}
}
template<class Future>static auto ready(Future& future)
{ assert(future.wait_for(8s)==std::future_status::ready);return future.get(); }
template<class Codec,class Native>static std::vector<std::uint8_t> encode(Native const& native)
{ std::vector<std::uint8_t> bytes;assert(Codec::encode(native,bytes));return bytes; }
template<class Codec>static typename Codec::Native decode(std::vector<std::uint8_t> const& bytes,std::size_t prefix=0)
{
    typename Codec::Native native{};assert(prefix<=bytes.size());
    assert(Codec::decode(bytes.data()+prefix,bytes.size()-prefix,native));return native;
}
static Child child(int ordinal)
{
    Child value;value.number=-1700-ordinal;value.empty="";value.text="native-peer-"+std::to_string(ordinal);
    value.signed_zero=-0.0;value.enabled=ordinal%2;return value;
}
static void same(Child const& value,int ordinal)
{
    assert(value.number==-1700-ordinal&&value.empty.empty()&&value.text=="native-peer-"+std::to_string(ordinal));
    assert(value.signed_zero==0&&std::signbit(value.signed_zero)&&value.enabled==bool(ordinal%2));
}
static Transform::Request request(int ordinal)
{ Transform::Request value;value.count=-300-ordinal;value.input=child(ordinal);value.tag="request";value.scale={{1.25,-2.5,-0.0}};return value; }
static void same(Transform::Request const& value,int ordinal)
{
    assert(value.count==-300-ordinal&&value.tag=="request"&&value.scale[0]==1.25&&value.scale[1]==-2.5);
    assert(value.scale[2]==0&&std::signbit(value.scale[2]));same(value.input,ordinal);
}
static Transform::Response response(int ordinal)
{ Transform::Response value;value.accepted=true;value.results={child(ordinal),child(ordinal+1)};value.reason="accepted";value.digest={0,17,128,255};return value; }
static void same(Transform::Response const& value,int ordinal)
{
    assert(value.accepted&&value.reason=="accepted"&&value.results.size()==2&&value.digest.size()==4);
    assert(value.digest[0]==0&&value.digest[1]==17&&value.digest[2]==128&&value.digest[3]==255);
    same(value.results[0],ordinal);same(value.results[1],ordinal+1);
}
static Traverse::Goal goal(int ordinal)
{ Traverse::Goal value;value.command=child(ordinal);value.steps=3;value.label="traverse";return value; }
static void same(Traverse::Goal const& value,int ordinal)
{ assert(value.steps==3&&value.label=="traverse");same(value.command,ordinal); }
static Traverse::Feedback feedback(int ordinal)
{ Traverse::Feedback value;value.fraction=0.625;value.state=child(ordinal);return value; }
static void same(Traverse::Feedback const& value,int ordinal)
{ assert(value.fraction==0.625);same(value.state,ordinal); }
static Traverse::Result result(int ordinal)
{ Traverse::Result value;value.completed=true;value.final_state=child(ordinal);value.message="finished";return value; }
static void same(Traverse::Result const& value,int ordinal)
{ assert(value.completed&&value.message=="finished");same(value.final_state,ordinal); }

/** Spawn/retire only the exact executable explicitly selected by the external
 * owner. No shell, daemon, inherited RPC descriptor or arbitrary argv tail. */
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
        assert(::kill(pid_,SIGTERM)==0);int status{};auto until=Clock::now()+8s;
        for(;;)
        {
            auto observed=::waitpid(pid_,&status,WNOHANG);
            if(observed==pid_)break;
            assert(observed==0&&Clock::now()<until);std::this_thread::sleep_for(2ms);
        }
        assert(WIFEXITED(status)&&WEXITSTATUS(status)==0);pid_=0;
    }
};
static void connect(ipc::client& client,gw::configuration const& configuration)
{
    eventually([&]{struct stat st{};return ::lstat(configuration.socket.c_str(),&st)==0&&S_ISSOCK(st.st_mode);});
    assert(client.connect(configuration.socket,2,::geteuid(),10000)==0);
}
static ipc::call_result call(ipc::client& client,char const* operation,char const* alias,std::vector<std::uint8_t> bytes={})
{ return client.call(operation,alias,std::move(bytes),10000); }
static void bind(ipc::client& client,gw::configuration const& configuration)
{ auto answer=call(client,"session.bind","target",ros::serialize_manifest(configuration.policy));assert(!answer.error&&answer.payload.empty()); }
static ipc::call_result poll(ipc::client& client,char const* operation,char const* alias,std::vector<std::uint8_t> bytes={})
{
    ipc::call_result answer;
    eventually([&]{answer=call(client,operation,alias,bytes);assert(!answer.error||answer.error==11);return !answer.error;});return answer;
}
static std::vector<std::uint8_t> id_bytes(std::uint64_t id)
{ std::vector<std::uint8_t> bytes;ipc::put(bytes,id,8);return bytes; }
template<class Codec,class Native>static std::vector<std::uint8_t> framed(std::uint64_t id,std::uint8_t outcome,Native const& value)
{ auto bytes=id_bytes(id);bytes.push_back(outcome);auto body=encode<Codec>(value);bytes.insert(bytes.end(),body.begin(),body.end());return bytes; }

int main(int argc,char** argv)
{
    // A separately spawned process has a different SO_PEERCRED PID even when
    // it possesses the socket path and uses the same UID and exact ELF.
    if(argc==3&&std::string_view{argv[1]}=="--credential-probe")
    { ipc::client client;return client.connect(argv[2],2,::geteuid(),1000)==0?1:0; }
    if(argc!=4||std::string_view{argv[1]}!="--trusted-native-fixture")return 13;
    checkpoint(); // External owner supplies immutable PID-bound test configs.
    std::string directory=argv[3];auto empty=gw::load(directory+"/empty.conf"),granted=gw::load(directory+"/granted.conf");
    assert(empty.policy.capabilities.empty()&&granted.policy.capabilities.size()==11);
    assert(empty.worker_pid==static_cast<unsigned>(::getpid())&&granted.worker_pid==empty.worker_pid);
    rclcpp::init(0,nullptr);
    rclcpp::NodeOptions options;options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    auto node=std::make_shared<rclcpp::Node>("original_native_peer","/uwvm_legacy",options);
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    std::thread spinning{[&]{executor.spin();}};
    struct executor_guard
    { rclcpp::executors::SingleThreadedExecutor& executor;std::thread& thread;~executor_guard(){executor.cancel();thread.join();rclcpp::shutdown();} } guard{executor,spinning};

    {
        gateway_process process{argv[2],directory+"/empty.conf"};ipc::client client;connect(client,empty);
        assert(call(client,"service.call","service-call",{}).error==13);bind(client,empty);
        for(auto operation:{"topic.publish","topic.subscribe","service.call","service.advertise","action.send-goal","action.advertise","action.debug","action.update"})
            assert(call(client,operation,"ungranted").error==13);
        eventually([&]{auto names=node->get_node_names();return std::find(names.begin(),names.end(),"/uwvm_legacy/gateway_empty")!=names.end();});
        assert(node->get_node_graph_interface()->get_publisher_names_and_types_by_node("gateway_empty","/uwvm_legacy").empty());
        assert(node->get_node_graph_interface()->get_subscriber_names_and_types_by_node("gateway_empty","/uwvm_legacy").empty());
        assert(node->get_service_names_and_types_by_node("gateway_empty","/uwvm_legacy").empty());
        // A foreign event claiming the Gateway's own name must not enable an
        // ungranted simulation clock reader through the SDK's TimeSource.
        auto injection=node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::ParameterEventsQoS());
        rcl_interfaces::msg::ParameterEvent event;event.node="/uwvm_legacy/gateway_empty";
        rcl_interfaces::msg::Parameter parameter;parameter.name="use_sim_time";
        parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_BOOL;parameter.value.bool_value=true;
        event.changed_parameters.push_back(parameter);
        for(unsigned i{};i!=3;++i){injection->publish(event);std::this_thread::sleep_for(30ms);}
        assert(node->get_node_graph_interface()->get_subscriber_names_and_types_by_node("gateway_empty","/uwvm_legacy").empty());
        assert(node->count_subscribers("/clock")==0);
        client.close();process.stop();note("zero-grant-production-gateway-and-no-implicit-endpoints: PASS\n");
    }

    std::atomic<unsigned> service_calls{},topic_calls{},cancellations{},native_feedback{};
    auto service=node->create_service<Transform>("peer_service",[&](std::shared_ptr<Transform::Request> input,std::shared_ptr<Transform::Response> output)
    {same(*input,1);*output=response(1);++service_calls;});
    auto topic_subscriber=node->create_subscription<Child>("from_gateway",rclcpp::QoS(16),[&](Child::ConstSharedPtr input){same(*input,3);++topic_calls;});
    auto topic_publisher=node->create_publisher<Child>("to_gateway",rclcpp::QoS(16));
    std::mutex goals_mutex;std::shared_ptr<rclcpp_action::ServerGoalHandle<Traverse>> native_goal;
    auto action_server=rclcpp_action::create_server<Traverse>(node,"peer_action",
        [](rclcpp_action::GoalUUID const&,std::shared_ptr<Traverse::Goal const> input){same(*input,5);return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;},
        [&](std::shared_ptr<rclcpp_action::ServerGoalHandle<Traverse>>){++cancellations;return rclcpp_action::CancelResponse::ACCEPT;},
        [&](std::shared_ptr<rclcpp_action::ServerGoalHandle<Traverse>> value){std::lock_guard<std::mutex> lock{goals_mutex};native_goal=std::move(value);});
    auto current_goal=[&]{std::lock_guard<std::mutex> lock{goals_mutex};return native_goal;};
    auto clear_goal=[&]{std::lock_guard<std::mutex> lock{goals_mutex};native_goal.reset();};
    gateway_process process{argv[2],directory+"/granted.conf"};ipc::client client;connect(client,granted);
    assert(call(client,"service.call","service-call",encode<RequestCodec>(request(1))).error==13);
    auto wrong=ros::serialize_manifest(granted.policy);wrong.back()^=1;
    assert(call(client,"session.bind","target",wrong).error==13);bind(client,granted);
    assert(call(client,"session.bind","target",ros::serialize_manifest(granted.policy)).error==13);
    assert(call(client,"service.call","absent",encode<RequestCodec>(request(1))).error==13);
    assert(call(client,"action.debug","action-send").error==13&&call(client,"action.update","action-send").error==13);
    pid_t probe{};char* probe_args[]{argv[0],const_cast<char*>("--credential-probe"),const_cast<char*>(granted.socket.c_str()),nullptr};
    assert(::posix_spawn(&probe,argv[0],nullptr,nullptr,probe_args,environ)==0);int probe_status{};
    assert(::waitpid(probe,&probe_status,0)==probe&&WIFEXITED(probe_status)&&WEXITSTATUS(probe_status)==0);
    auto damaged=encode<RequestCodec>(request(1));damaged.push_back(1);
    assert(call(client,"service.call","service-call",damaged).error==22&&service_calls==0);
    auto serviced=call(client,"service.call","service-call",encode<RequestCodec>(request(1)));
    assert(!serviced.error&&service_calls==1);same(decode<ResponseCodec>(serviced.payload),1);
    note("real-production-gateway-service-call-and-refusals: PASS\n");

    assert(!call(client,"topic.publish","topic-publish",encode<ChildCodec>(child(3))).error);
    // Volatile delivery before first discovery is not guaranteed. Wait for
    // actual matching before the second publication; validate every delivery.
    eventually([&]{return topic_subscriber->get_publisher_count()==1;});
    assert(!call(client,"topic.publish","topic-publish",encode<ChildCodec>(child(3))).error);
    eventually([&]{return topic_calls>=1;});assert(!call(client,"topic.subscribe","topic-subscribe").error);
    eventually([&]{return topic_publisher->get_subscription_count()==1;});topic_publisher->publish(child(4));
    auto subscribed=poll(client,"event.poll","topic-subscribe");same(decode<ChildCodec>(subscribed.payload),4);
    note("real-production-gateway-bidirectional-topics: PASS\n");

    assert(!call(client,"service.advertise","service-advertise").error);
    auto native_client=node->create_client<Transform>("gateway_service");assert(native_client->wait_for_service(8s));
    auto pending_service=native_client->async_send_request(std::make_shared<Transform::Request>(request(2)));
    auto service_event=poll(client,"event.poll","service-advertise");assert(service_event.payload.size()>=8);
    auto service_id=ipc::get(service_event.payload.data(),8);assert(service_id);same(decode<RequestCodec>(service_event.payload,8),2);
    auto service_answer=id_bytes(service_id);auto response_bytes=encode<ResponseCodec>(response(2));service_answer.insert(service_answer.end(),response_bytes.begin(),response_bytes.end());
    assert(!call(client,"service.respond","service-respond",service_answer).error);same(*ready(pending_service),2);
    assert(call(client,"service.respond","service-respond",service_answer).error==9);
    note("real-production-gateway-service-advertise-respond: PASS\n");

    auto sent=call(client,"action.send-goal","action-send",encode<GoalCodec>(goal(5)));assert(!sent.error&&sent.payload.size()==8);
    auto goal_id=ipc::get(sent.payload.data(),8);assert(goal_id);eventually([&]{return bool(current_goal());});
    ipc::client foreign;connect(foreign,granted);bind(foreign,granted);
    assert(call(foreign,"action.receive-result","action-result",id_bytes(goal_id)).error==13);
    assert(call(foreign,"action.cancel-own-goal","action-cancel",id_bytes(goal_id)).error==13&&cancellations==0);
    current_goal()->publish_feedback(std::make_shared<Traverse::Feedback>(feedback(5)));
    auto feedback_event=poll(client,"action.receive-feedback","action-feedback",id_bytes(goal_id));
    assert(ipc::get(feedback_event.payload.data(),8)==goal_id);same(decode<FeedbackCodec>(feedback_event.payload,8),5);
    current_goal()->succeed(std::make_shared<Traverse::Result>(result(5)));
    auto result_event=poll(client,"action.receive-result","action-result",id_bytes(goal_id));
    assert(result_event.payload.size()>=9&&ipc::get(result_event.payload.data(),8)==goal_id&&result_event.payload[8]==2);same(decode<ResultCodec>(result_event.payload,9),5);
    assert(call(client,"action.receive-result","action-result",id_bytes(goal_id)).error==9);
    clear_goal();sent=call(client,"action.send-goal","action-send",encode<GoalCodec>(goal(5)));assert(!sent.error&&sent.payload.size()==8);
    auto cancel_id=ipc::get(sent.payload.data(),8);assert(cancel_id&&cancel_id!=goal_id);eventually([&]{return bool(current_goal());});
    assert(!call(client,"action.cancel-own-goal","action-cancel",id_bytes(cancel_id)).error);
    eventually([&]{return cancellations==1&&current_goal()->is_canceling();});current_goal()->canceled(std::make_shared<Traverse::Result>(result(5)));
    result_event=poll(client,"action.receive-result","action-result",id_bytes(cancel_id));
    assert(result_event.payload.size()>=9&&result_event.payload[8]==4);same(decode<ResultCodec>(result_event.payload,9),5);
    note("real-production-gateway-action-feedback-result-cancel-and-session-isolation: PASS\n");

    assert(!call(client,"action.advertise","action-advertise").error);
    auto native_action=rclcpp_action::create_client<Traverse>(node,"gateway_action");assert(native_action->wait_for_action_server(8s));
    rclcpp_action::Client<Traverse>::SendGoalOptions native_options;
    native_options.feedback_callback=[&](rclcpp_action::ClientGoalHandle<Traverse>::SharedPtr,std::shared_ptr<Traverse::Feedback const> value){same(*value,6);++native_feedback;};
    auto pending_goal=native_action->async_send_goal(goal(6),native_options);auto accepted_goal=ready(pending_goal);assert(accepted_goal);
    auto pending_result=native_action->async_get_result(accepted_goal);
    auto incoming=poll(client,"event.poll","action-advertise");assert(incoming.payload.size()>=8);
    auto incoming_id=ipc::get(incoming.payload.data(),8);assert(incoming_id&&incoming_id!=goal_id&&incoming_id!=cancel_id);same(decode<GoalCodec>(incoming.payload,8),6);
    assert(!call(client,"action.respond","action-respond",framed<FeedbackCodec>(incoming_id,1,feedback(6))).error);
    eventually([&]{return native_feedback==1;});assert(!call(client,"action.respond","action-respond",framed<ResultCodec>(incoming_id,2,result(6))).error);
    auto native_result=ready(pending_result);assert(native_result.code==rclcpp_action::ResultCode::SUCCEEDED&&native_result.result);same(*native_result.result,6);
    assert(call(client,"action.respond","action-respond",framed<ResultCodec>(incoming_id,2,result(6))).error==9);
    note("real-production-gateway-action-advertise-feedback-result: PASS\n");
    checkpoint(); // Capture both original actors while all real evidence lives.
    foreign.close();client.close();process.stop();
    eventually([&]{auto names=node->get_node_names();return std::find(names.begin(),names.end(),"/uwvm_legacy/gateway_granted")==names.end();});
    assert(service_calls==1&&topic_calls>=1&&topic_calls<=2&&cancellations==1&&native_feedback==1);
    note("real-production-gateway-full-session-and-process-retirement: PASS\n");
}
