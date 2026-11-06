
#include <uwvm2/imported/ros/capability/manager.h>
#include <uwvm2/utils/control/host_diagnostics.h>
#include <cassert>
#include <iostream>
namespace c=uwvm2::imported::ros;
namespace d=uwvm2::utils::control::host_diagnostics;
namespace a=uwvm2::utils::control::host_interfaces;
static std::string output;static unsigned messages{};
static void receive(std::string_view text) noexcept {try{output=text;++messages;}catch(...){}}
static c::target_policy policy(c::version v)
{
    c::target_policy p{v,c::scope::single,"main",true,
        {{"gateway-socket","/unused-unit-contract.sock"},{"gateway-uid","0"},{"node-name","quota_unit"},
         {"namespace","/"},{"connect-timeout-ms","1000"}},{}};
    if(v==c::version::ros1)p.connection.insert({{"master-uri","http://127.0.0.1:11311"},{"advertise-host","127.0.0.1"},{"transport","tcpros"}});
    else p.connection.insert({{"domain-id","73"},{"rmw-implementation","rmw_fastrtps_cpp"},{"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}});
    c::capability_spec cap{"clock",v==c::version::ros1?"time.read-wall":"time.read-steady","/clock","builtin/time-v1","builtin:time-nanoseconds-v1",{{"audit","deny-only"}}};
    p.capabilities.emplace(cap.alias,cap);assert(c::validate_target(p));return p;
}
int main()
{
    d::sink=receive;
    for(auto v:{c::version::ros1,c::version::ros2})
    {
        auto const family=v==c::version::ros1?a::family::ros1:a::family::ros2;
        auto const key=v==c::version::ros1?"uwvm_ros1_v1.capability_open":"uwvm_ros2_v1.capability_open";
        auto const id=a::find(family,std::string_view{key});
        auto initial=policy(v);
        for(auto mode:{0,1,2})
        {
            c::capability_manager manager;int identity{};
            auto selected=initial;
            if(mode)
            {
                auto& limits=selected.capabilities.at("clock").limits;
                limits["max-rate-hz"]="1";limits["burst"]="1";
                limits["max-rate-hz.on-exhausted"]=mode==2?"wait":"reject";
            }
            assert(c::validate_target(selected));
            assert(manager.register_instance(&identity,1,"main",v==c::version::ros1?&selected:nullptr,v==c::version::ros2?&selected:nullptr));
            std::uint64_t first{},second{};
            assert(manager.open(&identity,1,v,"clock",first));assert(first);
            auto before=std::chrono::steady_clock::now();auto count=messages;
            {
                d::call_scope call{id,"single","main"};
                auto result=manager.open(&identity,1,v,"clock",second);
                assert(result.code==(mode==1?c::errc::rate_limited:c::errc::success));
                if(mode==1)assert(second==0);
                if(mode==2)assert(std::chrono::steady_clock::now()-before>=std::chrono::milliseconds(900));
            }
            // Defaults keep all quota events silent even when calls reject/wait.
            assert(messages==count);
            manager.clear();
        }
        d::configure(std::string_view{"all_host_limited"},d::severity::warning);
        c::capability_manager manager;int identity{};auto selected=initial;
        selected.capabilities.at("clock").limits["max-inflight"]="0";
        assert(manager.register_instance(&identity,1,"main",v==c::version::ros1?&selected:nullptr,v==c::version::ros2?&selected:nullptr));
        std::uint64_t denied=999;auto count=messages;
        {d::call_scope call{id,"single","main"};auto result=manager.open(&identity,1,v,"clock",denied);assert(result.code==c::errc::rate_limited);}
        assert(denied==0 && messages==count+1);
        assert(output.find("quota rejected: max-inflight")!=output.npos);
        assert(output.find("clock")!=output.npos);
        d::configure(std::string_view{"all_host_limited"},d::severity::off);
    }
    std::cout<<"ROS1/ROS2 actual manager unlimited, reject, monotonic wait and warning checks passed\n";
}
