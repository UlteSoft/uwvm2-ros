/** @file @brief Stable-to-attenuated authority preserves shared accounting and never widens permissions. */
#include <uwvm2/imported/ros/scene/authority.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <barrier>
namespace s=uwvm2::imported::ros::scene;using s::error;
int main()
{
    s::authority authority;int identity{};s::caller who{&identity,1};s::handle first{},second{};s::lease permission;
    s::attenuation rate;rate.calls=3;rate.bytes=20;rate.uncertain=true;rate.window=std::chrono::hours(1);
    assert(authority.attenuate("device",rate)==error::denied&&authority.open(who,"device",first)==error::denied);
    s::grant grant;grant.alias="device";grant.resource="robot";grant.binding="framed-scalar";grant.kind=s::purpose::device;
    grant.actions={"device.submit","device.stop"};
    for(auto name:{"capability.open","capability.close","device.submit","device.stop"})assert(authority.expose(name,s::exposure::allow)==error::success);
    assert(authority.configure(grant)==error::success&&authority.enroll(who)==error::success);
    assert(authority.open(who,"device",first)==error::success&&authority.open(who,"device",second)==error::success);
    for(unsigned n{};n<10;++n){assert(authority.admit(who,first,"device.submit",100,permission)==error::success);permission.release();}
    assert(authority.attenuate("device",rate)==error::success);
    assert(authority.admit(who,first,"device.submit",10,permission)==error::success);permission.release();
    assert(authority.admit(who,second,"device.submit",10,permission)==error::success);permission.release();
    assert(authority.attenuate("device",rate)==error::success); // Does not reset the existing window.
    assert(authority.admit(who,first,"device.submit",1,permission)==error::again);
    assert(authority.close(who,first)==error::success&&authority.open(who,"device",first)==error::success);
    assert(authority.admit(who,first,"device.submit",1,permission)==error::again);
    auto changed=rate;changed.calls=4;assert(authority.attenuate("device",changed)==error::denied);
    changed=rate;changed.bytes.reset();assert(authority.attenuate("device",changed)==error::denied);
    changed=rate;changed.window*=2;assert(authority.attenuate("device",changed)==error::denied);
    changed=rate;changed.uncertain=false;assert(authority.attenuate("device",changed)==error::invalid);
    rate.calls=0;rate.bytes=0;assert(authority.attenuate("device",rate)==error::success);
    assert(authority.admit(who,second,"device.submit",0,permission)==error::again);
    // Exact reserved cleanup authorization remains independently available;
    // no charge=false call is a guest-callable primitive.
    assert(authority.admit(who,second,"device.stop",0,permission,false)==error::success);permission.release();
    assert(authority.admit(who,second,"device.describe",0,permission,false)==error::denied);
    uwvm2::utils::container::std_vector<s::authority_snapshot> rows;
    assert(authority.inspect(rows,1)&&rows.size()==1&&rows[0].attenuated&&rows[0].open_handles==2&&rows[0].admissions==12);
    assert(authority.revoke("device",[](auto){})==error::success&&authority.attenuate("device",rate)==error::denied);
    assert(authority.admit(who,second,"device.stop",0,permission,false)==error::denied);
    // Existing complete admission groups exclude concurrent management; a
    // reduction cannot split the compound decision or grant a new operation.
    s::authority concurrent;int instance{};s::caller caller{&instance,1};s::handle cap{};
    for(auto name:{"capability.open","device.submit","device.stop"})assert(concurrent.expose(name,s::exposure::allow)==error::success);
    assert(concurrent.configure(grant)==error::success&&concurrent.enroll(caller)==error::success&&concurrent.open(caller,"device",cap)==error::success);
    s::admission_claim claim{cap,"device.submit",1};s::group_lease group;
    assert(concurrent.admit_group(caller,{&claim,1},group)==error::success);
    std::barrier ready{2};std::atomic_bool done{};
    std::thread manager{[&]{ready.arrive_and_wait();assert(concurrent.attenuate("device",rate)==error::success);done.store(true);}};
    ready.arrive_and_wait();assert(!done.load());group.release();manager.join();assert(done.load());
    assert(concurrent.admit(caller,cap,"device.submit",0,permission)==error::again);
    claim={cap,"device.stop",0};claim.cleanup_only=true;
    assert(concurrent.admit_group(caller,{&claim,1},group)==error::success);group.release();
    std::puts("PASS stable authority, monotonic attenuation, shared quota and revocation");
}
