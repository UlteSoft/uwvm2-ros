#include "../../../../../src/ros/gateway/common/service.h"
#include "../ros_ipc/probe_common.h"
#include <cstdlib>
#include <new>

namespace service_state_test
{
    thread_local std::size_t allocation_size{};
    thread_local bool delayed{};
    std::atomic<bool> preparing{};
    unsigned checks{};
    void check(bool value,char const* message) { ++checks;ros_probe::require(value,message); }
    void disarm() noexcept { allocation_size=0; }
}
void* operator new(std::size_t size)
{
    if(service_state_test::allocation_size&&size==service_state_test::allocation_size)
    { service_state_test::disarm();service_state_test::delayed=true;service_state_test::preparing=true;std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
    if(auto memory=std::malloc(size?size:1)) { return memory; }throw std::bad_alloc{};
}
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete(void* memory,std::size_t) noexcept { std::free(memory); }

int main()
{
    try
    {
        using namespace service_state_test;
        namespace gw=uwvm2::ros_gateway;
        namespace ipc=uwvm2::imported::ros::ipc;
        for(auto version:{ros_probe::core::version::ros1,ros_probe::core::version::ros2})
        {
            auto cap{ros_probe::service(version)};cap.operation="service.advertise";cap.limits["timeout-ms"]="2000";cap.limits["max-response-bytes"]="4096";
            cap.limits["max-event-queue-depth"]="8";cap.limits["max-event-queue-bytes"]="8192";cap.limits["overflow-policy"]="reject";
            auto response_cap{cap};response_cap.operation="service.respond";std::string text(2051,'u');
            auto wire=[&](std::uint64_t id)
            { std::vector<std::uint8_t> bytes;bytes.reserve(13+text.size());ipc::put(bytes,id,8);bytes.push_back(1);ipc::put(bytes,text.size(),4);bytes.insert(bytes.end(),text.begin(),text.end());return bytes; };
            auto fixture=[&]() { return std::make_shared<gw::service_state>(cap,std::make_shared<gw::event_queue>(cap)); };
            {
                auto state{fixture()};auto pending{state->begin({})};check(bool(pending),"normal pending");auto data{wire(pending->id)};
                check(state->answer(response_cap,data,ipc::after(0)).error==110,"zero budget");check(!pending->answered&&pending->response.empty()&&state->pending.size()==1,"zero budget changed pending");
                check(!state->answer(response_cap,data,ipc::after(500)).error,"normal retry");
                check(state->answer(response_cap,data,ipc::after(500)).error==110&&!pending->cancelled,"duplicate response cancelled already committed answer");
                std::vector<std::uint8_t> output{9};check(state->wait(pending,output)&&output==std::vector<std::uint8_t>(data.begin()+8,data.end()),"exact committed response wait");
                check(state->answer(response_cap,data,ipc::after(500)).error==9,"completed request ID reuse");
            }
            {
                auto state{fixture()};auto pending{state->begin({})};auto data{wire(pending->id)};allocation_size=5+text.size();delayed=false;
                check(state->answer(response_cap,data,ipc::after(50)).error==110&&delayed,"wire preparation ignored call deadline");
                check(!pending->answered&&pending->response.empty()&&state->pending.size()==1,"expired preparation consumed request");
                check(!state->answer(response_cap,data,ipc::after(500)).error,"live retry after wire timeout");
                std::vector<std::uint8_t> output;check(state->wait(pending,output)&&output.size()==5+text.size(),"wire retry wait");
            }
            {
                auto short_cap{cap};short_cap.limits["timeout-ms"]="50";auto state{std::make_shared<gw::service_state>(short_cap,std::make_shared<gw::event_queue>(short_cap))};
                auto pending{state->begin({})};auto data{wire(pending->id)};allocation_size=5+text.size();delayed=false;
                check(state->answer(response_cap,data,ipc::after(1000)).error==110&&delayed,"wire preparation ignored native request deadline");
                check(state->pending.empty()&&pending->cancelled&&!pending->answered,"expired native request not retired");
            }
            {
                auto state{fixture()};auto pending{state->begin({})};auto data{wire(pending->id)};std::atomic<bool> locked{};
                std::thread holder{[&] { std::lock_guard<std::mutex> lock{state->mutex};locked=true;std::this_thread::sleep_for(std::chrono::milliseconds(150)); }};
                while(!locked) { std::this_thread::yield(); }auto result{state->answer(response_cap,data,ipc::after(50))};holder.join();
                check(result.error==110&&!pending->answered,"waiting for state mutex reset deadline");
            }
            {
                auto state{fixture()};auto pending{state->begin({})};auto data{wire(pending->id)};preparing=false;ipc::call_result result;
                std::thread answering{[&] { allocation_size=5+text.size();result=state->answer(response_cap,data,ipc::after(1000)); }};
                while(!preparing) { std::this_thread::yield(); }state->deactivate();answering.join();
                check(result.error==9&&pending->cancelled&&!pending->answered,"close during preparation admitted answer");
                check(state->pending.empty()&&state->events->poll().error==125,"close retained request or queue");
            }
            {
                auto state{fixture()};std::atomic<unsigned> delivered{};
                auto pending{state->begin({},[&](std::vector<std::uint8_t> const&,ipc::deadline until)->std::uint32_t
                { std::this_thread::sleep_for(std::chrono::milliseconds(150));if(std::chrono::steady_clock::now()>=until) { return 110; }++delivered;return 0; })};
                auto data{wire(pending->id)};check(state->answer(response_cap,data,ipc::after(50)).error==110&&delivered==0,"native preparation ignored effective call deadline");
                check(!pending->answered&&state->pending.size()==1,"native preparation failure consumed live request");
                check(!state->answer(response_cap,data,ipc::after(500)).error&&delivered==1,"native preparation retry");
                check(state->pending.empty(),"delivered native request not retired");
            }
            {
                auto state{fixture()};auto pending{state->begin({})};auto data{wire(pending->id)};check(!state->answer(response_cap,data,ipc::after(50)).error,"short committed answer");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));std::vector<std::uint8_t> output{7};
                check(!state->wait(pending,output)&&output==std::vector<std::uint8_t>{7},"expired answer wait delivered or changed output");
            }
            {
                auto state{fixture()};auto pending{state->begin({})};auto data{wire(pending->id)};auto foreign{response_cap};foreign.resource+="/foreign";
                check(state->answer(foreign,data,ipc::after(500)).error==13&&!pending->answered,"foreign request response");
            }
        }
        gw::native_io::fixed_text<128> message;check(message.append("PASS shared Service response deadline state checks=")&&message.number(checks)&&message.append("\n"),"unit output formatting");
        auto line{message.view()};check(::write(STDOUT_FILENO,line.data(),line.size())==static_cast<ssize_t>(line.size()),"unit output write");return 0;
    }
    catch(std::exception const& error) { service_state_test::disarm();std::fprintf(stderr,"FAIL %s\n",error.what());return 1; }
}
