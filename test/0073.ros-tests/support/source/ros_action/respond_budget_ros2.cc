#include "ros2_common.h"
#include "../../../../../src/ros/gateway/ros2/action.h"
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <cstdlib>
#include <iostream>
#include <new>
#include <thread>

namespace respond_budget_allocations
{
    // The first 8-byte allocation decodes two canonical values. The second
    // copies those values into the native Feedback/Result on this thread.
    // Executor and middleware allocation paths are never armed.
    thread_local bool armed{};
    thread_local unsigned matches{},stalls{};
    thread_local std::chrono::steady_clock::time_point release;
    void pause(std::size_t size)
    {
        if(armed&&size==2*sizeof(std::int32_t)&&++matches==2)
        { armed=false;++stalls;std::this_thread::sleep_until(release); }
    }
    struct barrier
    {
        explicit barrier(std::chrono::steady_clock::time_point until)
        { matches=stalls=0;release=until;armed=true; }
        ~barrier() { armed=false; }
    };
}
void* operator new(std::size_t size)
{
    respond_budget_allocations::pause(size);
    if(auto pointer=std::malloc(size?size:1)) { return pointer; }
    throw std::bad_alloc{};
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* pointer) noexcept { std::free(pointer); }
void operator delete[](void* pointer) noexcept { std::free(pointer); }
void operator delete(void* pointer,std::size_t) noexcept { std::free(pointer); }
void operator delete[](void* pointer,std::size_t) noexcept { std::free(pointer); }

namespace respond_budget_test
{
    namespace action=uwvm2::ros_gateway::ros2_action;
    using fibonacci=action::fibonacci;
    using steady=std::chrono::steady_clock;
    unsigned checks{};
    void check(bool value,char const* message) { ++checks;action_probe::require(value,message); }
    template<typename Ready> bool pump(rclcpp::executors::SingleThreadedExecutor& executor,
        steady::time_point until,Ready ready)
    {
        while(steady::now()<until)
        { executor.spin_some();if(ready()) { return true; }std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
        return false;
    }
    std::shared_ptr<action::goal_state> find(std::uint64_t id)
    {
        std::lock_guard<std::mutex> lock{action::registry_mutex};auto it{action::registry.find(id)};
        return it==action::registry.end()?nullptr:it->second.lock();
    }
    std::uint8_t kind(std::string const& value)
    { return value=="feedback"?1:value=="success"?2:value=="abort"?3:4; }
    rclcpp_action::ResultCode code(std::uint8_t value)
    { return value==2?rclcpp_action::ResultCode::SUCCEEDED:value==3?rclcpp_action::ResultCode::ABORTED:rclcpp_action::ResultCode::CANCELED; }

    void scenario(std::string const& phase,std::string const& response_kind,action::adapter& adapter,
        rclcpp::Node::SharedPtr const& native,rclcpp::executors::SingleThreadedExecutor& executor)
    {
        auto suffix{phase+"_"+response_kind};auto value{kind(response_kind)};
        auto advertise{action_probe::action(action_probe::core::version::ros2,"advertise"+suffix,"action.advertise","/uwvm_respond_budget/"+suffix)};
        auto respond{action_probe::action(action_probe::core::version::ros2,"respond"+suffix,"action.respond",advertise.resource)};
        if(phase=="goal") { advertise.limits["timeout-ms"]="1500"; }
        if(phase=="cap") { respond.limits["timeout-ms"]="100"; }
        auto events{std::make_shared<uwvm2::ros_gateway::event_queue>(advertise)};
        unsigned baseline_feedback{},forbidden_feedback{};
        using feedback_message=fibonacci::Impl::FeedbackMessage;
        auto feedback_witness{native->create_subscription<feedback_message>(advertise.resource+"/_action/feedback",rclcpp::QoS{8}.reliable(),
            [&](std::shared_ptr<feedback_message const> message) {
                if(message->feedback.partial_sequence==std::vector<std::int32_t>{0,1}) { ++baseline_feedback; }
                if(message->feedback.partial_sequence==std::vector<std::int32_t>{7,8}) { ++forbidden_feedback; }
            })};
        check(!adapter.advertise(advertise,events).error,"actual incoming server advertisement failed");
        auto client{rclcpp_action::create_client<fibonacci>(native,advertise.resource)};
        check(pump(executor,steady::now()+std::chrono::seconds(4),[&] { return client->action_server_is_ready(); }),"native Action server was not discovered");
        check(pump(executor,steady::now()+std::chrono::seconds(3),[&] {
            return native->count_publishers(advertise.resource+"/_action/feedback")>=1&&native->count_subscribers(advertise.resource+"/_action/feedback")>=2;
        }),"independent DDS feedback witness was not discovered before goal acceptance");
        fibonacci::Goal request;request.order=1;auto acceptance{client->async_send_goal(request)};
        check(pump(executor,steady::now()+std::chrono::seconds(2),[&] { return acceptance.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"native goal acceptance did not complete");
        auto handle{acceptance.get()};check(handle!=nullptr,"native goal was rejected");
        auto result_future{client->async_get_result(handle)};
        uwvm2::imported::ros::ipc::call_result ticket;
        check(pump(executor,steady::now()+std::chrono::milliseconds(500),[&] { ticket=events->poll();return ticket.error!=11; }),"incoming goal ticket did not arrive");
        check(!ticket.error&&ticket.payload.size()==12,"incoming goal ticket wire is invalid");
        auto id{action_probe::ipc::get(ticket.payload.data(),8)};auto goal{find(id)};check(id&&goal,"incoming goal registry is missing");
        steady::time_point goal_deadline;
        { std::lock_guard<std::mutex> lock{goal->mutex};goal_deadline=goal->deadline;check(goal->incoming&&goal->active&&!goal->terminal&&goal->server&&goal->timer,"actual incoming goal is not active"); }
        auto baseline_wire{action_probe::ipc::encode_action_sequence({id,1,{0,1}},true)};
        check(!adapter.operation(respond,baseline_wire,1000).error,"normal feedback before budget probe failed");
        check(pump(executor,steady::now()+std::chrono::milliseconds(500),[&] { return baseline_feedback==1; }),"normal native DDS feedback witness failed");
        if(value==4)
        {
            auto cancel{client->async_cancel_goal(handle)};
            check(pump(executor,steady::now()+std::chrono::milliseconds(500),[&] { return cancel.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"native cancel preparation did not complete");
            check(!cancel.get()->goals_canceling.empty(),"native cancel preparation was rejected");
            std::lock_guard<std::mutex> lock{goal->mutex};check(goal->server->is_canceling(),"native goal is not canceling before canceled response probe");
        }
        check(steady::now()+std::chrono::milliseconds(100)<goal_deadline,"native setup consumed the goal deadline");
        auto wire{action_probe::ipc::encode_action_sequence({id,value,{7,8}},true)};
        auto started{steady::now()};std::uint32_t budget{phase=="zero"?0U:phase=="call"?100U:phase=="cap"?1000U:3000U};
        uwvm2::imported::ros::ipc::call_result answer;
        if(phase=="zero") { answer=adapter.operation(respond,wire,budget); }
        else
        {
            respond_budget_allocations::barrier pause{phase=="goal"?goal_deadline+std::chrono::milliseconds(30):started+std::chrono::milliseconds(250)};
            answer=adapter.operation(respond,wire,budget);
        }
        auto elapsed{std::chrono::duration_cast<std::chrono::milliseconds>(steady::now()-started).count()};
        if(phase!="zero")
        { check(respond_budget_allocations::matches==2&&respond_budget_allocations::stalls==1,"native payload preparation allocation barrier was not hit exactly once"); }
        // Finish all dispatch cycles before checking effects. The independent
        // DDS subscription observes feedback even after the goal is terminal.
        pump(executor,steady::now()+std::chrono::milliseconds(150),[] { return false; });
        if(phase=="goal") { pump(executor,steady::now()+std::chrono::seconds(2),[&] { return result_future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }); }
        bool native_terminal{result_future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready};
        std::vector<std::int32_t> native_values;auto native_code{rclcpp_action::ResultCode::UNKNOWN};
        if(native_terminal) { auto result{result_future.get()};native_code=result.code;if(result.result) { native_values=result.result->sequence; } }
        std::cout<<"OBSERVED actual incoming respond mode="<<phase<<'-'<<response_kind<<" budget_ms="<<budget<<" cap_ms="<<respond.limits.at("timeout-ms")
            <<" error="<<answer.error<<" payload_bytes="<<answer.payload.size()<<" elapsed_ms="<<elapsed<<" preparation_stalls="<<(phase=="zero"?0:respond_budget_allocations::stalls)
            <<" baseline_DDS_feedback="<<baseline_feedback<<" forbidden_DDS_feedback="<<forbidden_feedback<<" native_terminal="<<native_terminal
            <<" native_code="<<static_cast<int>(native_code)<<" native_values="<<native_values.size()<<" registry_present="<<bool(find(id))<<'\n';
        check(answer.error==110&&answer.payload.empty(),"expired incoming respond budget accepted a native effect or success");
        check(!forbidden_feedback,"expired incoming respond published forbidden native DDS feedback");
        if(phase=="goal")
        {
            auto expired_code{value==4?rclcpp_action::ResultCode::CANCELED:rclcpp_action::ResultCode::ABORTED};
            check(native_terminal&&native_code==expired_code&&native_values.empty(),"expired stored goal did not produce the expected native empty terminal result");
            check(!find(id),"expired stored incoming goal registry remains live");
            std::lock_guard<std::mutex> lock{goal->mutex};check(!goal->active&&goal->terminal&&goal->error==110&&!goal->timer,"expired stored incoming goal or timer remains active");
        }
        else
        {
            check(!native_terminal&&find(id)==goal,"operation budget alone consumed the incoming goal");
            { std::lock_guard<std::mutex> lock{goal->mutex};check(goal->active&&!goal->terminal&&!goal->error&&goal->timer,"operation budget alone retired the incoming goal or timer"); }
            auto normal_value{value==1?std::uint8_t{2}:value};
            auto normal{action_probe::ipc::encode_action_sequence({id,normal_value,{0,1}},true)};
            check(!adapter.operation(respond,normal,1000).error,"normal retry after operation budget failed");
            check(pump(executor,steady::now()+std::chrono::seconds(2),[&] { return result_future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"normal retry native terminal result did not arrive");
            auto actual{result_future.get()};check(actual.code==code(normal_value)&&actual.result&&actual.result->sequence==std::vector<std::int32_t>{0,1},"normal retry native terminal result differs from exact expected values");
            check(!find(id),"normal incoming completion kept registry ownership");
        }
        adapter.close(advertise.alias);
        std::cout<<"PASS actual incoming respond budget "<<phase<<'-'<<response_kind<<":110 empty, no forbidden DDS/native result, "<<(phase=="goal"?"stored goal retired":"goal retained and exact normal retry completed")<<'\n';
        (void)feedback_witness;
    }
    int run(int argc,char** argv)
    {
        std::string selected{argc==2?argv[1]:"all"};check(argc<=2,"usage respond-budget [all|{zero,call,cap,goal}-{feedback,success,abort,canceled}]");
        rclcpp::init(argc,argv);rclcpp::NodeOptions options;
        options.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto gateway{std::make_shared<rclcpp::Node>("gateway_respond_budget","/uwvm_respond_budget",options)};
        auto native{std::make_shared<rclcpp::Node>("native_respond_budget","/uwvm_respond_budget",options)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(gateway);executor.add_node(native);action::adapter adapter{gateway};
        unsigned scenarios{};
        for(auto phase:{"zero","call","cap","goal"}) { for(auto response_kind:{"feedback","success","abort","canceled"})
        { if(selected=="all"||selected==std::string{phase}+"-"+response_kind) { scenario(phase,response_kind,adapter,native,executor);++scenarios; } } }
        check(scenarios>0,"invalid response budget scenario");std::cout<<"PASS actual ROS2 incoming respond budget scenarios="<<scenarios<<" checks="<<checks<<'\n';return 0;
    }
}
int main(int argc,char** argv)
{
    try { auto result{respond_budget_test::run(argc,argv)};rclcpp::shutdown();return result; }
    catch(std::exception const& error) { if(rclcpp::ok()) { rclcpp::shutdown(); }std::cerr<<"FAIL "<<error.what()<<'\n';return 1; }
}
