/** @file @brief Canonical observation output preflight and isolated single-charge accounting. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
namespace rpc=s::communication_worker_protocol;
struct query_dispatch_fixture:fixture
{
    std::array<int,2> input{},output{},diagnostics{};int identity{};s::communication_worker_async::transport transport;
    explicit query_dispatch_fixture(std::uint64_t calls):fixture(false,false,{},false,false,false,false,c::service_role::client,calls)
    {
        assert(!::pipe2(input.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(output.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({input[1],output[0],diagnostics[0],identity},{128,256,256,32,10000000000,1000000},71)==c::error::ok);
        native->query_dispatch=&dispatch;assert(dispatch.prepare(native)==error::success);
        context_id=opened(contract::operation::context_open,context_cap,open_profile("ctx"));
        auto parent=encode([&](auto& out){return out.u64(context_cap)&&out.u64(context_id)&&contract::encode_profile(out,profile("node"));});
        node_id=opened(contract::operation::node_open,node_cap,parent);pub_id=opened(contract::operation::topic_open,pub_cap,endpoint("pub","publisher"));
    }
    ~query_dispatch_fixture(){transport.retire();for(auto fd:{input[0],input[1],output[0],output[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    auto& worker(){return *native->publisher->observed;}
    std::size_t sent()
    {
        std::array<std::uint8_t,512> data;auto size=::read(input[0],data.data(),data.size());
        if(size<0){assert(errno==EAGAIN);return 0;}
        rpc::request value;assert(size>0&&rpc::decode_request({data.data(),static_cast<std::size_t>(size)},71,128,value)&&value.action==rpc::operation::topic_qos&&value.body.empty());
        return static_cast<std::size_t>(size);
    }
    void complete()
    {
        assert(worker().pump(transport)==c::error::ok&&sent()>0);
        auto frame=encode([&](auto& out){return out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(1)&&out.u64(0)&&out.u64(0)&&out.u64(8)&&out.u64(3);});
        assert(::write(output[1],frame.data(),frame.size())==static_cast<ssize_t>(frame.size()));
        assert(worker().pump(transport)==c::error::ok);
    }
};
int main()
{
    query_dispatch_fixture f{2};bytes result;auto input=resource(f.pub_id);
    assert(f.admissions("publisher")==1);
    assert(f.call(contract::operation::qos_describe,f.pub_cap,input,result,1)==error::too_large&&!f.worker().occupied()&&!f.sent()&&f.admissions("publisher")==1);
    assert(f.call(contract::operation::qos_describe,f.pub_cap,input,result)==error::again&&f.worker().occupied()&&f.admissions("publisher")==2);
    assert(f.call(contract::operation::qos_describe,f.pub_cap,input,result)==error::again&&f.admissions("publisher")==2);
    f.complete();
    assert(f.call(contract::operation::qos_describe,f.pub_cap,input,result,1)==error::too_large&&f.worker().occupied()&&f.admissions("publisher")==2);
    assert(f.call(contract::operation::qos_describe,f.pub_cap,input,result)==error::success&&!f.worker().occupied()&&f.admissions("publisher")==2);
    assert(schema::validate(result,schema::find("qos.describe")->result,{4096,100,20,10}));
    assert(f.call(contract::operation::qos_describe,f.pub_cap,input,result)==error::again&&!f.worker().occupied()&&!f.sent()&&f.admissions("publisher")==2);
    query_dispatch_fixture zero{1};assert(zero.call(contract::operation::qos_describe,zero.pub_cap,resource(zero.pub_id),result)==error::again&&!zero.worker().occupied()&&!zero.sent());
    query_dispatch_fixture revoked{2};
    assert(revoked.call(contract::operation::qos_describe,revoked.pub_cap,resource(revoked.pub_id),result)==error::again);
    assert(revoked.authority->close(revoked.who,revoked.request_cap)==error::success);
    assert(revoked.worker().pump(revoked.transport)==c::error::ok&&!revoked.sent());
    assert(revoked.call(contract::operation::qos_describe,revoked.pub_cap,resource(revoked.pub_id),result)==error::bad_handle);
    std::puts("PASS canonical queued observations: output-before-RPC, one charge across pending polls, exhausted-budget completion, zero new quota and current schema authority");
}
