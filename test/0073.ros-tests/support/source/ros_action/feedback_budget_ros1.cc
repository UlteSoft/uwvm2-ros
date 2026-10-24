// Real ROS1 native feedback witness for advertiser/responder byte boundaries.
// Run with an isolated ROS_MASTER_URI only inside the qualified Linux 64 GiB
// cgroup. This prepared fixture calls the actual adapter and observes its ROS
// Actionlib feedback/result publications; it does not replace a Wasm E2E test.
#include "common_ros1.h"
#include "../../../../../src/ros/gateway/ros1/action.h"
#include <actionlib/client/simple_action_client.h>
#include <atomic>

namespace ros1_feedback_budget
{
    namespace adapter_ns=uwvm2::ros_gateway::ros1_action;
    using namespace ros_action_probe;
    unsigned checks{};
    void check(bool value,char const* message) { ++checks;require(value,message); }
    struct witness
    {
        std::mutex mutex;std::vector<std::vector<std::int32_t>> values;
        void record(std::vector<std::int32_t> const& value)
        { std::lock_guard<std::mutex> lock{mutex};values.push_back(value); }
        std::vector<std::vector<std::int32_t>> snapshot()
        { std::lock_guard<std::mutex> lock{mutex};return values; }
    };
    template<typename Ready> bool wait(Ready ready,std::chrono::milliseconds timeout)
    {
        auto deadline{std::chrono::steady_clock::now()+timeout};
        do { if(ready()) { return true; }std::this_thread::sleep_for(std::chrono::milliseconds(2)); }while(std::chrono::steady_clock::now()<deadline);
        return ready();
    }
    ipc::call_result incoming(std::shared_ptr<uwvm2::ros_gateway::event_queue> const& events)
    {
        ipc::call_result result;
        check(wait([&] { result=events->poll();return result.error!=11; },std::chrono::milliseconds(2000)),"actual incoming Action goal did not arrive");
        check(!result.error&&result.payload.size()==12,"incoming Action goal ticket is invalid");return result;
    }
    void scenario(::ros::NodeHandle& node,unsigned count,char const* boundary,unsigned ordinal)
    {
        std::string resource{"/uwvm_ros1_feedback_budget/case_"+std::to_string(ordinal)};
        auto advertise{grant(core::version::ros1,"advertise","action.advertise",resource)};
        auto respond{grant(core::version::ros1,"respond","action.respond",resource)};
        auto canonical_size{12U+4U*count},request_size{canonical_size+1U};
        bool advertiser_short{std::string{boundary}=="advertiser"},responder_short{std::string{boundary}=="responder"};
        advertise.limits["max-feedback-bytes"]=std::to_string(canonical_size-(advertiser_short?1U:0U));
        advertise.limits["max-result-bytes"]="13";
        respond.limits["max-feedback-bytes"]=std::to_string(request_size-(responder_short?1U:0U));
        respond.limits["max-result-bytes"]="21";
        gateway::validate(core::version::ros1,advertise);gateway::validate(core::version::ros1,respond);
        adapter_ns::adapter adapter{node};auto events{std::make_shared<gateway::event_queue>(advertise)};
        auto observed{std::make_shared<witness>()};
        boost::function<void(actionlib_tutorials::FibonacciActionFeedback::ConstPtr const&)> callback{
            [observed](actionlib_tutorials::FibonacciActionFeedback::ConstPtr const& message) { observed->record(message->feedback.sequence); }};
        auto subscription{node.subscribe<actionlib_tutorials::FibonacciActionFeedback>(resource+"/feedback",10,callback)};
        check(!adapter.advertise(advertise,events).error,"actual server advertisement failed");
        actionlib::SimpleActionClient<actionlib_tutorials::FibonacciAction> client{node,resource,false};
        check(client.waitForServer(::ros::Duration{3}),"actual native client did not discover server");
        check(wait([&] { return subscription.getNumPublishers()==1; },std::chrono::milliseconds(1000)),"independent feedback witness did not discover publisher");
        actionlib_tutorials::FibonacciGoal native_goal;native_goal.order=1;client.sendGoal(native_goal);
        auto ticket{incoming(events)};auto id{ipc::get(ticket.payload.data(),8)};
        std::vector<std::int32_t> values(count,7);
        auto feedback{ipc::encode_action_sequence({id,1,values},true)};
        check(feedback.size()==request_size,"independent canonical feedback request size differs");
        auto reply{adapter.call(respond,feedback,1000)};
        if(advertiser_short||responder_short)
        {
            check(reply.error==90&&reply.payload.empty(),"one-byte-short explicit feedback quota must reject");
            check(!client.waitForResult(::ros::Duration{0.05}),"feedback quota denial must retain live native goal");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            check(observed->snapshot().empty(),"denied feedback produced an actual native publication");
            if(advertiser_short) { values.resize(count-1); }
            else { respond.limits["max-feedback-bytes"]=std::to_string(request_size); }
            auto allowed{ipc::encode_action_sequence({id,1,values},true)};
            check(!adapter.call(respond,allowed,1000).error,"live goal cannot produce subsequent permitted feedback");
        }
        else { check(!reply.error&&reply.payload.empty(),"exact canonical advertiser feedback quota must succeed"); }
        check(wait([&] { return !observed->snapshot().empty(); },std::chrono::milliseconds(1000)),"permitted native feedback was not observed");
        check(observed->snapshot()==std::vector<std::vector<std::int32_t>>{values},"actual feedback values/count differ");
        // The outcome byte remains part of both terminal-result limits.
        auto oversized_result{ipc::encode_action_sequence({id,2,{3,5}},true)};
        check(oversized_result.size()==21&&adapter.call(respond,oversized_result,1000).error==90,"advertiser result quota must still include outcome byte");
        check(!client.waitForResult(::ros::Duration{0.05}),"result quota denial must retain live native goal");
        auto terminal{ipc::encode_action_sequence({id,2,{}},true)};
        check(terminal.size()==13&&!adapter.call(respond,terminal,1000).error,"exact thirteen-byte terminal result must succeed");
        check(client.waitForResult(::ros::Duration{3})&&client.getState()==actionlib::SimpleClientGoalState::SUCCEEDED&&client.getResult()->sequence.empty(),"native terminal result differs");
        check(adapter.call(respond,terminal,1000).error==9,"consumed goal must not be revived by a later response");
        adapter.close(advertise.alias);
    }
    void terminal_scenario(::ros::NodeHandle& node,std::uint8_t kind,char const* boundary,unsigned ordinal)
    {
        std::string resource{"/uwvm_ros1_feedback_budget/terminal_"+std::to_string(ordinal)};
        auto advertise{grant(core::version::ros1,"advertise","action.advertise",resource)};
        auto respond{grant(core::version::ros1,"respond","action.respond",resource)};
        bool advertiser_short{std::string{boundary}=="advertiser"},responder_short{std::string{boundary}=="responder"};
        advertise.limits["max-result-bytes"]=advertiser_short?"20":"21";
        respond.limits["max-result-bytes"]=responder_short?"20":"21";
        gateway::validate(core::version::ros1,advertise);gateway::validate(core::version::ros1,respond);
        adapter_ns::adapter adapter{node};auto events{std::make_shared<gateway::event_queue>(advertise)};
        check(!adapter.advertise(advertise,events).error,"terminal boundary server advertisement failed");
        actionlib::SimpleActionClient<actionlib_tutorials::FibonacciAction> client{node,resource,false};
        check(client.waitForServer(::ros::Duration{3}),"native terminal client did not discover server");
        actionlib_tutorials::FibonacciGoal goal;goal.order=1;client.sendGoal(goal);
        auto ticket{incoming(events)};auto id{ipc::get(ticket.payload.data(),8)};
        std::vector<std::int32_t> values{3,5};auto wire{ipc::encode_action_sequence({id,kind,values},true)};
        check(wire.size()==21,"terminal outcome/count/value framing differs");
        auto response{adapter.call(respond,wire,1000)};
        if(advertiser_short||responder_short)
        {
            check(response.error==90&&response.payload.empty(),"terminal one-byte-short quota must retain outcome-byte charge");
            check(!client.waitForResult(::ros::Duration{0.05}),"terminal quota denial must retain native goal");
            if(advertiser_short) { values.clear(); }
            else { respond.limits["max-result-bytes"]="21"; }
            wire=ipc::encode_action_sequence({id,kind,values},true);
            check(!adapter.call(respond,wire,1000).error,"permitted terminal response after denial failed");
        }
        else { check(!response.error,"exact terminal quota must include and permit outcome byte"); }
        auto expected_state{kind==2?actionlib::SimpleClientGoalState::SUCCEEDED:actionlib::SimpleClientGoalState::ABORTED};
        check(client.waitForResult(::ros::Duration{3})&&client.getState()==expected_state&&client.getResult()->sequence==values,"actual terminal status/values differ");
        adapter.close(advertise.alias);
    }
}
int main(int argc,char** argv)
{
    using namespace ros1_feedback_budget;
    try
    {
        ::ros::init(argc,argv,"uwvm_ros1_feedback_budget_probe",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);
        ::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();
        unsigned ordinal{};
        for(unsigned count:{0U,1U,2U,48U})
        {
            scenario(node,count,"exact",ordinal++);scenario(node,count,"responder",ordinal++);
            // Advertiser's public minimum is twelve bytes: its one-byte-short
            // empty case is rejected by configuration, rather than admitted.
            if(count) { scenario(node,count,"advertiser",ordinal++); }
        }
        for(std::uint8_t kind:{std::uint8_t{2},std::uint8_t{3}})
        { for(auto boundary:{"exact","advertiser","responder"}) { terminal_scenario(node,kind,boundary,ordinal++); } }
        spinner.stop();::ros::shutdown();gateway::native_io::fixed_text<160> result;
        result.append("PASS actual ROS1 feedback/result framing byte budgets scenarios=");result.number(ordinal);result.append(" checks=");result.number(checks);result.append("\n");
        gateway::native_io::write_all(STDOUT_FILENO,result.view());return 0;
    }
    catch(std::exception const& error)
    { uwvm2::ros_gateway::native_io::write_all(STDERR_FILENO,std::string{"FAIL "}+error.what()+"\n");return 1; }
}
