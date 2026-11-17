/** @file @brief Real registry, isolated topic facade and nonblocking pipe composition. */
#define main communication_core_fixture_entry
#include "communication_core.cpp"
#undef main
#define main communication_async_fixture_entry
#include "communication_worker_async.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_worker_topic.h>
namespace owned=s::communication_worker;
namespace jobs=s::communication_jobs;
namespace contract=uwvm2::imported::ros::communication_contract;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;

struct topic_environment
{
    fixture pipes{128};
    s::authority permissions;int instance{};s::caller who{&instance,1};std::array<s::handle,4> caps{};
    std::shared_ptr<test_backend> backend{uwvm2::utils::container::make_shared<test_backend>()};
    c::registry registry{backend,{1,1,1,1,128,64}};
    std::shared_ptr<owned::topic_endpoint> endpoint{uwvm2::utils::container::make_shared<owned::topic_endpoint>()};
    c::owner owner{17,1,1},node_owner{17,2,1},context_owner{17,3,1},schema_owner{17,4,1};
    c::profile context{profile("ctx",1)},node{profile("node",2)},type{profile("type",3)},topic{profile("topic",4)};
    c::qos quality{1,1,2,1,2,{},{},{}};
    c::handle context_id{},node_id{},topic_id{};
    c::endpoint_slot slot;
    std::shared_ptr<int> origin{uwvm2::utils::container::make_shared<int>(1)};
    c::operation_token last;unsigned charges{},validations{};
    explicit topic_environment(bool subscription=false,std::optional<std::uint64_t> quota={},bool prepare_only=false)
    {
        constexpr std::array names{"topic.publish","qos.describe","qos.liveliness.assert","qos.acknowledgements.wait",
            "topic.reserve","topic.acknowledge","topic.release","resource.close"};
        for(auto name:names)assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        for(auto name:{"capability.open","capability.close","node.open","context.open","type.describe"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        constexpr std::array aliases{"topic","node","context","schema"};
        constexpr std::array kinds{s::purpose::topic,s::purpose::node,s::purpose::context,s::purpose::type};
        for(std::size_t i{};i<4;++i)
        {
            s::grant grant;grant.alias=aliases[i];grant.resource="fixture";grant.binding="private-worker";grant.kind=kinds[i];
            if(!i){for(auto name:names)grant.actions.emplace(name);grant.limits.calls=quota;grant.limits.uncertain=quota.has_value();grant.limits.window=std::chrono::hours{1};}
            else grant.actions.emplace(i==1?"node.open":i==2?"context.open":"type.describe");
            assert(permissions.configure(std::move(grant))==s::error::success);
        }
        assert(permissions.enroll(who)==s::error::success);
        for(std::size_t i{};i<4;++i)assert(permissions.open(who,aliases[i],caps[i])==s::error::success);
        assert(registry.configure(c::context_plan{context_owner,context,4,{},c::transport::unauthenticated_loopback})==c::error::ok);
        assert(registry.configure(c::node_plan{node_owner,node,context,"worker","/scene",1})==c::error::ok);
        c::type_plan schema{type,"test/Bytes","canonical",{3},&bytes_valid,schema_owner};
        assert(registry.configure(schema)==c::error::ok);
        c::topic_plan selected{owner,topic,node,type,"/scene/data",subscription?c::role::subscription:c::role::publisher,quality,64,
            subscription?2u:0u,subscription?128u:0u,c::overflow::reject,1000000000};
        assert(registry.configure(selected)==c::error::ok);
        assert(registry.open_context(context_owner,context,context_id)==c::error::ok);
        assert(registry.open_node(node_owner,context_owner,context_id,node,node_id)==c::error::ok);
        c::node_lease lease;assert(registry.attach_node(node_owner,node_id,node,lease)==c::error::ok);
        assert(registry.prepare_adoption(owner,lease,topic,c::operation_family::topic,slot)==c::error::ok);topic_id=slot.resource();
        if(prepare_only)return;
        auto reserved=(subscription?3776u:2816u)+owned::endpoint_closure::reserved_bytes;
        jobs::plan plan{owner,c::operation_family::topic,topic_id,1,71,1,128,512,1000000000,reserved};
        assert(endpoint->configure(selected,schema,std::move(lease),plan,9001,{128,512,reserved},admit,charge,this)==c::error::ok);
        assert(registry.adopt_topic(slot,endpoint,71)==c::error::ok);
    }
    c::operation_token token(c::permission action)const
    {
        c::operation_token value;value.caller_instance=who.instance;value.caller_generation=who.generation;value.authority=owner;value.resource=topic_id;
        value.capability=caps[0];value.family=c::operation_family::topic;value.action=action;value.count=4;value.retained_origins=origin;
        value.origins[0]={owner,caps[0],topic_id,action};value.origins[1]={node_owner,caps[1],node_id,c::permission::node_open};
        value.origins[2]={context_owner,caps[2],context_id,c::permission::context_open};value.origins[3]={schema_owner,caps[3],0,c::permission::type_describe};return value;
    }
    c::error authorize(c::operation_token const& actual,s::group_lease& held,bool charged,std::uint64_t extent=0)noexcept
    {
        auto expected=token(actual.action);
        if(!actual.valid(actual.action,c::operation_family::topic,owner,topic_id,false)||actual.caller_instance!=who.instance||
            actual.caller_generation!=who.generation||actual.origins!=expected.origins||actual.retained_origins!=origin)return c::error::denied;
        jobs::claim_set claims;auto status=permissions.admit_group(who,jobs::admission_claims(actual,claims,extent),held,charged);
        if(status==s::error::success){if(charged)++charges;else ++validations;last=actual;}
        return static_cast<c::error>(status);
    }
    static c::error admit(void* context,c::operation_token const& token,s::group_lease& held)noexcept
    {return static_cast<topic_environment*>(context)->authorize(token,held,false);}
    static c::error charge(void* context,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
    {return static_cast<topic_environment*>(context)->authorize(token,held,true,bytes);}
    void reply(std::uint64_t sequence,c::native_result result,s::wire::bytes body)
    {
        std::array<std::uint8_t,1024> storage;s::wire::writer out{storage};
        assert(out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(sequence)&&out.u64(static_cast<std::uint64_t>(result.status))&&
            out.u64(static_cast<std::uint64_t>(result.observed))&&out.u64(body.size())&&out.encoded(body));
        assert(::write(pipes.output[1],storage.data(),out.size())==static_cast<ssize_t>(out.size()));
    }
    void complete_mutation(std::uint64_t sequence,c::error status,c::effect observed,unsigned malformed=0)
    {
        std::array<std::uint8_t,256> body{};s::wire::writer out{body};
        assert(contract::encode_receipt(out,{9002,malformed==3?72u:71u,{},status,observed}));
        reply(sequence,{status,observed},malformed==2?s::wire::bytes{body.data(),200}:malformed==1?out.written().first(1):out.written());
        assert(endpoint->pump(pipes.transport)==c::error::ok);
    }
};

static void publications_and_observations()
{
    topic_environment f;auto origin=f.token(c::permission::topic_publish);std::array<std::uint8_t,2> data{1,2};
    assert(f.endpoint->publish(data,c::now()+800000000).status==c::error::denied);
    auto deadline=c::now()+800000000;c::receipt receipt;
    assert(f.registry.publish(f.owner,f.topic_id,{f.topic_id,1},"test/Bytes","canonical",data,deadline,receipt,&origin)==c::error::ok);
    assert(receipt.observed==c::effect::locally_accepted&&receipt.provider_generation==71&&f.charges==1&&f.endpoint->occupied());
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.pipes.drain()>0);
    f.complete_mutation(1,c::error::ok,c::effect::locally_accepted);
    c::operation_token completed;c::native_result result;
    assert(f.endpoint->completion(completed,result)==c::error::ok&&result.request_identity==receipt.request);
    assert(f.registry.settle_operation(completed,result)==c::error::ok&&f.endpoint->release_completion(completed)==c::error::ok&&!f.endpoint->occupied());
    assert(f.registry.publish(f.owner,f.topic_id,{f.topic_id,1},"test/Bytes","canonical",data,deadline,receipt,&origin)==c::error::ok&&f.charges==1);
    auto query=f.token(c::permission::qos_describe);c::qos observed;std::array<std::uint8_t,32> gid{};std::size_t size{};
    assert(f.endpoint->inspect(observed,gid,size,query)==c::error::again&&f.charges==2);
    assert(f.endpoint->inspect(observed,gid,size,query)==c::error::again&&f.charges==2);
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.pipes.drain()>0);
    std::array<std::uint8_t,128> body;s::wire::writer out{body};std::array<std::uint8_t,4> identity{1,2,3,4};
    assert(contract::qos_body(out,f.quality)&&out.blob(identity));f.reply(2,{c::error::ok,c::effect::none},out.written());
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok);
    assert(f.endpoint->inspect(observed,gid,size,query)==c::error::ok&&size==4&&gid[3]==4&&f.charges==2&&!f.endpoint->occupied());
    auto changed=query;++changed.origins[1].resource;
    assert(f.endpoint->inspect(observed,gid,size,changed)==c::error::denied&&f.charges==2);
    auto live=f.token(c::permission::qos_liveliness_assert);
    assert(f.registry.assert_liveliness(f.owner,f.topic_id,{f.topic_id,2},receipt,&live)==c::error::ok&&f.charges==3);
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.pipes.drain()>0);
    f.complete_mutation(3,c::error::denied,c::effect::none);
    assert(f.endpoint->completion(completed,result)==c::error::ok&&result.status==c::error::denied);
    assert(f.registry.settle_operation(completed,result)==c::error::ok&&f.endpoint->release_completion(completed)==c::error::ok);
}

static void acknowledgement_waits()
{
    for(unsigned malformed{};malformed!=4;++malformed)
    {
        topic_environment f(false,1);auto origin=f.token(c::permission::qos_acknowledgements_wait);c::receipt receipt;
        auto deadline=c::now()+800000000;
        assert(f.registry.wait_acknowledged(f.owner,f.topic_id,deadline,receipt,&origin)==c::error::again&&f.charges==1);
        auto request=f.last.request;
        assert(f.registry.wait_acknowledged(f.owner,f.topic_id,deadline,receipt,&origin)==c::error::again&&f.charges==1);
        assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.pipes.drain()>0);
        f.complete_mutation(1,c::error::ok,c::effect::observed_remote,malformed);
        assert(f.registry.wait_acknowledged(f.owner,f.topic_id,deadline,receipt,&origin)==c::error::ok&&receipt.request==request);
        assert(receipt.observed==(malformed?c::effect::unknown:c::effect::observed_remote)&&f.charges==1);
        assert(f.registry.wait_acknowledged(f.owner,f.topic_id,deadline,receipt,&origin)==c::error::ok&&receipt.request==request&&f.charges==1);
        assert(f.endpoint->occupied()==(malformed!=0));
    }
    topic_environment f(false,1);auto origin=f.token(c::permission::qos_acknowledgements_wait);c::receipt receipt;
    auto deadline=c::now()+50000000;
    assert(f.registry.wait_acknowledged(f.owner,f.topic_id,deadline,receipt,&origin)==c::error::again);
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.pipes.drain()>0);
    f.complete_mutation(1,c::error::timeout,c::effect::none);
    while(c::now()<deadline)::poll(nullptr,0,1);
    assert(f.registry.wait_acknowledged(f.owner,f.topic_id,deadline,receipt,&origin)==c::error::ok&&receipt.status==c::error::timeout&&
        receipt.observed==c::effect::none&&f.charges==1&&!f.endpoint->occupied());
}

static void fencing_and_capacity()
{
    for(unsigned fault{};fault<5;++fault)
    {
        topic_environment f(false,fault==0?std::optional<std::uint64_t>{0}:std::nullopt);
        auto origin=f.token(c::permission::topic_publish);c::receipt receipt;std::array<std::uint8_t,1> body{1};
        auto status=f.registry.publish(f.owner,f.topic_id,{f.topic_id,1},"test/Bytes","canonical",body,c::now()+800000000,receipt,&origin);
        if(!fault){assert(status==c::error::again&&!f.charges&&!f.pipes.drain());continue;}
        assert(status==c::error::ok&&f.charges==1);
        if(fault==1)assert(f.permissions.close(f.who,f.caps[1])==s::error::success);
        assert(f.endpoint->pump(f.pipes.transport)==c::error::ok);
        if(fault!=1){assert(f.pipes.drain()>0);f.complete_mutation(1,c::error::ok,c::effect::locally_accepted,fault-1);}
        else assert(!f.pipes.drain());
        c::operation_token completed;c::native_result result;assert(f.endpoint->completion(completed,result)==c::error::ok);
        assert(f.registry.settle_operation(completed,result)==c::error::ok);
        if(fault==1)assert(result.status==c::error::bad_handle&&result.observed==c::effect::none&&f.endpoint->release_completion(completed)==c::error::ok);
        else assert(result.status==c::error::io&&result.observed==c::effect::unknown&&f.endpoint->release_completion(completed)==c::error::busy&&f.endpoint->occupied());
    }
    topic_environment f;auto child=::fork();assert(child>=0);
    if(!child){assert(f.endpoint->pump(f.pipes.transport)==c::error::denied&&f.endpoint->occupied());::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
static void exact_native_closure()
{
    topic_environment f(false,0);auto token=f.token(c::permission::resource_close);
    assert(f.registry.close(f.owner,f.topic_id)==c::error::busy);
    assert(f.permissions.close(f.who,f.caps[3])==s::error::success);
    assert(f.registry.close_owned(f.owner,f.topic_id,token)==c::error::again&&f.charges==1);
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.pipes.drain()>0);
    std::array<std::uint8_t,8> body;s::wire::writer out{body};assert(out.u64(9001));f.reply(1,{c::error::ok,c::effect::none},out.written());
    assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&f.endpoint->cleanup_confirmed());
    assert(f.registry.close_owned(f.owner,f.topic_id,token)==c::error::ok&&f.charges==1);
    auto mutation=f.token(c::permission::topic_publish).with_request(uwvm2::imported::ros::resources::allocate_identity(),f.topic_id,1);
    assert(f.endpoint->publish({},c::now()+100000000,mutation).status==c::error::cancelled);
}

#if defined(UWVM_COMMUNICATION_TOPIC_FIXTURE_ONLY)
int communication_topic_fixture_entry()
#else
int main()
#endif
{
    publications_and_observations();acknowledgement_waits();fencing_and_capacity();exact_native_closure();
    std::puts("PASS isolated typed topic endpoint: actual registry adoption, per-call authority, queued publication and QoS, bounded acknowledgement recovery, revocation and malformed-result retention");
    return 0;
}
