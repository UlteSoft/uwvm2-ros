#include "ros2_common.h"
#include "../../gateways/ros2/action.h"
#include <rclcpp/rclcpp.hpp>
#include <future>
#include <iostream>
#include <thread>

namespace action_deadline_test
{
    namespace action=uwvm2::ros_gateway::ros2_action;
    using fibonacci=action::fibonacci;
    template<typename Ready>
    bool pump_until(rclcpp::executors::SingleThreadedExecutor& executor,
                    std::chrono::steady_clock::time_point until,Ready ready)
    {
        while(std::chrono::steady_clock::now()<until)
        {
            // Each dispatch cycle returns fully: no executor callback remains in
            // flight when the test enters its deliberately unpumped interval.
            executor.spin_some();
            if(ready()) { return true; }
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        return false;
    }
    std::shared_ptr<action::goal_state> find_goal(std::uint64_t id)
    {
        std::lock_guard<std::mutex> lock{action::registry_mutex};auto found{action::registry.find(id)};
        return found==action::registry.end()?nullptr:found->second.lock();
    }
    void outgoing_cutoffs(action::adapter& adapter,rclcpp::Node::SharedPtr const& native,
                          rclcpp::executors::SingleThreadedExecutor& executor)
    {
        unsigned goal_requests{},cancel_requests{};std::shared_ptr<action::server_goal> native_goal;
        auto send{action_probe::action(action_probe::core::version::ros2,"send","action.send-goal","/uwvm_action_deadline/native_fibonacci")};
        auto cancel_own{action_probe::action(action_probe::core::version::ros2,"cancel-own","action.cancel-own-goal",send.resource)};
        auto cancel_any{action_probe::action(action_probe::core::version::ros2,"cancel-any","action.cancel-any-goal",send.resource)};
        auto receive{action_probe::action(action_probe::core::version::ros2,"result","action.receive-result",send.resource)};
        auto server{rclcpp_action::create_server<fibonacci>(native,send.resource,
            [&](rclcpp_action::GoalUUID const&,std::shared_ptr<fibonacci::Goal const>) {
                ++goal_requests;return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            },
            [&](std::shared_ptr<action::server_goal>) {
                ++cancel_requests;return rclcpp_action::CancelResponse::REJECT;
            },
            [&](std::shared_ptr<action::server_goal> handle) { native_goal=std::move(handle); })};
        auto discovery{rclcpp_action::create_client<fibonacci>(native,send.resource)};
        action_probe::require(pump_until(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] {
            return discovery->action_server_is_ready();
        }),"native outgoing deadline witness server was not discovered");
        auto zero_send{adapter.operation(send,action_probe::number(1,4),0)};
        action_probe::require(zero_send.error==110&&zero_send.payload.empty(),"zero-budget outgoing send returned success or nonempty output");
        pump_until(executor,std::chrono::steady_clock::now()+std::chrono::milliseconds(100),[] { return false; });
        action_probe::require(goal_requests==0&&cancel_requests==0&&!native_goal,"zero-budget send reached the actual native Goal callback");
        // The adapter waits for the actual native response on a worker; only the
        // main thread pumps callbacks, and no executor.cancel/restart is used.
        auto sending{std::async(std::launch::async,[&] {
            return adapter.operation(send,action_probe::number(1,4),2000);
        })};
        action_probe::require(pump_until(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] {
            return sending.wait_for(std::chrono::milliseconds(0))==std::future_status::ready;
        }),"actual outgoing goal setup did not complete");
        auto sent{sending.get()};
        action_probe::require(!sent.error&&sent.payload.size()==8,"actual outgoing goal setup failed before zero-budget cancel probe");
        auto id{action_probe::ipc::get(sent.payload.data(),8)};auto goal{find_goal(id)};
        action_probe::require(id&&goal&&goal_requests==1&&native_goal&&native_goal->is_active(),"native outgoing goal or adapter ownership was not active");
        {
            std::lock_guard<std::mutex> lock{goal->mutex};
            action_probe::require(goal->active&&!goal->terminal&&goal->outgoing,"actual outgoing ClientGoalHandle is missing before cancel probe");
        }
        auto own{adapter.operation(cancel_own,action_probe::number(id),0)};
        auto any{adapter.operation(cancel_any,action_probe::number(0),0)};
        action_probe::require(own.error==110&&own.payload.empty()&&any.error==110&&any.payload.empty(),"zero-budget outgoing cancel returned success or nonempty output");
        pump_until(executor,std::chrono::steady_clock::now()+std::chrono::milliseconds(100),[] { return false; });
        action_probe::require(goal_requests==1&&cancel_requests==0&&native_goal->is_active(),"zero-budget own/any cancel reached the actual native cancel callback or retired the goal");
        std::cout<<"PASS actual discovered native Action server: send timeout0=110 empty native_goal_requests0; actual owned active goal: cancel-own/any timeout0=110 empty native_cancel_requests0\n";
        native_goal->abort(std::make_shared<fibonacci::Result>());
        action_probe::require(pump_until(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] {
            std::lock_guard<std::mutex> lock{goal->mutex};return goal->terminal;
        }),"native outgoing witness cleanup result did not arrive");
        auto terminal{adapter.operation(receive,action_probe::number(id),1000)};
        action_probe::require(!terminal.error&&!find_goal(id),"actual outgoing witness ownership was not released after native abort");
        (void)server;
    }
    int run(int argc,char** argv)
    {
        rclcpp::init(argc,argv);
        rclcpp::NodeOptions options;options.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto gateway{std::make_shared<rclcpp::Node>("gateway_deadline","/uwvm_action_deadline",options)};
        auto native{std::make_shared<rclcpp::Node>("native_deadline","/uwvm_action_deadline",options)};
        action::adapter adapter{gateway};
        auto advertise{action_probe::action(action_probe::core::version::ros2,"advertise","action.advertise","/uwvm_action_deadline/fibonacci")};
        auto respond{action_probe::action(action_probe::core::version::ros2,"respond","action.respond",advertise.resource)};
        advertise.limits["timeout-ms"]=respond.limits["timeout-ms"]="1500";
        auto events{std::make_shared<uwvm2::ros_gateway::event_queue>(advertise)};
        action_probe::require(!adapter.advertise(advertise,events).error,"actual generated Action server advertisement failed");
        auto client{rclcpp_action::create_client<fibonacci>(native,advertise.resource)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(gateway);executor.add_node(native);
        action_probe::require(pump_until(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] {
            return client->action_server_is_ready();
        }),"actual native client did not discover Action server");
        fibonacci::Goal request;request.order=1;
        auto accepted{client->async_send_goal(request)};
        action_probe::require(pump_until(executor,std::chrono::steady_clock::now()+std::chrono::seconds(2),[&] {
            return accepted.wait_for(std::chrono::milliseconds(0))==std::future_status::ready;
        }),"actual native goal was not accepted");
        auto handle{accepted.get()};action_probe::require(handle!=nullptr,"actual native goal handle is null");
        auto result{client->async_get_result(handle)};
        uwvm2::imported::ros::ipc::call_result ticket;
        auto ticket_until{std::chrono::steady_clock::now()+std::chrono::milliseconds(500)};
        pump_until(executor,ticket_until,[&] { ticket=events->poll();return ticket.error!=11; });
        action_probe::require(!ticket.error&&ticket.payload.size()==12&&action_probe::ipc::get(ticket.payload.data()+8,4)==1,"actual incoming canonical goal ticket was not received");
        auto id{action_probe::ipc::get(ticket.payload.data(),8)};auto goal{find_goal(id)};
        action_probe::require(id&&goal,"incoming goal ownership is missing before deadline");
        std::chrono::steady_clock::time_point deadline;
        { std::lock_guard<std::mutex> lock{goal->mutex};deadline=goal->deadline; }
        // Give the actual GetResult RPC a pre-cutoff dispatch window, without
        // canceling an executor between taking a response and delivering it.
        auto settle_until{std::chrono::steady_clock::now()+std::chrono::milliseconds(100)};
        action_probe::require(settle_until<deadline,"native setup left no pre-deadline result dispatch window");
        pump_until(executor,settle_until,[] { return false; });
        action_probe::require(std::chrono::steady_clock::now()<deadline,"executor did not finish dispatch before the actual stored goal deadline");
        action_probe::require(result.wait_for(std::chrono::milliseconds(0))!=std::future_status::ready,"native goal was already terminal before cutoff probe");
        std::this_thread::sleep_until(deadline+std::chrono::milliseconds(30));
        {
            std::lock_guard<std::mutex> lock{goal->mutex};
            action_probe::require(goal->active&&!goal->terminal&&goal->timer,"deadline timer dispatched despite the unpumped executor");
        }
        auto wire{action_probe::ipc::encode_action_sequence({id,2,{0,1}},true)};
        auto denied{adapter.operation(respond,wire,1000)};
        action_probe::require(denied.error==110&&denied.payload.empty(),"expired incoming respond accepted native success or returned nonempty output");
        {
            std::lock_guard<std::mutex> lock{goal->mutex};
            action_probe::require(!goal->active&&goal->terminal&&goal->error==110&&!goal->timer,"expired incoming goal or timer was not retired");
        }
        action_probe::require(!find_goal(id)&&adapter.operation(respond,wire,1000).error==9,"expired incoming goal ID remains usable");
        action_probe::require(pump_until(executor,std::chrono::steady_clock::now()+std::chrono::seconds(3),[&] {
            return result.wait_for(std::chrono::milliseconds(0))==std::future_status::ready;
        }),"native terminal result did not arrive after executor pumping resumed");
        auto actual{result.get()};
        action_probe::require(actual.code==rclcpp_action::ResultCode::ABORTED&&actual.result&&actual.result->sequence.empty(),"expired incoming goal produced native SUCCEEDED or the forbidden success sequence");
        std::cout<<"PASS actual incoming Action accepted/event ticket before1500ms deadline, executor unpumped through deadline with no callback in flight, respond110 empty, ownership/timer retired, native ABORTED empty after pumping resumed\n";
        outgoing_cutoffs(adapter,native,executor);
        return 0;
    }
}

int main(int argc,char** argv)
{
    try { auto result{action_deadline_test::run(argc,argv)};rclcpp::shutdown();return result; }
    catch(std::exception const& error)
    { if(rclcpp::ok()) { rclcpp::shutdown(); }std::cerr<<"FAIL "<<error.what()<<'\n';return 1; }
}
