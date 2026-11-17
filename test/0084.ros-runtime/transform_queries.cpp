/** @file
 * @brief Verify recoverable bounded TF waits, exact retries and concurrent delivery fences.
 */
#include <uwvm2/imported/ros/scene/transform_queries.h>
#include <cassert>
#include <iostream>
#include <atomic>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace t=uwvm2::imported::ros::scene::transforms;
namespace c=uwvm2::imported::ros::transform_contract;
namespace b=uwvm2::imported::ros::scene::buffers;
using t::error;
static thread_local std::size_t allocations{},fail_after{};
static bool fail() noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
static std::atomic<std::uint64_t> now{100};
static std::uint64_t clock_now() noexcept{return now.load();}
static constexpr t::clock_context clock_sample{7,0,10000,true};
static c::name name(std::string_view text){c::name value;assert(value.assign(text));return value;}
static b::digest digest(unsigned char n){b::digest value{};value.fill(n);return value;}
static t::session_plan plan()
{
    t::session_plan value;assert(value.profile.identity.assign("robot.frames"));value.profile.sha256=digest(9);
    auto& p=value.history;p.frames={name("base"),name("camera"),name("map")};p.frame_count=3;
    p.edges={t::edge{2,0,true,false},t::edge{0,1,false,true}};p.edge_count=2;
    p.max_samples=32;p.samples_per_child=16;p.max_batch=8;p.history_ns=10000;p.geometry={1000,1e-12};
    p.linear_speed=p.angular_speed=10;p.velocity_window_ns=10000;
    assert(t::cache::bank_bytes(p,p.data_bytes));p.workspace_bytes=p.data_bytes;
    value.requests=4;value.deadline_ns=1000;assert(t::session::request_storage(value,value.request_bytes));return value;
}
static c::query query(std::int64_t time=10)
{return {name("map"),name("base"),{},{7,0,time},{7,0,time},true,true,false};}
static c::transform record(std::int64_t time=10)
{return {name("map"),name("base"),{7,0,time},{{static_cast<double>(time),0,0},{}},false};}
struct fixture
{
    t::session session{clock_now};std::uint64_t id{};
    explicit fixture(t::session_plan p=plan())
    {now=100;assert(session.configure(p,88,clock_sample)==error::success);t::session_usage use;assert(session.inspect(use)==error::success);id=use.id;assert(id);}
    t::receipt submit(std::uint64_t key=1,std::int64_t time=10)
    {t::receipt out;assert(session.submit({id,key},digest(key),query(time),500,clock_sample,out)==error::success);return out;}
    t::receipt publish(std::uint64_t key,std::int64_t time=10)
    {auto value=record(time);t::receipt out;assert(session.publish({id,key},digest(key),{&value,1},clock_sample,false,out)==error::success);return out;}
    t::session_usage usage(){t::session_usage value;assert(session.inspect(value)==error::success);return value;}
};
static void exact_key_stream_and_capacity()
{
    fixture f;t::receipt out;auto first=f.submit();assert(first.state==t::query_state::queued&&f.usage().pending==1);
    bool replay{};assert(f.session.replay(c::operation::submit,{f.id,1},digest(1),replay,out)==error::success&&replay&&out==first);
    assert(f.session.submit({f.id,1},digest(2),query(),500,clock_sample,out)==error::conflict&&f.usage().requests==1);
    assert(f.session.submit({f.id,3},digest(3),query(),500,clock_sample,out)==error::conflict&&f.usage().last_sequence==1);
    assert(f.session.submit({f.id+1,2},digest(2),query(),500,clock_sample,out)==error::denied);
    auto bad=query();bad.source=name("hidden");assert(f.session.submit({f.id,2},digest(2),bad,500,clock_sample,out)==error::denied&&f.usage().last_sequence==1);
    auto second=f.submit(2);f.submit(3);f.submit(4);
    assert(f.session.submit({f.id,5},digest(5),query(),500,clock_sample,out)==error::too_large&&f.usage().last_sequence==4);
    assert(f.session.release(second.request)==error::again);
    assert(f.session.cancel(second.request,out)==error::success&&out.state==t::query_state::cancelled);
    assert(f.session.release(second.request)==error::success);f.submit(5);
    assert(f.session.replay(c::operation::submit,{f.id,1},digest(1),replay,out)==error::conflict);
    auto before=f.usage();auto value=record();
    assert(f.session.publish({f.id,6},digest(6),{&value,1},clock_sample,false,out)==error::success&&out.mutation&&f.usage().pending==0);
    auto generation=f.usage().history.generation;
    assert(f.session.publish({f.id,6},digest(6),{},clock_sample,false,out)==error::success&&f.usage().history.generation==generation); // trusted digest proof
    assert(f.usage().requests==before.requests);
}
static void failed_delivery_and_historical_receipt()
{
    fixture f;auto first=f.submit();f.publish(2);t::receipt out;
    unsigned calls{};auto failed=[&](t::receipt const& value,c::transform const* result)
    {++calls;assert(value.request==first.request&&value.state==t::query_state::succeeded&&result&&result->pose.translation.x==10);return false;};
    assert(f.session.take(first.request,clock_sample,failed)==error::io&&calls==1);
    auto throwing=[](auto const&,auto const*)->bool{throw 1;};assert(f.session.take(first.request,clock_sample,throwing)==error::io);
    auto copied=[&](t::receipt const& value,c::transform const* result)
    {
        assert(result&&value.request==first.request);assert(f.session.release(first.request)==error::again);
        assert(f.session.take(first.request,clock_sample,failed)==error::again);return true;
    };
    assert(f.session.take(first.request,clock_sample,copied)==error::success);
    assert(f.session.take(first.request,clock_sample,copied)==error::again&&f.session.release(first.request)==error::success);
    assert(f.session.describe(first.request,clock_sample,out)==error::bad_handle);
    // Last-key recovery remains valid after request release and needs no live clock.
    auto immediate=f.submit(3);assert(immediate.state==t::query_state::succeeded);
    assert(f.session.release(immediate.request)==error::success);bool replay{};
    assert(f.session.replay(c::operation::submit,{f.id,3},digest(3),replay,out)==error::success&&replay&&out.request==immediate.request&&out.state==t::query_state::succeeded);
    auto invalid_clock=clock_sample;invalid_clock.identity=999;
    assert(f.session.submit({f.id,3},digest(3),{},0,invalid_clock,out)==error::success&&out.request==immediate.request&&f.usage().requests==0);
}
static void deadline_epoch_and_inspection()
{
    fixture f;t::receipt out;auto first=f.submit();auto before=f.usage();now=600;
    assert(f.usage().pending==1&&f.usage().history.generation==before.history.generation); // inspect is inert
    bool ready{};assert(f.session.ready(clock_sample,ready)==error::success&&ready&&f.usage().pending==0);
    assert(f.session.describe(first.request,clock_sample,out)==error::success&&out.state==t::query_state::failed&&out.reason==t::query_reason::deadline);
    assert(f.session.submit({f.id,2},digest(2),query(),600,clock_sample,out)==error::timeout&&f.usage().last_sequence==1);
    assert(f.session.submit({f.id,2},digest(2),query(),1601,clock_sample,out)==error::too_large);
    now=100;auto second=f.submit(2);auto changed=clock_sample;changed.epoch=1;
    assert(f.session.take(second.request,changed,[](auto const&,auto const*){return true;})==error::conflict);
    auto generation=f.usage().history.generation;
    assert(f.session.clear({f.id,3},digest(3),generation+1,changed,out)==error::conflict&&f.usage().last_sequence==2);
    assert(f.session.clear({f.id,3},digest(3),generation,changed,out)==error::success);
    assert(f.session.describe(second.request,changed,out)==error::success&&out.state==t::query_state::cancelled&&out.reason==t::query_reason::history_cleared);
    assert(f.session.take(second.request,changed,[](auto const& r,auto const* value){return r.state==t::query_state::cancelled&&!value;})==error::success);
}
static void reentrant_clear_and_retire()
{
    fixture f;auto first=f.submit();f.publish(2);auto generation=f.usage().history.generation;
    auto clear=[&](auto const&,auto const* result)
    {assert(result);t::receipt out;assert(f.session.clear({f.id,3},digest(3),generation,clock_sample,out)==error::success);return true;};
    assert(f.session.take(first.request,clock_sample,clear)==error::cancelled);
    assert(f.session.take(first.request,clock_sample,[](auto const& r,auto const* result){return r.state==t::query_state::cancelled&&!result;})==error::success);
    auto second=f.submit(4);f.publish(5);
    auto retire=[&](auto const&,auto const* result){assert(result);f.session.retire();return true;};
    assert(f.session.take(second.request,clock_sample,retire)==error::cancelled&&f.usage().retired);
    c::transform out;assert(f.session.lookup(query(),clock_sample,out)==error::cancelled);
}
static void concurrent_delivery_and_shutdown()
{
    fixture f;auto first=f.submit();f.publish(2);std::barrier entered{2},done{2};error result{};
    std::thread consumer([&]
    {
        auto copy=[&](auto const&,auto const* value){assert(value);entered.arrive_and_wait();done.arrive_and_wait();return true;};
        result=f.session.take(first.request,clock_sample,copy);
    });
    entered.arrive_and_wait();
    assert(f.session.take(first.request,clock_sample,[](auto const&,auto const*){return true;})==error::again);
    assert(f.session.release(first.request)==error::again);f.session.retire();done.arrive_and_wait();consumer.join();
    assert(result==error::cancelled);
}
static void fork_before_locks()
{
    fixture f;auto first=f.submit();auto pid=::fork();assert(pid>=0);
    if(!pid)
    {
        t::receipt out;t::session_usage use;bool replay{},ready{};c::transform transform;
        assert(f.session.inspect(use)==error::denied&&f.session.cancel(first.request,out)==error::denied&&f.session.release(first.request)==error::denied);
        assert(f.session.configure(plan(),88,clock_sample)==error::denied&&f.session.lookup(query(),clock_sample,transform)==error::denied);
        assert(f.session.replay(c::operation::submit,{f.id,1},digest(1),replay,out)==error::denied&&f.session.ready(clock_sample,ready)==error::denied);
        assert(f.session.submit({f.id,2},digest(2),query(),500,clock_sample,out)==error::denied);
        assert(f.session.publish({f.id,2},digest(2),{},clock_sample,false,out)==error::denied);
        assert(f.session.clear({f.id,2},digest(2),1,clock_sample,out)==error::denied);
        assert(f.session.take(first.request,clock_sample,[](auto const&,auto const*){return true;})==error::denied);
        assert(f.session.describe(first.request,clock_sample,out)==error::denied);f.session.retire();::_exit(0);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0&&!f.usage().retired);
}
static void allocation_recovery_and_steady_storage()
{
    for(std::size_t fault=1;fault<=3;++fault)
    {
        t::session session{clock_now};auto p=plan();fail_after=fault;assert(session.configure(p,88,clock_sample)==error::no_memory&&!fail_after);
        t::session_usage use;assert(session.inspect(use)==error::success&&!use.id&&!use.request_bytes&&!use.history.generation);
        assert(session.configure(p,88,clock_sample)==error::success);
    }
    fixture f;auto before=allocations;fail_after=1;auto first=f.submit();f.publish(2);
    assert(f.session.take(first.request,clock_sample,[](auto const&,auto const* result){return result;})==error::success);
    assert(f.session.release(first.request)==error::success);t::receipt out;auto usage=f.usage();
    assert(f.session.clear({f.id,3},digest(3),usage.history.generation,clock_sample,out)==error::success);
    assert(allocations==before&&fail_after==1);fail_after=0;
}
int main()
{
    exact_key_stream_and_capacity();failed_delivery_and_historical_receipt();deadline_epoch_and_inspection();
    reentrant_clear_and_retire();concurrent_delivery_and_shutdown();fork_before_locks();allocation_recovery_and_steady_storage();
    std::cout<<"passed 7 transform query security groups\n";
}
