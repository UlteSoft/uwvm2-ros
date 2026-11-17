/** @file
 * @brief Executable volatile-output ownership, retry and capacity contracts.
 * @details Uses the real registry, broker and deployment. Allocation faults
 * happen before effects; failed delivery is simulated after an actual mutation.
 * No filesystem output provider or SDK implementation is substituted.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <new>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>

namespace scene=uwvm2::imported::ros::scene;
namespace outputs=scene::outputs;
using op=uwvm2::imported::ros::output_contract::operation;
using error=scene::error;
using receipt=outputs::receipt;
static std::atomic<int> allocation_fail_after{-1};
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define UWVM_OUTPUT_THREAD_SANITIZED
#endif
#endif
#if defined(UWVM_OUTPUT_THREAD_SANITIZED)
// Native ASan/UBSan, GCC and cross-ABI builds inject actual fast_io malloc
// failures. Keep concurrent TSan qualification separate from those faults.
inline constexpr bool allocation_injection_enabled=false;
#else
inline constexpr bool allocation_injection_enabled=true;
#endif
static bool reject_allocation() noexcept
{
    auto count=allocation_fail_after.load();
    if(count>=0&&allocation_fail_after.fetch_sub(1)==0){errno=ENOMEM;return true;}
    return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t size){return reject_allocation()?nullptr:__real_malloc(size);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t size){return reject_allocation()?nullptr:__real_calloc(count,size);}
extern "C" void* __wrap_realloc(void* pointer,std::size_t size){return reject_allocation()?nullptr:__real_realloc(pointer,size);}

static uwvm2::utils::container::std_vector<std::uint8_t> request(std::initializer_list<std::uint64_t> values,std::string_view data={})
{
    uwvm2::utils::container::std_vector<std::uint8_t> bytes(values.size()*8+data.size());std::size_t index{};
    for(auto value:values)scene::put(bytes,index++,value);
    std::copy(data.begin(),data.end(),bytes.begin()+static_cast<std::ptrdiff_t>(values.size()*8));return bytes;
}
static receipt mutate(outputs::registry& registry,op action,uwvm2::utils::container::std_vector<std::uint8_t> const& bytes,std::string_view alias="cache",std::uint64_t capacity=UINT64_MAX)
{receipt result;assert(registry.mutate(alias,action,bytes,capacity,result)==error::success);return result;}
static scene::grant grant(uwvm2::utils::container::std_string alias)
{
    scene::grant value;value.alias=std::move(alias);value.resource="scratch";value.binding="volatile-output";value.kind=scene::purpose::cache;
    value.actions={"artifact.begin","artifact.append","artifact.commit","artifact.abort","artifact.remove","artifact.status","artifact.receipt","artifact.read"};return value;
}
static void install(scene::broker& broker,scene::caller who,scene::grant value)
{
    assert(broker.permissions.expose("capability.open",scene::exposure::allow)==error::success);
    assert(broker.permissions.expose("capability.close",scene::exposure::allow)==error::success);
    for(auto const& action:value.actions)assert(broker.permissions.expose(action,scene::exposure::allow)==error::success);
    assert(broker.permissions.configure(value)==error::success);
    assert(broker.output_store.bind(value.alias,value.resource,{64,2,32,8})==error::success);
    assert(broker.permissions.enroll(who)==error::success);
}
static receipt decode(scene::bytes input)
{
    assert(input.size()==72);return {scene::word(input,0),scene::word(input,1),scene::word(input,2),scene::word(input,3),
        scene::word(input,4),scene::word(input,5),scene::word(input,6),scene::word(input,7),scene::word(input,8)};
}
static void lifecycle()
{
    outputs::registry registry;assert(registry.bind("cache","scratch",{64,2,32,8})==error::success);
    receipt absent{55};assert(registry.receipt_for("cache",1,absent)==error::conflict&&absent.key==55);
    auto begin=request({1,16});auto first=mutate(registry,op::begin,begin);auto id=first.object;
    assert(id&&first.key==1&&first.operation==1&&!first.effect_status&&first.state==1&&first.retained_bytes==16);
    allocation_fail_after=0;receipt replay;assert(registry.mutate("cache",op::begin,begin,0,replay)==error::success);allocation_fail_after=-1;
    assert(replay==first); // A lost result needs no allocation and creates no second object.
    std::array<std::uint8_t,8> data;data.fill(0x55);std::size_t count=77;
    assert(registry.read("cache",id,0,data,count)==error::denied&&count==0&&data[0]==0x55);
    assert(registry.read_size("cache",id,0,8,count)==error::denied&&count==0);
    auto append=request({2,id,0},"abc");auto written=mutate(registry,op::append,append);
    assert(written.accepted_bytes==3&&written.total_bytes==3&&written.retained_bytes==16);
    assert(mutate(registry,op::append,append)==written);
    auto changed=append;changed.back()='d';replay={123};assert(registry.mutate("cache",op::append,changed,2,replay)==error::conflict&&replay.key==123);
    auto committed=mutate(registry,op::commit,request({3,id,3}));assert(!committed.effect_status&&committed.state==2&&committed.flags==1&&committed.retained_bytes==16);
    assert(registry.read_size("cache",id,1,8,count)==error::success&&count==2);
    assert(registry.read_size("cache",id,3,8,count)==error::success&&count==0);
    assert(registry.read_size("cache",id,3,9,count)==error::too_large&&count==0);
    assert(registry.read_size("cache",id,4,0,count)==error::too_large&&count==0);
    assert(registry.read_size("unknown",id,0,8,count)==error::denied&&count==0);
    assert(registry.read_size("cache",0,0,8,count)==error::bad_handle&&count==0);
    assert(registry.request_size("cache",32)==error::success&&registry.request_size("cache",33)==error::too_large);
    assert(registry.read("cache",id,0,data,count)==error::success&&count==3&&std::string_view(reinterpret_cast<char*>(data.data()),count)=="abc");
    assert(registry.read("cache",id,3,data,count)==error::success&&count==0);
    assert(registry.read("cache",id,UINT64_MAX,data,count)==error::too_large&&count==0);
    auto denied=mutate(registry,op::append,request({4,id,3},"d"));assert(denied.effect_status==13&&denied.flags==1&&denied.total_bytes==3);
    assert(registry.status("cache",id,replay)==error::success&&replay.key==3&&replay.operation==3&&replay.accepted_bytes==0);
    auto removed=mutate(registry,op::remove,request({5,id}));assert(removed.state==4&&removed.object==id&&removed.total_bytes==3&&!removed.retained_bytes&&!removed.flags);
    assert(mutate(registry,op::remove,request({5,id}))==removed);
    assert(registry.status("cache",id,replay)==error::bad_handle);
    uwvm2::utils::container::std_vector<outputs::snapshot> rows;assert(registry.inspect(rows,1)&&rows.size()==1&&rows[0].receipt_only&&rows[0].retained_receipts==1&&rows[0].id==id);
    auto second=mutate(registry,op::begin,request({6,32}));assert(second.object!=id&&!second.effect_status);
    assert(registry.receipt_for("cache",5,replay)==error::conflict);
    auto aborted=mutate(registry,op::abort,request({7,second.object}));assert(aborted.state==3&&!aborted.retained_bytes);
}
static void ordering_capacity_and_privacy()
{
    outputs::registry registry,other;assert(registry.bind("cache","scratch",{8,1,8,4})==error::success);
    assert(registry.bind("peer","scratch",{8,1,8,4})==error::success);assert(other.bind("cache","scratch",{8,1,8,4})==error::success);
    receipt sentinel{88};
    for(auto key:std::array<std::uint64_t,3>{0,2,UINT64_MAX})assert(registry.mutate("cache",op::begin,request({key,4}),1,sentinel)==error::conflict&&sentinel.key==88);
    assert(registry.mutate("cache",op::begin,request({1}),1,sentinel)==error::invalid&&sentinel.key==88);
    auto denied=mutate(registry,op::begin,request({1,4}),"cache",0);assert(denied.effect_status==11&&!denied.object&&!denied.state);
    auto first=mutate(registry,op::begin,request({2,8}));auto id=first.object;
    assert(mutate(registry,op::begin,request({3,0})).effect_status==11); // Empty objects still need a slot.
    assert(registry.mutate("cache",op::append,request({4,id,0},"large"),1,sentinel)==error::too_large);
    auto wrong=mutate(registry,op::append,request({4,id,1},"abcd"));assert(wrong.effect_status==17&&!wrong.total_bytes);
    assert(mutate(registry,op::append,request({5,id,0},"abcd")).accepted_bytes==4);
    assert(mutate(registry,op::commit,request({6,id,5})).effect_status==17);
    assert(mutate(registry,op::append,request({7,id,4},"abcd")).total_bytes==8);
    assert(mutate(registry,op::append,request({8,id,8},"x")).effect_status==90);
    auto foreign=mutate(registry,op::remove,request({1,id}),"peer");assert(foreign.effect_status==9&&!foreign.object&&!foreign.state&&!foreign.total_bytes);
    foreign=mutate(other,op::abort,request({1,id}));assert(foreign.effect_status==9&&!foreign.object);
    uwvm2::imported::ros::resources::registry scheduling;scene::handle guard{};assert(scheduling.guard_create("cache",1,guard)==error::success);
    assert(guard!=id);foreign=mutate(registry,op::abort,request({9,guard}));assert(foreign.effect_status==9&&!foreign.object);
    auto aborted=mutate(registry,op::abort,request({10,id}));assert(aborted.retained_bytes==0);
    assert(!mutate(registry,op::begin,request({11,8})).effect_status);
    uwvm2::utils::container::std_vector<outputs::snapshot> rows;assert(registry.inspect(rows,3));auto unchanged=rows.size();
    assert(!registry.inspect(rows,0)&&rows.size()==unchanged);
    assert(outputs::receipt_bytes({0,0,0,4})==100);
    assert(!outputs::valid({UINT64_MAX,1,1,1})&&!outputs::valid({1,UINT64_MAX,1,1})&&!outputs::valid({1,1,2,1}));
    assert(!outputs::valid({1,1,1,outputs::maximum_transfer+1}));
    outputs::registry empty;assert(empty.bind("cache","scratch",{0,1,0,0})==error::success);
    auto zero=mutate(empty,op::begin,request({1,0}));assert(zero.object&&!zero.retained_bytes);
    assert(!mutate(empty,op::commit,request({2,zero.object,0})).effect_status);
    std::array<std::uint8_t,1> untouched{0x55};std::size_t count{};
    assert(empty.read("cache",zero.object,0,untouched,count)==error::too_large&&untouched[0]==0x55);
    assert(empty.read("cache",zero.object,0,{},count)==error::success&&count==0);
    assert(mutate(empty,op::begin,request({3,0})).effect_status==11);
    assert(!mutate(empty,op::remove,request({4,zero.object})).effect_status);
    assert(!mutate(empty,op::begin,request({5,0})).effect_status);
    if constexpr(sizeof(std::size_t)<8)assert(!outputs::valid({UINT64_C(1)<<32,1,UINT64_C(1)<<32,1}));
}
static void allocation_atomicity()
{
    outputs::registry registry;assert(registry.bind("cache","scratch",{16,1,16,0})==error::success);
    auto begin=request({1,16});receipt result{999};
    for(int position=0;allocation_injection_enabled&&position!=2;++position)
    {
        allocation_fail_after=position;auto code=registry.mutate("cache",op::begin,begin,1,result);allocation_fail_after=-1;
        assert(code==error::no_memory&&result.key==999);
        receipt absent;assert(registry.receipt_for("cache",1,absent)==error::conflict);
        uwvm2::utils::container::std_vector<outputs::snapshot> rows;assert(registry.inspect(rows,2)&&rows.empty());
    }
    auto first=mutate(registry,op::begin,begin);assert(first.object&&first.retained_bytes==16);
    auto commit=request({2,first.object,0});allocation_fail_after=0;
    assert(registry.mutate("cache",op::commit,commit,1,result)==error::success);allocation_fail_after=-1;
    assert(result.flags==1&&result.total_bytes==0&&result.retained_bytes==16);

    // A zero-transfer binding rejects a large read before allocating a copy.
    // Keep allocation failure armed throughout the real broker path; any
    // attempted staging allocation would throw instead of returning too_large.
    scene::broker broker;int identity{};scene::caller who{&identity,1};auto value=grant("cache");
    assert(broker.permissions.expose("capability.open",scene::exposure::allow)==error::success);
    for(auto const& action:value.actions)assert(broker.permissions.expose(action,scene::exposure::allow)==error::success);
    assert(broker.permissions.configure(value)==error::success);
    assert(broker.output_store.bind("cache","scratch",{16,1,16,0})==error::success);
    assert(broker.permissions.enroll(who)==error::success);
    scene::handle cap{};assert(broker.permissions.open(who,"cache",cap)==error::success);
    auto object=mutate(broker.output_store,op::begin,request({1,16})).object;
    assert(!mutate(broker.output_store,op::commit,request({2,object,0})).effect_status);
    auto read=request({object,0,outputs::maximum_transfer});std::size_t actual=999,required=999;
    scene::sink deliver=[](scene::bytes){assert(false);return true;};
    allocation_fail_after=0;
    assert(broker.output_store.read_size("cache",object,0,outputs::maximum_transfer,actual)==error::too_large&&actual==0);
    assert(broker.invoke_artifact(who,uwvm2::imported::ros::artifact_contract::operation::read,cap,read,
        outputs::maximum_transfer,required,deliver)==error::too_large&&required==0);
    assert(broker.preflight_output(who,op::append,cap,25,true)==error::too_large);
    assert(broker.preflight_output(who,op::commit,cap,24,true)==error::success);
    assert(allocation_fail_after.load()==0);allocation_fail_after=-1;
}
static void broker_delivery_and_admission()
{
    scene::broker broker;int identity{};scene::caller who{&identity,1};auto value=grant("cache");
    value.limits.calls=3;value.limits.uncertain=true;value.limits.window=std::chrono::hours(1);install(broker,who,value);
    scene::handle cap{};assert(broker.permissions.open(who,"cache",cap)==error::success);
    auto begin=request({1,16});std::size_t required{};receipt saved;
    auto deliver=[&](scene::bytes bytes){saved=decode(bytes);return true;};
    assert(broker.invoke_output(who,op::begin,cap,begin,71,required,deliver)==error::too_large&&required==72);
    uwvm2::utils::container::std_vector<scene::authority_snapshot> accounts;assert(broker.permissions.inspect(accounts,2)&&accounts[0].admissions==0);
    assert(broker.invoke_output(who,op::begin,cap,begin,72,required,[](scene::bytes){return false;})==error::invalid);
    assert(broker.invoke_output(who,op::begin,cap,begin,72,required,deliver)==error::success);auto id=saved.object;
    assert(id&&saved.retained_bytes==16);
    assert(broker.permissions.close(who,cap)==error::success);assert(broker.permissions.open(who,"cache",cap)==error::success);
    assert(broker.invoke_output(who,op::receipt,cap,request({1}),72,required,deliver)==error::success&&saved.object==id);
    assert(broker.invoke_output(who,op::abort,cap,request({2,id}),72,required,deliver)==error::again);
    assert(broker.permissions.inspect(accounts,2)&&accounts[0].admissions==3);
    scene::caller foreign{&required,1};assert(broker.invoke_output(foreign,op::receipt,cap,request({1}),72,required,deliver)==error::denied);

    // Exhausted authority must reject before copying, without changing any
    // counters. Short-receipt size queries still inspect authority only.
    allocation_fail_after=0;
    assert(broker.preflight_output(who,op::append,cap,32,true)==error::again);
    assert(broker.preflight_output(who,op::append,cap,32,false)==error::success);
    assert(broker.preflight_output(foreign,op::append,cap,32,false)==error::denied);
    assert(allocation_fail_after.load()==0);allocation_fail_after=-1;
    assert(broker.permissions.inspect(accounts,2)&&accounts[0].admissions==3&&accounts[0].refused==1);

    for(bool byte_limit:{false,true})
    {
        scene::broker zero;auto empty_policy=grant("cache");empty_policy.limits.uncertain=true;
        if(byte_limit)empty_policy.limits.bytes=0;else empty_policy.limits.calls=0;
        install(zero,who,empty_policy);scene::handle empty_cap{};
        assert(zero.permissions.open(who,"cache",empty_cap)==error::success);
        allocation_fail_after=0;
        assert(zero.preflight_output(who,op::append,empty_cap,32,true)==error::again);
        assert(zero.preflight_output(who,op::append,empty_cap,32,false)==error::success);
        assert(allocation_fail_after.load()==0);allocation_fail_after=-1;
        assert(zero.permissions.inspect(accounts,2)&&!accounts[0].admissions&&!accounts[0].refused);

        scene::broker bounded;auto policy=grant("cache");policy.limits.uncertain=true;
        policy.limits.window=std::chrono::hours(1);
        if(byte_limit)policy.limits.bytes=88;else policy.limits.calls=1;
        install(bounded,who,policy);scene::handle bounded_cap{};
        assert(bounded.permissions.open(who,"cache",bounded_cap)==error::success);
        // Preview neither reserves the first call nor advances a window. The
        // real first mutation uses all 88 bytes / the single call allowance.
        for(int n=0;n!=3;++n)assert(bounded.preflight_output(who,op::begin,bounded_cap,16,true)==error::success);
        assert(bounded.permissions.inspect(accounts,2)&&!accounts[0].admissions&&!accounts[0].refused);
        if(byte_limit)assert(bounded.preflight_output(who,op::append,bounded_cap,24,true)==error::again);
        assert(bounded.invoke_output(who,op::begin,bounded_cap,begin,72,required,deliver)==error::success);
        assert(bounded.preflight_output(who,op::begin,bounded_cap,16,true)==error::again);
        assert(bounded.preflight_output(who,op::begin,bounded_cap,16,false)==error::success);
        // A competing admission after preflight still wins atomically; no
        // earlier preview permits this next effect to exceed the quota.
        assert(bounded.invoke_output(who,op::begin,bounded_cap,begin,72,required,deliver)==error::again);
        assert(bounded.permissions.inspect(accounts,2)&&accounts[0].admissions==1&&accounts[0].refused==1);
    }
}
static void deployment_reservations()
{
    auto configure=[](scene::deployment& deployment,std::uint64_t memory,std::uint64_t objects,std::uint64_t receipts)
    {
        assert(deployment.create("robot")==error::success);assert(deployment.bind("robot","main")==error::success);
        assert(deployment.expose("robot","capability.open",scene::exposure::allow)==error::success);
        assert(deployment.allow("robot",grant("cache"))==error::success);
        assert(deployment.output("robot","cache",{16,2,8,4})==error::success);
        assert(deployment.output_budget("robot",memory,objects,receipts)==error::success);
    };
    for(auto limits:std::array{std::array<std::uint64_t,3>{15,2,100},std::array<std::uint64_t,3>{16,1,100},std::array<std::uint64_t,3>{16,2,99}})
    {
        scene::deployment denied;configure(denied,limits[0],limits[1],limits[2]);int calls{};
        assert(denied.validate()==error::invalid);
        assert(denied.prepare_resources([&](auto const&,auto,auto,auto&){++calls;return error::success;})==error::invalid&&calls==0);
    }
    scene::deployment live;configure(live,16,2,100);assert(live.validate()==error::success);
    int identity{},calls{};scene::caller who{&identity,1};std::shared_ptr<scene::broker> selected;
    assert(live.select("main",who,selected)==error::denied&&!selected);
    assert(live.prepare_resources([&](auto const&,auto,auto,auto&){++calls;return error::denied;})==error::success&&calls==0);
    assert(live.select("main",who,selected)==error::success&&selected);
    auto first=mutate(selected->output_store,op::begin,request({1,8}));assert(first.object);
    live.retire_generation(1);receipt denied;assert(selected->output_store.status("cache",first.object,denied)==error::denied);
}
static void completion_boundaries()
{
    scene::broker broker;int identity{};scene::caller who{&identity,1};install(broker,who,grant("cache"));
    scene::handle cap{};assert(broker.permissions.open(who,"cache",cap)==error::success);
    std::size_t required{999};receipt saved;bool copied{};
    auto deliver=[&](scene::bytes bytes){copied=true;saved=decode(bytes);return true;};
    auto begin=request({1,16});assert(broker.invoke_output(who,op::begin,cap,begin,72,required,deliver)==error::success);
    auto initial=saved;auto oversized=request({2,initial.object,0},"123456789");
    for(auto capacity:{71u,72u})
    {
        copied=false;required=999;
        assert(broker.invoke_output(who,op::append,cap,oversized,capacity,required,deliver)==error::too_large&&!copied&&!required);
        receipt current;assert(broker.output_store.receipt_for("cache",1,current)==error::success&&current==initial);
    }
    copied=false;
    assert(broker.invoke_output(who,op::append,cap,oversized,72,required,deliver,[]{return true;})==error::cancelled&&!copied);
    // Failed delivery retains exactly one accepted append; retrying its bytes
    // recovers that receipt without appending twice or consuming another key.
    auto append=request({2,initial.object,0},"12345678");
    assert(broker.invoke_output(who,op::append,cap,append,72,required,[](scene::bytes){return false;})==error::invalid);
    assert(broker.invoke_output(who,op::append,cap,append,72,required,deliver)==error::success&&saved.total_bytes==8);
    auto before=saved;bool checked{};
    auto revoke=[&]{if(!checked){checked=true;assert(broker.permissions.revoke("cache",[](std::string_view){})==error::success);}return false;};
    copied=false;auto commit=request({3,initial.object,8});
    assert(broker.invoke_output(who,op::commit,cap,commit,72,required,deliver,revoke)==error::denied&&!copied);
    receipt current;assert(broker.output_store.receipt_for("cache",2,current)==error::success&&current==before);
}
static void concurrent_and_fork()
{
    outputs::registry registry;assert(registry.bind("cache","scratch",{16,1,16,4})==error::success);
    auto begin=request({1,16});std::array<receipt,8> results;uwvm2::utils::container::std_vector<std::thread> threads;
    for(auto& result:results)threads.emplace_back([&registry,&begin,&result]{assert(registry.mutate("cache",op::begin,begin,1,result)==error::success);});
    for(auto& thread:threads)thread.join();
    for(auto const& result:results)assert(result==results[0]);
    auto id=results[0].object;assert(!mutate(registry,op::append,request({2,id,0},"data")).effect_status);
    assert(!mutate(registry,op::commit,request({3,id,4})).effect_status);
    auto pid=::fork();assert(pid>=0);
    if(!pid)
    {
        receipt result;uwvm2::utils::container::std_vector<outputs::snapshot> rows;std::array<std::uint8_t,4> data{};std::size_t count{};
        bool denied=registry.status("cache",id,result)==error::denied&&registry.receipt_for("cache",3,result)==error::denied&&
            registry.mutate("cache",op::remove,request({4,id}),1,result)==error::denied&&registry.read("cache",id,0,data,count)==error::denied&&
            registry.read_size("cache",id,0,4,count)==error::denied&&registry.request_size("cache",24)==error::denied&&
            registry.bind("child","scratch",{1,1,1,1})==error::denied&&!registry.inspect(rows,2);
        ::_exit(denied?0:2);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    std::atomic<bool> running{},finished{};std::thread reader([&]
    {
        running=true;std::array<std::uint8_t,4> data{};std::size_t count{};
        while(!finished.load())
        {auto code=registry.read("cache",id,0,data,count);assert(code==error::success||code==error::denied);if(code==error::success)assert(count==4&&data[0]=='d');}
    });
    while(!running.load())std::this_thread::yield();
    registry.revoke("cache");finished=true;reader.join();
    receipt result;assert(registry.status("cache",id,result)==error::denied);
    std::size_t count{};assert(registry.read_size("cache",id,0,4,count)==error::denied&&registry.request_size("cache",24)==error::denied);
    uwvm2::utils::container::std_vector<outputs::snapshot> rows;assert(registry.inspect(rows,2)&&rows.size()==1&&rows[0].retired&&rows[0].reserved==16);
    registry.retire();assert(registry.bind("later","scratch",{1,1,1,1})==error::denied);
}
int main()
{
    lifecycle();ordering_capacity_and_privacy();allocation_atomicity();broker_delivery_and_admission();deployment_reservations();completion_boundaries();concurrent_and_fork();
    if(!allocation_injection_enabled)std::cout<<"TSan allocator interceptors active; allocation fault injection qualified separately\n";
    std::cout<<"passed 7 volatile-output safety groups\n";
}
