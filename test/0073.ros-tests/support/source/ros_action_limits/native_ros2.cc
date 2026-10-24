#include "../ros_action/ros2_common.h"
#include <rclcpp_action/rclcpp_action.hpp>
#include <action_tutorials_interfaces/action/fibonacci.hpp>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <fast_io.h>
#include <future>
#include <stdexcept>

namespace action_limits_probe
{
    using namespace action_probe;
    using fibonacci=action_tutorials_interfaces::action::Fibonacci;
    using clock=std::chrono::steady_clock;
    using feedback_message=fibonacci::Impl::FeedbackMessage;
    unsigned checks{};
    void check(bool value,char const* reason) { ++checks;require(value,reason); }
    template<typename Ready> bool pump(rclcpp::executors::SingleThreadedExecutor& executor,
        clock::time_point until,Ready ready)
    {
        while(clock::now()<until)
        { executor.spin_some();if(ready()) { return true; }std::this_thread::sleep_for(std::chrono::milliseconds(2)); }
        return ready();
    }
    bool ready(std::shared_future<rclcpp_action::ClientGoalHandle<fibonacci>::WrappedResult> const& future)
    { return future.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }
    rclcpp_action::ResultCode result_code(std::uint8_t kind)
    { return kind==2?rclcpp_action::ResultCode::SUCCEEDED:kind==3?rclcpp_action::ResultCode::ABORTED:rclcpp_action::ResultCode::CANCELED; }

    int run(int argc,char** argv)
    {
        check(argc==3,"usage native-action-limits gateway feedback|success|abort|canceled|feedback-edge|result-edge|responder-feedback|responder-result");
        std::string mode{argv[2]};
        std::string slug{mode};std::replace(slug.begin(),slug.end(),'-','_');
        bool feedback{mode=="feedback"||mode=="feedback-edge"||mode=="responder-feedback"};
        bool edge{mode=="feedback-edge"||mode=="result-edge"};
        bool responder_bound{mode=="responder-feedback"||mode=="responder-result"};
        check(mode=="feedback"||mode=="success"||mode=="abort"||mode=="canceled"||edge||responder_bound,"unknown Action limit mode");
        std::uint8_t kind{feedback?std::uint8_t{1}:mode=="abort"?std::uint8_t{3}:mode=="canceled"?std::uint8_t{4}:std::uint8_t{2}};
        std::string resource{"/uwvm_action_limits/"+slug+"_"+std::to_string(::getpid())};
        // Allow the native runner to witness the real ELF before startup. All
        // meaningful readiness below is SDK/DDS discovery or exact callback ACK.
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        fixture session{core::version::ros2,argv[1],[&](gateway::configuration& config) {
            config.policy.capabilities.clear();
            config.policy.connection["domain-id"]=::getenv("ROS_DOMAIN_ID")?::getenv("ROS_DOMAIN_ID"):"198";
            config.policy.connection["node-name"]="gateway_action_limits_"+slug;
            auto advertised{action(core::version::ros2,"advertised","action.advertise",resource)};
            auto response{action(core::version::ros2,"response","action.respond",resource)};
            advertised.limits["max-feedback-bytes"]="12";advertised.limits["max-result-bytes"]="13";
            response.limits["max-feedback-bytes"]="21";response.limits["max-result-bytes"]="21";
            if(edge) { response.limits["max-feedback-bytes"]="13";response.limits["max-result-bytes"]="13"; }
            if(responder_bound)
            { advertised.limits["max-feedback-bytes"]="20";advertised.limits["max-result-bytes"]="21";response.limits["max-feedback-bytes"]="13";response.limits["max-result-bytes"]="13"; }
            // The real Gateway config loader and TypeIdentity acceptance must
            // accept these separate explicit grants before any probe is useful.
            config.policy.capabilities.emplace(advertised.alias,std::move(advertised));
            config.policy.capabilities.emplace(response.alias,std::move(response));
        }};
        ipc::client rpc;session.connect(rpc);
        int init_argc{1};rclcpp::init(init_argc,argv);
        auto options{rclcpp::NodeOptions{}.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false)};
        options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto native{std::make_shared<rclcpp::Node>("native_action_limits_"+slug,options)};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(native);
        unsigned forbidden_feedback{},empty_feedback{};
        auto witness{native->create_subscription<feedback_message>(resource+"/_action/feedback",rclcpp::QoS{8}.reliable(),
            [&](feedback_message::ConstSharedPtr message) {
                if(message->feedback.partial_sequence==std::vector<std::int32_t>{7,8}) { ++forbidden_feedback; }
                if(message->feedback.partial_sequence.empty()) { ++empty_feedback; }
            })};
        auto client{rclcpp_action::create_client<fibonacci>(native,resource)};
        check(!rpc.call("action.advertise","advertised",{},1000).error,"different advertiser/responder byte limits rejected at startup");
        check(pump(executor,clock::now()+std::chrono::seconds(4),[&] { return client->action_server_is_ready()&&witness->get_publisher_count()==1&&native->count_subscribers(resource+"/_action/feedback")>=2; }),"actual Action server and independent DDS feedback discovery");
        fibonacci::Goal request;request.order=1;auto acceptance{client->async_send_goal(request)};
        check(pump(executor,clock::now()+std::chrono::seconds(2),[&] { return acceptance.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"actual goal acceptance ACK");
        auto handle{acceptance.get()};check(handle!=nullptr,"actual native goal rejected");
        auto result{client->async_get_result(handle)};
        auto ticket{poll(rpc,"event.poll","advertised",{})};
        check(!ticket.error&&ticket.payload.size()==12&&ipc::get(ticket.payload.data()+8,4)==1,"exact incoming GoalID event ACK");
        auto id{ipc::get(ticket.payload.data(),8)};
        if(kind==4)
        {
            auto cancel{client->async_cancel_goal(handle)};
            check(pump(executor,clock::now()+std::chrono::seconds(1),[&] { return cancel.wait_for(std::chrono::milliseconds(0))==std::future_status::ready; }),"native cancel ACK");
            check(!cancel.get()->goals_canceling.empty(),"native cancel preparation accepted");
        }
        std::vector<std::int32_t> values;if(!edge) { values={7,8}; }
        auto wire{ipc::encode_action_sequence({id,kind,values},true)};
        auto response{rpc.call("action.respond","response",wire,1000)};
        if(response.error==0)
        { check(pump(executor,clock::now()+std::chrono::seconds(1),[&] { return feedback?(edge?empty_feedback:forbidden_feedback)==1:ready(result); }),"successful oversized/edge reply has actual DDS/native effect ACK"); }
        else
        { pump(executor,clock::now()+std::chrono::milliseconds(150),[] { return false; }); }
        bool terminal{ready(result)};std::size_t native_values{};auto native_code{rclcpp_action::ResultCode::UNKNOWN};
        if(terminal) { auto actual{result.get()};native_code=actual.code;check(bool(actual.result),"native terminal Result object missing");native_values=actual.result->sequence.size(); }
        fast_io::io::println("OBSERVED action-limits mode=",mode," wire_bytes=",wire.size()," feedback_canonical_bytes=",wire.size()-(feedback?1:0)," error=",response.error," forbidden_DDS_feedback=",forbidden_feedback," empty_DDS_feedback=",empty_feedback," native_terminal=",terminal," native_code=",static_cast<int>(native_code)," native_values=",native_values);
        if(!edge)
        {
            check(response.error==90&&response.payload.empty(),"advertiser byte quota accepted oversized native Action response");
            check(!forbidden_feedback&&!terminal,"rejected Action response produced native feedback or terminal result");
            auto retry{ipc::encode_action_sequence({id,kind,{}},true)};
            check(!rpc.call("action.respond","response",retry,1000).error,"same live goal bounded response retry failed");
        }
        else { check(!response.error&&response.payload.empty(),"exact empty canonical Action boundary was rejected"); }
        if(feedback)
        {
            check(pump(executor,clock::now()+std::chrono::seconds(1),[&] { return empty_feedback==1; }),"native empty feedback exact boundary ACK");
            check(!rpc.call("action.respond","response",ipc::encode_action_sequence({id,2,{}},true),1000).error,"empty result completion after feedback");
            check(pump(executor,clock::now()+std::chrono::seconds(1),[&] { return ready(result); }),"native exact empty result after feedback ACK");
            auto actual{result.get()};check(actual.code==rclcpp_action::ResultCode::SUCCEEDED&&actual.result&&actual.result->sequence.empty(),"exact native terminal after feedback");
        }
        else if(!edge)
        {
            check(pump(executor,clock::now()+std::chrono::seconds(1),[&] { return ready(result); }),"native exact bounded result retry ACK");
            auto actual{result.get()};check(actual.code==result_code(kind)&&actual.result&&actual.result->sequence.empty(),"native retry exact empty outcome");
        }
        else { check(native_code==rclcpp_action::ResultCode::SUCCEEDED&&!native_values,"exact native empty edge result"); }
        check(rpc.call("action.respond","response",ipc::encode_action_sequence({id,2,{}},true),1000).error==9,"completed GoalID was not retired");
        check(!rpc.call("capability.close","advertised",{},1000).error,"advertised endpoint close failed");rpc.close();
        executor.remove_node(native);rclcpp::shutdown();
        fast_io::io::println("PASS action-limits mode=",mode," advertiser/responder native canonical byte quotas, live retry, exact empty DDS/result and retired GoalID checks=",checks);
        return 0;
    }
}
int main(int argc,char** argv)
{
    try { return action_limits_probe::run(argc,argv); }
    catch(std::exception const& error)
    { if(rclcpp::ok()) { rclcpp::shutdown(); }fast_io::io::println(fast_io::err(),"FAIL ",fast_io::mnp::os_c_str(error.what()));return 1; }
}
