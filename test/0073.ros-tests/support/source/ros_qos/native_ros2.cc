#include "common.h"
#include "../../../../../src/ros/gateway/ros2/action.h"
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <thread>
#include <tuple>

namespace native_action_qos_test
{
    using namespace action_qos_test;
    namespace action=gateway::ros2_action;
    using steady=std::chrono::steady_clock;
    using fibonacci=action::fibonacci;
    unsigned checks{};
    void require(bool value,std::string const& message) { ++checks;check(value,message); }
    template<typename Predicate> bool pump(rclcpp::executors::SingleThreadedExecutor& executor,
        steady::time_point until,Predicate predicate)
    {
        while(steady::now()<until)
        { executor.spin_some();if(predicate()) { return true; }std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
        return false;
    }
    std::shared_ptr<action::goal_state> find(std::uint64_t id)
    {
        std::lock_guard<std::mutex> guard{action::registry_mutex};auto it{action::registry.find(id)};
        return it==action::registry.end()?nullptr:it->second.lock();
    }
    struct retained_state
    {
        std::shared_ptr<action::goal_state> goal;
        std::shared_ptr<action::owner_state> owner;
        std::size_t owner_count{},registry_count{};
        std::tuple<bool,bool,bool,bool,bool,bool,bool,bool,std::uint32_t,
            steady::time_point,rclcpp::TimerBase::SharedPtr,std::shared_ptr<action::server_goal>,
            std::size_t,std::deque<std::vector<std::uint8_t>>,std::vector<std::uint8_t>> fields;
        explicit retained_state(std::shared_ptr<action::goal_state> value):goal{std::move(value)},owner{goal->owner.lock()}
        {
            require(owner!=nullptr,"actual incoming owner is missing");
            { std::lock_guard<std::mutex> guard{goal->mutex};fields={goal->active,goal->terminal,goal->incoming,
                goal->response_pending,goal->cleanup_pending,goal->native_started,goal->native_terminal,
                goal->guest_retired,goal->error,goal->deadline,goal->timer,goal->server,
                goal->feedback_bytes,goal->feedback,goal->result}; }
            { std::lock_guard<std::mutex> guard{owner->mutex};owner_count=owner->goals.size(); }
            { std::lock_guard<std::mutex> guard{action::registry_mutex};registry_count=action::registry.size(); }
        }
        void unchanged() const
        {
            require(find(goal->id)==goal,"QoS refusal changed goal registry ownership");
            { std::lock_guard<std::mutex> guard{goal->mutex};require(fields==decltype(fields){goal->active,
                goal->terminal,goal->incoming,goal->response_pending,goal->cleanup_pending,
                goal->native_started,goal->native_terminal,goal->guest_retired,goal->error,
                goal->deadline,goal->timer,goal->server,goal->feedback_bytes,goal->feedback,goal->result},
                "QoS refusal changed incoming state, deadline, timer, queues or native handle");
                require(goal->server->is_active()&&!goal->server->is_canceling(),"QoS refusal changed native execution state"); }
            { std::lock_guard<std::mutex> guard{owner->mutex};auto it{owner->goals.find(goal->id)};
                require(owner->goals.size()==owner_count&&it!=owner->goals.end()&&it->second==goal,
                    "QoS refusal changed owner goal quota"); }
            { std::lock_guard<std::mutex> guard{action::registry_mutex};require(action::registry.size()==registry_count,
                "QoS refusal changed global registry occupancy"); }
        }
    };
    int run(int argc,char** argv)
    {
        require(argc==1,"usage: uwvm-ros2-action-qos (no native assertion overrides)");
        rclcpp::init(argc,argv);rclcpp::NodeOptions options;
        options.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto gateway_node{std::make_shared<rclcpp::Node>("gateway_action_qos","/uwvm_action_qos",options)};
        auto native{std::make_shared<rclcpp::Node>("native_action_qos","/uwvm_action_qos",options)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(gateway_node);executor.add_node(native);
        auto cleanup{std::make_shared<action::cleanup_pool>(1)};action::adapter adapter{gateway_node,cleanup},foreign{gateway_node};
        auto advertise{capability("advertise","action.advertise")},respond{capability("respond","action.respond")};
        auto events{std::make_shared<gateway::event_queue>(advertise)};
        unsigned normal_feedback{},forbidden_feedback{};
        auto feedback_witness{native->create_subscription<fibonacci::Impl::FeedbackMessage>(advertise.resource+"/_action/feedback",
            rclcpp::QoS{8}.reliable(),[&](std::shared_ptr<fibonacci::Impl::FeedbackMessage const> message) {
                if(message->feedback.partial_sequence==std::vector<std::int32_t>{0,1}) { ++normal_feedback; }
                if(message->feedback.partial_sequence==std::vector<std::int32_t>{7,8}) { ++forbidden_feedback; }
            })};
        require(!adapter.advertise(advertise,events).error,"actual server advertisement failed");
        rcl_action_client_options_t client_options{};client_options.allocator=rcl_get_default_allocator();
        client_options.goal_service_qos=action::explicit_qos(advertise,"goal-service");
        client_options.cancel_service_qos=action::explicit_qos(advertise,"cancel-service");
        client_options.result_service_qos=action::explicit_qos(advertise,"result-service");
        client_options.feedback_topic_qos=action::explicit_qos(advertise,"feedback");
        client_options.status_topic_qos=action::explicit_qos(advertise,"status");
        auto client{rclcpp_action::create_client<fibonacci>(native,advertise.resource,nullptr,client_options)};
        require(pump(executor,steady::now()+std::chrono::seconds(4),[&] { return client->action_server_is_ready(); }),"native server discovery failed");
        require(pump(executor,steady::now()+std::chrono::seconds(3),[&] {
            return native->count_publishers(advertise.resource+"/_action/feedback")>=1&&native->count_subscribers(advertise.resource+"/_action/feedback")>=2;
        }),"independent native feedback witness discovery failed");
        auto send=[&] {
            fibonacci::Goal request;request.order=1;auto future{client->async_send_goal(request)};
            require(pump(executor,steady::now()+std::chrono::seconds(2),[&] { return future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"native goal acceptance did not complete");
            return future.get();
        };
        auto handle{send()};require(handle!=nullptr,"first native goal rejected");
        auto result_future{client->async_get_result(handle)};core::ipc::call_result ticket;
        require(pump(executor,steady::now()+std::chrono::milliseconds(500),[&] { ticket=events->poll();return ticket.error!=11; }),"incoming goal ticket missing");
        require(!ticket.error&&ticket.payload.size()==12,"incoming goal ticket wire differs");
        auto id{core::ipc::get(ticket.payload.data(),8)};auto goal{find(id)};require(id&&goal,"actual incoming goal registry missing");
        auto normal_wire{core::ipc::encode_action_sequence({id,1,{0,1}},true)};
        require(!adapter.operation(respond,normal_wire,1000).error,"matching 40-field normal feedback rejected");
        require(pump(executor,steady::now()+std::chrono::milliseconds(500),[&] { return normal_feedback==1; }),"exact baseline DDS feedback missing");
        require(send()==nullptr,"initial max-active-goals=1 quota not enforced");
        retained_state before{goal};auto cleanup_count{cleanup->count()};
        auto bad{respond};bad.limits["qos.feedback.depth"]="9";
        auto wire{core::ipc::encode_action_sequence({id,1,{7,8}},true)};
        auto refuse=[&](action::adapter& selected,core::capability_spec const& cap,
            std::vector<std::uint8_t> const& payload,std::uint32_t expected,std::string const& label) {
            auto answer{selected.operation(cap,payload,1000)};
            require(answer.error==expected&&answer.payload.empty(),label+": unexpected error or response bytes");
        };
        refuse(adapter,bad,{1},22,"malformed wire checked before QoS");
        auto unknown{id^0x8000000000000000ULL};while(find(unknown)) { --unknown; }
        refuse(adapter,bad,core::ipc::encode_action_sequence({unknown,1,{7,8}},true),9,"unknown owned goal checked before QoS");
        refuse(foreign,bad,wire,13,"foreign owner checked before QoS");
        auto wrong_resource{bad};wrong_resource.resource="/uwvm_action_qos/other";
        refuse(adapter,wrong_resource,wire,13,"resource ownership checked before QoS");
        { std::lock_guard<std::mutex> guard{goal->mutex};goal->error=110; }
        refuse(adapter,bad,wire,110,"stored timeout checked before QoS");
        { std::lock_guard<std::mutex> guard{goal->mutex};goal->error=0; }
        before.unchanged();unsigned valid_fields{},denials{};
        for(auto const& row:mutations)
        {
            if(!row.valid) { continue; }++valid_fields;
            auto cap{respond};cap.limits[row.key]=row.value;gateway::validate(core::version::ros2,cap);
            for(std::uint8_t kind:{1,2,3,4})
            {
                refuse(adapter,cap,core::ipc::encode_action_sequence({id,kind,{7,8}},true),13,std::string{row.key}+" kind="+std::to_string(kind));
                before.unchanged();require(cleanup->count()==cleanup_count,"QoS refusal changed cleanup occupancy");++denials;
            }
        }
        for(auto const& row:mutations)
        {
            if(std::string_view{row.key}.find(".depth")==std::string_view::npos) { continue; }
            auto cap{respond};cap.limits[row.key]="08";gateway::validate(core::version::ros2,cap);
            refuse(adapter,cap,wire,13,std::string{row.key}+" exact 8 versus 08");before.unchanged();++denials;
        }
        require(valid_fields==35&&denials==145,"native valid mismatch matrix was incomplete");
        pump(executor,steady::now()+std::chrono::milliseconds(150),[] { return false; });
        require(!forbidden_feedback&&normal_feedback==1,"mismatched contract produced native feedback");
        require(result_future.wait_for(std::chrono::milliseconds(0))!=std::future_status::ready,"mismatched contract produced terminal native result");
        require(events->poll().error==11,"mismatched contract produced an incoming event");
        before.unchanged();require(send()==nullptr,"QoS denials released live server reservation quota");
        auto normal_result{core::ipc::encode_action_sequence({id,2,{0,1}},true)};
        require(!adapter.operation(respond,normal_result,1000).error,"matching contract result after denials rejected");
        require(pump(executor,steady::now()+std::chrono::seconds(2),[&] { return result_future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"normal native result missing after denials");
        auto result{result_future.get()};require(result.code==rclcpp_action::ResultCode::SUCCEEDED&&result.result&&result.result->sequence==std::vector<std::int32_t>{0,1},"normal terminal result differs after denials");
        require(!find(id),"normal completion did not retire old goal registry");
        auto replacement{send()};require(replacement!=nullptr,"normal completion did not release server quota");
        core::ipc::call_result replacement_ticket;
        require(pump(executor,steady::now()+std::chrono::milliseconds(500),[&] { replacement_ticket=events->poll();return replacement_ticket.error!=11; }),"replacement goal ticket missing");
        require(!replacement_ticket.error&&replacement_ticket.payload.size()==12,"replacement ticket wire invalid");
        auto replacement_id{core::ipc::get(replacement_ticket.payload.data(),8)};
        auto replacement_result{client->async_get_result(replacement)};
        require(!adapter.operation(respond,core::ipc::encode_action_sequence({replacement_id,2,{0,1}},true),1000).error,"replacement matching result failed");
        require(pump(executor,steady::now()+std::chrono::seconds(2),[&] { return replacement_result.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"replacement native result missing");
        auto actual_replacement{replacement_result.get()};require(actual_replacement.code==rclcpp_action::ResultCode::SUCCEEDED&&actual_replacement.result&&actual_replacement.result->sequence==std::vector<std::int32_t>{0,1},"replacement exact native result differs");
        adapter.close(advertise.alias);
        refuse(adapter,bad,wire,9,"retired goal checked before QoS");
        line("PASS actual ROS2 incoming Action QoS legal_fields=35 denied_responses=145 forbidden_DDS_feedback=0 normal_DDS_feedback=1 goal_state_and_quota_preserved=1 checks="+std::to_string(checks));
        line("SCOPE stored-error priority uses a task-owned internal error=110 injection, restored before the mismatch matrix; it is not an actual timeout lifecycle observation");
        line("SCOPE explicit contract rejection; this does not assert that the RMW reports or applies all 40 requested QoS fields");
        (void)feedback_witness;return 0;
    }
}
int main(int argc,char** argv)
{
    try { auto result{native_action_qos_test::run(argc,argv)};rclcpp::shutdown();return result; }
    catch(std::exception const& error)
    {
        if(rclcpp::ok()) { rclcpp::shutdown(); }
        uwvm2::ros_gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1;
    }
}
