/** @file @brief Receiver-wide ownership across distinct authenticated VM origins. */
#include <uwvm2/imported/ros/scene/driver_control.h>
#include <cassert>
#include <barrier>
#include <thread>
#include <cstdio>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;namespace d=s::drivers;namespace p=d::protocol;namespace c=s::control;
using d::error;
static constexpr d::credentials first{101,1000,1000},second{102,1000,1000};
static p::binding identity(unsigned n)
{return {{1,0},{n+11,0},{n+21,0},{41,0},{n+51,0},{1,2,3,4}};}
struct fixture
{
    d::receiver_control gate;c::handle a{},b{};c::lease owner;
    fixture()
    {
        auto one=identity(0);assert(gate.configure(one.deployment,one.driver,one.contract,7,2,{1000,100,10,true})==error::success);
        assert(gate.bind(one,first,5000,1000,a)==error::success);
        assert(gate.bind(identity(1),second,5000,1000,b)==error::success);
        assert(gate.freeze()==error::success);
    }
    void grant(){assert(gate.grant(a,1,900,1000,owner)==error::success);}
    static p::header request(unsigned n,std::uint64_t sequence=1){return {p::kind::request,identity(n),sequence,1800,0};}
};
static c::status inspect(d::receiver_control const& gate){c::status value;assert(gate.inspect(value)==error::success);return value;}
static void closed_registration()
{
    d::receiver_control gate;auto one=identity(0);c::handle id=77;
    assert(gate.bind(one,first,5000,1000,id)==error::denied&&id==77);
    assert(gate.configure(one.deployment,one.driver,one.contract,7,0,{1000,100,10,true})==error::invalid);
    assert(gate.configure({},one.driver,one.contract,7,1,{1000,100,10,true})==error::invalid);
    assert(gate.configure(one.deployment,one.driver,one.contract,7,1,{1000,100,10,true})==error::success);
    auto wrong=one;wrong.driver[0]++;assert(gate.bind(wrong,first,5000,1000,id)==error::invalid);
    wrong=one;wrong.deployment[1]++;
    assert(gate.bind(wrong,first,5000,1000,id)==error::denied&&id==77);
    assert(gate.bind(one,first,5000,1000,id)==error::success);auto retained=id;
    assert(gate.bind(one,first,5000,1000,id)==error::conflict&&id==retained);
    assert(gate.bind(identity(1),second,5000,1000,id)==error::too_large&&id==retained);
    assert(gate.bind(wrong,second,5000,1000,id)==error::denied&&id==retained);
    assert(gate.freeze()==error::success&&gate.bind(one,first,5000,1000,id)==error::denied);
}
static void registration_is_not_grant()
{
    fixture f;c::handle effect=999;
    assert(f.gate.begin(f.a,first,fixture::request(0),17,2,1001,effect)==error::conflict&&effect==999);
    f.grant();assert(f.gate.begin(f.a,first,fixture::request(0),17,f.owner.epoch,1001,effect)==error::conflict);
    assert(f.gate.begin(f.a,first,fixture::request(0,2),17,f.owner.epoch,1001,effect)==error::success);
    assert(f.gate.finish(f.b,effect,c::effect_result::executed)==error::denied);
    assert(f.gate.finish(f.a,effect,c::effect_result::executed)==error::success);
}
static void identities_deadlines_and_replay()
{
    fixture f;f.grant();c::handle effect=999;auto request=fixture::request(0);
    assert(f.gate.begin(f.a,second,request,17,f.owner.epoch,1001,effect)==error::denied);
    auto changed=request;changed.bound.vm[0]++;assert(f.gate.begin(f.a,first,changed,17,f.owner.epoch,1001,effect)==error::denied);
    changed=request;changed.deadline=5001;assert(f.gate.begin(f.a,first,changed,17,f.owner.epoch,1001,effect)==error::timeout);
    changed=request;changed.sequence=2;assert(f.gate.begin(f.a,first,changed,17,f.owner.epoch,1001,effect)==error::conflict);
    assert(effect==999);assert(f.gate.begin(f.a,first,request,17,f.owner.epoch,1001,effect)==error::success);
    auto accepted=effect;assert(f.gate.begin(f.a,first,request,17,f.owner.epoch,1001,effect)==error::conflict&&effect==accepted);
    assert(f.gate.finish(f.a,effect,c::effect_result::executed)==error::success);
}
static void same_deployment_handover()
{
    fixture f;f.grant();c::handle effect{};c::lease next;
    assert(f.gate.begin(f.b,second,fixture::request(1),17,f.owner.epoch,1001,effect)==error::conflict&&!effect);
    assert(f.gate.grant(f.b,1,500,1002,next)==error::denied);
    assert(f.gate.grant(f.b,2,900,1002,next)==error::success&&next.epoch>f.owner.epoch);
    assert(f.gate.begin(f.a,first,fixture::request(0),17,f.owner.epoch,1003,effect)==error::conflict);
    assert(f.gate.release(f.a)==error::success&&!inspect(f.gate).latched);
    assert(f.gate.begin(f.b,second,fixture::request(1,2),17,next.epoch,1003,effect)==error::success);
    assert(f.gate.finish(f.b,effect,c::effect_result::executed)==error::success);
    assert(f.gate.release(f.b)==error::success&&inspect(f.gate).latched&&inspect(f.gate).stop);
}
static void unknown_requires_independent_reconciliation()
{
    fixture f;f.grant();c::handle effect{};c::lease next;
    assert(f.gate.begin(f.a,first,fixture::request(0),17,f.owner.epoch,1001,effect)==error::success);
    assert(f.gate.release(f.a)==error::success);auto state=inspect(f.gate);assert(state.outcome_unknown&&state.effect==effect&&state.stop);
    assert(f.gate.finish(f.a,effect,c::effect_result::no_effect)==error::conflict);
    assert(f.gate.grant(f.b,2,900,1002,next)==error::cancelled);
    assert(f.gate.reconcile_stopped(state.stop,1,0)==error::denied);
    assert(f.gate.reconcile_stopped(state.stop,0,1)==error::denied);
    assert(f.gate.reconcile_stopped(state.stop,1,1)==error::success);
    state=inspect(f.gate);assert(!state.controller.id&&!state.effect&&!state.outcome_unknown&&!state.latched);
    assert(f.gate.grant(f.b,2,900,1002,next)==error::success);
    assert(f.gate.grant(f.a,3,500,1002,next)==error::denied);
}
static void concurrent_effect_and_expiry()
{
    fixture f;f.grant();std::barrier start{3};std::atomic<unsigned> successes{};c::handle winner{};
    auto submit=[&]{start.arrive_and_wait();c::handle effect{};auto result=f.gate.begin(f.a,first,fixture::request(0),17,f.owner.epoch,1001,effect);
        if(result==error::success){++successes;winner=effect;}else assert(result==error::conflict);};
    std::thread a{submit},b{submit};start.arrive_and_wait();a.join();b.join();assert(successes==1&&winner);
    c::lease next;assert(f.gate.grant(f.b,2,900,1002,next)==error::again);
    assert(f.gate.finish(f.a,winner,c::effect_result::executed)==error::success);
    auto late=fixture::request(0,2);late.deadline=2000;c::handle effect{};
    assert(f.gate.begin(f.a,first,late,17,f.owner.epoch,1900,effect)==error::timeout&&!effect);
    assert(f.gate.grant(f.b,2,500,1900,next)==error::timeout);
    c::handle stop{};assert(f.gate.stop(stop)==error::success);assert(f.gate.reconcile_stopped(stop,1,0)==error::success);
    assert(f.gate.grant(f.b,2,500,1901,next)==error::success);
}
static void fork_rejected_before_mutex()
{
    fixture f;f.grant();auto pid=::fork();assert(pid>=0);
    if(!pid){::alarm(3);c::status state;c::handle stop{};::_exit(f.gate.inspect(state)==error::denied&&f.gate.stop(stop)==error::denied&&f.gate.release(f.a)==error::denied?0:1);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
int main()
{closed_registration();registration_is_not_grant();identities_deadlines_and_replay();same_deployment_handover();unknown_requires_independent_reconciliation();concurrent_effect_and_expiry();fork_rejected_before_mutex();std::puts("PASS 7 receiver ownership security groups");}
