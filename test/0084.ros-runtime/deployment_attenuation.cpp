/** @file
 * @brief Exercise live deployment attenuation through real broker calls and detached inspection.
 * @details No provider or device is simulated. Local guard operations establish
 * the same parent authority account used by common and platform dispatchers.
 * Inspection must not admit work, reset a meter, or acquire additional rights.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
using s::error;
using op=uwvm2::imported::ros::runtime_contract::operation;

int main()
{
    s::deployment deployment;s::attenuation rate;rate.uncertain=true;
    rate.calls=2;rate.window=std::chrono::hours(1);
    assert(deployment.attenuate("robot","signal",rate)==error::denied);
    assert(deployment.create("robot")==error::success&&deployment.bind("robot","main")==error::success);
    assert(deployment.attenuate("robot","signal",rate)==error::denied);
    s::grant grant;grant.alias="signal";grant.resource="local.signal";grant.kind=s::purpose::guard;grant.binding="local";
    grant.actions={"guard.create","guard.trigger","guard.take","resource.close"};grant.max_handles=3;grant.max_resources=1;
    assert(deployment.allow("robot",grant)==error::success);
    for(auto action:{"capability.open","capability.close","guard.create","guard.trigger","guard.take","resource.close"})
        assert(deployment.expose("robot",action,s::exposure::allow)==error::success);
    assert(deployment.validate()==error::success);
    assert(deployment.attenuate("robot","signal",rate)==error::denied); // No live runtime yet.
    int identity{};s::caller who{&identity,1};std::shared_ptr<s::broker> broker;
    assert(deployment.select("main",who,broker)==error::success);
    s::handle first{},second{};
    assert(broker->permissions.open(who,"signal",first)==error::success&&broker->permissions.open(who,"signal",second)==error::success);
    s::debug::request query;query.selected=s::debug::selection::authority;query.environment.assign("robot");query.alias.assign("signal");
    auto inspect=[&]
    {
        s::debug::view view;deployment.inspect(query,view);
        assert(view.result==s::debug::status::ok&&view.size==1&&view.total==1&&s::debug::valid(view.rows[0]));
        return view.rows[0];
    };
    std::array<std::uint8_t,8> object{};std::size_t required{};unsigned deliveries{};
    auto call=[&](s::handle capability,op action,s::bytes request)
    {
        return broker->invoke(who,action,capability,request,8,required,[&](s::bytes bytes)
        {++deliveries;if(action==op::guard_create){assert(bytes.size()==8);std::copy(bytes.begin(),bytes.end(),object.begin());}return true;});
    };
    assert(call(first,op::guard_create,{})==error::success);
    for(unsigned n{};n<4;++n)assert(call(first,op::guard_trigger,object)==error::success);
    auto stable=inspect();assert(!stable.attenuated&&!stable.revoked&&stable.admissions==5&&stable.open_handles==2);
    assert(deployment.attenuate("missing","signal",rate)==error::denied);
    assert(deployment.attenuate("robot","missing",rate)==error::denied);
    assert(deployment.attenuate("robot","signal",rate)==error::success);
    auto limited=inspect();assert(limited.attenuated&&!limited.revoked&&limited.admissions==stable.admissions&&limited.refused==stable.refused);
    assert(call(first,op::guard_trigger,object)==error::success&&call(second,op::guard_trigger,object)==error::success);
    auto before=inspect();auto delivered=deliveries;
    for(unsigned n{};n<3;++n)
    {
        assert(deployment.attenuate("robot","signal",rate)==error::success);
        auto row=inspect();assert(row.admissions==before.admissions&&row.attenuated);
        assert(call(first,op::guard_trigger,object)==error::again&&deliveries==delivered);
    }
    assert(broker->permissions.close(who,first)==error::success&&broker->permissions.open(who,"signal",first)==error::success);
    assert(call(first,op::guard_trigger,object)==error::again);
    auto widening=rate;widening.calls=3;assert(deployment.attenuate("robot","signal",widening)==error::denied);
    rate.calls=0;assert(deployment.attenuate("robot","signal",rate)==error::success);
    assert(call(second,op::guard_trigger,object)==error::again);
    assert(call(second,op::guard_create,{})==error::again);
    assert(call(second,op::guard_take,object)==error::again);
    assert(call(second,op::resource_close,{})==error::invalid);
    assert(call(second+1000,op::resource_close,object)==error::bad_handle);
    auto before_cleanup=inspect();
    assert(broker->invoke(who,op::resource_close,second,object,0,required,[](s::bytes){return false;})==error::invalid);
    // Zero traffic is distinct from revocation; reserved resource cleanup is
    // still possible and its handle remains tied to the same exact grant.
    assert(call(second,op::resource_close,object)==error::success);
    assert(inspect().admissions==before_cleanup.admissions);
    assert(call(second,op::resource_close,object)==error::bad_handle);
    assert(broker->revoke("signal")==error::success);
    auto revoked=inspect();assert(revoked.revoked&&revoked.attenuated);
    assert(deployment.attenuate("robot","signal",rate)==error::denied);
    deployment.retire_generation(1);
    assert(deployment.attenuate("robot","signal",rate)==error::denied);
    std::puts("PASS deployment stable authority, live monotonic attenuation, pure inspection and reserved cleanup");
}
