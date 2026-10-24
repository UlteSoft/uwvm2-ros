#include "ros2_common.h"
#include "../../../../../src/ros/gateway/ros2/action.h"
#include <rclcpp/rclcpp.hpp>
#include <future>
#include <iostream>
#include <thread>

namespace query_deadline_test
{
    namespace action=uwvm2::ros_gateway::ros2_action;
    using fibonacci=action::fibonacci;
    template<typename Ready> bool pump(rclcpp::executors::SingleThreadedExecutor& executor,
        std::chrono::steady_clock::time_point until,Ready ready)
    {
        while(std::chrono::steady_clock::now()<until)
        { executor.spin_some();if(ready()) { return true; }std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
        return false;
    }
    std::shared_ptr<action::goal_state> find(std::uint64_t id)
    {
        std::lock_guard<std::mutex> lock{action::registry_mutex};auto it{action::registry.find(id)};
        return it==action::registry.end()?nullptr:it->second.lock();
    }
    void scenario(std::string const& kind,action::adapter& adapter,rclcpp::Node::SharedPtr const& native,
        rclcpp::executors::SingleThreadedExecutor& executor)
    {
        auto send{action_probe::action(action_probe::core::version::ros2,"send"+kind,"action.send-goal","/uwvm_query_deadline/"+kind)};
        send.limits["timeout-ms"]="1500";
        auto feedback{action_probe::action(action_probe::core::version::ros2,"feedback"+kind,"action.receive-feedback",send.resource)};
        auto receive{action_probe::action(action_probe::core::version::ros2,"result"+kind,"action.receive-result",send.resource)};
        unsigned requests{},cancellations{};std::shared_ptr<action::server_goal> native_goal;
        auto server{rclcpp_action::create_server<fibonacci>(native,send.resource,
            [&](rclcpp_action::GoalUUID const&,std::shared_ptr<fibonacci::Goal const>) { ++requests;return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE; },
            [&](std::shared_ptr<action::server_goal>) { ++cancellations;return rclcpp_action::CancelResponse::ACCEPT; },
            [&](std::shared_ptr<action::server_goal> handle) { native_goal=std::move(handle); })};
        auto sending{std::async(std::launch::async,[&] { return adapter.operation(send,action_probe::number(1,4),1500); })};
        action_probe::require(pump(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] {
            return sending.wait_for(std::chrono::milliseconds(0))==std::future_status::ready;
        }),"native goal acceptance did not finish");
        auto sent{sending.get()};action_probe::require(!sent.error&&sent.payload.size()==8&&requests==1&&native_goal,"actual outgoing goal was not accepted");
        auto id{action_probe::ipc::get(sent.payload.data(),8)};auto goal{find(id)};action_probe::require(id&&goal,"outgoing goal registry is missing");
        std::chrono::steady_clock::time_point deadline;
        { std::lock_guard<std::mutex> lock{goal->mutex};deadline=goal->deadline; }
        action_probe::require(std::chrono::steady_clock::now()+std::chrono::milliseconds(100)<deadline,"setup consumed the stored deadline");
        if(kind=="completed")
        {
            auto result{std::make_shared<fibonacci::Result>()};result->sequence={0,1};native_goal->succeed(result);
            action_probe::require(pump(executor,deadline,[&] { std::lock_guard<std::mutex> lock{goal->mutex};return goal->terminal; }),"on-time native result did not complete");
            { std::lock_guard<std::mutex> lock{goal->mutex};action_probe::require(!goal->error&&!goal->timer&&!goal->result.empty(),"on-time result or timer retirement is missing"); }
            std::this_thread::sleep_until(deadline+std::chrono::milliseconds(30));
            auto result_wire{adapter.operation(receive,action_probe::number(id),1000)};
            auto expected{action_probe::ipc::encode_action_sequence({id,2,{0,1}},true)};
            action_probe::require(!result_wire.error&&result_wire.payload==expected&&!find(id)&&!cancellations,"cached on-time result was expired or native cancellation sent");
            std::cout<<"PASS actual on-time SUCCEEDED result remains readable after stored deadline, exact wire and registry retirement\n";
            return;
        }
        if(kind=="feedback")
        {
            auto message{std::make_shared<fibonacci::Feedback>()};message->partial_sequence={0,1};
            // The goal has been registered by the client before this feedback.
            native_goal->publish_feedback(message);
            action_probe::require(pump(executor,deadline,[&] { std::lock_guard<std::mutex> lock{goal->mutex};return !goal->feedback.empty(); }),"actual native feedback did not enter the bounded queue");
        }
        std::this_thread::sleep_until(deadline+std::chrono::milliseconds(30));
        { std::lock_guard<std::mutex> lock{goal->mutex};action_probe::require(goal->active&&!goal->terminal&&goal->timer,"timer dispatched in the unpumped interval"); }
        auto first{adapter.operation(kind=="feedback"?feedback:receive,action_probe::number(id),1000)};
        action_probe::require(first.error==110&&first.payload.empty(),"expired "+kind+" query delivered queued data or returned EAGAIN instead of timeout");
        { std::lock_guard<std::mutex> lock{goal->mutex};action_probe::require(!goal->active&&goal->terminal&&goal->error==110&&!goal->timer&&goal->feedback.empty()&&!goal->feedback_bytes,"query did not retire expired goal/timer/queued feedback"); }
        if(kind=="feedback")
        {
            action_probe::require(find(id)==goal,"feedback query lost pending timeout result ownership");
            auto result{adapter.operation(receive,action_probe::number(id),1000)};
            action_probe::require(result.error==110&&result.payload.empty(),"timeout result was not preserved after feedback query");
        }
        action_probe::require(!find(id)&&adapter.operation(receive,action_probe::number(id),1000).error==9,"expired result ID remained usable");
        goal.reset(); // Observe native cancellation with normal retired ownership.
        action_probe::require(pump(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] { return cancellations==1&&native_goal->is_canceling(); }),"expired query did not send actual native cancellation");
        native_goal->canceled(std::make_shared<fibonacci::Result>());
        std::cout<<"PASS actual "<<kind<<" query after unpumped deadline:110 empty, inactive terminal, timer/queue retired, native cancel observed, result ID retired\n";
        (void)server;
    }
    int run(int argc,char** argv)
    {
        std::string selected{argc>1?argv[1]:"all"};action_probe::require(selected=="all"||selected=="feedback"||selected=="result"||selected=="completed","invalid scenario");
        rclcpp::init(argc,argv);rclcpp::NodeOptions options;
        options.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto gateway{std::make_shared<rclcpp::Node>("gateway_query_deadline","/uwvm_query_deadline",options)};
        auto native{std::make_shared<rclcpp::Node>("native_query_deadline","/uwvm_query_deadline",options)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(gateway);executor.add_node(native);
        action::adapter adapter{gateway};
        for(auto const* kind:{"feedback","result","completed"}) { if(selected=="all"||selected==kind) { scenario(kind,adapter,native,executor); } }
        return 0;
    }
}
int main(int argc,char** argv)
{
    try { auto result{query_deadline_test::run(argc,argv)};rclcpp::shutdown();return result; }
    catch(std::exception const& error) { if(rclcpp::ok()) { rclcpp::shutdown(); }std::cerr<<"FAIL "<<error.what()<<'\n';return 1; }
}
