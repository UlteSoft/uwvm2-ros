/**
 * @file cancel_native_ros2.cc
 * @brief Prepared native cancellation preflight and actual-QoS fault fixture.
 *
 * Execute only in the verified Linux 64 GiB cgroup, using a fresh Jazzy build
 * and an isolated explicit ROS_DOMAIN_ID. There is no local execution claim.
 * Outgoing cases use actual Action clients and a server-side cancel counter.
 * Incoming cases deliberately interpose the two public rcl QoS query functions
 * on the calling thread. This controlled fault proves rejection ordering; it
 * is not evidence that the real RMW naturally downgraded a requested profile.
 * Link this executable with libdl. Never link its interposers into a product.
 */
#include "common.h"
#include "cancel_matrix.h"
#include "../../../../../src/ros/gateway/ros2/action.h"
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <rcl/client.h>
#include <rcl/graph.h>
#include <dlfcn.h>
#include <atomic>
#include <cstdlib>
#include <thread>
#include <tuple>

namespace cancellation_qos_test
{
    namespace action=uwvm2::ros_gateway::ros2_action;
    namespace gateway=uwvm2::ros_gateway;
    namespace core=uwvm2::imported::ros;
    using steady=std::chrono::steady_clock;
    using fibonacci=action::fibonacci;

    /** Thread-local injection affects only this fixture's incoming cancel call. */
    struct injection
    {
        enum class mode { off,request_mismatch,response_mismatch,request_missing,response_missing };
        mode selected{mode::off};
        rmw_qos_profile_t expected{},changed{};
        std::string service;
        unsigned request_queries{},response_queries{},discovery_queries{},requests{};
        bool applies(rcl_client_t const* client) const
        {
            auto name{rcl_client_get_service_name(client)};
            return selected!=mode::off&&name&&service==name;
        }
    };
    inline thread_local injection fault;

    /** Resolve the real dynamic-library symbol; an unavailable hook is a failure. */
    template<typename Function> Function next(char const* name)
    {
        auto symbol{::dlsym(RTLD_NEXT,name)};
        if(!symbol) { std::abort(); }
        return reinterpret_cast<Function>(symbol);
    }
}

/** Test-only request-QoS fault at the API used by rclcpp::ClientBase. */
extern "C" rmw_qos_profile_t const* rcl_client_request_publisher_get_actual_qos(rcl_client_t const* client)
{
    using namespace cancellation_qos_test;
    if(fault.applies(client))
    {
        ++fault.request_queries;
        if(fault.selected==injection::mode::request_missing) { return nullptr; }
        return fault.selected==injection::mode::request_mismatch?&fault.changed:&fault.expected;
    }
    using function=rmw_qos_profile_t const*(*)(rcl_client_t const*);
    static auto original{next<function>("rcl_client_request_publisher_get_actual_qos")};return original(client);
}

/** Test-only response-QoS fault; all other callers retain the real implementation. */
extern "C" rmw_qos_profile_t const* rcl_client_response_subscription_get_actual_qos(rcl_client_t const* client)
{
    using namespace cancellation_qos_test;
    if(fault.applies(client))
    {
        ++fault.response_queries;
        if(fault.selected==injection::mode::response_missing) { return nullptr; }
        return fault.selected==injection::mode::response_mismatch?&fault.changed:&fault.expected;
    }
    using function=rmw_qos_profile_t const*(*)(rcl_client_t const*);
    static auto original{next<function>("rcl_client_response_subscription_get_actual_qos")};return original(client);
}

/** Count service discovery under injection, before delegating to the real graph. */
extern "C" rcl_ret_t rcl_service_server_is_available(rcl_node_t const* node,rcl_client_t const* client,bool* available)
{
    using namespace cancellation_qos_test;
    if(fault.applies(client)) { ++fault.discovery_queries; }
    using function=rcl_ret_t(*)(rcl_node_t const*,rcl_client_t const*,bool*);
    static auto original{next<function>("rcl_service_server_is_available")};return original(node,client,available);
}

/** Count native service requests under injection, before delegating to rcl. */
extern "C" rcl_ret_t rcl_send_request(rcl_client_t const* client,void const* request,std::int64_t* sequence)
{
    using namespace cancellation_qos_test;
    if(fault.applies(client)) { ++fault.requests; }
    using function=rcl_ret_t(*)(rcl_client_t const*,void const*,std::int64_t*);
    static auto original{next<function>("rcl_send_request")};return original(client,request,sequence);
}

namespace cancellation_qos_test
{
    unsigned checks{};
    void require(bool value,std::string const& label) { ++checks;action_qos_test::check(value,label); }
    template<typename Predicate> bool wait(steady::time_point until,Predicate predicate)
    {
        while(steady::now()<until)
        { if(predicate()) { return true; }std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
        return predicate();
    }
    struct spinning
    {
        rclcpp::executors::SingleThreadedExecutor& executor;
        std::exception_ptr error;
        std::thread thread;
        explicit spinning(rclcpp::executors::SingleThreadedExecutor& e):executor{e},thread{[this] {
            try { executor.spin(); }catch(...) { error=std::current_exception(); }
        }} {}
        void stop() { executor.cancel();if(thread.joinable()) { thread.join(); }if(error) { std::rethrow_exception(error); } }
        ~spinning() { executor.cancel();if(thread.joinable()) { thread.join(); } }
    };
    core::capability_spec capability(std::string alias,std::string operation,std::string resource)
    {
        auto cap{action_qos_test::capability(std::move(alias),std::move(operation),std::move(resource))};
        cap.limits["max-active-goals"]="2";
        if(cap.operation=="action.cancel-own-goal") { cap.limits["cancel-scope"]="own"; }
        if(cap.operation=="action.cancel-any-goal") { cap.limits["cancel-scope"]="any"; }
        return cap;
    }
    std::vector<std::uint8_t> id_wire(std::uint64_t id)
    { std::vector<std::uint8_t> wire;core::ipc::put(wire,id,8);return wire; }
    std::shared_ptr<action::goal_state> find(std::uint64_t id)
    {
        std::lock_guard<std::mutex> guard{action::registry_mutex};auto it{action::registry.find(id)};
        return it==action::registry.end()?nullptr:it->second.lock();
    }
    struct retained
    {
        std::shared_ptr<action::goal_state> goal;
        std::tuple<bool,bool,bool,bool,bool,bool,bool,bool,std::uint32_t,steady::time_point,
            rclcpp::TimerBase::SharedPtr,action::client_goal::SharedPtr,std::shared_ptr<action::server_goal>> state;
        explicit retained(std::shared_ptr<action::goal_state> g):goal{std::move(g)}
        {
            require(goal!=nullptr,"actual Goal registry entry missing");
            std::lock_guard<std::mutex> guard{goal->mutex};state={goal->active,goal->terminal,goal->incoming,
                goal->response_pending,goal->cleanup_pending,goal->native_started,goal->native_terminal,
                goal->guest_retired,goal->error,goal->deadline,goal->timer,goal->outgoing,goal->server};
        }
        void unchanged() const
        {
            require(find(goal->id)==goal,"cancellation refusal retired registry ownership");
            std::lock_guard<std::mutex> guard{goal->mutex};
            require(state==decltype(state){goal->active,goal->terminal,goal->incoming,
                goal->response_pending,goal->cleanup_pending,goal->native_started,goal->native_terminal,
                goal->guest_retired,goal->error,goal->deadline,goal->timer,goal->outgoing,goal->server},
                "cancellation refusal changed Goal state, handle or deadline");
        }
    };
    rcl_action_server_options_t server_options(core::capability_spec const& cap)
    {
        auto options{rcl_action_server_get_default_options()};
        options.goal_service_qos=action::explicit_qos(cap,"goal-service");
        options.cancel_service_qos=action::explicit_qos(cap,"cancel-service");
        options.result_service_qos=action::explicit_qos(cap,"result-service");
        options.feedback_topic_qos=action::explicit_qos(cap,"feedback");
        options.status_topic_qos=action::explicit_qos(cap,"status");return options;
    }
    int run(int argc,char** argv)
    {
        require(argc==1,"usage: cancellation QoS fixture without assertion overrides");
        rclcpp::init(argc,argv);rclcpp::NodeOptions options;
        options.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto node{std::make_shared<rclcpp::Node>("cancel_gateway","/uwvm_cancel_qos",options)};
        auto native{std::make_shared<rclcpp::Node>("cancel_native","/uwvm_cancel_qos",options)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);executor.add_node(native);
        spinning spinner{executor};std::atomic<unsigned> native_cancels{};
        std::mutex handles_mutex;std::map<std::string,std::shared_ptr<action::server_goal>> handles;
        auto sent{capability("sent","action.send-goal","/uwvm_cancel_qos/outgoing")};
        auto server{rclcpp_action::create_server<fibonacci>(native,sent.resource,
            [](rclcpp_action::GoalUUID const&,std::shared_ptr<fibonacci::Goal const>) { return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE; },
            [&](std::shared_ptr<action::server_goal>) { ++native_cancels;return rclcpp_action::CancelResponse::ACCEPT; },
            [&](std::shared_ptr<action::server_goal> handle) { auto key{action::uuid(handle->get_goal_id())};std::lock_guard<std::mutex> guard{handles_mutex};handles.emplace(std::move(key),std::move(handle)); },
            server_options(sent))};
        auto cleanup{std::make_shared<action::cleanup_pool>(4)};action::adapter first{node,cleanup},second{node,cleanup};
        auto send=[&](action::adapter& adapter,core::capability_spec const& cap) {
            std::vector<std::uint8_t> order;core::ipc::put(order,1,4);
            auto result{adapter.operation(cap,order,4000)};require(!result.error&&result.payload.size()==8,"actual outgoing Goal send failed");
            auto id{core::ipc::get(result.payload.data(),8)};require(id&&find(id),"outgoing Goal ID missing");return id;
        };
        auto cancel=[&](action::adapter& adapter,core::capability_spec const& cap,std::uint64_t id,std::uint32_t expected) {
            auto result{adapter.operation(cap,id_wire(id),1000)};
            require(result.error==expected&&(expected?result.payload.empty():result.payload==std::vector<std::uint8_t>{1}),"unexpected cancellation result");
        };
        auto complete=[&](action::adapter& adapter,std::uint64_t id,core::capability_spec const& origin) {
            auto goal{find(id)};require(goal!=nullptr,"Goal disappeared before native completion");action::client_goal::SharedPtr outgoing;
            { std::lock_guard<std::mutex> guard{goal->mutex};outgoing=goal->outgoing; }
            require(outgoing!=nullptr,"outgoing native handle missing");std::shared_ptr<action::server_goal> handle;
            require(wait(steady::now()+std::chrono::seconds(1),[&] {
                std::lock_guard<std::mutex> guard{handles_mutex};auto it{handles.find(action::uuid(outgoing->get_goal_id()))};
                if(it==handles.end()) { return false; }handle=it->second;return handle->is_canceling();
            }),"native server did not enter canceling");
            auto result{std::make_shared<fibonacci::Result>()};result->sequence={0,1};handle->canceled(result);
            auto receive{capability("result","action.receive-result",origin.resource)};
            receive.limits["qos.cancel-service.depth"]=origin.limits.at("qos.cancel-service.depth");
            core::ipc::call_result terminal;
            require(wait(steady::now()+std::chrono::seconds(2),[&] { terminal=adapter.operation(receive,id_wire(id),1000);return terminal.error!=11; }),"native canceled result not delivered");
            require(!terminal.error&&terminal.payload.size()>=13&&terminal.payload[8]==4,"actual canceled result differs");
            require(!find(id),"completed Goal registry not retired");
        };
        auto own{capability("own","action.cancel-own-goal",sent.resource)};
        auto id{send(first,sent)};retained before{find(id)};auto before_count{native_cancels.load()};unsigned denials{};
        for(auto const& row:mutations)
        {
            if(!row.individually_valid) { continue; }
            auto changed{own};changed.limits[row.key]=row.replacement;gateway::validate(core::version::ros2,changed);
            cancel(first,changed,id,13);before.unchanged();require(native_cancels.load()==before_count,"own mismatch caused native cancellation");++denials;
        }
        auto padded{own};padded.limits["qos.cancel-service.depth"]="08";cancel(first,padded,id,13);++denials;
        cancel(second,own,id,13);before.unchanged();
        auto zero_budget{first.operation(own,id_wire(id),0)};require(zero_budget.error==110,"zero cancellation budget lost timeout priority");
        auto low_quota{capability("any-low","action.cancel-any-goal",sent.resource)};low_quota.limits["max-active-goals"]="1";
        cancel(first,own,id,0);require(native_cancels.load()==before_count+1,"matching own cancellation did not reach native server");complete(first,id,sent);

        auto first_id{send(first,sent)};auto different{sent};different.alias="second-sent";different.limits["qos.cancel-service.depth"]="9";
        auto second_id{send(second,different)};require(first_id<second_id,"later-mismatch fixture did not preserve registry iteration order");
        auto any{capability("any","action.cancel-any-goal",sent.resource)};retained first_before{find(first_id)},second_before{find(second_id)};
        before_count=native_cancels.load();cancel(first,low_quota,0,11);
        cancel(first,any,0,13);first_before.unchanged();second_before.unchanged();
        require(native_cancels.load()==before_count,"later any-goal mismatch canceled an earlier matching Goal");
        auto different_own{own};different_own.limits["qos.cancel-service.depth"]="9";
        cancel(second,different_own,second_id,0);complete(second,second_id,different);
        cancel(first,any,0,0);complete(first,first_id,sent);

        // Create an actual incoming Goal through an independent native client.
        auto advertise{capability("incoming","action.advertise","/uwvm_cancel_qos/incoming")};
        auto events{std::make_shared<gateway::event_queue>(advertise)};require(!first.advertise(advertise,events).error,"incoming advertisement failed");
        rcl_action_client_options_t client_options{};client_options.allocator=rcl_get_default_allocator();
        client_options.goal_service_qos=action::explicit_qos(advertise,"goal-service");client_options.cancel_service_qos=action::explicit_qos(advertise,"cancel-service");
        client_options.result_service_qos=action::explicit_qos(advertise,"result-service");client_options.feedback_topic_qos=action::explicit_qos(advertise,"feedback");
        client_options.status_topic_qos=action::explicit_qos(advertise,"status");
        auto incoming_client{rclcpp_action::create_client<fibonacci>(native,advertise.resource,nullptr,client_options)};
        require(wait(steady::now()+std::chrono::seconds(4),[&] { return incoming_client->action_server_is_ready(); }),"incoming server discovery failed");
        fibonacci::Goal request;request.order=1;auto accepted{incoming_client->async_send_goal(request)};
        require(accepted.wait_for(std::chrono::seconds(2))==std::future_status::ready,"incoming Goal acceptance timed out");
        auto incoming_handle{accepted.get()};require(incoming_handle!=nullptr,"incoming native Goal rejected");
        auto incoming_result{incoming_client->async_get_result(incoming_handle)};core::ipc::call_result ticket;
        require(wait(steady::now()+std::chrono::seconds(1),[&] { ticket=events->poll();return ticket.error!=11; }),"incoming Goal event missing");
        require(!ticket.error&&ticket.payload.size()==12,"incoming Goal event malformed");auto incoming_id{core::ipc::get(ticket.payload.data(),8)};
        auto incoming_cancel{capability("incoming-own","action.cancel-own-goal",advertise.resource)};retained incoming_before{find(incoming_id)};
        for(auto mode:{injection::mode::request_mismatch,injection::mode::response_mismatch,injection::mode::request_missing,injection::mode::response_missing})
        {
            fault={};fault.selected=mode;fault.service=advertise.resource+"/_action/cancel_goal";
            fault.expected=action::explicit_qos(incoming_cancel,"cancel-service");fault.changed=fault.expected;fault.changed.depth=9;
            cancel(first,incoming_cancel,incoming_id,95);
            require(fault.request_queries==1,"request actual-QoS interposer did not run exactly once");
            require(fault.response_queries==((mode==injection::mode::response_mismatch||mode==injection::mode::response_missing)?1u:0u),"response actual-QoS query ordering differs");
            require(!fault.discovery_queries&&!fault.requests,"incoming mismatch reached discovery or native request");
            incoming_before.unchanged();
            { auto goal{find(incoming_id)};std::lock_guard<std::mutex> guard{goal->mutex};require(goal->server->is_active()&&!goal->server->is_canceling(),"injected QoS refusal canceled native incoming Goal"); }
            fault={};
        }
        // No injection: the actual RMW must report a matching profile for success.
        cancel(first,incoming_cancel,incoming_id,0);
        auto respond{capability("incoming-response","action.respond",advertise.resource)};
        auto outcome{first.operation(respond,core::ipc::encode_action_sequence({incoming_id,4,{0,1}},true),1000)};
        require(!outcome.error,"matching incoming cancellation could not produce canceled result");
        require(incoming_result.wait_for(std::chrono::seconds(2))==std::future_status::ready,"incoming canceled result timed out");
        require(incoming_result.get().code==rclcpp_action::ResultCode::CANCELED,"incoming native outcome differs");first.close(advertise.alias);
        require(denials==8&&cleanup->count()==0,"cancellation matrix or cleanup quota incomplete");
        spinner.stop();action_qos_test::line("PASS actual cancellation preflight own_denials=8 later_any_mismatch_no_effect=1 matching_own_any_incoming=1 injected_actual_qos_refusals=4 checks="+std::to_string(checks));
        action_qos_test::line("SCOPE incoming downgrade/missing-QoS cases use test-only thread-local rcl query interposition; no natural RMW downgrade is claimed");
        (void)server;return 0;
    }
}

int main(int argc,char** argv)
{
    try { auto result{cancellation_qos_test::run(argc,argv)};rclcpp::shutdown();return result; }
    catch(std::exception const& error)
    {
        cancellation_qos_test::fault={};if(rclcpp::ok()) { rclcpp::shutdown(); }
        uwvm2::ros_gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1;
    }
}
