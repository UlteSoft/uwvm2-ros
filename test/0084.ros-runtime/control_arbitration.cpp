/** @file @brief Controller leases, execution fences, unknown debt and independent stop slots. */
#include <uwvm2/imported/ros/scene/control_arbitration.h>
#include <cassert>
#include <atomic>
#include <barrier>
#include <thread>
#include <iostream>
#include <sys/wait.h>
namespace c=uwvm2::imported::ros::scene::control;using c::error;
static constexpr c::principal first{1,2,3,4},second{1,5,3,6};
static void configure(c::arbitration& value){assert(value.configure(1,3,7,{1000,100,10,true})==error::success);}
static c::lease claim(c::arbitration& value,c::principal owner=first,std::uint64_t priority=1,std::uint64_t now=1000)
{c::prepared out;assert(value.prepare({owner,priority,500},now,out)==error::success&&value.commit(out,now)==error::success);return out.candidate;}
static c::status inspect(c::arbitration& value){c::status out;assert(value.inspect(out)==error::success);return out;}
static void prepared_copy_fence_and_priority()
{
    c::arbitration value;assert(value.configure(1,3,7,{})==error::invalid);configure(value);auto old=claim(value);
    c::prepared next;assert(value.prepare({first,1,500},1001,next)==error::conflict);
    assert(value.prepare({second,1,500},1001,next)==error::denied);
    assert(value.prepare({second,2,500},1001,next)==error::success);
    assert(inspect(value).controller==old&&value.discard(next.ticket)==error::success&&inspect(value).controller==old);
    assert(value.prepare({second,2,500},1001,next)==error::success);
    auto changed=next;changed.candidate.priority=9;assert(value.commit(changed,1001)==error::bad_handle);
    assert(value.commit(next,1001)==error::success);c::handle effect{};
    assert(value.begin_effect(first,old,1002,effect)==error::conflict&&!effect);
    assert(value.begin_effect(second,next.candidate,1002,effect)==error::success&&effect);
    c::prepared blocked;assert(value.prepare({first,3,500},1003,blocked)==error::again);
    assert(value.finish_effect(effect,c::effect_result::executed)==error::success);
}
static void deadline_reopen_and_competing_effect()
{
    c::arbitration value;configure(value);c::prepared next;
    assert(value.prepare({first,1,500},1000,next)==error::success&&value.commit(next,1100)==error::timeout&&!inspect(value).controller.id);
    auto held=claim(value);c::handle effect{};
    assert(value.begin_effect(first,held,1500,effect)==error::timeout&&!effect);
    assert(value.prepare({second,2,500},1500,next)==error::timeout);
    c::arbitration expiring;configure(expiring);c::prepared brief;
    assert(expiring.prepare({first,1,20},1000,brief)==error::success&&expiring.commit(brief,1000)==error::success);
    assert(expiring.prepare({second,2,500},1001,next)==error::success&&expiring.commit(next,1020)==error::timeout);
    assert(inspect(expiring).controller==brief.candidate); // Staging cannot bridge expired ownership.
    c::arbitration raced;configure(raced);held=claim(raced);std::barrier rendezvous{3};std::atomic<unsigned> success{};c::handle winner{};
    auto begin=[&]{rendezvous.arrive_and_wait();c::handle id{};auto result=raced.begin_effect(first,held,1001,id);if(result==error::success){winner=id;++success;}else assert(result==error::again);};
    std::thread a{begin},b{begin};rendezvous.arrive_and_wait();a.join();b.join();assert(success==1&&winner);
    assert(raced.finish_effect(winner,c::effect_result::no_effect)==error::success);
}
static void unknown_never_becomes_safe_failure()
{
    c::arbitration value;configure(value);auto held=claim(value);c::handle effect{},stop{};
    assert(value.begin_effect(first,held,1001,effect)==error::success&&value.finish_effect(effect,c::effect_result::unknown)==error::success);
    assert(inspect(value).outcome_unknown&&inspect(value).latched&&inspect(value).effect==effect);
    assert(value.finish_effect(effect,c::effect_result::no_effect)==error::conflict);
    c::handle denied{};assert(value.begin_effect(first,held,1002,denied)==error::cancelled);
    assert(value.stop(stop)==error::success&&stop);c::handle duplicate{};assert(value.stop(duplicate)==error::success&&duplicate==stop);
    assert(value.acknowledge_stop(7,stop)==error::success&&value.rearm(7,stop,9)==error::denied);
    assert(value.observe_stopped(7,stop,9)==error::success&&value.rearm(7,stop,9)==error::again);
    assert(value.reconcile_effect(7,effect,1)==error::success&&inspect(value).latched);
    assert(value.rearm(7,stop,9)==error::success&&!inspect(value).controller.id&&!inspect(value).latched);
    held=claim(value,first,1,1100);assert(value.begin_effect(first,held,1101,effect)==error::success&&value.finish_effect(effect,c::effect_result::unknown)==error::success);
    assert(value.reconcile_effect(7,effect,1)==error::conflict&&value.reconcile_effect(7,effect,2)==error::success);
}
static void provider_restart_and_retired_stop()
{
    c::arbitration value;configure(value);auto held=claim(value);c::handle effect{},stop{};
    assert(value.begin_effect(first,held,1001,effect)==error::success&&value.stop(stop)==error::success);
    assert(value.provider_changed(8)==error::success&&inspect(value).outcome_unknown&&inspect(value).effect==effect);
    assert(value.acknowledge_stop(7,stop)==error::conflict&&value.observe_stopped(7,stop,20)==error::conflict);
    assert(value.acknowledge_stop(8,stop)==error::success&&value.observe_stopped(8,stop,20)==error::success);
    assert(value.reconcile_effect(7,effect,1)==error::conflict&&value.reconcile_effect(8,effect,1)==error::success&&value.rearm(8,stop,20)==error::success);
    assert(value.begin_effect(first,held,1002,effect)==error::conflict);
    assert(value.retire()==error::success&&value.stop(stop)==error::success&&value.acknowledge_stop(8,stop)==error::success&&value.observe_stopped(8,stop,21)==error::success);
    assert(value.rearm(8,stop,21)==error::denied);
}
static void fork_before_mutex()
{
    c::arbitration value;configure(value);auto held=claim(value);auto child=::fork();assert(child>=0);
    if(!child){::alarm(3);c::status status;c::handle effect{};::_exit(value.inspect(status)==error::denied&&value.begin_effect(first,held,1001,effect)==error::denied&&value.retire()==error::denied?0:3);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
int main()
{prepared_copy_fence_and_priority();deadline_reopen_and_competing_effect();unknown_never_becomes_safe_failure();provider_restart_and_retired_stop();fork_before_mutex();std::cout<<"passed 5 control arbitration safety groups\n";return 0;}
