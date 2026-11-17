/** @file
 * @brief Equal logical clock and guard/wait workloads for scene/rcl observation.
 * @details Build twice with the same compiler; define UWVM_SCENE_BENCH_NATIVE
 * for installed rcl. This benchmark does not execute Wasm. Native rcl has no
 * scene capability checks; its guard wait consumes the notification, whereas
 * scene wait observes and a separate take consumes it. Both modes complete one
 * clock sample and/or one trigger-observe-consume cycle per logical iteration.
 * Batch-mean percentiles are throughput variation, not request-tail latency.
 * Profiling and baseline runs must use the existing 64 GiB cgroup. Never use
 * this narrow workload to claim complete ROS/native/robot compatibility.
 */
#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>
#ifdef UWVM_SCENE_BENCH_NATIVE
#include <rcl/rcl.h>
#include <rcl/time.h>
#else
#include <uwvm2/imported/ros/scene/broker.h>
#endif

namespace benchmark
{
    using timer=std::chrono::steady_clock;
    [[noreturn]] void fail(char const* operation){throw std::runtime_error(operation);}
#ifdef UWVM_SCENE_BENCH_NATIVE
    inline constexpr char provider[]="native-rcl";
    void check(rcl_ret_t result,char const* operation){if(result!=RCL_RET_OK)fail(operation);}
    struct environment
    {
        rcl_allocator_t allocator{rcl_get_default_allocator()};
        rcl_init_options_t options{rcl_get_zero_initialized_init_options()};
        rcl_context_t context{rcl_get_zero_initialized_context()};
        rcl_guard_condition_t guard{rcl_get_zero_initialized_guard_condition()};
        rcl_wait_set_t wait{rcl_get_zero_initialized_wait_set()};
        rcl_clock_t clock{};
        environment()
        {
            check(rcl_init_options_init(&options,allocator),"rcl_init_options_init");
            check(rcl_init_options_set_domain_id(&options,217),"rcl_init_options_set_domain_id");
            check(rcl_init(0,nullptr,&options,&context),"rcl_init");
            check(rcl_guard_condition_init(&guard,&context,rcl_guard_condition_get_default_options()),"rcl_guard_condition_init");
            check(rcl_wait_set_init(&wait,0,1,0,0,0,0,&context,allocator),"rcl_wait_set_init");
            check(rcl_clock_init(RCL_STEADY_TIME,&clock,&allocator),"rcl_clock_init");
        }
        [[gnu::noinline]] std::uint64_t clock_step()
        {
            rcl_time_point_value_t value{};check(rcl_clock_get_now(&clock,&value),"rcl_clock_get_now");
            return static_cast<std::uint64_t>(value);
        }
        [[gnu::noinline]] std::uint64_t guard_step()
        {
            check(rcl_wait_set_clear(&wait),"rcl_wait_set_clear");
            std::size_t slot{};check(rcl_wait_set_add_guard_condition(&wait,&guard,&slot),"rcl_wait_set_add_guard_condition");
            check(rcl_trigger_guard_condition(&guard),"rcl_trigger_guard_condition");
            check(rcl_wait(&wait,0),"rcl_wait");
            if(slot>=wait.size_of_guard_conditions||wait.guard_conditions[slot]!=&guard)fail("rcl guard was not observed");
            return 1;
        }
        void finish()
        {
            check(rcl_clock_fini(&clock),"rcl_clock_fini");check(rcl_wait_set_fini(&wait),"rcl_wait_set_fini");
            check(rcl_guard_condition_fini(&guard),"rcl_guard_condition_fini");
            check(rcl_shutdown(&context),"rcl_shutdown");check(rcl_context_fini(&context),"rcl_context_fini");
            check(rcl_init_options_fini(&options),"rcl_init_options_fini");
        }
    };
#else
    namespace scene=uwvm2::imported::ros::scene;
    namespace resources=uwvm2::imported::ros::resources;
    namespace contract=uwvm2::imported::ros::runtime_contract;
    using operation=contract::operation;
    inline constexpr char provider[]="scene-cpp-broker";
    struct environment
    {
        int identity{};scene::caller caller{&identity,1};scene::broker broker;
        scene::handle clock_cap{},guard_cap{},wait_cap{},clock{},guard{},wait{};
        std::array<std::uint8_t,32> output{};
        std::array<std::uint8_t,8> clock_request{},guard_request{};
        std::array<std::uint8_t,16> wait_request{};
        scene::sink sink;
        std::size_t required{};
        void check(scene::error result,char const* operation){if(result!=scene::error::success)fail(operation);}
        std::uint64_t invoke(operation action,scene::handle cap,scene::bytes bytes)
        {
            check(broker.invoke(caller,action,cap,bytes,output.size(),required,sink),"scene invoke");
            return required>=8?scene::word(output,0):0;
        }
        environment():sink{[this](scene::bytes data)
        {
            if(data.size()>output.size())return false;
            std::copy(data.begin(),data.end(),output.begin());return true;
        }}
        {
            for(auto const& item:contract::operations)check(broker.permissions.expose(std::string(item.name),scene::exposure::allow),"scene expose");
            check(broker.permissions.expose("capability.open",scene::exposure::allow),"scene capability expose");
            auto add=[&](std::string alias,scene::purpose purpose,std::initializer_list<std::string> actions)
            {
                scene::grant value;value.alias=alias;value.resource=alias;value.binding="local";value.kind=purpose;value.actions=actions;
                value.max_handles=1;value.max_resources=1;value.max_wait_members=1;value.max_wait_ns=0;
                check(broker.permissions.configure(std::move(value)),"scene configure");
            };
            add("clock",scene::purpose::steady_clock,{"clock.create","clock.read"});
            add("guard",scene::purpose::guard,{"guard.create","guard.trigger","guard.take","resource.observe"});
            add("wait",scene::purpose::wait,{"wait.create","wait.add","wait.poll"});
            check(broker.permissions.enroll(caller),"scene enroll");
            check(broker.permissions.open(caller,"clock",clock_cap),"scene clock open");
            check(broker.permissions.open(caller,"guard",guard_cap),"scene guard open");
            check(broker.permissions.open(caller,"wait",wait_cap),"scene wait open");
            clock=invoke(operation::clock_create,clock_cap,clock_request);
            guard=invoke(operation::guard_create,guard_cap,{});wait=invoke(operation::wait_set_create,wait_cap,{});
            std::array<std::uint8_t,40> attach{};
            scene::put(attach,0,wait);scene::put(attach,1,guard_cap);scene::put(attach,2,guard);
            scene::put(attach,3,resources::readable);scene::put(attach,4,1);
            invoke(operation::wait_set_add,wait_cap,attach);
            scene::put(clock_request,0,clock);scene::put(guard_request,0,guard);scene::put(wait_request,0,wait);scene::put(wait_request,1,0);
        }
        [[gnu::noinline]] std::uint64_t clock_step()
        {
            auto value=invoke(operation::clock_read,clock_cap,clock_request);
            if(required!=32||scene::word(output,2)!=1)fail("scene clock was unstarted");
            return value;
        }
        [[gnu::noinline]] std::uint64_t guard_step()
        {
            invoke(operation::guard_trigger,guard_cap,guard_request);
            if(invoke(operation::wait_set_wait,wait_cap,wait_request)!=guard||required!=32||scene::word(output,2)!=resources::readable)
                fail("scene guard was not observed");
            auto count=invoke(operation::guard_take,guard_cap,guard_request);
            if(count!=1)fail("scene guard consume count differs");
            return count;
        }
        void finish(){broker.retire();}
    };
#endif
    std::uint64_t integer(char const* text,std::uint64_t maximum)
    {
        std::uint64_t value{};auto end=text+std::strlen(text);auto result=std::from_chars(text,end,value);
        if(result.ec!=std::errc{}||result.ptr!=end||!value||value>maximum)fail("invalid positive benchmark bound");
        return value;
    }
}

int main(int argc,char** argv)
{
    try
    {
        using namespace benchmark;
        if(argc!=4)fail("usage: clock|guard|mixed iterations-per-batch batches");
        std::string_view workload=argv[1];if(workload!="clock"&&workload!="guard"&&workload!="mixed")fail("unknown workload");
        auto iterations=integer(argv[2],100000000),batches=integer(argv[3],10000);
        environment target;std::uint64_t checksum{};
        auto step=[&]{if(workload!="guard")checksum^=target.clock_step();if(workload!="clock")checksum+=target.guard_step();};
        for(unsigned i{};i!=1000;++i)step();
        std::vector<double> samples;samples.reserve(batches);
        auto started=timer::now();
        for(std::uint64_t batch{};batch!=batches;++batch)
        {
            auto begin=timer::now();for(std::uint64_t i{};i!=iterations;++i)step();auto end=timer::now();
            samples.push_back(std::chrono::duration<double,std::nano>(end-begin).count()/static_cast<double>(iterations));
        }
        auto elapsed=std::chrono::duration<double>(timer::now()-started).count();target.finish();
        std::sort(samples.begin(),samples.end());auto quantile=[&](unsigned percentage){return samples[(samples.size()-1)*percentage/100];};
        std::cout<<"{\"provider\":\""<<provider<<"\",\"workload\":\""<<workload<<"\",\"iterations_per_batch\":"<<iterations
            <<",\"batches\":"<<batches<<",\"logical_operations\":"<<iterations*batches<<",\"elapsed_seconds\":"<<elapsed
            <<",\"operations_per_second\":"<<static_cast<double>(iterations*batches)/elapsed
            <<",\"batch_mean_ns_p50\":"<<quantile(50)<<",\"batch_mean_ns_p95\":"<<quantile(95)<<",\"batch_mean_ns_p99\":"<<quantile(99)
            <<",\"checksum\":"<<checksum<<",\"wasm_executed\":false}\n";
    }
    catch(std::exception const& error){std::cerr<<error.what()<<'\n';return 1;}
}
