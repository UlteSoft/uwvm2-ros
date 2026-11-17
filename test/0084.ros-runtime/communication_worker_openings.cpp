/** @file @brief Production creation supervisor with exact origins and actual nonblocking pipes. */
#define UWVM_COMMUNICATION_CREATION_CORE_FIXTURE_ONLY
#include "communication_creation.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_openings.h>
#include <barrier>
struct supervised
{
    opening source;s::communication_worker::openings jobs;std::atomic<unsigned> charges{},validations{};
    explicit supervised(std::optional<std::uint64_t> calls={}):source(1,calls)
    {assert(jobs.configure({&source.plan,1},512,validate,charge,this)==error::ok);}
    static error validate(void* opaque,creation::identity const& origin,s::group_lease& group)noexcept
    {
        auto& self=*static_cast<supervised*>(opaque);++self.validations;
        if(origin!=self.source.identity)return error::denied;
        creation::claim_set claims;auto selected=creation::admission_claims(origin,self.source.plan.kind,claims);
        return static_cast<error>(self.source.pipes.rights.authority.admit_group(origin.who,selected,group,false));
    }
    static error charge(void* opaque,creation::identity const& origin,std::uint64_t bytes,s::group_lease& group)noexcept
    {
        auto& self=*static_cast<supervised*>(opaque);++self.charges;
        if(origin!=self.source.identity)return error::denied;
        creation::claim_set claims;auto selected=creation::admission_claims(origin,self.source.plan.kind,claims,bytes);
        return static_cast<error>(self.source.pipes.rights.authority.admit_group(origin.who,selected,group));
    }
    creation::view enqueue()
    {creation::view value;assert(jobs.enqueue(source.identity,source.carrier,value)==error::ok);return value;}
    error pump(){return jobs.pump(source.pipes.transport);}
    creation::view inspect(s::handle request)
    {creation::view value;assert(jobs.inspect(source.identity.who,request,value)==error::ok);return value;}
    void reply(std::uint64_t native=123,unsigned corrupt=0,bool trailing=false)
    {
        std::array<std::uint8_t,9> body;s::wire::writer out{body};assert(out.u64(native));
        source.pipes.response(1,trailing?s::wire::bytes(body):out.written(),corrupt);
    }
};
static void creation_and_publication()
{
    supervised f{1};auto first=f.enqueue();auto duplicate=f.enqueue();auto request=first.receipt.request_id;
    assert(request==duplicate.receipt.request_id&&f.charges==1&&f.source.pipes.drain()==0);
    creation::work origin;s::handle native{};assert(f.jobs.provider_result(request,origin,native)==error::busy);
    assert(f.pump()==error::ok&&f.source.pipes.drain()==48&&f.validations==1);
    assert(f.inspect(request).receipt.status==open_contract::state::waiting);
    assert(f.pump()==error::again&&f.validations==1);f.reply();assert(f.pump()==error::ok);
    assert(f.jobs.provider_result(request,origin,native)==error::ok&&native==123&&origin.original==f.source.identity&&origin.binding->provider_generation==71);
    assert(f.inspect(request).receipt.status==open_contract::state::ready&&f.jobs.release(f.source.identity.who,request)==error::busy);
    auto held=f.source.pipes.rights.admit();open_contract::published result;
    assert(f.jobs.publish(f.source.identity.who,request,held,456,result)==error::ok&&result.handle!=request&&result.handle!=native);
    assert(f.jobs.publish(f.source.identity.who,request,held,456,result)==error::ok&&f.jobs.release(f.source.identity.who,request)==error::busy);held.release();
    assert(f.jobs.cleanup(request,false)==error::busy&&f.jobs.release(f.source.identity.who,request)==error::busy);
    assert(f.jobs.cleanup(request,true)==error::ok&&f.jobs.release(f.source.identity.who,request)==error::ok);
    assert(f.jobs.provider_result(request,origin,native)==error::busy);
}
static void cancellation_and_revocation()
{
    for(bool revoke:{false,true})
    {
        supervised f;auto request=f.enqueue().receipt.request_id;creation::view cancelled;
        if(revoke)assert(f.source.pipes.rights.authority.close(f.source.identity.who,f.source.identity.origins[1].capability)==s::error::success);
        else assert(f.jobs.cancel(f.source.identity.who,request,cancelled)==error::ok);
        auto status=f.pump();assert(status==(revoke?error::bad_handle:error::again)&&f.source.pipes.drain()==0);
        auto current=f.inspect(request);assert(current.receipt.status==open_contract::state::cancelled&&!current.receipt.cleanup_debt&&
            current.receipt.observed==s::communication::effect::none&&f.jobs.release(f.source.identity.who,request)==error::ok);
    }
    supervised blocked;auto request=blocked.enqueue().receipt.request_id;std::array<char,4096> bytes{};std::size_t filled{};
    for(;;){auto count=::write(blocked.source.pipes.input[1],bytes.data(),bytes.size());if(count<0){assert(errno==EAGAIN);break;}filled+=count;}
    assert(blocked.pump()==error::again&&blocked.source.pipes.transport.inspect().sent==0);
    creation::view cancelled;assert(blocked.jobs.cancel(blocked.source.identity.who,request,cancelled)==error::ok&&!cancelled.receipt.cleanup_debt);
    assert(blocked.pump()==error::ok&&blocked.source.pipes.drain()==filled&&blocked.source.pipes.transport.inspect().state==a::phase::idle);
    assert(blocked.jobs.release(blocked.source.identity.who,request)==error::ok);
    supervised sent;request=sent.enqueue().receipt.request_id;assert(sent.pump()==error::ok&&sent.source.pipes.drain()==48);
    assert(sent.jobs.cancel(sent.source.identity.who,request,cancelled)==error::ok&&cancelled.receipt.cleanup_debt&&cancelled.receipt.observed==s::communication::effect::unknown);
    assert(sent.pump()==error::cancelled&&sent.jobs.cleanup(request,true)==error::ok&&sent.jobs.release(sent.source.identity.who,request)==error::busy);
}
static void malformed_and_incarnation()
{
    for(unsigned fault{};fault<3;++fault)
    {
        supervised f;auto request=f.enqueue().receipt.request_id;assert(f.pump()==error::ok&&f.source.pipes.drain()==48);
        f.reply(fault==0?0:123,fault==1?1:0,fault==2);auto status=f.pump();assert(status==error::ok||status==error::io);
        auto current=f.inspect(request);assert(current.receipt.status==open_contract::state::failed&&current.receipt.result==error::io&&
            current.receipt.observed==s::communication::effect::unknown&&current.receipt.cleanup_debt);
        assert(f.jobs.release(f.source.identity.who,request)==error::busy&&f.jobs.cleanup(request,true)==error::ok&&f.jobs.release(f.source.identity.who,request)==error::busy);
        creation::work origin;s::handle native{};assert(f.jobs.provider_result(request,origin,native)==error::busy);
    }
    supervised f;auto request=f.enqueue().receipt.request_id;a::transport wrong;
    auto& pipes=f.source.pipes;
    assert(wrong.prepare({pipes.input[1],pipes.output[0],pipes.diagnostics[0],pipes.identity},{256,256,256,32,1000000000,1000000},72)==error::ok);
    assert(f.jobs.pump(wrong)==error::denied&&pipes.drain()==0&&!f.inspect(request).receipt.cleanup_debt&&wrong.inspect().state==a::phase::idle);
    assert(f.jobs.release(f.source.identity.who,request)==error::ok);
}
static void concurrency_and_fork()
{
    supervised f{1};std::array<std::thread,6> threads;std::array<error,6> statuses;std::array<creation::view,6> answers;std::barrier start{6};
    for(std::size_t i{};i<threads.size();++i)threads[i]=std::thread([&,i]{start.arrive_and_wait();statuses[i]=f.jobs.enqueue(f.source.identity,f.source.carrier,answers[i]);});
    for(auto& thread:threads)thread.join();
    auto request=f.enqueue().receipt.request_id;assert(f.charges==1&&f.source.pipes.drain()==0);
    for(std::size_t i{};i<statuses.size();++i)assert(statuses[i]==error::again||(statuses[i]==error::ok&&answers[i].receipt.request_id==request));
    auto child=::fork();assert(child>=0);
    if(!child){assert(f.pump()==error::denied);f.jobs.retire();::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    assert(f.pump()==error::ok);f.jobs.request_stop();assert(f.pump()==error::cancelled);
    assert(f.jobs.cleanup(request,true)==error::ok&&f.jobs.release(f.source.identity.who,request)==error::busy);
    supervised zero{0};creation::view rejected;assert(zero.jobs.enqueue(zero.source.identity,zero.source.carrier,rejected)==error::again&&zero.pump()==error::again&&zero.source.pipes.drain()==0);
}
int main()
{
    creation_and_publication();cancellation_and_revocation();malformed_and_incarnation();concurrency_and_fork();
    std::puts("PASS owned endpoint creation supervisor: exact admitted retry, nonblocking send/receive, separately verified native/host identities, zero traffic, actual parent revocation, no-byte cancellation, written-byte unknown debt, live-resource release refusal, malformed replies, incarnation, concurrency and fork");
}
