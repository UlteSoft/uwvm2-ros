/** @file @brief Actual registry adoption and bounded service IPC, using deterministic private pipes. */
#define main communication_core_fixture_entry
#include "communication_core.cpp"
#undef main
#define main communication_async_fixture_entry
#include "communication_worker_async.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_worker_service.h>
namespace owned=s::communication_worker;
namespace jobs=s::communication_jobs;
namespace contract=uwvm2::imported::ros::communication_contract;
namespace receive=uwvm2::imported::ros::communication_receive_contract;
using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
struct service_environment
{
    fixture pipes{128};s::authority permissions;int instance{};s::caller who{&instance,1};std::array<s::handle,5> caps{};
    std::shared_ptr<test_backend> backend{uwvm2::utils::container::make_shared<test_backend>()};
    c::registry registry{backend,{1,1,0,2,128,8,1}};
    std::shared_ptr<owned::service_endpoint> endpoint{uwvm2::utils::container::make_shared<owned::service_endpoint>()};
    c::owner owner{17,1,1},node_owner{17,2,1},context_owner{17,3,1},request_owner{17,4,1},response_owner{17,5,1};
    c::profile context{profile("context",1)},node{profile("node",2)},request{profile("request",3)},response{profile("response",4)},service{profile("service",5)};
    c::qos quality{1,1,2,1,2,{},{},{}};c::handle context_id{},node_id{},service_id{};c::endpoint_slot slot;
    std::shared_ptr<int> origin{uwvm2::utils::container::make_shared<int>(1)};
    c::type_plan request_schema{request,"test/Request","canonical",{1},&bytes_valid,request_owner},response_schema{response,"test/Response","canonical",{2},&bytes_valid,response_owner};
    c::service_plan policy;unsigned charges{};std::uint64_t serial{};bool server{};
    std::atomic<bool> pause_admission{},admission_entered{},resume_admission{};
    explicit service_environment(bool serving=false,std::optional<std::uint64_t> quota={},bool prepare_only=false):server(serving)
    {
        constexpr std::array actions{"service.submit","service.respond","service.available","service.abandon","qos.describe",
            "service.reserve","service.acknowledge","service.release","resource.close"};
        for(auto name:actions)assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        for(auto name:{"capability.open","capability.close","node.open","context.open","type.describe"})assert(permissions.expose(name,s::exposure::allow)==s::error::success);
        constexpr std::array aliases{"service","node","context","request","response"};
        constexpr std::array kinds{s::purpose::service,s::purpose::node,s::purpose::context,s::purpose::type,s::purpose::type};
        for(std::size_t i{};i<caps.size();++i)
        {
            s::grant grant;grant.alias=aliases[i];grant.kind=kinds[i];grant.resource="fixture";grant.binding="private-worker";
            if(!i){for(auto name:actions)grant.actions.emplace(name);grant.limits.calls=quota;grant.limits.uncertain=quota.has_value();grant.limits.window=std::chrono::hours{1};}
            else grant.actions.emplace(i==1?"node.open":i==2?"context.open":"type.describe");
            assert(permissions.configure(std::move(grant))==s::error::success);
        }
        assert(permissions.enroll(who)==s::error::success);
        for(std::size_t i{};i<caps.size();++i)assert(permissions.open(who,aliases[i],caps[i])==s::error::success);
        assert(registry.configure(c::context_plan{context_owner,context,4,{},c::transport::unauthenticated_loopback})==c::error::ok);
        assert(registry.configure(c::node_plan{node_owner,node,context,"service","/scene",1})==c::error::ok);
        assert(registry.configure(request_schema)==c::error::ok&&registry.configure(response_schema)==c::error::ok);
        policy={owner,service,node,request,response,"/scene/service","test/Service",server?c::service_role::server:c::service_role::client,quality,2,8,8,16,1000000000};
        assert(registry.configure(policy)==c::error::ok);
        assert(registry.open_context(context_owner,context,context_id)==c::error::ok&&registry.open_node(node_owner,context_owner,context_id,node,node_id)==c::error::ok);
        c::node_lease lease;assert(registry.attach_node(node_owner,node_id,node,lease)==c::error::ok);
        assert(registry.prepare_adoption(owner,lease,service,c::operation_family::service,slot)==c::error::ok);service_id=slot.resource();
        if(prepare_only)return;
        assert(configure(std::move(lease))==c::error::ok&&registry.adopt_service(slot,endpoint,71)==c::error::ok);
    }
    c::error configure(c::node_lease lease,s::communication_workspace::lease* reservation=nullptr)
    {
        auto reserved=owned::service_endpoint::reservation_bytes(policy,128,512);
        jobs::plan plan{owner,c::operation_family::service,service_id,1,71,1,128,512,1000000000,reserved};
        return endpoint->configure(policy,request_schema,response_schema,std::move(lease),plan,9001,{128,512,reserved},admit,charge,this,reservation);
    }
    c::operation_token token(c::permission action)const
    {
        c::operation_token value;value.caller_instance=who.instance;value.caller_generation=who.generation;value.authority=owner;value.resource=service_id;
        value.capability=caps[0];value.family=c::operation_family::service;value.action=action;value.count=5;value.retained_origins=origin;
        value.origins[0]={owner,caps[0],service_id,action};value.origins[1]={node_owner,caps[1],node_id,c::permission::node_open};
        value.origins[2]={context_owner,caps[2],context_id,c::permission::context_open};value.origins[3]={request_owner,caps[3],0,c::permission::type_describe};
        value.origins[4]={response_owner,caps[4],0,c::permission::type_describe};return value;
    }
    c::delivery_consumer consumer()const{return {who.instance,who.generation,caps[0]};}
    c::error authorize(c::operation_token const& actual,s::group_lease& held,bool charged,std::uint64_t extent=0)noexcept
    {
        auto expected=token(actual.action);
        if(!actual.valid(actual.action,c::operation_family::service,owner,service_id,false)||actual.caller_instance!=who.instance||
            actual.caller_generation!=who.generation||actual.origins!=expected.origins||actual.retained_origins!=origin)return c::error::denied;
        jobs::claim_set claims;auto status=permissions.admit_group(who,jobs::admission_claims(actual,claims,extent),held,charged);
        if(status==s::error::success&&charged)++charges;
        return static_cast<c::error>(status);
    }
    static c::error admit(void* context,c::operation_token const& token,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<service_environment*>(context);
        if(self.pause_admission.load()){self.admission_entered=true;while(!self.resume_admission.load())std::this_thread::yield();}
        return self.authorize(token,held,false);
    }
    static c::error charge(void* context,c::operation_token const& token,std::uint64_t size,s::group_lease& held)noexcept
    {return static_cast<service_environment*>(context)->authorize(token,held,true,size);}
    bytes send(rpc::operation expected)
    {
        assert(endpoint->pump(pipes.transport)==c::error::ok);std::array<std::uint8_t,1024> data;
        auto count=::read(pipes.input[0],data.data(),data.size());assert(count>0);rpc::request record;
        assert(rpc::decode_request({data.data(),static_cast<std::size_t>(count)},71,128,record)&&record.action==expected);
        ++serial;return {record.body.begin(),record.body.end()};
    }
    void reply(c::error status,c::effect observed,s::wire::bytes body)
    {
        std::array<std::uint8_t,512> data;s::wire::writer out{data};assert(out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(serial)&&
            out.u64(static_cast<std::uint64_t>(status))&&out.u64(static_cast<std::uint64_t>(observed))&&out.u64(body.size())&&out.encoded(body));
        assert(::write(pipes.output[1],data.data(),out.size())==static_cast<ssize_t>(out.size()));assert(endpoint->pump(pipes.transport)==c::error::ok);
    }
    void mutation_reply(c::error status=c::error::ok,c::effect observed=c::effect::locally_accepted,c::handle native=9002,unsigned malformed=0)
    {
        std::array<std::uint8_t,256> bytes{};s::wire::writer out{bytes};assert(contract::encode_receipt(out,{native,malformed==3?72u:71u,{},status,observed}));
        reply(status,observed,malformed==2?s::wire::bytes{bytes.data(),200}:malformed==1?out.written().first(1):out.written());
    }
    c::receipt submit(std::uint64_t sequence=1,std::uint64_t until=0)
    {
        std::array<std::uint8_t,1> body{7};auto origin=token(c::permission::service_submit);c::receipt result;
        assert(registry.service_mutate(owner,service_id,{service_id,sequence},0,"test/Request","canonical",body,until?until:c::now()+800000000,false,result,&origin)==c::error::ok);return result;
    }
    c::native_result complete(bool release=true)
    {
        c::operation_token origin;c::native_result result;assert(endpoint->completion(origin,result)==c::error::ok);
        assert(registry.settle_operation(origin,result)==c::error::ok);
        if(release)assert(endpoint->release_completion(origin)==c::error::ok);
        return result;
    }
    c::handle receive_request(c::delivery_reference& reference,bool fail_first=false)
    {
        auto until=c::now()+800000000;auto origin=token(c::permission::service_reserve);c::handle correlation{};
        struct copied{c::handle* id;c::delivery_reference* ref;bool accept{};} target{&correlation,&reference,!fail_first};
        auto sink=[](void* into,c::delivery_reference ref,std::uint64_t,c::service_delivery const& value)
        {auto& target=*static_cast<copied*>(into);*target.id=value.correlation;*target.ref=ref;return target.accept;};
        assert(registry.reserve_service(owner,service_id,consumer(),until,&target,sink,&origin)==c::error::again);
        auto body=send(rpc::operation::service_reserve);s::wire::reader input{body};std::uint64_t native_consumer{},actual_until{};
        assert(input.u64(native_consumer)&&native_consumer&&input.u64(actual_until)&&actual_until==until&&input.done());
        std::array<std::uint8_t,256> storage;s::wire::writer out{storage};std::uint8_t byte=7;c::metadata info;info.source=9001;info.generation=71;info.sequence=3;
        assert(receive::encode(out,{9001,71,3,11},until,info,9002,server?"service.request":"service.response",
            {server?"test/Request":"test/Response","canonical",{&byte,1}}));reply(c::error::ok,c::effect::none,out.written());
        assert(registry.reserve_service(owner,service_id,consumer(),until,&target,sink,&origin)==(fail_first?c::error::io:c::error::ok));
        if(fail_first){auto previous=correlation;target.accept=true;assert(registry.reserve_service(owner,service_id,consumer(),until,&target,sink,&origin)==c::error::ok&&correlation==previous);}
        assert(reference.source==service_id&&reference.source!=9001);return correlation;
    }
    void acknowledge(c::delivery_reference ref)
    {
        auto origin=token(c::permission::service_acknowledge);c::delivery_receipt result;
        assert(registry.settle_service(owner,service_id,consumer(),ref,{service_id,1},true,result,&origin)==c::error::again);
        send(rpc::operation::service_acknowledge);mutation_reply();
        assert(registry.settle_service(owner,service_id,consumer(),ref,{service_id,1},true,result,&origin)==c::error::ok);
        auto calls=charges;assert(registry.settle_service(owner,service_id,consumer(),ref,{service_id,1},true,result,&origin)==c::error::ok&&charges==calls);
    }
};
static void service_client_flow()
{
    service_environment f;auto request=f.submit();assert(f.charges==1&&request.request!=9002);
    auto sent=f.send(rpc::operation::service_submit);s::wire::reader input{sent};std::uint64_t native{},deadline{};s::wire::bytes body;
    assert(input.u64(native)&&!native&&input.blob(body)&&body.size()==1&&input.u64(deadline)&&input.done());
    f.mutation_reply();assert(f.complete().observed==c::effect::locally_accepted&&f.endpoint->occupied());
    c::delivery_reference ref;assert(f.receive_request(ref,true)==request.request);f.acknowledge(ref);
    assert(!f.endpoint->occupied());
    bool ready{};auto query=f.token(c::permission::service_available);
    assert(f.registry.service_available(f.owner,f.service_id,ready,&query)==c::error::again);auto calls=f.charges;
    assert(f.registry.service_available(f.owner,f.service_id,ready,&query)==c::error::again&&f.charges==calls);
    f.send(rpc::operation::service_available);std::array<std::uint8_t,8> reply{1};f.reply(c::error::ok,c::effect::none,reply);
    assert(f.registry.service_available(f.owner,f.service_id,ready,&query)==c::error::ok&&ready&&f.charges==calls);
}
static void service_server_flow()
{
    service_environment f{true};c::delivery_reference ref;auto correlation=f.receive_request(ref,true);assert(correlation&&correlation!=9002);
    std::array<std::uint8_t,1> body{7};auto origin=f.token(c::permission::service_respond);c::receipt result;auto until=c::now()+800000000;
    auto respond=[&]{return f.registry.service_mutate(f.owner,f.service_id,{f.service_id,1},correlation,"test/Response","canonical",body,until,true,result,&origin);};
    assert(respond()==c::error::denied);f.acknowledge(ref);assert(respond()==c::error::ok);auto calls=f.charges;
    assert(respond()==c::error::ok&&f.charges==calls);auto encoded=f.send(rpc::operation::service_respond);s::wire::reader input{encoded};std::uint64_t native{};
    assert(input.u64(native)&&native==9002&&native!=correlation);f.mutation_reply();f.complete();assert(!f.endpoint->occupied());
    assert(respond()==c::error::ok&&f.charges==calls);
}
static void service_cancel_and_failures()
{
    for(unsigned fault{};fault!=5;++fault)
    {
        service_environment f;auto request=f.submit();f.send(rpc::operation::service_submit);f.mutation_reply();f.complete();
        auto origin=f.token(c::permission::service_abandon);c::receipt result;
        assert(f.registry.service_abandon(f.owner,f.service_id,request.request,result,&origin)==c::error::again);
        auto calls=f.charges;assert(f.registry.service_abandon(f.owner,f.service_id,request.request,result,&origin)==c::error::again&&f.charges==calls);
        auto encoded=f.send(rpc::operation::service_abandon);s::wire::reader input{encoded};std::uint64_t native{};assert(input.u64(native)&&native==9002&&input.done());
        f.mutation_reply(fault==1?c::error::timeout:c::error::ok,fault==1?c::effect::none:c::effect::locally_accepted,9002,fault>=2?fault-1:0);
        auto status=f.registry.service_abandon(f.owner,f.service_id,request.request,result,&origin);
        assert(status==(fault==1?c::error::timeout:c::error::ok));
        if(fault>=2)assert(result.status==c::error::io&&result.observed==c::effect::unknown&&result.request&&result.provider_generation==71);
        assert(f.endpoint->occupied()==(fault!=0)&&f.charges==calls);
        if(!fault)assert(f.registry.service_abandon(f.owner,f.service_id,request.request,result,&origin)==c::error::ok&&f.charges==calls);
    }
    for(unsigned fault{};fault!=4;++fault)
    {
        service_environment f(false,fault==0?std::optional<std::uint64_t>{0}:std::nullopt);
        auto origin=f.token(c::permission::service_submit);std::array<std::uint8_t,1> body{7};c::receipt receipt;
        auto status=f.registry.service_mutate(f.owner,f.service_id,{f.service_id,1},0,"test/Request","canonical",body,c::now()+800000000,false,receipt,&origin);
        if(!fault){assert(status==c::error::again&&!f.charges&&!f.endpoint->occupied()&&!f.pipes.drain());continue;}
        assert(status==c::error::ok);
        if(fault==1){assert(f.permissions.close(f.who,f.caps[4])==s::error::success);assert(f.endpoint->pump(f.pipes.transport)==c::error::ok&&!f.pipes.drain());}
        else{f.send(rpc::operation::service_submit);f.mutation_reply(c::error::ok,c::effect::locally_accepted,9002,fault-1);}
        auto result=f.complete(fault==1);
        assert(fault==1?result.status==c::error::bad_handle&&result.observed==c::effect::none:result.status==c::error::io&&result.observed==c::effect::unknown);
        assert(f.endpoint->occupied()==(fault!=1));
    }
}
static void exact_native_closure()
{
    service_environment f(false,0);auto token=f.token(c::permission::resource_close);
    assert(f.registry.close(f.owner,f.service_id)==c::error::busy);
    assert(f.permissions.close(f.who,f.caps[4])==s::error::success);
    assert(f.registry.close_owned(f.owner,f.service_id,token)==c::error::again&&f.charges==1);
    f.send(rpc::operation::resource_close);std::array<std::uint8_t,8> body;s::wire::writer out{body};assert(out.u64(9001));
    f.reply(c::error::ok,c::effect::none,out.written());
    assert(f.registry.close_owned(f.owner,f.service_id,token)==c::error::ok&&f.charges==1&&f.endpoint->cleanup_confirmed());
}
#if defined(UWVM_COMMUNICATION_SERVICE_FIXTURE_ONLY)
int communication_service_fixture_entry()
#else
int main()
#endif
{
    service_client_flow();service_server_flow();service_cancel_and_failures();exact_native_closure();
    std::puts("PASS isolated typed service endpoint: actual registry adoption, complete per-call authority, separate native correlations, ACK before response, detached copy recovery, independent local abandonment, bounded queues and unknown retention");
    return 0;
}
