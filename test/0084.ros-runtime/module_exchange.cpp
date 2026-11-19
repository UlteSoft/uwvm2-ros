/** @brief Registered-principal messaging and erasure on every terminal path. */
#include <uwvm2/imported/ros/scene/module_exchange.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
namespace s=uwvm2::imported::ros::scene;
namespace x=s::module_exchange;
using E=s::error;using O=x::operation;
struct endpoint{mutable std::array<std::uint8_t,128> memory{};bool erase_failure{};};
static bool wipe(s::caller who,std::uint64_t memory,std::uint64_t offset,std::uint64_t size,std::uint64_t length_offset,std::uint64_t length_size)noexcept
{
    auto* owner=static_cast<endpoint const*>(who.instance);
    if(owner->erase_failure||memory||offset>128||size>128-offset)return false;
    s::erase_bytes(std::span(owner->memory).subspan(offset,size));
    if(length_size){if(length_offset>128||length_size>128-length_offset)return false;s::erase_bytes(std::span(owner->memory).subspan(length_offset,length_size));}return true;
}
static bool zero(endpoint const& value){for(auto c:value.memory)if(c)return false;return true;}
// Inspect actual retained bytes across long, short, empty and cancellation paths.
static void reserved_slot_stays_zero_outside_live_payload()
{
    x::slot slot;
    slot.prepare(8192,256);
    auto clean=[](s::erased_buffer const& buffer)
    {return std::ranges::all_of(buffer.view(),[](auto byte){return byte==0;});};
    assert(clean(slot.request)&&clean(slot.response));
    for(std::size_t size:{8192u,7u,0u,512u,4u})
    {
        slot.request_size=size;slot.response_size=std::min(size,slot.response.view().size());
        std::ranges::fill(slot.request.writable().first(size),0xab);
        std::ranges::fill(slot.response.writable().first(slot.response_size),0xcd);
        slot.erase_request();
        assert(clean(slot.request));
        assert(!slot.response_size||slot.response.view().front()==0xcd);
        assert(slot.erase()&&clean(slot.response));
    }
}
static void independent_retained_regions()
{
    endpoint client,server;s::caller c{&client,1},who{&server,1};
    x::router router;std::array<s::grant,4> permissions;
    std::array<std::uint64_t,4> tokens{};std::array<x::region,4> regions{};
    std::array<char const*,4> names{"one","two","three","four"};
    auto output=[](std::uint64_t,std::span<std::uint8_t const>){return true;};
    for(std::size_t i{};i<names.size();++i)
    {
        x::plan plan{names[i],"main","worker","echo","request","response",8,8,1,1000};
        assert(router.bind(plan,x::reservation(plan)*names.size())==E::success);
        auto& permission=permissions[i];permission.alias=permission.resource=names[i];
        permission.kind=s::purpose::module_call;permission.binding="registered-message";
        for(auto name:x::names)permission.actions.emplace(name);
        router.bind_grant(permission);
        regions[i]={who,0,i*32,16,wipe,i*32+24,8};
        assert(router.invoke(who,"worker",permission,O::take,0,{},regions[i],8,0,output)==E::again);
        auto receipt=[&](std::uint64_t id,std::span<std::uint8_t const>){tokens[i]=id;return true;};
        assert(router.invoke(c,"main",permission,O::submit,0,{}, {},8,0,receipt)==E::success);
        assert(router.invoke(who,"worker",permission,O::take,0,{},regions[i],8,0,output)==E::success);
    }
    auto retained=[&](std::size_t i)
    {
        assert(!router.available(regions[i],{})&&!router.available({},regions[i]));
        auto length=regions[i];length.offset=length.length_offset;length.size=8;length.length_size=0;
        assert(!router.available(length,{}));
    };
    for(std::size_t i{};i<names.size();++i)retained(i);
    for(auto i:{0u,2u})
    {
        assert(router.invoke(c,"main",permissions[i],O::cancel,tokens[i],{}, {},0,0,output)==E::success);
        assert(router.available(regions[i],{}));retained(1);retained(3);
    }
    server.erase_failure=true;
    assert(router.invoke(c,"main",permissions[1],O::cancel,tokens[1],{}, {},0,0,output)==E::io);
    retained(1);retained(3);
    server.erase_failure=false;router.revoke(permissions[3].alias);
    assert(router.available(regions[3],{}));retained(1);
    // Repeated erasure and failed delivery must not drop a different retained
    // region. Its quarantine remains visible until that exact wipe succeeds.
    router.revoke(permissions[3].alias);retained(1);
    assert(router.invoke(c,"main",permissions[1],O::cancel,tokens[1],{}, {},0,0,output)==E::success);
    for(auto const& region:regions)assert(router.available(region,region));
}
static void indexed_authority()
{
    int identity{},other{};s::caller who{&identity,1},foreign{&other,1};
    for(std::size_t grant_index{};grant_index<x::names.size();++grant_index)
    for(std::size_t visible_index{};visible_index<x::names.size();++visible_index)
    for(auto visibility:{s::exposure::hidden,s::exposure::deny,s::exposure::allow})
    {
        s::authority a;s::grant grant;grant.alias="exchange";grant.resource="route";
        grant.kind=s::purpose::module_call;grant.binding="registered-message";grant.actions.emplace(x::names[grant_index]);
        assert(a.configure(grant)==E::success);
        assert(a.expose("capability.open",s::exposure::allow)==E::success);
        assert(a.expose(uwvm2::utils::container::std_string{x::names[visible_index]},visibility)==E::success);
        assert(a.enroll(who)==E::success);s::handle cap{};assert(a.open(who,"exchange",cap)==E::success);
        for(std::size_t action{};action<x::names.size();++action)
        {
            s::lease held;auto result=a.admit(who,cap,static_cast<O>(action),0,held,false);
            assert(result==(action==grant_index&&action==visible_index&&visibility==s::exposure::allow?E::success:E::denied));
            held.release();assert(a.admit(who,cap,x::names[action],0,held,false)==result);held.release();
            assert(a.admit(foreign,cap,static_cast<O>(action),0,held,false)==E::denied);
            assert(a.admit({&identity,2},cap,static_cast<O>(action),0,held,false)==E::denied);
        }
        s::lease held;
        assert(a.admit(who,cap,static_cast<O>(255),0,held)==E::unsupported&&!held);
    }
    // Differential string/indexed budget and revocation behavior, including a
    // non-charging preflight and reserved cleanup after attenuation to zero.
    s::authority a,b;s::grant grant;grant.alias="exchange";grant.resource="route";
    grant.kind=s::purpose::module_call;grant.binding="registered-message";
    for(auto name:x::names)grant.actions.emplace(name);
    grant.limits.calls=1;grant.limits.bytes=32;grant.limits.uncertain=true;grant.limits.window=std::chrono::hours{1};
    s::handle ca{},cb{};
    for(auto* authority:{&a,&b})
    {
        assert(authority->configure(grant)==E::success);
        assert(authority->expose("capability.open",s::exposure::allow)==E::success);
        for(auto name:x::names)assert(authority->expose(uwvm2::utils::container::std_string{name},s::exposure::allow)==E::success);
        assert(authority->enroll(who)==E::success);
    }
    assert(a.open(who,"exchange",ca)==E::success&&b.open(who,"exchange",cb)==E::success);
    auto compare=[&](O operation,std::uint64_t bytes,bool charge,bool preflight,E expected)
    {
        s::lease left,right;
        assert(a.admit(who,ca,operation,bytes,left,charge,preflight)==expected);
        assert((preflight?b.preflight(who,cb,x::names[static_cast<std::size_t>(operation)],bytes,right):
            b.admit(who,cb,x::names[static_cast<std::size_t>(operation)],bytes,right,charge))==expected);
        left.release();right.release();
        uwvm2::utils::container::std_vector<s::authority_snapshot> one,two;
        assert(a.inspect(one,1)&&b.inspect(two,1)&&one.size()==1&&two.size()==1);
        assert(one[0].admissions==two[0].admissions&&one[0].refused==two[0].refused&&one[0].revoked==two[0].revoked);
    };
    compare(O::submit,32,false,true,E::success);compare(O::submit,32,false,true,E::success);
    compare(O::submit,32,true,false,E::success);compare(O::submit,0,false,true,E::again);
    compare(O::submit,0,true,false,E::again);
    auto reduction=grant.limits;reduction.calls=0;reduction.bytes=0;
    assert(a.attenuate("exchange",reduction)==E::success&&b.attenuate("exchange",reduction)==E::success);
    compare(O::cancel,0,false,false,E::success);compare(O::submit,0,false,true,E::again);
    assert(a.revoke("exchange",[](auto const&)noexcept{})==E::success&&b.revoke("exchange",[](auto const&)noexcept{})==E::success);
    compare(O::cancel,0,false,false,E::denied);
}
int main()
{
    reserved_slot_stays_zero_outside_live_payload();
    indexed_authority();independent_retained_regions();
    endpoint client,server,intruder;s::caller c{&client,1},s{&server,1},bad{&intruder,1};
    x::router router;x::plan plan{"route","main","worker","solve","request-octets","response-octets",64,64,1,100};
    assert(router.bind(plan,x::reservation(plan)-1)==E::too_large);
    assert(router.bind(plan,x::reservation(plan))==E::success);
    assert(router.bind(plan,x::reservation(plan)*2)==E::conflict);
    s::grant grant;grant.alias="exchange";grant.resource="route";grant.kind=s::purpose::module_call;grant.binding="registered-message";
    for(auto name:x::names)grant.actions.emplace(name);
    router.bind_grant(grant);
    std::array<std::uint8_t,4> request{0xde,0xad,0xbe,0xef},reply{1,3,5,7};std::uint64_t id{};
    x::region client_region{c,0,0,120,wipe,120,8},server_region{s,0,0,120,wipe,120,8};
    auto receipt=[&](std::uint64_t token,auto){id=token;return true;};
    auto receive=[&](std::uint64_t token,std::span<std::uint8_t const> bytes)
    {id=token;std::memcpy(server.memory.data(),bytes.data(),bytes.size());return true;};
    auto result=[&](std::uint64_t token,std::span<std::uint8_t const> bytes)
    {id=token;std::memcpy(client.memory.data(),bytes.data(),bytes.size());return true;};
    auto invoke=[&](s::caller who,std::string_view name,O op,std::uint64_t token,std::span<std::uint8_t const> bytes,
                    x::region region,std::uint64_t now,x::sink sink)
    {return router.invoke(who,name,grant,op,token,bytes,region,64,now,sink);};
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::again); // callee not registered
    assert(invoke(bad,"intruder",O::take,0,{},server_region,0,receive)==E::denied);
    assert(invoke(s,"worker",O::take,0,{},server_region,0,receive)==E::again); // exact live receiver
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::success);auto token=id;
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::again); // bounded outstanding calls
    assert(invoke(bad,"main",O::result,token,{},client_region,0,result)==E::denied);
    assert(invoke(c,"main",O::result,token,{},client_region,0,result)==E::again);
    assert(invoke(s,"worker",O::take,0,{},server_region,0,receive)==E::success);
    assert(std::equal(request.begin(),request.end(),server.memory.begin()));
    assert(!router.available(server_region,{})); // neither input erasure nor output may alias a live delivery
    assert(!router.available({},server_region));
    x::region length_alias{s,0,120,8,wipe};
    assert(!router.available(length_alias,{})); // includes retained result-length cells
    auto other_plan=plan;other_plan.route="other-route";
    assert(router.bind(other_plan,x::reservation(plan)*2)==E::success);
    auto other_grant=grant;other_grant.resource="other-route";
    assert(router.invoke(s,"worker",other_grant,O::take,0,{},server_region,64,0,receive)==E::conflict);
    assert(std::equal(request.begin(),request.end(),server.memory.begin()));
    assert(invoke(s,"worker",O::respond,token,reply,{},0,receipt)==E::success&&zero(server));
    assert(router.available(server_region,{}));
    assert(invoke(c,"main",O::result,token,{},client_region,0,result)==E::success);
    assert(std::equal(reply.begin(),reply.end(),client.memory.begin()));
    assert(invoke(c,"main",O::result,token,{},client_region,0,result)==E::conflict);
    assert(invoke(c,"main",O::acknowledge,token,{}, {},0,receipt)==E::success&&zero(client));
    assert(invoke(c,"main",O::acknowledge,token,{}, {},0,receipt)==E::success); // no new data on retry
    // A partially written failed delivery is wiped and remains receivable.
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::success);token=id;
    auto failed=[&](std::uint64_t,std::span<std::uint8_t const> bytes){server.memory[0]=bytes.front();return false;};
    assert(invoke(s,"worker",O::take,0,{},server_region,0,failed)==E::invalid&&zero(server));
    assert(invoke(s,"worker",O::take,0,{},server_region,0,receive)==E::success);
    assert(invoke(c,"main",O::cancel,token,{}, {},0,receipt)==E::success&&zero(server));
    // Expiry erases the registered consumer region before reporting timeout.
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::success);token=id;
    assert(invoke(s,"worker",O::take,0,{},server_region,0,receive)==E::success);
    assert(invoke(c,"main",O::result,token,{},client_region,100,result)==E::timeout&&zero(server));
    // An eraser failure quarantines instead of recycling a dirty region.
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::success);token=id;
    assert(invoke(s,"worker",O::take,0,{},server_region,0,receive)==E::success);server.erase_failure=true;
    assert(invoke(c,"main",O::cancel,token,{}, {},0,receipt)==E::io);
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::again);
    server.erase_failure=false;assert(invoke(c,"main",O::cancel,token,{}, {},0,receipt)==E::success&&zero(server));
    // Stale generations cannot take over a route even with the same name.
    assert(invoke({&server,2},"worker",O::take,0,{},server_region,0,receive)==E::denied);
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::success);token=id;
    assert(invoke(s,"worker",O::take,0,{},server_region,0,receive)==E::success);
    router.retire_member(s);assert(zero(server));
    assert(invoke(c,"main",O::submit,0,request,{},0,receipt)==E::denied);
    assert(router.invoke(c,"main",grant,static_cast<O>(255),0,{}, {},0,0,receipt)==E::unsupported);
    std::puts("PASS registered messages: bilateral incarnation pins, bounds, delivery rollback, response/ack zeroization, cancel/expiry/retirement, quarantine");
}
