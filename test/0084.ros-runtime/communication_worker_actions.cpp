/** @file @brief Actual isolated LookupTransform action creation, ownership, effects and cleanup. */
#define UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY
#include "communication_worker_provider.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_open_control.h>
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace ac=uwvm2::imported::ros::action_contract;
namespace act=c::actions;
namespace wire=s::wire;

struct action_state
{
    environment* env{};std::size_t side{};std::shared_ptr<c::registry> registry;
    std::shared_ptr<w::action_endpoint> endpoint;std::shared_ptr<act::endpoint> core;
    std::shared_ptr<w::paired_factory> factory;
    act::plan policy;creation::plan creation_plan;creation::identity opening;
    c::handle context{},node{},resource{},request{};c::node_lease parent;
    std::shared_ptr<int> retained;unsigned charges{},validations{};std::uint64_t sequence{};
    c::operation_token token(c::permission action,c::permission secondary=c::permission::unconfigured)const
    {
        c::operation_token value;value.caller_instance=env->who[side].instance;value.caller_generation=env->who[side].generation;
        value.authority=policy.authority;value.capability=env->action_caps[side];value.resource=resource;value.family=c::operation_family::action;
        value.action=action;value.secondary=secondary;value.count=secondary==c::permission::unconfigured?6:7;value.retained_origins=retained;
        value.origins[0]={policy.authority,value.capability,resource,action};
        value.origins[1]={env->input[side].node.authority,env->request[side].node_capability,node,c::permission::node_open};
        value.origins[2]={env->input[side].context.authority,env->request[side].context_capability,context,c::permission::context_open};
        value.origins[3]={policy.goal.authority,env->schema_caps[side],0,c::permission::type_describe};
        value.origins[4]={policy.feedback.authority,env->feedback_caps[side],0,c::permission::type_describe};
        value.origins[5]={policy.result.authority,env->result_caps[side],0,c::permission::type_describe};
        if(value.count==7)value.origins[6]={policy.authority,value.capability,resource,secondary};
        return value;
    }
    c::error actual_origins()const noexcept
    {
        auto const& input=env->input[side];auto status=registry->validate_context(input.context.authority,context,input.context.selected);
        return status==c::error::ok?registry->validate_node(input.node.authority,node,input.node.selected):status;
    }
    static c::error admit(void* opaque,c::operation_token const& current,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<action_state*>(opaque);auto expected=self.token(current.action,current.secondary);
        if(!current.valid(current.action,c::operation_family::action,self.policy.authority,self.resource,false)||
            current.caller_instance!=expected.caller_instance||current.caller_generation!=expected.caller_generation||
            current.origins!=expected.origins||current.retained_origins!=self.retained)return c::error::denied;
        s::communication_jobs::claim_set claims;
        auto status=static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],s::communication_jobs::admission_claims(current,claims),held,false));
        if(status!=c::error::ok)return status;
        // Independently granted cleanup must remain possible after schema or parent authority closes.
        status=current.action==c::permission::resource_close?c::error::ok:self.actual_origins();
        if(status!=c::error::ok)held.release();else ++self.validations;
        return status;
    }
    static c::error charge(void* opaque,c::operation_token const& current,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<action_state*>(opaque);s::group_lease validate;auto status=admit(opaque,current,validate);if(status!=c::error::ok)return status;
        s::communication_jobs::claim_set claims;
        status=static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],s::communication_jobs::admission_claims(current,claims,bytes),held));
        if(status==c::error::ok)++self.charges;
        return status;
    }
    static c::error admit_open(void* opaque,creation::identity const& current,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<action_state*>(opaque);if(current!=self.opening)return c::error::denied;
        creation::claim_set claims;auto status=static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],
            creation::admission_claims(current,opens::endpoint_kind::action,claims),held,false));
        if(status!=c::error::ok)return status;
        status=self.actual_origins();if(status!=c::error::ok)held.release();return status;
    }
    static c::error charge_open(void* opaque,creation::identity const& current,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        s::group_lease validated;auto status=admit_open(opaque,current,validated);if(status!=c::error::ok)return status;
        auto& self=*static_cast<action_state*>(opaque);creation::claim_set claims;
        return static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],creation::admission_claims(current,opens::endpoint_kind::action,claims,bytes),held));
    }
    static c::error validate_creation(void* opaque,creation::identity const& origin)noexcept
    {auto& self=*static_cast<action_state*>(opaque);return origin==self.opening?self.actual_origins():c::error::denied;}
    static c::error adopt_creation(void* opaque,s::caller who,s::handle request,s::handle& output)noexcept
    {
        auto& self=*static_cast<action_state*>(opaque);
        if(!creation::same_caller(who,self.env->who[self.side])||request!=self.request)return c::error::denied;
        if(!self.core)
        {
            auto status=self.factory->create_verified_action(request,self.policy,std::move(self.parent),self.resource,admit,charge,&self,self.endpoint,self.core);
            if(status!=c::error::ok)return status;
        }
        output=self.resource;return c::error::ok;
    }
    bytes carrier()const
    {
        std::array<std::uint8_t,2048> storage;wire::writer out{storage};
        assert(out.u64(env->request[side].node_capability)&&out.u64(node)&&out.u64(3)&&out.u64(env->schema_caps[side])&&out.u64(env->feedback_caps[side])&&
            out.u64(env->result_caps[side])&&contract::encode_profile(out,policy.selected)&&out.text(policy.resolved_name)&&out.text(policy.action_type)&&
            out.text(side?"server":"client")&&out.u64(opening.key)&&out.u64(opening.deadline));
        return {out.written().begin(),out.written().end()};
    }
};
struct action_capture
{
    act::delivery event;bytes body;bool fail{};unsigned failures{};
    static bool sink(void* opaque,act::delivery const& value)
    {
        auto& self=*static_cast<action_capture*>(opaque);self.event=value;self.body.assign(value.body.begin(),value.body.end());
        if(self.fail){self.fail=false;++self.failures;return false;}return true;
    }
};

int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<peer,2> peers;int at=1;for(auto& item:peers)item.parse(argc,argv,at);assert(at==argc);
    environment env{peers,false,false,false,true};auto retained_state=uwvm2::utils::container::make_shared<std::array<action_state,2>>();auto& state=*retained_state;
    std::array<uwvm2::utils::container::std_unique_ptr<s::communication_open_dispatch>,2> controls;
    for(std::size_t i{};i<2;++i)
    {
        auto& value=state[i];value.env=&env;value.side=i;value.retained=uwvm2::utils::container::make_shared<int>(i);
        auto const& definitions=peers[i].definition.definitions;
        for(std::size_t n{};n<definitions.size();++n)if(definitions[n].kind==s::communication_configuration::family::action&&
            cfg::profile_of(definitions[n]).identity==(i?"action-server":"action-client"))
        {value.policy=std::get<s::communication_action_dispatch::plan>(definitions[n].policy).policy;value.creation_plan.worker_binding=n+1;}
        assert(c::valid(value.policy.selected));value.policy.authority={71,100+i,1};value.policy.goal.authority={71,200+i,1};
        std::fprintf(stderr,"action side=%zu name=%s role=%u\n",i,value.policy.resolved_name.c_str(),static_cast<unsigned>(value.policy.direction));
        value.policy.feedback.authority={71,300+i,1};value.policy.result.authority={71,400+i,1};
        value.creation_plan.authority=value.policy.authority;value.creation_plan.selected=value.policy.selected;value.creation_plan.kind=opens::endpoint_kind::action;
        value.creation_plan.provider_generation=peers[i].definition.network.generation;value.creation_plan.carrier_bytes=2048;value.creation_plan.max_deadline_ns=4000000000;
        env.input[i].action_openings={&value.creation_plan,1};env.input[i].action_carrier_bytes=4096;env.input[i].action_bytes=4u*1024u*1024u;
        env.input[i].admit_action=action_state::admit_open;env.input[i].charge_action=action_state::charge_open;env.input[i].action_authority=&value;
    }
    auto factory=uwvm2::utils::container::make_shared<w::paired_factory>();auto insufficient=env.input;insufficient[0].action_bytes=1;
    assert(factory->configure(insufficient,environment::admit,&env,5000000000)==c::error::too_large&&!factory->inspect().configured);
    assert(factory->configure(env.input,environment::admit,&env,5000000000)==c::error::ok);
    for(auto& item:state)
    {
        item.factory=factory;auto alias="action."+uwvm2::utils::container::to_string(item.side);
        w::open_control::adoption adoption{std::shared_ptr<void>{retained_state,&item},action_state::validate_creation,action_state::adopt_creation};
        auto source=uwvm2::utils::container::make_shared<w::open_control>(factory,item.policy.authority,opens::endpoint_kind::action,std::move(adoption));
        controls[item.side]=uwvm2::utils::container::make_unique<s::communication_open_dispatch>(env.permission_owner,s::communication_open_dispatch::limits{1,1040,2});
        assert(controls[item.side]->bind(alias,item.policy.authority,opens::endpoint_kind::action,std::move(source))==s::error::success&&controls[item.side]->prepare()==s::error::success);
    }
    for(auto& item:state)
    {
        auto i=item.side;item.registry=uwvm2::utils::container::make_shared<c::registry>(factory,c::limits{1,1,0,3,0,65536,0});
        assert(item.registry->configure(env.input[i].context)==c::error::ok&&item.registry->configure(env.input[i].node)==c::error::ok);
        assert(item.registry->configure(item.policy.goal)==c::error::ok&&item.registry->configure(item.policy.feedback)==c::error::ok&&item.registry->configure(item.policy.result)==c::error::ok);
        assert(item.registry->open_context(env.input[i].context.authority,env.input[i].context.selected,item.context)==c::error::ok);
        env.request[i].context=item.context;
    }
    auto deadline=c::now()+15000000000ull;std::array<bool,2> ready{};
    while(!(ready[0]&&ready[1]))
    {
        for(auto& item:state)if(!ready[item.side])
        {auto i=item.side;auto status=item.registry->prepare_node(env.input[i].node.authority,env.input[i].context.authority,item.context,env.input[i].node.selected,env.request[i]);
            assert(status==c::error::ok||status==c::error::again);ready[i]=status==c::error::ok;}
        assert(c::now()<deadline);if(!(ready[0]&&ready[1]))::poll(nullptr,0,1);
    }
    for(auto& item:state)
    {auto i=item.side;assert(item.registry->open_node(env.input[i].node.authority,env.input[i].context.authority,item.context,env.input[i].node.selected,item.node)==c::error::ok);}
    assert(factory->inspect().link.active);
    for(auto& item:state)
    {
        auto i=item.side;assert(item.registry->attach_node(env.input[i].node.authority,item.node,env.input[i].node.selected,item.parent)==c::error::ok);
        item.resource=uwvm2::imported::ros::resources::allocate_identity();auto& origin=item.opening;
        origin.who=env.who[i];origin.authority=item.policy.authority;origin.key=1;origin.deadline=c::now()+3500000000;
        origin.count=6;origin.origins[0]={creation::dependency::operation,item.policy.authority,env.action_caps[i],0};
        origin.origins[1]={creation::dependency::node,env.input[i].node.authority,env.request[i].node_capability,item.node};
        origin.origins[2]={creation::dependency::context,env.input[i].context.authority,env.request[i].context_capability,item.context};
        origin.origins[3]={creation::dependency::schema,item.policy.goal.authority,env.schema_caps[i],0};
        origin.origins[4]={creation::dependency::schema,item.policy.feedback.authority,env.feedback_caps[i],0};
        origin.origins[5]={creation::dependency::schema,item.policy.result.authority,env.result_caps[i],0};
        auto body=item.carrier();creation::view receipt,retry;wire::writer corrupt{body};assert(corrupt.u64(env.request[i].context_capability));
        assert(factory->open_action_request(item.parent,origin,body,receipt)==c::error::denied&&!receipt.receipt.request_id);
        body=item.carrier();assert(factory->open_action_request(item.parent,origin,body,receipt)==c::error::ok);
        assert(factory->open_action_request(item.parent,origin,body,retry)==c::error::ok&&retry.receipt.request_id==receipt.receipt.request_id);
        item.request=receipt.receipt.request_id;assert(item.request!=item.resource);
    }
    auto pump=[&]{auto status=factory->pump_actions();assert(status==c::error::ok||status==c::error::again);};
    auto finish_open=[&]
    {
        ready={};deadline=c::now()+4000000000;
        while(!(ready[0]&&ready[1]))
        {
            pump();for(auto& item:state){creation::view result;assert(factory->inspect_action_open(env.who[item.side],item.request,result)==c::error::ok);
                ready[item.side]=result.receipt.status==opens::state::ready;}
            assert(c::now()<deadline);if(!(ready[0]&&ready[1]))::poll(nullptr,0,1);
        }
        for(auto& item:state)
        {
            std::array<std::uint8_t,8> request;wire::writer in{request};assert(in.u64(item.request));std::size_t required{};
            auto call=[&](std::size_t capacity,bool deliver){return controls[item.side]->invoke(env.who[item.side],s::communication_open_dispatch::operation::take,
                env.action_caps[item.side],request,capacity,required,[&](wire::bytes body){wire::reader out{body};std::uint64_t value{};
                    for(unsigned i{};i<7;++i)assert(out.u64(value));
                    assert(value==static_cast<std::uint64_t>(opens::endpoint_kind::action)&&out.u64(value)&&value==item.resource&&out.done());return deliver;});};
            assert(call(63,true)==s::error::too_large&&!item.core);
            assert(call(64,false)==s::error::io&&item.core&&item.endpoint);
            assert(call(64,true)==s::error::success);
        }
    };
    finish_open();auto& client=state[0];auto& server=state[1];
    // Native DDS endpoint discovery is asynchronous. Allow the fixed local
    // peer route to exchange discovery traffic before this effect-only fixture.
    auto discovery_until=c::now()+2000000000;
    while(c::now()<discovery_until){pump();::poll(nullptr,0,2);}
    auto reconcile=[&](action_state& item)
    {
        c::operation_token completed;c::native_result result;auto until=c::now()+3000000000;
        auto status=item.endpoint->completion(completed,result);
        while(status==c::error::again){pump();assert(c::now()<until);status=item.endpoint->completion(completed,result);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&result.status==c::error::ok&&result.observed==c::effect::locally_accepted);
        assert(item.core->settle_operation(completed.request,result)==c::error::ok&&item.endpoint->release_completion(completed)==c::error::ok);
    };
    auto receive=[&](action_state& item,act::event_kind wanted,action_capture& capture)
    {
        std::fprintf(stderr,"receive side=%zu kind=%llu\n",item.side,static_cast<unsigned long long>(wanted));
        auto until=c::now()+6000000000;auto token=item.token(c::permission::action_take);
        while(true)
        {
            auto status=item.core->take(item.policy.authority,&capture,action_capture::sink,&token);
            if(status==c::error::io)
            {
                auto charges=item.charges;auto sequence=capture.event.information.sequence;
                status=item.core->take(item.policy.authority,&capture,action_capture::sink,&token);
                assert(status==c::error::ok&&capture.event.information.sequence==sequence&&item.charges==charges);
            }
            assert(status==c::error::ok||status==c::error::again);
            if(status==c::error::ok)
            {
                std::fprintf(stderr,"event side=%zu kind=%llu goal=%llu state=%llu\n",item.side,static_cast<unsigned long long>(capture.event.kind),static_cast<unsigned long long>(capture.event.goal),static_cast<unsigned long long>(capture.event.observed));
                assert(capture.event.information.source==item.resource&&capture.event.information.generation==peers[item.side].definition.network.generation);
                if(capture.event.kind==wanted)return;
                assert(capture.event.kind==act::event_kind::status||capture.event.kind==act::event_kind::feedback||capture.event.kind==act::event_kind::acceptance);
            }
            pump();assert(c::now()<until);::poll(nullptr,0,1);
        }
    };
    std::array<std::uint8_t,512> goal_bytes,result_bytes;wire::writer goal{goal_bytes},result{result_bytes};
    assert(goal.text("world")&&goal.text("robot")&&goal.i64(0)&&goal.u64(0)&&goal.i64(0)&&goal.u64(0)&&goal.i64(0)&&goal.u64(0)&&goal.text("")&&goal.boolean(false));
    assert(result.i64(0)&&result.u64(0)&&result.text("world")&&result.text("robot"));
    for(double scalar:{0.,0.,0.,0.,0.,0.,1.})assert(result.f64(scalar));
    assert(result.u64(0)&&result.text(""));
    auto respond=[&](c::handle goal,c::handle request,act::transition change,bool accept=false)
    {
        auto token=server.token(c::permission::action_respond,static_cast<c::permission>(static_cast<unsigned>(c::permission::action_accept)+static_cast<unsigned>(change)-1));
        act::response response{request,goal,change,(change==act::transition::succeed||change==act::transition::cancelled)?result.written():wire::bytes{},accept};
        auto deadline=c::now()+900000000;c::receipt receipt;
        assert(server.core->respond(server.policy.authority,{server.resource,++server.sequence},response,deadline,receipt,&token)==c::error::ok);
        reconcile(server);auto charges=server.charges;
        assert(server.core->respond(server.policy.authority,{server.resource,server.sequence},response,deadline,receipt,&token)==c::error::ok&&server.charges==charges);
    };
    for(unsigned round{};round<2;++round)
    {
        act::uuid uuid{};uuid[0]=71+round;auto token=client.token(c::permission::action_submit);c::receipt receipt;
        assert(client.core->submit(client.policy.authority,{client.resource,++client.sequence},uuid,goal.written(),c::now()+900000000,receipt,&token)==c::error::ok);
        auto client_goal=receipt.request;reconcile(client);action_capture proposed;proposed.fail=true;receive(server,act::event_kind::proposal,proposed);
        assert(proposed.event.goal_uuid==uuid&&proposed.body==bytes(goal.written().begin(),goal.written().end())&&proposed.failures==1);
        auto server_goal=proposed.event.goal,proposal=proposed.event.request;
        assert(server_goal!=proposal&&server_goal!=client_goal);respond(server_goal,proposal,act::transition::accept);respond(server_goal,proposal,act::transition::execute);
        action_capture accepted;receive(client,act::event_kind::acceptance,accepted);assert(accepted.event.goal==client_goal&&accepted.event.goal_uuid==uuid);
        if(!round)
        {
            respond(server_goal,proposal,act::transition::feedback);action_capture feedback;receive(client,act::event_kind::feedback,feedback);
            assert(feedback.event.goal==client_goal&&feedback.body.empty());respond(server_goal,proposal,act::transition::succeed);
        }
        else
        {
            auto cancel=client.token(c::permission::action_cancel);
            assert(client.core->cancel(client.policy.authority,{client.resource,++client.sequence},client_goal,c::now()+900000000,receipt,&cancel)==c::error::ok);
            auto cancellation=receipt.request;reconcile(client);action_capture offered;receive(server,act::event_kind::cancel_request,offered);
            assert(offered.event.goal==server_goal&&offered.event.request!=proposal&&offered.event.request!=server_goal);
            respond(server_goal,offered.event.request,act::transition::cancel_response,true);action_capture cancelled;
            receive(client,act::event_kind::cancel_response,cancelled);assert(cancelled.event.request==cancellation&&cancelled.event.observed==act::state::cancelling);
            respond(server_goal,proposal,act::transition::cancelled);
        }
        action_capture completed;completed.fail=true;receive(client,act::event_kind::result,completed);
        assert(completed.event.goal==client_goal&&completed.event.observed==(round?act::state::cancelled:act::state::succeeded)&&
            completed.body==bytes(result.written().begin(),result.written().end())&&completed.failures==1);
    }
    auto close=[&](action_state& item)
    {
        auto token=item.token(c::permission::resource_close);auto until=c::now()+3000000000;
        auto status=item.core->close(item.policy.authority,&token);
        while(status==c::error::again){pump();assert(c::now()<until);status=item.core->close(item.policy.authority,&token);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&item.endpoint->cleanup_confirmed());
        assert(factory->reconcile_close(item.request,item.resource)==c::error::ok);
        std::array<std::uint8_t,8> request;wire::writer out{request};assert(out.u64(item.request));std::size_t required{};
        assert(controls[item.side]->invoke(env.who[item.side],s::communication_open_dispatch::operation::release,env.action_caps[item.side],request,8,required,
            [](wire::bytes reply){wire::reader in{reply};bool released{};return in.boolean(released)&&released&&in.done();})==s::error::success);
    };
    for(auto& item:state)
    {
        close(item);auto old_request=item.request,old_resource=item.resource;auto i=item.side;
        assert(item.registry->attach_node(env.input[i].node.authority,item.node,env.input[i].node.selected,item.parent)==c::error::ok);
        item.opening.key=2;item.opening.deadline=c::now()+3500000000;auto body=item.carrier();creation::view receipt;
        assert(factory->open_action_request(item.parent,item.opening,body,receipt)==c::error::again&&!receipt.receipt.request_id);
        item.core.reset();assert(factory->open_action_request(item.parent,item.opening,body,receipt)==c::error::again&&!receipt.receipt.request_id);
        item.endpoint.reset();assert(factory->open_action_request(item.parent,item.opening,body,receipt)==c::error::ok&&receipt.receipt.request_id!=old_request);
        item.request=receipt.receipt.request_id;item.resource=uwvm2::imported::ros::resources::allocate_identity();assert(item.resource!=old_resource);
    }
    finish_open();for(auto& item:state){close(item);item.core.reset();item.endpoint.reset();}
    factory->retire();assert(!factory->cleanup_confirmed());
    std::puts("PASS actual sealed typed actions: factory-owned creation, three schemas, distinct goal/proposal/cancel correlations, LookupTransform feedback/result/cancel, retained failed copies, independent transitions and close-reopen capacity");
}
