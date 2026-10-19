#define main uwvm_ros2_gateway_production_main
#include "../../gateways/ros2/main.cc"
#undef main
#include "probe_common.h"
#include <cstdlib>
#include <new>

namespace response_deadline_probe
{
    thread_local std::size_t wire_allocation_size{};
    thread_local bool legacy_state_wire_byte{};
    thread_local std::atomic<bool>* native_response_prepared{};
    thread_local bool delayed{};
    constexpr std::uint32_t budget_ms{1500};
    constexpr std::uint32_t stall_ms{1700};
    void disarm() noexcept { wire_allocation_size=0;legacy_state_wire_byte=false;native_response_prepared=nullptr; }
    ros_probe::core::capability_spec lifecycle(std::string operation)
    {
        using namespace ros_probe;
        bool get{operation=="lifecycle.get-state"};
        auto cap{service(core::version::ros2)};
        cap.alias=get?"state":"change";cap.operation=std::move(operation);
        cap.resource="/uwvm_response_deadline/lifecycle";
        cap.type=get?"lifecycle_msgs/srv/GetState":"lifecycle_msgs/srv/ChangeState";
        cap.type_identity=get?gateway::ros2_lifecycle::identity<lifecycle_msgs::srv::GetState>():gateway::ros2_lifecycle::identity<lifecycle_msgs::srv::ChangeState>();
        cap.limits["timeout-ms"]=std::to_string(budget_ms);cap.limits["max-response-bytes"]="16384";
        if(!get) { cap.limits["allowed-transition"]="configure"; }
        return cap;
    }
}
void* operator new(std::size_t size)
{
    using namespace response_deadline_probe;
    if((size==wire_allocation_size||(legacy_state_wire_byte&&size==1))&&native_response_prepared&&native_response_prepared->load())
    {
        disarm();delayed=true;
        // Only the calling thread's one exact wire allocation is delayed;
        // requests, discovery and native executor response allocations are not.
        std::this_thread::sleep_for(std::chrono::milliseconds(stall_ms));
    }
    if(auto memory=std::malloc(size?size:1)) { return memory; }
    throw std::bad_alloc{};
}
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete(void* memory,std::size_t) noexcept { std::free(memory); }

int main(int argc,char** argv)
{
    try
    {
        using namespace ros_probe;
        using namespace response_deadline_probe;
        require(argc<=2,"usage response-deadline [all|service|get|change]");
        std::string mode{argc==2?argv[1]:"all"};
        require(mode=="all"||mode=="service"||mode=="get"||mode=="change","unknown response-deadline mode");
        bool selected_service{mode=="all"||mode=="service"},selected_state{mode=="all"||mode=="get"},selected_change{mode=="all"||mode=="change"};
        rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("uwvm_response_deadline_adapter")};
        auto witness{std::make_shared<rclcpp::Node>("uwvm_response_deadline_witness")};
        ros2_adapter adapter{node};
        std::string trigger_message(4093,'s'),state_label(8193,'g');
        std::atomic<unsigned> trigger_requests{},state_requests{},change_requests{};
        std::atomic<bool> trigger_prepared{},state_prepared{},change_prepared{};
        std::atomic<unsigned> change_transition{};
        auto trigger{witness->create_service<std_srvs::srv::Trigger>("/uwvm_response_deadline/trigger",
            [&](std_srvs::srv::Trigger::Request::SharedPtr,std_srvs::srv::Trigger::Response::SharedPtr response)
            { ++trigger_requests;response->success=true;response->message=trigger_message;trigger_prepared=true; })};
        auto get_state{witness->create_service<lifecycle_msgs::srv::GetState>("/uwvm_response_deadline/lifecycle/get_state",
            [&](lifecycle_msgs::srv::GetState::Request::SharedPtr,lifecycle_msgs::srv::GetState::Response::SharedPtr response)
            { ++state_requests;response->current_state.id=lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE;response->current_state.label=state_label;state_prepared=true; })};
        auto change_state{witness->create_service<lifecycle_msgs::srv::ChangeState>("/uwvm_response_deadline/lifecycle/change_state",
            [&](lifecycle_msgs::srv::ChangeState::Request::SharedPtr request,lifecycle_msgs::srv::ChangeState::Response::SharedPtr response)
            { ++change_requests;change_transition=request->transition.id;response->success=true;change_prepared=true; })};
        rclcpp::executors::MultiThreadedExecutor executor;executor.add_node(node);executor.add_node(witness);
        std::thread spinning{[&] { executor.spin(); }};
        struct cleanup
        {
            rclcpp::Executor& executor;std::thread& thread;
            ~cleanup() { disarm();executor.cancel();thread.join();rclcpp::shutdown(); }
        } stop{executor,spinning};
        auto service_cap{service(core::version::ros2)};service_cap.resource="/uwvm_response_deadline/trigger";
        service_cap.limits["timeout-ms"]=std::to_string(budget_ms);service_cap.limits["max-response-bytes"]="16384";
        auto state_cap{lifecycle("lifecycle.get-state")},change_cap{lifecycle("lifecycle.change-state")};
        std::vector<std::uint8_t> expected_trigger{1};ipc::put(expected_trigger,trigger_message.size(),4);
        expected_trigger.insert(expected_trigger.end(),trigger_message.begin(),trigger_message.end());
        std::vector<std::uint8_t> expected_state{lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE};ipc::put(expected_state,state_label.size(),4);
        expected_state.insert(expected_state.end(),state_label.begin(),state_label.end());
        auto call_service=[&](std::uint32_t budget) { return adapter.service(service_cap,{},budget); };
        auto call_state=[&](std::uint32_t budget) { return gateway::ros2_lifecycle::call(node,state_cap,{},budget); };
        auto call_change=[&](std::uint32_t budget) { return gateway::ros2_lifecycle::call(node,change_cap,{},budget); };
        auto zero=[&](auto const& operation,std::atomic<unsigned> const& requests)
        {
            auto before{requests.load()};auto reply{operation(0)};
            require(reply.error==110&&reply.payload.empty(),"zero-budget native response operation must return110 empty");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            require(requests==before,"zero-budget operation dispatched an actual native request");
        };
        if(selected_service) { zero(call_service,trigger_requests); }
        if(selected_state) { zero(call_state,state_requests); }
        if(selected_change) { zero(call_change,change_requests); }
        auto normal=[&](auto const& operation,std::vector<std::uint8_t> const& expected)
        { auto reply{operation(budget_ms)};require(!reply.error&&reply.payload==expected,"normal actual native response did not preserve exact canonical wire"); };
        if(selected_service) { normal(call_service,expected_trigger); }
        if(selected_state) { normal(call_state,expected_state); }
        if(selected_change) { normal(call_change,std::vector<std::uint8_t>{1});require(change_transition==lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE,"Lifecycle native transition was not the independently configured configure request"); }
        auto late=[&](auto const& operation,std::atomic<unsigned> const& requests,std::atomic<bool>& prepared,std::size_t exact_wire_size,char const* expired_success,bool allow_legacy_state_byte=false)
        {
            prepared=false;delayed=false;wire_allocation_size=exact_wire_size;legacy_state_wire_byte=allow_legacy_state_byte;native_response_prepared=&prepared;
            auto before{requests.load()};auto start{std::chrono::steady_clock::now()};auto reply{operation(budget_ms)};
            disarm();
            require(delayed&&prepared&&requests==before+1,"one actual native response and exact final wire allocation barrier were not exercised");
            require(std::chrono::steady_clock::now()-start>=std::chrono::milliseconds(stall_ms),"final wire allocation barrier did not cross deadline");
            require(reply.error==110&&reply.payload.empty(),expired_success);
        };
        if(selected_service) { late(call_service,trigger_requests,trigger_prepared,expected_trigger.size(),"expired native Service response accepted success"); }
        // The frozen legacy GetState encoder starts with vector{stateID} (one
        // byte), whereas the candidate reserves the complete exact wire once.
        // Both allocations occur on this thread after pending.get() returned.
        if(selected_state) { late(call_state,state_requests,state_prepared,expected_state.size(),"expired native Lifecycle state response accepted success",true); }
        if(selected_change) { late(call_change,change_requests,change_prepared,1,"expired native Lifecycle change response accepted success"); }
        if(selected_service) { normal(call_service,expected_trigger); }
        if(selected_state) { normal(call_state,expected_state); }
        if(selected_change) { normal(call_change,std::vector<std::uint8_t>{1}); }
        require(trigger_requests==(selected_service?3u:0u)&&state_requests==(selected_state?3u:0u)&&change_requests==(selected_change?3u:0u),"actual zero/normal/late native request counts");
        std::cout<<"PASS actual ROS2 Trigger/GetState/ChangeState normal exact wire, timeout0 dispatches0 requests, ready native response wire allocation stalled1700ms beyond1500ms returns110 empty, selected native requests3 each mode="<<mode<<'\n';
        return 0;
    }
    catch(std::exception const& error) { response_deadline_probe::disarm();std::cerr<<error.what()<<'\n';return 1; }
}
