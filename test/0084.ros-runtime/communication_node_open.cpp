/** @file @brief Node creation coordinator with actual registry/authority and modelled worker lifetime. */
#define main node_evidence_fixture_entry
#include "communication_node_evidence.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_node_open_provider.h>
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
struct lifetime final:c::provider_lifetime
{std::atomic<bool> stopped{},confirmed{};void request_stop()noexcept override{stopped=true;}void retire()noexcept override{stopped=true;confirmed=true;}bool cleanup_confirmed()const noexcept override{return confirmed.load();}};
static bytes encode(auto&& fill)
{std::array<std::uint8_t,512> storage;w::writer out{storage};assert(fill(out));return {out.written().begin(),out.written().end()};}
struct pipeline
{
    std::shared_ptr<s::authority> authority=uwvm2::utils::container::make_shared<s::authority>();
    std::shared_ptr<backend> native=uwvm2::utils::container::make_shared<backend>();
    std::shared_ptr<lifetime> worker=uwvm2::utils::container::make_shared<lifetime>();
    std::shared_ptr<s::communication_dispatch> dispatch=uwvm2::utils::container::make_shared<s::communication_dispatch>(authority,19,3,
        s::communication_dispatch::limits{{1,1,0,0,0,256},2,4096,4096,8192,4,10000000000});
    std::shared_ptr<s::communication_node_open_provider> source=uwvm2::utils::container::make_shared<s::communication_node_open_provider>();
    s::communication_open_dispatch controls{authority,{1,1040,2}};
    int instance{};s::caller who{&instance,1};s::handle context_cap{},cap{},context{},request{},node{};bytes carrier;
    explicit pipeline(std::optional<std::uint64_t> calls=2,bool expose=true)
    {
        for(auto name:{"capability.open","capability.close","context.open","resource.close","communication.open.take","communication.open.cancel","request.describe","request.release"})
            assert(authority->expose(name,s::exposure::allow)==s::error::success);
        if(expose)assert(authority->expose("node.open",s::exposure::allow)==s::error::success);
        for(bool is_node:{false,true})
        {
            s::grant grant;grant.alias=is_node?"node":"context";grant.resource=grant.alias;grant.binding="native-communication";grant.kind=is_node?s::purpose::node:s::purpose::context;grant.max_resources=1;
            grant.actions.emplace(is_node?"node.open":"context.open");grant.actions.emplace("resource.close");
            if(is_node)for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release"})grant.actions.emplace(name);
            grant.limits.calls=is_node?calls:std::optional<std::uint64_t>{1};grant.limits.uncertain=true;grant.limits.window=std::chrono::hours{1};
            assert(authority->configure(std::move(grant))==s::error::success);
        }
        assert(dispatch->bind("context",c::context_plan{{},profile("context"),0,{},c::transport::unauthenticated_loopback})==s::error::success);
        assert(dispatch->bind("node",c::node_plan{{},profile("node"),profile("context"),"robot","/scene",2})==s::error::success);
        c::node_plan node_plan;assert(dispatch->describe_plan("node",node_plan)==s::error::success&&dispatch->prepare_all_async(native)==s::error::success);
        assert(source->configure("node",{node_plan.authority,node_plan.selected,opens::endpoint_kind::node,1,71,256,10000000000},dispatch,worker)==c::error::ok);
        assert(controls.bind("node",node_plan.authority,opens::endpoint_kind::node,source)==s::error::success&&controls.prepare()==s::error::success);
        assert(authority->enroll(who)==s::error::success&&authority->open(who,"node",cap)==s::error::success&&authority->open(who,"context",context_cap)==s::error::success);
        auto context_body=encode([](auto& out){return uwvm2::imported::ros::communication_contract::encode_profile(out,profile("context"));});std::size_t required{};
        assert(dispatch->invoke(who,uwvm2::imported::ros::communication_contract::operation::context_open,context_cap,context_body,8,required,[&](w::bytes body){w::reader in{body};return in.u64(context)&&context&&in.done();})==s::error::success);
        carrier=encode([&](auto& out){return out.u64(context_cap)&&out.u64(context)&&uwvm2::imported::ros::communication_contract::encode_profile(out,profile("node"))&&out.u64(1)&&out.u64(c::now()+9000000000);});
    }
    s::error open(bool copied=true,std::size_t capacity=48)
    {
        std::size_t required{};return source->invoke_open(who,cap,carrier,capacity,required,[&](w::bytes body)
            {w::reader in{body};std::uint64_t key{};assert(in.u64(key)&&key==1&&in.u64(request)&&request);return copied;});
    }
    s::error control(s::communication_open_dispatch::operation operation,bool copied=true,std::size_t capacity=512)
    {
        auto body=encode([&](auto& out){return out.u64(request);});std::size_t required{};
        return controls.invoke(who,operation,cap,body,capacity,required,[&](w::bytes output)
        {
            if(operation==s::communication_open_dispatch::operation::take)
            {w::reader in{output};std::uint64_t field{};for(unsigned i{};i<7;++i)assert(in.u64(field));assert(field==1&&in.u64(node)&&node&&in.done());}
            return copied;
        });
    }
};
#ifndef UWVM_COMMUNICATION_NODE_OPEN_FIXTURE_ONLY
static void canonical_node()
{
    pipeline f;assert(f.open(true,47)==s::error::too_large&&!f.request&&!f.native->stage_calls);
    assert(f.open(false)==s::error::io&&f.request&&!f.native->stage_calls&&!f.native->node_creations);
    assert(f.open()==s::error::success);
    assert(f.control(s::communication_open_dispatch::operation::take,true,63)==s::error::too_large&&!f.native->stage_calls);
    f.native->ready=false;assert(f.source->pump()==c::error::again&&f.native->stage_calls==1&&!f.native->node_creations);
    f.native->ready=true;assert(f.source->pump()==c::error::ok&&f.native->node_creations==1);
    s::communication_dispatch::usage usage;assert(f.dispatch->inspect("node",usage)==s::error::success&&!usage.handles);
    assert(f.control(s::communication_open_dispatch::operation::take,false)==s::error::io&&f.node!=f.request);
    auto node=f.node;assert(f.control(s::communication_open_dispatch::operation::take)==s::error::success&&f.node==node);
    assert(f.open()==s::error::success&&f.native->node_creations==1);
    assert(f.control(s::communication_open_dispatch::operation::cancel)==s::error::again);
    assert(f.authority->close(f.who,f.context_cap)==s::error::success);
    assert(f.dispatch->close(f.who,f.cap,f.node)==s::error::success);
    assert(f.control(s::communication_open_dispatch::operation::release)==s::error::again&&f.source->reconcile_cleanup()==c::error::busy);
    f.worker->retire();assert(f.source->reconcile_cleanup()==c::error::ok);
    assert(f.control(s::communication_open_dispatch::operation::release,false)==s::error::io);
    assert(f.control(s::communication_open_dispatch::operation::release)==s::error::success);
    assert(f.control(s::communication_open_dispatch::operation::release)==s::error::success);
}
static void deny_cancel_and_uncertainty()
{
    using op=s::communication_open_dispatch::operation;
    pipeline zero(0);assert(zero.open()==s::error::again&&zero.native->stage_calls==0&&zero.native->node_creations==0);
    pipeline hidden(2,false);assert(hidden.open()==s::error::denied&&hidden.native->stage_calls==0);
    pipeline unsent;assert(unsent.open()==s::error::success&&unsent.control(op::cancel)==s::error::success&&!unsent.worker->stopped);
    assert(unsent.control(op::release)==s::error::success&&!unsent.native->node_creations);
    pipeline pending;assert(pending.open()==s::error::success);pending.native->ready=false;assert(pending.source->pump()==c::error::again);
    assert(pending.control(op::cancel)==s::error::success&&pending.worker->stopped&&pending.control(op::release)==s::error::again);
    pending.worker->retire();assert(pending.source->reconcile_cleanup()==c::error::ok&&pending.control(op::release)==s::error::again);
    pipeline ready;assert(ready.open()==s::error::success&&ready.source->pump()==c::error::ok);
    assert(ready.control(op::cancel)==s::error::success&&ready.worker->stopped);ready.worker->retire();
    assert(ready.source->reconcile_cleanup()==c::error::ok&&ready.control(op::release)==s::error::success);
}
static void reentrant_and_origin_close()
{
    pipeline f;std::size_t required{};assert(f.source->invoke_open(f.who,f.cap,f.carrier,48,required,[&](w::bytes)
        {assert(f.open()==s::error::again);return true;})==s::error::success);
    assert(f.open()==s::error::success&&f.request);
    assert(f.authority->close(f.who,f.context_cap)==s::error::success&&f.source->pump()!=c::error::ok&&!f.native->stage_calls);
    assert(f.control(s::communication_open_dispatch::operation::release)==s::error::success);
    pipeline throwing;unsigned calls{};assert(throwing.source->invoke_open(throwing.who,throwing.cap,throwing.carrier,48,required,[](w::bytes){return true;},
        [&]()->bool{if(++calls==2)throw 1;return false;})==s::error::io);
    assert(throwing.open()==s::error::success);
}
static void concurrent_cancellation()
{
    pipeline f;f.native->ready=false;assert(f.open()==s::error::success);
    std::atomic<bool> running{true},entered{};
    std::thread pumping{[&]{while(running.load())
    {auto status=f.source->pump();assert(status==c::error::again||status==c::error::cancelled||status==c::error::denied);entered=true;std::this_thread::yield();}}};
    while(!entered.load())std::this_thread::yield();
    assert(f.control(s::communication_open_dispatch::operation::cancel)==s::error::success);
    running=false;pumping.join();assert(f.worker->stopped&&!f.native->node_creations);
    assert(f.control(s::communication_open_dispatch::operation::release)==s::error::again);
}
static void startup_registry_reentry()
{
    pipeline f;assert(f.open()==s::error::success);f.native->stage_context=&f;
    f.native->stage_hook=[](void* opaque,c::node_plan const& plan,c::node_stage_request const& request)noexcept
    {
        auto& actual=*static_cast<pipeline*>(opaque);c::context_plan context;
        assert(actual.dispatch->describe_plan("context",context)==s::error::success);
        s::group_lease held;assert(actual.dispatch->admit_node_activation(plan,context,request,held)==s::error::success&&held.size()==2);
        return c::error::ok;
    };
    f.native->create_hook=[](void* opaque,c::node_plan const& plan)noexcept
    {
        auto& actual=*static_cast<pipeline*>(opaque);c::context_plan context;
        assert(actual.dispatch->describe_plan("context",context)==s::error::success);
        c::node_stage_request request{actual.who.instance,actual.who.generation,actual.cap,actual.context_cap,actual.context,
            context.authority,s::buffers::hash(actual.carrier)};
        s::group_lease held;assert(actual.dispatch->admit_node_activation(plan,context,request,held)==s::error::success);
        assert(actual.dispatch->close(actual.who,actual.context_cap,actual.context)==s::error::again);
        return c::error::ok;
    };
    assert(f.source->pump()==c::error::ok&&f.native->node_creations==1);
    pipeline closing;assert(closing.open()==s::error::success);closing.native->stage_context=&closing;
    closing.native->stage_hook=[](void* opaque,c::node_plan const&,c::node_stage_request const&)noexcept
    {
        auto& actual=*static_cast<pipeline*>(opaque);
        assert(actual.dispatch->close(actual.who,actual.context_cap,actual.context)==s::error::success);
        return c::error::ok;
    };
    assert(closing.source->pump()==c::error::cancelled&&closing.worker->stopped&&!closing.native->node_creations);
    assert(closing.control(s::communication_open_dispatch::operation::release)==s::error::again);
}
static void supervisor_exit_receipts()
{
    using op=s::communication_open_dispatch::operation;
    pipeline inert;inert.source->request_stop();
    assert(inert.open()==s::error::cancelled&&!inert.native->stage_calls&&!inert.native->node_creations);
    pipeline queued;assert(queued.open()==s::error::success);queued.source->request_stop();
    assert(queued.control(op::take)==s::error::cancelled&&queued.control(op::describe)==s::error::success);
    assert(queued.control(op::release)==s::error::success&&!queued.native->stage_calls);
    pipeline started;assert(started.open()==s::error::success);started.native->ready=false;
    assert(started.source->pump()==c::error::again);started.source->request_stop();
    assert(started.control(op::take)==s::error::cancelled&&started.control(op::release)==s::error::again);
    started.worker->retire();assert(started.source->reconcile_cleanup()==c::error::ok);
    assert(started.control(op::take)==s::error::cancelled&&started.control(op::release)==s::error::again);
    pipeline ready;assert(ready.open()==s::error::success&&ready.source->pump()==c::error::ok);
    ready.source->request_stop();assert(ready.control(op::take)==s::error::cancelled);
    ready.worker->retire();assert(ready.source->reconcile_cleanup()==c::error::ok&&ready.control(op::release)==s::error::success);
    pipeline racing;assert(racing.open()==s::error::success);racing.native->stage_context=&racing;
    racing.native->stage_hook=[](void* opaque,c::node_plan const&,c::node_stage_request const&)noexcept
    {static_cast<pipeline*>(opaque)->source->request_stop();return c::error::ok;};
    assert(racing.source->pump()==c::error::cancelled&&racing.control(op::take)==s::error::cancelled);
    assert(racing.control(op::release)==s::error::again);
    racing.worker->retire();assert(racing.source->reconcile_cleanup()==c::error::ok);
    assert(racing.control(op::release)==s::error::again); // Reaping does not resolve an unknown effect.
}
static void startup_failure_evidence()
{
    using op=s::communication_open_dispatch::operation;
    {pipeline pending;pending.native->stage_hook=[](void*,c::node_plan const&,c::node_stage_request const&)noexcept{return c::error::busy;};
        assert(pending.open()==s::error::success&&pending.source->pump()==c::error::again&&!pending.worker->stopped&&pending.control(op::take)==s::error::again);}
    for(auto reason:{c::error::io,c::error::denied,c::error::invalid,c::error::timeout,c::error::too_large,c::error::no_memory})
    {
        pipeline f;f.native->stage_context=&reason;
        f.native->stage_hook=[](void* context,c::node_plan const&,c::node_stage_request const&)noexcept{return *static_cast<c::error*>(context);};
        assert(f.open()==s::error::success&&f.source->pump()==reason&&f.worker->stopped);
        assert(f.control(op::take)==static_cast<s::error>(reason));
        s::communication_open_dispatch::observation observed;assert(f.source->inspect(f.who,f.request,observed)==c::error::ok);
        assert(observed.state.receipt.status==opens::state::failed&&observed.state.receipt.result==reason&&observed.state.receipt.cleanup_debt&&observed.state.receipt.observed==c::effect::unknown);
        f.source->request_stop();assert(f.control(op::take)==static_cast<s::error>(reason));
        f.worker->retire();assert(f.source->reconcile_cleanup()==c::error::ok&&f.control(op::release)==s::error::again);
    }
}
int main(){startup_failure_evidence();canonical_node();deny_cancel_and_uncertainty();reentrant_and_origin_close();concurrent_cancellation();startup_registry_reentry();supervisor_exit_receipts();
    std::puts("PASS asynchronous node coordinator: inert admission, bounded startup, exact registry evidence, separate publication, recovery, zero authority, independent cleanup and persistent unknown debt");return 0;}
#endif
