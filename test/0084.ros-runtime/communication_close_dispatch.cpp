/** @file @brief Public close admission after traffic exhaustion and parent revocation. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
namespace rpc=s::communication_worker_protocol;
struct close_dispatch_fixture:fixture
{
    std::array<int,2> input{},output{},diagnostics{};int identity{};s::communication_worker_async::transport transport;
    close_dispatch_fixture():fixture(false,false,{},false,false,false,false,c::service_role::client,1)
    {
        assert(!::pipe2(input.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(output.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({input[1],output[0],diagnostics[0],identity},{128,256,256,32,10000000000,1000000},71)==c::error::ok);
        native->closing_dispatch=&dispatch;assert(dispatch.prepare(native)==error::success);
        context_id=opened(contract::operation::context_open,context_cap,open_profile("ctx"));
        auto parent=encode([&](auto& out){return out.u64(context_cap)&&out.u64(context_id)&&contract::encode_profile(out,profile("node"));});
        node_id=opened(contract::operation::node_open,node_cap,parent);pub_id=opened(contract::operation::topic_open,pub_cap,endpoint("pub","publisher"));
    }
    ~close_dispatch_fixture(){transport.retire();for(auto fd:{input[0],input[1],output[0],output[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    auto& worker(){return *native->publisher->closure;}
    error close(){return dispatch.close(who,pub_cap,pub_id);}
    bool sent()
    {
        std::array<std::uint8_t,512> data;auto size=::read(input[0],data.data(),data.size());
        if(size<0){assert(errno==EAGAIN);return false;}
        rpc::request value;assert(size>0&&rpc::decode_request({data.data(),static_cast<std::size_t>(size)},71,128,value)&&value.action==rpc::operation::resource_close&&value.body.empty());
        return true;
    }
    void reply(std::uint64_t native=9001)
    {
        auto frame=encode([&](auto& out){return out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(1)&&out.u64(0)&&out.u64(0)&&out.u64(8)&&out.u64(native);});
        assert(::write(output[1],frame.data(),frame.size())==static_cast<ssize_t>(frame.size()));
        assert(worker().pump(transport)==c::error::ok);
    }
};
int main()
{
    {
        close_dispatch_fixture f;assert(f.admissions("publisher")==1);
        {auto pin=f.worker().enter();assert(pin&&f.close()==error::again&&!f.worker().occupied()&&!f.sent());}
        for(auto cap:{f.request_cap,f.node_cap,f.context_cap})assert(f.authority->close(f.who,cap)==error::success);
        assert(f.dispatch.close(f.who,f.sub_cap,f.pub_id)!=error::success&&!f.sent());
        assert(f.close()==error::again&&f.worker().occupied()&&!f.worker().enter()&&f.admissions("publisher")==1);
        assert(f.close()==error::again&&!f.sent());
        assert(f.worker().pump(f.transport)==c::error::ok&&f.sent());f.reply();
        assert(f.close()==error::success&&!f.worker().occupied()&&f.admissions("publisher")==1);
        assert(f.close()==error::bad_handle&&!f.sent());
    }
    {
        close_dispatch_fixture f;assert(f.close()==error::again);
        assert(f.authority->close(f.who,f.pub_cap)==error::success);
        assert(f.worker().pump(f.transport)==c::error::ok&&!f.sent()&&!f.worker().cleanup_confirmed());
        assert(f.close()==error::bad_handle);
    }
    {
        close_dispatch_fixture f;assert(f.close()==error::again);
        assert(f.worker().pump(f.transport)==c::error::ok&&f.sent());f.reply(9002);
        assert(f.close()==error::io&&f.worker().cleanup_debt()&&f.worker().occupied()&&!f.worker().enter());
        assert(f.close()==error::io&&!f.sent());
    }
    std::puts("PASS canonical endpoint closure: exhausted traffic, closed schema and parents, exact retained cohort, concurrent-call pin, single cleanup slot, revoked primary and native identity mismatch");
}
