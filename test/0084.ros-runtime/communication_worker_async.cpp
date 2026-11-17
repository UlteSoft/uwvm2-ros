/** @file @brief Nonblocking RPC transport tests; pipe fixture, not native ROS qualification. */
#include <uwvm2/imported/ros/scene/communication_worker_async.h>
#include <cassert>
#include <cstdio>
#include <sys/syscall.h>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace a=s::communication_worker_async;
namespace rpc=s::communication_worker_protocol;
using error=s::communication::error;
#if defined(UWVM_RPC_READ_FENCE)
static int fenced_read_fd{-1};
static std::atomic<bool>* fenced_read_stop{};
extern "C" ssize_t __real_read(int,void*,size_t);
extern "C" ssize_t __wrap_read(int fd,void* bytes,size_t size)
{
    auto result=__real_read(fd,bytes,size);
    if(fd==fenced_read_fd&&result>0&&fenced_read_stop)fenced_read_stop->store(true,std::memory_order_release);
    return result;
}
#endif
struct permission
{
    s::authority authority;int instance{};s::caller who{&instance,1};std::array<s::admission_claim,4> claims;
    explicit permission(std::optional<std::uint64_t> calls={})
    {
        for(auto action:{"capability.open","capability.close","topic.open","topic.publish","node.open","context.open","type.describe"})assert(authority.expose(action,s::exposure::allow)==s::error::success);
        std::array<std::string_view,4> aliases{"topic","node","context","type"},actions{"topic.open","node.open","context.open","type.describe"};
        std::array<s::purpose,4> kinds{s::purpose::topic,s::purpose::node,s::purpose::context,s::purpose::type};
        for(std::size_t i{};i<4;++i){s::grant grant;grant.alias=aliases[i];grant.resource="closed.fixture";grant.binding="private-worker";grant.kind=kinds[i];grant.actions.emplace(actions[i]);
            if(i==0){grant.actions.emplace("topic.publish");grant.limits.calls=calls;grant.limits.uncertain=calls.has_value();grant.limits.window=std::chrono::hours{1};}
            assert(authority.configure(std::move(grant))==s::error::success);}
        assert(authority.enroll(who)==s::error::success);
        for(std::size_t i{};i<4;++i){s::handle cap{};assert(authority.open(who,aliases[i],cap)==s::error::success);claims[i]={cap,actions[i],0,false,i!=0};}
    }
    s::group_lease admit(bool charged=false)
    {s::group_lease held;assert(authority.admit_group(who,claims,held,charged)==s::error::success);return held;}
};
struct fixture
{
    std::array<int,2> input{},output{},diagnostics{};int identity{};std::atomic<bool> stopped{};a::transport transport;permission rights;
    explicit fixture(std::uint64_t limit=16384,std::optional<std::uint64_t> calls={},std::uint64_t expiry=UINT64_MAX):rights(calls)
    {
        assert(!::pipe2(input.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(output.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({input[1],output[0],diagnostics[0],identity,expiry,&stopped},{limit,256,256,32,1000000000,1000000},71)==error::ok);
    }
    ~fixture(){transport.retire();for(auto fd:{input[0],input[1],output[0],output[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    a::ticket stage(s::wire::bytes body={})
    {a::ticket id;assert(transport.stage(rpc::operation::topic_open,1,body,s::communication::now()+900000000,id)==error::ok);return id;}
    void response(std::uint64_t sequence,s::wire::bytes body={},unsigned corrupt=0)
    {
        std::array<std::uint8_t,304> data{};s::wire::writer writer{data};
        assert(writer.u64(rpc::reply_magic)&&writer.u64(71)&&writer.u64(sequence)&&writer.u64(0)&&writer.u64(1)&&writer.u64(body.size())&&writer.encoded(body));
        if(corrupt==1)data[8]^=1;
        if(corrupt==2)data[40]=1,data[41]=1;
        if(corrupt==3)data[24]=255;
        assert(::write(output[1],data.data(),writer.size())==static_cast<ssize_t>(writer.size()));
    }
    std::size_t drain()
    {std::array<std::uint8_t,4096> bytes;std::size_t total{};for(;;){auto got=::read(input[0],bytes.data(),bytes.size());if(got<0){assert(errno==EAGAIN);return total;}assert(got);total+=static_cast<std::size_t>(got);}}
};
int main()
{
#if defined(UWVM_RPC_READ_FENCE)
    {
        fixture f;auto id=f.stage();auto held=f.rights.admit();
        assert(f.transport.send(id,held)==error::ok);held.release();
        std::array<std::uint8_t,3> body{7,8,9};f.response(1,body);
        fenced_read_fd=f.output[0];fenced_read_stop=&f.stopped;
        // Revocation after the header read must stop the next body read in
        // this SAME receive() call, preserving an unknown effect receipt.
        assert(f.transport.receive(id)==error::cancelled);
        assert(f.transport.inspect().receipt.observed==s::communication::effect::unknown);
        fenced_read_fd=-1;fenced_read_stop=nullptr;
        std::array<std::uint8_t,3> remaining{};
        assert(::read(f.output[0],remaining.data(),remaining.size())==3&&remaining==body);
    }
#endif
    {
        fixture f;auto id=f.stage();s::group_lease empty;
        assert(f.transport.send(id,empty)==error::denied&&f.transport.inspect().sent==0);
        auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::ok);held.release();assert(f.drain()==48);
        std::array<std::uint8_t,3> body{4,5,6};f.response(1,body);assert(f.transport.receive(id)==error::ok);
        a::result result;std::array<std::uint8_t,2> short_copy;assert(f.transport.copy(id,short_copy,result)==error::too_large);
        std::array<std::uint8_t,256> output;assert(f.transport.copy(id,output,result)==error::ok&&result.status==error::ok&&result.observed==s::communication::effect::locally_accepted&&result.bytes==3&&std::equal(body.begin(),body.end(),output.begin()));
        assert(f.transport.copy(id,output,result)==error::ok&&f.transport.acknowledge(id)==error::ok);
        auto next=f.stage();assert(next.request==id.request+1&&f.transport.copy(id,output,result)==error::stale);
        held=f.rights.admit();assert(f.transport.send(next,held)==error::ok);held.release();f.response(2);assert(f.transport.receive(next)==error::ok);
        f.transport.request_stop();assert(f.transport.copy(next,output,result)==error::ok&&result.wire_sequence==2&&result.status==error::ok);
        assert(f.transport.acknowledge(next)==error::ok&&f.transport.stage(rpc::operation::topic_open,1,{},s::communication::now()+100000000,next)==error::denied);
    }
    {
        fixture f;std::array<std::uint8_t,4096> bytes{};std::size_t filled{};
        for(;;){auto n=::write(f.input[1],bytes.data(),bytes.size());if(n<0){assert(errno==EAGAIN);break;}filled+=static_cast<std::size_t>(n);}
        auto id=f.stage();auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::again&&f.transport.inspect().sent==0);held.release();
        assert(f.drain()==filled);held=f.rights.admit();assert(f.transport.send(id,held)==error::ok);held.release();f.response(1);assert(f.transport.receive(id)==error::ok);
    }
    {
        fixture f;assert(::fcntl(f.input[1],F_SETPIPE_SZ,4096)==4096);std::array<std::uint8_t,8192> body{};auto id=f.stage(body);auto held=f.rights.admit(true);
        assert(f.transport.send(id,held)==error::again);held.release();assert(f.transport.inspect().state==a::phase::sending&&f.transport.inspect().sent>0);
        f.transport.request_stop();held=f.rights.admit();assert(f.transport.send(id,held)==error::cancelled);held.release();
        a::result result;assert(f.transport.copy(id,{},result)==error::ok&&result.observed==s::communication::effect::unknown&&result.status==error::cancelled);
        assert(f.transport.acknowledge(id)==error::cancelled);
    }
    for(unsigned corruption:{1u,2u,3u})
    {
        fixture f;auto id=f.stage();auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::ok);held.release();f.response(1,{},corruption);
        assert(f.transport.receive(id)==error::io&&f.transport.inspect().receipt.observed==s::communication::effect::unknown);
    }
    {
        fixture f;auto id=f.stage();std::array<char,33> excess{};assert(::write(f.diagnostics[1],excess.data(),excess.size())==33);
        auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::too_large&&f.transport.inspect().receipt.observed==s::communication::effect::none);
    }
    {
        fixture f;auto id=f.stage();auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::ok);held.release();
        auto child=::fork();assert(child>=0);if(!child){assert(f.transport.receive(id)==error::denied);::_exit(0);}int status{};
        assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));f.transport.retire();assert(f.transport.inspect().receipt.observed==s::communication::effect::unknown);
    }
    {
        fixture f;auto id=f.stage();auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::ok);held.release();
        struct task{a::transport* transport;a::ticket id;std::atomic<bool> entered{};error result{error::again};} work{&f.transport,id};pthread_t thread;
        assert(!::pthread_create(&thread,nullptr,[](void* opaque)->void*
        {auto& value=*static_cast<task*>(opaque);value.entered=true;while((value.result=value.transport->receive(value.id))==error::again)::poll(nullptr,0,1);return nullptr;},&work));
        while(!work.entered.load())::poll(nullptr,0,1);
        f.transport.request_stop();assert(!::pthread_join(thread,nullptr));assert(work.result==error::cancelled&&f.transport.inspect().receipt.observed==s::communication::effect::unknown);
    }
    {
        fixture f;auto id=f.stage();f.stopped=true;
        auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::cancelled);held.release();
        assert(f.drain()==0&&f.transport.inspect().receipt.observed==s::communication::effect::none);
    }
    {
        fixture f;auto id=f.stage();auto held=f.rights.admit(true);assert(f.transport.send(id,held)==error::ok);held.release();
        f.stopped=true;f.response(1);assert(f.transport.receive(id)==error::cancelled);
        assert(f.transport.inspect().receipt.observed==s::communication::effect::unknown);
    }
    {
        fixture f{16384,{},s::communication::now()-1};a::ticket id;
        assert(f.transport.stage(rpc::operation::topic_open,1,{},s::communication::now()+1000000,id)==error::denied&&f.drain()==0);
    }
    std::puts("PASS nonblocking worker RPC: full-group send, full-pipe retry, partial-write unknown, exact receipt retention, stale ticket, malformed reply, diagnostic bound, fork and fence");
    return 0;
}
