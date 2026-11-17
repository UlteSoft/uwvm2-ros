/** @file @brief Retained TF budgets, failed publication and process-safe session ownership. */
#include <uwvm2/imported/ros/scene/transform_registry.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace t=uwvm2::imported::ros::scene::transforms;
namespace c=uwvm2::imported::ros::transform_contract;
namespace b=uwvm2::imported::ros::scene::buffers;
using t::error;
static thread_local std::size_t fail_after{};
static bool fail() noexcept{return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
static constexpr t::clock_context clock_sample{7,0,1000,true};
static c::name name(std::string_view text){c::name value;assert(value.assign(text));return value;}
static t::plan plan()
{
    t::plan result;auto& s=result.session;auto& p=s.history;
    assert(s.profile.identity.assign("frames"));s.profile.sha256.fill(1);
    p.frames={name("base"),name("map")};p.frame_count=2;p.edges={t::edge{1,0,true,false}};p.edge_count=1;
    p.max_samples=16;p.samples_per_child=8;p.max_batch=8;p.history_ns=1000;p.geometry={1000,1e-12};p.velocity_window_ns=100;
    p.linear_speed=p.angular_speed=10;assert(t::cache::bank_bytes(p,p.data_bytes));p.workspace_bytes=p.data_bytes;
    s.requests=4;s.deadline_ns=1000;assert(t::session::request_storage(s,s.request_bytes));
    result.sessions=1;result.data_bytes=p.data_bytes;result.workspace_bytes=p.workspace_bytes;result.request_bytes=s.request_bytes;result.carrier_bytes=128;
    return result;
}
static t::usage usage(t::registry& registry,std::string_view alias="frames")
{t::usage value;assert(registry.inspect(alias,value)==error::success);return value;}
static std::uint64_t open(t::registry& registry)
{std::uint64_t id{};assert(registry.open("frames",plan().session.profile,88,clock_sample,1,[&](auto value){id=value;return true;})==error::success);return id;}
static void exact_binding_and_failed_handle_delivery()
{
    t::registry registry;auto p=plan();assert(registry.bind("frames",p)==error::success);
    assert(registry.bind("frames",p)==error::conflict);
    auto wrong=p.session.profile;wrong.sha256[0]^=1;
    assert(registry.open("frames",wrong,88,clock_sample,1,[](auto){return true;})==error::denied&&!usage(registry).retained_sessions);
    assert(registry.open("frames",p.session.profile,88,clock_sample,0,[](auto){return true;})==error::too_large);
    auto failed=[&](auto id)
    {
        std::shared_ptr<t::session> value;assert(registry.borrow("frames",id,value)==error::bad_handle);
        assert(registry.close("frames",id)==error::bad_handle);
        assert(registry.open("frames",p.session.profile,88,clock_sample,1,[](auto){return true;})==error::too_large);
        return false;
    };
    assert(registry.open("frames",p.session.profile,88,clock_sample,1,failed)==error::io);
    auto u=usage(registry);assert(!u.retained_sessions&&!u.open_handles&&!u.data_bytes&&!u.request_bytes);
    auto id=open(registry);std::shared_ptr<t::session> value;
    assert(registry.borrow("other",id,value)==error::bad_handle);
    assert(registry.bind("other",p)==error::success&&registry.borrow("other",id,value)==error::bad_handle);
    assert(registry.borrow("frames",id,value)==error::success);
    t::session_usage state;assert(value->inspect(state)==error::success&&state.clock==7&&state.clock_capability==88&&state.id==id);
}
static void closed_handles_keep_retained_capacity()
{
    t::registry registry;auto p=plan();assert(registry.bind("frames",p)==error::success);auto id=open(registry);
    std::shared_ptr<t::session> held;assert(registry.borrow("frames",id,held)==error::success);
    assert(registry.close("frames",id)==error::success);auto u=usage(registry);
    assert(u.retained_sessions==1&&!u.open_handles&&u.data_bytes==p.data_bytes&&u.request_bytes==p.request_bytes);
    assert(registry.open("frames",p.session.profile,88,clock_sample,1,[](auto){return true;})==error::too_large);
    c::transform out;c::query q{name("map"),name("base"),{},{},{},false,false,false};assert(held->lookup(q,clock_sample,out)==error::cancelled);
    held.reset();u=usage(registry);assert(!u.retained_sessions&&!u.data_bytes&&!u.workspace_bytes&&!u.request_bytes);
    auto next=open(registry);assert(next!=id);
}
static void concurrent_borrow_close_and_revoke()
{
    t::registry registry;auto p=plan();assert(registry.bind("frames",p)==error::success);auto id=open(registry);
    std::barrier held{2},release{2};
    std::thread reader([&]
    {
        std::shared_ptr<t::session> session;assert(registry.borrow("frames",id,session)==error::success);
        held.arrive_and_wait();release.arrive_and_wait();c::transform out;c::query q{name("map"),name("base"),{},{},{},false,false,false};
        assert(session->lookup(q,clock_sample,out)==error::cancelled);
    });
    held.arrive_and_wait();registry.revoke("frames");assert(usage(registry).retained_sessions==1&&usage(registry).retired);
    std::shared_ptr<t::session> denied;assert(registry.borrow("frames",id,denied)==error::cancelled);
    release.arrive_and_wait();reader.join();assert(!usage(registry).retained_sessions);
    assert(registry.open("frames",p.session.profile,88,clock_sample,1,[](auto){return true;})==error::cancelled);
}
static void carrier_reservation_and_retirement()
{
    t::registry registry;auto p=plan();assert(registry.bind("frames",p)==error::success);
    t::registry::carrier_lease first,second;assert(registry.reserve_carrier("frames",100,first)==error::success&&first.live());
    assert(registry.reserve_carrier("frames",29,second)==error::too_large&&usage(registry).carrier_bytes==100);
    assert(registry.reserve_carrier("frames",28,second)==error::success&&usage(registry).carrier_bytes==128);
    first=std::move(second);assert(usage(registry).carrier_bytes==28&&!second&&first.live());
    registry.revoke("frames");assert(!first.live()&&usage(registry).carrier_bytes==28);first.reset();assert(!usage(registry).carrier_bytes);
    std::shared_ptr<t::session> held;
    {
        t::registry temporary;assert(temporary.bind("frames",p)==error::success);auto id=open(temporary);
        assert(temporary.borrow("frames",id,held)==error::success);
    }
    c::transform out;c::query q{name("map"),name("base"),{},{},{},false,false,false};assert(held->lookup(q,clock_sample,out)==error::cancelled);
}
static void real_allocation_faults()
{
    auto p=plan();
    for(std::size_t fault=1;fault<=5;++fault)
    {
        t::registry registry;assert(registry.bind("frames",p)==error::success);fail_after=fault;bool copied{};
        auto status=registry.open("frames",p.session.profile,88,clock_sample,1,[&](auto){copied=true;return true;});
        assert(status==error::no_memory&&!fail_after&&!copied&&!usage(registry).retained_sessions);
        auto id=open(registry);assert(registry.close("frames",id)==error::success&&!usage(registry).retained_sessions);
    }
    t::registry registry;assert(registry.bind("frames",p)==error::success);
    auto revoked=[&](auto){registry.revoke("frames");return true;};
    assert(registry.open("frames",p.session.profile,88,clock_sample,1,revoked)==error::cancelled&&!usage(registry).retained_sessions);
}
static void fork_and_configuration_bounds()
{
    auto p=plan();assert(t::valid(p));auto bad=p;bad.data_bytes--;assert(!t::valid(bad));bad=p;bad.workspace_bytes--;assert(!t::valid(bad));
    bad=p;bad.request_bytes--;assert(!t::valid(bad));bad=p;bad.session.history.data_bytes++;assert(!t::valid(bad));
    bad=p;bad.session.deadline_ns=UINT64_MAX;assert(!t::valid(bad));
    t::registry registry;assert(registry.bind("frames",p)==error::success);auto id=open(registry);
    t::registry::carrier_lease token;assert(registry.reserve_carrier("frames",1,token)==error::success);
    auto pid=::fork();assert(pid>=0);
    if(!pid)
    {
        std::shared_ptr<t::session> held;t::usage use;std::size_t bound{};b::profile profile;
        assert(!token.live()&&registry.borrow("frames",id,held)==error::denied&&registry.inspect("frames",use)==error::denied);
        assert(registry.close("frames",id)==error::denied&&registry.bind("other",p)==error::denied);
        assert(registry.limits("frames",c::operation::open,bound,profile)==error::denied);
        registry.retire();::_exit(0);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0&&token.live()&&!usage(registry).retired);
}
int main()
{
    exact_binding_and_failed_handle_delivery();closed_handles_keep_retained_capacity();concurrent_borrow_close_and_revoke();
    carrier_reservation_and_retirement();real_allocation_faults();fork_and_configuration_bounds();
    std::cout<<"passed 6 transform registry security groups\n";
}
