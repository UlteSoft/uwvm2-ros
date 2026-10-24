/** Real original-SDK protocol peers for the production industrial Gateway.
 *
 * Only the external owner may launch this executable, inside a bounded cgroup
 * and an isolated loopback network namespace. Synthetic values deliberately
 * cover transport fields, including empty arrays and nonphysical trajectories;
 * they must never be sent to a robot or used as motion examples. Each callback
 * checks original native fields. IPC bytes come from the independent Python
 * installed-IDL oracle, not a production codec round trip.
 */
#pragma once
#ifdef NDEBUG
#error "Original industrial protocol assertions must remain enabled"
#endif
#include "src/ros/gateway/common/config.h"
#include <uwvm2/imported/ros/common/manifest.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <fast_io.h>
#include <atomic>
#include <cassert>
#include <csignal>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

namespace gw=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
namespace ros=uwvm2::imported::ros;
using Clock=std::chrono::steady_clock;
using namespace std::chrono_literals;
template<class Native>struct oracle;

template<class Predicate>inline void eventually(Predicate predicate)
{
    auto until=Clock::now()+10s;
    while(!predicate()){assert(Clock::now()<until);std::this_thread::sleep_for(2ms);}
}
template<class Future>inline auto ready(Future& future)
{ assert(future.wait_for(10s)==std::future_status::ready);return future.get(); }
inline ipc::call_result call(ipc::client& client,std::string_view operation,std::string_view alias,
                             std::vector<std::uint8_t> bytes={})
{ return client.call(operation,alias,std::move(bytes),10000); }
inline void connect(ipc::client& client,gw::configuration const& configuration)
{
    eventually([&]{struct stat st{};return ::lstat(configuration.socket.c_str(),&st)==0&&S_ISSOCK(st.st_mode);});
    assert(client.connect(configuration.socket,2,::geteuid(),10000)==0);
}
inline void bind(ipc::client& client,gw::configuration const& configuration)
{ auto answer=call(client,"session.bind","target",ros::serialize_manifest(configuration.policy));assert(!answer.error&&answer.payload.empty()); }
inline ipc::call_result poll(ipc::client& client,std::string_view operation,std::string_view alias,
                             std::vector<std::uint8_t> bytes={})
{
    ipc::call_result answer;
    eventually([&]{answer=call(client,operation,alias,bytes);assert(!answer.error||answer.error==11);return !answer.error;});
    return answer;
}
inline std::vector<std::uint8_t> id_bytes(std::uint64_t id)
{ std::vector<std::uint8_t> bytes;ipc::put(bytes,id,8);return bytes; }
inline std::vector<std::uint8_t> prefix(std::uint64_t id,std::vector<std::uint8_t> body)
{ auto bytes=id_bytes(id);bytes.insert(bytes.end(),body.begin(),body.end());return bytes; }
inline void body_equals(std::vector<std::uint8_t> const& framed,std::size_t prefix_size,
                        std::vector<std::uint8_t> const& expected)
{
    assert(framed.size()==prefix_size+expected.size());
    assert(std::equal(expected.begin(),expected.end(),framed.begin()+prefix_size));
}
inline std::string alias(std::string_view root,std::string_view suffix)
{ std::string value(root);value.append(suffix);return value; }
inline std::string resource(std::string_view root,std::string_view suffix)
{ std::string value("/uwvm_industrial/");value.append(root);value.append(suffix);return value; }
struct context
{
    std::shared_ptr<rclcpp::Node> node;
    ipc::client& client;
    ipc::client& foreign;
    unsigned messages{},services{},actions{},topic_cases{},service_cases{},action_cases{};
};

template<class Native>void run_message(context& ctx,char const* root,char const* name)
{
    struct state {std::mutex mutex;std::vector<Native> observed;};
    auto seen=std::make_shared<state>();
    auto subscriber=ctx.node->create_subscription<Native>(resource(root,"_out"),rclcpp::QoS(16).reliable(),
        [seen](std::shared_ptr<Native const> value)
        {
            assert(*value==oracle<Native>::value(0)||*value==oracle<Native>::value(1)||*value==oracle<Native>::value(2));
            std::lock_guard<std::mutex> lock{seen->mutex};assert(seen->observed.size()<16);seen->observed.push_back(*value);
        });
    auto publisher=ctx.node->create_publisher<Native>(resource(root,"_in"),rclcpp::QoS(16).reliable());
    // A first volatile publication creates the Gateway publisher. Discovery
    // may occur after that sample; all three checked publications follow it.
    assert(!call(ctx.client,"topic.publish",alias(root,"_pub"),oracle<Native>::bytes(0)).error);
    eventually([&]{return subscriber->get_publisher_count()==1;});
    assert(!call(ctx.client,"topic.subscribe",alias(root,"_sub")).error);
    eventually([&]{return publisher->get_subscription_count()==1;});
    for(unsigned mode=0;mode!=3;++mode)
    {
        std::size_t begin{};{std::lock_guard<std::mutex> lock{seen->mutex};begin=seen->observed.size();}
        assert(!call(ctx.client,"topic.publish",alias(root,"_pub"),oracle<Native>::bytes(mode)).error);
        eventually([&]{std::lock_guard<std::mutex> lock{seen->mutex};
            return std::find(seen->observed.begin()+begin,seen->observed.end(),oracle<Native>::value(mode))!=seen->observed.end();});
        publisher->publish(oracle<Native>::value(mode));
        auto incoming=poll(ctx.client,"event.poll",alias(root,"_sub"));
        assert(incoming.payload==oracle<Native>::bytes(mode));++ctx.topic_cases;
    }
    ++ctx.messages;fast_io::io::println("PASS real-bidirectional-topic ",fast_io::mnp::os_c_str(name)," modes=3");
}

template<class Native>void run_service(context& ctx,char const* root,char const* name)
{
    using Request=typename Native::Request;using Response=typename Native::Response;
    struct state {std::atomic<unsigned> mode{},calls{};};auto seen=std::make_shared<state>();
    auto server=ctx.node->create_service<Native>(resource(root,"_peer"),
        [seen](std::shared_ptr<Request> request,std::shared_ptr<Response> response)
        {
            auto mode=seen->mode.load();assert(*request==oracle<Request>::value(mode));
            *response=oracle<Response>::value(mode);++seen->calls;
        });
    for(unsigned mode=0;mode!=3;++mode)
    {
        seen->mode=mode;
        auto damaged=oracle<Request>::bytes(mode);damaged.push_back(1);
        assert(call(ctx.client,"service.call",alias(root,"_call"),damaged).error==22&&seen->calls==mode);
        auto answer=call(ctx.client,"service.call",alias(root,"_call"),oracle<Request>::bytes(mode));
        assert(!answer.error&&answer.payload==oracle<Response>::bytes(mode)&&seen->calls==mode+1);
    }
    assert(!call(ctx.client,"service.advertise",alias(root,"_adv")).error);
    auto client=ctx.node->create_client<Native>(resource(root,"_gateway"));assert(client->wait_for_service(10s));
    for(unsigned mode=0;mode!=3;++mode)
    {
        auto future=client->async_send_request(std::make_shared<Request>(oracle<Request>::value(mode)));
        auto event=poll(ctx.client,"event.poll",alias(root,"_adv"));assert(event.payload.size()>=8);
        auto id=ipc::get(event.payload.data(),8);assert(id);body_equals(event.payload,8,oracle<Request>::bytes(mode));
        auto bytes=prefix(id,oracle<Response>::bytes(mode));
        assert(!call(ctx.client,"service.respond",alias(root,"_respond"),bytes).error);
        auto answer=ready(future);assert(answer&&*answer==oracle<Response>::value(mode));
        assert(call(ctx.client,"service.respond",alias(root,"_respond"),bytes).error==9);++ctx.service_cases;
    }
    ++ctx.services;fast_io::io::println("PASS real-bidirectional-service ",fast_io::mnp::os_c_str(name)," modes=3 malformed=3 duplicate=3");
}

template<class Native>void run_action(context& ctx,char const* root,char const* name)
{
    using Goal=typename Native::Goal;using Result=typename Native::Result;using Feedback=typename Native::Feedback;
    using Handle=rclcpp_action::ServerGoalHandle<Native>;
    struct state
    { std::atomic<unsigned> mode{},cancels{},feedback{};std::mutex mutex;std::shared_ptr<Handle> accepted; };
    auto seen=std::make_shared<state>();
    auto current=[seen]{std::lock_guard<std::mutex> lock{seen->mutex};return seen->accepted;};
    auto clear=[seen]{std::lock_guard<std::mutex> lock{seen->mutex};seen->accepted.reset();};
    auto server=rclcpp_action::create_server<Native>(ctx.node,resource(root,"_peer"),
        [seen](rclcpp_action::GoalUUID const&,std::shared_ptr<Goal const> goal)
        {assert(*goal==oracle<Goal>::value(seen->mode));return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;},
        [seen](std::shared_ptr<Handle>){++seen->cancels;return rclcpp_action::CancelResponse::ACCEPT;},
        [seen](std::shared_ptr<Handle> goal){std::lock_guard<std::mutex> lock{seen->mutex};assert(!seen->accepted);seen->accepted=goal;});
    for(unsigned mode=0;mode!=3;++mode)
    {
        seen->mode=mode;clear();auto sent=call(ctx.client,"action.send-goal",alias(root,"_send"),oracle<Goal>::bytes(mode));
        assert(!sent.error&&sent.payload.size()==8);auto id=ipc::get(sent.payload.data(),8);assert(id);
        eventually([&]{return bool(current());});
        assert(call(ctx.foreign,"action.receive-result",alias(root,"_result"),id_bytes(id)).error==13);
        assert(call(ctx.foreign,"action.cancel-own-goal",alias(root,"_cancel"),id_bytes(id)).error==13&&seen->cancels==0);
        current()->publish_feedback(std::make_shared<Feedback>(oracle<Feedback>::value(mode)));
        auto feedback=poll(ctx.client,"action.receive-feedback",alias(root,"_feedback"),id_bytes(id));
        assert(feedback.payload.size()>=8&&ipc::get(feedback.payload.data(),8)==id);body_equals(feedback.payload,8,oracle<Feedback>::bytes(mode));
        current()->succeed(std::make_shared<Result>(oracle<Result>::value(mode)));
        auto result=poll(ctx.client,"action.receive-result",alias(root,"_result"),id_bytes(id));
        assert(result.payload.size()>=9&&ipc::get(result.payload.data(),8)==id&&result.payload[8]==2);body_equals(result.payload,9,oracle<Result>::bytes(mode));
        assert(call(ctx.client,"action.receive-result",alias(root,"_result"),id_bytes(id)).error==9);
    }
    clear();auto sent=call(ctx.client,"action.send-goal",alias(root,"_send"),oracle<Goal>::bytes(2));
    assert(!sent.error&&sent.payload.size()==8);auto cancel_id=ipc::get(sent.payload.data(),8);eventually([&]{return bool(current());});
    assert(!call(ctx.client,"action.cancel-own-goal",alias(root,"_cancel"),id_bytes(cancel_id)).error);
    eventually([&]{return seen->cancels==1&&current()->is_canceling();});
    current()->canceled(std::make_shared<Result>(oracle<Result>::value(2)));
    auto canceled=poll(ctx.client,"action.receive-result",alias(root,"_result"),id_bytes(cancel_id));
    assert(canceled.payload.size()>=9&&canceled.payload[8]==4);body_equals(canceled.payload,9,oracle<Result>::bytes(2));
    assert(!call(ctx.client,"action.advertise",alias(root,"_adv")).error);
    auto client=rclcpp_action::create_client<Native>(ctx.node,resource(root,"_gateway"));assert(client->wait_for_action_server(10s));
    for(unsigned mode=0;mode!=3;++mode)
    {
        seen->mode=mode;typename rclcpp_action::Client<Native>::SendGoalOptions options;
        options.feedback_callback=[seen,mode](typename rclcpp_action::ClientGoalHandle<Native>::SharedPtr,std::shared_ptr<Feedback const> value)
            {assert(*value==oracle<Feedback>::value(mode));++seen->feedback;};
        auto goal=client->async_send_goal(oracle<Goal>::value(mode),options);auto handle=ready(goal);assert(handle);
        auto result=client->async_get_result(handle);auto event=poll(ctx.client,"event.poll",alias(root,"_adv"));assert(event.payload.size()>=8);
        auto id=ipc::get(event.payload.data(),8);assert(id);body_equals(event.payload,8,oracle<Goal>::bytes(mode));
        auto feedback=id_bytes(id);feedback.push_back(1);auto body=oracle<Feedback>::bytes(mode);feedback.insert(feedback.end(),body.begin(),body.end());
        assert(!call(ctx.client,"action.respond",alias(root,"_respond"),feedback).error);eventually([&]{return seen->feedback==mode+1;});
        auto terminal=id_bytes(id);terminal.push_back(2);body=oracle<Result>::bytes(mode);terminal.insert(terminal.end(),body.begin(),body.end());
        assert(!call(ctx.client,"action.respond",alias(root,"_respond"),terminal).error);
        auto answer=ready(result);assert(answer.code==rclcpp_action::ResultCode::SUCCEEDED&&answer.result&&*answer.result==oracle<Result>::value(mode));
        assert(call(ctx.client,"action.respond",alias(root,"_respond"),terminal).error==9);++ctx.action_cases;
    }
    ++ctx.actions;fast_io::io::println("PASS real-bidirectional-action ",fast_io::mnp::os_c_str(name)," modes=3 cancel=1 foreign-refusals=6");
}
