/** @file
 * @brief Actual constant-space ticket and shared Service ownership regressions.
 * No ROS node, DDS endpoint, physical robot or Guest permission is created.
 * Root alone compiles/runs this source in its original guarded Linux cgroup.
 */
#include "../../gateways/common/service.h"
#include "../ros_ipc/probe_common.h"
#include <algorithm>
#include <array>
#include <atomic>
#include <csignal>
#include <exception>
#include <limits>
#include <thread>
#include <vector>

namespace
{
    namespace gw=uwvm2::ros_gateway;
    namespace ipc=gw::ipc;
    constexpr std::size_t beyond_old_limit{1048576+32};
    unsigned groups{};
    void need(bool ok,char const* message) { ros_probe::require(ok,message); }

    gw::ros::capability_spec service(gw::ros::version version,std::string resource)
    {
        auto cap{ros_probe::service(version)};
        cap.operation="service.advertise";cap.resource=std::move(resource);
        cap.limits["max-services"]="1";
        cap.limits["max-event-queue-depth"]="8";cap.limits["max-event-queue-bytes"]="8192";
        cap.limits["overflow-policy"]="reject";cap.limits["timeout-ms"]="60000";
        // Traffic checks exercise identity lifetime rather than waiting for a
        // low-rate token bucket. These are finite normal policy maxima.
        cap.limits["max-rate-hz"]="1000000";cap.limits["burst"]="1048576";
        cap.limits["max-bytes-per-second"]="1073741824";
        gw::validate(version,cap);return cap;
    }
    gw::ros::capability_spec responder(gw::ros::capability_spec cap)
    {
        cap.operation="service.respond";cap.alias="response";
        for(auto name:{"max-services","max-event-queue-depth","max-event-queue-bytes","overflow-policy"})
        { cap.limits.erase(name); }
        gw::validate(cap.type.find("/srv/")!=std::string::npos?gw::ros::version::ros2:gw::ros::version::ros1,cap);
        return cap;
    }
    std::vector<std::uint8_t> answer(std::uint64_t id)
    {
        std::vector<std::uint8_t> data;data.reserve(13);
        ipc::put(data,id,8);data.push_back(1);ipc::put(data,0,4);return data;
    }
    std::shared_ptr<gw::service_state> state(gw::ros::capability_spec const& cap)
    { return std::make_shared<gw::service_state>(cap,std::make_shared<gw::event_queue>(cap)); }
    std::shared_ptr<gw::pending_service> begin(std::shared_ptr<gw::service_state> const& selected,unsigned& delivered)
    {
        return selected->begin({},[&delivered](auto const& body,ipc::deadline until)->std::uint32_t {
            if(std::chrono::steady_clock::now()>=until) { return 110; }
            if(body.size()!=5||body[0]!=1||std::any_of(body.begin()+1,body.end(),[](auto byte){return byte!=0;})) { return 22; }
            ++delivered;return 0;
        });
    }
    void consume_request(std::shared_ptr<gw::service_state> const& selected,std::uint64_t id)
    {
        auto event{selected->events->poll()};
        need(!event.error&&event.payload.size()==8&&ipc::get(event.payload.data(),8)==id,
            "actual queued Trigger event must carry its exact live request ID");
    }

    void exhaustive_sequence()
    {
        // Every possible nonzero seed traverses the entire uint8 domain. This
        // proves wrap and true exhaustion, rather than faking a64-bit counter.
        for(unsigned first{1};first<=255;++first)
        {
            gw::id_detail::nonreusing_sequence<std::uint8_t> ids{static_cast<std::uint8_t>(first)};
            std::array<bool,256> observed{};
            for(unsigned count{};count<255;++count)
            {
                auto id{ids.take()};need(id&&!observed[id],"uint8 issued zero or repeated a live historical ID");
                observed[id]=true;
            }
            need(std::all_of(observed.begin()+1,observed.end(),[](bool seen){return seen;}),
                "true exhaustion must follow all255 distinct nonzero values");
            for(unsigned repeat{};repeat<4;++repeat) { need(!ids.take(),"exhausted sequence reseeded or reused an ID"); }
        }
        gw::id_detail::nonreusing_sequence<std::uint64_t> boundary{UINT64_MAX};
        need(boundary.take()==UINT64_MAX&&boundary.take()==1&&boundary.take()==2,
            "maximum seed must not cause premature64-bit exhaustion or emit zero");
        gw::id_detail::nonreusing_sequence<std::uint8_t> invalid{0};
        need(!invalid.take(),"zero cannot initialize a live sequence");++groups;
    }
    void allocator_lifetime()
    {
        gw::nonreusing_id_allocator ids;unsigned entropy_calls{};
        auto failed=[&]()->std::uint64_t { ++entropy_calls;return 0; };
        auto seed=[&]()->std::uint64_t { ++entropy_calls;return UINT64_MAX-4; };
        need(!ids.allocate(failed),"failed entropy must fail closed without consuming an identifier");
        std::vector<std::uint64_t> observed;observed.reserve(beyond_old_limit);
        for(std::size_t i{};i<beyond_old_limit;++i) { observed.push_back(ids.allocate(seed)); }
        need(entropy_calls==2,"successful allocator must seed once, including across wrap");
        std::sort(observed.begin(),observed.end());
        need(observed.front()&&std::adjacent_find(observed.begin(),observed.end())==observed.end(),
            "more than old lifetime cap must remain nonzero and unique");
        need(observed.size()==beyond_old_limit,"whole lifetime traffic inventory retained");++groups;
    }
    void parallel_allocations()
    {
        gw::nonreusing_id_allocator ids;std::atomic<unsigned> entropy_calls{};std::atomic<bool> go{};
        constexpr unsigned threads{8},per_thread{16384};
        std::vector<std::uint64_t> observed(threads*per_thread);std::vector<std::thread> actors;
        std::array<std::exception_ptr,threads> errors{};
        try
        {
            for(unsigned t{};t<threads;++t) { actors.emplace_back([&,t] {
                while(!go.load(std::memory_order_acquire)) { std::this_thread::yield(); }
                try
                {
                    for(unsigned i{};i<per_thread;++i)
                    { observed[t*per_thread+i]=ids.allocate([&]()->std::uint64_t { ++entropy_calls;return UINT64_MAX-1024; }); }
                }
                catch(...) { errors[t]=std::current_exception(); }
            }); }
        }
        catch(...) { go.store(true,std::memory_order_release);for(auto& actor:actors) {actor.join();}throw; }
        go.store(true,std::memory_order_release);for(auto& actor:actors) {actor.join();}
        for(auto const& error:errors) { if(error) { std::rethrow_exception(error); } }
        std::sort(observed.begin(),observed.end());
        need(entropy_calls==1&&observed.front()&&std::adjacent_find(observed.begin(),observed.end())==observed.end(),
            "shared allocator must serialize initialization/wrap and all parallel IDs");++groups;
    }
    void completed_service_traffic()
    {
        auto a{state(service(gw::ros::version::ros1,"/ticket_lifetime/a"))};
        auto b{state(service(gw::ros::version::ros2,"/ticket_lifetime/b"))};
        auto ac{responder(a->capability)},bc{responder(b->capability)};
        unsigned delivered{};std::uint64_t previous{};
        for(std::size_t i{};i<beyond_old_limit;++i)
        {
            auto const& selected{i%2?b:a};auto const& cap{i%2?bc:ac};
            auto request{begin(selected,delivered)};
            need(request&&request->id!=previous,"completed requests must not exhaust the process at old cap");
            previous=request->id;consume_request(selected,request->id);
            need(!selected->answer(cap,answer(request->id),ipc::after(1000)).error,
                "sequential real Service response must remain deliverable beyond old cap");
            need(request->answered&&selected->pending.empty(),"successful native delivery must retire only its live pending state");
        }
        need(delivered==beyond_old_limit&&a->pending.empty()&&b->pending.empty(),
            "all completed traffic must produce exactly one response with zero final inflight");
        a->deactivate();b->deactivate();++groups;
    }
    void queue_rejected_service_traffic()
    {
        auto rejected{state(service(gw::ros::version::ros2,"/ticket_lifetime/rejected"))};
        // Keep the genuine bounded queue full of valid incoming Trigger frames.
        // service_state stays active with no pending request, so each begin
        // reserves an ID and then reaches the real overflow-reject path.
        for(unsigned i{};i<8;++i)
        {
            std::vector<std::uint8_t> queued;ipc::put(queued,1000+i,8);
            need(rejected->events->enqueue(std::move(queued)),"fill bounded request event queue");
        }
        unsigned delivered{};
        for(std::size_t i{};i<beyond_old_limit;++i)
        { need(!begin(rejected,delivered)&&rejected->pending.empty(),"queue rejection retained a pending request"); }
        auto statistics{rejected->events->statistics()};
        need(statistics.admission_refused==beyond_old_limit&&statistics.depth==8,
            "each rejected attempt must reach the actual full-queue branch without exhausting IDs");
        auto healthy{state(service(gw::ros::version::ros2,"/ticket_lifetime/healthy"))};
        auto request{begin(healthy,delivered)};need(bool(request),"rejected native traffic exhausted unrelated Service IDs");
        consume_request(healthy,request->id);
        need(!healthy->answer(responder(healthy->capability),answer(request->id),ipc::after(1000)).error&&delivered==1,
            "healthy service must still deliver exactly once after more than old rejected-attempt cap");
        healthy->deactivate();rejected->deactivate();++groups;
    }
    void ownership_and_retirement()
    {
        auto cap{service(gw::ros::version::ros2,"/ticket_lifetime/owned")};
        auto owner{state(cap)},another_session{state(cap)};unsigned delivered{};
        auto old{begin(owner,delivered)},other{begin(another_session,delivered)};
        need(old&&other&&old->id!=other->id,"same resource in two owners must receive process-unique IDs");
        consume_request(owner,old->id);consume_request(another_session,other->id);
        auto wrong_resource{responder(cap)};wrong_resource.resource="/ticket_lifetime/foreign";
        need(owner->answer(wrong_resource,answer(old->id),ipc::after(1000)).error==13,
            "an ID must not grant a foreign resource/type operation");
        need(another_session->answer(responder(cap),answer(old->id),ipc::after(1000)).error==9&&!other->answered,
            "a known ID from another session must not authorize its pending map");
        need(!owner->answer(responder(cap),answer(old->id),ipc::after(1000)).error&&delivered==1,
            "matching owned response admitted once");
        auto current{begin(owner,delivered)};need(current&&current->id!=old->id,"completed ID must not be reused");
        consume_request(owner,current->id);
        need(owner->answer(responder(cap),answer(old->id),ipc::after(1000)).error==9&&!current->answered,
            "stale completed ticket must not answer a newer request");
        // Expire the genuine stored deadline directly; no scheduler/sleep race
        // determines whether the next begin retires the old pending record.
        { std::lock_guard<std::mutex> lock{owner->mutex};current->deadline=std::chrono::steady_clock::now()-std::chrono::milliseconds(1); }
        auto after_expiry{begin(owner,delivered)};
        need(after_expiry&&after_expiry->id!=current->id&&current->cancelled,
            "expired request must retire without exhausting or resetting identity order");
        consume_request(owner,after_expiry->id);
        need(owner->answer(responder(cap),answer(current->id),ipc::after(1000)).error==9&&!after_expiry->answered,
            "expired ticket must not address a later request");
        owner->deactivate();need(after_expiry->cancelled&&owner->pending.empty(),"close must cancel and drain old pending state");
        auto reopened{state(cap)};auto fresh{begin(reopened,delivered)};
        need(fresh&&fresh->id!=after_expiry->id,"new endpoint must not reset process identity order");
        consume_request(reopened,fresh->id);
        need(reopened->answer(responder(cap),answer(after_expiry->id),ipc::after(1000)).error==9&&!fresh->answered,
            "closed endpoint ticket must remain stale after endpoint recreation");
        need(!reopened->answer(responder(cap),answer(fresh->id),ipc::after(1000)).error&&delivered==2,
            "only the two authorized owned responses may reach native callbacks");
        another_session->deactivate();reopened->deactivate();++groups;
    }
    void parallel_service_owners()
    {
        // Each actor has a different live owner but deliberately uses the same
        // ROS resource. Only the real process-global Service ID domain is shared.
        constexpr unsigned threads{8},per_thread{1024};std::atomic<bool> go{};
        std::array<unsigned,threads> delivered{};std::array<std::exception_ptr,threads> errors{};
        std::vector<std::uint64_t> observed(threads*per_thread);std::vector<std::thread> actors;
        auto cap{service(gw::ros::version::ros2,"/ticket_lifetime/parallel")};
        std::array<std::shared_ptr<gw::service_state>,threads> owners;
        for(auto& owner:owners) { owner=state(cap); }
        try
        {
            for(unsigned t{};t<threads;++t) { actors.emplace_back([&,t] {
                while(!go.load(std::memory_order_acquire)) { std::this_thread::yield(); }
                try
                {
                    auto const& owner{owners[t]};auto response_cap{responder(cap)};
                    for(unsigned i{};i<per_thread;++i)
                    {
                        auto request{begin(owner,delivered[t])};need(bool(request),"parallel owned Service begin failed");
                        observed[t*per_thread+i]=request->id;consume_request(owner,request->id);
                        need(!owner->answer(response_cap,answer(request->id),ipc::after(10000)).error,
                            "parallel owned Service response failed");
                    }
                }
                catch(...) { errors[t]=std::current_exception(); }
            }); }
        }
        catch(...) { go.store(true,std::memory_order_release);for(auto& actor:actors) {actor.join();}throw; }
        go.store(true,std::memory_order_release);for(auto& actor:actors) {actor.join();}
        for(auto const& error:errors) { if(error) { std::rethrow_exception(error); } }
        std::sort(observed.begin(),observed.end());
        need(observed.front()&&std::adjacent_find(observed.begin(),observed.end())==observed.end(),
            "parallel independent Service owners shared or reused a ticket");
        for(unsigned t{};t<threads;++t)
        {
            need(delivered[t]==per_thread&&owners[t]->pending.empty(),"parallel owner did not retire all requests");
            owners[t]->deactivate();
        }
        ++groups;
    }
}
int main(int argc,char** argv)
{
    try
    {
        if(argc==2&&std::string_view{argv[1]}=="--owned-stop") { ::raise(SIGSTOP); }
        else { need(argc==1,"only explicit --owned-stop fixture option is supported"); }
        exhaustive_sequence();allocator_lifetime();parallel_allocations();
        completed_service_traffic();queue_rejected_service_traffic();ownership_and_retirement();parallel_service_owners();
        need(groups==7,"all seven regression groups must run");
        ros_probe::print_fixture(STDOUT_FILENO,
            "PASS ticket-lifetime groups=7 completed=",beyond_old_limit,
            " queue_rejected=",beyond_old_limit," exact_uint8_nonzero_domain=255 parallel_ids=131072 parallel_Service_ids=8192 no_ROS_or_Guest_execution\n");
        return 0;
    }
    catch(std::exception const& error)
    { try {ros_probe::print_fixture(STDERR_FILENO,"FAIL ticket-lifetime ",error.what(),"\n");}catch(...) {}return 1; }
}
