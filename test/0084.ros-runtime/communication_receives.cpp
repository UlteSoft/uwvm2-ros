/** @file @brief Actual dispatcher/pipe receive recovery, cleanup authority and native evidence. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
#include <uwvm2/imported/ros/scene/communication_receives.h>
namespace worker=s::communication_worker;
namespace rpc=s::communication_worker_protocol;
namespace receive=uwvm2::imported::ros::communication_receive_contract;
struct receive_pipes
{
    std::array<int,2> incoming{},outgoing{},diagnostics{};int identity{};std::uint64_t sequence{};
    s::communication_worker_async::transport transport;
    receive_pipes()
    {
        assert(!::pipe2(incoming.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(outgoing.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({incoming[1],outgoing[0],diagnostics[0],identity},{128,512,512,32,10000000000,1000000},71)==c::error::ok);
    }
    ~receive_pipes(){transport.retire();for(auto fd:{incoming[0],incoming[1],outgoing[0],outgoing[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    bytes sent(rpc::operation action)
    {
        std::array<std::uint8_t,512> storage;auto count=::read(incoming[0],storage.data(),storage.size());assert(count>0);
        rpc::request parsed;assert(rpc::decode_request({storage.data(),static_cast<std::size_t>(count)},71,128,parsed)&&parsed.action==action&&parsed.binding==1);
        sequence=parsed.sequence;return {parsed.body.begin(),parsed.body.end()};
    }
    bool empty(){std::uint8_t byte;assert(::read(incoming[0],&byte,1)==-1&&errno==EAGAIN);return true;}
    void reply(c::error status,c::effect effect,w::bytes body)
    {
        auto frame=encode([&](auto& out){return out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(sequence)&&out.u64(static_cast<std::uint64_t>(status))&&
            out.u64(static_cast<std::uint64_t>(effect))&&out.u64(body.size())&&out.encoded(body);});
        assert(::write(outgoing[1],frame.data(),frame.size())==static_cast<ssize_t>(frame.size()));
    }
};
struct receive_fixture:fixture
{
    receive_pipes pipes;worker::receives receiver;worker::receive_direction direction;
    c::operation_token origin;receive::reference reference;std::uint64_t deadline=c::now()+5000000000ULL,consumer{},native_sequence{};
    std::uint64_t deliveries{};bool failed_decode{},reentrant{};
    explicit receive_fixture(worker::receive_direction chosen=worker::receive_direction::topic,std::optional<std::uint64_t> calls={}):
        fixture(true,true,{},false,false,false,false,chosen==worker::receive_direction::service_request?c::service_role::server:c::service_role::client,calls),direction(chosen)
    {
        if(is_topic())sub_id=opened(contract::operation::topic_open,sub_cap,endpoint("sub","subscription"));
        else service_id=opened(contract::operation::service_open,service_cap,endpoint("srv",direction==worker::receive_direction::service_request?"server":"client"));
        origin=token(is_topic()?c::permission::topic_reserve:c::permission::service_reserve);
        s::communication_jobs::plan plan{origin.authority,origin.family,origin.resource,1,71,1,128,512,10000000000,1664};
        worker::receive_schema schema{10001,71,64,direction,direction==worker::receive_direction::service_response?"test/Response":"test/Request","scene-canonical",valid_body};
        auto validate=[](void* object,c::operation_token const& value,s::group_lease& held)noexcept
        {return static_cast<c::error>(static_cast<receive_fixture*>(object)->dispatch.revalidate(value,held));};
        auto charge=[](void* object,c::operation_token const& value,std::uint64_t bytes,s::group_lease& held)noexcept
        {
            auto& self=*static_cast<receive_fixture*>(object);
            if(self.reentrant)assert(self.reserve()==c::error::again);
            return static_cast<c::error>(self.dispatch.charge_operation(value,bytes,held));
        };
        assert(receiver.configure(plan,schema,validate,charge,this)==c::error::ok);
    }
    bool is_topic()const{return direction==worker::receive_direction::topic;}
    c::operation_token token(c::permission action)
    {
        c::operation_token value;assert(dispatch.retain_receive(who,is_topic()?sub_cap:service_cap,is_topic()?sub_id:service_id,action,value)==error::success);return value;
    }
    c::operation_token altered(c::permission action)const{auto value=origin;value.action=action;value.origins[0].action=action;return value;}
    auto charged(){return admissions(is_topic()?"subscriber":"service");}
    c::error reserve(c::operation_token const* replacement=nullptr)
    {
        return receiver.reserve(replacement?*replacement:origin,deadline,[&](receive::reference host,worker::received_record const& record)
        {
            ++deliveries;if(failed_decode)throw std::bad_alloc{};
            assert(host.endpoint==origin.resource&&host.provider_generation==71&&host.sequence==31&&host.nonce==11);
            assert(record.native_reference.endpoint==10001&&record.native_metadata.source==10001&&record.payload.size()==1&&record.payload[0]==7);
            assert(record.native_correlation==(is_topic()?0:91));reference=host;return c::error::ok;
        });
    }
    void send_reserve()
    {
        assert(receiver.pump(pipes.transport)==c::error::ok);auto body=pipes.sent(is_topic()?rpc::operation::topic_reserve:rpc::operation::service_reserve);
        w::reader input{body};std::uint64_t until{};assert(input.u64(consumer)&&consumer&&input.u64(until)&&until==deadline&&input.done());
    }
    void reserve_reply(bool malformed=false,c::error status=c::error::ok,c::effect effect=c::effect::none)
    {
        std::uint8_t byte=7;c::metadata info;info.source=10001;info.generation=71;info.sequence=31;
        auto body=encode([&](auto& out){return receive::encode(out,{10001,71,31,11},deadline,info,is_topic()?0:91,
            is_topic()?"topic.message":direction==worker::receive_direction::service_request?"service.request":"service.response",
            {direction==worker::receive_direction::service_response?"test/Response":"test/Request","scene-canonical",{&byte,1}});});
        if(malformed)body.pop_back();
        if(status!=c::error::ok)body.clear();
        pipes.reply(status,effect,body);assert(receiver.pump(pipes.transport)==c::error::ok);
    }
    c::error settle(bool ack,std::uint64_t key,c::receipt& output,receive::reference const* replacement=nullptr)
    {
        auto value=altered(is_topic()?(ack?c::permission::topic_acknowledge:c::permission::topic_release):
            (ack?c::permission::service_acknowledge:c::permission::service_release));
        return receiver.settle(value,replacement?*replacement:reference,{origin.resource,key},ack,c::now()+2000000000,output);
    }
    void send_settle(bool ack,std::uint64_t expected_key)
    {
        assert(receiver.pump(pipes.transport)==c::error::ok);auto body=pipes.sent(is_topic()?(ack?rpc::operation::topic_acknowledge:rpc::operation::topic_release):
            (ack?rpc::operation::service_acknowledge:rpc::operation::service_release));
        w::reader input{body};std::uint64_t actual{},key{},sequence{},nonce{};
        assert(input.u64(actual)&&actual==consumer&&input.u64(key)&&key==expected_key&&input.u64(sequence)&&sequence==31&&input.u64(nonce)&&nonce==11&&input.done());
    }
    void settle_reply(c::error status=c::error::ok,c::effect effect=c::effect::locally_accepted,bool malformed=false)
    {
        bytes body;if(status==c::error::ok)body=encode([&](auto& out){return contract::encode_receipt(out,{981,71,{},status,effect});});
        if(malformed)body.pop_back();
        pipes.reply(status,effect,body);assert(receiver.pump(pipes.transport)==c::error::ok);
    }
    void borrowed(){assert(reserve()==c::error::again);send_reserve();reserve_reply();assert(reserve()==c::error::ok);}
};
#ifndef UWVM_COMMUNICATION_RECEIVES_FIXTURE_ONLY
static void decode_recovery_and_exact_receipt()
{
    for(auto direction:{worker::receive_direction::topic,worker::receive_direction::service_request,worker::receive_direction::service_response})
    {
        receive_fixture f{direction};f.reentrant=true;
        assert(f.reserve()==c::error::again&&f.charged()==2&&f.receiver.occupied()&&f.pipes.empty());
        assert(f.reserve()==c::error::again&&f.charged()==2);f.send_reserve();f.reserve_reply();
        f.failed_decode=true;assert(f.reserve()==c::error::no_memory&&f.receiver.occupied());
        f.failed_decode=false;assert(f.reserve()==c::error::ok&&f.charged()==2&&f.deliveries==2&&f.pipes.empty());
        auto reference=f.reference;auto bad=reference;++bad.nonce;c::receipt receipt;
        assert(f.settle(true,1,receipt,&bad)==c::error::denied&&f.charged()==2);
        assert(f.settle(true,2,receipt)==c::error::stale);
        assert(f.settle(true,1,receipt)==c::error::again&&f.charged()==3&&f.reserve()==c::error::busy);
        assert(f.settle(true,1,receipt)==c::error::again&&f.charged()==3);
        assert(f.settle(false,1,receipt)==c::error::stale);f.send_settle(true,1);f.settle_reply();
        assert(f.settle(true,1,receipt)==c::error::ok&&!f.receiver.occupied()&&receipt.request&&receipt.request!=981&&receipt.provider_generation==71&&receipt.key.sequence==1);
        auto id=receipt.request;assert(f.settle(true,1,receipt)==c::error::ok&&receipt.request==id&&f.charged()==3&&f.pipes.empty());
        assert(f.settle(false,1,receipt)==c::error::stale);
    }
}
static void cleanup_and_native_sequence()
{
    receive_fixture f;f.borrowed();c::receipt receipt;
    assert(f.settle(true,1,receipt)==c::error::again);f.send_settle(true,1);f.settle_reply(c::error::timeout,c::effect::none);
    assert(f.settle(true,1,receipt)==c::error::ok&&receipt.status==c::error::timeout&&receipt.observed==c::effect::none&&f.receiver.occupied());
    auto charge=f.charged();assert(f.settle(false,2,receipt)==c::error::again&&f.charged()==charge);f.send_settle(false,1);f.settle_reply();
    assert(f.settle(false,2,receipt)==c::error::ok&&!f.receiver.occupied()&&receipt.key.sequence==2&&f.pipes.empty());
    receive_fixture exhausted{worker::receive_direction::topic,2};exhausted.borrowed();
    assert(exhausted.settle(true,1,receipt)==c::error::again&&exhausted.pipes.empty()); // Admission refused; no ACK was queued.
    assert(exhausted.authority->close(exhausted.who,exhausted.node_cap)==error::success);
    assert(exhausted.authority->close(exhausted.who,exhausted.request_cap)==error::success);
    assert(exhausted.reserve()==c::error::bad_handle);
    auto release=exhausted.token(c::permission::topic_release);s::group_lease held;
    assert(exhausted.dispatch.revalidate(release,held)==error::success);held.release();
    assert(exhausted.settle(false,1,receipt)==c::error::again&&exhausted.charged()==2);exhausted.send_settle(false,1);exhausted.settle_reply();
    assert(exhausted.settle(false,1,receipt)==c::error::ok&&!exhausted.receiver.occupied()&&exhausted.charged()==2);
}
static void unknown_and_current_caller()
{
    for(bool malformed:{false,true})
    {
        receive_fixture f;f.borrowed();c::receipt receipt;assert(f.settle(true,1,receipt)==c::error::again);f.send_settle(true,1);
        if(malformed)f.settle_reply(c::error::ok,c::effect::locally_accepted,true);
        else{ s::communication_jobs::receipt ignored;assert(f.receiver.cancel(f.pipes.transport,ignored)==c::error::ok&&ignored.observed==c::effect::unknown);}
        assert(f.settle(true,1,receipt)==c::error::ok&&receipt.observed==c::effect::unknown&&f.receiver.occupied());
        auto id=receipt.request;assert(f.settle(true,1,receipt)==c::error::ok&&receipt.request==id&&f.pipes.empty());
        assert(f.reserve()==c::error::cancelled&&f.settle(false,2,receipt)==c::error::cancelled);
    }
    receive_fixture malformed;assert(malformed.reserve()==c::error::again);malformed.send_reserve();malformed.reserve_reply(true);
    assert(malformed.reserve()==c::error::io&&malformed.receiver.occupied()&&!malformed.deliveries);
    receive_fixture caller;caller.borrowed();auto different=caller.origin;++different.caller_generation;
    assert(caller.reserve(&different)==c::error::denied);different=caller.origin;++different.origins[1].resource;assert(caller.reserve(&different)==c::error::denied);
    assert(caller.authority->close(caller.who,caller.sub_cap)==error::success);
    c::receipt receipt;assert(caller.settle(false,1,receipt)==c::error::bad_handle&&caller.pipes.empty());
    receive_fixture unsent;assert(unsent.reserve()==c::error::again);
    assert(unsent.authority->close(unsent.who,unsent.node_cap)==error::success);
    assert(unsent.receiver.pump(unsent.pipes.transport)==c::error::ok&&unsent.pipes.empty());
    assert(unsent.receiver.reconcile()==c::error::bad_handle&&!unsent.receiver.occupied());
}
static void concurrency_and_fork()
{
    receive_fixture f;std::array<c::error,8> result;std::array<std::thread,8> threads;std::barrier start{8};
    for(std::size_t i{};i<threads.size();++i)threads[i]=std::thread([&,i]{start.arrive_and_wait();result[i]=f.reserve();});
    for(auto& thread:threads)thread.join();
    for(auto value:result)assert(value==c::error::again);
    assert(f.charged()==2);auto pid=::fork();assert(pid>=0);
    if(!pid){assert(f.reserve()==c::error::denied);f.receiver.retire();::_exit(0);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
    f.send_reserve();f.reserve_reply();assert(f.reserve()==c::error::ok);
}
static void cleanup_claim_boundaries()
{
    receive_fixture f{worker::receive_direction::topic,1};auto before=f.charged();
    s::admission_claim cleanup{f.sub_cap,"topic.release",0,false,false,true};s::group_lease held;
    assert(f.authority->admit_group(f.who,{&cleanup,1},held,true)==error::success&&f.charged()==before);
    assert(held.consume(f.who,{&cleanup,1})&&!held.consume(f.who,{&cleanup,1}));held.release();
    for(auto malformed:{s::admission_claim{f.sub_cap,"topic.acknowledge",0,false,false,true},
        s::admission_claim{f.sub_cap,"topic.release",1,false,false,true},s::admission_claim{f.sub_cap,"topic.release",0,true,false,true},
        s::admission_claim{f.sub_cap,"topic.release",0,false,true,true}})
        assert(f.authority->admit_group(f.who,{&malformed,1},held,true)==error::invalid&&!held&&f.charged()==before);
    s::authority disabled;assert(disabled.expose("capability.open",s::exposure::allow)==error::success);
    s::grant policy;policy.alias="cleanup";policy.resource="record";policy.binding="native-communication";policy.kind=s::purpose::topic;
    policy.actions={"topic.release"};policy.limits.calls=0;policy.limits.uncertain=true;policy.limits.window=std::chrono::hours{1};
    assert(disabled.configure(std::move(policy))==error::success&&disabled.enroll(f.who)==error::success);
    assert(disabled.open(f.who,"cleanup",cleanup.capability)==error::success);
    assert(disabled.admit_group(f.who,{&cleanup,1},held,true)==error::denied&&!held); // Cleanup never bypasses import exposure.
}
int main()
{
    decode_recovery_and_exact_receipt();cleanup_and_native_sequence();unknown_and_current_caller();concurrency_and_fork();cleanup_claim_boundaries();
    std::puts("PASS owned communication receives: complete origins, nonblocking pipes, bounded retained payload, host/native keys, exact receipts, cleanup after parent closure, exhausted traffic, unknown ACK, callers and fork");
}
#endif
