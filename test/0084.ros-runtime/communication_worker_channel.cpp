/** @file @brief Private worker channel boundaries, retained reply capacity and unknown outcomes. */
#include <uwvm2/imported/ros/scene/communication_worker_channel.h>
#include <cassert>
#include <cstdio>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace ch=s::communication_worker_channel;
namespace rpc=s::communication_worker_protocol;
using error=ch::error;
struct fixture
{
    std::array<int,2> input{},output{},diagnostics{};ch::channel channel;
    pthread_t thread{};std::atomic<bool> stop{};std::atomic<unsigned> effects{};unsigned mode{};bool started{};
    explicit fixture(unsigned selected=0,std::uint64_t stderr_limit=32):mode(selected)
    {
        assert(!::pipe2(input.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(output.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        assert(channel.prepare({input[1],output[0],diagnostics[0],-1},{256,256,512,stderr_limit,100000000,1000000},71)==error::ok);
        assert(!::pthread_create(&thread,nullptr,[](void* p)->void*{static_cast<fixture*>(p)->serve();return nullptr;},this));started=true;
    }
    bool read(std::span<std::uint8_t> bytes)
    {
        std::size_t offset{};
        while(offset<bytes.size()&&!stop.load())
        {auto got=::read(input[0],bytes.data()+offset,bytes.size()-offset);if(got>0)offset+=static_cast<std::size_t>(got);else if(!got)return false;else if(errno!=EAGAIN&&errno!=EINTR)return false;else ::poll(nullptr,0,1);}
        return offset==bytes.size();
    }
    void serve()
    {
        rpc::ledger ledger;assert(ledger.prepare(71,256,256));
        while(!stop.load())
        {
            std::array<std::uint8_t,304> input_bytes{};if(!read(std::span{input_bytes}.first(rpc::header_bytes)))return;std::uint64_t extent{};
            assert(rpc::request_extent(std::span{input_bytes}.first(rpc::header_bytes),71,256,extent));if(!read(std::span{input_bytes}.subspan(rpc::header_bytes,extent)))return;
            ++effects;if(mode==4){while(!stop.load())::poll(nullptr,0,1);return;}
            if(mode==5){char text[33]{};assert(::write(diagnostics[1],text,sizeof(text))==sizeof(text));}
            s::wire::bytes reply;assert(ledger.execute({input_bytes.data(),rpc::header_bytes+extent},[](rpc::request const& request,std::span<std::uint8_t> out)
            {std::copy(request.body.begin(),request.body.end(),out.begin());return rpc::result{error::ok,ch::effect::locally_accepted,request.body.size()};},reply));
            std::array<std::uint8_t,304> bytes{};std::copy(reply.begin(),reply.end(),bytes.begin());
            if(mode==1)bytes[8]^=1;
            if(mode==2)bytes[40]=1,bytes[41]=1;
            if(mode==3){::close(output[1]);output[1]=-1;return;}
            for(std::size_t i{};i<reply.size();++i){auto wrote=::write(output[1],bytes.data()+i,1);assert(wrote==1);}
        }
    }
    ~fixture()
    {stop=true;if(started)::pthread_join(thread,nullptr);for(auto fd:{input[0],input[1],output[0],output[1],diagnostics[0],diagnostics[1]})if(fd>=0)::close(fd);}
};
int main()
{
    {
        fixture f;std::array<std::uint8_t,3> bytes{4,5,6};ch::reply first,second,third;
        assert(f.channel.exchange(rpc::operation::topic_publish,1,bytes,first)==error::ok&&first.status==error::ok&&first.observed==ch::effect::locally_accepted&&first.sequence==1&&std::equal(bytes.begin(),bytes.end(),first.bytes().begin()));
        assert(f.channel.exchange(rpc::operation::topic_take,1,{},second)==error::ok&&second.sequence==2&&second.bytes().empty());
        assert(f.channel.retained_bytes()==512&&f.channel.exchange(rpc::operation::topic_take,1,{},third)==error::too_large&&f.effects==2);
        first=ch::reply{};assert(f.channel.retained_bytes()==256);
        assert(f.channel.exchange(rpc::operation::node_inspect,1,{},third)==error::ok&&third.sequence==3);
        auto child=::fork();assert(child>=0);if(!child){ch::reply denied;assert(f.channel.exchange(rpc::operation::node_inspect,1,{},denied)==error::denied);::_exit(0);}
        int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    }
    for(unsigned mode:{1u,2u,3u,4u,5u})
    {
        fixture f(mode);ch::reply result;
        assert(f.channel.exchange(rpc::operation::node_inspect,1,{},result)!=error::ok&&result.observed==ch::effect::unknown&&!f.channel.live());
        assert(f.channel.exchange(rpc::operation::node_inspect,1,{},result)==error::cancelled&&f.effects==1);
    }
    {
        fixture f(4);ch::reply result;pthread_t revoker;assert(!::pthread_create(&revoker,nullptr,[](void* p)->void*{auto& f=*static_cast<fixture*>(p);while(!f.effects.load())::poll(nullptr,0,1);f.channel.fence();return nullptr;},&f));
        assert(f.channel.exchange(rpc::operation::topic_publish,1,{},result)==error::cancelled&&result.observed==ch::effect::unknown);::pthread_join(revoker,nullptr);
    }
    {
        int commands[2],responses[2],diagnostics[2];assert(!::pipe2(commands,O_CLOEXEC|O_NONBLOCK)&&!::pipe2(responses,O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics,O_CLOEXEC|O_NONBLOCK));
        ch::channel channel;assert(channel.prepare({commands[1],responses[0],diagnostics[0],-1},{256,256,256,0,100000000,1000000},1)==error::ok);::close(commands[0]);
        ch::reply out;assert(channel.exchange(rpc::operation::node_inspect,1,{},out)==error::io&&out.observed==ch::effect::none);
        for(auto fd:{commands[1],responses[0],responses[1],diagnostics[0],diagnostics[1]})::close(fd);
    }
    {
        sigset_t mask,old,pending;::sigemptyset(&mask);::sigaddset(&mask,SIGPIPE);assert(!::pthread_sigmask(SIG_BLOCK,&mask,&old));
        {ch::broken_pipe_guard guard;assert(guard.valid());assert(!::pthread_kill(::pthread_self(),SIGPIPE));}
        assert(!::sigpending(&pending)&&::sigismember(&pending,SIGPIPE)==1);timespec zero{};assert(::sigtimedwait(&mask,nullptr,&zero)==SIGPIPE);assert(!::pthread_sigmask(SIG_SETMASK,&old,nullptr));
    }
    std::puts("passed bounded worker RPC channel: detached capacity, invalid generation/extent, diagnostics, timeout, revoke, SIGPIPE and fork");
}
