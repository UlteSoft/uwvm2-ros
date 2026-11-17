/** @file
 * @brief Qualify prepared registered-message admission with exact group membership.
 * @details Exact exposure, action, epoch, membership, handle, revocation and
 * attenuation remain independent. Refusal cannot freeze configuration, publish
 * a broker or retain authority. Successful preflight never consumes a budget;
 * final admission must recheck after release. Existing members allocate no
 * storage; first-use group enrollment remains fallible and rolls back safely.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <cstdio>
#include <thread>
namespace s=uwvm2::imported::ros::scene;
namespace x=s::module_exchange;
using E=s::error;using O=x::operation;
static thread_local bool forbid_allocation{};
static thread_local std::size_t allocation_attempts{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
static bool blocked()noexcept{if(!forbid_allocation)return false;++allocation_attempts;return true;}
extern "C" void* __wrap_malloc(std::size_t n){return blocked()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t width){return blocked()?nullptr:__real_calloc(n,width);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return blocked()?nullptr:__real_realloc(p,n);}
static void configure(s::deployment& d,char const* environment,char const* client,char const* server,
                      O selected=O::submit,s::exposure exposure=s::exposure::allow,bool grant_selected=true)
{
    assert(d.create(environment)==E::success);
    assert(d.bind(environment,client)==E::success&&d.bind(environment,server)==E::success);
    assert(d.expose(environment,"capability.open",s::exposure::allow)==E::success);
    s::grant grant;grant.alias="exchange";grant.resource="route";grant.kind=s::purpose::module_call;grant.binding="registered-message";
    for(std::size_t i{};i<x::names.size();++i)
    {
        auto action=static_cast<O>(i);
        assert(d.expose(environment,uwvm2::utils::container::std_string{x::names[i]},action==selected?exposure:s::exposure::allow)==E::success);
        if(action!=selected||grant_selected)grant.actions.emplace(x::names[i]);
    }
    assert(d.module_call_budget(environment,131072,2)==E::success);
    assert(d.module_call_plan(environment,{"route",client,server,"echo","request","response",8,8,1,60000000000})==E::success);
    assert(d.allow(environment,std::move(grant))==E::success);
}
static auto counters(s::broker& broker)
{
    uwvm2::utils::container::std_vector<s::authority_snapshot> rows;
    assert(broker.permissions.inspect(rows,1)&&rows.size()==1);return rows[0];
}
int main()
{
    int principal{},peer{},alien{};s::caller who{&principal,7},second{&peer,7},other{&alien,7};
    s::deployment d;configure(d,"robot","main","worker");configure(d,"separate","foreign","foreign-worker");
    assert(d.module_call_input_clear("missing","route",false)==E::invalid);
    assert(d.module_call_input_clear("robot","missing",false)==E::invalid);
    assert(d.module_call_input_clear("robot","route",false)==E::success);
    assert(d.module_call_input_clear("robot","route",true)==E::success);
    std::shared_ptr<s::broker> selected,broker,foreign;s::lease held;
    assert(d.preflight_module_call("main",who,1,O::submit,selected,held)==E::denied&&!selected&&!held);
    assert(d.bind("robot","unactivated")==E::success); // Refusal did not freeze policy.
    assert(d.validate()==E::success);
    assert(d.module_call_input_clear("robot","route",false)==E::invalid);
    assert(d.preflight_module_call("main",who,1,O::submit,selected,held)==E::bad_handle&&!selected&&!held);
    assert(d.select_exposed("main","capability.open",who,broker)==E::success);
    assert(d.select_exposed("foreign","capability.open",other,foreign)==E::success);
    s::handle cap{},foreign_cap{};
    assert(broker->permissions.open(who,"exchange",cap)==E::success);
    assert(foreign->permissions.open(other,"exchange",foreign_cap)==E::success);
    selected=foreign;
    auto refusal=[&](char const* module,s::caller caller,s::handle handle,O action,E expected)
    {
        assert(!held);auto previous=selected;allocation_attempts=0;forbid_allocation=true;
        bool clear=false;
        auto result=d.preflight_module_call(module,caller,handle,action,selected,held,&clear);
        forbid_allocation=false;assert(!allocation_attempts&&result==expected&&!held&&selected==previous&&!clear);
    };
    // First use by an explicitly bound group member remains supported. Its
    // recoverable allocation cannot publish authority when enrollment fails.
    bool threw{};allocation_attempts=0;forbid_allocation=true;
    try{(void)d.preflight_module_call("worker",second,cap,O::take,selected,held);}
    catch(std::bad_alloc const&){threw=true;}
    forbid_allocation=false;assert(threw&&allocation_attempts&&!held&&selected==foreign);
    s::handle guessed{};assert(broker->permissions.open(second,"exchange",guessed)==E::denied);
    assert(d.preflight_module_call("worker",second,cap,O::take,selected,held)==E::success&&selected==broker);held.release();
    refusal("absent",who,cap,O::submit,E::denied);
    refusal("main",{&principal,0},cap,O::submit,E::denied);
    refusal("main",{&principal,8},cap,O::submit,E::denied);
    refusal("main",{nullptr,7},cap,O::submit,E::denied);
    refusal("main",who,foreign_cap,O::submit,E::bad_handle);
    assert(d.select_exposed("foreign","capability.open",who,selected)==E::success);
    refusal("foreign",who,cap,O::submit,E::bad_handle); // Enrolling in both groups cannot mix their handles.
    refusal("main",who,cap,static_cast<O>(255),E::unsupported);
    auto before=counters(*broker);
    for(std::size_t i{};i<x::names.size();++i)
    {
        allocation_attempts=0;forbid_allocation=true;
        bool clear=false;
        auto result=d.preflight_module_call("main",who,cap,static_cast<O>(i),selected,held,&clear);
        forbid_allocation=false;assert(!allocation_attempts&&result==E::success&&selected==broker&&held&&clear);held.release();
    }
    auto after=counters(*broker);assert(before.admissions==after.admissions&&before.refused==after.refused&&before.open_handles==after.open_handles);
    // Repeated enrollment preserves the same group-sharing semantics.
    assert(d.select_exposed("worker","capability.open",second,selected)==E::success);
    assert(d.preflight_module_call("worker",second,cap,O::take,selected,held)==E::success);held.release();
    s::attenuation rate;rate.uncertain=true;rate.calls=1;rate.window=std::chrono::hours(1);
    assert(d.attenuate("robot","exchange",rate)==E::success);
    for(unsigned i{};i<10;++i){assert(d.preflight_module_call("main",who,cap,O::submit,selected,held)==E::success);held.release();}
    assert(broker->permissions.admit(who,cap,O::submit,0,held)==E::success);held.release();
    refusal("main",who,cap,O::submit,E::again);
    rate.calls=0;assert(d.attenuate("robot","exchange",rate)==E::success);
    refusal("main",who,cap,O::take,E::again);
    for(auto action:{O::cancel,O::acknowledge}){assert(d.preflight_module_call("main",who,cap,action,selected,held)==E::success);held.release();}
    assert(d.preflight_module_call("main",who,cap,O::cancel,selected,held)==E::success);held.release();
    std::thread revoker([&]{assert(broker->revoke("exchange")==E::success);});revoker.join();
    assert(broker->permissions.admit(who,cap,O::cancel,0,held,false)==E::denied&&!held);
    refusal("main",who,cap,O::cancel,E::denied);
    d.retire_member(second);refusal("worker",second,cap,O::take,E::denied);
    assert(d.select_exposed("worker","capability.open",second,selected)==E::denied);
    d.retire();refusal("main",who,cap,O::acknowledge,E::denied);
    {
        s::deployment retired;configure(retired,"robot","main","worker");
        std::shared_ptr<s::broker> live;assert(retired.select_exposed("main","capability.open",who,live)==E::success);
        s::handle id{};assert(live->permissions.open(who,"exchange",id)==E::success);
        retired.retire_member(second);auto output=foreign;
        assert(retired.preflight_module_call("worker",second,id,O::take,output,held)==E::denied&&!held&&output==foreign);
        assert(retired.select_exposed("worker","capability.open",second,output)==E::denied);
        retired.retire_member(who);
        assert(retired.preflight_module_call("main",who,id,O::submit,output,held)==E::denied&&!held&&output==foreign);
    }
    std::size_t combinations{};
    for(bool clearing:{false,true})for(std::size_t i{};i<x::names.size();++i)for(auto visibility:{s::exposure::hidden,s::exposure::deny,s::exposure::allow})for(bool granted:{false,true})
    {
        s::deployment exact;auto action=static_cast<O>(i);configure(exact,"robot","main","worker",action,visibility,granted);
        assert(exact.module_call_input_clear("robot","route",clearing)==E::success);
        std::shared_ptr<s::broker> live,output=foreign;
        assert(exact.select_exposed("main","capability.open",who,live)==E::success);
        s::handle id{};assert(live->permissions.open(who,"exchange",id)==E::success);
        auto expected=visibility==s::exposure::allow&&granted?E::success:E::denied;
        bool actual=!clearing;
        assert(exact.preflight_module_call("main",who,id,action,output,held,&actual)==expected);
        assert(actual==(expected==E::success?clearing:!clearing));
        assert(output==(expected==E::success?live:foreign));assert(bool(held)==(expected==E::success));held.release();
        s::lease reference;assert(live->permissions.preflight(who,id,x::names[i],0,reference)==expected);reference.release();++combinations;
    }
    {
        // A retained-input route cannot weaken another route in the same group,
        // another environment, or any action/exposure/epoch check above.
        s::deployment policy;configure(policy,"robot","main","worker");configure(policy,"other","foreign","foreign-worker");
        assert(policy.module_call_input_clear("robot","route",false)==E::success);
        assert(policy.module_call_plan("robot",{"secure","main","worker","echo","request","response",8,8,1,60000000000})==E::success);
        s::grant grant;grant.alias="secure";grant.resource="secure";grant.kind=s::purpose::module_call;grant.binding="registered-message";
        grant.actions.emplace(x::names[0]);assert(policy.allow("robot",std::move(grant))==E::success);
        for(auto name:{"main","foreign"})
        {
            std::shared_ptr<s::broker> live;assert(policy.select_exposed(name,"capability.open",who,live)==E::success);
            s::handle id{};assert(live->permissions.open(who,"exchange",id)==E::success);
            bool clear=true;assert(policy.preflight_module_call(name,who,id,O::submit,selected,held,&clear)==E::success);
            assert(clear==(std::string_view{name}=="foreign"));held.release();
            if(std::string_view{name}=="main")
            {
                assert(live->permissions.open(who,"secure",id)==E::success);clear=false;
                assert(policy.preflight_module_call(name,who,id,O::submit,selected,held,&clear)==E::success&&clear);held.release();
            }
        }
        assert(policy.module_call_input_clear("robot","route",true)==E::invalid);
    }
    std::printf("PASS prepared exact preflight: %zu exposure/action combinations, zero allocations, epochs, group isolation, budgets and revocation\n",combinations);
}
