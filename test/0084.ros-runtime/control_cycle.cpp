/** @file @brief Actual controller lease + sealed phase pins + device ledger, without native effect claims. */
#define main prior_device_core_main
#include "device_core.cpp"
#undef main
#include <uwvm2/imported/ros/scene/control_cycle.h>
namespace ctl=s::control;
static std::atomic<std::uint64_t> cycle_now{1000};
static std::uint64_t cycle_clock()noexcept{return cycle_now.load();}
static constexpr ctl::principal controller_owner{1,11,3,12};
struct cycle_fixture:fixture
{
    ctl::arbitration arbitration;ctl::cycle cycle{arbitration,core,cycle_clock};ctl::lease lease;ctl::cycle::sample_record frame;
    cycle_fixture()
    {
        cycle_now=1000;assert(arbitration.configure(1,3,4,{1000,100,10,true})==error::success);ctl::prepared prepared;
        assert(arbitration.prepare({controller_owner,1,100},1000,prepared)==error::success&&arbitration.commit(prepared,1000)==error::success);lease=prepared.candidate;
        assert(cycle.configure(controller_owner,owner,device,lease,{50},1000)==error::success);
    }
    void sample()
    {auto pins=evidence();assert(cycle.sample(controller_owner,cycle_now,pins,[&](auto const& value){frame=value;return true;})==error::success&&!pins.count);}
    d::receipt submit(d::handle key=1)
    {d::receipt output;assert(cycle.submit(controller_owner,key,frame.sequence,plan.schema.profile,{command_bytes.data(),command_size},cycle_now,[](auto const&,auto){return error::success;},output)==error::success);return output;}
    ctl::cycle::status inspect(){ctl::cycle::status value;assert(cycle.inspect(value)==error::success);return value;}
    void finish(d::completion completion)
    {
        d::work work;assert(core.claim(cycle_now,work)==error::success);d::handle effect{};
        assert(arbitration.begin_effect(controller_owner,lease,cycle_now,effect)==error::success&&core.cross_effect(work.ticket,cycle_now)==error::success);
        assert(core.finish(work.ticket,completion)==error::success&&arbitration.finish_effect(effect,completion==d::completion::unknown?ctl::effect_result::unknown:ctl::effect_result::executed)==error::success);
    }
};
static void actual_configuration_and_failed_sample()
{
    fixture raw;ctl::arbitration arb;ctl::cycle absent{arb,raw.core,cycle_clock};ctl::lease forged{};
    assert(absent.configure(controller_owner,owner,raw.device,forged,{50},1000)==error::denied);
    cycle_fixture f;auto pins=f.evidence();
    assert(f.cycle.sample({1,11,3,13},1000,pins,[](auto const&){assert(false);return true;})==error::denied&&pins.count==1);
    assert(f.cycle.sample(controller_owner,1000,pins,[](auto const&){return false;})==error::io&&!pins.count&&!f.inspect().sequence);
    b::usage usage;assert(f.buffers.inspect("sample",usage)==error::success&&!usage.leases);
    f.sample();auto first=f.frame;auto unused=f.evidence();
    assert(f.cycle.sample(controller_owner,1000,unused,[&](auto const& value){assert(value.sequence==first.sequence&&value.deadline==first.deadline&&value.observations==first.observations);return true;})==error::success&&unused.count==1);
    assert(f.inspect().sequence==1&&f.inspect().current==ctl::cycle::phase::sampled);
}
static void real_submit_retry_result_and_next_cycle()
{
    cycle_fixture f;f.sample();d::receipt receipt;unsigned checks{};
    auto validate=[&](auto const& actual,auto){++checks;return actual.count==1&&actual.values[0].buffer==f.buffer?error::success:error::denied;};
    auto command=s::wire::bytes{f.command_bytes.data(),f.command_size};
    assert(f.cycle.submit(controller_owner,1,2,f.plan.schema.profile,command,1000,validate,receipt)==error::conflict&&!checks);
    assert(f.cycle.submit(controller_owner,1,1,f.plan.schema.profile,command,1000,validate,receipt)==error::success&&checks==1&&f.usage().requests==1);
    auto first=receipt;assert(f.cycle.submit(controller_owner,1,1,f.plan.schema.profile,command,9999,validate,receipt)==error::success&&receipt==first&&checks==1);
    auto changed=f.command_bytes;changed[0]^=1;
    assert(f.cycle.submit(controller_owner,1,1,f.plan.schema.profile,{changed.data(),f.command_size},1000,validate,receipt)==error::conflict);
    f.finish(d::completion::executed);assert(f.cycle.result(controller_owner,1,receipt)==error::success&&receipt.status==d::state::succeeded);
    cycle_now=1002;f.sample();assert(f.frame.sequence==2&&!f.usage().requests);
    assert(f.cycle.result(controller_owner,1,receipt)==error::success&&receipt.status==d::state::succeeded);auto second=f.submit(2);
    assert(second.request!=first.request&&f.cycle.result(controller_owner,1,receipt)==error::bad_handle);
}
static void callback_deadlines_are_resampled()
{
    cycle_fixture f;auto pins=f.evidence();
    assert(f.cycle.sample(controller_owner,1000,pins,[](auto const&){cycle_now=1050;return true;})==error::timeout&&f.inspect().current==ctl::cycle::phase::latched&&f.inspect().missed==1);
    assert(f.usage().stop_pending&&!f.usage().requests);ctl::status arb;assert(f.arbitration.inspect(arb)==error::success&&arb.stop&&arb.latched);
    cycle_fixture late;late.sample();d::receipt output;
    assert(late.cycle.submit(controller_owner,1,1,late.plan.schema.profile,{late.command_bytes.data(),late.command_size},1000,
        [](auto const&,auto){cycle_now=1050;return error::success;},output)==error::timeout&&!late.usage().requests&&late.usage().stop_pending);
    cycle_fixture backwards;backwards.sample();assert(backwards.cycle.submit(controller_owner,1,1,backwards.plan.schema.profile,{backwards.command_bytes.data(),backwards.command_size},1000,
        [](auto const&,auto){cycle_now=999;return error::success;},output)==error::timeout&&!backwards.usage().requests);
}
static void source_refusal_reentry_and_revocation()
{
    cycle_fixture f;f.sample();d::receipt output;
    assert(f.cycle.submit(controller_owner,1,1,f.plan.schema.profile,{f.command_bytes.data(),f.command_size},1000,
        [](auto const&,auto){return error::denied;},output)==error::denied&&!f.usage().requests);
    assert(f.submit().request&&f.usage().requests==1);
    cycle_fixture retired;retired.sample();assert(retired.cycle.submit(controller_owner,1,1,retired.plan.schema.profile,{retired.command_bytes.data(),retired.command_size},1000,
        [&](auto const&,auto){assert(retired.cycle.retire()==error::success);return error::success;},output)==error::cancelled&&!retired.usage().requests&&retired.usage().stop_pending);
    cycle_fixture source_gone;source_gone.sample();assert(source_gone.cycle.submit(controller_owner,1,1,source_gone.plan.schema.profile,{source_gone.command_bytes.data(),source_gone.command_size},1000,
        [&](auto const&,auto){source_gone.buffers.revoke("sample");return error::success;},output)!=error::success&&!source_gone.usage().requests);
    cycle_fixture preempted;preempted.sample();ctl::prepared replacement;
    assert(preempted.arbitration.prepare({{1,13,3,14},2,100},1000,replacement)==error::success&&preempted.arbitration.commit(replacement,1000)==error::success);
    assert(preempted.cycle.submit(controller_owner,1,1,preempted.plan.schema.profile,{preempted.command_bytes.data(),preempted.command_size},1000,
        [](auto const&,auto){return error::success;},output)==error::conflict&&!preempted.usage().requests);
}
static void unknown_outcome_keeps_debt_and_independent_stop()
{
    cycle_fixture f;f.sample();auto submitted=f.submit();f.finish(d::completion::unknown);d::receipt result;
    assert(f.cycle.result(controller_owner,1,result)==error::success&&result.status==d::state::outcome_unknown&&f.inspect().current==ctl::cycle::phase::latched);
    assert(f.usage().pins==1&&f.usage().unknown==1&&f.core.release(owner,submitted.request)==error::again);
    assert(f.cycle.stop(controller_owner,result)==error::success&&result.request!=submitted.request&&f.usage().stop_pending);
    auto extra=f.evidence();assert(f.cycle.sample(controller_owner,1000,extra,[](auto const&){assert(false);return true;})==error::cancelled);
    cycle_fixture full;full.fixture::submit(1);full.fixture::submit(2);assert(full.usage().requests==2);
    assert(full.cycle.stop(controller_owner,result)==error::success&&full.usage().stop_pending&&result.request);
}
static void concurrent_retire_never_commits_a_sample()
{
    cycle_fixture f;auto pins=f.evidence();std::barrier gate{2};error result{};
    std::thread thread{[&]{result=f.cycle.sample(controller_owner,1000,pins,[&](auto const&){gate.arrive_and_wait();gate.arrive_and_wait();return true;});}};
    gate.arrive_and_wait();assert(f.cycle.retire()==error::success);gate.arrive_and_wait();thread.join();
    assert(result==error::cancelled&&!f.inspect().sequence&&!f.usage().requests&&f.usage().stop_pending);b::usage retained;
    assert(f.buffers.inspect("sample",retained)==error::success&&!retained.leases);
}
static void fixed_storage_hot_path_and_fork()
{
    cycle_fixture f;auto pins=f.evidence();d::receipt result;
#if defined(UWVM_DEVICE_FAULT_INJECTION)
    allocation_forbidden=true;allocation_attempted=false;
#endif
    assert(f.cycle.sample(controller_owner,1000,pins,[&](auto const& value){f.frame=value;return true;})==error::success);
    assert(f.cycle.submit(controller_owner,1,1,f.plan.schema.profile,{f.command_bytes.data(),f.command_size},1000,
        [](auto const&,auto){return error::success;},result)==error::success);
    assert(f.cycle.result(controller_owner,1,result)==error::success&&f.cycle.stop(controller_owner,result)==error::success);
#if defined(UWVM_DEVICE_FAULT_INJECTION)
    allocation_forbidden=false;assert(!allocation_attempted);
#endif
    auto pid=::fork();assert(pid>=0);if(!pid){::alarm(3);ctl::cycle::status state;::_exit(f.cycle.inspect(state)==error::denied&&f.cycle.stop(controller_owner,result)==error::denied?0:4);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
int main()
{
    prior_device_core_main();actual_configuration_and_failed_sample();real_submit_retry_result_and_next_cycle();callback_deadlines_are_resampled();
    source_refusal_reentry_and_revocation();unknown_outcome_keeps_debt_and_independent_stop();concurrent_retire_never_commits_a_sample();fixed_storage_hot_path_and_fork();
    std::cout<<"passed 7 control cycle safety groups\n";return 0;
}
