#pragma once
#include "common.h"
#include "../ros_ipc/probe_common.h"
#include <atomic>
#include <thread>

namespace ros_perf
{
    namespace core=ros_probe::core;
    namespace ipc=ros_probe::ipc;
    struct roundtrips
    {
        std::mutex mutex;
        std::vector<std::uint64_t> values;
        std::uint64_t began{now()};
        rusage before{};
        roundtrips(){getrusage(RUSAGE_SELF,&before);}
        void add(std::uint64_t value){std::lock_guard<std::mutex> lock{mutex};values.push_back(value);}
        void report(char const* workload,unsigned concurrency,unsigned expected)
        {
            ros_probe::require(values.size()==expected,"exact bounded roundtrip count");
            std::sort(values.begin(),values.end());rusage after{};getrusage(RUSAGE_SELF,&after);
            auto cpu=[](timeval v){return double(v.tv_sec)+double(v.tv_usec)/1e6;};auto elapsed=double(now()-began)/1e9;
            std::cout<<"{\"status\":\"PASS\",\"workload\":\""<<workload<<"\",\"concurrency\":"<<concurrency<<",\"completed\":"<<values.size()<<",\"errors\":0,\"request_bytes\":0,\"max_response_bytes\":1024,\"queue_depth_per_session\":1,\"latency_us_p50\":"<<double(values[expected/2])/1000<<",\"latency_us_p95\":"<<double(values[std::min(expected-1,unsigned(expected*.95))])/1000<<",\"latency_us_max\":"<<double(values.back())/1000<<",\"throughput_hz\":"<<expected/elapsed<<",\"elapsed_seconds\":"<<elapsed<<",\"cpu_seconds\":"<<cpu(after.ru_utime)+cpu(after.ru_stime)-cpu(before.ru_utime)-cpu(before.ru_stime)<<",\"max_rss_kib\":"<<after.ru_maxrss<<",\"voluntary_context_switches\":"<<after.ru_nvcsw-before.ru_nvcsw<<",\"involuntary_context_switches\":"<<after.ru_nivcsw-before.ru_nivcsw<<"}\n"<<std::flush;
        }
    };
    inline void measure(core::version version,std::string const& gateway,std::atomic<unsigned>& native_calls)
    {
        auto time_operation=version==core::version::ros1?"time.read-wall":"time.read-system";
        ros_probe::fixture fixture{version,gateway,[&](ros_probe::gateway::configuration& config)
        {
            config.policy.capabilities.clear();
            config.policy.connection["namespace"]="/";
            if(version==core::version::ros2){config.policy.connection["domain-id"]=::getenv("ROS_DOMAIN_ID")?::getenv("ROS_DOMAIN_ID"):"72";}
            auto service=ros_probe::service(version);service.limits["max-rate-hz"]="1000";service.limits["burst"]="1000";service.limits["max-bytes-per-second"]="1048576";
            config.policy.capabilities.emplace("service",service);
            core::capability_spec time{"clock",time_operation,"/clock","builtin/time-v1","builtin:time-nanoseconds-v1",{{"max-inflight","1"},{"max-response-bytes","8"},{"max-bytes-per-second","1048576"},{"max-rate-hz","10000"},{"burst","2000"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}}};
            config.policy.capabilities.emplace("clock",time);
        }};
        ipc::client client;fixture.connect(client);
        // Warm adapter initialization and discovery are excluded explicitly.
        auto warm=client.call(time_operation,"clock",{},1000);ros_probe::require(!warm.error&&warm.payload.size()==8,"actual native clock warmup");
        roundtrips time;
        for(unsigned i{};i!=1000;++i)
        {
            auto begin=now();auto response=client.call(time_operation,"clock",{},1000);auto elapsed=now()-begin;
            ros_probe::require(!response.error&&response.payload.size()==8&&ipc::get(response.payload.data(),8)>0,"authorized native clock result");time.add(elapsed);
        }
        time.report("authenticated-gateway-clock-ipc-inclusive",1,1000);client.close();
        std::vector<std::unique_ptr<ipc::client>> clients;
        for(unsigned i{};i!=4;++i){auto c=std::make_unique<ipc::client>();fixture.connect(*c);clients.push_back(std::move(c));}
        // Check native discovery before starting the measured concurrent workload.
        auto ready=clients[0]->call("service.call","service",{},1000);ros_probe::require(!ready.error,"actual native service discovery");native_calls=0;
        roundtrips services;std::atomic<bool> failed{};std::vector<std::thread> workers;
        for(auto& c:clients)
        {
            auto pointer=c.get();workers.emplace_back([&,pointer]
            {
                for(unsigned i{};i!=25;++i)
                {
                    auto begin=now();auto response=pointer->call("service.call","service",{},1000);auto elapsed=now()-begin;
                    if(response.error||response.payload!=std::vector<std::uint8_t>{1,2,0,0,0,'o','k'}){failed=true;return;}services.add(elapsed);
                }
            });
        }
        for(auto& worker:workers){worker.join();}
        ros_probe::require(!failed&&native_calls==100,"100 exact actual native callbacks at concurrency4");services.report("authenticated-native-trigger-service",4,100);
    }
}
