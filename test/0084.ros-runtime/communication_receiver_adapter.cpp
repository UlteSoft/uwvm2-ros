/** @file @brief Real canonical dispatcher plus bounded isolated receive adapter and private pipes. */
#define UWVM_COMMUNICATION_RECEIVES_FIXTURE_ONLY
#include "communication_receives.cpp"
struct adapter_fixture:fixture
{
    receive_pipes pipes;worker::receive_direction direction;
    worker::endpoint_receiver* receiver{};c::handle id{},cap{},host_request{},correlation{};
    std::uint64_t deadline=c::now()+5000000000ull,consumer{},sequence{31},nonce{11};
    std::size_t required{};bytes output;bool accept{true},reenter{};
    explicit adapter_fixture(worker::receive_direction selected=worker::receive_direction::topic,std::optional<std::uint64_t> calls={}):
        fixture(true,true,{},false,false,false,false,selected==worker::receive_direction::service_request?c::service_role::server:c::service_role::client,calls),direction(selected)
    {
        native->receiver_dispatch=&dispatch;
        if(topic())
        {id=sub_id=opened(contract::operation::topic_open,sub_cap,endpoint("sub","subscription"));cap=sub_cap;receiver=native->subscriber->receiver.get();}
        else
        {
            id=service_id=opened(contract::operation::service_open,service_cap,endpoint("srv",selected==worker::receive_direction::service_request?"server":"client"));
            cap=service_cap;receiver=native->client->receiver.get();
            if(selected==worker::receive_direction::service_response)
            {
                host_request=uwvm2::imported::ros::resources::allocate_identity();assert(host_request);
                assert(native->client->correlations->reserve_client({who.instance,who.generation,cap},host_request)==c::error::ok);
                assert(native->client->correlations->submitted({who.instance,who.generation,cap},host_request,
                    {c::error::ok,c::effect::locally_accepted,host_request,71,false},91)==c::error::ok);
            }
        }
        assert(receiver);
    }
    bool topic()const{return direction==worker::receive_direction::topic;}
    auto charged(){return admissions(topic()?"subscriber":"service");}
    auto reserve_action()const{return topic()?receive::operation::topic_reserve:receive::operation::service_reserve;}
    bytes request(){return encode([&](auto& out){return out.u64(id)&&out.u64(deadline);});}
    receive::reference reference()
    {
        w::reader in{output};receive::reference result;std::uint64_t until{},source{},generation{},seq{},loss{};bool present{};
        assert(in.u64(result.endpoint)&&result.endpoint==id&&in.u64(result.provider_generation)&&result.provider_generation==71&&
            in.u64(result.sequence)&&result.sequence==sequence&&in.u64(result.nonce)&&result.nonce==nonce&&in.u64(until)&&until==deadline);
        assert(in.u64(source)&&source==id&&in.u64(generation)&&generation==71&&in.u64(seq)&&seq==sequence&&
            in.boolean(present)&&!present&&in.boolean(present)&&!present&&in.boolean(present)&&!present&&in.u64(loss)&&in.u64(correlation));
        assert(topic()?correlation==0:(direction==worker::receive_direction::service_response?correlation==host_request:correlation&&correlation!=91));
        return result;
    }
    error reserve(std::size_t capacity=4096)
    {
        auto copy=[&](w::bytes body)
        {
            if(reenter)
            {
                std::size_t needed{};auto reject=[](w::bytes){assert(false);return false;};
                assert(dispatch.invoke_receive(who,reserve_action(),cap,request(),4096,needed,reject)==error::again);
                assert(receiver->occupied());
            }
            output.assign(body.begin(),body.end());return accept;
        };
        return dispatch.invoke_receive(who,reserve_action(),cap,request(),capacity,required,copy);
    }
    void receive_reply(bool malformed=false)
    {
        assert(receiver->pump(pipes.transport)==c::error::ok);auto body=pipes.sent(topic()?rpc::operation::topic_reserve:rpc::operation::service_reserve);
        w::reader in{body};std::uint64_t until{};assert(in.u64(consumer)&&consumer&&in.u64(until)&&until==deadline&&in.done());
        std::uint8_t byte=7;c::metadata meta;meta.source=10001;meta.generation=71;meta.sequence=sequence;
        auto record=encode([&](auto& out){return receive::encode(out,{10001,71,sequence,nonce},deadline,meta,topic()?0:91,
            topic()?"topic.message":direction==worker::receive_direction::service_response?"service.response":"service.request",
            {direction==worker::receive_direction::service_response?"test/Response":"test/Request","scene-canonical",{&byte,1}});});
        if(malformed)record.pop_back();
        pipes.reply(c::error::ok,c::effect::none,record);assert(receiver->pump(pipes.transport)==c::error::ok);
    }
    error settle(receive::reference ref,bool ack,std::uint64_t key=1,std::size_t capacity=4096)
    {
        auto input=encode([&](auto& out){return out.u64(id)&&out.u64(key)&&out.u64(ref.endpoint)&&out.u64(ref.provider_generation)&&out.u64(ref.sequence)&&out.u64(ref.nonce);});
        auto operation=topic()?(ack?receive::operation::topic_acknowledge:receive::operation::topic_release):
            (ack?receive::operation::service_acknowledge:receive::operation::service_release);
        auto copy=[&](w::bytes body){output.assign(body.begin(),body.end());return accept;};
        return dispatch.invoke_receive(who,operation,cap,input,capacity,required,copy);
    }
    void settle_send(bool ack,std::uint64_t expected)
    {
        assert(receiver->pump(pipes.transport)==c::error::ok);
        auto body=pipes.sent(topic()?(ack?rpc::operation::topic_acknowledge:rpc::operation::topic_release):
            (ack?rpc::operation::service_acknowledge:rpc::operation::service_release));
        w::reader in{body};std::uint64_t actual{},key{},seq{},borrow{};
        assert(in.u64(actual)&&actual==consumer&&in.u64(key)&&key==expected&&in.u64(seq)&&seq==sequence&&in.u64(borrow)&&borrow==nonce&&in.done());
    }
    void settle_reply(c::error status=c::error::ok,c::effect observed=c::effect::locally_accepted)
    {
        bytes body;if(status==c::error::ok)body=encode([&](auto& out){return contract::encode_receipt(out,{981,71,{},status,observed});});
        pipes.reply(status,observed,body);assert(receiver->pump(pipes.transport)==c::error::ok);
    }
};
static void dispatch_single_accounting()
{
    for(auto direction:{worker::receive_direction::topic,worker::receive_direction::service_request,worker::receive_direction::service_response})
    {
        adapter_fixture f{direction};assert(f.reserve(1)==error::too_large&&f.charged()==1&&!f.receiver->occupied()&&f.pipes.empty());
        assert(f.reserve()==error::again&&f.charged()==2&&f.pipes.empty());
        assert(f.reserve()==error::again&&f.charged()==2);f.receive_reply();
        f.accept=false;f.reenter=true;auto delivered=f.reserve();
        if(delivered!=error::io||f.charged()!=2||!f.receiver->occupied())std::fprintf(stderr,"adapter direction=%u status=%u charges=%lu occupied=%d output=%zu\n",
            static_cast<unsigned>(direction),static_cast<unsigned>(delivered),f.charged(),f.receiver->occupied(),f.output.size());
        assert(delivered==error::io&&f.charged()==2&&f.receiver->occupied());
        auto previous=f.output;auto ref=f.reference();f.accept=true;
        assert(f.reserve()==error::success&&f.output==previous&&f.charged()==2);
        assert(f.settle(ref,true,1,159)==error::too_large&&f.charged()==2);
        assert(f.settle(ref,true)==error::again&&f.charged()==3&&f.pipes.empty());
        assert(f.settle(ref,true)==error::again&&f.charged()==3);f.settle_send(true,1);f.settle_reply();
        f.accept=false;assert(f.settle(ref,true)==error::io&&!f.receiver->occupied()&&f.charged()==3);previous=f.output;
        f.accept=true;assert(f.settle(ref,true)==error::success&&f.output==previous&&f.charged()==3&&f.pipes.empty());
        if(direction==worker::receive_direction::service_response)
        {worker::correlations::view value;assert(f.native->client->correlations->inspect({f.who.instance,f.who.generation,f.cap},f.host_request,value)==c::error::denied);}
        if(direction==worker::receive_direction::service_request)
        {c::handle native{};assert(f.native->client->correlations->response_target({f.who.instance,f.who.generation,f.cap},f.correlation,native)==c::error::ok&&native==91);}
    }
}
static void cleanup_and_known_refusal()
{
    adapter_fixture f;assert(f.reserve()==error::again);f.receive_reply();assert(f.reserve()==error::success);auto ref=f.reference();
    assert(f.settle(ref,true)==error::again);f.settle_send(true,1);f.settle_reply(c::error::timeout,c::effect::none);
    assert(f.settle(ref,true)==error::success&&f.charged()==3&&f.receiver->occupied());auto receipt=f.output;
    assert(f.settle(ref,true)==error::success&&f.output==receipt&&f.charged()==3);
    assert(f.reserve()==error::success&&f.charged()==3); // A failed ACK retains the same admitted borrow.
    assert(f.authority->close(f.who,f.node_cap)==error::success&&f.authority->close(f.who,f.request_cap)==error::success);
    assert(f.settle(ref,false,2)==error::again&&f.charged()==3);f.settle_send(false,1);f.settle_reply();
    assert(f.settle(ref,false,2)==error::success&&!f.receiver->occupied()&&f.charged()==3);
    adapter_fixture exhausted{worker::receive_direction::topic,2};
    assert(exhausted.reserve()==error::again&&exhausted.charged()==2);exhausted.receive_reply();
    assert(exhausted.reserve()==error::success&&exhausted.charged()==2);ref=exhausted.reference();
    assert(exhausted.settle(ref,true)==error::again&&exhausted.charged()==2&&exhausted.pipes.empty());
    assert(exhausted.settle(ref,false)==error::again);exhausted.settle_send(false,1);exhausted.settle_reply();
    assert(exhausted.settle(ref,false)==error::success&&exhausted.charged()==2);
    adapter_fixture zero{worker::receive_direction::topic,1};assert(zero.reserve()==error::again&&!zero.receiver->occupied()&&zero.pipes.empty());
}
static void unknown_and_concurrent()
{
    adapter_fixture f;assert(f.reserve()==error::again);f.receive_reply();assert(f.reserve()==error::success);auto ref=f.reference();
    assert(f.settle(ref,true)==error::again);f.settle_send(true,1);s::communication_jobs::receipt abandoned;
    assert(f.receiver->cancel(f.pipes.transport,abandoned)==c::error::ok&&abandoned.observed==c::effect::unknown);
    assert(f.settle(ref,true)==error::success&&f.receiver->occupied());auto receipt=f.output;
    assert(f.settle(ref,true)==error::success&&f.output==receipt&&f.pipes.empty());
    assert(f.reserve()==error::conflict&&f.dispatch.close(f.who,f.cap,f.id)==error::conflict);
    adapter_fixture concurrent;std::array<std::thread,4> threads;std::array<error,4> status;std::barrier barrier{4};
    for(std::size_t i{};i<threads.size();++i)threads[i]=std::thread([&,i]
    {
        barrier.arrive_and_wait();std::size_t required{};auto reject=[](w::bytes){assert(false);return false;};
        status[i]=concurrent.dispatch.invoke_receive(concurrent.who,concurrent.reserve_action(),concurrent.cap,concurrent.request(),4096,required,reject);
    });
    for(auto& thread:threads)thread.join();
    for(auto result:status)assert(result==error::again);
    assert(concurrent.charged()==2);auto child=::fork();assert(child>=0);
    if(!child){assert(concurrent.reserve()==error::denied);concurrent.receiver->retire();::_exit(0);}
    int result{};assert(::waitpid(child,&result,0)==child&&WIFEXITED(result)&&!WEXITSTATUS(result));
    concurrent.receive_reply();assert(concurrent.reserve()==error::success);
}
int main()
{
    dispatch_single_accounting();cleanup_and_known_refusal();unknown_and_concurrent();
    std::puts("PASS isolated endpoint receive adapter: current tokens through canonical dispatch, one charge across pending IPC/copy retries, native/host source and correlation translation, detached reentrant copies, exact known/unknown receipts, parent cleanup, zero budget, fork and concurrency");
}
