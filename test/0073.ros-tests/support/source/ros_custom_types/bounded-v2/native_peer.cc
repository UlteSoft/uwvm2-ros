/** @file
 * @brief Independent official native bounded Service/Action peer.
 *
 * This process never connects to Gateway IPC, adopts a Guest handle or uses a
 * UWVM custom adapter. Build against the separately installed official fixture
 * package. The controller must own its PID, map its ARM ELF and SDK closure,
 * enforce an external absolute process deadline, and reap it on every outcome.
 * In particular an official ROS1 synchronous ServiceClient call has no intrinsic
 * call deadline; its acceptance deadline is the owned controller deadline.
 * That fixture limitation is distinct from the production bounded TCPROS client.
 *
 * Modes: server creates native Echo/Empty/Digest endpoints consumed by a Guest;
 * client calls the separately advertised Guest Echo/Digest endpoints. stdin Q is
 * required before final counters and PASS; the controller sends it only after
 * independently checking the Guest's complete response/feedback/result marker.
 * Server mode retains one accepted Goal, reads F to publish feedback, and reads
 * E only after the actual Guest decoded it. Client mode confirms native feedback
 * before emitting FEEDBACK_DELIVERED, which releases the Guest's terminal reply.
 * Separate DDS channels never rely on publication order or an arbitrary sleep.
 */
#include "native_protocol.h"
#include <string>
#include <array>
#include <atomic>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <memory>
#include <future>
#include <csignal>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <uwvm_ros_bounded_v2/Envelope.h>
#include <uwvm_ros_bounded_v2/Echo.h>
#include <uwvm_ros_bounded_v2/Empty.h>
#include <uwvm_ros_bounded_v2/DigestAction.h>
namespace official=uwvm_ros_bounded_v2;
using Envelope=official::Envelope;using Echo=official::Echo;using Empty=official::Empty;
using Digest=official::DigestAction;using Goal=official::DigestGoal;using Result=official::DigestResult;using Feedback=official::DigestFeedback;
#else
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <uwvm_ros_bounded_v2/msg/envelope.hpp>
#include <uwvm_ros_bounded_v2/srv/echo.hpp>
#include <uwvm_ros_bounded_v2/srv/empty.hpp>
#include <uwvm_ros_bounded_v2/action/digest.hpp>
using Envelope=uwvm_ros_bounded_v2::msg::Envelope;using Echo=uwvm_ros_bounded_v2::srv::Echo;using Empty=uwvm_ros_bounded_v2::srv::Empty;
using Digest=uwvm_ros_bounded_v2::action::Digest;using Goal=Digest::Goal;using Result=Digest::Result;using Feedback=Digest::Feedback;
#endif
namespace
{
    std::atomic<unsigned> service_calls{},empty_calls{},goals{},feedbacks{},results{},mismatch{},topic_received{},topic_sent{};
    unsigned quiet_samples{};std::uint64_t quiet_ns{};
    /** A callback owns this state until its official subscription retires. */
    struct feedback_observation {std::promise<bool> first;std::atomic<unsigned> callbacks{};};
    /** The independent peer uses native C++ values, with its own field oracle.
     * It does not import a generated canonical codec or compare raw CDR bytes. */
    Envelope sample()
    {
        Envelope value;value.label="bounded-native";value.samples={3,5,8};value.points.resize(2);
        value.points[0].value=-7;value.points[0].enabled=true;value.points[1].value=11;value.points[1].enabled=false;
        value.labels[0]="first";value.labels[1]="second";return value;
    }
    bool equal(Envelope const& value)
    {
        return value.label=="bounded-native"&&value.samples.size()==3&&value.samples[0]==3&&value.samples[1]==5&&value.samples[2]==8&&
            value.points.size()==2&&value.points[0].value==-7&&value.points[0].enabled&&value.points[1].value==11&&!value.points[1].enabled&&value.labels[0]=="first"&&value.labels[1]=="second";
    }
    Result result() {Result value;value.accepted=true;value.sum=16;value.note="bounded-native-result";return value;}
    Feedback feedback() {Feedback value;value.processed=3;value.phase="accepted";return value;}
    void require(bool condition,char const* reason) {if(!condition) {throw std::runtime_error(reason);}}
    void emit(char const* name,std::string const& mode) {bounded_native_io::print(STDOUT_FILENO,name," backend=",UWVM_CUSTOM_NATIVE_ROS_VERSION," pid=",::getpid()," mode=",mode,"\n");}
    /** Checked basic stdin protocol; the controller owns pacing/deadlines. */
    void final_gate() {
        bounded_native_io::gate('Q');
        const std::array<unsigned,8> before{service_calls.load(),empty_calls.load(),goals.load(),feedbacks.load(),results.load(),mismatch.load(),topic_received.load(),topic_sent.load()};
        auto begin{std::chrono::steady_clock::now()};auto until{begin+std::chrono::seconds{2}};unsigned samples{};
        do {std::this_thread::sleep_for(std::chrono::milliseconds{10});++samples;require(before==std::array<unsigned,8>{service_calls.load(),empty_calls.load(),goals.load(),feedbacks.load(),results.load(),mismatch.load(),topic_received.load(),topic_sent.load()},"late forbidden native effect");}while(std::chrono::steady_clock::now()<until||samples<30);quiet_samples=samples;quiet_ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-begin).count();
    }
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
    /** The peer explicitly requests the same complete five-channel contract
     * as the acceptance grants; no ROS2 endpoint chooses implicit QoS defaults. */
    rmw_qos_profile_t profile()
    {rmw_qos_profile_t p{};p.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;p.depth=4;p.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;p.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;p.deadline=RMW_DURATION_INFINITE;p.lifespan=RMW_DURATION_INFINITE;p.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;p.liveliness_lease_duration=RMW_DURATION_INFINITE;p.avoid_ros_namespace_conventions=false;return p;}
#endif
}
int main(int argc,char** argv)
{
    try
    {
        require(argc==2,"select exactly server, client or absent mode");std::string mode{argv[1]};require(mode=="server"||mode=="client"||mode=="absent"||mode=="topics","unknown peer mode");
        // The controller proves ELF birth/map/cgroup ownership while stopped,
        // then releases I before the first ROS initializer or entity creation.
        require(::raise(SIGSTOP)==0,"native mapping stop failed");bounded_native_io::gate('I');
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
        ::ros::init(argc,argv,"bounded_native_peer",::ros::init_options::NoSigintHandler);::ros::NodeHandle node{"/uwvm_bounded_v2"};::ros::AsyncSpinner spinner{2};spinner.start();
        if(mode=="topics")
        {
            auto publication=node.advertise<Envelope>("/uwvm_bounded_v2/native_topic",4,false);
            auto callback=boost::function<void(Envelope::ConstPtr const&)>{[&](auto const& value){++topic_received;if(!value||!equal(*value)) {++mismatch;return;}publication.publish(sample());++topic_sent;}};
            auto subscription=node.subscribe<Envelope>("/uwvm_bounded_v2/guest_topic",4,callback);
            emit("READY_BOUNDED_NATIVE",mode);
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds{10}};
            while((publication.getNumSubscribers()!=1||subscription.getNumPublishers()!=1)&&std::chrono::steady_clock::now()<until) {std::this_thread::sleep_for(std::chrono::milliseconds{5});}
            require(publication.getNumSubscribers()==1&&subscription.getNumPublishers()==1,"exact custom topic counterpart absent");
            emit("MATCHED_BOUNDED_NATIVE",mode);final_gate();
        }
        else if(mode!="client")
        {
            auto unexpected=boost::function<void(Envelope::ConstPtr const&)>{[](auto const&){++topic_received;}};
            auto topic_guard=node.subscribe<Envelope>("/uwvm_bounded_v2/guest_topic",4,unexpected);
            boost::function<bool(Echo::Request&,Echo::Response&)> echo{[](auto& request,auto& response){++service_calls;if(!equal(request.sample)||request.limit!=32) {++mismatch;return false;}response.accepted=true;response.echoed=request.sample;return true;}};
            boost::function<bool(Empty::Request&,Empty::Response&)> empty{[](auto&,auto&){++empty_calls;return true;}};
            auto echo_server=node.advertiseService<Echo::Request,Echo::Response>("/uwvm_bounded_v2/native_echo",echo);
            auto empty_server=node.advertiseService<Empty::Request,Empty::Response>("/uwvm_bounded_v2/native_empty",empty);
            using Server=actionlib::SimpleActionServer<Digest>;std::unique_ptr<Server> action;
            auto accepted=std::make_shared<std::promise<bool>>();auto admitted=accepted->get_future();
            action=std::make_unique<Server>(node,"/uwvm_bounded_v2/native_digest",false);
            action->registerGoalCallback([&,accepted]{auto goal=action->acceptNewGoal();const auto count{++goals};const bool valid{count==1&&goal&&equal(goal->sample)};if(!valid){++mismatch;action->setAborted(Result{});}if(count==1)accepted->set_value(valid);});action->start();
            emit("READY_BOUNDED_NATIVE",mode);
            if(mode=="server"){
                bounded_native_io::gate('F');require(admitted.wait_for(std::chrono::seconds{10})==std::future_status::ready&&admitted.get(),"native accepted Goal missing");
                action->publishFeedback(feedback());++feedbacks;emit("FEEDBACK_SENT_BOUNDED_NATIVE",mode);
                bounded_native_io::gate('E');action->setSucceeded(result());++results;emit("RESULT_SENT_BOUNDED_NATIVE",mode);
            }
            final_gate();
        }
        else
        {
            emit("READY_BOUNDED_NATIVE",mode);
            auto client=node.serviceClient<Echo>("/uwvm_bounded_v2/guest_echo",false);require(client.waitForExistence(::ros::Duration{3}),"Guest service missing");
            Echo call;call.request.sample=sample();call.request.limit=32;require(client.call(call)&&call.response.accepted&&equal(call.response.echoed),"Guest service native response mismatch");++service_calls;
            actionlib::SimpleActionClient<Digest> action{node,"/uwvm_bounded_v2/guest_digest",false};require(action.waitForServer(::ros::Duration{3}),"Guest Action missing");
            auto observed=std::make_shared<feedback_observation>();auto received=observed->first.get_future();
            Goal request;request.sample=sample();action.sendGoal(request,{}, {},[observed](official::DigestFeedbackConstPtr const& value){const auto count{++observed->callbacks};const bool valid{count==1&&value&&value->processed==3&&value->phase=="accepted"};if(!valid){++mismatch;}else{++feedbacks;}if(count==1)observed->first.set_value(valid);});
            require(received.wait_for(std::chrono::seconds{10})==std::future_status::ready&&received.get()&&feedbacks.load()==1,"Guest native feedback deadline/count");emit("FEEDBACK_DELIVERED_BOUNDED_NATIVE",mode);
            require(action.waitForResult(::ros::Duration{3}),"Guest native Action result deadline");auto value{action.getResult()};require(value&&value->accepted&&value->sum==16&&value->note=="bounded-native-result","Guest Action native result mismatch");++goals;++results;
            require(feedbacks.load()==1,"Guest native feedback deadline/count");
            emit("DELIVERED_BOUNDED_NATIVE",mode);final_gate();
        }
        spinner.stop();::ros::shutdown();
#else
        rclcpp::InitOptions init;init.set_domain_id(205);rclcpp::init(argc,argv,init);auto node{std::make_shared<rclcpp::Node>("bounded_native_peer","/uwvm_bounded_v2")};
        rclcpp::executors::MultiThreadedExecutor executor{rclcpp::ExecutorOptions{},2};executor.add_node(node);
        std::thread worker{[&]{executor.spin();}};
        // RAII always cancels/joins the owned native executor, including errors.
        struct shutdown {rclcpp::executors::MultiThreadedExecutor& executor;std::thread& worker;~shutdown(){executor.cancel();if(worker.joinable()) {worker.join();}rclcpp::shutdown();}} cleanup{executor,worker};
        if(mode=="topics")
        {
            auto p{profile()};rclcpp::QoS qos{rclcpp::QoSInitialization::from_rmw(p),p};
            auto publication=node->create_publisher<Envelope>("/uwvm_bounded_v2/native_topic",qos);
            auto subscription=node->create_subscription<Envelope>("/uwvm_bounded_v2/guest_topic",qos,[&](std::shared_ptr<Envelope const> value){++topic_received;if(!value||!equal(*value)) {++mismatch;return;}publication->publish(sample());++topic_sent;});
            emit("READY_BOUNDED_NATIVE",mode);
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds{10}};
            while((publication->get_subscription_count()!=1||node->count_publishers("/uwvm_bounded_v2/guest_topic")!=1)&&std::chrono::steady_clock::now()<until) {std::this_thread::sleep_for(std::chrono::milliseconds{5});}
            require(publication->get_subscription_count()==1&&node->count_publishers("/uwvm_bounded_v2/guest_topic")==1,"exact custom topic counterpart absent");
            emit("MATCHED_BOUNDED_NATIVE",mode);final_gate();
        }
        else if(mode!="client")
        {
            auto guard_profile{profile()};rclcpp::QoS guard_qos{rclcpp::QoSInitialization::from_rmw(guard_profile),guard_profile};
            auto topic_guard=node->create_subscription<Envelope>("/uwvm_bounded_v2/guest_topic",guard_qos,[](std::shared_ptr<Envelope const>){++topic_received;});
            auto qos{profile()};rclcpp::QoS configured{rclcpp::QoSInitialization::from_rmw(qos),qos};
            auto echo=node->create_service<Echo>("/uwvm_bounded_v2/native_echo",[](std::shared_ptr<Echo::Request> request,std::shared_ptr<Echo::Response> response){++service_calls;if(!request||!equal(request->sample)||request->limit!=32) {++mismatch;return;}response->accepted=true;response->echoed=request->sample;},configured);
            auto empty=node->create_service<Empty>("/uwvm_bounded_v2/native_empty",[](std::shared_ptr<Empty::Request> request,std::shared_ptr<Empty::Response> response){if(!request||!response) {++mismatch;return;}++empty_calls;response->structure_needs_at_least_one_member=0;},configured);
            rcl_action_server_options_t options{};options.allocator=rcl_get_default_allocator();options.goal_service_qos=qos;options.result_service_qos=qos;options.cancel_service_qos=qos;options.feedback_topic_qos=qos;options.status_topic_qos=qos;options.result_timeout.nanoseconds=3000000000;
            using Handle=rclcpp_action::ServerGoalHandle<Digest>;auto accepted=std::make_shared<std::promise<std::shared_ptr<Handle>>>();auto admitted=accepted->get_future();
            auto action=rclcpp_action::create_server<Digest>(node,"/uwvm_bounded_v2/native_digest",[](rclcpp_action::GoalUUID const&,std::shared_ptr<Goal const> request){if(!request||!equal(request->sample)||goals.load()!=0) {++mismatch;return rclcpp_action::GoalResponse::REJECT;}return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;},[](auto){return rclcpp_action::CancelResponse::REJECT;},[accepted](std::shared_ptr<Handle> goal){const auto count{++goals};if(count!=1||!goal){++mismatch;return;}accepted->set_value(std::move(goal));},options);
            emit("READY_BOUNDED_NATIVE",mode);
            if(mode=="server"){
                bounded_native_io::gate('F');require(admitted.wait_for(std::chrono::seconds{10})==std::future_status::ready,"native accepted Goal missing");auto goal=admitted.get();require(bool(goal),"null native Goal");
                goal->publish_feedback(std::make_shared<Feedback>(feedback()));++feedbacks;emit("FEEDBACK_SENT_BOUNDED_NATIVE",mode);
                bounded_native_io::gate('E');goal->succeed(std::make_shared<Result>(result()));++results;emit("RESULT_SENT_BOUNDED_NATIVE",mode);
            }
            final_gate();
        }
        else
        {
            emit("READY_BOUNDED_NATIVE",mode);
            auto qos{profile()};rclcpp::QoS configured{rclcpp::QoSInitialization::from_rmw(qos),qos};
            auto client=node->create_client<Echo>("/uwvm_bounded_v2/guest_echo",configured);require(client->wait_for_service(std::chrono::seconds{3}),"Guest service missing");auto request{std::make_shared<Echo::Request>()};request->sample=sample();request->limit=32;auto pending{client->async_send_request(request)};require(pending.wait_for(std::chrono::seconds{3})==std::future_status::ready,"Guest service deadline");auto response{pending.get()};require(response&&response->accepted&&equal(response->echoed),"Guest native service response mismatch");++service_calls;
            rcl_action_client_options_t client_options{};client_options.allocator=rcl_get_default_allocator();client_options.goal_service_qos=qos;client_options.result_service_qos=qos;client_options.cancel_service_qos=qos;client_options.feedback_topic_qos=qos;client_options.status_topic_qos=qos;
            auto action=rclcpp_action::create_client<Digest>(node,"/uwvm_bounded_v2/guest_digest",nullptr,client_options);require(action->wait_for_action_server(std::chrono::seconds{3}),"Guest Action missing");rclcpp_action::Client<Digest>::SendGoalOptions options;
            auto observed=std::make_shared<feedback_observation>();auto received=observed->first.get_future();
            options.feedback_callback=[observed](auto,std::shared_ptr<Feedback const> value){const auto count{++observed->callbacks};const bool valid{count==1&&value&&value->processed==3&&value->phase=="accepted"};if(!valid){++mismatch;}else{++feedbacks;}if(count==1)observed->first.set_value(valid);};Goal goal;goal.sample=sample();auto admitted{action->async_send_goal(goal,options)};require(admitted.wait_for(std::chrono::seconds{3})==std::future_status::ready,"Guest goal deadline");auto handle{admitted.get()};require(bool(handle),"Guest goal refused");
            require(received.wait_for(std::chrono::seconds{10})==std::future_status::ready&&received.get()&&feedbacks.load()==1,"Guest native feedback deadline/count");emit("FEEDBACK_DELIVERED_BOUNDED_NATIVE",mode);
            auto completed{action->async_get_result(handle)};require(completed.wait_for(std::chrono::seconds{3})==std::future_status::ready,"Guest result deadline");auto answer{completed.get()};require(answer.code==rclcpp_action::ResultCode::SUCCEEDED&&answer.result&&answer.result->accepted&&answer.result->sum==16&&answer.result->note=="bounded-native-result","Guest native Action result mismatch");++goals;++results;
            require(feedbacks.load()==1,"Guest native feedback deadline/count");
            emit("DELIVERED_BOUNDED_NATIVE",mode);final_gate();
        }
#endif
        const unsigned expected{mode=="absent"||mode=="topics"?0u:1u};require(service_calls==expected&&goals==expected&&feedbacks==expected&&results==expected&&mismatch==0,"native effect count mismatch");
        require(mode=="server"?empty_calls==1:empty_calls==0,"native empty-service count mismatch");
        require(topic_received==(mode=="topics"?1u:0u)&&topic_sent==(mode=="topics"?1u:0u),"native custom topic counts differ");
        bounded_native_io::print(STDOUT_FILENO,"PASS_BOUNDED_NATIVE backend=",UWVM_CUSTOM_NATIVE_ROS_VERSION," pid=",::getpid()," mode=",mode," services=",service_calls.load()," empty=",empty_calls.load()," goals=",goals.load()," feedback=",feedbacks.load()," result=",results.load()," mismatch=",mismatch.load()," topic_received=",topic_received.load()," topic_sent=",topic_sent.load()," quiet_samples=",quiet_samples," quiet_ns=",quiet_ns,'\n');return 0;
    }
    catch(std::exception const& error) {bounded_native_io::print(STDERR_FILENO,"FAIL_BOUNDED_NATIVE\n");return 1;}
}
