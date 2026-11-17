/** @file @brief Canonical creation controls against the real receipt/authority ledger.
 * @details The provider below models already verified native evidence. This
 * qualifies public control admission and recovery, not native or VM transport.
 */
#include <uwvm2/imported/ros/scene/communication_open_dispatch.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace creation=s::communication_creation;
namespace wire=s::wire;
namespace contract=uwvm2::imported::ros::communication_open_contract;
using dispatch=s::communication_open_dispatch;
using operation=dispatch::operation;
struct source final:dispatch::provider
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    int instance{};s::caller who{&instance,1};std::array<s::handle,4> caps{};
    creation::ledger ledger;creation::plan policy;creation::identity original;
    s::handle request{},resource{};unsigned preparations{},publications{},cancellations{},releases{};
    bool invalid_origin{},revoke_preparing{},fail_prepare{};unsigned corrupt{};
    explicit source(std::optional<std::uint64_t> calls={})
    {
        for(auto name:{"capability.open","capability.close","topic.open","node.open","context.open","type.describe",
            "communication.open.take","communication.open.cancel","request.describe","request.release"})assert(permissions->expose(name,s::exposure::allow)==s::error::success);
        constexpr std::array aliases{"topic","node","context","schema"};constexpr std::array kinds{s::purpose::topic,s::purpose::node,s::purpose::context,s::purpose::type};
        for(std::size_t i{};i<4;++i)
        {
            s::grant grant;grant.alias=aliases[i];grant.kind=kinds[i];grant.resource="controlled.creation";grant.binding="native-communication";
            if(!i)
            {for(auto name:{"topic.open","communication.open.take","communication.open.cancel","request.describe","request.release"})grant.actions.emplace(name);
                grant.limits.calls=calls;grant.limits.uncertain=calls.has_value();grant.limits.window=std::chrono::hours{1};}
            else grant.actions.emplace(i==1?"node.open":i==2?"context.open":"type.describe");
            assert(permissions->configure(std::move(grant))==s::error::success);
        }
        assert(permissions->enroll(who)==s::error::success);for(std::size_t i{};i<4;++i)assert(permissions->open(who,aliases[i],caps[i])==s::error::success);
        policy.authority={17,1,1};policy.selected.identity="topic";policy.selected.digest[0]=1;policy.kind=contract::endpoint_kind::topic;
        policy.worker_binding=1;policy.provider_generation=71;policy.carrier_bytes=128;policy.max_deadline_ns=1000000000;
        assert(ledger.configure({&policy,1},256)==c::error::ok);
        original.who=who;original.authority=policy.authority;original.key=1;original.deadline=c::now()+900000000;original.count=4;
        for(std::size_t i{};i<4;++i)original.origins[i]={i==0?creation::dependency::operation:i==1?creation::dependency::node:i==2?creation::dependency::context:creation::dependency::schema,
            {17,i+1,1},caps[i],i==1?44u:i==2?55u:0u};
        std::array<std::uint8_t,24> bytes;wire::writer out{bytes};assert(out.u64(42)&&out.u64(1)&&out.u64(original.deadline));
        creation::view queued;assert(ledger.enqueue(original,bytes,charge,this,queued)==c::error::ok);request=queued.receipt.request_id;
    }
    static c::error charge(void* object,creation::identity const& origin,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<source*>(object);creation::claim_set claims;
        return static_cast<c::error>(self.permissions->admit_group(self.who,creation::admission_claims(origin,self.policy.kind,claims,bytes),held));
    }
    void complete(bool unknown=false)
    {
        creation::claim_set claims;s::group_lease held;assert(permissions->admit_group(who,creation::admission_claims(original,policy.kind,claims),held,false)==s::error::success);
        creation::work work;assert(ledger.begin(request,held,work)==c::error::ok);held.release();
        assert(ledger.complete(request,unknown?c::error::io:c::error::ok,unknown?c::effect::unknown:c::effect::locally_accepted,unknown?0:9001)==c::error::ok);
    }
    c::error inspect(s::caller who,s::handle request,dispatch::observation& out)noexcept override
    {
        auto status=ledger.inspect_origin(who,request,out.state,out.origin);if(status!=c::error::ok)return status;
        if(corrupt==1)++out.origin.authority.grant;
        if(corrupt==2)++out.origin.origins[2].authority.environment;
        if(corrupt==3)++out.origin.who.generation;
        if(corrupt==4)out.state.kind=contract::endpoint_kind::service;
        if(corrupt==5)out.state.provider_generation=0;
        return c::error::ok;
    }
    c::error validate(creation::identity const& origin)noexcept override
    {return origin==original&&!invalid_origin?c::error::ok:c::error::denied;}
    c::error prepare(s::caller who,s::handle request,s::handle& out)noexcept override
    {
        if(!creation::same_caller(who,this->who)||request!=this->request)return c::error::denied;
        ++preparations;if(fail_prepare)return c::error::no_memory;
        if(!resource)resource=uwvm2::imported::ros::resources::allocate_identity();
        out=resource;
        if(revoke_preparing)assert(permissions->revoke("topic",[](auto){})==s::error::success);
        return c::error::ok;
    }
    c::error publish(s::caller who,s::handle request,s::group_lease const& held,s::handle resource,contract::published& out)noexcept override
    {++publications;return resource==this->resource?ledger.publish(who,request,held,resource,out):c::error::denied;}
    c::error cancel(s::caller who,s::handle request,creation::view& out)noexcept override
    {++cancellations;return ledger.cancel(who,request,out);}
    c::error release(s::caller who,s::handle request)noexcept override
    {++releases;return ledger.release(who,request);}
};
struct fixture
{
    std::shared_ptr<source> actual;dispatch control;std::array<std::uint8_t,8> input;std::size_t required{},copies{};
    std::array<std::uint8_t,512> copied{};bool accept{true};
    explicit fixture(std::optional<std::uint64_t> calls={}):actual(uwvm2::utils::container::make_shared<source>(calls)),control(actual->permissions,{1,1040,2})
    {
        assert(control.bind("topic",actual->policy.authority,actual->policy.kind,actual)==s::error::success&&control.prepare()==s::error::success);
        wire::writer out{input};assert(out.u64(actual->request));
    }
    s::error call(operation op,std::size_t capacity=512)
    {
        return control.invoke(actual->who,op,actual->caps[0],input,capacity,required,[&](wire::bytes bytes)
            {++copies;std::copy(bytes.begin(),bytes.end(),copied.begin());return accept;});
    }
};
static void publication()
{
    fixture f{2};f.actual->complete();assert(f.call(operation::take,63)==s::error::too_large&&f.required==64&&!f.actual->preparations);
    f.accept=false;assert(f.call(operation::take)==s::error::io&&f.actual->preparations==1&&f.actual->publications==1);
    auto first=f.copied;f.accept=true;assert(f.call(operation::take)==s::error::success&&f.copied==first&&f.actual->preparations==1);
    assert(f.call(operation::cancel)==s::error::again&&!f.actual->cancellations);
    assert(f.call(operation::release)==s::error::again&&!f.actual->releases);
    assert(f.call(operation::describe)==s::error::again); // Creation and first take exhausted both traffic units.
    assert(f.actual->ledger.cleanup(f.actual->request,true)==c::error::ok);
    assert(f.call(operation::release,7)==s::error::too_large&&!f.actual->releases);
    f.accept=false;assert(f.call(operation::release)==s::error::io);f.accept=true;
    assert(f.call(operation::release)==s::error::success);
}
static void authority_and_cleanup()
{
    fixture exhausted{1};exhausted.actual->complete();assert(exhausted.call(operation::take)==s::error::again&&!exhausted.actual->preparations);
    fixture cancelled{1};assert(cancelled.call(operation::cancel,47)==s::error::too_large&&!cancelled.actual->cancellations);
    assert(cancelled.actual->permissions->close(cancelled.actual->who,cancelled.actual->caps[1])==s::error::success);
    cancelled.accept=false;assert(cancelled.call(operation::cancel)==s::error::io);cancelled.accept=true;
    assert(cancelled.call(operation::cancel)==s::error::success&&cancelled.call(operation::release)==s::error::success);
    fixture unknown;unknown.actual->complete(true);assert(unknown.call(operation::describe)==s::error::success);
    assert(unknown.call(operation::release)==s::error::again&&unknown.actual->ledger.cleanup(unknown.actual->request,true)==c::error::ok);
    assert(unknown.call(operation::release)==s::error::again&&!unknown.actual->releases);
    fixture changed;changed.actual->complete();changed.actual->invalid_origin=true;assert(changed.call(operation::take)==s::error::denied&&!changed.actual->preparations);
    assert(changed.call(operation::describe)==s::error::success);
    fixture revoked;revoked.actual->complete();revoked.actual->revoke_preparing=true;assert(revoked.call(operation::take)==s::error::denied&&revoked.actual->preparations==1&&!revoked.actual->publications);
    fixture failed;failed.actual->complete();failed.actual->fail_prepare=true;assert(failed.call(operation::take)==s::error::no_memory&&!failed.actual->publications);
    failed.actual->fail_prepare=false;assert(failed.call(operation::take)==s::error::success);
    for(unsigned fault{1};fault<=5;++fault)
    {fixture f;f.actual->complete();f.actual->corrupt=fault;assert(f.call(operation::take)==s::error::denied&&!f.actual->preparations);}
}
static void copies_and_concurrency()
{
    fixture f;f.actual->complete();std::size_t required{};
    assert(f.control.invoke(f.actual->who,operation::take,f.actual->caps[0],f.input,512,required,[&](wire::bytes)
        {assert(f.call(operation::take)==s::error::again);return true;})==s::error::success);
    auto foreign=f.actual->who;++foreign.generation;
    assert(f.control.invoke(foreign,operation::describe,f.actual->caps[0],f.input,512,required,[](auto){return true;})==s::error::denied);
    std::atomic<unsigned> reads{};std::array<std::thread,4> threads;
    for(auto& thread:threads)thread=std::thread{[&]{for(unsigned i{};i<32;++i){std::size_t extent{};auto status=f.control.invoke(f.actual->who,operation::describe,f.actual->caps[0],f.input,512,extent,[](auto){return true;});assert(status==s::error::success||status==s::error::again);if(status==s::error::success)++reads;}}};
    for(auto& thread:threads)thread.join();
    assert(reads);
    assert(f.control.invoke(f.actual->who,operation::describe,f.actual->caps[0],f.input,512,required,[](auto){return true;},[]{throw 7;return false;})==s::error::io);
    auto child=::fork();assert(child>=0);if(!child){assert(f.call(operation::take)==s::error::denied);::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    assert(f.actual->permissions->close(f.actual->who,f.actual->caps[3])==s::error::success);
    assert(f.call(operation::take)==s::error::bad_handle&&f.call(operation::describe)==s::error::success);
    f.control.retire();assert(f.call(operation::describe)==s::error::denied);
}
int main(){publication();authority_and_cleanup();copies_and_concurrency();std::puts("PASS canonical creation controls: exact caller/cohort, capacity before effects, replay without re-debit, independent cleanup, unknown debt, reentrancy, concurrency and fork");}
