/** @file
 * @brief Test bounded inert model admission without simulating physical execution.
 * @details Exact authority, effect classification, stale observations, independent
 * epochs, zero rates, lost receipts, callback reentry/revocation, concurrency and
 * fork isolation are exercised against the actual isolated admission core.
 */
#include <uwvm2/imported/ros/scene/proposal_admission.h>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <new>
#include <stdexcept>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>

namespace p=uwvm2::imported::ros::scene::proposal_admission;
namespace c=uwvm2::imported::ros::proposal_contract;
using p::result;
using core=p::registry<4,64,2,4,4,4>;
static std::atomic<std::uint64_t> current_time{1000};
static std::uint64_t now() noexcept{return current_time.load();}
static std::atomic<bool> forbid_allocation{};
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define UWVM_PROPOSAL_THREAD_SANITIZED
#endif
#endif
#if defined(UWVM_PROPOSAL_THREAD_SANITIZED)
inline constexpr bool allocation_trap=false;
#else
inline constexpr bool allocation_trap=true;
void* operator new(std::size_t size)
{
    if(forbid_allocation.load())throw std::bad_alloc{};
    if(auto value=std::malloc(size?size:1))return value;
    throw std::bad_alloc{};
}
void* operator new[](std::size_t size){return ::operator new(size);}
[[gnu::noinline]] static void release_allocation(void* value) noexcept{std::free(value);}
void operator delete(void* value) noexcept{release_allocation(value);}
void operator delete[](void* value) noexcept{release_allocation(value);}
void operator delete(void* value,std::size_t) noexcept{release_allocation(value);}
void operator delete[](void* value,std::size_t) noexcept{release_allocation(value);}
#endif
static constexpr std::array<std::uint8_t,4> bytes{1,2,3,4};
static constexpr p::limits bounds{4,64,2,4,4,4,1000,500};
static p::tool physical(c::target_action action=c::target_action::device_submit,
                        p::observation_policy policy=p::observation_policy::latest_required)
{
    p::tool value;
    value.profile=11;value.schema=12;value.target_capability=13;value.target=14;value.provider_generation=15;
    value.frame=16;value.operation=action;value.max_observation_age_ns=200;value.effect=p::effect_class::physical;
    value.target_authority_epoch=17;value.physical_scope=18;value.controller_lease_epoch=19;value.minimum_observations=1;value.observations=policy;
    return value;
}
static p::grant arbiter()
{
    p::grant value;
    value.actor=200;value.capability=201;value.proposal_capability=101;value.target_capability=13;value.authority_epoch=202;
    value.target_actions=c::all_actions;value.read=value.accept=true;return value;
}
static p::observation observed()
{return {21,22,23,24,25,26,16,27,1000,10,30,31,32};}
struct fixture
{
    core registry{now};p::identity tool{},reader{},source{};
    explicit fixture(p::grant owner=arbiter(),p::tool selected=physical())
    {
        current_time=1000;
        assert(registry.configure(bounds,100,101)==result::success);
        assert(registry.bind_tool(selected,tool)==result::success);
        assert(registry.bind_reader(owner,reader)==result::success);
        assert(registry.bind_source(observed(),source)==result::success);
    }
    p::identity publish(std::uint64_t expiry=1500)
    {
        p::identity id{};std::array sources{source};
        assert(registry.publish(100,301,tool,bytes,sources,expiry,id)==result::success);return id;
    }
    p::accept_request request(p::identity id,std::uint64_t sequence=1)
    {return {{reader,sequence},101,id,13,14,1400};}
    result accept(p::accept_request const& request,p::receipt& receipt)
    {return registry.accept(200,request,[](auto const&){return result::success;},receipt);}
};

static void zero_authority_and_closed_actions()
{
    core registry{now};p::identity id=77;p::receipt receipt{77};
    assert(registry.bind_tool(physical(),id)==result::denied&&id==77);
    assert(registry.bind_reader(arbiter(),id)==result::denied&&id==77);
    assert(registry.publish(100,301,1,bytes,{},1500,id)==result::denied&&id==77);
    assert(registry.accept(200,{},[](auto const&){assert(false);return result::success;},receipt)==result::denied&&receipt.request==77);
    assert(registry.configure({},100,101)==result::success);
    assert(registry.bind_tool(physical(),id)==result::invalid); // required observation cannot fit zero capacity
    auto zero_tool=physical();zero_tool.effect=p::effect_class::non_physical;
    zero_tool.physical_scope=zero_tool.controller_lease_epoch=zero_tool.minimum_observations=0;
    assert(registry.bind_tool(zero_tool,id)==result::again);
    assert(registry.bind_reader(arbiter(),id)==result::again);
    assert(registry.configure(bounds,100,101)==result::conflict);
    core invalid{now};auto excessive=bounds;excessive.argument_bytes=65;
    assert(invalid.configure(excessive,100,101)==result::too_large);
    excessive=bounds;excessive.max_lifetime_ns=UINT64_MAX;
    assert(invalid.configure(excessive,100,101)==result::too_large);
    assert(invalid.configure(bounds,0,101)==result::invalid);
    assert(invalid.configure(bounds,100,101)==result::success);
    auto self=arbiter();self.actor=100;assert(invalid.bind_reader(self,id)==result::invalid);
    self=arbiter();self.capability=101;assert(invalid.bind_reader(self,id)==result::invalid);
    self=arbiter();self.authority_epoch=0;assert(invalid.bind_reader(self,id)==result::invalid);
    self=arbiter();self.target_actions=256;assert(invalid.bind_reader(self,id)==result::invalid);
    self=arbiter();self.read=false;assert(invalid.bind_reader(self,id)==result::invalid);
    assert(!c::valid(static_cast<c::target_action>(255))&&c::action_bit(static_cast<c::target_action>(255))==0);
    assert(c::names[0]=="proposal.read"&&c::accept_fields.size()==6);
    auto invalid_tool=physical();invalid_tool.operation=static_cast<c::target_action>(255);
    assert(invalid.bind_tool(invalid_tool,id)==result::invalid);
    invalid_tool=physical();invalid_tool.max_observation_age_ns=UINT64_MAX;
    assert(invalid.bind_tool(invalid_tool,id)==result::invalid);
}

static void immutable_bounded_publication()
{
    fixture f;p::identity id=77;
    std::array ids{f.source};auto mutable_bytes=bytes;
    assert(f.registry.publish(999,301,f.tool,bytes,ids,1500,id)==result::denied&&id==77);
    assert(f.registry.publish(100,0,f.tool,bytes,ids,1500,id)==result::invalid&&id==77);
    std::array<std::uint8_t,65> excessive{};
    assert(f.registry.publish(100,301,f.tool,excessive,ids,1500,id)==result::too_large&&id==77);
    std::array duplicate{f.source,f.source};
    assert(f.registry.publish(100,301,f.tool,bytes,duplicate,1500,id)==result::invalid&&id==77);
    assert(f.registry.publish(100,301,f.tool,bytes,{},1500,id)==result::invalid&&id==77);
    assert(f.registry.publish(100,301,f.tool,mutable_bytes,ids,1500,id)==result::success);
    mutable_bytes.fill(255);
    bool visited{};
    assert(f.registry.read(200,f.reader,101,id,[&](auto const& record){
        visited=true;assert(record.argument_size==4&&record.arguments[0]==1&&record.observations[0]==observed());
        return false;})==result::delivery_failed&&visited);
    assert(f.registry.read(200,f.reader,101,id,[](auto const&){throw std::runtime_error("sink");return true;})==result::delivery_failed);
    assert(f.registry.read(200,f.reader,101,id,[](auto const& record){return record.arguments[3]==4;})==result::success);
    assert(f.registry.read(999,f.reader,101,id,[](auto const&){assert(false);return true;})==result::denied);
    assert(f.registry.read(200,f.reader,102,id,[](auto const&){assert(false);return true;})==result::denied);
    f.publish();f.publish();f.publish();p::identity extra=77;
    assert(f.registry.publish(100,301,f.tool,bytes,ids,1500,extra)==result::again&&extra==77);
    assert(f.registry.release(id)==result::success);
    auto replacement=f.publish();assert(replacement!=id);
    assert(f.registry.read(200,f.reader,101,id,[](auto const&){assert(false);return true;})==result::bad_handle);
}

static void effect_classification_every_action()
{
    for(unsigned i{};i!=c::target_names.size();++i)
    {
        auto action=static_cast<c::target_action>(i);fixture f{arbiter(),physical(action)};
        auto id=f.publish();p::receipt receipt;
        assert(f.accept(f.request(id),receipt)==result::success);
        assert(receipt.provider_generation==15&&receipt.target_authority_epoch==17&&receipt.controller_lease_epoch==19&&receipt.arbiter_authority_epoch==202);
        core rejected{now};assert(rejected.configure(bounds,100,101)==result::success);p::identity ignored{};
        auto value=physical(action);value.effect=p::effect_class::unclassified;
        assert(rejected.bind_tool(value,ignored)==result::invalid);
        value=physical(action);value.physical_scope=0;assert(rejected.bind_tool(value,ignored)==result::invalid);
        value=physical(action);value.controller_lease_epoch=0;assert(rejected.bind_tool(value,ignored)==result::invalid);
        value=physical(action);value.minimum_observations=0;assert(rejected.bind_tool(value,ignored)==result::invalid);
        auto restricted=arbiter();restricted.target_actions=c::all_actions&~c::action_bit(action);
        fixture denied{restricted,physical(action)};p::receipt untouched{77};
        assert(denied.accept(denied.request(denied.publish()),untouched)==result::denied&&untouched.request==77);
    }
}

static void exact_replay_and_nonconsuming_failure()
{
    fixture f;auto id=f.publish();auto request=f.request(id);p::receipt output{77};unsigned calls{};
    auto validate=[&](auto const&){++calls;return result::success;};
    auto invalid=request;invalid.key.sequence=2;
    assert(f.registry.accept(200,invalid,validate,output)==result::conflict&&output.request==77&&calls==0);
    invalid=request;invalid.key.sequence=UINT64_MAX;
    assert(f.registry.accept(200,invalid,validate,output)==result::conflict&&output.request==77&&calls==0);
    invalid=request;invalid.deadline=UINT64_MAX;
    assert(f.registry.accept(200,invalid,validate,output)==result::timeout&&output.request==77&&calls==0);
    invalid=request;invalid.target_capability=999;
    assert(f.registry.accept(200,invalid,validate,output)==result::denied&&calls==0);
    invalid=request;invalid.target=999;
    assert(f.registry.accept(200,invalid,validate,output)==result::denied&&calls==0);
    assert(f.registry.accept(100,request,validate,output)==result::denied&&calls==0);
    assert(f.registry.accept(200,request,[](auto const&){return result::denied;},output)==result::denied&&output.request==77);
    assert(f.registry.accept(200,request,[](auto const&){throw std::runtime_error("validator");return result::success;},output)==result::denied);
    assert(f.registry.accept(200,request,validate,output)==result::success&&calls==1);
    assert(f.registry.accept(300,request,validate,output)==result::denied&&calls==1);
    auto expected=output;output={}; // simulate loss of an already committed guest output
    assert(f.registry.accept(200,request,validate,output)==result::success&&calls==1&&output==expected);
    invalid=request;invalid.deadline=1399;
    assert(f.registry.accept(200,invalid,validate,output)==result::conflict&&calls==1&&output==expected);
    auto repeat=request;repeat.key.sequence=2;
    assert(f.registry.accept(200,repeat,validate,output)==result::conflict);
    assert(f.registry.release(id)==result::success);
    current_time=2000; // historical receipt recovery does not create fresh execution permission
    assert(f.registry.accept(200,request,validate,output)==result::success&&output==expected&&calls==1);
    f.registry.revoke_reader(f.reader);
    assert(f.registry.accept(200,request,validate,output)==result::denied);
}

static void stale_observations_and_epoch_fences()
{
    for(unsigned mode{};mode!=7;++mode)
    {
        fixture f;auto id=f.publish();p::receipt output{77};auto request=f.request(id);
        result expected=result::conflict;
        if(mode==0){auto next=observed();++next.source_generation;++next.epoch;assert(f.registry.update_source(f.source,next)==result::success);}
        if(mode==1)assert(f.registry.fence_target(f.tool,16,17,19)==result::success);
        if(mode==2)assert(f.registry.fence_target(f.tool,15,18,19)==result::success);
        if(mode==3)assert(f.registry.fence_target(f.tool,15,17,20)==result::success);
        if(mode==4)f.registry.revoke_source(f.source);
        if(mode==5){current_time=1201;expected=result::timeout;}
        if(mode==6)f.registry.revoke_tool(f.tool);
        assert(f.accept(request,output)==expected&&output.request==77);
    }
    fixture f;p::identity id=77;auto obs=observed();++obs.source;obs.captured_steady_ns=1001;
    assert(f.registry.bind_source(obs,id)==result::invalid&&id==77);
    obs=observed();assert(f.registry.update_source(f.source,obs)==result::invalid);
    assert(f.registry.fence_target(f.tool,15,17,19)==result::invalid);
    assert(f.registry.fence_target(f.tool,14,18,19)==result::invalid);
    auto proposal=f.publish();p::receipt receipt{77};auto request=f.request(proposal);request.deadline=1501;
    assert(f.accept(request,receipt)==result::timeout&&receipt.request==77);
    request=f.request(proposal);request.deadline=1000;
    assert(f.accept(request,receipt)==result::timeout&&receipt.request==77);
}

static void callback_reentry_and_revocation()
{
    for(unsigned mode{};mode!=7;++mode)
    {
        fixture f;auto id=f.publish();auto request=f.request(id);p::receipt output{77};
        auto callback=[&](auto const&){
            p::receipt nested{88};assert(f.accept(request,nested)==result::again&&nested.request==88);
            assert(f.registry.release(id)==result::again);
            assert(f.registry.read(200,f.reader,101,id,[](auto const&){return true;})==result::success);
            if(mode==0)return result::denied;
            if(mode==1)f.registry.revoke_reader(f.reader);
            if(mode==2)f.registry.retire();
            if(mode==3)current_time=1400;
            if(mode==4)assert(f.registry.fence_target(f.tool,15,17,20)==result::success);
            if(mode==5){auto next=observed();++next.source_generation;assert(f.registry.update_source(f.source,next)==result::success);}
            if(mode==6)assert(f.registry.attenuate_reader(f.reader,1,100,203)==result::success);
            return result::success;
        };
        auto expected=mode==0?result::denied:(mode==1||mode==2)?result::cancelled:mode==3?result::timeout:result::conflict;
        assert(f.registry.accept(200,request,callback,output)==expected&&output.request==77);
        assert(f.registry.release(id)==result::success); // reservation always released
        if(mode==0){auto another=f.publish();assert(f.accept(f.request(another),output)==result::success);}
    }
}

static void slow_models_with_explicit_observation_policy()
{
    core unconfigured{now};p::identity ignored{};
    assert(unconfigured.configure(bounds,100,101)==result::success);
    auto missing=physical();missing.observations=p::observation_policy::unconfigured;
    assert(unconfigured.bind_tool(missing,ignored)==result::invalid);
    missing.observations=static_cast<p::observation_policy>(255);
    assert(unconfigured.bind_tool(missing,ignored)==result::invalid);
    for(auto policy:{p::observation_policy::latest_required,p::observation_policy::bounded_age})
    {
        auto profile=physical(c::target_action::topic_publish,policy);
        fixture f{arbiter(),profile};auto id=f.publish();auto request=f.request(id);p::receipt output{77};
        for(unsigned sample=1;sample!=4;++sample)
        {
            current_time=1000+20*sample;auto next=observed();next.sample_sequence+=sample;next.buffer+=sample;
            next.source_ns+=20*sample;next.captured_steady_ns=now();
            assert(f.registry.update_source(f.source,next)==result::success);
        }
        bool validated{};
        auto value=f.registry.accept(200,request,[&](auto const& original){
            validated=true;assert(original.observations[0]==observed());return result::success;},output);
        auto expected=policy==p::observation_policy::bounded_age?result::success:result::conflict;
        assert(value==expected&&validated==(policy==p::observation_policy::bounded_age));
        if(policy==p::observation_policy::latest_required)assert(output.request==77);
        // A sensor update during validation has the same policy-dependent result.
        fixture during{arbiter(),profile};id=during.publish();request=during.request(id);output={77};
        assert(during.registry.accept(200,request,[&](auto const& original){
            assert(original.observations[0]==observed());current_time=1050;
            auto next=observed();++next.sample_sequence;++next.buffer;next.captured_steady_ns=now();
            assert(during.registry.update_source(during.source,next)==result::success);return result::success;},output)==expected);
        // Source authority withdrawal is independent of sampling and fences both policies.
        fixture permissions{arbiter(),profile};id=permissions.publish();request=permissions.request(id);output={77};
        assert(permissions.registry.accept(200,request,[&](auto const&){
            assert(permissions.registry.fence_source_authority(permissions.source,33)==result::success);return result::success;
        },output)==result::conflict&&output.request==77);
        assert(permissions.registry.fence_source_authority(permissions.source,33)==result::invalid);
        for(unsigned mode{};mode!=10;++mode)
        {
            fixture fenced{arbiter(),profile};id=fenced.publish();request=fenced.request(id);output={77};
            auto next=observed();++next.sample_sequence;
            if(mode==0){++next.source_generation;next.sample_sequence=1;}
            if(mode==1)++next.calibration_generation;
            if(mode==2)++next.transform_generation;
            if(mode==3)++next.epoch;
            if(mode==4){++next.clock;++next.source_generation;}
            if(mode==5){++next.frame;++next.transform_generation;}
            if(mode==6)++next.source_authority_epoch;
            if(mode==7){++next.capability;++next.source_authority_epoch;}
            if(mode<8)assert(fenced.registry.update_source(fenced.source,next)==result::success);
            if(mode==8)fenced.registry.revoke_source(fenced.source);
            if(mode==9)assert(fenced.registry.fence_target(fenced.tool,16,17,19)==result::success);
            assert(fenced.accept(request,output)==result::conflict&&output.request==77);
        }
    }
    auto profile=physical(c::target_action::control_submit,p::observation_policy::bounded_age);
    for(unsigned edge{};edge!=2;++edge)
    {
        fixture age{arbiter(),profile};auto id=age.publish();auto request=age.request(id);p::receipt output{77};
        current_time=1200+edge;auto next=observed();++next.sample_sequence;++next.buffer;next.captured_steady_ns=now();
        assert(age.registry.update_source(age.source,next)==result::success);
        assert(age.accept(request,output)==(edge?result::timeout:result::success));
        if(edge)assert(output.request==77);
    }
    fixture late{arbiter(),profile};auto id=late.publish();auto request=late.request(id);p::receipt output{77};
    assert(late.registry.accept(200,request,[](auto const&){current_time=1201;return result::success;},output)==result::timeout&&output.request==77);
    fixture revisions{arbiter(),profile};
    for(unsigned mode{};mode!=6;++mode)
    {
        auto wrong=observed();++wrong.sample_sequence;
        if(mode==0)--wrong.source_generation;
        if(mode==1)--wrong.calibration_generation;
        if(mode==2)--wrong.transform_generation;
        if(mode==3)--wrong.source_authority_epoch;
        if(mode==4)--wrong.epoch;
        if(mode==5)++wrong.capability; // authority epoch did not change
        assert(revisions.registry.update_source(revisions.source,wrong)==result::invalid);
    }
    auto same_sample=observed();++same_sample.buffer;
    assert(revisions.registry.update_source(revisions.source,same_sample)==result::invalid);
    // Binding changes cannot return to an old frame/clock and revive its lineage.
    for(bool clock_binding:{false,true})
    {
        fixture aba{arbiter(),profile};auto proposal=aba.publish();auto original=aba.request(proposal);p::receipt unchanged{77};
        auto changed=observed();++changed.sample_sequence;
        if(clock_binding)++changed.clock;else ++changed.frame;
        assert(aba.registry.update_source(aba.source,changed)==result::invalid);
        if(clock_binding)++changed.source_generation;else ++changed.transform_generation;
        assert(aba.registry.update_source(aba.source,changed)==result::success);
        assert(aba.accept(original,unchanged)==result::conflict&&unchanged.request==77);
        auto restored=changed;++restored.sample_sequence;
        if(clock_binding)restored.clock=observed().clock;else restored.frame=observed().frame;
        assert(aba.registry.update_source(aba.source,restored)==result::invalid);
        if(clock_binding)++restored.source_generation;else ++restored.transform_generation;
        assert(aba.registry.update_source(aba.source,restored)==result::success);
        assert(aba.accept(original,unchanged)==result::conflict&&unchanged.request==77);
    }
    // Source restart resets sampling only, not independent permission or frame revisions.
    fixture restart{arbiter(),profile};
    assert(restart.registry.fence_source_authority(restart.source,33)==result::success);
    auto changed=observed();++changed.source_generation;changed.sample_sequence=1;
    assert(restart.registry.update_source(restart.source,changed)==result::invalid); // authority rollback
    changed.source_authority_epoch=33;--changed.calibration_generation;
    assert(restart.registry.update_source(restart.source,changed)==result::invalid);
    changed.calibration_generation=observed().calibration_generation;--changed.transform_generation;
    assert(restart.registry.update_source(restart.source,changed)==result::invalid);
    changed.transform_generation=observed().transform_generation;--changed.epoch;
    assert(restart.registry.update_source(restart.source,changed)==result::invalid);
    changed.epoch=observed().epoch;
    assert(restart.registry.update_source(restart.source,changed)==result::success);
}

static void quotas_are_optional_and_separate()
{
    for(unsigned mode{};mode!=3;++mode)
    {
        auto owner=arbiter();if(mode!=2){owner.calls=mode;owner.window_ns=100;}
        fixture f{owner};auto id=f.publish();auto request=f.request(id);p::receipt output{77};
        if(mode==0)
        {
            assert(f.accept(request,output)==result::again&&output.request==77);
            current_time=1100;assert(f.accept(request,output)==result::again&&output.request==77);
            continue;
        }
        assert(f.registry.accept(200,request,[](auto const&){return result::denied;},output)==result::denied);
        assert(f.accept(request,output)==result::success);auto saved=output;
        assert(f.accept(request,output)==result::success&&output==saved);
        auto next=f.request(f.publish(),2);
        if(mode==1)
        {assert(f.accept(next,output)==result::again&&output==saved);current_time=1100;}
        assert(f.accept(next,output)==result::success);
        assert(f.accept(request,output)==result::conflict); // only the last receipt retained
    }
    auto invalid=arbiter();invalid.calls=0;invalid.window_ns=0;core registry{now};p::identity id=77;
    assert(registry.configure(bounds,100,101)==result::success);
    assert(registry.bind_reader(invalid,id)==result::invalid&&id==77);
    fixture gradual;p::receipt output;
    assert(gradual.accept(gradual.request(gradual.publish()),output)==result::success); // stable unlimited scope
    assert(gradual.registry.attenuate_reader(gradual.reader,2,100,203)==result::success);
    auto second=gradual.request(gradual.publish(),2);
    assert(gradual.accept(second,output)==result::success);
    assert(gradual.registry.attenuate_reader(gradual.reader,1,100,204)==result::success); // retains consumed count
    auto third=gradual.request(gradual.publish(),3);auto saved=output;
    assert(gradual.accept(third,output)==result::again&&output==saved);
    assert(gradual.registry.attenuate_reader(gradual.reader,2,100,205)==result::denied); // no rights expansion
    assert(gradual.registry.attenuate_reader(gradual.reader,1,99,205)==result::denied); // no window reset
    assert(gradual.registry.attenuate_reader(gradual.reader,0,100,205)==result::success);
    assert(gradual.accept(second,output)==result::success&&output==saved); // history only, no quota consumption
    current_time=1100;assert(gradual.accept(third,output)==result::again);
    auto one_per_window=arbiter();one_per_window.calls=1;one_per_window.window_ns=50;
    fixture three_windows{one_per_window};
    for(unsigned window{};window!=3;++window)
    {
        current_time=1000+50*window;auto proposal=three_windows.publish();auto request=three_windows.request(proposal,window+1);
        assert(three_windows.accept(request,output)==result::success);
        assert(three_windows.registry.release(proposal)==result::success);
    }
}

static void fixed_storage_has_no_allocation_path()
{
    fixture f;p::receipt output;forbid_allocation=true;
    auto id=f.publish();auto request=f.request(id);
    assert(f.registry.read(200,f.reader,101,id,[](auto const& value){return value.argument_size==4;})==result::success);
    assert(f.accept(request,output)==result::success);
    assert(f.accept(request,output)==result::success);
    assert(f.registry.attenuate_reader(f.reader,0,100,203)==result::success);
    assert(f.registry.release(id)==result::success);
    f.registry.revoke_reader(f.reader);f.registry.revoke_source(f.source);f.registry.revoke_tool(f.tool);f.registry.retire();
    forbid_allocation=false;
}

static void concurrent_acceptance_and_fork()
{
    fixture f;auto proposal=f.publish();auto request=f.request(proposal);p::receipt a,b{77};
    std::atomic<bool> entered{},finish{};result first{};
    std::thread worker([&]{first=f.registry.accept(200,request,[&](auto const&){entered.store(true);entered.notify_all();
        finish.wait(false);return result::success;},a);});
    entered.wait(false);
    assert(f.accept(request,b)==result::again&&b.request==77);
    auto concurrent=f.request(f.publish());
    assert(f.accept(concurrent,b)==result::again&&b.request==77);
    auto other=arbiter();other.actor=300;other.capability=302;p::identity other_reader{};
    assert(f.registry.bind_reader(other,other_reader)==result::success);
    auto other_request=request;other_request.key.session=other_reader;
    assert(f.registry.accept(300,other_request,[](auto const&){return result::success;},b)==result::again);
    // A forked child must fail before touching any inherited synchronization.
    auto child=::fork();assert(child>=0);
    if(child==0){p::receipt output;auto value=f.accept(request,output);::_exit(value==result::denied?0:1);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
    finish.store(true);finish.notify_all();worker.join();assert(first==result::success);
    assert(f.accept(request,b)==result::success&&a==b);
    assert(f.registry.accept(300,other_request,[](auto const&){return result::success;},b)==result::conflict);
}

int main()
{
    zero_authority_and_closed_actions();immutable_bounded_publication();effect_classification_every_action();
    exact_replay_and_nonconsuming_failure();stale_observations_and_epoch_fences();callback_reentry_and_revocation();
    quotas_are_optional_and_separate();concurrent_acceptance_and_fork();fixed_storage_has_no_allocation_path();
    slow_models_with_explicit_observation_policy();
    std::cout<<"passed 10 bounded proposal-admission safety groups\nallocation_trap="<<(allocation_trap?"enabled":"disabled under TSan")<<'\n';
}
