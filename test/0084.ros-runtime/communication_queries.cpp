/** @file @brief Query authority, actual nonblocking IPC and recoverable result ownership. */
#define UWVM_COMMUNICATION_GRAPH_FIXTURE_ONLY
#include "communication_graph_dispatch.cpp"
#include <uwvm2/imported/ros/scene/communication_queries.h>
namespace rpc=s::communication_worker_protocol;
using query=s::communication_worker::queries;
struct query_pipes
{
    std::array<int,2> incoming{},outgoing{},diagnostics{};int identity{};
    s::communication_worker_async::transport transport;
    explicit query_pipes(std::uint64_t reply_bytes=256)
    {
        assert(!::pipe2(incoming.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(outgoing.data(),O_CLOEXEC|O_NONBLOCK)&&!::pipe2(diagnostics.data(),O_CLOEXEC|O_NONBLOCK));
        identity=static_cast<int>(::syscall(SYS_pidfd_open,::getpid(),0));assert(identity>=0);
        assert(transport.prepare({incoming[1],outgoing[0],diagnostics[0],identity},{128,reply_bytes,reply_bytes,32,10000000000,1000000},71)==c::error::ok);
    }
    ~query_pipes(){transport.retire();for(auto fd:{incoming[0],incoming[1],outgoing[0],outgoing[1],diagnostics[0],diagnostics[1],identity})::close(fd);}
    std::size_t sent()
    {
        std::array<std::uint8_t,512> storage;auto count=::read(incoming[0],storage.data(),storage.size());
        if(count<0){assert(errno==EAGAIN);return 0;}
        assert(count>0);rpc::request parsed;assert(rpc::decode_request({storage.data(),static_cast<std::size_t>(count)},71,128,parsed)&&parsed.action==rpc::operation::graph_capture);
        return static_cast<std::size_t>(count);
    }
    void response(c::error status=c::error::ok,c::effect observed=c::effect::none,bool malformed=false)
    {
        auto body=encode([&](auto& out){return out.u64(51);});if(malformed)body.pop_back();
        auto frame=encode([&](auto& out){return out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(1)&&out.u64(static_cast<std::uint64_t>(status))&&
            out.u64(static_cast<std::uint64_t>(observed))&&out.u64(body.size())&&out.encoded(body);});
        assert(::write(outgoing[1],frame.data(),frame.size())==static_cast<ssize_t>(frame.size()));
    }
};
struct query_fixture
{
    graph_fixture source;
    query_pipes pipes;
    query worker;
    c::operation_token token;
    std::uint64_t deadline=c::now()+5000000000ULL,value{};
    unsigned decodes{};bool fail_allocation{},reentrant{};
    explicit query_fixture(std::optional<std::uint64_t> calls={}):source(true,calls)
    {
        bytes ignored;assert(source.call(gc::operation::snapshot,ignored)==error::success);token=source.native->last;
        s::communication_jobs::plan plan{token.authority,c::operation_family::graph,token.resource,1,71,1,128,256,10000000000,768};
        auto validate=[](void* object,c::operation_token const& value,s::group_lease& held)noexcept
        {return static_cast<c::error>(static_cast<query_fixture*>(object)->source.dispatch.revalidate(value,held));};
        auto charge=[](void* object,c::operation_token const& value,std::uint64_t bytes,s::group_lease& held)noexcept
        {
            auto& self=*static_cast<query_fixture*>(object);
            if(self.reentrant)assert(self.call()==c::error::again);
            return static_cast<c::error>(self.source.dispatch.charge_operation(value,bytes,held));
        };
        assert(worker.configure(plan,validate,charge,this)==c::error::ok);
    }
    c::error call(c::operation_token const* replacement=nullptr)
    {
        return worker.graph_capture(replacement?*replacement:token,deadline,[&](w::bytes body)
        {
            ++decodes;if(fail_allocation)throw std::bad_alloc{};
            w::reader input{body};std::uint64_t candidate{};if(!input.u64(candidate)||!input.done())return c::error::invalid;
            value=candidate;return c::error::ok;
        });
    }
    auto charged(){return source.origin.admissions("graph");}
    void complete(c::error status=c::error::ok,c::effect effect=c::effect::none,bool malformed=false)
    {
        assert(worker.pump(pipes.transport)==c::error::ok&&pipes.sent()==rpc::header_bytes);
        pipes.response(status,effect,malformed);assert(worker.pump(pipes.transport)==c::error::ok);
    }
};
#ifndef UWVM_COMMUNICATION_QUERY_FIXTURE_ONLY
static void query_charge_and_recovery()
{
    query_fixture f{2};f.reentrant=true;assert(f.charged()==1&&f.call()==c::error::again&&f.charged()==2&&!f.pipes.sent()&&!f.decodes);
    assert(f.call()==c::error::again&&f.charged()==2);f.complete();
    f.fail_allocation=true;assert(f.call()==c::error::no_memory&&f.charged()==2&&f.worker.occupied()&&!f.value);
    f.fail_allocation=false;assert(f.call()==c::error::ok&&f.value==51&&!f.worker.occupied()&&f.charged()==2&&f.decodes==2);
    assert(f.call()==c::error::again&&f.charged()==2&&!f.worker.occupied()&&!f.pipes.sent());
    query_fixture zero{1};assert(zero.call()==c::error::again&&!zero.worker.occupied()&&zero.charged()==1&&!zero.pipes.sent());
}
static void query_original_authority()
{
    query_fixture f;assert(f.call()==c::error::again);auto different=f.token;
    ++different.caller_generation;assert(f.call(&different)==c::error::busy);
    different=f.token;assert(f.source.origin.authority->open(f.source.origin.who,"graph",different.capability)==error::success);
    different.origins[0].capability=different.capability;assert(f.call(&different)==c::error::busy);
    f.complete();assert(f.source.origin.authority->close(f.source.origin.who,f.token.origins[2].capability)==error::success);
    assert(f.call()==c::error::bad_handle&&!f.value&&!f.decodes&&f.worker.occupied());
    query_fixture before;assert(before.call()==c::error::again);
    assert(before.source.origin.authority->close(before.source.origin.who,before.token.capability)==error::success);
    assert(before.worker.pump(before.pipes.transport)==c::error::ok&&!before.pipes.sent());
    assert(before.call()==c::error::bad_handle&&!before.decodes);
}
static void query_unknown_and_cancellation()
{
    for(bool sent:{false,true})
    {
        query_fixture f;assert(f.call()==c::error::again);
        if(sent)assert(f.worker.pump(f.pipes.transport)==c::error::ok&&f.pipes.sent()==rpc::header_bytes);
        s::communication_jobs::receipt result;assert(f.worker.cancel(f.pipes.transport,result)==c::error::ok&&result.observed==(sent?c::effect::unknown:c::effect::none));
        assert(f.call()==c::error::cancelled&&!f.decodes&&f.worker.occupied()==sent);
    }
    query_fixture unknown;assert(unknown.call()==c::error::again);unknown.complete(c::error::ok,c::effect::unknown);
    assert(unknown.call()==c::error::io&&unknown.worker.occupied()&&!unknown.decodes);
    assert(unknown.call()==c::error::cancelled);
    query_fixture malformed;assert(malformed.call()==c::error::again);malformed.complete(c::error::ok,c::effect::none,true);
    assert(malformed.call()==c::error::invalid&&malformed.worker.occupied()&&!malformed.value&&malformed.decodes==1);
    assert(malformed.call()==c::error::cancelled);
    query_fixture failure;assert(failure.call()==c::error::again);failure.complete(c::error::denied);
    assert(failure.call()==c::error::denied&&!failure.worker.occupied()&&!failure.decodes);
}
static void query_concurrency_and_fork()
{
    query_fixture f;std::array<c::error,8> outcomes;std::array<std::thread,8> threads;std::barrier start{8};
    for(std::size_t i{};i<threads.size();++i)threads[i]=std::thread([&,i]{start.arrive_and_wait();outcomes[i]=f.call();});
    for(auto& thread:threads)thread.join();
    for(auto result:outcomes)assert(result==c::error::again);
    assert(f.charged()==2&&f.worker.occupied()&&!f.decodes);
    auto pid=::fork();assert(pid>=0);if(!pid){assert(f.call()==c::error::denied);f.worker.retire();::_exit(0);}
    int result{};assert(::waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&!WEXITSTATUS(result));
    f.complete();assert(f.call()==c::error::ok&&f.value==51&&!f.worker.occupied());
}
int main()
{
    query_charge_and_recovery();query_original_authority();query_unknown_and_cancellation();query_concurrency_and_fork();
    std::puts("PASS owned communication queries: current graph origins, single charge, nonblocking pipes, decoder retry, exact caller, concurrent submission, revoke, unknown result and fork fencing");
}
#endif
