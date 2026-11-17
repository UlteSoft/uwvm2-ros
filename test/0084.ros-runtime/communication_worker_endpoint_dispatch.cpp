/** @file @brief Real sealed ROS topics/services through canonical creation and operation dispatch.
 * @details Context is logical; node and endpoint creation are asynchronous. It does not register VM
 * imports or claim a scheduler latency bound. All endpoint authority is minted
 * by the actual dispatcher, including every worker send and receive settlement.
 */
#define UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY
#include "communication_worker_provider.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_endpoint_open.h>
#include <uwvm2/imported/ros/scene/communication_worker_activation.h>
#include <uwvm2/imported/ros/scene/communication_node_open_provider.h>
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace receive=uwvm2::imported::ros::communication_receive_contract;
namespace wire=s::wire;
template<class F> static bytes encode(F&& fill)
{std::array<std::uint8_t,4096> storage;wire::writer out{storage};assert(fill(out));return {out.written().begin(),out.written().end()};}
struct side
{
    environment* env{};std::size_t index{};bool service{};
    std::shared_ptr<s::communication_dispatch> dispatch;
    std::shared_ptr<s::communication_node_open_provider> node_provider;
    std::shared_ptr<w::endpoint_open_provider> source;
    c::handle completed{};
    c::topic_plan topic_policy;c::service_plan service_policy;std::array<c::type_plan,2> schemas;
    creation::plan opening;
    c::handle context{},node{},node_request{},resource{},request{},cap{};bytes node_body;std::uint64_t sequence{},ack_sequence{};
    c::profile const& profile()const noexcept{return service?service_policy.selected:topic_policy.selected;}
    s::error call(contract::operation action,wire::bytes input,bytes& output,bool copy=true)
    {std::size_t required{};return dispatch->invoke(env->who[index],action,cap,input,65536,required,[&](wire::bytes value){output.assign(value.begin(),value.end());return copy;});}
    s::error receive_call(receive::operation action,wire::bytes input,bytes& output,bool copy=true)
    {std::size_t required{};return dispatch->invoke_receive(env->who[index],action,cap,input,65536,required,[&](wire::bytes value){output.assign(value.begin(),value.end());return copy;});}
};
struct delivery
{receive::reference reference;c::handle correlation{};bytes payload;};
static delivery decode_delivery(side const& from,bytes const& data,std::uint64_t generation)
{
    wire::reader in{data};delivery result;auto& ref=result.reference;
    std::uint64_t deadline{},source{},actual{},sequence{},lost{};bool present{};wire::bytes ignored,body;std::string_view kind,type,encoding;
    assert(in.u64(ref.endpoint)&&ref.endpoint==from.resource&&in.u64(ref.provider_generation)&&ref.provider_generation==generation&&in.u64(ref.sequence)&&ref.sequence&&in.u64(ref.nonce)&&ref.nonce&&in.u64(deadline)&&deadline);
    assert(in.u64(source)&&source==ref.endpoint&&in.u64(actual)&&actual==generation&&in.u64(sequence)&&sequence==ref.sequence);
    assert(in.boolean(present)&&!present&&in.boolean(present)&&!present&&in.boolean(present)&&(!present||in.blob(ignored))&&in.u64(lost));
    assert(in.u64(result.correlation)&&in.text(kind)&&kind==(from.service?(from.index?"service.request":"service.response"):"topic.message")&&
        in.text(type)&&in.text(encoding)&&in.blob(body)&&in.done());
    auto const* schema=&from.schemas[from.service&&!from.index?1:0];
    assert(schema&&type==schema->type_identity&&encoding==schema->encoding&&schema->validate(body));result.payload.assign(body.begin(),body.end());return result;
}
static void qualify(std::array<peer,2> const& peers,bool service)
{
    environment env{peers,false,!service,service,false,true};auto retained=uwvm2::utils::container::make_shared<std::array<side,2>>();auto& sides=*retained;
    std::array<uwvm2::utils::container::std_unique_ptr<s::communication_open_dispatch>,2> controls,node_controls;
    auto factory=uwvm2::utils::container::make_shared<w::paired_factory>();
    for(std::size_t i{};i<2;++i)
    {
        auto& item=sides[i];item.env=&env;item.index=i;item.service=service;item.cap=service?env.service_caps[i]:env.topic_caps[i];
        auto suffix=uwvm2::utils::container::to_string(i);auto alias=(service?"service.":"topic.")+suffix;
        for(std::size_t n{};n<peers[i].definition.definitions.size();++n)
        {
            auto const& definition=peers[i].definition.definitions[n];
            if(definition.kind==(service?s::communication_configuration::family::service:s::communication_configuration::family::topic)&&
                cfg::profile_of(definition).identity==(service?(i?"service-server":"service-client"):(i?"subscription":"publisher")))
            {if(service)item.service_policy=std::get<c::service_plan>(definition.policy);else item.topic_policy=std::get<c::topic_plan>(definition.policy);item.opening.worker_binding=n+1;}
        }
        assert(c::valid(item.profile()));
        for(std::size_t n{};n<(service?2u:1u);++n)
        {
            auto const& profile=service?(n?item.service_policy.response_type:item.service_policy.request_type):item.topic_policy.type;
            auto schema=cfg::find(peers[i].definition,s::communication_configuration::family::type,profile);assert(schema);item.schemas[n]=std::get<c::type_plan>(schema->policy);
        }
        item.dispatch=uwvm2::utils::container::make_shared<s::communication_dispatch>(env.permission_owner,71,1,
            s::communication_dispatch::limits{{1,1,service?0u:1u,service?2u:1u,service?item.service_policy.reserved_bytes:item.topic_policy.queue_bytes,65536,service?1u:0u},5,65536,65536,524288,8,15000000000});
        assert(item.dispatch->bind("context."+suffix,env.input[i].context)==s::error::success&&item.dispatch->bind("node."+suffix,env.input[i].node)==s::error::success);
        assert(item.dispatch->bind((service?"request.":"schema.")+suffix,item.schemas[0])==s::error::success);
        if(service)assert(item.dispatch->bind("response."+suffix,item.schemas[1])==s::error::success);
        assert((service?item.dispatch->bind(alias,item.service_policy):item.dispatch->bind(alias,item.topic_policy))==s::error::success);
        assert(item.dispatch->describe_plan("context."+suffix,env.input[i].context)==s::error::success&&item.dispatch->describe_plan("node."+suffix,env.input[i].node)==s::error::success);
        assert((service?item.dispatch->describe_plan(alias,item.service_policy):item.dispatch->describe_plan(alias,item.topic_policy))==s::error::success);
        auto& plan=item.opening;plan.authority=service?item.service_policy.authority:item.topic_policy.authority;plan.selected=item.profile();
        plan.kind=service?opens::endpoint_kind::service:opens::endpoint_kind::topic;plan.provider_generation=peers[i].definition.network.generation;plan.carrier_bytes=2048;plan.max_deadline_ns=4000000000;
        item.source=uwvm2::utils::container::make_shared<w::endpoint_open_provider>();
        assert(item.source->configure(alias,plan.kind,item.dispatch,factory)==c::error::ok);
        auto admission=item.source->admission_context();assert(admission);
        if(service){env.input[i].service_openings={&plan,1};env.input[i].service_carrier_bytes=4096;env.input[i].service_bytes=4u*1024u*1024u;
            env.input[i].admit_service=w::dispatch_admission::admit_open;env.input[i].charge_service=w::dispatch_admission::charge_open;
            env.input[i].service_authority=admission.get();env.input[i].service_authority_owner=admission;}
        else{env.input[i].topic_openings={&plan,1};env.input[i].topic_carrier_bytes=4096;env.input[i].topic_bytes=4u*1024u*1024u;
            env.input[i].admit_topic=w::dispatch_admission::admit_open;env.input[i].charge_topic=w::dispatch_admission::charge_open;
            env.input[i].topic_authority=admission.get();env.input[i].topic_authority_owner=admission;}
    }
    auto activation=uwvm2::utils::container::make_shared<w::activation_admission>(
        std::array<std::shared_ptr<s::communication_dispatch>,2>{sides[0].dispatch,sides[1].dispatch});
    assert(factory->configure(env.input,w::activation_admission::admit,&env,5000000000,activation)==c::error::invalid&&!factory->inspect().configured);
    assert(factory->configure(env.input,w::activation_admission::admit,activation.get(),5000000000,activation)==c::error::ok);
    for(auto& item:sides)
    {
        auto i=item.index;
        s::communication_dispatch::observation_clock clock{nullptr,[](void*,contract::clock_epoch& epoch,std::int64_t& time)noexcept{epoch={999,1};time=c::now();return time>0;}};
        assert(item.dispatch->prepare_all_async(factory,clock)==s::error::success);
        auto body=encode([&](auto& out){return contract::encode_profile(out,env.input[i].context.selected);});std::size_t required{};
        assert(item.dispatch->invoke(env.who[i],contract::operation::context_open,env.request[i].context_capability,body,8,required,[&](wire::bytes value){wire::reader in{value};return in.u64(item.context)&&item.context&&in.done();})==s::error::success);
        item.node_body=encode([&](auto& out){return out.u64(env.request[i].context_capability)&&out.u64(item.context)&&contract::encode_profile(out,env.input[i].node.selected)&&out.u64(1)&&out.u64(c::now()+14000000000);});
        env.request[i]={env.who[i].instance,env.who[i].generation,env.request[i].node_capability,env.request[i].context_capability,item.context,env.input[i].context.authority,s::buffers::hash(item.node_body)};
        std::uint64_t node_binding{};for(std::size_t n{};n<peers[i].definition.definitions.size();++n)
            if(peers[i].definition.definitions[n].kind==s::communication_configuration::family::node)node_binding=n+1;
        item.node_provider=uwvm2::utils::container::make_shared<s::communication_node_open_provider>();
        assert(item.node_provider->configure("node."+uwvm2::utils::container::to_string(i),{env.input[i].node.authority,env.input[i].node.selected,
            opens::endpoint_kind::node,node_binding,peers[i].definition.network.generation,2048,15000000000},item.dispatch,factory)==c::error::ok);
        node_controls[i]=uwvm2::utils::container::make_unique<s::communication_open_dispatch>(env.permission_owner,s::communication_open_dispatch::limits{1,1040,2});
        assert(node_controls[i]->bind("node."+uwvm2::utils::container::to_string(i),env.input[i].node.authority,opens::endpoint_kind::node,item.node_provider)==s::error::success&&node_controls[i]->prepare()==s::error::success);
        bytes first,retry;
        auto open_node=[&](bool copied,bytes& output){return item.node_provider->invoke_open(env.who[i],env.request[i].node_capability,item.node_body,48,required,[&](wire::bytes body)
            {output.assign(body.begin(),body.end());wire::reader in{body};std::uint64_t key{};assert(in.u64(key)&&key==1&&in.u64(item.node_request)&&item.node_request);return copied;});};
        assert(open_node(false,first)==s::error::io&&open_node(true,retry)==s::error::success&&first==retry);
        controls[i]=uwvm2::utils::container::make_unique<s::communication_open_dispatch>(env.permission_owner,s::communication_open_dispatch::limits{1,1040,2});
        assert(controls[i]->bind((service?"service.":"topic.")+uwvm2::utils::container::to_string(i),item.opening.authority,item.opening.kind,item.source)==s::error::success&&controls[i]->prepare()==s::error::success);
    }
    auto inert=factory->inspect();assert(!inert.link.active);for(auto const& stage:inert.stages)assert(stage.launches==0);
    {
        std::array<c::activation_origin,2> origins{{{&env.input[0].node,&env.input[0].context,&env.request[0]},
            {&env.input[1].node,&env.input[1].context,&env.request[1]}}};
        c::activation_guard held;assert(w::activation_admission::admit(activation.get(),origins,held)==c::error::ok&&held);
        held={};auto forged=env.request[1];++forged.context;origins[1].request=&forged;
        assert(w::activation_admission::admit(activation.get(),origins,held)==c::error::denied&&!held.participants[0]&&!held.participants[1]);
        forged=env.request[1];forged.request_digest[0]^=1;
        assert(w::activation_admission::admit(activation.get(),origins,held)==c::error::denied&&!held.participants[0]&&!held.participants[1]);
    }
    auto deadline=c::now()+15000000000ull;
    while(!sides[0].node||!sides[1].node)
    {
        for(auto& item:sides)if(!item.node)
        {
            auto progressed=item.node_provider->pump();assert(progressed==c::error::ok||progressed==c::error::again);
            std::size_t required{};auto i=item.index;auto body=encode([&](auto& out){return out.u64(item.node_request);});
            auto status=node_controls[i]->invoke(env.who[i],s::communication_open_dispatch::operation::take,env.request[i].node_capability,body,64,required,
                [&](wire::bytes value){wire::reader in{value};std::uint64_t field{};for(unsigned n{};n<7;++n)if(!in.u64(field))return false;
                    return field==1&&in.u64(item.node)&&item.node&&item.node!=item.node_request&&in.done();});
            assert(status==s::error::success||status==s::error::again);
        }
        assert(c::now()<deadline);if(!sides[0].node||!sides[1].node)::poll(nullptr,0,1);
    }
    assert(factory->inspect().link.active);
    for(auto& item:sides)
    {
        auto i=item.index;
        auto body=encode([&](auto& out){return out.u64(env.request[i].node_capability)&&out.u64(item.node)&&out.u64(service?2:1)&&out.u64(env.schema_caps[i])&&(!service||out.u64(env.response_caps[i]))&&
            contract::encode_profile(out,item.profile())&&out.text(service?item.service_policy.resolved_name:item.topic_policy.resolved_name)&&out.text(service?item.service_policy.service_type:item.schemas[0].type_identity)&&
            out.text(service?(i?"server":"client"):(i?"subscription":"publisher"))&&out.u64(1)&&out.u64(c::now()+std::min<std::uint64_t>(3500000000,service?item.service_policy.max_deadline_ns:item.topic_policy.max_deadline_ns));});
        std::size_t required{};bytes first,second;
        auto invoke=[&](bool deliver,bytes& target){return item.source->invoke_open(env.who[i],item.cap,body,48,required,[&](wire::bytes value)
            {target.assign(value.begin(),value.end());wire::reader in{value};std::uint64_t key{};assert(in.u64(key)&&key==1&&in.u64(item.request)&&item.request);return deliver;});};
        assert(invoke(false,first)==s::error::io&&invoke(true,second)==s::error::success&&first==second&&item.request&&!item.resource);
    }
    auto pump=[&]{for(auto& item:sides){c::handle completed{};auto status=item.source->pump(&completed);
        assert(status==c::error::ok||status==c::error::again);if(completed)item.completed=completed;}};
    for(auto& item:sides)
    {
        auto body=encode([&](auto& out){return out.u64(item.request);});std::size_t required{};deadline=c::now()+4000000000;
        auto take=[&](bool deliver){return controls[item.index]->invoke(env.who[item.index],s::communication_open_dispatch::operation::take,item.cap,body,64,required,[&](wire::bytes value){wire::reader in{value};std::uint64_t field{};for(unsigned n{};n<7;++n)assert(in.u64(field));
            assert(field==static_cast<std::uint64_t>(item.opening.kind)&&in.u64(item.resource)&&item.resource&&item.resource!=item.request&&in.done());return deliver;});};
        s::error status{};while((status=take(false))==s::error::again){pump();assert(c::now()<deadline);::poll(nullptr,0,1);}
        assert(status==s::error::io&&take(true)==s::error::success);
    }
    // Discovery allowance belongs to this semantic fixture, never a latency bound.
    auto discovery=c::now()+2000000000;while(c::now()<discovery){pump();::poll(nullptr,0,2);}
    auto settle=[&](side& item,c::handle request)
    {
        auto until=c::now()+3000000000;
        while(item.completed!=request){pump();assert(c::now()<until);if(item.completed!=request)::poll(nullptr,0,1);}
    };
    auto mutate=[&](side& item,wire::bytes payload,c::handle correlation=0)
    {
        auto schema=&item.schemas[correlation?1:0];
        auto body=encode([&](auto& out){return out.u64(item.resource)&&out.u64(++item.sequence)&&out.u64(item.resource)&&(!correlation||out.u64(correlation))&&
            contract::encode_message(out,{schema->type_identity,schema->encoding,payload})&&out.u64(c::now()+900000000);});
        auto action=service?(correlation?contract::operation::service_respond:contract::operation::service_submit):contract::operation::topic_publish;bytes receipt;
        assert(item.call(action,body,receipt)==s::error::success);wire::reader read{receipt};c::handle request{};assert(read.u64(request)&&request);
        settle(item,request);assert(item.call(action,body,receipt)==s::error::success);return request;
    };
    auto take=[&](side& item)
    {
        auto end=c::now()+6000000000;bytes output;auto action=service?receive::operation::service_reserve:receive::operation::topic_reserve;
        while(true)
        {
            auto body=encode([&](auto& out){return out.u64(item.resource)&&out.u64(c::now()+900000000);});auto status=item.receive_call(action,body,output,false);
            while(status==s::error::again)
            {pump();assert(c::now()<end);status=item.receive_call(action,body,output,false);if(status==s::error::again)::poll(nullptr,0,1);}
            if(status==s::error::io)
            {auto first=output;assert(item.receive_call(action,body,output)==s::error::success&&first==output);return decode_delivery(item,output,peers[item.index].definition.network.generation);}
            assert(status==s::error::again&&c::now()<end);pump();::poll(nullptr,0,1);
        }
    };
    auto ack=[&](side& item,delivery const& delivered)
    {
        auto ref=delivered.reference;auto body=encode([&](auto& out){return out.u64(item.resource)&&out.u64(++item.ack_sequence)&&out.u64(ref.endpoint)&&out.u64(ref.provider_generation)&&out.u64(ref.sequence)&&out.u64(ref.nonce);});
        auto action=service?receive::operation::service_acknowledge:receive::operation::topic_acknowledge;bytes receipt;auto end=c::now()+3000000000;auto status=item.receive_call(action,body,receipt);
        while(status==s::error::again){pump();assert(c::now()<end);status=item.receive_call(action,body,receipt);if(status==s::error::again)::poll(nullptr,0,1);}
        assert(status==s::error::success&&item.receive_call(action,body,receipt)==s::error::success);
    };
    auto& client=sides[0];auto& server=sides[1];
    auto payload=service?bytes{1}:encode([](auto& out){return out.text("canonical isolated topic");});
    auto submitted=mutate(client,payload);auto incoming=take(server);assert(incoming.payload==payload);ack(server,incoming);
    if(service)
    {
        auto response=encode([](auto& out){std::array<std::uint8_t,1> success{1};return out.encoded(success)&&out.text("canonical isolated service");});
        mutate(server,response,incoming.correlation);auto result=take(client);assert(result.correlation==submitted&&result.payload==response);ack(client,result);
    }
    // Cleanup requires its own current action but cannot depend on live schemas.
    assert(env.permissions.close(env.who[0],env.schema_caps[0])==s::error::success);
    for(auto& item:sides)
    {
        auto end=c::now()+3000000000;auto close=[&]{return item.dispatch->close(env.who[item.index],item.cap,item.resource);};auto status=close();
        while(status==s::error::again){pump();assert(c::now()<end);status=close();if(status==s::error::again)::poll(nullptr,0,1);}
        assert(status==s::error::success);pump();
        auto body=encode([&](auto& out){return out.u64(item.request);});std::size_t required{};
        auto release=[&](bool copied){return controls[item.index]->invoke(env.who[item.index],s::communication_open_dispatch::operation::release,item.cap,body,8,required,
            [&](wire::bytes value){wire::reader in{value};bool released{};assert(in.boolean(released)&&released&&in.done());return copied;});};
        assert(release(false)==s::error::io&&release(true)==s::error::success);pump();
        assert(release(true)==s::error::success);pump();
        assert(item.dispatch->close(env.who[item.index],env.request[item.index].node_capability,item.node)==s::error::success);
    }
    factory->retire();assert(!factory->cleanup_confirmed());
    for(auto& item:sides)
    {
        assert(item.node_provider->reconcile_cleanup()==c::error::busy);std::size_t required{};
        auto body=encode([&](auto& out){return out.u64(item.node_request);});
        assert(node_controls[item.index]->invoke(env.who[item.index],s::communication_open_dispatch::operation::release,env.request[item.index].node_capability,
            body,8,required,[](wire::bytes){return true;})==s::error::again);
    }
    // Both factory-owned and externally retained contexts are weak: dropping
    // dispatch owners after positive retirement must not leak a reference cycle.
    for(auto& item:sides)
    {
        auto i=item.index;std::weak_ptr<s::communication_dispatch> previous=item.dispatch;auto admission=item.source->admission_context();
        controls[i].reset();node_controls[i].reset();item.source.reset();item.node_provider.reset();item.dispatch.reset();
        assert(previous.expired());s::group_lease held;c::operation_token token;
        assert(w::dispatch_admission::admit(admission.get(),token,held)==c::error::denied&&!held);
    }
}
int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<peer,2> peers;int at=1;
    for(auto& item:peers)item.parse(argc,argv,at);
    assert(at==argc);
    qualify(peers,false);qualify(peers,true);
    std::puts("PASS actual sealed canonical endpoints: exact dispatcher origins, production asynchronous coordinator, native String and SetBool, failed-copy creation/publication/release recovery, explicit ACK, retained remote cleanup debt");
}
