/** @file @brief Actual sealed ROS actions through canonical host dispatch and creation controls.
 * @details Context/node setup exercises the existing trusted host dispatcher.
 * Action creation uses the new receipt protocol. This is not a Wasm VM test.
 */
#define UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY
#include "communication_worker_provider.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_open_control.h>
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace ac=uwvm2::imported::ros::action_contract;
namespace act=c::actions;
namespace wire=s::wire;
template<class F> static bytes encode(F&& fill)
{std::array<std::uint8_t,4096> storage;wire::writer out{storage};assert(fill(out));return {out.written().begin(),out.written().end()};}
struct side
{
    environment* env{};std::size_t index{};
    uwvm2::utils::container::std_unique_ptr<s::communication_dispatch> origins;
    uwvm2::utils::container::std_unique_ptr<s::communication_action_dispatch> actions;
    std::shared_ptr<s::communication_action_dispatch::opening> draft;
    std::shared_ptr<w::paired_factory> factory;std::shared_ptr<w::action_endpoint> facade;std::shared_ptr<act::endpoint> core;
    s::communication_action_dispatch::plan definition;creation::plan opening;
    c::handle context{},node{},resource{},request{};bytes node_body;std::uint64_t sequence{};
    static c::error admit(void* data,c::operation_token const& token,s::group_lease& held)noexcept
    {return static_cast<c::error>(static_cast<side*>(data)->actions->revalidate(token,held));}
    static c::error charge(void* data,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
    {return static_cast<c::error>(static_cast<side*>(data)->actions->charge_operation(token,bytes,held));}
    static c::error admit_open(void* data,creation::identity const& original,s::group_lease& held)noexcept
    {return static_cast<c::error>(static_cast<side*>(data)->actions->admit_open(original,0,held));}
    static c::error charge_open(void* data,creation::identity const& original,std::uint64_t bytes,s::group_lease& held)noexcept
    {return static_cast<c::error>(static_cast<side*>(data)->actions->admit_open(original,bytes,held,true));}
    static c::error validate(void* data,creation::identity const& original)noexcept
    {s::group_lease held;return admit_open(data,original,held);}
    static c::error enqueue(void* data,s::communication_action_dispatch::opening const& pending,wire::bytes body,creation::view& result)noexcept
    {
        auto& self=*static_cast<side*>(data);auto status=self.factory->open_action_request(pending.node(),pending.identity(),body,result);
        if(status==c::error::ok)self.request=result.receipt.request_id;
        return status;
    }
    static c::error inspect(void* data,s::communication_action_dispatch::opening const& pending,creation::view& result)noexcept
    {return static_cast<side*>(data)->factory->inspect_action_open(pending.identity().who,pending.request(),result);}
    static c::error prepare(void* data,s::caller who,s::handle request,s::handle& out)noexcept
    {
        auto& self=*static_cast<side*>(data);if(!self.draft||self.request!=request||!creation::same_caller(who,self.env->who[self.index]))return c::error::denied;
        if(!self.core)
        {
            c::node_lease parent;auto status=self.actions->adoption_node(self.draft,parent);if(status!=s::error::success)return static_cast<c::error>(status);
            auto result=self.factory->create_verified_action(request,self.draft->specification(),std::move(parent),self.resource,admit,charge,&self,self.facade,self.core);
            if(result!=c::error::ok)return result;
        }
        auto status=self.actions->adopt_open(self.draft,self.core);if(status==s::error::success)out=self.resource;
        return static_cast<c::error>(status);
    }
    static c::error published(void* data,opens::published const& result,s::group_lease const& held)noexcept
    {auto& self=*static_cast<side*>(data);return static_cast<c::error>(self.actions->publish_open(self.draft,result,held));}
    static c::error released(void* data,creation::view const& result)noexcept
    {auto& self=*static_cast<side*>(data);return static_cast<c::error>(self.actions->release_open(self.draft,result));}
    s::error call(ac::operation action,wire::bytes input,bytes& output,bool copy=true)
    {std::size_t required{};return actions->invoke(env->who[index],action,env->action_caps[index],input,65536,required,[&](wire::bytes value){output.assign(value.begin(),value.end());return copy;});}
};
static c::handle mutation(bytes const& body,std::uint64_t generation)
{
    wire::reader in{body};std::uint64_t request{},actual{};std::string_view phase,effect,reason;
    assert(in.u64(request)&&request&&in.text(phase)&&phase=="admitted"&&in.text(effect)&&effect=="locally_accepted"&&in.u64(actual)&&actual==generation&&in.text(reason)&&reason=="none"&&in.done());
    return request;
}
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<peer,2> peers;int at=1;for(auto& item:peers)item.parse(argc,argv,at);assert(at==argc);
    environment env{peers,false,false,false,true};auto retained=uwvm2::utils::container::make_shared<std::array<side,2>>();auto& sides=*retained;
    std::array<uwvm2::utils::container::std_unique_ptr<s::communication_open_dispatch>,2> controls;
    for(std::size_t i{};i<2;++i)
    {
        auto& item=sides[i];item.env=&env;item.index=i;auto suffix=uwvm2::utils::container::to_string(i);
        for(std::size_t n{};n<peers[i].definition.definitions.size();++n)
        {
            auto const& definition=peers[i].definition.definitions[n];
            if(definition.kind==s::communication_configuration::family::action&&cfg::profile_of(definition).identity==(i?"action-server":"action-client"))
            {item.definition=std::get<s::communication_action_dispatch::plan>(definition.policy);item.opening.worker_binding=n+1;}
        }
        item.origins=uwvm2::utils::container::make_unique<s::communication_dispatch>(env.permission_owner,71,1,s::communication_dispatch::limits{{1,1,0,3,0,65536,0},5,65536,65536,524288,8});
        assert(item.origins->bind("context."+suffix,env.input[i].context)==s::error::success&&item.origins->bind("node."+suffix,env.input[i].node)==s::error::success);
        assert(item.origins->bind("goal."+suffix,item.definition.policy.goal)==s::error::success&&item.origins->bind("feedback."+suffix,item.definition.policy.feedback)==s::error::success&&
            item.origins->bind("result."+suffix,item.definition.policy.result)==s::error::success);
        assert(item.origins->describe_plan("context."+suffix,env.input[i].context)==s::error::success&&item.origins->describe_plan("node."+suffix,env.input[i].node)==s::error::success);
        item.actions=uwvm2::utils::container::make_unique<s::communication_action_dispatch>(env.permission_owner,*item.origins,71,1,s::communication_action_dispatch::limits{1,65536,65536,524288,8});
        assert(item.actions->bind("action."+suffix,item.definition)==s::error::success&&item.actions->prepare_async()==s::error::success&&item.actions->describe_plan("action."+suffix,item.definition)==s::error::success);
        auto& plan=item.opening;plan.authority=item.definition.policy.authority;plan.selected=item.definition.policy.selected;plan.kind=opens::endpoint_kind::action;
        plan.provider_generation=peers[i].definition.network.generation;plan.carrier_bytes=2048;plan.max_deadline_ns=4000000000;
        env.input[i].action_openings={&plan,1};env.input[i].action_carrier_bytes=4096;env.input[i].action_bytes=4u*1024u*1024u;
        env.input[i].admit_action=side::admit_open;env.input[i].charge_action=side::charge_open;env.input[i].action_authority=&item;
    }
    auto factory=uwvm2::utils::container::make_shared<w::paired_factory>();assert(factory->configure(env.input,environment::admit,&env,5000000000)==c::error::ok);
    for(auto& item:sides)
    {
        auto i=item.index;item.factory=factory;assert(item.origins->prepare(factory)==s::error::success);
        auto body=encode([&](auto& out){return contract::encode_profile(out,env.input[i].context.selected);});std::size_t required{};
        assert(item.origins->invoke(env.who[i],contract::operation::context_open,env.request[i].context_capability,body,8,required,[&](wire::bytes value){wire::reader in{value};return in.u64(item.context)&&item.context&&in.done();})==s::error::success);
        item.node_body=encode([&](auto& out){return out.u64(env.request[i].context_capability)&&out.u64(item.context)&&contract::encode_profile(out,env.input[i].node.selected);});
        env.request[i]={env.who[i].instance,env.who[i].generation,env.request[i].node_capability,env.request[i].context_capability,item.context,env.input[i].context.authority,s::buffers::hash(item.node_body)};
        w::open_control::adoption adoption{std::shared_ptr<void>{retained,&item},side::validate,side::prepare,side::published,side::released};
        auto source=uwvm2::utils::container::make_shared<w::open_control>(factory,item.definition.policy.authority,opens::endpoint_kind::action,std::move(adoption));
        controls[i]=uwvm2::utils::container::make_unique<s::communication_open_dispatch>(env.permission_owner,s::communication_open_dispatch::limits{1,1040,2});
        assert(controls[i]->bind("action."+uwvm2::utils::container::to_string(i),item.definition.policy.authority,opens::endpoint_kind::action,std::move(source))==s::error::success&&controls[i]->prepare()==s::error::success);
    }
    auto deadline=c::now()+15000000000ull;
    while(!sides[0].node||!sides[1].node)
    {
        for(auto& item:sides)if(!item.node)
        {
            std::size_t required{};auto i=item.index;auto status=item.origins->invoke(env.who[i],contract::operation::node_open,env.request[i].node_capability,item.node_body,8,required,
                [&](wire::bytes value){wire::reader in{value};return in.u64(item.node)&&item.node&&in.done();});
            assert(status==s::error::success||status==s::error::again);
        }
        assert(c::now()<deadline);if(!sides[0].node||!sides[1].node)::poll(nullptr,0,1);
    }
    assert(factory->inspect().link.active);
    for(auto& item:sides)
    {
        auto i=item.index;auto const& policy=item.definition.policy;
        auto body=encode([&](auto& out){return out.u64(env.request[i].node_capability)&&out.u64(item.node)&&out.u64(3)&&out.u64(env.schema_caps[i])&&out.u64(env.feedback_caps[i])&&out.u64(env.result_caps[i])&&
            contract::encode_profile(out,policy.selected)&&out.text(policy.resolved_name)&&out.text(policy.action_type)&&out.text(i?"server":"client")&&out.u64(1)&&out.u64(c::now()+std::min<std::uint64_t>(3500000000,policy.max_deadline_ns));});
        auto staged=item.actions->stage_open(env.who[i],env.action_caps[i],body,48,item.draft);
        std::fprintf(stderr,"action stage side=%zu status=%u\n",i,static_cast<unsigned>(staged));
        assert(staged==s::error::success);item.resource=item.draft->resource();
        std::size_t required{};bytes first,second;
        auto invoke=[&](bool deliver,bytes& target){return item.actions->invoke_open(env.who[i],env.action_caps[i],body,48,required,[&](wire::bytes value){target.assign(value.begin(),value.end());return deliver;},{&item,side::enqueue,side::inspect});};
        assert(invoke(false,first)==s::error::io&&invoke(true,second)==s::error::success&&first==second&&item.request&&item.resource!=item.request);
    }
    auto pump=[&]{auto status=factory->pump_actions();assert(status==c::error::ok||status==c::error::again);};
    for(auto& item:sides)
    {
        auto body=encode([&](auto& out){return out.u64(item.request);});std::size_t required{};deadline=c::now()+4000000000;
        auto take=[&](bool deliver){return controls[item.index]->invoke(env.who[item.index],s::communication_open_dispatch::operation::take,env.action_caps[item.index],body,64,required,[&](wire::bytes value){assert(value.size()==64);return deliver;});};
        s::error status{};while((status=take(false))==s::error::again){pump();assert(c::now()<deadline);::poll(nullptr,0,1);}
        assert(status==s::error::io&&item.core&&take(true)==s::error::success);
    }
    auto discovery_until=c::now()+2000000000;while(c::now()<discovery_until){pump();::poll(nullptr,0,2);}
    auto settle=[&](side& item)
    {
        c::operation_token token;c::native_result result;auto until=c::now()+3000000000;auto status=item.facade->completion(token,result);
        while(status==c::error::again){pump();assert(c::now()<until);status=item.facade->completion(token,result);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&result.status==c::error::ok&&item.core->settle_operation(token.request,result)==c::error::ok&&item.facade->release_completion(token)==c::error::ok);
    };
    auto take=[&](side& item,act::event_kind wanted)
    {
        auto body=encode([&](auto& out){return out.u64(item.resource);});auto until=c::now()+6000000000;bytes output;
        while(true)
        {
            auto status=item.call(ac::operation::take,body,output,false);
            if(status==s::error::io)
            {
                auto first=output;assert(item.call(ac::operation::take,body,output)==s::error::success&&first==output);
                w::action_record record;assert(w::decode_action_record(output,item.draft->specification(),item.resource,peers[item.index].definition.network.generation,record));
                if(record.kind==wanted){record.payload={};return record;}
                assert(record.kind==act::event_kind::status||record.kind==act::event_kind::acceptance);
            }
            else assert(status==s::error::again);
            pump();assert(c::now()<until);::poll(nullptr,0,1);
        }
    };
    auto& client=sides[0];auto& server=sides[1];act::uuid uuid{};uuid[0]=91;
    auto goal=encode([](auto& out){return out.text("world")&&out.text("robot")&&out.i64(0)&&out.u64(0)&&out.i64(0)&&out.u64(0)&&out.i64(0)&&out.u64(0)&&out.text("")&&out.boolean(false);});
    auto submit=encode([&](auto& out){auto const& schema=client.draft->specification().goal;return out.u64(client.resource)&&out.u64(++client.sequence)&&out.u64(client.resource)&&out.blob(uuid)&&contract::encode_message(out,{schema.type_identity,schema.encoding,goal})&&out.u64(c::now()+900000000);});
    bytes receipt;assert(client.call(ac::operation::submit,submit,receipt)==s::error::success);auto client_goal=mutation(receipt,peers[0].definition.network.generation);settle(client);
    auto proposal=take(server,act::event_kind::proposal);assert(proposal.identity==uuid&&proposal.native_goal!=proposal.native_request&&proposal.native_goal!=client_goal);
    auto result=encode([](auto& out){if(!out.i64(0)||!out.u64(0)||!out.text("world")||!out.text("robot"))return false;for(double v:{0.,0.,0.,0.,0.,0.,1.})if(!out.f64(v))return false;return out.u64(0)&&out.text("");});
    auto respond=[&](act::transition change,c::handle request,bool accepts=false)
    {
        auto const& schema=change==act::transition::feedback?server.draft->specification().feedback:server.draft->specification().result;
        bool message=change==act::transition::feedback||change==act::transition::cancelled;
        auto body=encode([&](auto& out){return out.u64(server.resource)&&out.u64(++server.sequence)&&out.u64(server.resource)&&out.u64(request)&&out.u64(proposal.native_goal)&&
            out.text(ac::transition_names[static_cast<std::size_t>(change)-1])&&out.boolean(message)&&(!message||contract::encode_message(out,{schema.type_identity,schema.encoding,change==act::transition::cancelled?wire::bytes{result}:wire::bytes{}}))&&
            out.u64(accepts?1:0)&&(!accepts||out.u64(proposal.native_goal))&&out.u64(c::now()+900000000);});
        assert(server.call(ac::operation::respond,body,receipt)==s::error::success);settle(server);assert(server.call(ac::operation::respond,body,receipt)==s::error::success);
    };
    respond(act::transition::accept,proposal.native_request);respond(act::transition::execute,proposal.native_request);
    assert(take(client,act::event_kind::acceptance).native_goal==client_goal);
    respond(act::transition::feedback,proposal.native_request);assert(take(client,act::event_kind::feedback).native_goal==client_goal);
    auto cancel=encode([&](auto& out){return out.u64(client.resource)&&out.u64(++client.sequence)&&out.u64(client.resource)&&out.u64(client_goal)&&out.u64(c::now()+900000000);});
    assert(client.call(ac::operation::cancel,cancel,receipt)==s::error::success);auto cancellation=mutation(receipt,peers[0].definition.network.generation);settle(client);
    auto offered=take(server,act::event_kind::cancel_request);assert(offered.native_goal==proposal.native_goal&&offered.native_request!=proposal.native_request);
    respond(act::transition::cancel_response,offered.native_request,true);assert(take(client,act::event_kind::cancel_response).native_request==cancellation);
    respond(act::transition::cancelled,proposal.native_request);auto completed=take(client,act::event_kind::result);assert(completed.native_goal==client_goal&&completed.observed==act::state::cancelled);
    for(auto& item:sides)
    {
        auto until=c::now()+3000000000;auto close=[&]{return item.actions->close(env.who[item.index],env.action_caps[item.index],item.resource);};auto status=close();
        while(status==s::error::again){pump();assert(c::now()<until);status=close();if(status==s::error::again)::poll(nullptr,0,1);}
        assert(status==s::error::success&&factory->reconcile_close(item.request,item.resource)==c::error::ok);
        auto body=encode([&](auto& out){return out.u64(item.request);});std::size_t required{};
        assert(controls[item.index]->invoke(env.who[item.index],s::communication_open_dispatch::operation::release,env.action_caps[item.index],body,8,required,[](wire::bytes value){wire::reader in{value};bool released{};return in.boolean(released)&&released&&in.done();})==s::error::success);
    }
    factory->retire();assert(!factory->cleanup_confirmed());
    std::puts("PASS actual sealed canonical actions: dispatcher-minted origins, asynchronous creation and publication, canonical native goal/feedback/cancel/result, exact failed-copy recovery and retained remote cleanup debt");
}
