/** @file @brief Canonical receive dispatch: no short-copy effects, exact receipts and explicit cleanup. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
namespace receive=uwvm2::imported::ros::communication_receive_contract;
struct receive_dispatch_fixture:fixture
{
    std::uint64_t deadline=c::now()+5000000000ull;std::size_t required{};bytes output;
    explicit receive_dispatch_fixture(std::optional<std::uint64_t> calls={}):fixture(true,true,{},false,false,false,false,c::service_role::client,calls)
    {
        sub_id=opened(contract::operation::topic_open,sub_cap,endpoint("sub","subscription"));
        native->subscriber->evidence.provider_generation=71;
        c::metadata info;info.source=sub_id;info.generation=native->generation;std::array<std::uint8_t,1> body{7};
        assert(native->queue->push(info,body)==c::error::ok);
    }
    bytes reservation(){return encode([&](auto& out){return out.u64(sub_id)&&out.u64(deadline);});}
    receive::reference reference()
    {
        w::reader in{output};receive::reference result;assert(in.u64(result.endpoint)&&result.endpoint==sub_id&&in.u64(result.provider_generation)&&result.provider_generation==71&&
            in.u64(result.sequence)&&result.sequence&&in.u64(result.nonce)&&result.nonce);return result;
    }
    bytes settle(receive::reference ref,std::uint64_t key=1)
    {return encode([&](auto& out){return out.u64(sub_id)&&out.u64(key)&&out.u64(ref.endpoint)&&out.u64(ref.provider_generation)&&out.u64(ref.sequence)&&out.u64(ref.nonce);});}
    error call_receive(receive::operation operation,w::bytes request,std::size_t capacity=4096,s::communication_dispatch::sink sink={},
                       uwvm2::utils::container::function_ref<bool()> cancelled={})
    {
        auto copy=[&](w::bytes bytes){output.assign(bytes.begin(),bytes.end());return true;};
        auto result=dispatch.invoke_receive(who,operation,sub_cap,request,capacity,required,sink?sink:s::communication_dispatch::sink(copy),cancelled);
        if(result==error::success)
        {auto shape=schema::find(receive::operations[static_cast<std::size_t>(operation)].name);assert(shape&&schema::validate(output,shape->result,{4096,256,64,32}));}
        return result;
    }
};
static void preflight_and_recovery()
{
    receive_dispatch_fixture f;auto input=f.reservation();auto before=f.admissions("subscriber");
    assert(f.call_receive(receive::operation::topic_reserve,input,16)==error::too_large&&f.required>16&&f.admissions("subscriber")==before&&!f.native->queue->has_reservation());
    f.native->subscriber->evidence.provider_generation.reset();
    assert(f.call_receive(receive::operation::topic_reserve,input)==error::unsupported&&!f.native->queue->has_reservation());
    f.native->subscriber->evidence.provider_generation=71;
    auto reject=[&](w::bytes bytes){f.output.assign(bytes.begin(),bytes.end());return false;};
    assert(f.call_receive(receive::operation::topic_reserve,input,4096,reject)==error::io&&f.native->queue->has_reservation()&&f.admissions("subscriber")==before+1);
    auto original=f.output;auto ref=f.reference();
    assert(f.call_receive(receive::operation::topic_reserve,input)==error::success&&f.output==original&&f.admissions("subscriber")==before+1);
    auto ack=f.settle(ref);assert(f.call_receive(receive::operation::topic_acknowledge,ack,159)==error::too_large&&f.admissions("subscriber")==before+1);
    auto forged=ref;++forged.provider_generation;assert(f.call_receive(receive::operation::topic_acknowledge,f.settle(forged))==error::denied);
    assert(f.call_receive(receive::operation::topic_acknowledge,ack,4096,reject)==error::io&&!f.native->queue->has_reservation());
    auto receipt=f.output;assert(f.call_receive(receive::operation::topic_acknowledge,ack)==error::success&&f.output==receipt&&f.admissions("subscriber")==before+2);
    assert(f.call_receive(receive::operation::topic_release,ack)==error::conflict);
    assert(f.native->queue->take([](auto const&){assert(false);return true;})==c::error::again);
}
static void cleanup_after_parent_close()
{
    receive_dispatch_fixture f{2};auto input=f.reservation();assert(f.call_receive(receive::operation::topic_reserve,input)==error::success);auto ref=f.reference();
    assert(f.authority->close(f.who,f.node_cap)==error::success&&f.authority->close(f.who,f.request_cap)==error::success);
    assert(f.call_receive(receive::operation::topic_reserve,input)==error::bad_handle);
    assert(f.call_receive(receive::operation::topic_acknowledge,f.settle(ref))==error::bad_handle);
    assert(f.call_receive(receive::operation::topic_release,f.settle(ref))==error::success&&f.admissions("subscriber")==2&&!f.native->queue->has_reservation());
    assert(f.call_receive(receive::operation::topic_release,f.settle(ref))==error::success&&f.admissions("subscriber")==2);
    assert(f.native->queue->take([](auto const& value){return value.body.size()==1&&value.body[0]==7;})==c::error::ok);
    receive_dispatch_fixture zero{1};assert(zero.call_receive(receive::operation::topic_reserve,zero.reservation())==error::again&&!zero.native->queue->has_reservation());
}
static void cancel_and_current_caller()
{
    receive_dispatch_fixture f;bool cancel{};auto cancelled=[&]{return cancel;};
    auto copy=[&](w::bytes bytes){f.output.assign(bytes.begin(),bytes.end());cancel=true;return true;};
    auto input=f.reservation();assert(f.call_receive(receive::operation::topic_reserve,input,4096,copy,cancelled)==error::cancelled&&f.native->queue->has_reservation());
    cancel=false;auto ref=f.reference();assert(f.call_receive(receive::operation::topic_reserve,input)==error::success&&f.reference()==ref&&f.admissions("subscriber")==2);
    int instance{};s::caller other{&instance,1};assert(f.authority->enroll(other)==error::success);
    auto sink=[](w::bytes){assert(false);return true;};std::size_t required{};
    assert(f.dispatch.invoke_receive(other,receive::operation::topic_reserve,f.sub_cap,input,4096,required,sink)==error::again);
    assert(f.dispatch.invoke_receive(other,receive::operation::topic_release,f.sub_cap,f.settle(ref),4096,required,sink)==error::denied);
    assert(f.call_receive(receive::operation::topic_acknowledge,f.settle(ref))==error::success);
    auto receipt=f.output;assert(f.dispatch.invoke_receive(other,receive::operation::topic_acknowledge,f.sub_cap,f.settle(ref),4096,required,sink)==error::denied&&f.output==receipt);
}
static void service_roles_and_receipts()
{
    for(auto direction:{c::service_role::client,c::service_role::server})
    {
        fixture f{true,true,{},false,false,false,false,direction};f.native->receive_services=true;
        f.service_id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv",direction==c::service_role::client?"client":"server"));
        auto until=c::now()+5000000000ull;auto input=encode([&](auto& out){return out.u64(f.service_id)&&out.u64(until);});
        std::size_t required{};bytes result;bool accepted=false;
        auto sink=[&](w::bytes bytes){result.assign(bytes.begin(),bytes.end());return accepted;};
        auto invoke=[&](auto action,w::bytes body,std::size_t capacity=4096)
        {return f.dispatch.invoke_receive(f.who,action,f.service_cap,body,capacity,required,sink);};
        assert(invoke(receive::operation::service_reserve,input,16)==error::too_large&&!f.native->client->occupied());
        assert(invoke(receive::operation::service_reserve,input)==error::io&&f.native->client->occupied());
        auto original=result;accepted=true;assert(invoke(receive::operation::service_reserve,input)==error::success&&result==original&&f.admissions("service")==2);
        auto shape=schema::find("service.reserve");assert(shape&&schema::validate(result,shape->result,{4096,256,64,32}));
        w::reader fields{result};receive::reference reference;std::uint64_t deadline{};
        assert(fields.u64(reference.endpoint)&&reference.endpoint==f.service_id&&fields.u64(reference.provider_generation)&&reference.provider_generation==71&&
            fields.u64(reference.sequence)&&fields.u64(reference.nonce)&&fields.u64(deadline)&&deadline==until);
        auto settlement=[&](std::uint64_t key)
        {return encode([&](auto& out){return out.u64(f.service_id)&&out.u64(key)&&out.u64(reference.endpoint)&&out.u64(reference.provider_generation)&&out.u64(reference.sequence)&&out.u64(reference.nonce);});};
        auto release=settlement(1);assert(invoke(receive::operation::service_release,release)==error::success&&!f.native->client->occupied()&&f.admissions("service")==2);
        assert(invoke(receive::operation::service_release,release)==error::success&&f.admissions("service")==2);
        assert(invoke(receive::operation::service_reserve,input)==error::success&&f.native->client->occupied());
        w::reader next{result};receive::reference second;
        assert(next.u64(second.endpoint)&&next.u64(second.provider_generation)&&next.u64(second.sequence)&&next.u64(second.nonce)&&
            second.sequence==reference.sequence&&second.nonce!=reference.nonce);reference=second;
        auto ack=settlement(2);accepted=false;assert(invoke(receive::operation::service_acknowledge,ack)==error::io&&!f.native->client->occupied());
        auto receipt=result;accepted=true;assert(invoke(receive::operation::service_acknowledge,ack)==error::success&&result==receipt&&f.admissions("service")==4);
        assert(f.native->client->submissions==0); // Neither operation can send a service response.
    }
}
static void malformed_service_settlement()
{
    for(unsigned fault=1;fault<=3;++fault)
    {
        fixture f;f.native->receive_services=true;
        f.service_id=f.opened(contract::operation::service_open,f.service_cap,f.endpoint("srv","client"));
        f.native->client->invalid_settlement=fault;auto until=c::now()+5000000000ull;
        auto reserve=encode([&](auto& out){return out.u64(f.service_id)&&out.u64(until);});
        bytes output;std::size_t required{};auto sink=[&](w::bytes value){output.assign(value.begin(),value.end());return true;};
        auto invoke=[&](auto action,w::bytes input){return f.dispatch.invoke_receive(f.who,action,f.service_cap,input,4096,required,sink);};
        assert(invoke(receive::operation::service_reserve,reserve)==error::success);
        receive::reference reference;w::reader fields{output};assert(fields.u64(reference.endpoint)&&fields.u64(reference.provider_generation)&&fields.u64(reference.sequence)&&fields.u64(reference.nonce));
        auto mutation=[&](std::uint64_t key){return encode([&](auto& out){return out.u64(f.service_id)&&out.u64(key)&&out.u64(reference.endpoint)&&
            out.u64(reference.provider_generation)&&out.u64(reference.sequence)&&out.u64(reference.nonce);});};
        auto ack=mutation(1);assert(invoke(receive::operation::service_acknowledge,ack)==error::success&&f.native->client->settlements==1);
        w::reader receipt{output};std::uint64_t request{},generation{};std::string_view state,effect,reason;
        assert(receipt.u64(request)&&request&&receipt.text(state)&&state=="outcome_unknown"&&receipt.text(effect)&&effect=="unknown"&&
            receipt.u64(generation)&&generation==71&&receipt.text(reason)&&reason=="outcome.unknown"&&receipt.done());
        auto original=output;assert(invoke(receive::operation::service_acknowledge,ack)==error::success&&output==original&&f.native->client->settlements==1);
        assert(invoke(receive::operation::service_reserve,reserve)==error::conflict&&invoke(receive::operation::service_release,mutation(2))==error::conflict&&f.native->client->settlements==1);
        bytes ignored;assert(f.call(contract::operation::service_take,f.service_cap,resource(f.service_id),ignored)==error::conflict);
        assert(f.dispatch.close(f.who,f.service_cap,f.service_id)==error::conflict);
    }
}
int main()
{
    preflight_and_recovery();cleanup_after_parent_close();cancel_and_current_caller();service_roles_and_receipts();malformed_service_settlement();
    std::puts("PASS canonical receive dispatch: exact provider identity, bounded output before polling, failed copy recovery, one charge, ACK receipt replay, original record release, zero-budget preflight, parent closure and caller fencing");
}
