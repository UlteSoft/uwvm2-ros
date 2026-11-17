/** @file @brief Creation-key/authority/debt ledger with actual nonblocking pipe writes.
 * @details This finite transport fixture does not substitute for sealed native
 * endpoint/VM integration. Cleanup(true) explicitly models trusted positive
 * endpoint teardown evidence; no test treats timeout as such evidence.
 */
#define main communication_async_fixture_entry
#include "communication_worker_async.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_creation.h>
#include <thread>
namespace creation=s::communication_creation;
namespace open_contract=uwvm2::imported::ros::communication_open_contract;
struct opening
{
    fixture pipes;creation::ledger ledger;creation::identity identity;creation::plan plan;
    std::array<std::uint8_t,24> carrier{};
    explicit opening(std::uint64_t key=1,std::optional<std::uint64_t> calls={}):pipes(16384,calls)
    {
        plan.authority={17,1,1};plan.selected.identity="topic.profile";plan.selected.digest[0]=1;plan.kind=open_contract::endpoint_kind::topic;
        plan.worker_binding=1;plan.provider_generation=71;plan.carrier_bytes=256;plan.max_deadline_ns=1000000000;
        assert(ledger.configure({&plan,1},512)==error::ok);identity.who=pipes.rights.who;identity.authority=plan.authority;identity.key=key;
        identity.deadline=s::communication::now()+900000000;identity.count=4;
        for(std::size_t i{};i<4;++i)
        {auto kind=i==0?creation::dependency::operation:i==1?creation::dependency::node:i==2?creation::dependency::context:creation::dependency::schema;
         identity.origins[i]={kind,{17,i+1,1},pipes.rights.claims[i].capability,i==1?44u:i==2?55u:0u};}
        encode();
    }
    void encode()
    {s::wire::writer out{carrier};assert(out.u64(42)&&out.u64(identity.key)&&out.u64(identity.deadline));}
    creation::view submit()
    {
        creation::view prepared,queued;assert(ledger.reserve(identity,carrier,prepared)==error::ok&&prepared.reserved);
        auto claims=pipes.rights.claims;claims[0].bytes=carrier.size()+open_contract::receipt_bytes;s::group_lease group;
        assert(pipes.rights.authority.admit_group(identity.who,claims,group)==s::error::success);
        assert(ledger.commit(identity.who,prepared.receipt.request_id,group,queued)==error::ok&&!queued.reserved);
        return queued;
    }
};
#ifndef UWVM_COMMUNICATION_CREATION_CORE_FIXTURE_ONLY
static void exact_creation_groups()
{
    for(unsigned fault{};fault<7;++fault)
    {
        opening f;creation::view pending,answer;assert(f.ledger.reserve(f.identity,f.carrier,pending)==error::ok);
        auto claims=f.pipes.rights.claims;claims[0].bytes=f.carrier.size()+open_contract::receipt_bytes;
        if(fault==1)claims[0].action="topic.publish";
        if(fault==2){s::handle other{};assert(f.pipes.rights.authority.open(f.identity.who,"topic",other)==s::error::success);claims[0].capability=other;}
        if(fault==3)std::swap(claims[1],claims[2]);
        if(fault==4)claims[1].validation_only=false;
        if(fault==5)++claims[0].bytes;
        s::group_lease held;assert(f.pipes.rights.authority.admit_group(f.identity.who,claims,held,fault!=0)==s::error::success&&held.size()==4);
        if(fault==6)assert(f.pipes.rights.authority.close(f.identity.who,claims[1].capability)==s::error::success);
        assert(f.ledger.commit(f.identity.who,pending.receipt.request_id,held,answer)==error::denied&&f.pipes.drain()==0);
        assert(f.ledger.inspect(f.identity.who,pending.receipt.request_id,answer)==error::ok&&answer.reserved);
    }
    {
        opening f;auto queued=f.submit();creation::work work;a::ticket wire;
        assert(f.pipes.transport.stage(rpc::operation::topic_open,1,{},f.identity.deadline,wire)==error::ok);
        auto claims=f.pipes.rights.claims;claims[0].action="topic.publish";s::group_lease wrong;
        assert(f.pipes.rights.authority.admit_group(f.identity.who,claims,wrong,false)==s::error::success);
        assert(f.ledger.begin(queued.receipt.request_id,wrong,work)==error::denied&&f.ledger.send(queued.receipt.request_id,wrong,f.pipes.transport,wire)==error::denied&&f.pipes.drain()==0);
        wrong.release();auto held=f.pipes.rights.admit();assert(f.ledger.begin(queued.receipt.request_id,held,work)==error::ok);held.release();
        assert(f.ledger.complete(queued.receipt.request_id,error::ok,s::communication::effect::locally_accepted,123)==error::ok);
        assert(f.pipes.rights.authority.admit_group(f.identity.who,claims,wrong,false)==s::error::success);open_contract::published published;
        assert(f.ledger.publish(f.identity.who,queued.receipt.request_id,wrong,456,published)==error::denied);
        wrong.release();held=f.pipes.rights.admit();assert(f.ledger.publish(f.identity.who,queued.receipt.request_id,held,456,published)==error::ok);
    }
    {
        opening f;creation::view reserved,queued;assert(f.ledger.reserve(f.identity,f.carrier,reserved)==error::ok);
        auto claims=f.pipes.rights.claims;claims[0].bytes=f.carrier.size()+open_contract::receipt_bytes;s::group_lease held;
        assert(f.pipes.rights.authority.admit_group(f.identity.who,claims,held)==s::error::success);
        assert(f.ledger.commit(f.identity.who,reserved.receipt.request_id,held,queued)==error::ok);
        assert(f.ledger.commit(f.identity.who,reserved.receipt.request_id,held,queued)==error::ok);
        assert(f.ledger.cancel(f.identity.who,queued.receipt.request_id,queued)==error::ok&&f.ledger.release(f.identity.who,queued.receipt.request_id)==error::ok);
        ++f.identity.key;f.encode();assert(f.ledger.reserve(f.identity,f.carrier,reserved)==error::ok);
        assert(f.ledger.commit(f.identity.who,reserved.receipt.request_id,held,queued)==error::denied); // Consumed charge cannot publish a second creation.
    }
}
struct managed_creation
{
    opening source;std::atomic<unsigned> charges{};std::atomic<bool> entered{},proceed{true};bool reenter{};
    explicit managed_creation(std::uint64_t limit):source(1,limit){}
    static error charge(void* opaque,creation::identity const& identity,std::uint64_t bytes,s::group_lease& held)noexcept
    {
        auto& self=*static_cast<managed_creation*>(opaque);auto& f=self.source;++self.charges;
        creation::view result;
        assert(f.ledger.reserve(identity,f.carrier,result)==error::busy);
        if(self.reenter)assert(self.enqueue(result)==error::again);
        self.entered=true;while(!self.proceed.load())std::this_thread::yield();
        creation::claim_set claims;auto expected=creation::admission_claims(identity,f.plan.kind,claims,bytes);
        return static_cast<error>(f.pipes.rights.authority.admit_group(identity.who,expected,held));
    }
    error enqueue(creation::view& output){return source.ledger.enqueue(source.identity,source.carrier,charge,this,output);}
};
static void managed_creation_admission()
{
    for(auto budget:{0u,1u})
    {
        managed_creation f{budget};f.reenter=true;creation::view result;
        assert(f.enqueue(result)==(budget?error::ok:error::again)&&f.charges==1&&f.source.pipes.drain()==0);
        if(budget)
        {
            auto request=result.receipt.request_id;assert(f.enqueue(result)==error::ok&&result.receipt.request_id==request&&f.charges==1);
            assert(f.source.ledger.abandon(f.source.identity.who,request)==error::denied);
        }
    }
    for(unsigned repeat{};repeat<8;++repeat)
    {
        managed_creation f{1};f.proceed=false;creation::view accepted,duplicate;error status{};
        std::thread submitting{[&]{status=f.enqueue(accepted);}};
        while(!f.entered.load())std::this_thread::yield();
        assert(f.enqueue(duplicate)==error::again);f.proceed=true;submitting.join();
        assert(status==error::ok&&f.charges==1&&f.enqueue(duplicate)==error::ok&&duplicate.receipt.request_id==accepted.receipt.request_id&&f.charges==1);
    }
}
int main()
{
    assert(communication_async_fixture_entry()==0);
    exact_creation_groups();managed_creation_admission();
    for(auto observed:{s::communication::effect::locally_accepted,s::communication::effect::observed_remote})
    {
        opening f;auto queued=f.submit();creation::work work;auto held=f.pipes.rights.admit();
        assert(f.ledger.begin(queued.receipt.request_id,held,work)==error::ok);held.release();
        assert(f.ledger.complete(queued.receipt.request_id,static_cast<error>(999),s::communication::effect::none,0)==error::invalid);
        assert(f.ledger.complete(queued.receipt.request_id,error::ok,static_cast<s::communication::effect>(255),123)==error::invalid);
        assert(f.ledger.complete(queued.receipt.request_id,error::io,observed,0)==error::ok);
        creation::view failed;assert(f.ledger.inspect(f.identity.who,queued.receipt.request_id,failed)==error::ok&&
            failed.receipt.result==error::io&&failed.receipt.observed==s::communication::effect::unknown&&failed.receipt.cleanup_debt);
        assert(f.ledger.release(f.identity.who,queued.receipt.request_id)==error::busy);
        assert(f.ledger.cleanup(queued.receipt.request_id,true)==error::ok&&f.ledger.release(f.identity.who,queued.receipt.request_id)==error::busy);
    }
    {
        for(auto kind:{s::purpose::node,s::purpose::topic,s::purpose::service,s::purpose::action})
        {assert(s::permits(kind,"communication.open.take")&&s::permits(kind,"communication.open.cancel")&&s::permits(kind,"request.describe")&&s::permits(kind,"request.release"));}
        assert(s::permits(s::purpose::context,"request.describe")&&!s::permits(s::purpose::context,"communication.open.take"));
        assert(!s::permits(s::purpose::device,"communication.open.take")&&!s::permits(s::purpose::language_model,"communication.open.cancel"));
        std::array<std::uint8_t,16> bytes{};s::wire::writer out{bytes};out.u64(123);open_contract::request request;
        assert(open_contract::decode(open_contract::operation::take,out.written(),request)&&request.request_id==123);
        assert(!open_contract::decode(open_contract::operation::take,bytes,request));
    }
    {
        opening f;creation::view reserved,again;assert(f.ledger.reserve(f.identity,f.carrier,reserved)==error::ok&&reserved.reserved);
        assert(f.ledger.reserve(f.identity,f.carrier,again)==error::ok&&again.receipt.request_id==reserved.receipt.request_id);
        auto changed=f.identity;++changed.who.generation;assert(f.ledger.reserve(changed,f.carrier,again)==error::busy);
        changed=f.identity;--changed.count;assert(f.ledger.reserve(changed,f.carrier,again)==error::denied);
        s::group_lease empty;assert(f.ledger.commit(f.identity.who,reserved.receipt.request_id,empty,again)==error::denied);
        assert(f.ledger.abandon(f.identity.who,reserved.receipt.request_id)==error::ok);
        auto queued=f.submit();auto request=queued.receipt.request_id;
        assert(f.ledger.reserve(f.identity,f.carrier,again)==error::ok&&!again.reserved&&again.receipt.request_id==request);
        changed=f.identity;changed.origins[1].resource=99;assert(f.ledger.reserve(changed,f.carrier,again)==error::denied);
        creation::work work;assert(f.ledger.next(work)==error::ok&&work.request==request&&work.original==f.identity);
        a::ticket wire;assert(f.pipes.transport.stage(rpc::operation::topic_open,1,{},f.identity.deadline,wire)==error::ok);
        auto held=f.pipes.rights.admit();assert(f.ledger.send(request,held,f.pipes.transport,wire)==error::ok);held.release();
        assert(f.pipes.drain()==48);std::array<std::uint8_t,8> native{};s::wire::writer native_writer{native};assert(native_writer.u64(123));f.pipes.response(1,native);
        assert(f.pipes.transport.receive(wire)==error::ok);a::result received;std::array<std::uint8_t,256> copy;
        assert(f.pipes.transport.copy(wire,copy,received)==error::ok&&received.bytes==8);
        assert(f.ledger.complete(request,received.status,received.observed,123)==error::ok&&f.ledger.release(f.identity.who,request)==error::busy);
        open_contract::published published;held=f.pipes.rights.admit();assert(f.ledger.publish(f.identity.who,request,held,456,published)==error::ok);
        assert(f.ledger.publish(f.identity.who,request,held,456,published)==error::ok&&f.ledger.publish(f.identity.who,request,held,457,published)==error::denied);held.release();
        std::array<std::uint8_t,64> encoded{};s::wire::writer output{encoded};assert(open_contract::encode(output,published)&&output.size()==64);
        assert(published.kind==open_contract::endpoint_kind::topic&&published.handle==456&&published.creation.request_id!=published.handle);
        assert(f.ledger.release(f.identity.who,request)==error::busy);
        assert(f.ledger.inspect(f.identity.who,request,again)==error::ok&&again.endpoint_live&&again.receipt.status==open_contract::state::published);
        auto old=f.identity;auto old_carrier=f.carrier;++f.identity.key;f.encode();
        assert(f.ledger.reserve(f.identity,f.carrier,again)==error::busy);
        assert(f.ledger.cleanup(request,false)==error::busy&&f.ledger.release(f.identity.who,request)==error::busy);
        assert(f.ledger.cleanup(request,true)==error::ok&&f.ledger.release(f.identity.who,request)==error::ok);
        assert(f.ledger.reserve(f.identity,f.carrier,again)==error::ok&&again.reserved);assert(f.ledger.abandon(f.identity.who,again.receipt.request_id)==error::ok);
        assert(f.ledger.reserve(old,old_carrier,again)==error::ok&&again.receipt.status==open_contract::state::released);
        auto second=f.submit();assert(f.ledger.reserve(old,old_carrier,again)==error::stale);
        assert(f.ledger.cancel(f.identity.who,second.receipt.request_id,again)==error::ok&&!again.receipt.cleanup_debt&&again.receipt.observed==s::communication::effect::none);
        assert(f.ledger.release(f.identity.who,second.receipt.request_id)==error::ok);
    }
    {
        opening f;auto queued=f.submit();auto request=queued.receipt.request_id;a::ticket wire;
        assert(f.pipes.transport.stage(rpc::operation::topic_open,1,{},f.identity.deadline,wire)==error::ok);
        auto held=f.pipes.rights.admit();assert(f.ledger.send(request,held,f.pipes.transport,wire)==error::ok);held.release();creation::view result;
        assert(f.ledger.cancel(f.identity.who,request,result)==error::ok&&result.receipt.cleanup_debt&&result.receipt.observed==s::communication::effect::unknown);
        assert(f.ledger.release(f.identity.who,request)==error::busy&&f.ledger.cleanup(request,false)==error::busy&&f.ledger.cleanup(request,true)==error::ok);
        // Process cleanup resolves lifetime debt, not an unobserved native result.
        assert(f.ledger.release(f.identity.who,request)==error::busy);
        assert(f.ledger.inspect(f.identity.who,request,result)==error::ok&&result.provider_generation==71);
        std::array<std::uint8_t,256> canonical;s::wire::writer encoded{canonical};
        assert(open_contract::encode_description(encoded,result.receipt,result.provider_generation));s::wire::reader decoded{encoded.written()};
        std::uint64_t request_id{},generation{};std::string_view phase,effect,reason;
        assert(decoded.u64(request_id)&&request_id==request&&decoded.text(phase)&&phase=="outcome_unknown"&&decoded.text(effect)&&effect=="unknown"&&
            decoded.u64(generation)&&generation==71&&decoded.text(reason)&&reason=="outcome.unknown"&&decoded.done());
        std::array<std::uint8_t,8> released;s::wire::writer release{released};assert(open_contract::encode_release(release,false)&&release.size()==8);
    }
    for(unsigned run{};run<12;++run)
    {
        opening f;auto queued=f.submit();auto request=queued.receipt.request_id;a::ticket wire;
        assert(f.pipes.transport.stage(rpc::operation::topic_open,1,{},f.identity.deadline,wire)==error::ok);
        struct cancel_task{opening* value;s::handle request;std::atomic<bool> entered{};creation::view result{};} task{&f,request};pthread_t thread;
        auto held=f.pipes.rights.admit();assert(!::pthread_create(&thread,nullptr,[](void* opaque)->void*
        {auto& work=*static_cast<cancel_task*>(opaque);work.entered=true;assert(work.value->ledger.cancel(work.value->identity.who,work.request,work.result)==error::ok);return nullptr;},&task));
        while(!task.entered.load()){}
        auto status=f.ledger.send(request,held,f.pipes.transport,wire);held.release();assert(status==error::ok||status==error::cancelled);assert(!::pthread_join(thread,nullptr));
        auto sent=f.pipes.drain();assert(sent==0||sent==48);
        assert(task.result.receipt.cleanup_debt==bool(sent)&&task.result.receipt.observed==(sent?s::communication::effect::unknown:s::communication::effect::none));
    }
    std::puts("PASS asynchronous communication creation: exact key, explicit receipt/kind, independent origin group, one charge, retained publication, atomic send/cancel and unreleasable unknown debt");
}
#endif
