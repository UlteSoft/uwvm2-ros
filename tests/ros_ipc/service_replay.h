#pragma once
#include "probe_common.h"
#include <future>
namespace ros_probe
{
    template<typename Begin>
    void service_replay(core::version version,std::string const& executable,Begin begin)
    {
        auto resource{"/uwvm_test/service_replay_"+std::to_string(::getpid())};
        auto configure=[&](gateway::configuration& cfg)
        {
            cfg.policy.connection["node-name"]="uwvm_service_replay_"+std::to_string(::getpid());
            cfg.policy.capabilities.clear();
            for(auto const& name:{"server","responder"})
            {
                auto cap{service(version)};cap.alias=name;cap.operation=std::string{name}=="server"?"service.advertise":"service.respond";cap.resource=resource;cap.limits["timeout-ms"]="3000";cap.limits["burst"]="4";
                if(version==core::version::ros2) { cap.limits["qos.depth"]="7"; }
                if(cap.operation=="service.advertise"){cap.limits["max-services"]="1";cap.limits["max-event-queue-depth"]="4";cap.limits["max-event-queue-bytes"]="4096";cap.limits["overflow-policy"]="reject";}
                cfg.policy.capabilities.emplace(cap.alias,std::move(cap));
            }
        };
        auto poll=[&](ipc::client& client)
        {
            for(unsigned n{};n!=40;++n)
            {
                auto event{client.call("event.poll","server",{},1000)};
                if(!event.error){require(event.payload.size()==8&&ipc::get(event.payload.data(),8),"actual native Trigger request opaque ID");return ipc::get(event.payload.data(),8);}
                require(event.error==11,"native Trigger request bounded pending");std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            throw std::runtime_error("FAIL actual native Trigger request deadline");
        };
        auto response=[](std::uint64_t id){std::vector<std::uint8_t> bytes;ipc::put(bytes,id,8);bytes.push_back(1);ipc::put(bytes,5,4);bytes.insert(bytes.end(),{'h','e','l','l','o'});return bytes;};
        fixture f{version,executable,configure};ipc::client client;f.connect(client);
        require(!client.call("service.advertise","server",{},1000).error,"real queued service advertised");
        auto first{begin(resource)};auto old{poll(client)};
        require(!client.call("capability.close","server",{},1000).error,"advertisement closes with responder alias live");
        require(first.wait_for(std::chrono::seconds(5))==std::future_status::ready&&!first.get(),"old actual native pending call cancelled on close");
        require(!client.call("service.advertise","server",{},1000).error,"real queued service re-advertised");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        auto second{begin(resource)};auto fresh{poll(client)};require(fresh!=old,"request ID never aliases after close/reopen");
        require(client.call("service.respond","responder",response(old),1000).error==9,"old request ID cannot respond to new actual request");
        require(second.wait_for(std::chrono::milliseconds(50))==std::future_status::timeout,"old response emitted no actual native reply");
        require(!client.call("service.respond","responder",response(fresh),1000).error,"fresh exact native response");
        require(second.wait_for(std::chrono::seconds(5))==std::future_status::ready&&second.get(),"native second call gets exact true/hello");
        client.close();::kill(f.gateway_pid,SIGKILL);::waitpid(f.gateway_pid,nullptr,0);f.gateway_pid=-1;
        fixture restarted{version,executable,configure};ipc::client rebound;restarted.connect(rebound);
        require(!rebound.call("service.advertise","server",{},1000).error,"new Gateway real queued service advertised");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto third{begin(resource)};auto newest{poll(rebound)};require(newest!=old&&newest!=fresh,"request ID is fresh across Gateway process restart");
        require(rebound.call("service.respond","responder",response(fresh),1000).error==9,"prior Gateway request ID fails closed");
        require(third.wait_for(std::chrono::milliseconds(50))==std::future_status::timeout,"prior Gateway response emitted no native reply");
        require(!rebound.call("service.respond","responder",response(newest),1000).error,"new Gateway exact native response");
        require(third.wait_for(std::chrono::seconds(5))==std::future_status::ready&&third.get(),"native third call gets exact true/hello");rebound.close();
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS",static_cast<unsigned>(version)," ServiceServer stale request-ID denial after close/reopen and SIGKILL/restart; native replies unchanged\n");
    }
}
