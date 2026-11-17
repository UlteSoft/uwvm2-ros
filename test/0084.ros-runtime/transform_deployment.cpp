/** @file @brief Actual TF deployment, shared requests, isolation and all-or-nothing policy freezing. */
#include "transform_deployment_fixture.h"
#include <iostream>
namespace f=transform_fixture;
namespace s=f::s;
namespace t=f::t;
namespace c=f::c;
using s::error;
static void complete_explicit_configuration()
{
    s::deployment value;f::environment(value);
    assert(value.transform_profile("robot","frames",f::profile())==error::denied);
    assert(value.transform_limit("robot","frames","sessions",1)==error::denied);
    assert(value.transform_definition("robot","frames",f::definition())==error::denied);
    f::configure(value);assert(value.transform_profile("robot","frames",f::profile())==error::conflict);
    assert(value.transform_limit("robot","frames","sessions",1)==error::conflict);
    assert(value.transform_limit("robot","frames","unknown",1)==error::invalid);
    assert(value.transform_definition("robot","frames",f::definition())==error::conflict);
    // Duplicate settings are rejected before attempting any host file access.
    assert(value.transform_definition_file("robot","frames","/does-not-exist",1,{})==error::conflict);
    assert(value.transform_budget("robot",f::budget)==error::success&&value.transform_budget("robot",f::budget)==error::invalid);
    assert(value.validate()==error::success&&value.validate()==error::success);
    assert(value.transform_limit("robot","frames","sessions",1)==error::invalid);
    assert(value.transform_definition("robot","frames",f::definition())==error::invalid);
    int identity{};std::shared_ptr<s::broker> state;
    assert(value.select("unknown",{&identity,1},state)==error::denied&&!state);
    assert(value.select("main",{&identity,1},state)==error::success&&state);
    t::usage usage;assert(state->transform_store.inspect("frames",usage)==error::success&&!usage.retained_sessions);
    assert(state->transform_store.inspect("missing",usage)==error::bad_handle);
}
static void missing_bounds_and_budget()
{
    for(std::size_t skip{};skip<f::limits.size();++skip)
    {
        s::deployment value;f::environment(value);f::configure(value,"frames","robot",skip);
        assert(value.transform_budget("robot",f::budget)==error::success&&value.validate()==error::invalid);
        assert(value.transform_limit("robot","frames",t::limit_names[skip],f::limits[skip])==error::success&&value.validate()==error::success);
    }
    s::deployment missing;f::environment(missing);assert(missing.allow("robot",f::grant())==error::success&&missing.validate()==error::invalid);
    s::deployment missing_budget;f::environment(missing_budget);f::configure(missing_budget);assert(missing_budget.validate()==error::invalid);
    assert(missing_budget.transform_budget("robot",f::budget)==error::success&&missing_budget.validate()==error::success);
    s::deployment zero;f::environment(zero);f::configure(zero);assert(zero.transform_budget("robot",{})==error::success&&zero.validate()==error::invalid);
    s::deployment overflow;f::environment(overflow);auto excessive=f::budget;excessive.carrier_bytes=UINT64_MAX;
    assert(overflow.transform_budget("robot",excessive)==error::invalid);
}
static void whole_policy_remains_mutable_after_failure()
{
    s::deployment value;f::environment(value);f::configure(value);assert(value.transform_budget("robot",f::budget)==error::success);
    f::environment(value,"second","other");f::configure(value,"frames","second",0);assert(value.transform_budget("second",f::budget)==error::success);
    assert(value.validate()==error::invalid);
    // An earlier prepared environment must not become live or freeze its settings.
    int identity{};std::shared_ptr<s::broker> state;assert(value.select("main",{&identity,1},state)==error::invalid&&!state);
    assert(value.transform_limit("second","frames","cache.samples",16)==error::success&&value.validate()==error::success);
    assert(value.select("main",{&identity,1},state)==error::success);
    std::shared_ptr<s::broker> other;int second{};assert(value.select("other",{&second,1},other)==error::success&&other!=state);
    s::handle cap{},foreign{};assert(state->permissions.open({&identity,1},"frames",cap)==error::success);
    assert(other->permissions.open({&second,1},"frames",foreign)==error::success);
    s::lease lease;assert(state->permissions.admit({&second,1},cap,"transform.open",0,lease,false)==error::denied);
    assert(state->revoke("frames")==error::success);
    assert(other->permissions.admit({&second,1},foreign,"transform.open",0,lease,false)==error::success);
}
static void shared_request_and_close_dispatch()
{
    s::deployment value;f::environment(value);f::configure(value);f::clock(value);assert(value.transform_budget("robot",f::budget)==error::success);
    int instance{};s::caller who{&instance,1};std::shared_ptr<s::broker> state;assert(value.select("main",who,state)==error::success);
    s::handle cap{},clock_cap{};assert(state->permissions.open(who,"frames",cap)==error::success&&state->permissions.open(who,"clock",clock_cap)==error::success);
    f::bytes result;auto sink=[&](s::bytes bytes){result.assign(bytes.begin(),bytes.end());return true;};std::size_t required{};
    auto create_clock=f::encode([](auto& out){assert(out.u64(static_cast<unsigned>(uwvm2::imported::ros::resources::clock_kind::ros)));});
    assert(state->invoke(who,uwvm2::imported::ros::runtime_contract::operation::clock_create,clock_cap,create_clock,64,required,sink)==error::success);
    auto clock=s::word(result,0);assert(state->runtime.clock_update("clock",clock,1000000000,true)==error::success);
    auto profile=f::profile();auto opening=f::encode([&](auto& out){assert(out.text(profile.identity.view())&&out.blob(profile.sha256)&&out.u64(clock_cap)&&out.u64(clock));});
    assert(state->transform_store.invoke(who,c::operation::open,cap,opening,8,required,sink)==error::success);auto buffer=s::word(result,0);
    auto submission=f::encode([&](auto& out)
    {
        assert(out.u64(buffer)&&out.u64(1)&&out.u64(buffer)&&out.text("map")&&out.boolean(true)&&c::write_stamp(out,{clock,1,10})&&
            out.text("base")&&out.boolean(false)&&out.boolean(false)&&out.u64(uwvm2::imported::ros::resources::steady_now()+500000000));
    });
    assert(state->transform_store.invoke(who,c::operation::submit,cap,submission,160,required,sink)==error::success);auto request=s::word(result,0);
    auto input=f::encode([&](auto& out){assert(out.u64(request));});namespace m=uwvm2::imported::ros::model_contract;
    assert(state->preflight_model_or_request(who,m::operation::request_describe,cap,input.size())==error::success);
    assert(state->invoke_model(who,m::operation::request_describe,cap,input,160,required,sink)==error::success&&s::word(result,0)==request);
    assert(state->invoke_model(who,m::operation::open,cap,input,160,required,sink)==error::denied);
    assert(state->revoke("clock")==error::success);
    auto cancel=f::encode([&](auto& out){assert(out.u64(buffer)&&out.u64(request));});
    assert(state->transform_store.invoke(who,c::operation::cancel,cap,cancel,160,required,sink)==error::success);
    assert(state->invoke_model(who,m::operation::request_describe,cap,input,160,required,sink)==error::success);
    assert(state->invoke_model(who,m::operation::request_release,cap,input,8,required,sink)==error::success&&s::word(result,0)==1);
    auto close=f::encode([&](auto& out){assert(out.u64(buffer));});
    assert(state->invoke(who,uwvm2::imported::ros::runtime_contract::operation::resource_close,cap,close,0,required,sink)==error::success);
    t::usage usage;assert(state->transform_store.inspect("frames",usage)==error::success&&!usage.retained_sessions);
}
int main()
{
    complete_explicit_configuration();missing_bounds_and_budget();whole_policy_remains_mutable_after_failure();shared_request_and_close_dispatch();
    std::cout<<"passed 4 transform deployment security groups\n";
}
