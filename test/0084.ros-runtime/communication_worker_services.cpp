/** @file @brief Actual isolated ROS service creation, host adoption and SetBool request/response exchange. */
#define UWVM_COMMUNICATION_FACTORY_FIXTURE_ONLY
#include "communication_worker_provider.cpp"
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace wire=s::wire;

struct service_state
{
    environment* env{};std::size_t side{};
    std::shared_ptr<c::registry> registry;std::shared_ptr<w::service_endpoint> endpoint;
    c::service_plan policy;c::type_plan schema,response_schema;creation::plan creation_plan;creation::identity opening;
    c::handle context{},node{},resource{},request{};c::node_lease parent;c::endpoint_slot slot;
    std::shared_ptr<int> retained;unsigned charges{},validations{};
    c::operation_token token(c::permission action)const
    {
        c::operation_token value;value.caller_instance=env->who[side].instance;value.caller_generation=env->who[side].generation;
        value.authority=policy.authority;value.capability=env->service_caps[side];value.resource=resource;value.family=c::operation_family::service;value.action=action;
        value.count=5;value.retained_origins=retained;value.origins[0]={policy.authority,value.capability,resource,action};
        value.origins[1]={env->input[side].node.authority,env->request[side].node_capability,node,c::permission::node_open};
        value.origins[2]={env->input[side].context.authority,env->request[side].context_capability,context,c::permission::context_open};
        value.origins[3]={schema.authority,env->schema_caps[side],0,c::permission::type_describe};value.origins[4]={response_schema.authority,env->response_caps[side],0,c::permission::type_describe};return value;
    }
    c::error actual_origins()const noexcept
    {
        auto const& input=env->input[side];auto status=registry->validate_context(input.context.authority,context,input.context.selected);
        return status==c::error::ok?registry->validate_node(input.node.authority,node,input.node.selected):status;
    }
    static c::error admit(void* opaque,c::operation_token const& current,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<service_state*>(opaque);auto expected=self.token(current.action);
        if(!current.valid(current.action,c::operation_family::service,self.policy.authority,self.resource,false)||
            current.caller_instance!=expected.caller_instance||current.caller_generation!=expected.caller_generation||
            current.origins!=expected.origins||current.retained_origins!=self.retained)return c::error::denied;
        s::communication_jobs::claim_set claims;
        auto status=static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],s::communication_jobs::admission_claims(current,claims),held,false));
        if(status!=c::error::ok)return status;
        status=self.actual_origins();if(status!=c::error::ok)held.release();else ++self.validations;return status;
    }
    static c::error charge(void* opaque,c::operation_token const& current,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<service_state*>(opaque);s::group_lease validate;auto status=admit(opaque,current,validate);if(status!=c::error::ok)return status;
        s::communication_jobs::claim_set claims;
        status=static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],s::communication_jobs::admission_claims(current,claims,bytes),held));
        if(status==c::error::ok)++self.charges;
        return status;
    }
    static c::error admit_open(void* opaque,creation::identity const& current,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<service_state*>(opaque);if(current!=self.opening)return c::error::denied;
        creation::claim_set claims;auto status=static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],
            creation::admission_claims(current,opens::endpoint_kind::service,claims),held,false));
        if(status!=c::error::ok)return status;
        status=self.actual_origins();if(status!=c::error::ok)held.release();return status;
    }
    static c::error charge_open(void* opaque,creation::identity const& current,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        s::group_lease validated;auto status=admit_open(opaque,current,validated);if(status!=c::error::ok)return status;
        auto& self=*static_cast<service_state*>(opaque);creation::claim_set claims;
        return static_cast<c::error>(self.env->permissions.admit_group(self.env->who[self.side],creation::admission_claims(current,opens::endpoint_kind::service,claims,bytes),held));
    }
};

int main(int argc,char** argv)
{
    ::signal(SIGPIPE,SIG_IGN);std::array<peer,2> peers;int at=1;for(auto& item:peers)item.parse(argc,argv,at);assert(at==argc);
    environment env{peers,false,false,true};std::array<service_state,2> state;
    for(std::size_t i{};i<2;++i)
    {
        auto& value=state[i];value.env=&env;value.side=i;value.retained=uwvm2::utils::container::make_shared<int>(i);
        auto const& definitions=peers[i].definition.definitions;
        for(std::size_t n{};n<definitions.size();++n)if(definitions[n].kind==s::communication_configuration::family::service&&
            cfg::profile_of(definitions[n]).identity==(i?"service-server":"service-client"))
        {value.policy=std::get<c::service_plan>(definitions[n].policy);value.creation_plan.worker_binding=n+1;}
        assert(c::valid(value.policy.selected));value.policy.authority={71,100+i,1};
        auto type=cfg::find(peers[i].definition,s::communication_configuration::family::type,value.policy.request_type);assert(type);
        value.schema=std::get<c::type_plan>(type->policy);value.schema.authority={71,200+i,1};
        auto response=cfg::find(peers[i].definition,s::communication_configuration::family::type,value.policy.response_type);assert(response);
        value.response_schema=std::get<c::type_plan>(response->policy);value.response_schema.authority={71,300+i,1};
        value.creation_plan.authority=value.policy.authority;value.creation_plan.selected=value.policy.selected;value.creation_plan.kind=opens::endpoint_kind::service;
        value.creation_plan.provider_generation=peers[i].definition.network.generation;value.creation_plan.carrier_bytes=2048;value.creation_plan.max_deadline_ns=4000000000;
        env.input[i].service_openings={&value.creation_plan,1};env.input[i].service_carrier_bytes=4096;env.input[i].service_bytes=4u*1024u*1024u;
        env.input[i].admit_service=service_state::admit_open;env.input[i].charge_service=service_state::charge_open;env.input[i].service_authority=&value;
    }
    auto factory=uwvm2::utils::container::make_shared<w::paired_factory>();
    auto insufficient=env.input;insufficient[0].service_bytes=1;
    assert(factory->configure(insufficient,environment::admit,&env,5000000000)==c::error::too_large&&!factory->inspect().configured);
    assert(factory->configure(env.input,environment::admit,&env,5000000000)==c::error::ok);
    for(auto& item:state)
    {
        auto i=item.side;item.registry=uwvm2::utils::container::make_shared<c::registry>(factory,
            c::limits{1,1,0,2,item.policy.reserved_bytes,std::max(item.policy.max_request_bytes,item.policy.max_response_bytes),1});
        assert(item.registry->configure(env.input[i].context)==c::error::ok&&item.registry->configure(env.input[i].node)==c::error::ok);
        assert(item.registry->configure(item.schema)==c::error::ok&&item.registry->configure(item.response_schema)==c::error::ok&&item.registry->configure(item.policy)==c::error::ok);
        assert(item.registry->open_context(env.input[i].context.authority,env.input[i].context.selected,item.context)==c::error::ok);
        env.request[i].context=item.context;
    }
    auto deadline=c::now()+15000000000ull;std::array<bool,2> ready{};
    while(!(ready[0]&&ready[1]))
    {
        for(auto& item:state)if(!ready[item.side])
        {
            auto i=item.side;auto status=item.registry->prepare_node(env.input[i].node.authority,env.input[i].context.authority,item.context,env.input[i].node.selected,env.request[i]);
            assert(status==c::error::ok||status==c::error::again);ready[i]=status==c::error::ok;
        }
        assert(c::now()<deadline);if(!(ready[0]&&ready[1]))::poll(nullptr,0,1);
    }
    for(auto& item:state)
    {auto i=item.side;assert(item.registry->open_node(env.input[i].node.authority,env.input[i].context.authority,item.context,env.input[i].node.selected,item.node)==c::error::ok);}
    assert(factory->inspect().link.active);
    for(auto& item:state)
    {
        auto i=item.side;assert(item.registry->attach_node(env.input[i].node.authority,item.node,env.input[i].node.selected,item.parent)==c::error::ok);
        assert(item.registry->prepare_adoption(item.policy.authority,item.parent,item.policy.selected,c::operation_family::service,item.slot)==c::error::ok);
        item.resource=item.slot.resource();auto& origin=item.opening;origin.who=env.who[i];origin.authority=item.policy.authority;origin.key=1;origin.deadline=c::now()+3500000000;
        origin.count=5;origin.origins[0]={creation::dependency::operation,item.policy.authority,env.service_caps[i],0};
        origin.origins[1]={creation::dependency::node,env.input[i].node.authority,env.request[i].node_capability,item.node};
        origin.origins[2]={creation::dependency::context,env.input[i].context.authority,env.request[i].context_capability,item.context};
        origin.origins[3]={creation::dependency::schema,item.schema.authority,env.schema_caps[i],0};
        origin.origins[4]={creation::dependency::schema,item.response_schema.authority,env.response_caps[i],0};
        std::array<std::uint8_t,2048> storage;wire::writer out{storage};
        assert(out.u64(env.request[i].node_capability)&&out.u64(item.node)&&out.u64(2)&&out.u64(env.schema_caps[i])&&out.u64(env.response_caps[i])&&contract::encode_profile(out,item.policy.selected)&&
            out.text(item.policy.resolved_name)&&out.text(item.policy.service_type)&&out.text(i?"server":"client")&&out.u64(1)&&out.u64(origin.deadline));
        creation::view receipt,retry;
        // A valid origin token cannot make a contradictory audit carrier valid.
        wire::writer corrupt{storage};assert(corrupt.u64(env.request[i].context_capability));
        assert(factory->open_service_request(item.parent,origin,out.written(),receipt)==c::error::denied&&!receipt.receipt.request_id);
        wire::writer restore{storage};assert(restore.u64(env.request[i].node_capability));
        assert(factory->open_service_request(item.parent,origin,out.written(),receipt)==c::error::ok);
        assert(factory->open_service_request(item.parent,origin,out.written(),retry)==c::error::ok&&retry.receipt.request_id==receipt.receipt.request_id);
        item.request=receipt.receipt.request_id;assert(item.request!=item.resource&&item.resource!=item.node);
    }
    ready={};deadline=c::now()+4000000000;
    while(!(ready[0]&&ready[1]))
    {
        auto progress=factory->pump_services();assert(progress==c::error::ok||progress==c::error::again);
        for(auto& item:state){creation::view result;assert(factory->inspect_service_open(env.who[item.side],item.request,result)==c::error::ok);
            ready[item.side]=result.receipt.status==opens::state::ready;}
        assert(c::now()<deadline);if(!(ready[0]&&ready[1]))::poll(nullptr,0,1);
    }
    for(auto& item:state)
    {
        assert(factory->create_verified_service(item.request,item.policy,item.schema,item.response_schema,std::move(item.parent),item.resource,service_state::admit,service_state::charge,&item,item.endpoint)==c::error::ok);
        assert(item.registry->adopt_service(item.slot,item.endpoint,peers[item.side].definition.network.generation)==c::error::ok);
        s::group_lease held;assert(service_state::admit_open(&item,item.opening,held)==c::error::ok);opens::published published;
        assert(factory->publish_service(env.who[item.side],item.request,held,item.resource,published)==c::error::ok&&published.handle==item.resource);
        assert(factory->publish_service(env.who[item.side],item.request,held,item.resource,published)==c::error::ok);
    }
    auto pump=[&]{auto status=factory->pump_services();assert(status==c::error::ok||status==c::error::again);};
    for(auto& item:state)
    {
        auto token=item.token(c::permission::qos_describe);c::qos actual;std::array<std::uint8_t,32> gid{};std::size_t size{};deadline=c::now()+3000000000;
        auto status=item.registry->describe_qos(item.policy.authority,item.resource,actual,gid,size,&token);
        while(status==c::error::again){pump();assert(c::now()<deadline);status=item.registry->describe_qos(item.policy.authority,item.resource,actual,gid,size,&token);if(status==c::error::again)::poll(nullptr,0,1);}
        // A service has no native publisher GID in this collapsed QoS observation.
        assert(status==c::error::ok&&c::valid(actual)&&size==0);
    }
    auto& client=state[0];auto& server=state[1];bool available{};auto query=client.token(c::permission::service_available);
    auto end=c::now()+8000000000;auto status=c::error::again;
    while(!available)
    {
        status=client.registry->service_available(client.policy.authority,client.resource,available,&query);
        assert(status==c::error::ok||status==c::error::again);pump();assert(c::now()<end);if(!available)::poll(nullptr,0,1);
    }
    auto reconcile=[&](service_state& item)
    {
        c::operation_token completed;c::native_result result;auto until=c::now()+3000000000;
        auto status=item.endpoint->completion(completed,result);
        while(status==c::error::again){pump();assert(c::now()<until);status=item.endpoint->completion(completed,result);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&result.status==c::error::ok&&result.observed==c::effect::locally_accepted);
        assert(item.registry->settle_operation(completed,result)==c::error::ok&&item.endpoint->release_completion(completed)==c::error::ok);
    };
    std::array<std::uint8_t,1> request_body{1};auto submit=client.token(c::permission::service_submit);c::receipt submitted;
    auto until=c::now()+900000000;
    assert(client.registry->service_mutate(client.policy.authority,client.resource,{client.resource,1},0,client.schema.type_identity,client.schema.encoding,
        request_body,until,false,submitted,&submit)==c::error::ok);reconcile(client);
    struct delivered
    {
        c::delivery_reference reference;c::handle correlation{};bytes body;bool fail{true};unsigned failures{};
        static bool sink(void* opaque,c::delivery_reference reference,std::uint64_t,c::service_delivery const& value)
        {
            auto& self=*static_cast<delivered*>(opaque);self.reference=reference;self.correlation=value.correlation;self.body.assign(value.body.begin(),value.body.end());
            if(self.fail){self.fail=false;++self.failures;return false;}return true;
        }
    } incoming,outgoing;
    auto receive_record=[&](service_state& item,delivered& record)
    {
        auto origin=item.token(c::permission::service_reserve);c::delivery_consumer consumer{env.who[item.side].instance,env.who[item.side].generation,env.service_caps[item.side]};
        auto end=c::now()+4000000000;bool received{};
        while(!received)
        {
            auto until=c::now()+900000000;
            auto status=item.registry->reserve_service(item.policy.authority,item.resource,consumer,until,&record,delivered::sink,&origin);
            do
            {
                if(status!=c::error::again)break;
                pump();assert(c::now()<end);
                status=item.registry->reserve_service(item.policy.authority,item.resource,consumer,until,&record,delivered::sink,&origin);
                if(status==c::error::again)::poll(nullptr,0,1);
                // An empty native receive completes this observation. A new
                // observation has a fresh finite deadline and independent debit.
                if(status==c::error::again&&!item.endpoint->occupied())break;
            }while(true);
            if(status==c::error::io)status=item.registry->reserve_service(item.policy.authority,item.resource,consumer,until,&record,delivered::sink,&origin);
            assert(status==c::error::ok||status==c::error::again);received=status==c::error::ok;assert(c::now()<end);
        }
        assert(record.reference.source==item.resource&&record.reference.generation==peers[item.side].definition.network.generation&&record.failures==1);
    };
    auto ack=[&](service_state& item,delivered const& record)
    {
        auto origin=item.token(c::permission::service_acknowledge);c::delivery_consumer consumer{env.who[item.side].instance,env.who[item.side].generation,env.service_caps[item.side]};
        c::delivery_receipt result;auto end=c::now()+3000000000;
        auto status=item.registry->settle_service(item.policy.authority,item.resource,consumer,record.reference,{item.resource,1},true,result,&origin);
        while(status==c::error::again)
        {pump();assert(c::now()<end);status=item.registry->settle_service(item.policy.authority,item.resource,consumer,record.reference,{item.resource,1},true,result,&origin);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&result.status==c::error::ok&&result.observed==c::effect::locally_accepted);
        auto charged=item.charges;
        assert(item.registry->settle_service(item.policy.authority,item.resource,consumer,record.reference,{item.resource,1},true,result,&origin)==c::error::ok&&item.charges==charged);
    };
    receive_record(server,incoming);assert(incoming.body==bytes{1}&&incoming.correlation!=submitted.request);
    std::array<std::uint8_t,128> storage;wire::writer encoded{storage};assert(encoded.encoded(request_body)&&encoded.text("isolated typed service"));
    auto respond=server.token(c::permission::service_respond);c::receipt responded;until=c::now()+900000000;
    auto reply=[&]{return server.registry->service_mutate(server.policy.authority,server.resource,{server.resource,1},incoming.correlation,
        server.response_schema.type_identity,server.response_schema.encoding,encoded.written(),until,true,responded,&respond);};
    assert(reply()==c::error::denied);ack(server,incoming);assert(reply()==c::error::ok);reconcile(server);
    auto charged=server.charges;assert(reply()==c::error::ok&&server.charges==charged);
    receive_record(client,outgoing);assert(outgoing.correlation==submitted.request&&outgoing.body[0]==1);
    wire::reader text{wire::bytes{outgoing.body}.subspan(1)};std::string_view message;assert(text.text(message)&&text.done()&&message=="isolated typed service");
    ack(client,outgoing);assert(!client.endpoint->occupied()&&!server.endpoint->occupied());
    assert(env.permissions.close(env.who[0],env.response_caps[0])==s::error::success);
    assert(client.registry->service_mutate(client.policy.authority,client.resource,{client.resource,2},0,client.schema.type_identity,client.schema.encoding,
        request_body,c::now()+900000000,false,submitted,&submit)==c::error::bad_handle);
    for(auto& item:state)
    {
        assert(item.registry->close(item.policy.authority,item.resource)==c::error::busy);
        auto origin=item.token(c::permission::resource_close);auto end=c::now()+3000000000;
        auto status=item.registry->close_owned(item.policy.authority,item.resource,origin);
        while(status==c::error::again){pump();assert(c::now()<end);status=item.registry->close_owned(item.policy.authority,item.resource,origin);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&item.endpoint->cleanup_confirmed());
    }
// A close receipt does not release buffers held by an external observer.
    assert(env.permissions.open(env.who[0],"response.0",env.response_caps[0])==s::error::success);
    for(auto& item:state)
    {
        auto i=item.side;auto old_resource=item.resource,old_request=item.request;
        assert(factory->release_open(env.who[i],old_request)==c::error::busy);
        assert(factory->reconcile_close(old_request,old_resource+1)==c::error::denied);
        assert(factory->reconcile_close(old_request,old_resource)==c::error::ok);
        assert(factory->reconcile_close(old_request,old_resource)==c::error::ok);
        assert(factory->release_open(env.who[1-i],old_request)!=c::error::ok);
        assert(factory->release_open(env.who[i],old_request)==c::error::ok);
        assert(factory->release_open(env.who[i],old_request)==c::error::ok);
        creation::view released;assert(factory->inspect_service_open(env.who[i],old_request,released)==c::error::ok&&released.receipt.status==opens::state::released&&!released.endpoint_live&&!released.receipt.cleanup_debt);
        assert(item.registry->attach_node(env.input[i].node.authority,item.node,env.input[i].node.selected,item.parent)==c::error::ok);
        item.opening.key=2;item.opening.deadline=c::now()+3500000000;item.opening.origins[4].capability=env.response_caps[i];
        std::array<std::uint8_t,2048> storage;wire::writer out{storage};
        assert(out.u64(env.request[i].node_capability)&&out.u64(item.node)&&out.u64(2)&&out.u64(env.schema_caps[i])&&out.u64(env.response_caps[i])&&contract::encode_profile(out,item.policy.selected)&&
            out.text(item.policy.resolved_name)&&out.text(item.policy.service_type)&&out.text(i?"server":"client")&&out.u64(item.opening.key)&&out.u64(item.opening.deadline));
        creation::view next;
        assert(factory->open_service_request(item.parent,item.opening,out.written(),next)==c::error::again&&!next.receipt.request_id);
        item.endpoint.reset();
        assert(factory->open_service_request(item.parent,item.opening,out.written(),next)==c::error::ok&&next.receipt.request_id!=old_request);
        item.request=next.receipt.request_id;
        assert(item.registry->prepare_adoption(item.policy.authority,item.parent,item.policy.selected,c::operation_family::service,item.slot)==c::error::ok);
        item.resource=item.slot.resource();assert(item.resource!=old_resource);
    }
    ready={};deadline=c::now()+4000000000;
    while(!(ready[0]&&ready[1]))
    {
        pump();
        for(auto& item:state){creation::view current;assert(factory->inspect_service_open(env.who[item.side],item.request,current)==c::error::ok);ready[item.side]=current.receipt.status==opens::state::ready;}
        assert(c::now()<deadline);if(!(ready[0]&&ready[1]))::poll(nullptr,0,1);
    }
    for(auto& item:state)
    {
        auto i=item.side;
        assert(factory->create_verified_service(item.request,item.policy,item.schema,item.response_schema,std::move(item.parent),item.resource,service_state::admit,service_state::charge,&item,item.endpoint)==c::error::ok);
        assert(item.registry->adopt_service(item.slot,item.endpoint,peers[i].definition.network.generation)==c::error::ok);
        {s::group_lease held;assert(service_state::admit_open(&item,item.opening,held)==c::error::ok);opens::published published;
        assert(factory->publish_service(env.who[i],item.request,held,item.resource,published)==c::error::ok);}
        auto origin=item.token(c::permission::resource_close);auto end=c::now()+3000000000;
        auto status=item.registry->close_owned(item.policy.authority,item.resource,origin);
        while(status==c::error::again){pump();assert(c::now()<end);status=item.registry->close_owned(item.policy.authority,item.resource,origin);if(status==c::error::again)::poll(nullptr,0,1);}
        assert(status==c::error::ok&&factory->reconcile_close(item.request,item.resource)==c::error::ok&&factory->release_open(env.who[i],item.request)==c::error::ok);
        item.endpoint.reset();
        // Cancellation before any native bytes releases preparation without
        // sending cleanup or changing another binding's native worker.
        assert(item.registry->attach_node(env.input[i].node.authority,item.node,env.input[i].node.selected,item.parent)==c::error::ok);
        for(std::uint64_t key:{3u,4u})
        {
            item.opening.key=key;item.opening.deadline=c::now()+3500000000;
            std::array<std::uint8_t,2048> storage;wire::writer out{storage};
            assert(out.u64(env.request[i].node_capability)&&out.u64(item.node)&&out.u64(2)&&out.u64(env.schema_caps[i])&&out.u64(env.response_caps[i])&&contract::encode_profile(out,item.policy.selected)&&
                out.text(item.policy.resolved_name)&&out.text(item.policy.service_type)&&out.text(i?"server":"client")&&out.u64(key)&&out.u64(item.opening.deadline));
            creation::view next,cancelled;
            assert(factory->open_service_request(item.parent,item.opening,out.written(),next)==c::error::ok);
            assert(factory->cancel_open(env.who[1-i],next.receipt.request_id,cancelled)!=c::error::ok);
            assert(factory->cancel_open(env.who[i],next.receipt.request_id,cancelled)==c::error::ok&&cancelled.receipt.status==opens::state::cancelled&&!cancelled.receipt.cleanup_debt);
            assert(factory->release_open(env.who[i],next.receipt.request_id)==c::error::ok);
        }
    }
    factory->retire();assert(!factory->cleanup_confirmed());
    std::puts("PASS actual sealed typed services: factory-owned creation, separate schemas and host/native correlations, actual SetBool roundtrip, ACK before response, failed-copy recovery, exact receipts and revoked-schema fencing");
}
