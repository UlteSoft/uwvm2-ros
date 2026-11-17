#include <uwvm2/imported/ros/runtime/resources.h>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <latch>
#include <stdexcept>
#include <thread>
#include <vector>
namespace r=uwvm2::imported::ros::resources;
using namespace std::chrono_literals;
void initial_contract()
{
    r::registry a,b;r::handle guard{},set{},other{},clock{},timer{};
    assert(a.guard_create("",5,guard)==r::error::denied);
    assert(a.guard_create("signal",0,guard)==r::error::again);
    assert(a.guard_create("signal",2,guard)==r::error::success);
    assert(b.guard_create("signal",2,other)==r::error::success&&guard!=other);
    assert(a.guard_trigger("signal",other)==r::error::bad_handle);
    assert(a.guard_trigger("other",guard)==r::error::bad_handle);
    assert(a.wait_set_create("wait",1,set)==r::error::success);
    assert(a.wait_set_add("wait",set,"signal",guard,r::readable|r::closed,42,0)==r::error::again);
    assert(a.wait_set_add("wait",set,"signal",guard,r::readable|r::closed,42,4)==r::error::success);
    std::vector<r::ready_record> out;std::size_t required{};
    assert(a.wait("wait",set,std::chrono::steady_clock::now(),4,out,required)==r::error::timeout);
    auto wake=std::async(std::launch::async,[&]{return a.wait("wait",set,std::chrono::steady_clock::now()+2s,4,out,required);});
    // Notification before or during wait registration must work identically.
    assert(a.guard_trigger("signal",guard)==r::error::success);
    assert(wake.get()==r::error::success&&out.size()==1&&out[0].token==42);
    assert(a.wait("wait",set,std::chrono::steady_clock::now(),0,out,required)==r::error::too_large&&required==1);
    std::uint64_t count{};
    assert(a.guard_take("signal",guard,count)==r::error::success&&count==1);
    assert(a.guard_take("signal",guard,count)==r::error::again);
    assert(a.clock_create("time",1,r::clock_kind::ros,clock)==r::error::success);
    assert(a.clock_update("time",clock,100,true)==r::error::success);
    assert(a.timer_create("tick",1,"time",clock,timer)==r::error::success);
    assert(a.timer_reset("tick",timer,110,10)==r::error::success);
    assert(a.wait_set_add("wait",set,"tick",timer,r::expired|r::closed,7,4)==r::error::success);
    assert(a.wait_set_add("wait",set,"time",clock,r::changed,8,4)==r::error::success);
    auto ticking=std::async(std::launch::async,[&]{return a.wait("wait",set,std::chrono::steady_clock::now()+2s,4,out,required);});
    assert(a.clock_update("time",clock,135,true)==r::error::success);
    assert(ticking.get()==r::error::success&&out.size()==2);
    r::timer_sample tick{};
    assert(a.timer_take("tick",timer,tick)==r::error::success&&tick.expected==110&&tick.actual==135&&tick.occurrences==3);
    assert(a.timer_take("tick",timer,tick)==r::error::again);
    assert(a.clock_update("time",clock,50,true)==r::error::success);
    assert(a.clock_update("time",clock,200,true)==r::error::success);
    assert(a.timer_take("tick",timer,tick)==r::error::again); // backward jump cancelled schedule
    assert(a.wait_set_acknowledge("wait",set,clock,UINT64_MAX)==r::error::invalid);
    assert(a.timer_reset("tick",timer,INT64_MAX,1)==r::error::success);
    assert(a.clock_update("time",clock,INT64_MAX,true)==r::error::success);
    assert(a.timer_take("tick",timer,tick)==r::error::too_large);
    assert(a.timer_reset("tick",timer,INT64_MIN,1)==r::error::success);
    assert(a.timer_take("tick",timer,tick)==r::error::too_large);
    assert(a.close("time",clock)==r::error::success);
    assert(a.timer_take("tick",timer,tick)==r::error::bad_handle);
    assert(a.wait("wait",set,std::chrono::steady_clock::now(),4,out,required)==r::error::success);
    assert(out.size()==1&&out[0].source==timer&&out[0].events==r::closed);
    assert(a.wait_set_remove("wait",set,timer)==r::error::success);
    auto cancelled=std::async(std::launch::async,[&]{return a.wait("wait",set,std::chrono::steady_clock::now()+2s,4,out,required);});
    a.revoke("wait");auto retired_result=cancelled.get();
    assert(retired_result==r::error::cancelled||retired_result==r::error::bad_handle);
    // Recreate identities after retirement cannot resurrect the old wait set.
    r::handle replacement{};assert(a.wait_set_create("wait",1,replacement)==r::error::success&&replacement!=set);
    a.retire();assert(a.guard_create("signal",1,guard)==r::error::denied);
    assert(a.guard_trigger("signal",guard)==r::error::bad_handle);
    r::registry real;r::handle wall{},periodic{},ready{};
    assert(real.clock_create("steady",1,r::clock_kind::steady,wall)==r::error::success);
    assert(real.timer_create("timer",1,"steady",wall,periodic)==r::error::success);
    assert(real.wait_set_create("wait",1,ready)==r::error::success);
    assert(real.wait_set_add("wait",ready,"timer",periodic,r::expired,0,1)==r::error::success);
    assert(real.timer_reset("timer",periodic,r::steady_now()+1000000,0)==r::error::success);
    assert(real.wait("wait",ready,std::chrono::steady_clock::now()+2s,1,out,required)==r::error::success);
    assert(real.timer_take("timer",periodic,tick)==r::error::success&&tick.occurrences==1);
    assert(real.wait("wait",ready,std::chrono::steady_clock::now()+2s,1,out,required,[]{return true;})==r::error::cancelled);
}

void transactional_delivery()
{
    r::registry registry;r::handle guard{},clock{},timer{};
    assert(registry.guard_create("signal",1,guard)==r::error::success);
    assert(registry.guard_trigger("signal",guard)==r::error::success);
    assert(registry.guard_trigger("signal",guard)==r::error::success);
    unsigned calls{};
    assert(registry.guard_deliver("signal",guard,[&](auto count){++calls;assert(count==2);return false;})==r::error::invalid);
    bool caught{};
    try {registry.guard_deliver("signal",guard,[](auto)->bool{throw std::runtime_error("checked copy failed");});}
    catch(std::runtime_error const&) {caught=true;}
    assert(caught&&calls==1);
    std::uint64_t count{};assert(registry.guard_take("signal",guard,count)==r::error::success&&count==2);
    assert(registry.guard_take("signal",guard,count)==r::error::again&&count==2);
    assert(registry.clock_create("clock",1,r::clock_kind::ros,clock)==r::error::success);
    assert(registry.clock_update("clock",clock,135,true)==r::error::success);
    assert(registry.timer_create("timer",1,"clock",clock,timer)==r::error::success);
    assert(registry.timer_reset("timer",timer,110,10)==r::error::success);
    auto check=[](r::timer_sample const& value){assert(value.expected==110&&value.actual==135&&value.occurrences==3);};
    assert(registry.timer_deliver("timer",timer,[&](auto const& value){check(value);return false;})==r::error::invalid);
    caught=false;
    try {registry.timer_deliver("timer",timer,[&](auto const& value)->bool{check(value);throw std::runtime_error("copy");});}
    catch(std::runtime_error const&) {caught=true;}
    assert(caught);
    r::timer_sample sample{};assert(registry.timer_take("timer",timer,sample)==r::error::success);check(sample);
    assert(registry.timer_take("timer",timer,sample)==r::error::again);
    assert(registry.clock_update("clock",clock,140,true)==r::error::success);
    assert(registry.timer_take("timer",timer,sample)==r::error::success&&sample.expected==140&&sample.occurrences==1);
}

void observation_and_capacity()
{
    r::registry registry;r::handle clock{},first{},second{},guard{},timer{},replacement{};
    assert(registry.clock_create("clock",1,r::clock_kind::ros,clock)==r::error::success);
    assert(registry.wait_set_create("wait",2,first)==r::error::success);
    assert(registry.wait_set_create("wait",2,second)==r::error::success);
    assert(registry.wait_set_add("wait",first,"wait",second,r::closed,0,5)==r::error::invalid);
    assert(registry.wait_set_add("wait",first,"clock",clock,r::expired,0,5)==r::error::invalid);
    for(auto set:{first,second})assert(registry.wait_set_add("wait",set,"clock",clock,r::changed|r::closed,set,5)==r::error::success);
    assert(registry.clock_update("clock",clock,1,true)==r::error::success);
    std::vector<r::ready_record> result;std::size_t required{};
    assert(registry.wait("wait",first,std::chrono::steady_clock::now(),1,result,required)==r::error::success);
    auto sequence=result[0].sequence;
    assert(registry.wait_set_acknowledge("wait",first,clock,sequence)==r::error::success);
    assert(registry.wait_set_acknowledge("wait",first,clock,sequence-1)==r::error::invalid);
    assert(registry.wait("wait",first,std::chrono::steady_clock::now(),1,result,required)==r::error::timeout);
    assert(registry.wait("wait",second,std::chrono::steady_clock::now(),1,result,required)==r::error::success);
    assert(registry.guard_create("guard",1,guard)==r::error::success);
    assert(registry.guard_trigger("guard",guard)==r::error::success);
    assert(registry.wait_set_add("wait",second,"guard",guard,r::readable|r::closed,42,5)==r::error::success);
    auto original=result;
    assert(registry.wait("wait",second,std::chrono::steady_clock::now(),1,result,required)==r::error::too_large&&required==2);
    assert(result.size()==original.size()&&result[0].source==original[0].source&&result[0].events==original[0].events);
    assert(registry.wait("wait",second,std::chrono::steady_clock::now(),0,result,required)==r::error::too_large&&required==2);
    assert(registry.timer_create("timer",1,"clock",clock,timer)==r::error::success);
    assert(registry.close("clock",clock)==r::error::success);
    assert(registry.clock_create("clock",1,r::clock_kind::ros,replacement)==r::error::success&&replacement!=clock);
    assert(registry.timer_create("timer",1,"clock",replacement,guard)==r::error::again);
    assert(registry.timer_reset("timer",timer,0,0)==r::error::bad_handle);
    assert(registry.close("timer",timer)==r::error::success);
    assert(registry.timer_create("timer",1,"clock",replacement,timer)==r::error::success);
    assert(registry.wait("wait",second,std::chrono::steady_clock::now(),2,result,required)==r::error::success&&required==2);
    assert(result[0].source==clock&&result[0].events==r::closed);
    assert(registry.wait_set_remove("wait",second,clock)==r::error::success);
    assert(registry.wait_set_remove("wait",second,clock)==r::error::bad_handle);
    std::vector<r::snapshot> snapshot{{999,r::kind::guard,false,false,0,0,"unchanged"}};
    assert(!registry.inspect(snapshot,0)&&snapshot.size()==1&&snapshot[0].id==999);
    assert(registry.inspect(snapshot,10)&&snapshot.size()==5);
}

void unstarted_and_signed_time()
{
    r::registry registry;r::handle clock{},timer{},system{};
    assert(registry.clock_create("clock",1,r::clock_kind::ros,clock)==r::error::success);
    assert(registry.clock_create("system",1,r::clock_kind::system,system)==r::error::success);
    assert(registry.clock_update("system",system,0,true)==r::error::denied);
    assert(registry.timer_create("timer",1,"clock",clock,timer)==r::error::success);
    assert(registry.timer_reset("timer",timer,-100,0)==r::error::success);
    r::timer_sample sample{};r::clock_sample time{};
    assert(registry.clock_read("clock",clock,time)==r::error::success&&!time.started);
    assert(registry.timer_take("timer",timer,sample)==r::error::again);
    assert(registry.clock_update("clock",clock,0,true)==r::error::success);
    assert(registry.timer_take("timer",timer,sample)==r::error::again);
    assert(registry.timer_reset("timer",timer,-100,0)==r::error::success);
    assert(registry.timer_take("timer",timer,sample)==r::error::success&&sample.occurrences==1);
    assert(registry.timer_reset("timer",timer,0,-1)==r::error::invalid);
    assert(registry.clock_update("clock",clock,-1,true)==r::error::success);
    assert(registry.timer_reset("timer",timer,INT64_MIN,INT64_MAX)==r::error::success);
    assert(registry.timer_take("timer",timer,sample)==r::error::success&&sample.occurrences==2&&sample.expected==INT64_MIN);
    assert(registry.timer_take("timer",timer,sample)==r::error::again);
    r::nanoseconds unchanged=7;
    assert(!r::add(INT64_MAX,1,unchanged)&&unchanged==7);
    assert(!r::add(INT64_MIN,-1,unchanged)&&unchanged==7);
    assert(r::add(INT64_MIN,INT64_MAX,unchanged)&&unchanged==-1);
}

void callback_reentrancy_and_retirement()
{
    r::registry registry;r::handle guard{},set{};
    assert(registry.guard_create("signal",1,guard)==r::error::success);
    assert(registry.wait_set_create("wait",1,set)==r::error::success);
    assert(registry.wait_set_add("wait",set,"signal",guard,r::readable,1,1)==r::error::success);
    std::vector<r::ready_record> result;std::size_t required{};
    auto once=false;
    assert(registry.wait("wait",set,std::chrono::steady_clock::now()+2s,1,result,required,[&]
    {
        if(!once){once=true;assert(registry.guard_trigger("signal",guard)==r::error::success);}
        return false;
    })==r::error::success);
    std::uint64_t count{};assert(registry.guard_take("signal",guard,count)==r::error::success);
    assert(registry.wait("wait",set,std::chrono::steady_clock::now()+2s,1,result,required,[&]
    {registry.retire();return false;})==r::error::cancelled);
    assert(registry.wait_set_create("wait",1,set)==r::error::denied);
}

void concurrent_wait_and_consume()
{
    r::registry registry;r::handle guard{},set{};
    assert(registry.guard_create("signal",1,guard)==r::error::success);
    assert(registry.wait_set_create("wait",1,set)==r::error::success);
    assert(registry.wait_set_add("wait",set,"signal",guard,r::readable|r::closed,1,1)==r::error::success);
    std::latch entered{2};
    auto waiter=[&]
    {
        bool announced{};std::vector<r::ready_record> out;std::size_t required{};
        auto result=registry.wait("wait",set,std::chrono::steady_clock::now()+5s,1,out,required,[&]
        {if(!announced){announced=true;entered.count_down();}return false;});
        assert(result==r::error::success&&out.size()==1&&out[0].events==r::readable);
    };
    auto first=std::async(std::launch::async,waiter),second=std::async(std::launch::async,waiter);
    entered.wait();assert(registry.guard_trigger("signal",guard)==r::error::success);first.get();second.get();
    std::uint64_t count{};assert(registry.guard_take("signal",guard,count)==r::error::success&&count==1);
    constexpr unsigned triggers=10000;
    auto producer=std::async(std::launch::async,[&]{for(unsigned i{};i<triggers;++i)assert(registry.guard_trigger("signal",guard)==r::error::success);});
    std::uint64_t delivered{};auto deadline=std::chrono::steady_clock::now()+10s;
    while(delivered<triggers)
    {
        std::vector<r::ready_record> out;std::size_t required{};
        assert(registry.wait("wait",set,deadline,1,out,required)==r::error::success);
        assert(registry.guard_take("signal",guard,count)==r::error::success);delivered+=count;
    }
    producer.get();assert(delivered==triggers);
    std::promise<void> registered;auto notification=registered.get_future();
    auto closing=std::async(std::launch::async,[&]
    {
        bool announced{};std::vector<r::ready_record> out;std::size_t required{};
        return registry.wait("wait",set,std::chrono::steady_clock::now()+5s,1,out,required,[&]
        {if(!announced){announced=true;registered.set_value();}return false;});
    });
    notification.get();assert(registry.close("wait",set)==r::error::success);assert(closing.get()==r::error::cancelled);
}

int main()
{
    initial_contract();transactional_delivery();observation_and_capacity();
    unstarted_and_signed_time();callback_reentrancy_and_retirement();concurrent_wait_and_consume();
    std::cout<<"PASS: ownership, transactional copies, bounded waits, independent acknowledgement, parent retirement, signed time, reentrant cancellation, concurrent notification and consumption\n";
}
