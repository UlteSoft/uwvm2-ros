/** @file
 * @brief Graph dispatcher provenance, exact pagination recovery and retirement tests.
 * @details Mock captures exercise admission decisions; actual native discovery
 * interoperability remains covered by the separate native graph qualification.
 */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
#include <uwvm2/imported/ros/scene/communication_graph_dispatch.h>
#include <uwvm2/imported/ros/scene/communication_jobs.h>
#include <uwvm2/imported/ros/scene/communication_event_dispatch.h>
namespace graph=c::graph;
namespace gc=uwvm2::imported::ros::graph_contract;
struct graph_native final:graph::source
{
    c::node_lease node;std::atomic<unsigned> captures{},unscoped{};c::operation_token last;
    explicit graph_native(c::node_lease held):node(std::move(held)){}
    bool live() const noexcept override{return node.live();}
    c::handle context() const noexcept override{return node.context();}
    c::owner authority() const noexcept override{return node.authority();}
    c::error capture(graph::plan const&,uwvm2::utils::container::std_vector<graph::entity>&) noexcept override
    {++unscoped;return c::error::denied;}
    c::error capture(graph::plan const& plan,uwvm2::utils::container::std_vector<graph::entity>& out,c::operation_token const& token) noexcept override
    {
        assert(token.valid(token.action,c::operation_family::graph,plan.authority,token.resource,false)&&token.origins[2].resource==plan.context);
        last=token;
        try{++captures;out.push_back({graph::kind::node,"/scene/robot",{}, {}});
            out.push_back({graph::kind::publisher,"/scene/robot","/scene/data","test/Request"});return c::error::ok;}
        catch(...){return c::error::no_memory;}
    }
};
static void graph_permissions(s::authority& permissions,std::optional<std::uint64_t> calls={})
{
    for(auto const& item:gc::operations)assert(permissions.expose(text(item.name),s::exposure::allow)==error::success);
    s::grant grant;grant.alias="graph";grant.resource="graph";grant.kind=s::purpose::discovery;grant.binding="native-graph";grant.max_resources=1;
    if(calls){grant.limits.calls=calls;grant.limits.uncertain=true;grant.limits.window=std::chrono::hours{1};}
    for(auto action:{"graph.snapshot","graph.watch","resource.observe","resource.close"})grant.actions.emplace(action);
    assert(permissions.configure(std::move(grant))==error::success);
    for(auto const& item:uwvm2::imported::ros::event_contract::operations)assert(permissions.expose(text(item.name),s::exposure::allow)==error::success);
    grant={};grant.alias="events";grant.resource="events";grant.kind=s::purpose::event;grant.binding="event-stream";grant.max_resources=1;
    for(auto action:{"event.stream.open","event.stream.poll","event.stream.close"})grant.actions.emplace(action);
    assert(permissions.configure(std::move(grant))==error::success);
}
struct graph_fixture
{
    fixture origin;
    s::communication_graph_dispatch dispatch{origin.authority,origin.dispatch,111,3,{1,4096,4096,32768,8}};
    std::shared_ptr<graph_native> native;
    s::handle cap{};
    explicit graph_fixture(bool prepared=true,std::optional<std::uint64_t> calls={}):
        origin{true,true,[&](s::authority& permissions){graph_permissions(permissions,calls);}}
    {
        s::communication_graph_dispatch::plan policy;
        policy.context_profile=profile("ctx");policy.node_profile=profile("node");policy.policy.selected=profile("graph");
        policy.policy.nodes={"/scene/robot"};policy.policy.fields=63;policy.policy.max_rows=2;policy.policy.max_bytes=1024;
        policy.policy.max_page_items=1;policy.policy.cursor_lifetime_ns=10000000000ULL;policy.policy.capture_deadline_ns=10000000000ULL;
        assert(dispatch.bind("graph",std::move(policy))==error::success);
        if(prepared)assert(dispatch.prepare({this,[](void* argument,graph::plan const&,c::node_lease held,std::shared_ptr<graph::source>& out) noexcept
            {try{auto& self=*static_cast<graph_fixture*>(argument);self.native=uwvm2::utils::container::make_shared<graph_native>(std::move(held));out=self.native;return c::error::ok;}
             catch(...){return c::error::no_memory;}}})==error::success);
        assert(origin.authority->open(origin.who,"graph",cap)==error::success);
    }
    bytes request(gc::operation action,std::optional<s::handle> cursor={},s::handle nodecap=0,s::handle context=0)
    {
        return encode([&](auto& out){return out.u64(origin.context_cap)&&out.u64(context?context:origin.context_id)&&out.u64(nodecap?nodecap:origin.node_cap)&&out.u64(origin.node_id)&&
            contract::encode_profile(out,profile("graph"))&&(action==gc::operation::watch||(out.boolean(cursor.has_value())&&(!cursor||out.u64(*cursor))&&out.u64(1)));});
    }
    error call(gc::operation action,bytes& out,std::optional<s::handle> cursor={},std::size_t capacity=4096,s::communication_graph_dispatch::sink sink={})
    {
        auto input=request(action,cursor);std::size_t required{};auto copy=[&](w::bytes data){out.assign(data.begin(),data.end());return true;};
        auto status=dispatch.invoke(origin.who,action,cap,input,capacity,required,sink?sink:s::communication_graph_dispatch::sink{copy});
        if(status==error::success){auto* definition=schema::find(gc::operations[static_cast<std::size_t>(action)].name);assert(definition&&schema::validate(out,definition->result,{4096,256,64,32}));}
        return status;
    }
};
#ifndef UWVM_COMMUNICATION_GRAPH_FIXTURE_ONLY
static s::handle watch(graph_fixture& f)
{bytes out;assert(f.call(gc::operation::watch,out)==error::success);w::reader reader{out};s::handle value{};assert(reader.u64(value)&&reader.done());return value;}
static std::optional<s::handle> cursor(bytes const& value)
{w::reader reader{value};s::handle revision{},id{};bool present{};assert(reader.u64(revision)&&revision&&reader.boolean(present));if(present){assert(reader.u64(id)&&id);return id;}return {};}
static void graph_authority()
{
    { graph_fixture cancelled;auto input=cancelled.request(gc::operation::watch);auto action=gc::operation::watch;std::size_t required{};auto stop=[]{return true;};auto copy=[](w::bytes){return true;};
       assert(cancelled.dispatch.invoke(cancelled.origin.who,action,cancelled.cap,input,4096,required,copy,stop)==error::cancelled&&!cancelled.native); }
    graph_fixture inert(false);bytes out;assert(inert.call(gc::operation::watch,out)==error::denied&&!inert.native);
    graph_fixture f;auto calls=f.origin.admissions("graph");assert(f.call(gc::operation::watch,out,{},7)==error::too_large&&!f.native&&f.origin.admissions("graph")==calls);
    auto wrong=f.request(gc::operation::snapshot,{},f.origin.context_cap);std::size_t required{};auto copy=[](w::bytes){return true;};
    assert(f.dispatch.invoke(f.origin.who,gc::operation::snapshot,f.cap,wrong,4096,required,copy)==error::denied&&!f.native);
    wrong=f.request(gc::operation::snapshot,{},0,f.origin.node_id);assert(f.dispatch.invoke(f.origin.who,gc::operation::snapshot,f.cap,wrong,4096,required,copy)!=error::success&&!f.native);
    auto id=watch(f);assert(f.native&&f.origin.admissions("context")==1&&f.origin.admissions("node")==1);
    assert(f.origin.dispatch.close(f.origin.who,f.origin.node_cap,f.origin.node_id)==error::again);
    s::events::binding source;assert(f.dispatch.event_source(f.origin.who,f.cap,id,source)==error::success);
    assert(source.origin.environment==111&&source.origin.generation==3&&source.origin.grant&&source.resource==id);
    auto forged=source;forged.origin.grant++;assert(f.dispatch.validate_source(f.origin.who,f.cap,forged)==error::denied);
    assert(f.dispatch.close(f.origin.who,f.cap,id)==error::success);
    assert(f.dispatch.validate_source(f.origin.who,f.cap,source)==error::bad_handle);
    assert(watch(f)!=id);
}
static void graph_recovery()
{
    graph_fixture f;bytes out,failed;auto fail=[&](w::bytes value){failed.assign(value.begin(),value.end());return false;};
    auto charged=f.origin.admissions("graph");assert(f.call(gc::operation::snapshot,out,{},23)==error::too_large&&f.origin.admissions("graph")==charged&&!f.native);
    assert(f.call(gc::operation::snapshot,out,{},24)==error::too_large&&f.origin.admissions("graph")==charged&&f.native->captures==1);
    assert(f.call(gc::operation::snapshot,out,{},4096,fail)==error::io&&f.native->captures==1);
    assert(f.call(gc::operation::snapshot,out)==error::success&&out==failed&&f.native->captures==1);
    auto next=cursor(out);assert(next);
    assert(f.call(gc::operation::snapshot,out,next)==error::success&&!cursor(out)&&f.native->captures==1);
    assert(f.call(gc::operation::snapshot,out,next)==error::conflict);
    assert(f.origin.admissions("context")==1&&f.origin.admissions("node")==1);
    assert(f.call(gc::operation::snapshot,out)==error::success&&f.native->captures==2);
}
static void graph_visibility()
{
    graph_fixture f;bytes out;auto id=watch(f);s::events::binding source;assert(f.dispatch.event_source(f.origin.who,f.cap,id,source)==error::success);
    s::events::snapshot state;assert(source.provider->inspect(source.origin,id,source.generation,state)==c::error::ok&&!state.ready&&!f.native->captures&&!f.native->unscoped);
    assert(f.dispatch.observe_source(f.origin.who,f.cap,source,state)==c::error::ok&&state.ready&&f.native->captures==1&&!f.native->unscoped);
    auto observed=f.native->last;assert(observed.action==c::permission::resource_observe&&observed.watch==id&&observed.resource==observed.authority.grant);
    auto replaced=observed;++replaced.watch;
    auto actual=std::static_pointer_cast<graph::watch>(source.provider);
    assert(actual->inspect(source.origin,id,source.generation,state,&replaced)==c::error::denied&&f.native->captures==1);
    {s::group_lease held;assert(f.dispatch.revalidate(observed,held)==error::success);
        s::communication_jobs::claim_set claims;assert(held.matches(f.origin.who,s::communication_jobs::admission_claims(observed,claims)));}
    assert(f.origin.authority->close(f.origin.who,f.origin.context_cap)==error::success);
    assert(f.dispatch.validate_source(f.origin.who,f.cap,source)==error::bad_handle);
    assert(f.call(gc::operation::snapshot,out)==error::bad_handle);
    {s::group_lease held;assert(f.dispatch.revalidate(observed,held)!=error::success);}
    graph_fixture reentrant;auto revoke=[&](w::bytes){reentrant.dispatch.revoke("graph");return true;};
    assert(reentrant.call(gc::operation::watch,out,{},4096,revoke)==error::cancelled);
    s::communication_graph_dispatch::usage usage;assert(reentrant.dispatch.inspect("graph",usage)==error::success&&usage.watches==0);
}
static void graph_current_origins()
{
    graph_fixture f;auto original_context=f.origin.context_cap;
    assert(f.origin.authority->open(f.origin.who,"context",f.origin.context_cap)==error::success&&f.origin.context_cap!=original_context);
    bytes out;assert(f.call(gc::operation::snapshot,out)==error::success);
    auto token=f.native->last;
    assert(token.count==4&&token.capability==f.cap&&token.resource==token.authority.grant&&token.caller_instance==f.origin.who.instance);
    assert(token.origins[1].capability==f.origin.node_cap&&token.origins[2].capability==original_context&&token.origins[3].capability==f.origin.context_cap);
    assert(token.origins[2].authority==token.origins[3].authority&&token.origins[2].resource==token.origins[3].resource);
    {s::group_lease held;assert(f.dispatch.revalidate(token,held)==error::success);
        s::communication_jobs::claim_set claims;assert(held.matches(f.origin.who,s::communication_jobs::admission_claims(token,claims)));}
    auto denied=[&](c::operation_token changed){s::group_lease held;assert(f.dispatch.revalidate(changed,held)!=error::success&&!held);};
    for(std::size_t i{};i<token.count;++i)
    {
        auto changed=token;++changed.origins[i].authority.generation;denied(changed);
        changed=token;++changed.origins[i].resource;denied(changed);
        changed=token;++changed.origins[i].capability;denied(changed);
    }
    auto changed=token;changed.origins[2].capability=f.origin.context_cap;denied(changed);
    changed=token;changed.origins[3].capability=original_context;denied(changed);
    changed=token;changed.count=3;denied(changed);changed=token;changed.origins[4]=token.origins[1];denied(changed);
    changed=token;changed.retained_origins=uwvm2::utils::container::make_shared<unsigned>(1);denied(changed);
    changed=token;++changed.caller_generation;denied(changed);
    // A second capability is the actual primary, not the first observation's opener.
    auto old=f.cap;assert(f.origin.authority->open(f.origin.who,"graph",f.cap)==error::success&&f.cap!=old);
    assert(f.call(gc::operation::snapshot,out,cursor(out))==error::success);
    assert(f.call(gc::operation::snapshot,out)==error::success&&f.native->last.capability==f.cap);
    auto fresh=f.native->last;assert(f.origin.authority->close(f.origin.who,old)==error::success);denied(token);
    {s::group_lease held;assert(f.dispatch.revalidate(fresh,held)==error::success);}
    // Closing the node's original context still denies an explicitly live describe capability.
    assert(f.origin.authority->close(f.origin.who,original_context)==error::success);denied(fresh);
    auto captures=f.native->captures.load();assert(f.call(gc::operation::snapshot,out)!=error::success&&f.native->captures==captures);
    graph_fixture watching;auto id=watch(watching);s::events::binding source;assert(watching.dispatch.event_source(watching.origin.who,watching.cap,id,source)==error::success);
    s::events::snapshot state;assert(watching.dispatch.observe_source(watching.origin.who,watching.cap,source,state)==c::error::ok);auto before=watching.native->last;
    assert(watching.dispatch.close(watching.origin.who,watching.cap,id)==error::success);assert(watch(watching)!=id);
    s::group_lease held;assert(watching.dispatch.revalidate(before,held)!=error::success&&!held);
}
static void graph_event_observation()
{
    namespace ec=uwvm2::imported::ros::event_contract;
    graph_fixture f;auto watched=watch(f);s::handle source_cap{},event_cap{},stream{};
    assert(f.origin.authority->open(f.origin.who,"graph",source_cap)==error::success&&source_cap!=f.cap);
    assert(f.origin.authority->open(f.origin.who,"events",event_cap)==error::success);
    s::communication_event_dispatch events{f.origin.authority,111,3,{1,4096,4096,16384,4}};
    assert(events.bind("events",{{{},profile("events"),1,2,3,10000000000ULL},1,4096,4096})==error::success);
    assert(events.prepare(s::communication_event_dispatch::source_access::from(f.dispatch))==error::success);
    auto open=encode([&](auto& out){return out.u64(1)&&out.u64(source_cap)&&out.u64(watched)&&contract::encode_profile(out,profile("events"))&&out.u64(4096)&&out.u64(2)&&out.u64(4096);});
    bytes out,failed;std::size_t required{};auto copy=[&](w::bytes value){out.assign(value.begin(),value.end());return true;};
    assert(events.invoke(f.origin.who,ec::operation::open,event_cap,open,4096,required,copy)==error::success);
    {w::reader reader{out};assert(reader.u64(stream)&&reader.done());}
    assert(f.native->captures==1&&!f.native->unscoped&&f.native->last.capability==source_cap&&f.native->last.watch==watched);
    auto poll=encode([&](auto& writer){return writer.u64(stream)&&writer.u64(2)&&writer.u64(c::now()+1000000000ULL);});
    auto fail=[&](w::bytes value){failed.assign(value.begin(),value.end());return false;};
    assert(events.invoke(f.origin.who,ec::operation::poll,event_cap,poll,4096,required,fail)==error::io);
    assert(events.invoke(f.origin.who,ec::operation::poll,event_cap,poll,4096,required,copy)==error::success&&out==failed);
    assert(f.native->captures==3&&f.native->last.capability==source_cap&&!f.native->unscoped);
    assert(f.origin.authority->close(f.origin.who,f.cap)==error::success);
    // Readiness is level-triggered until graph.snapshot acknowledges the revision.
    assert(events.invoke(f.origin.who,ec::operation::poll,event_cap,poll,4096,required,copy)==error::success&&f.native->captures==4);
    assert(f.origin.authority->close(f.origin.who,source_cap)==error::success);
    assert(events.invoke(f.origin.who,ec::operation::poll,event_cap,poll,4096,required,copy)==error::bad_handle&&f.native->captures==4);
    // Closing a stream does not need the revoked origin to grant observation again.
    assert(events.invoke(f.origin.who,ec::operation::close,event_cap,resource(stream),4096,required,copy)==error::success);

    graph_fixture retiring;auto id=watch(retiring);s::events::binding source;
    assert(retiring.dispatch.event_source(retiring.origin.who,retiring.cap,id,source)==error::success);
    s::events::stream local;
    auto admit=[](void*,c::owner,c::handle)noexcept{return true;};
    auto retire=[](void* pointer,s::events::binding const& bound,s::events::snapshot& value)noexcept
    {static_cast<s::events::stream*>(pointer)->retire();return bound.provider->inspect(bound.origin,bound.resource,bound.generation,value);};
    // Native/provider callbacks must be able to retire a stream during setup.
    assert(local.configure({{111,999,3},profile("retire"),1,1,3,1000000000ULL},{&source,1},&local,admit,retire)==c::error::cancelled);
}
static void graph_fork_and_capacity()
{
    graph_fixture f;bytes out;auto throwing=[](w::bytes)->bool{throw 1;};assert(f.call(gc::operation::watch,out,{},4096,throwing)==error::io);
    auto id=watch(f);assert(id);
    s::communication_graph_dispatch::workspace_lease token;assert(f.dispatch.reserve_workspace(f.origin.who,gc::operation::snapshot,f.cap,4096,token)==error::success);
    auto pid=::fork();assert(pid>=0);if(!pid){token.reset();s::communication_graph_dispatch::usage usage;assert(f.dispatch.inspect({},usage)==error::denied);f.dispatch.retire();::_exit(0);}
    int result{};assert(::waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&!WEXITSTATUS(result));token.reset();
    s::communication_graph_dispatch::usage usage;assert(f.dispatch.inspect({},usage)==error::success&&!usage.workspace_calls&&usage.watches==1);
    f.dispatch.retire();assert(f.call(gc::operation::snapshot,out)==error::denied);
}
static void graph_cancellation_reentrancy()
{
    for(bool local:{false,true})for(auto alias:{"context","node","graph"})for(unsigned phase:{1u,2u,3u})
    {
        graph_fixture f;unsigned callbacks{},copies{};std::size_t required{};
        auto retire=[&](auto name){if(name==std::string_view{"graph"})f.dispatch.revoke(name);else f.origin.dispatch.revoke(name);};
        auto cancel=[&]{if(++callbacks==phase){if(local)retire(alias);else assert(f.origin.authority->revoke(alias,retire)==error::success);}return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.request(gc::operation::snapshot);
        assert(f.dispatch.invoke(f.origin.who,gc::operation::snapshot,f.cap,input,4096,required,copy,cancel)!=error::success&&callbacks>=phase&&!copies);
        if(phase<=2)assert(!f.native||!f.native->captures);
        else assert(f.native&&f.native->captures==1);
    }
}
int main(){graph_cancellation_reentrancy();graph_authority();graph_recovery();graph_visibility();graph_fork_and_capacity();graph_current_origins();graph_event_observation();std::cout<<"passed 6 graph dispatch security groups\n";}

#endif
