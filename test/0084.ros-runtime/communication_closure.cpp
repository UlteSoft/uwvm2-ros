/** @file @brief Exact isolated endpoint destruction and concurrent-call exclusion. */
#define main communication_async_fixture_entry
#include "communication_worker_async.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_worker_closure.h>
#include <thread>
#include <barrier>
namespace c=s::communication;
namespace jobs=s::communication_jobs;
using closure=s::communication_worker::endpoint_closure;
struct closing_fixture
{
    fixture pipes;closure closing;s::authority authority;int instance{};s::caller who{&instance,1};c::operation_token token;
    std::array<s::handle,4> caps{};unsigned charges{};bool quiet{true};
    closing_fixture()
    {
        for(auto name:{"capability.open","capability.close","resource.close","topic.publish","node.open","context.open","type.describe"})assert(authority.expose(name,s::exposure::allow)==s::error::success);
        constexpr std::array aliases{"endpoint","node","context","schema"},actions{"resource.close","node.open","context.open","type.describe"};
        constexpr std::array kinds{s::purpose::topic,s::purpose::node,s::purpose::context,s::purpose::type};
        for(std::size_t i{};i<4;++i)
        {
            s::grant value;value.alias=aliases[i];value.resource="closed.fixture";value.binding="native-communication";value.kind=kinds[i];value.actions.insert(actions[i]);
            if(!i){value.actions.insert("topic.publish");value.limits.calls=0;value.limits.bytes=0;value.limits.uncertain=true;value.limits.window=std::chrono::hours{1};}
            assert(authority.configure(std::move(value))==s::error::success);
        }
        assert(authority.enroll(who)==s::error::success);
        for(std::size_t i{};i<4;++i)assert(authority.open(who,aliases[i],caps[i])==s::error::success);
        token.caller_instance=who.instance;token.caller_generation=who.generation;token.authority={17,1,1};token.capability=caps[0];token.resource=41;
        token.family=c::operation_family::topic;token.action=c::permission::resource_close;token.count=4;token.retained_origins=uwvm2::utils::container::make_shared<int>(1);
        for(std::size_t i{};i<4;++i)token.origins[i]={{17,i+1,1},caps[i],i==0?41u:i==1?51u:i==2?61u:0u,
            i==0?c::permission::resource_close:i==1?c::permission::node_open:i==2?c::permission::context_open:c::permission::type_describe};
        assert(closing.configure({token.authority,token.family,41,1,71,1,1,8,1000000000,closure::reserved_bytes},9001,admit,charge,this)==error::ok);
    }
    static error authorize(void* object,c::operation_token const& token,s::group_lease& held,bool debit,std::uint64_t bytes=0)noexcept
    {
        auto& self=*static_cast<closing_fixture*>(object);
        if(!token.valid(c::permission::resource_close,self.token.family,self.token.authority,self.token.resource,false)||
            token.caller_instance!=self.who.instance||token.caller_generation!=self.who.generation||token.origins!=self.token.origins||
            token.retained_origins!=self.token.retained_origins)return error::denied;
        jobs::claim_set claims;auto result=self.authority.admit_group(self.who,jobs::admission_claims(token,claims,bytes),held,debit);
        if(result==s::error::success&&debit)++self.charges;
        return static_cast<error>(result);
    }
    static error admit(void* object,c::operation_token const& token,s::group_lease& held)noexcept{return authorize(object,token,held,false);}
    static error charge(void* object,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept{return authorize(object,token,held,true,bytes);}
    error close(){return closing.close(token,[&]noexcept{return quiet;});}
    void send()
    {
        assert(closing.pump(pipes.transport)==error::ok);std::array<std::uint8_t,128> bytes;
        auto size=::read(pipes.input[0],bytes.data(),bytes.size());rpc::request input;
        assert(size>0&&rpc::decode_request({bytes.data(),static_cast<std::size_t>(size)},71,16384,input)&&input.action==rpc::operation::resource_close&&input.body.empty());
    }
    void reply(error status=error::ok,c::effect effect=c::effect::none,std::uint64_t native=9001)
    {
        std::array<std::uint8_t,56> bytes;s::wire::writer out{bytes};
        assert(out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(1)&&out.u64(static_cast<std::uint64_t>(status))&&out.u64(static_cast<std::uint64_t>(effect))&&
            out.u64(status==error::ok?8:0)&&(status!=error::ok||out.u64(native)));
        assert(::write(pipes.output[1],bytes.data(),out.size())==static_cast<ssize_t>(out.size()));assert(closing.pump(pipes.transport)==error::ok);
    }
};
static void exact_close_and_cleanup_budget()
{
    closing_fixture f;
    {auto call=f.closing.enter();assert(call&&f.close()==error::again&&!f.charges&&!f.pipes.drain());}
    f.quiet=false;assert(f.close()==error::busy&&!f.charges&&!f.closing.occupied());f.quiet=true;
    for(std::size_t i{1};i<4;++i)assert(f.authority.close(f.who,f.caps[i])==s::error::success);
    assert(f.close()==error::again&&f.charges==1&&f.closing.occupied()&&!f.closing.enter());
    assert(f.close()==error::again&&f.charges==1);f.send();f.reply();
    assert(f.close()==error::ok&&f.closing.cleanup_confirmed()&&!f.closing.cleanup_debt()&&!f.closing.occupied()&&!f.closing.enter());
    assert(f.close()==error::ok&&f.charges==1&&!f.pipes.drain());
    assert(f.authority.close(f.who,f.caps[0])==s::error::success&&f.close()==error::bad_handle);
}
static void failed_close_is_not_confirmation()
{
    for(unsigned fault{};fault!=4;++fault)
    {
        closing_fixture f;assert(f.close()==error::again);f.send();
        if(fault==0)f.reply(error::busy);
        if(fault==1)f.reply(error::ok,c::effect::none,9002);
        if(fault==2)f.reply(error::ok,c::effect::locally_accepted);
        if(fault==3){f.pipes.transport.request_stop();assert(f.closing.pump(f.pipes.transport)==error::ok);}
        assert(!f.closing.cleanup_confirmed());
        assert(f.close()==(fault?error::io:error::busy));
        assert(f.closing.cleanup_debt()==(fault!=0));
        assert(bool(f.closing.enter())==(fault==0));
        if(fault)assert(f.close()==error::io&&f.charges==1&&!f.pipes.drain());
    }
    closing_fixture denied;assert(denied.authority.close(denied.who,denied.caps[0])==s::error::success);
    assert(denied.close()==error::bad_handle&&!denied.charges&&!denied.closing.occupied()&&!denied.pipes.drain());
}
static void concurrency_and_fork()
{
    closing_fixture f;std::barrier entered{2},release{2};
    std::thread active{[&]{auto pin=f.closing.enter();assert(pin);entered.arrive_and_wait();release.arrive_and_wait();}};
    entered.arrive_and_wait();assert(f.close()==error::again&&!f.charges);release.arrive_and_wait();active.join();
    assert(f.close()==error::again&&f.charges==1);
    auto child=::fork();assert(child>=0);if(!child){assert(f.close()==error::denied&&!f.closing.enter()&&f.closing.cleanup_debt());::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    f.send();f.reply();assert(f.close()==error::ok);
}
int main()
{
    exact_close_and_cleanup_budget();failed_close_is_not_confirmation();concurrency_and_fork();
    std::puts("PASS isolated endpoint closure: concurrent-call exclusion, zero traffic budget, fenced parent cleanup, exact native identity, known refusal, unknown retention, primary-capability revocation and fork");
}
