/** @file @brief Actual bounded sealed pins, recovery, unknown outcomes and stop reservations. */
#include <uwvm2/imported/ros/scene/device_core.h>
#include <cassert>
#include <iostream>
#include <barrier>
#include <thread>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;namespace d=s::devices;namespace b=s::buffers;using d::error;
static constexpr d::owner owner{1,2,3};static constexpr std::array<std::uint8_t,4> payload{1,2,3,4};
#if defined(UWVM_DEVICE_FAULT_INJECTION)
static thread_local bool allocation_forbidden{},allocation_attempted{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){if(allocation_forbidden){allocation_attempted=true;return nullptr;}return __real_malloc(n);}
#endif
static b::profile profile(std::string_view text){b::profile out;assert(out.identity.assign(text));out.sha256.fill(7);return out;}
static d::plan policy()
{
    d::plan out;out.profile=profile("device.scalar");out.stop_profile=profile("device.stop");out.schema.profile=profile("command");
    assert(out.schema.frame.assign("base")&&out.schema.fields[0].name.assign("joint"));out.schema.count=1;out.schema.fields[0].quantity=d::unit::radian;
    out.schema.fields[0].minimum=-1;out.schema.fields[0].maximum=1;
    out.handles=2;out.requests=2;out.body_bytes=256;out.observations=1;out.minimum_observations=1;out.maximum_age_ns=100;out.maximum_deadline_ns=100;
    assert(d::core::required_storage(out,out.storage_bytes));return out;
}
static b::plan buffer_policy()
{b::plan out;out.schema=profile("sample");out.objects=4;out.bytes=16;out.object_bytes=4;out.transfer_bytes=4;out.workspace_bytes=4;out.leases=32;out.elements=4;out.element_mask=1;out.order_mask=1;out.color_mask=1;out.rank=1;return out;}
struct fixture
{
    d::core core;b::registry buffers;d::plan plan=policy();std::array<std::uint8_t,256> command_bytes{};std::size_t command_size{};d::handle device{},buffer{};
    fixture()
    {
        assert(core.configure(owner,4,plan)==error::success&&buffers.bind("sample",buffer_policy())==error::success);
        b::layout layout;layout.schema=profile("sample");layout.bytes=4;layout.rank=1;layout.shape[0]=4;layout.strides[0]=1;
        assert(buffers.create("sample",layout,4,[&](auto id){buffer=id;return true;})==error::success);std::uint64_t accepted{};
        assert(buffers.write("sample",buffer,0,payload,accepted)==error::success&&buffers.seal("sample",buffer,b::hash(payload))==error::success);
        assert(core.open(owner,plan.profile,2,[&](auto id){device=id;return true;})==error::success);
        d::scalar_command value;value.count=1;value.values[0]=0.5;s::wire::writer writer{command_bytes};assert(d::encode(writer,plan.schema,value));command_size=writer.size();
    }
    d::observations evidence()
    {
        d::observations out;out.count=1;out.values[0]={5,6,7,8,buffer,1000,b::hash(payload),plan.schema.frame};
        assert(buffers.pin("sample",buffer,out.pins[0])==error::success);return out;
    }
    d::submission submission(d::handle key=1)
    {return {device,key,1050,{9,10,4,{1,11,3,12},1,1100},plan.schema.profile,{command_bytes.data(),command_size}};}
    d::receipt submit(d::handle key=1)
    {auto pins=evidence();d::receipt out;assert(core.submit(owner,submission(key),pins,1000,out)==error::success&&pins.count==0);return out;}
    d::usage usage(){d::usage out;assert(core.inspect(owner,out)==error::success);return out;}
};
static void configuration_exact_keys_and_failed_delivery()
{
    d::core core;d::receipt receipt;assert(core.result(owner,1,2,receipt)==error::denied);auto plan=policy();auto bad=plan;bad.requests=17;std::uint64_t size{};
    assert(!d::core::required_storage(bad,size));bad=plan;bad.minimum_observations=0;assert(!d::core::required_storage(bad,size));
    bad=plan;bad.storage_bytes--;assert(core.configure(owner,4,bad)==error::too_large&&core.configure(owner,4,plan)==error::success);
    assert(core.open(owner,plan.profile,0,[](auto){assert(false);return true;})==error::too_large);
    assert(core.open(owner,plan.profile,1,[](auto){return false;})==error::io);d::usage used;assert(core.inspect(owner,used)==error::success&&!used.retained_handles);
    assert(core.open(owner,plan.profile,1,[&](auto){assert(core.retire()==error::success);return true;})==error::cancelled);
    fixture f;auto pins=f.evidence();auto original=pins.values;auto value=f.submission();value.key=2;
    assert(f.core.submit(owner,value,pins,1000,receipt)==error::conflict&&pins.count==1);
    value.key=1;assert(f.core.submit(owner,value,pins,1000,receipt)==error::success);auto first=receipt;
    d::observations replay;replay.count=1;replay.values=original;
    assert(f.core.submit(owner,value,replay,5000,receipt)==error::success&&receipt==first);
    value.deadline++;assert(f.core.submit(owner,value,replay,1000,receipt)==error::conflict);value.deadline--;
    f.submit(2);assert(f.core.submit(owner,value,replay,1000,receipt)==error::conflict);
    auto third=f.evidence();assert(f.core.submit(owner,f.submission(3),third,1000,receipt)==error::again&&third.count==1);
}
static void retained_pins_claim_boundary_and_recovery()
{
    fixture f;auto request=f.submit();assert(f.buffers.close("sample",f.buffer)==error::success);
    b::usage used;assert(f.buffers.inspect("sample",used)==error::success&&used.objects==1&&used.leases==1);
    d::work work;assert(f.core.claim(1001,work)==error::success&&work.result.request==request.request&&work.observation_count==1&&work.command.values[0]==0.5);
    d::work other;assert(f.core.claim(1001,other)==error::again&&f.core.release(owner,request.request)==error::again);
    assert(f.core.finish(work.ticket,d::completion::executed)==error::conflict);
    assert(f.core.cross_effect(work.ticket,1001)==error::success&&f.core.cross_effect(work.ticket,1001)==error::conflict);
    assert(f.core.finish(work.ticket,d::completion::executed)==error::success&&f.core.finish(work.ticket,d::completion::executed)==error::bad_handle);
    d::receipt result;assert(f.core.result(owner,f.device,request.request,result)==error::success&&result.status==d::state::succeeded&&result.outcome==d::effect::committed);
    assert(f.core.release(owner,request.request)==error::success&&f.buffers.inspect("sample",used)==error::success&&!used.objects&&!used.leases);
    assert(f.core.result(owner,f.device,request.request,result)==error::bad_handle); // Replay tombstone is not an ordinary live result.
    assert(f.core.close(owner,f.device)==error::success&&!f.usage().retained_handles&&f.core.result(owner,f.device,request.request,result)==error::bad_handle);
}
static void freshness_digest_and_cancellation()
{
    fixture f;d::receipt out;auto value=f.submission();auto pins=f.evidence();pins.values[0].digest[0]^=1;
    assert(f.core.submit(owner,value,pins,1000,out)==error::denied);pins.values[0].digest[0]^=1;
    pins.values[0].captured_ns=1001;assert(f.core.submit(owner,value,pins,1000,out)==error::denied);pins.values[0].captured_ns=899;
    assert(f.core.submit(owner,value,pins,1000,out)==error::denied);pins.values[0].captured_ns=1000;
    value.controller.provider_generation++;assert(f.core.submit(owner,value,pins,1000,out)==error::denied);value.controller.provider_generation--;
    value.deadline=1000;assert(f.core.submit(owner,value,pins,1000,out)==error::timeout);value.deadline=1050;
    assert(f.core.submit(owner,value,pins,1000,out)==error::success);d::work work;assert(f.core.claim(1001,work)==error::success);
    assert(f.core.close(owner,f.device)==error::success&&f.usage().retained_handles==1);
    assert(f.core.cross_effect(work.ticket,1001)==error::cancelled&&f.core.finish(work.ticket,d::completion::no_effect)==error::success);
    assert(f.core.release(owner,out.request)==error::success&&!f.usage().retained_handles);
    fixture revoked;auto request=revoked.submit();revoked.buffers.revoke("sample");
    assert(revoked.core.claim(1001,work)==error::timeout&&revoked.core.result(owner,revoked.device,request.request,out)==error::success&&out.why==d::reason::source_unavailable);
    assert(revoked.core.release(owner,request.request)==error::success);
}
static void unknown_stop_debt_reconciliation_and_rearm()
{
    fixture f;auto first=f.submit();auto second=f.submit(2);d::work work;assert(f.core.claim(1001,work)==error::success&&f.core.cross_effect(work.ticket,1001)==error::success);
    assert(f.core.finish(work.ticket,d::completion::unknown)==error::success);auto used=f.usage();assert(used.unknown==1&&used.requests==2&&used.pins==2&&used.latched&&used.stop_pending);
    d::receipt stop;assert(f.core.stop(owner,f.device,1,f.plan.stop_profile,stop)==error::success&&stop.status==d::state::queued);
    d::receipt replay;assert(f.core.stop(owner,f.device,1,f.plan.stop_profile,replay)==error::success&&replay==stop);
    assert(f.core.release(owner,first.request)==error::again&&f.core.release(owner,second.request)==error::success);
    assert(f.core.acknowledge_stop(3,stop.request)==error::conflict&&f.core.acknowledge_stop(4,stop.request)==error::success);
    assert(f.core.rearm(4,stop.request,99)==error::denied&&f.core.observe_stopped(4,stop.request,99)==error::success);
    assert(f.core.rearm(4,stop.request,99)==error::again&&f.core.reconcile(3,first.request,100,d::completion::no_effect)==error::conflict);
    assert(f.core.reconcile(4,first.request,100,d::completion::no_effect)==error::success&&f.core.reconcile(4,first.request,101,d::completion::executed)==error::conflict);
    assert(f.core.rearm(4,stop.request,99)==error::success&&!f.usage().latched&&f.core.release(owner,first.request)==error::success);
    auto next=f.submit(3);assert(next.request!=first.request);d::receipt next_stop;assert(f.core.stop(owner,f.device,2,f.plan.stop_profile,next_stop)==error::success&&next_stop.request!=stop.request);
    assert(f.core.stop(owner,f.device,1,f.plan.stop_profile,replay)==error::conflict);
    assert(f.core.release(owner,next.request)==error::success&&f.core.close(owner,f.device)==error::success&&f.usage().retained_handles==1);
    assert(f.core.open(owner,f.plan.profile,1,[](auto){assert(false);return true;})==error::too_large);
    assert(f.core.release(owner,next_stop.request)==error::again&&f.core.acknowledge_stop(4,next_stop.request)==error::success);
    assert(f.core.release(owner,next_stop.request)==error::success&&!f.usage().retained_handles);
    assert(f.core.result(owner,f.device,next_stop.request,replay)==error::bad_handle&&f.usage().latched); // Release is not rearm.
}
static void provider_change_retire_and_allocation_free_hot_path()
{
    fixture f;auto pins=f.evidence();d::receipt request;
#if defined(UWVM_DEVICE_FAULT_INJECTION)
    allocation_forbidden=true;
#endif
    assert(f.core.submit(owner,f.submission(),pins,1000,request)==error::success);d::work work;
    assert(f.core.claim(1001,work)==error::success&&f.core.cross_effect(work.ticket,1001)==error::success);
    assert(f.core.provider_changed(5)==error::success&&f.core.provider_changed(4)==error::conflict);
    assert(f.core.finish(work.ticket,d::completion::executed)==error::conflict&&f.core.finish(work.ticket,d::completion::no_effect)==error::conflict);
    assert(f.core.finish(work.ticket,d::completion::unknown)==error::success);
    d::receipt result;assert(f.core.result(owner,f.device,request.request,result)==error::success&&result.status==d::state::outcome_unknown);
    assert(f.core.retire()==error::success&&f.core.reconcile(5,request.request,42,d::completion::executed)==error::success);
    assert(f.core.result(owner,f.device,request.request,result)==error::success&&result.status==d::state::succeeded);
    assert(f.core.release(owner,request.request)==error::success);
#if defined(UWVM_DEVICE_FAULT_INJECTION)
    allocation_forbidden=false;assert(!allocation_attempted);
#endif
}
static void concurrent_claim_and_fork()
{
    fixture f;f.submit();f.submit(2);std::barrier gate{3};std::array<error,2> results{};std::array<d::work,2> values;
    std::thread a{[&]{gate.arrive_and_wait();results[0]=f.core.claim(1001,values[0]);}};
    std::thread b{[&]{gate.arrive_and_wait();results[1]=f.core.claim(1001,values[1]);}};gate.arrive_and_wait();a.join();b.join();
    assert((results[0]==error::success&&results[1]==error::again)||(results[1]==error::success&&results[0]==error::again));
    auto ticket=results[0]==error::success?values[0].ticket:values[1].ticket;
    assert(f.core.retire()==error::success&&f.core.cross_effect(ticket,1001)==error::cancelled&&f.core.finish(ticket,d::completion::no_effect)==error::success);
    auto child=::fork();assert(child>=0);if(!child){d::usage used;::_exit(f.core.inspect(owner,used)==error::denied?0:1);}int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
}
int main()
{
    configuration_exact_keys_and_failed_delivery();retained_pins_claim_boundary_and_recovery();freshness_digest_and_cancellation();
    unknown_stop_debt_reconciliation_and_rearm();provider_change_retire_and_allocation_free_hot_path();concurrent_claim_and_fork();
    std::cout<<"passed 6 device core safety groups\n";
    return 0;
}
