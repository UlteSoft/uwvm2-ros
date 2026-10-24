#define main uwvm_ros2_gateway_production_main
#include "../../../../../src/ros/gateway/ros2/main.cc"
#undef main
#include "../ros_ipc/probe_common.h"
#include <cstdlib>
#include <new>

namespace service_respond_probe
{
    thread_local std::size_t stalled_size{};
    thread_local bool stalled{};
    constexpr unsigned stall_ms{150};
    void disarm() noexcept { stalled_size=0; }
    void observed(std::string const& mode,std::uint32_t error,bool ready,bool exact,bool delayed)
    {
        gateway::native_io::fixed_text<512> text;
        ros_probe::require(text.append("OBSERVED service.respond mode=")&&text.append(mode)&&text.append(" error=")&&text.number(error)&&
            text.append(" native_ready=")&&text.number(unsigned(ready))&&text.append(" native_exact=")&&text.number(unsigned(exact))&&
            text.append(" allocation_stalled=")&&text.number(unsigned(delayed))&&text.append("\n"),"probe observation formatting");
        auto line{text.view()};ros_probe::require(::write(STDOUT_FILENO,line.data(),line.size())==static_cast<ssize_t>(line.size()),"probe observation write");
    }
}
void* operator new(std::size_t size)
{
    if(service_respond_probe::stalled_size&&size==service_respond_probe::stalled_size)
    {
        service_respond_probe::disarm();service_respond_probe::stalled=true;
        std::this_thread::sleep_for(std::chrono::milliseconds(service_respond_probe::stall_ms));
    }
    if(auto memory=std::malloc(size?size:1)) { return memory; }throw std::bad_alloc{};
}
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete(void* memory,std::size_t) noexcept { std::free(memory); }

int main(int argc,char** argv)
{
    try
    {
        using namespace ros_probe;
        using namespace service_respond_probe;
        require(argc==2,"usage respond-native-ros2 {normal|zero|call-wire|cap-wire|request-wire|native-copy}");
        std::string mode{argv[1]};require(mode=="normal"||mode=="zero"||mode=="call-wire"||mode=="cap-wire"||mode=="request-wire"||mode=="native-copy","unknown service.respond probe mode");
        rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("service_respond_deadline_adapter")};
        auto native{std::make_shared<rclcpp::Node>("service_respond_deadline_witness")};
        ros2_adapter adapter{node};
        auto cap{service(core::version::ros2)};cap.alias="server";cap.operation="service.advertise";cap.resource="/service_respond_deadline/trigger";
        cap.limits["timeout-ms"]=mode=="request-wire"?"50":"5000";cap.limits["max-response-bytes"]="4096";
        cap.limits["max-services"]="1";cap.limits["max-event-queue-depth"]="8";cap.limits["max-event-queue-bytes"]="8192";cap.limits["overflow-policy"]="reject";
        auto events{std::make_shared<gateway::event_queue>(cap)};require(!adapter.advertise(cap,events).error,"real generated Trigger service advertise");
        auto response_cap{cap};response_cap.alias="respond";response_cap.operation="service.respond";response_cap.limits["timeout-ms"]=mode=="cap-wire"?"50":"2000";
        auto client{native->create_client<std_srvs::srv::Trigger>(cap.resource,qos(cap))};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);executor.add_node(native);
        std::thread spinner{[&] { executor.spin(); }};
        struct stop
        {
            rclcpp::Executor& executor;std::thread& thread;
            ~stop() { disarm();executor.cancel();thread.join();rclcpp::shutdown(); }
        } cleanup{executor,spinner};
        require(client->wait_for_service(std::chrono::seconds(3)),"native service DDS discovery");
        auto request=[&]()
        {
            auto future{client->async_send_request(std::make_shared<std_srvs::srv::Trigger::Request>())};
            auto until{std::chrono::steady_clock::now()+std::chrono::seconds(2)};ipc::call_result event;
            do { event=events->poll();if(!event.error) { break; }require(event.error==11,"native request poll failed");std::this_thread::sleep_for(std::chrono::milliseconds(1)); } while(std::chrono::steady_clock::now()<until);
            require(!event.error&&event.payload.size()==8,"real native Trigger request did not reach event queue");
            return std::make_pair(std::move(future),ipc::get(event.payload.data(),8));
        };
        std::string text(2051,'r');
        auto response=[&](std::uint64_t id)
        { std::vector<std::uint8_t> wire;wire.reserve(13+text.size());ipc::put(wire,id,8);wire.push_back(1);ipc::put(wire,text.size(),4);wire.insert(wire.end(),text.begin(),text.end());return wire; };
        auto pending{request()};auto wire{response(pending.second)};
        if(mode!="normal"&&mode!="zero") { stalled_size=mode=="native-copy"?text.size()+1:5+text.size(); }
        auto started{std::chrono::steady_clock::now()};
        auto result{adapter.operation(response_cap,wire,mode=="zero"?0u:(mode=="call-wire"||mode=="native-copy"?50u:1000u))};disarm();
        bool ready{pending.first.wait_for(std::chrono::milliseconds(200))==std::future_status::ready};bool exact{};
        if(ready) { auto actual{pending.first.get()};exact=actual->success&&actual->message==text; }
        observed(mode,result.error,ready,exact,stalled);
        if(mode=="normal") { require(!result.error&&result.payload.empty()&&ready&&exact,"normal real native response differs"); }
        else
        {
            if(mode!="zero") { require(stalled&&std::chrono::steady_clock::now()-started>=std::chrono::milliseconds(stall_ms),"exact caller-thread response allocation barrier was not exercised"); }
            require(result.error==110&&result.payload.empty()&&!ready,"expired service respond budget accepted native response");
            if(mode=="request-wire")
            {
                client->remove_pending_request(pending.first);pending=request();wire=response(pending.second);
            }
            auto retry{adapter.operation(response_cap,wire,1000)};
            require(!retry.error&&retry.payload.empty(),"live request did not permit retry, or fresh request failed after native expiry");
            require(pending.first.wait_for(std::chrono::seconds(2))==std::future_status::ready,"retry real native response missing");
            auto actual{pending.first.get()};require(actual->success&&actual->message==text,"retry native canonical response content differs");
            require(adapter.operation(response_cap,wire,1000).error==9,"completed native request ID was not retired");
        }
        auto line="PASS service.respond caller/cap/request deadline, zero-budget native effects0, exact native bytes, retry and completed ID retirement\n";
        require(::write(STDOUT_FILENO,line,std::strlen(line))==static_cast<ssize_t>(std::strlen(line)),"probe PASS write");return 0;
    }
    catch(std::exception const& error) { service_respond_probe::disarm();std::fprintf(stderr,"FAIL %s\n",error.what());return 1; }
}
