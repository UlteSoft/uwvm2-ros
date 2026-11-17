/** @file @brief Actual dispatcher/owned-queue accounting and completion boundaries. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
namespace rpc=s::communication_worker_protocol;
struct pipe_pair
{
    std::array<int,2> incoming{},outgoing{},diagnostics{};int identity{};
    s::communication_worker_async::transport transport;
    pipe_pair()
    {
        assert(!::pipe2(incoming.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(outgoing.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({incoming[1],outgoing[0],diagnostics[0],identity},{128,256,256,32,10000000000,1000000},71)==c::error::ok);
    }
    ~pipe_pair(){transport.retire();for(auto fd:{incoming[0],incoming[1],outgoing[0],outgoing[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    bytes request()
    {
        std::array<std::uint8_t,512> storage;auto count=::read(incoming[0],storage.data(),storage.size());
        if(count<0){assert(errno==EAGAIN);return {};}
        assert(count>0);return {storage.begin(),storage.begin()+count};
    }
    void response(std::uint64_t sequence,c::error status,c::effect observed,w::bytes body)
    {
        auto frame=encode([&](auto& out){return out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(sequence)&&
            out.u64(static_cast<std::uint64_t>(status))&&out.u64(static_cast<std::uint64_t>(observed))&&out.u64(body.size())&&out.encoded(body);});
        assert(::write(outgoing[1],frame.data(),frame.size())==static_cast<ssize_t>(frame.size()));
    }
};
struct queued_fixture:fixture
{
    explicit queued_fixture(std::uint64_t calls=8):fixture(false,false,{},false,false,false,false,c::service_role::client,calls)
    {
        native->queue_dispatch=&dispatch;assert(dispatch.prepare(native)==error::success);
        context_id=opened(contract::operation::context_open,context_cap,open_profile("ctx"));
        auto input=encode([&](auto& out){return out.u64(context_cap)&&out.u64(context_id)&&contract::encode_profile(out,profile("node"));});
        node_id=opened(contract::operation::node_open,node_cap,input);
        pub_id=opened(contract::operation::topic_open,pub_cap,endpoint("pub","publisher"));
    }
    auto& worker(){return *native->publisher->queued;}
    c::operation_token current(){std::lock_guard lock{native->publisher->operation_mutex};return native->publisher->last_operation;}
    static void receipt(bytes const& body,std::string_view state,std::string_view effect,std::string_view reason)
    {
        w::reader in{body};std::uint64_t request{},generation{};std::string_view a,b,c;
        assert(in.u64(request)&&request&&in.text(a)&&a==state&&in.text(b)&&b==effect&&in.u64(generation)&&generation==71&&in.text(c)&&c==reason&&in.done());
    }
};
static void no_double_charge_and_completion()
{
    for(auto outcome:{c::error::ok,c::error::denied,c::error::timeout})
    {
        queued_fixture f{2};pipe_pair pipes;bytes response;auto input=f.mutation(f.pub_id,1,c::now()+2000000000);
        assert(f.admissions("publisher")==1);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response,159)==error::too_large&&f.admissions("publisher")==1&&pipes.request().empty());
        auto rejected=[](w::bytes){return false;};
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response,4096,rejected)==error::io&&f.admissions("publisher")==2);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success&&f.admissions("publisher")==2);
        queued_fixture::receipt(response,"admitted","locally_accepted","none");
        auto identity=f.current();assert(identity.request&&identity.session==f.pub_id&&identity.sequence==1);
        assert(f.worker().pump(pipes.transport)==c::error::ok);auto sent=pipes.request();rpc::request packet;
        assert(rpc::decode_request(sent,71,128,packet)&&packet.action==rpc::operation::topic_publish);
        w::reader body{packet.body};w::bytes payload;std::uint64_t deadline{};
        assert(body.blob(payload)&&payload.size()==1&&payload[0]==1&&body.u64(deadline)&&body.done());
        std::array<std::uint8_t,3> retained{8,7,6};auto effect=outcome==c::error::ok?c::effect::locally_accepted:c::effect::none;
        pipes.response(1,outcome,effect,retained);assert(f.worker().pump(pipes.transport)==c::error::ok);
        c::operation_token completed;c::native_result result;std::array<std::uint8_t,2> short_body;std::size_t required{};
        assert(f.worker().completion(completed,result,short_body,required)==c::error::too_large&&required==3);
        std::array<std::uint8_t,256> full;assert(f.worker().completion(completed,result,full,required)==c::error::ok&&required==3&&result.status==outcome&&
            result.observed==effect&&!result.deferred&&result.request_identity==identity.request&&std::equal(retained.begin(),retained.end(),full.begin()));
        auto wrong=result;++wrong.provider_generation;assert(f.dispatch.settle_operation(completed,wrong)==error::conflict);
        wrong=result;wrong.status=static_cast<c::error>(999);assert(f.dispatch.settle_operation(completed,wrong)==error::invalid);
        assert(f.dispatch.settle_operation(completed,result)==error::success&&f.dispatch.settle_operation(completed,result)==error::success);
        wrong=result;wrong.observed=c::effect::unknown;assert(f.dispatch.settle_operation(completed,wrong)==error::conflict);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success&&f.admissions("publisher")==2&&pipes.request().empty());
        queued_fixture::receipt(response,outcome==c::error::ok?"admitted":"failed",outcome==c::error::ok?"locally_accepted":"none",
            outcome==c::error::ok?"none":outcome==c::error::timeout?"deadline.elapsed":"provider.error");
        assert(f.worker().release(completed)==c::error::ok);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,2,c::now()+2000000000),response)==error::again&&f.admissions("publisher")==2&&pipes.request().empty());
    }
}
static void contradictory_completion_is_retained()
{
    for(auto status:{c::error::ok,c::error::io})
    {
        queued_fixture f;pipe_pair pipes;bytes response;auto input=f.mutation(f.pub_id,1,c::now()+2000000000);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success);
        assert(f.worker().pump(pipes.transport)==c::error::ok&&!pipes.request().empty());
        pipes.response(1,status,status==c::error::ok?c::effect::none:c::effect::locally_accepted,{});
        assert(f.worker().pump(pipes.transport)==c::error::ok);
        c::operation_token token;c::native_result result;std::size_t size{};
        assert(f.worker().completion(token,result,{},size)==c::error::ok&&f.dispatch.settle_operation(token,result)==error::success);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success);
        queued_fixture::receipt(response,"outcome_unknown","unknown","outcome.unknown");
        assert(f.dispatch.close(f.who,f.pub_cap,f.pub_id)==error::again);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,2,c::now()+2000000000),response)==error::again);
        assert(pipes.request().empty());
    }
}
static void revoke_before_and_after_send()
{
    for(bool send:{false,true})
    {
        queued_fixture f;pipe_pair pipes;bytes response;auto input=f.mutation(f.pub_id,1,c::now()+2000000000);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success);auto identity=f.current();
        if(send){assert(f.worker().pump(pipes.transport)==c::error::ok);assert(!pipes.request().empty());}
        assert(f.authority->revoke("request",[&](auto alias){f.dispatch.revoke(alias);})==error::success);
        if(send){s::communication_jobs::receipt cancelled;assert(f.worker().cancel(identity,pipes.transport,cancelled)==c::error::ok&&cancelled.cleanup_debt);}
        else assert(f.worker().pump(pipes.transport)==c::error::ok);
        assert(pipes.request().empty());c::operation_token completed;c::native_result result;std::size_t size{};std::array<std::uint8_t,256> body;
        assert(f.worker().completion(completed,result,body,size)==c::error::ok&&size==0&&result.observed==(send?c::effect::unknown:c::effect::none));
        assert(f.dispatch.settle_operation(completed,result)==error::success);
        assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success); // Receipt recovery cannot produce another effect.
        assert(f.worker().release(completed)==(send?c::error::busy:c::error::ok));
        assert(f.call(contract::operation::topic_publish,f.pub_cap,f.mutation(f.pub_id,2,c::now()+2000000000),response)==error::denied);
    }
}
static void pending_and_capacity()
{
    queued_fixture f;pipe_pair pipes;bytes response;auto input=f.mutation(f.pub_id,1,c::now()+2000000000);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,input,response)==error::success&&f.admissions("publisher")==2);
    auto next=f.mutation(f.pub_id,2,c::now()+2000000000);
    assert(f.call(contract::operation::topic_publish,f.pub_cap,next,response)==error::again&&f.admissions("publisher")==2&&pipes.request().empty());
    auto identity=f.current(); // The blocked next mutation never reached the native adapter.
    assert(f.dispatch.close(f.who,f.pub_cap,f.pub_id)==error::again);
    assert(f.worker().pump(pipes.transport)==c::error::ok);assert(!pipes.request().empty());
    pipes.response(1,c::error::denied,c::effect::none,{});assert(f.worker().pump(pipes.transport)==c::error::ok);
    c::operation_token complete;c::native_result result;std::size_t required{};
    assert(f.worker().completion(complete,result,{},required)==c::error::ok&&complete.request==identity.request);
    assert(f.dispatch.settle_operation(complete,result)==error::success&&f.worker().release(complete)==c::error::ok);
    auto before=f.admissions("publisher");assert(f.call(contract::operation::topic_publish,f.pub_cap,next,response)==error::success&&f.admissions("publisher")==before+1);
}
int main()
{
    no_double_charge_and_completion();contradictory_completion_is_retained();revoke_before_and_after_send();pending_and_capacity();
    std::puts("PASS dispatcher owned mutations: reservation-first single charge, zero remaining-budget retry, deferred exact completion, no-effect failure, cancellation unknown, complete-origin fencing and pending lifetime");
}
