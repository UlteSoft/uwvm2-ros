/** @file @brief Actual sealed native factory admission and retirement qualification. */
#define main communication_startup_fixture_entry
#include "communication_worker_startup.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_worker_provider.h>
#include <cassert>
#include "communication_external_fixture.h"
namespace c=s::communication;
namespace cfg=s::communication_worker_configuration;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
struct peer
{
    x::manifest manifest;uwvm2::utils::container::std_vector<x::descriptor> files;
    cfg::specification definition;
    void parse(int argc,char** argv,int& at)
    {
        assert(at+7<=argc);std::uint64_t images{},threads{},request{},response{},count{};
        assert(decimal(argv[at++],images)&&decimal(argv[at++],threads)&&decimal(argv[at++],request)&&decimal(argv[at++],response));
        auto launcher=argv[at++],hash=argv[at++];assert(add(launcher,hash,"/worker/launcher",x::image_role::runtime,manifest.launcher,files));
        assert(decimal(argv[at++],count)&&count<=256&&count<=static_cast<unsigned>(argc-at)/4);
        for(std::uint64_t i{};i<count;++i)
        {
            std::string_view role{argv[at++]};auto destination=argv[at++],path=argv[at++],sha=argv[at++];x::image_spec image;
            auto kind=role=="bootstrap"?x::image_role::bootstrap:role=="runtime"?x::image_role::runtime:x::image_role::configuration;
            assert(add(path,sha,destination,kind,image,files));
            if(image.destination.view()=="/worker/profile")
            {manifest.bound.profile_bytes=image.bytes;manifest.configuration.sha256=image.sha256;bytes data(image.bytes);assert(::pread(image.source,data.data(),data.size(),0)==static_cast<ssize_t>(data.size()));assert(cfg::decode(data,16u*1024u*1024u,definition)==s::error::success);}
            manifest.files.emplace_back(std::move(image));
        }
        assert(manifest.configuration.identity.assign("qualified-factory-peer")&&manifest.ros_prefix.assign("/opt/ros/jazzy"));auto profile=manifest.bound.profile_bytes;
        manifest.bound={images,2ull*1024*1024*1024,30,4096,manifest.files.size()+64,65536,0,request,response,65536,10000000000ull,1000000,1024u*1024u,profile,threads};
    }
};
struct environment
{
    std::array<uwvm2::utils::container::std_unique_ptr<external_fixture::owner>,2> external;
    std::shared_ptr<s::authority> permission_owner=uwvm2::utils::container::make_shared<s::authority>();
    s::authority& permissions{*permission_owner};int callers[2]{};std::array<s::caller,2> who{{{&callers[0],1},{&callers[1],1}}};
    std::array<w::peer_configuration,2> input;std::array<c::node_stage_request,2> request;
    std::array<std::shared_ptr<c::native_context>,2> contexts;std::array<std::shared_ptr<c::native_node>,2> nodes;
    std::array<s::handle,2> graph_caps{};
    std::array<s::handle,2> topic_caps{},schema_caps{};
    std::array<s::handle,2> service_caps{},response_caps{};
    std::array<s::handle,2> action_caps{},feedback_caps{},result_caps{};
    unsigned admitted{};
    std::atomic<bool> pause_admission{},entered{},resume{};
    explicit environment(std::array<peer,2> const& peers,bool graph=false,bool topics=false,bool services=false,bool actions=false,bool canonical=false)
    {
        for(auto name:{"capability.open","capability.close","context.open","node.open","resource.close"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        if(graph)for(auto name:{"context.describe","graph.snapshot"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        if(topics)for(auto name:{"type.describe","topic.open","topic.publish","qos.describe","qos.liveliness.assert","qos.acknowledgements.wait",
            "topic.reserve","topic.acknowledge","topic.release"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        if(services)for(auto name:{"type.describe","service.open","service.submit","service.respond","service.available","service.abandon","qos.describe",
            "service.reserve","service.acknowledge","service.release"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        if(canonical)for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release"})
            assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        if(actions)
        {
            for(auto name:{"type.describe","action.open","action.submit","action.take","action.respond","action.cancel","action.describe",
                "communication.open.take","communication.open.cancel","request.describe","request.release"})
                assert(permissions.expose(name,s::exposure::allow)==s::error::success);
            for(auto name:uwvm2::imported::ros::action_contract::transition_actions)
                assert(permissions.expose(uwvm2::utils::container::std_string{name},s::exposure::allow)==s::error::success);
        }
        for(std::size_t i{};i<2;++i)
        {
            external[i]=uwvm2::utils::container::make_unique<external_fixture::owner>(peers[i].manifest);
            input[i].attachment=&external[i]->attachment;input[i].definition=&peers[i].definition;
            for(auto const& definition:peers[i].definition.definitions)
            {
                if(definition.kind==s::communication_configuration::family::context)input[i].context=std::get<c::context_plan>(definition.policy);
                if(definition.kind==s::communication_configuration::family::node)input[i].node=std::get<c::node_plan>(definition.policy);
            }
            input[i].context.authority={71,2*i+1,1};input[i].node.authority={71,2*i+2,1};
            input[i].rpc={65536,65536,262144,65536,5000000000ull,1000000};
            if(graph)input[i].graph_query_bytes=2*(input[i].rpc.request_bytes+input[i].rpc.response_bytes);
            for(auto is_node:{false,true})
            {
                s::grant grant;grant.alias=(is_node?"node.":"context.")+uwvm2::utils::container::to_string(i);grant.resource="sealed.peer";
                grant.binding=actions||canonical?"native-communication":"private-worker";grant.max_resources=1;
                grant.kind=is_node?s::purpose::node:s::purpose::context;grant.actions.insert(is_node?"node.open":"context.open");
                if(graph&&!is_node)grant.actions.insert("context.describe");
                if(canonical&&is_node)for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release","resource.close"})grant.actions.insert(name);
                assert(permissions.configure(std::move(grant))==s::error::success);
            }
            if(graph)
            {s::grant grant;grant.alias="graph."+uwvm2::utils::container::to_string(i);grant.resource="sealed.graph";grant.binding="private-worker";grant.kind=s::purpose::discovery;grant.actions.insert("graph.snapshot");assert(permissions.configure(std::move(grant))==s::error::success);}
            if(topics)
            {
                s::grant grant;grant.alias="topic."+uwvm2::utils::container::to_string(i);grant.resource="sealed.topic";grant.binding=canonical?"native-communication":"private-worker";grant.kind=s::purpose::topic;grant.max_resources=1;
                for(auto name:{"topic.open","topic.publish","qos.describe","qos.liveliness.assert","qos.acknowledgements.wait","topic.reserve","topic.acknowledge","topic.release","resource.close"})grant.actions.insert(name);
                if(canonical)for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release"})grant.actions.insert(name);
                assert(permissions.configure(std::move(grant))==s::error::success);
                s::grant schema;schema.alias="schema."+uwvm2::utils::container::to_string(i);schema.resource="sealed.schema";schema.binding=canonical?"native-communication":"private-worker";
                schema.kind=s::purpose::type;schema.actions.insert("type.describe");assert(permissions.configure(std::move(schema))==s::error::success);
            }
            if(services)
            {
                s::grant grant;grant.alias="service."+uwvm2::utils::container::to_string(i);grant.resource="sealed.service";grant.binding=canonical?"native-communication":"private-worker";grant.kind=s::purpose::service;grant.max_resources=1;
                for(auto name:{"service.open","service.submit","service.respond","service.available","service.abandon","qos.describe","service.reserve","service.acknowledge","service.release","resource.close"})grant.actions.insert(name);
                if(canonical)for(auto name:{"communication.open.take","communication.open.cancel","request.describe","request.release"})grant.actions.insert(name);
                assert(permissions.configure(std::move(grant))==s::error::success);
                for(auto name:{"request.","response."})
                {s::grant schema;schema.alias=name+uwvm2::utils::container::to_string(i);schema.resource="sealed.schema";schema.binding=canonical?"native-communication":"private-worker";
                    schema.kind=s::purpose::type;schema.actions.insert("type.describe");assert(permissions.configure(std::move(schema))==s::error::success);}
            }
            if(actions)
            {
                s::grant grant;grant.alias="action."+uwvm2::utils::container::to_string(i);grant.resource="sealed.action";grant.binding="native-action";grant.kind=s::purpose::action;
                grant.max_resources=1;
                for(auto name:{"action.open","action.submit","action.take","action.respond","action.cancel","action.describe","resource.close",
                    "communication.open.take","communication.open.cancel","request.describe","request.release"})grant.actions.insert(name);
                for(auto name:uwvm2::imported::ros::action_contract::transition_actions)grant.actions.emplace(name);
                assert(permissions.configure(std::move(grant))==s::error::success);
                for(auto name:{"goal.","feedback.","result."})
                {s::grant schema;schema.alias=name+uwvm2::utils::container::to_string(i);schema.resource="sealed.schema";schema.binding="native-communication";
                    schema.kind=s::purpose::type;schema.actions.insert("type.describe");assert(permissions.configure(std::move(schema))==s::error::success);}
            }
        }
        for(std::size_t i{};i<2;++i)
        {
            assert(permissions.enroll(who[i])==s::error::success);s::handle node{},context{};
            assert(permissions.open(who[i],"node."+uwvm2::utils::container::to_string(i),node)==s::error::success);
            assert(permissions.open(who[i],"context."+uwvm2::utils::container::to_string(i),context)==s::error::success);
            if(graph)assert(permissions.open(who[i],"graph."+uwvm2::utils::container::to_string(i),graph_caps[i])==s::error::success);
            if(topics)
            {assert(permissions.open(who[i],"topic."+uwvm2::utils::container::to_string(i),topic_caps[i])==s::error::success);
                assert(permissions.open(who[i],"schema."+uwvm2::utils::container::to_string(i),schema_caps[i])==s::error::success);}
            if(services)
            {assert(permissions.open(who[i],"service."+uwvm2::utils::container::to_string(i),service_caps[i])==s::error::success);
                assert(permissions.open(who[i],"request."+uwvm2::utils::container::to_string(i),schema_caps[i])==s::error::success);
                assert(permissions.open(who[i],"response."+uwvm2::utils::container::to_string(i),response_caps[i])==s::error::success);}
            if(actions)
            {
                assert(permissions.open(who[i],"action."+uwvm2::utils::container::to_string(i),action_caps[i])==s::error::success);
                assert(permissions.open(who[i],"goal."+uwvm2::utils::container::to_string(i),schema_caps[i])==s::error::success);
                assert(permissions.open(who[i],"feedback."+uwvm2::utils::container::to_string(i),feedback_caps[i])==s::error::success);
                assert(permissions.open(who[i],"result."+uwvm2::utils::container::to_string(i),result_caps[i])==s::error::success);
            }
            request[i]={who[i].instance,who[i].generation,node,context,101+i,input[i].context.authority,{}};request[i].request_digest[0]=i+1;
        }
    }
    static c::error admit(void* opaque,std::array<c::activation_origin,2> const& origins,c::activation_guard& output)noexcept
    {
        auto& self=*static_cast<environment*>(opaque);
        if(self.pause_admission.load())
        {self.entered.store(true);while(!self.resume.load())::poll(nullptr,0,1);}
        for(std::size_t i{};i<2;++i)
        {
            if(!origins[i].node||!origins[i].context||!origins[i].request||*origins[i].request!=self.request[i]||
                origins[i].node->authority!=self.input[i].node.authority||origins[i].context->authority!=self.input[i].context.authority)return c::error::denied;
            std::array<s::admission_claim,2> claims{{{self.request[i].node_capability,"node.open",0,false,true},{self.request[i].context_capability,"context.open",0,false,true}}};
            if(self.permissions.admit_group(self.who[i],claims,output.participants[i],false)!=s::error::success)return c::error::denied;
        }
        ++self.admitted;return c::error::ok;
    }
    void ready(w::paired_factory& factory)
    {
        for(std::size_t i{};i<2;++i)assert(factory.create_context(input[i].context,contexts[i])==c::error::ok);
        auto before=factory.inspect();for(auto const& stage:before.stages)assert(!stage.launches);assert(!before.link.active);
        auto bad=request[0];++bad.caller_generation;
        // The first legitimate incarnation is sealed on first stage creation.
        assert(factory.prepare_node(input[0].node,contexts[0],request[0])==c::error::again);
        assert(factory.prepare_node(input[0].node,contexts[0],bad)==c::error::denied);
        auto until=c::now()+15000000000ull;std::array<bool,2> complete{};
        while(!(complete[0]&&complete[1]))
        {
            for(std::size_t i{};i<2;++i)if(!complete[i])
            {auto status=factory.prepare_node(input[i].node,contexts[i],request[i]);assert(status==c::error::ok||status==c::error::again);complete[i]=status==c::error::ok;}
            assert(c::now()<until);if(!(complete[0]&&complete[1]))::poll(nullptr,0,1);
        }
        auto final=factory.inspect();assert(!final.link.active&&admitted==0);
        for(auto const& stage:final.stages)assert(stage.phase==w::stage_phase::attached&&stage.launches==0&&stage.identity_confirmed);
    }
};
/** @brief Real host node leases and queued discovery on the sealed two-worker pair. */
static void qualified_graph(std::array<peer,2> const& peers,bool reserve)
{
    environment env{peers,true};if(!reserve)for(auto& input:env.input)input.graph_query_bytes=0;
    auto factory=uwvm2::utils::container::make_shared<w::paired_factory>();
    assert(factory->configure(env.input,environment::admit,&env,5000000000ull)==c::error::ok);
    struct observation
    {
        environment* env{};std::size_t side{};std::shared_ptr<c::registry> registry;
        c::graph::plan plan;c::operation_token token;
        static c::error admit(void* object,c::operation_token const& current,s::group_lease& held)noexcept
        {
            auto& self=*static_cast<observation*>(object);auto const& expected=self.token;
            if(!current.valid(c::permission::graph_snapshot,c::operation_family::graph,self.plan.authority,self.plan.authority.grant)||
                current.caller_instance!=expected.caller_instance||current.caller_generation!=expected.caller_generation||
                current.capability!=expected.capability||current.count!=expected.count||current.origins!=expected.origins||
                current.retained_origins!=expected.retained_origins)return c::error::denied;
            s::communication_jobs::claim_set claims;
            auto status=self.env->permissions.admit_group(self.env->who[self.side],s::communication_jobs::admission_claims(current,claims),held,false);
            if(status!=s::error::success)return static_cast<c::error>(status);
            auto const& input=self.env->input[self.side];
            auto valid=self.registry->validate_context(input.context.authority,current.origins[2].resource,input.context.selected);
            if(valid==c::error::ok)valid=self.registry->validate_node(input.node.authority,current.origins[1].resource,input.node.selected);
            if(valid!=c::error::ok)held.release();
            return valid;
        }
        static c::error charge(void* object,c::operation_token const& current,std::uint64_t bytes,s::group_lease& held)noexcept
        {
            s::group_lease validated;auto status=admit(object,current,validated);if(status!=c::error::ok)return status;
            auto& self=*static_cast<observation*>(object);s::communication_jobs::claim_set claims;
            return static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],
                s::communication_jobs::admission_claims(current,claims,bytes),held,true));
        }
    };
    std::array<observation,2> state;std::array<s::handle,2> contexts{},nodes{};
    for(std::size_t i{};i<2;++i)
    {
        auto& item=state[i];item.env=&env;item.side=i;item.registry=uwvm2::utils::container::make_shared<c::registry>(factory,c::limits{1,1,0,0,0,0,0});
        assert(item.registry->configure(env.input[i].context)==c::error::ok&&item.registry->configure(env.input[i].node)==c::error::ok);
        assert(item.registry->open_context(env.input[i].context.authority,env.input[i].context.selected,contexts[i])==c::error::ok);
        env.request[i].context=contexts[i];
        for(auto const& definition:peers[i].definition.definitions)if(definition.kind==s::communication_configuration::family::graph)
            item.plan=std::get<s::communication_graph_dispatch::plan>(definition.policy).policy;
        assert(c::valid(item.plan.selected));item.plan.authority={71,100+i,1};item.plan.context=contexts[i];
    }
    auto deadline=c::now()+15000000000ull;std::array<bool,2> complete{};
    while(!(complete[0]&&complete[1]))
    {
        for(std::size_t i{};i<2;++i)if(!complete[i])
        {
            s::group_lease held;std::array<s::admission_claim,2> claims{{{env.request[i].node_capability,"node.open",0,false,true},{env.request[i].context_capability,"context.open",0,false,true}}};
            assert(env.permissions.admit_group(env.who[i],claims,held,false)==s::error::success);
            auto status=state[i].registry->prepare_node(env.input[i].node.authority,env.input[i].context.authority,contexts[i],env.input[i].node.selected,env.request[i]);
            assert(status==c::error::ok||status==c::error::again);complete[i]=status==c::error::ok;
        }
        assert(c::now()<deadline);if(!(complete[0]&&complete[1]))::poll(nullptr,0,1);
    }
    for(std::size_t i{};i<2;++i)assert(state[i].registry->open_node(env.input[i].node.authority,env.input[i].context.authority,contexts[i],env.input[i].node.selected,nodes[i])==c::error::ok);
    assert(factory->inspect().link.active&&env.admitted==1);
    std::array<std::shared_ptr<c::graph::source>,2> sources;
    for(std::size_t i{};i<2;++i)
    {
        auto& item=state[i];auto& token=item.token;
        token.caller_instance=env.who[i].instance;token.caller_generation=env.who[i].generation;
        token.authority=item.plan.authority;token.capability=env.graph_caps[i];token.resource=item.plan.authority.grant;
        token.family=c::operation_family::graph;token.action=c::permission::graph_snapshot;token.count=4;
        token.origins[0]={token.authority,token.capability,token.resource,token.action};
        token.origins[1]={env.input[i].node.authority,env.request[i].node_capability,nodes[i],c::permission::node_open};
        token.origins[2]={env.input[i].context.authority,env.request[i].context_capability,contexts[i],c::permission::context_open};
        token.origins[3]={env.input[i].context.authority,env.request[i].context_capability,contexts[i],c::permission::context_describe};
        token.retained_origins=uwvm2::utils::container::make_shared<int>(i);
        c::node_lease attachment;assert(item.registry->attach_node(env.input[i].node.authority,nodes[i],env.input[i].node.selected,attachment)==c::error::ok);
        auto status=factory->create_graph(item.plan,std::move(attachment),observation::admit,observation::charge,&item,sources[i]);
        assert(status==(reserve?c::error::ok:c::error::too_large));
        if(!reserve){assert(!sources[i]);continue;}
        assert(item.registry->close(env.input[i].node.authority,nodes[i])==c::error::busy);
        std::shared_ptr<c::graph::source> retry;
        assert(item.registry->attach_node(env.input[i].node.authority,nodes[i],env.input[i].node.selected,attachment)==c::error::ok);
        assert(factory->create_graph(item.plan,std::move(attachment),observation::admit,observation::charge,&item,retry)==c::error::ok&&retry==sources[i]);
        auto changed=item.plan;--changed.max_rows;
        assert(item.registry->attach_node(env.input[i].node.authority,nodes[i],env.input[i].node.selected,attachment)==c::error::ok);
        assert(factory->create_graph(changed,std::move(attachment),observation::admit,observation::charge,&item,retry)==c::error::denied);
    }
    if(reserve)
    {
        std::array<uwvm2::utils::container::std_vector<c::graph::entity>,2> rows;complete={};deadline=c::now()+15000000000ull;
        while(!(complete[0]&&complete[1]))
        {
            for(std::size_t i{};i<2;++i)if(!complete[i])
            {
                auto status=sources[i]->capture(state[i].plan,rows[i],state[i].token);
                assert(status==c::error::ok||status==c::error::again);
                if(status==c::error::ok)for(auto const& row:rows[i])if(row.type==c::graph::kind::node)complete[i]=true;
            }
            auto progress=factory->pump_graphs();assert(progress==c::error::ok||progress==c::error::again);
            assert(c::now()<deadline);if(!(complete[0]&&complete[1]))::poll(nullptr,0,1);
        }
        // The queue has no ambient authority: closing the original context cap
        // before its supervisor sends prevents another native observation.
        assert(sources[0]->capture(state[0].plan,rows[0],state[0].token)==c::error::again);
        assert(env.permissions.close(env.who[0],env.request[0].context_capability)==s::error::success);
        auto progress=factory->pump_graphs();assert(progress==c::error::ok||progress==c::error::again);
        assert(sources[0]->capture(state[0].plan,rows[0],state[0].token)==c::error::bad_handle);
    }
    factory->retire();assert(!factory->cleanup_confirmed()&&factory->pump_graphs()==c::error::cancelled);
    if(reserve)
    {
        uwvm2::utils::container::std_vector<c::graph::entity> rows;
        assert(sources[1]->capture(state[1].plan,rows,state[1].token)!=c::error::ok);
    }
}
#if defined(UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY)
int communication_factory_fixture_entry(int argc,char** argv)
#else
int main(int argc,char** argv)
#endif
{
    ::signal(SIGPIPE,SIG_IGN);std::array<peer,2> peers;int at=1;for(auto& value:peers)value.parse(argc,argv,at);assert(at==argc);
    {
        environment env{peers};w::paired_factory factory;
        assert(factory.configure(env.input,nullptr,&env,5000000000ull)==c::error::invalid&&!factory.inspect().configured);
        auto wrong=env.input;wrong[1].node.authority.environment=72;
        assert(factory.configure(wrong,environment::admit,&env,5000000000ull)==c::error::denied&&!factory.inspect().configured);
        assert(factory.configure(env.input,environment::admit,&env,5000000000ull)==c::error::ok);env.ready(factory);
        assert(factory.configure(env.input,environment::admit,&env,5000000000ull)==c::error::denied);
        assert(factory.create_node(env.input[0].node,env.contexts[0],env.nodes[0])==c::error::ok&&!factory.inspect().link.active);
        assert(factory.create_node(env.input[1].node,env.contexts[1],env.nodes[1])==c::error::ok&&factory.inspect().link.active&&env.admitted==1);
        auto forked=::fork();assert(forked>=0);if(!forked){assert(factory.prepare_node(env.input[0].node,env.contexts[0],env.request[0])==c::error::denied);::_exit(0);}
        int status{};assert(::waitpid(forked,&status,0)==forked&&WIFEXITED(status)&&!WEXITSTATUS(status));
        factory.capability_closed(env.request[0].node_capability);assert(factory.inspect().stopping&&factory.inspect().link.fenced);
        factory.retire();auto final=factory.inspect();assert(final.drained&&!factory.cleanup_confirmed());for(auto const& stage:final.stages)assert(stage.cleanup_debt);
    }
    {
        environment env{peers};w::paired_factory factory;assert(factory.configure(env.input,environment::admit,&env,5000000000ull)==c::error::ok);env.ready(factory);
        assert(factory.create_node(env.input[0].node,env.contexts[0],env.nodes[0])==c::error::ok);
        // Deliberately omit the lifecycle hint: final CURRENT authority still
        // rejects activation even though both contexts and pidfds remain live.
        assert(env.permissions.close(env.who[0],env.request[0].context_capability)==s::error::success&&env.contexts[0]->live());
        assert(factory.create_node(env.input[1].node,env.contexts[1],env.nodes[1])==c::error::denied&&!factory.inspect().link.active&&env.admitted==0);
        factory.retire();assert(!factory.cleanup_confirmed());
    }
    {
        environment env{peers};w::paired_factory factory;assert(factory.configure(env.input,environment::admit,&env,5000000000ull)==c::error::ok);env.ready(factory);
        assert(factory.create_node(env.input[0].node,env.contexts[0],env.nodes[0])==c::error::ok);env.pause_admission=true;
        // Hold authority on this thread before the other thread enters the
        // callback. inspect() proves the callback released the factory mutex;
        // otherwise this creates the real authority -> factory inversion.
        s::lease held;assert(env.permissions.admit(env.who[0],env.request[0].node_capability,"node.open",0,held,false)==s::error::success);
        struct work{environment* env;w::paired_factory* factory;c::error result{};} task{&env,&factory};pthread_t thread;
        assert(!::pthread_create(&thread,nullptr,[](void* opaque)->void*
        {auto& item=*static_cast<work*>(opaque);item.result=item.factory->create_node(item.env->input[1].node,item.env->contexts[1],item.env->nodes[1]);return nullptr;},&task));
        auto until=c::now()+1000000000ull;while(!env.entered.load()){assert(c::now()<until);::poll(nullptr,0,1);}
        assert(!factory.inspect().link.active);
        assert(env.permissions.revoke("node.0",[&](std::string_view){factory.retire_owner(env.input[0].node.authority);})==s::error::success);
        env.resume=true;held.release();assert(!::pthread_join(thread,nullptr));
        assert(task.result==c::error::denied&&!factory.inspect().link.active);factory.retire();assert(!factory.cleanup_confirmed());
    }
    {
        environment env{peers};w::paired_factory factory;assert(factory.configure(env.input,environment::admit,&env,5000000000ull)==c::error::ok);
        assert(factory.create_context(env.input[0].context,env.contexts[0])==c::error::ok);
        assert(factory.prepare_node(env.input[0].node,env.contexts[0],env.request[0])==c::error::again);
        factory.retire();auto final=factory.inspect();assert(final.drained&&!final.cleanup_confirmed&&final.stages[0].cleanup_debt&&!final.stages[0].identity_confirmed);
    }
    qualified_graph(peers,false);qualified_graph(peers,true);
    std::puts("PASS actual sealed factory: zero preflight effects, exact incarnation, bilateral current authority, concurrent authority/factory lock order, explicit graph capacity, actual host node leases, nonblocking native discovery, current query authority, cap-close fencing, stop/drain, local drain and retained early unknown debt");
    return 0;
}
