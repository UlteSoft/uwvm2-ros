/** @file
 * @brief Action dispatcher origin, recovery, secondary transition and cleanup tests.
 */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
#include <uwvm2/imported/ros/scene/communication_action_dispatch.h>
#include <uwvm2/imported/ros/scene/communication_jobs.h>
namespace ac=uwvm2::imported::ros::action_contract;
namespace actions=c::actions;
static void action_permissions(s::authority& permissions,bool transitions,std::optional<std::uint64_t> calls={})
{
    for(auto const& item:ac::operations)assert(permissions.expose(text(item.name),s::exposure::allow)==error::success);
    s::grant grant;grant.alias="action";grant.resource="action";grant.kind=s::purpose::action;grant.binding="native-action";grant.max_resources=1;
    grant.limits.calls=calls;grant.limits.uncertain=calls.has_value();grant.limits.window=std::chrono::seconds{60};
    for(auto const& item:ac::operations)grant.actions.emplace(item.name);
    for(auto action:{"resource.observe","resource.close"})grant.actions.emplace(action);
    if(transitions)for(auto action:ac::transition_actions)grant.actions.emplace(action);
    assert(permissions.configure(std::move(grant))==error::success);
}
struct action_native final:actions::native
{
    uwvm2::utils::container::std_unique_ptr<s::communication_worker::mutations> queued;
    bool queued_mutations()const noexcept override{return bool(queued);}
    c::node_lease node;std::atomic<unsigned> calls{},reject_after{UINT_MAX};std::atomic<bool> active{true},unknown{},reject{};
    std::mutex token_mutex;c::operation_token last_operation;
    explicit action_native(c::node_lease value):node(std::move(value)){}
    bool live() const noexcept override{return active.load()&&node.live();}
    c::native_result effect() noexcept
    {if(reject||calls>=reject_after)return {c::error::again,c::effect::none};++calls;return {unknown?c::error::io:c::error::ok,unknown?c::effect::unknown:c::effect::locally_accepted};}
    c::native_result submit(c::handle,actions::uuid const&,w::bytes,std::uint64_t) noexcept override{return effect();}
    c::native_result respond(actions::response const&,actions::uuid const&,actions::state,std::uint64_t) noexcept override{return effect();}
    c::native_result cancel(c::handle,c::handle,actions::uuid const&,std::uint64_t) noexcept override{return effect();}
    void retain(c::operation_token const& token)noexcept
    {assert(token.valid(token.action,c::operation_family::action,token.authority,token.resource,false));std::lock_guard lock{token_mutex};last_operation=token;}
    c::native_result submit(c::handle,actions::uuid const& uuid,w::bytes body,std::uint64_t deadline,c::operation_token const& token)noexcept override
    {retain(token);return queued?queued->action_submit(token,uuid,body,deadline):effect();}
    c::native_result respond(actions::response const& value,actions::uuid const&,actions::state,std::uint64_t deadline,c::operation_token const& token)noexcept override
    {retain(token);return queued?queued->action_respond(token,value.request,value.goal,static_cast<std::uint64_t>(value.change),value.body,value.accept_cancel,deadline):effect();}
    c::native_result cancel(c::handle goal,c::handle,actions::uuid const&,std::uint64_t deadline,c::operation_token const& token)noexcept override
    {retain(token);return queued?queued->action_cancel(token,goal,deadline):effect();}
    c::error poll(c::operation_token const& token)noexcept override{retain(token);return c::error::ok;}
    void retire() noexcept override{active=false;}
};
struct action_fixture
{
    fixture origin;
    s::communication_action_dispatch dispatch;
    std::shared_ptr<action_native> native;
    std::shared_ptr<actions::endpoint> endpoint;
    c::owner authority;
    s::handle cap{},id{};
    actions::role role;
    bool startup_goal{},startup_failure{},queue_mutations{};
    explicit action_fixture(actions::role direction=actions::role::client,bool transitions=true,bool prepare=true,std::optional<std::uint64_t> calls={}):
        origin(true,true,[&](s::authority& p){action_permissions(p,transitions,calls);}),dispatch(origin.authority,origin.dispatch,111,3,{1,4096,4096,32768,8}),role(direction)
    {
        s::communication_action_dispatch::plan selected;selected.node_profile=profile("node");auto& p=selected.policy;
        p.selected=profile("action");p.resolved_name="/scene/action";p.action_type="test/Action";p.direction=direction;
        p.goal_quality=p.result_quality=p.cancel_quality=p.feedback_quality=p.status_quality=quality();
        p.goal={profile("request"),"test/Request","scene-canonical",{1,2,3},valid_body,{}};
        p.feedback=p.goal;p.result={profile("response"),"test/Response","scene-canonical",{1,2,3},valid_body,{}};
        p.max_goals=2;p.max_cancels_per_goal=2;p.feedback_slots=1;p.max_goal_bytes=p.max_feedback_bytes=p.max_result_bytes=64;
        p.reserved_bytes=4096;p.transition_mask=510;p.max_deadline_ns=10000000000ULL;p.retention_ns=1000000000ULL;
        assert(dispatch.bind("action",std::move(selected))==error::success);
        if(prepare)assert(dispatch.prepare({this,[](void* p,actions::plan const& policy,c::node_lease node,s::handle id,std::shared_ptr<actions::endpoint>& out) noexcept
        {
            auto& self=*static_cast<action_fixture*>(p);
            try
            {
                self.native=uwvm2::utils::container::make_shared<action_native>(std::move(node));self.endpoint=uwvm2::utils::container::make_shared<actions::endpoint>();self.authority=policy.authority;
                if(self.queue_mutations)
                {
                    self.native->queued=uwvm2::utils::container::make_unique<s::communication_worker::mutations>();
                    s::communication_jobs::plan capacity{policy.authority,c::operation_family::action,id,1,71,2,256,256,10000000000,1280};
                    auto check=[](void* object,c::operation_token const& token,s::group_lease& held)noexcept
                    {return static_cast<c::error>(static_cast<s::communication_action_dispatch*>(object)->revalidate(token,held));};
                    auto charge=[](void* object,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
                    {return static_cast<c::error>(static_cast<s::communication_action_dispatch*>(object)->charge_operation(token,bytes,held));};
                    auto ready=self.native->queued->configure(capacity,check,charge,&self.dispatch);if(ready!=c::error::ok)return ready;
                }
                auto result=self.endpoint->configure(policy,id,self.native);if(result!=c::error::ok)return result;out=self.endpoint;
                assert(policy.goal.authority.environment==111&&policy.goal.authority.generation==3&&policy.goal.authority!=policy.authority);
                if(self.startup_goal){actions::uuid uuid{};uuid[0]=9;std::array<std::uint8_t,1> body{1};s::handle goal{};assert(self.endpoint->offer(uuid,1,body,goal)==c::error::ok);}
                return self.startup_failure?c::error::io:c::error::ok;
            }
            catch(...){return c::error::no_memory;}
        }})==error::success);
        assert(origin.authority->open(origin.who,"action",cap)==error::success);
    }
    bytes open(s::handle schema=0,std::string_view selected_role={})
    {
        return encode([&](auto& out){return out.u64(origin.node_cap)&&out.u64(origin.node_id)&&out.u64(3)&&out.u64(schema?schema:origin.request_cap)&&out.u64(origin.request_cap)&&out.u64(origin.response_cap)&&
            contract::encode_profile(out,profile("action"))&&out.text("/scene/action")&&out.text("test/Action")&&out.text(selected_role.empty()?(role==actions::role::client?"client":"server"):selected_role);});
    }
    bytes submit(std::uint64_t sequence,std::uint64_t deadline,std::uint8_t value=1)
    {return encode([&](auto& out){actions::uuid uuid{};uuid[0]=1;return out.u64(id)&&out.u64(sequence)&&out.u64(id)&&out.blob(uuid)&&contract::encode_message(out,{"test/Request","scene-canonical",{&value,1}})&&out.u64(deadline);});}
    bytes respond(std::uint64_t sequence,std::uint64_t request,std::uint64_t goal,std::string_view change,std::uint64_t deadline,bool message=false)
    {return encode([&](auto& out){std::uint8_t body=1;return out.u64(id)&&out.u64(sequence)&&out.u64(id)&&out.u64(request)&&out.u64(goal)&&out.text(change)&&out.boolean(message)&&
        (!message||contract::encode_message(out,{"test/Response","scene-canonical",{&body,1}}))&&out.u64(0)&&out.u64(deadline);});}
    error call(ac::operation action,w::bytes input,bytes& out,std::size_t capacity=4096,s::communication_action_dispatch::sink sink={})
    {
        std::size_t required{};auto copy=[&](w::bytes value){out.assign(value.begin(),value.end());return true;};
        auto result=dispatch.invoke(origin.who,action,cap,input,capacity,required,sink?sink:s::communication_action_dispatch::sink{copy});
        if(result==error::success)
        {
            // Immediate trusted core creation is not the public asynchronous ABI.
            if(action==ac::operation::open){w::reader fields{out};s::handle resource{};assert(fields.u64(resource)&&resource&&fields.done());}
            else{auto* definition=schema::find(ac::operations[static_cast<std::size_t>(action)].name);assert(definition&&schema::validate(out,definition->result,{4096,256,64,32}));}
        }
        return result;
    }
    void opened(){bytes out;assert(call(ac::operation::open,open(),out)==error::success);w::reader reader{out};assert(reader.u64(id)&&reader.done());}
};
#ifndef UWVM_COMMUNICATION_ACTION_FIXTURE_ONLY
static void queued_transition_accounting()
{
    for(auto budget:{3ULL,4ULL})
    {
        action_fixture f{actions::role::server,true,true,budget};f.queue_mutations=true;f.opened();bytes out;
        actions::uuid uuid{};uuid[0]=5;std::array<std::uint8_t,1> body{1};c::handle goal{};
        assert(f.endpoint->offer(uuid,91,body,goal)==c::error::ok&&f.call(ac::operation::take,resource(f.id),out)==error::success);
        auto input=f.respond(1,91,goal,"accept",c::now()+1000000000ULL);auto before=f.origin.admissions("action");assert(before==2);
        auto result=f.call(ac::operation::respond,input,out);assert(result==(budget==4?error::success:error::again));
        assert(f.origin.admissions("action")== (budget==4?4:2));
        actions::goal_info info;assert(f.endpoint->describe(f.authority,goal,info)==c::error::ok&&info.observed==actions::state::pending);
        if(budget==4)
        {
            auto receipt=out;assert(f.call(ac::operation::respond,input,out)==error::success&&out==receipt&&f.origin.admissions("action")==4);
            auto token=f.native->last_operation;s::communication_jobs::receipt pending;
            assert(f.native->queued->inspect(token,pending)==c::error::ok&&pending.phase==s::communication_jobs::state::queued);
            auto changed=f.respond(1,91,goal,"reject",c::now()+1000000000ULL);
            assert(f.call(ac::operation::respond,changed,out)==error::conflict&&f.origin.admissions("action")==4);
        }
    }
}
static void action_origins()
{
    { action_fixture cancelled;auto input=cancelled.open();auto action=ac::operation::open;std::size_t required{};auto stop=[]{return true;};auto copy=[](w::bytes){return true;};
       assert(cancelled.dispatch.invoke(cancelled.origin.who,action,cancelled.cap,input,4096,required,copy,stop)==error::cancelled&&!cancelled.native); }
    action_fixture inert(actions::role::client,true,false);bytes out;assert(inert.call(ac::operation::open,inert.open(),out)==error::denied&&!inert.native);
    action_fixture f;auto admissions=f.origin.admissions("action");assert(f.call(ac::operation::open,f.open(),out,7)==error::too_large&&!f.native&&f.origin.admissions("action")==admissions);
    assert(f.call(ac::operation::open,f.open(f.origin.response_cap),out)==error::denied&&!f.native);
    assert(f.call(ac::operation::open,f.open(0,"server"),out)==error::denied&&!f.native);
    auto fail=[](w::bytes){return false;};assert(f.call(ac::operation::open,f.open(),out,4096,fail)==error::io&&!f.native->active);
    s::communication_action_dispatch::usage usage;assert(f.dispatch.inspect({},usage)==error::success&&!usage.handles&&!usage.fenced);
    f.opened();assert(f.origin.admissions("context")==1&&f.origin.admissions("node")==1);
    s::events::binding source;assert(f.dispatch.event_source(f.origin.who,f.cap,f.id,source)==error::success);s::events::snapshot state;
    assert(source.provider->inspect(source.origin,source.resource,source.generation,state)==c::error::ok&&!state.ready);
    auto forged=source;forged.origin.grant++;assert(f.dispatch.validate_source(f.origin.who,f.cap,forged)==error::denied);
    assert(f.dispatch.close(f.origin.who,f.cap,f.id)==error::success&&f.dispatch.validate_source(f.origin.who,f.cap,source)==error::bad_handle);
}
static void action_client_recovery()
{
    action_fixture f;f.opened();bytes out,failed;auto deadline=c::now()+1000000000ULL;auto request=f.submit(1,deadline);
    assert(f.call(ac::operation::submit,request,out,159)==error::too_large&&!f.native->calls);
    auto fail=[&](w::bytes value){failed.assign(value.begin(),value.end());return false;};assert(f.call(ac::operation::submit,request,out,4096,fail)==error::io&&f.native->calls==1);
    assert(f.origin.authority->close(f.origin.who,f.origin.node_cap)==error::success);
    assert(f.call(ac::operation::submit,request,out)==error::success&&out==failed&&f.native->calls==1);
    assert(f.call(ac::operation::submit,f.submit(1,deadline,2),out)==error::conflict&&f.native->calls==1);
    assert(f.call(ac::operation::submit,f.submit(2,deadline),out)==error::bad_handle);
    action_fixture unknown;unknown.opened();unknown.native->unknown=true;assert(unknown.call(ac::operation::submit,unknown.submit(1,deadline),out)==error::success);
    assert(unknown.call(ac::operation::submit,unknown.submit(2,deadline),out)==error::again&&unknown.native->calls==1);
    assert(unknown.dispatch.close(unknown.origin.who,unknown.cap,unknown.id)==error::again);
    unknown.dispatch.revoke("action");assert(unknown.endpoint->cleanup_debt());s::communication_action_dispatch::usage usage;
    assert(unknown.dispatch.inspect({},usage)==error::success&&usage.fenced==1);
    action_fixture startup(actions::role::server);startup.startup_goal=startup.startup_failure=true;
    assert(startup.call(ac::operation::open,startup.open(),out)==error::io&&startup.endpoint->cleanup_debt());
    assert(startup.dispatch.inspect({},usage)==error::success&&usage.fenced==1&&!usage.handles);
    assert(startup.call(ac::operation::open,startup.open(),out)==error::again);
}
static s::handle proposal(action_fixture& f)
{actions::uuid uuid{};uuid[0]=7;std::array<std::uint8_t,1> body{1};s::handle goal{};assert(f.endpoint->offer(uuid,91,body,goal)==c::error::ok);return goal;}
static void action_server_transitions()
{
    action_fixture denied(actions::role::server,false);denied.opened();auto goal=proposal(denied);bytes out;
    assert(denied.call(ac::operation::take,resource(denied.id),out)==error::success);
    assert(denied.call(ac::operation::respond,denied.respond(1,91,goal,"accept",c::now()+1000000000ULL),out)==error::denied&&!denied.native->calls);
    action_fixture f(actions::role::server);f.opened();goal=proposal(f);bytes failed;auto fail=[&](w::bytes value){failed.assign(value.begin(),value.end());return false;};
    auto admissions=f.origin.admissions("action");assert(f.call(ac::operation::take,resource(f.id),out,1)==error::too_large&&f.origin.admissions("action")==admissions);
    assert(f.call(ac::operation::take,resource(f.id),out,4096,fail)==error::io);
    assert(f.call(ac::operation::take,resource(f.id),out)==error::success&&out==failed);
    auto deadline=c::now()+1000000000ULL;auto accepted=f.respond(1,91,goal,"accept",deadline);
    assert(f.call(ac::operation::respond,f.respond(1,91,goal,"accept",deadline,true),out)==error::invalid&&!f.native->calls);
    assert(f.call(ac::operation::respond,accepted,out,4096,fail)==error::io&&f.native->calls==1);
    assert(f.call(ac::operation::respond,accepted,out)==error::success&&out==failed&&f.native->calls==1);
    assert(f.call(ac::operation::respond,f.respond(2,91,goal,"execute",deadline),out)==error::success&&f.native->calls==2);
    assert(f.call(ac::operation::respond,f.respond(3,91,goal,"succeed",deadline,true),out)==error::success&&f.native->calls==3);
    auto described=encode([&](auto& writer){return writer.u64(f.id)&&writer.boolean(true)&&writer.u64(goal);});
    assert(f.call(ac::operation::describe,described,out)==error::success);
    assert(f.dispatch.close(f.origin.who,f.cap,f.id)==error::success);
}
static void action_pre_effect_and_sources()
{
    action_fixture f;f.opened();bytes out;auto deadline=c::now()+1000000000ULL;f.native->reject=true;
    assert(f.call(ac::operation::submit,f.submit(1,deadline),out)==error::again&&!f.native->calls);
    f.native->reject=false;assert(f.call(ac::operation::submit,f.submit(1,deadline),out)==error::success&&f.native->calls==1);
    s::communication_action_dispatch::workspace_lease token;assert(f.dispatch.reserve_workspace(f.origin.who,ac::operation::describe,f.cap,100,token)==error::success);
    auto pid=::fork();assert(pid>=0);if(!pid){token.reset();s::communication_action_dispatch::usage usage;assert(f.dispatch.inspect({},usage)==error::denied);f.dispatch.retire();::_exit(0);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));token.reset();
    action_fixture reentrant(actions::role::server);reentrant.opened();proposal(reentrant);
    auto revoke=[&](w::bytes){assert(reentrant.origin.authority->revoke("request",[&](auto alias){reentrant.origin.dispatch.revoke(alias);})==error::success);return true;};
    assert(reentrant.call(ac::operation::take,resource(reentrant.id),out,4096,revoke)==error::denied);
}
static void action_cancellation_reentrancy()
{
    for(bool local:{false,true})for(auto alias:{"context","node","request","response","action"})for(unsigned phase:{1u,2u,3u})
    {
        action_fixture f;unsigned callbacks{},copies{};std::size_t required{};
        auto retire=[&](auto name){if(name==std::string_view{"action"})f.dispatch.revoke(name);else f.origin.dispatch.revoke(name);};
        auto cancel=[&]{if(++callbacks==phase){if(local)retire(alias);else assert(f.origin.authority->revoke(alias,retire)==error::success);}return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.open();
        assert(f.dispatch.invoke(f.origin.who,ac::operation::open,f.cap,input,4096,required,copy,cancel)!=error::success&&callbacks>=phase&&!copies);
        if(phase<=2)assert(!f.native);
    }
    for(auto alias:{"context","node","request","response","action"})for(unsigned phase:{2u,3u})
    {
        action_fixture f;f.opened();unsigned callbacks{},copies{};std::size_t required{};
        auto cancel=[&]{if(++callbacks==phase)assert(f.origin.authority->revoke(alias,[&](auto name){if(name==std::string_view{"action"})f.dispatch.revoke(name);else f.origin.dispatch.revoke(name);})==error::success);return false;};
        auto copy=[&](w::bytes){++copies;return true;};auto input=f.submit(1,c::now()+1000000000ULL);
        assert(f.dispatch.invoke(f.origin.who,ac::operation::submit,f.cap,input,4096,required,copy,cancel)!=error::success&&!copies);
        assert(f.native->calls==(phase==2?0u:1u));
    }
}
static void action_current_call_origins()
{
    action_fixture f;f.opened();auto opener=f.cap;
    assert(f.origin.authority->open(f.origin.who,"action",f.cap)==error::success&&f.cap!=opener);
    bytes out;auto deadline=c::now()+1000000000ULL;
    assert(f.call(ac::operation::submit,f.submit(1,deadline),out)==error::success);
    auto token=f.native->last_operation;
    assert(token.capability==f.cap&&token.capability!=opener&&token.caller_instance==f.origin.who.instance&&
        token.caller_generation==f.origin.who.generation&&token.count==6&&token.request&&token.session==f.id&&token.sequence==1);
    assert(token.origins[1].capability==f.origin.node_cap&&token.origins[1].resource==f.origin.node_id&&
        token.origins[2].capability==f.origin.context_cap&&token.origins[2].resource==f.origin.context_id&&
        token.origins[3].capability==f.origin.request_cap&&token.origins[4].capability==f.origin.request_cap&&
        token.origins[5].capability==f.origin.response_cap);
    auto admitted=f.origin.admissions("action");s::group_lease held;
    assert(f.dispatch.revalidate(token,held)==error::success);
    s::communication_jobs::claim_set claims;assert(held.matches(f.origin.who,s::communication_jobs::admission_claims(token,claims)));held.release();
    assert(f.origin.admissions("action")==admitted&&f.origin.admissions("context")==1&&f.origin.admissions("node")==1);
    assert(f.dispatch.charge_operation(token,355,held)==error::success);
    auto costs=s::communication_jobs::admission_claims(token,claims,355);
    assert(held.matches(f.origin.who,costs)&&held.consume(f.origin.who,costs)&&!held.consume(f.origin.who,costs));held.release();
    assert(f.origin.admissions("action")==admitted+1&&f.origin.admissions("context")==1&&f.origin.admissions("node")==1);
    auto forged=token;forged.origins[2].resource=f.origin.node_id;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    assert(f.dispatch.charge_operation(forged,355,held)==error::denied&&!held&&f.origin.admissions("action")==admitted+1);
    forged=token;forged.origins[4].capability=f.origin.response_cap;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    forged=token;forged.count=5;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    forged=token;++forged.caller_generation;
    assert(f.dispatch.revalidate(forged,held)==error::denied&&!held);
    assert(f.origin.authority->close(f.origin.who,opener)==error::success);
    assert(f.dispatch.revalidate(token,held)==error::success);held.release();
    assert(f.origin.authority->close(f.origin.who,f.cap)==error::success);
    assert(f.dispatch.revalidate(token,held)!=error::success&&!held);
    assert(f.origin.authority->open(f.origin.who,"action",f.cap)==error::success);
    auto cancel=encode([&](auto& writer){return writer.u64(f.id)&&writer.u64(2)&&writer.u64(f.id)&&writer.u64(token.request)&&writer.u64(deadline);});
    assert(f.call(ac::operation::cancel,cancel,out)==error::success);
    token=f.native->last_operation;assert(token.capability==f.cap&&token.action==c::permission::action_cancel);
    assert(f.origin.authority->revoke("request",[&](auto alias){f.origin.dispatch.revoke(alias);})==error::success);
    assert(f.dispatch.revalidate(token,held)!=error::success&&!held);
    for(auto budget:{3ULL,4ULL})
    {
        action_fixture server(actions::role::server,true,true,budget);server.opened();auto goal=proposal(server);
        assert(server.call(ac::operation::take,resource(server.id),out)==error::success);
        assert(server.native->last_operation.action==c::permission::action_take);
        auto status=server.call(ac::operation::respond,server.respond(1,91,goal,"accept",c::now()+1000000000ULL),out);
        assert(status==(budget==3?error::again:error::success));
        assert(server.native->calls==(budget==3?0u:1u)&&server.origin.admissions("action")== (budget==3?2u:4u));
        if(budget==4)
        {
            token=server.native->last_operation;
            assert(token.count==7&&token.secondary==c::permission::action_accept&&token.origins[6].capability==server.cap);
            assert(server.dispatch.revalidate(token,held)==error::success);
            assert(held.matches(server.origin.who,s::communication_jobs::admission_claims(token,claims)));held.release();
            assert(server.dispatch.charge_operation(token,355,held)==error::again&&!held&&server.origin.admissions("action")==4);
            forged=token;forged.secondary=c::permission::action_reject;
            assert(server.dispatch.revalidate(forged,held)==error::denied&&!held);
        }
    }
}
static void bounded_cancellation_sets()
{
    for(unsigned mode{};mode<4;++mode)
    {
        action_fixture f;f.opened();bytes out;auto deadline=c::now()+1000000000ULL;
        std::array<s::handle,2> goals{};
        for(std::size_t i{};i<2;++i)
        {
            auto body=encode([&](auto& writer){actions::uuid uuid{};uuid[0]=static_cast<std::uint8_t>(i+1);std::uint8_t body=1;
                return writer.u64(f.id)&&writer.u64(i+1)&&writer.u64(f.id)&&writer.blob(uuid)&&contract::encode_message(writer,{"test/Request","scene-canonical",{&body,1}})&&writer.u64(deadline);});
            assert(f.call(ac::operation::submit,body,out)==error::success);goals[i]=f.native->last_operation.request;
        }
        auto request=[&](std::span<s::handle const> selected,std::uint64_t sequence=3)
        {return encode([&](auto& writer){if(!writer.u64(f.id)||!writer.u64(sequence)||!writer.u64(f.id)||!writer.u64(selected.size()))return false;
            for(auto goal:selected)if(!writer.u64(goal))return false;
            return writer.u64(deadline);});};
        auto before=f.native->calls.load();auto admitted=f.origin.admissions("action");
        auto input=request(goals);
        assert(f.call(ac::operation::cancel_set,input,out,247)==error::too_large&&f.native->calls==before&&f.origin.admissions("action")==admitted);
        std::array<s::handle,2> duplicate{goals[0],goals[0]},foreign{goals[0],UINT64_MAX};
        assert(f.call(ac::operation::cancel_set,request(duplicate),out)==error::invalid&&f.native->calls==before);
        assert(f.call(ac::operation::cancel_set,request(foreign),out)==error::bad_handle&&f.native->calls==before);
        assert(f.call(ac::operation::cancel_set,request({}),out)==error::invalid&&f.native->calls==before);
        f.native->reject=mode==1;f.native->unknown=mode==2;
        if(mode==3)f.native->reject_after=before+1;
        bytes lost;auto reject=[&](w::bytes value){lost.assign(value.begin(),value.end());return false;};
        assert(f.call(ac::operation::cancel_set,input,out,4096,reject)==error::io);
        assert(f.native->calls==before+(mode==0?2u:mode==1?0u:1u));
        assert(f.call(ac::operation::cancel_set,input,out)==error::success&&out==lost);
        actions::cancellation_selection state;
        // The complete result is cached independently of the caller's sink.
        w::reader fields{out};std::uint64_t aggregate{};
        assert(fields.u64(aggregate));
        assert(f.endpoint->selection_for(f.authority,aggregate,state)==c::error::ok&&state.count==2);
        assert(state.items[0].goal==goals[0]&&state.items[1].goal==goals[1]);
        if(mode==0)assert(state.items[0].result.request&&state.items[1].result.request&&state.items[0].result.request!=state.items[1].result.request);
        if(mode==1)assert(!state.items[0].result.request&&!state.items[1].result.request&&state.items[0].result.observed==c::effect::none);
        if(mode==2)assert(state.batch.observed==c::effect::unknown&&state.items[0].result.observed==c::effect::unknown&&state.items[1].result.status==c::error::cancelled&&!state.items[1].result.request);
        if(mode==3)assert(state.items[0].result.request&&!state.items[1].result.request&&state.items[1].result.status==c::error::again);
        auto altered=goals;std::swap(altered[0],altered[1]);assert(f.call(ac::operation::cancel_set,request(altered),out)==error::conflict);
        assert(f.origin.authority->close(f.origin.who,f.origin.request_cap)==error::success);
        assert(f.call(ac::operation::cancel_set,input,out)==error::success&&out==lost);
        assert(f.call(ac::operation::cancel_set,request(goals,4),out)!=error::success);
    }
    action_fixture limited(actions::role::client,true,true,2);limited.opened();bytes out;
    auto deadline=c::now()+1000000000ULL;
    assert(limited.call(ac::operation::submit,limited.submit(1,deadline),out)==error::success);
    auto goal=limited.native->last_operation.request;
    auto request=encode([&](auto& writer){return writer.u64(limited.id)&&writer.u64(2)&&writer.u64(limited.id)&&writer.u64(1)&&writer.u64(goal)&&writer.u64(deadline);});
    auto before=limited.native->calls.load();assert(limited.call(ac::operation::cancel_set,request,out)==error::again&&limited.native->calls==before);
}
int main(){queued_transition_accounting();action_current_call_origins();action_cancellation_reentrancy();action_origins();action_client_recovery();action_server_transitions();action_pre_effect_and_sources();bounded_cancellation_sets();std::cout<<"passed 8 action dispatch security groups\n";}

#endif
