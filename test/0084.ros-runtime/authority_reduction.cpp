/** @file @brief One-shot revocation, exact retries, no-rate-stop and provider retirement. */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <atomic>
#include <barrier>
#include <cstdio>
#include <thread>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace rc=uwvm2::imported::ros::revocation_contract;
using s::error;
static thread_local bool deny_allocation{};
static thread_local unsigned allocations{};
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){if(deny_allocation){++allocations;return nullptr;}return __real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t z){if(deny_allocation){++allocations;return nullptr;}return __real_calloc(n,z);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){if(deny_allocation){++allocations;return nullptr;}return __real_realloc(p,n);}
static s::grant grant(std::string_view alias="stop",bool revoke=true,bool zero=true)
{
    s::grant value;value.alias=alias;value.resource="signal";value.binding="local";value.kind=s::purpose::guard;
    value.actions={"guard.create","guard.trigger","guard.take","resource.close"};
    if(revoke)value.actions.emplace("capability.revoke");
    if(zero){value.limits.calls=0;value.limits.bytes=0;value.limits.uncertain=true;}
    value.max_resources=1;return value;
}
struct fixture
{
    s::broker broker;int instance{},peer{};s::caller who{&instance,1},other{&peer,1};s::handle cap{},sibling{},foreign{};
    explicit fixture(bool exposed=true,bool permission=true,bool zero=true)
    {
        auto& a=broker.permissions;
        for(auto name:{"capability.open","capability.close","guard.create","guard.trigger","guard.take","resource.close"})assert(a.expose(name,s::exposure::allow)==error::success);
        if(exposed)assert(a.expose("capability.revoke",s::exposure::allow)==error::success);
        assert(a.configure(grant("stop",permission,zero))==error::success&&a.configure(grant("foreign",true,false))==error::success);
        assert(a.enroll(who)==error::success&&a.enroll(other)==error::success);
        assert(a.open(who,"stop",cap)==error::success&&a.open(who,"stop",sibling)==error::success&&a.open(who,"foreign",foreign)==error::success);
    }
    std::array<std::uint8_t,16> request(std::uint64_t key=13,s::handle target=0)
    {std::array<std::uint8_t,16> result;s::put(result,0,key);s::put(result,1,target?target:cap);return result;}
    error invoke(std::uint64_t key,std::array<std::uint8_t,32>& result,std::size_t capacity=32,s::sink delivery={})
    {
        std::size_t required{};auto copy=[&](s::bytes bytes){std::copy(bytes.begin(),bytes.end(),result.begin());return true;};
        auto status=broker.invoke_revocation(who,cap,request(key),capacity,required,delivery?delivery:s::sink{copy});
        if(status==error::too_large)assert(required==32);
        return status;
    }
};
static void exact_denial_and_no_rate()
{
    std::array<std::uint8_t,32> out{};
    fixture hidden{false};assert(hidden.invoke(1,out)==error::denied);
    fixture forbidden{true,false};assert(forbidden.invoke(1,out)==error::denied);
    fixture f;assert(f.invoke(1,out,31)==error::too_large&&f.broker.permissions.preflight_revoke(f.who,f.cap)==error::success);
    std::size_t required{};auto copy=[](s::bytes){return true;};
    assert(f.broker.invoke_revocation(f.who,f.cap,f.request(1,f.foreign),32,required,copy)==error::denied);
    auto cancelled=[](){return true;};
    assert(f.broker.invoke_revocation(f.who,f.cap,f.request(1),32,required,copy,cancelled)==error::cancelled);
    assert(f.invoke(0,out)==error::invalid);
    allocations=0;deny_allocation=true;
    assert(f.invoke(1,out)==error::success);auto saved=out;
    assert(f.invoke(1,out)==error::success&&out==saved&&allocations==0);
    deny_allocation=false;
    assert(s::word(out,0)==1&&s::word(out,1)==f.cap&&s::word(out,2)&&s::word(out,3)==2);
    assert(f.invoke(2,out)==error::conflict);
    s::lease admission;assert(f.broker.permissions.admit(f.who,f.sibling,"guard.create",0,admission)==error::denied);
    s::handle reopened{};assert(f.broker.permissions.open(f.who,"stop",reopened)==error::denied&&!reopened);
    assert(f.broker.permissions.open(f.who,"foreign",reopened)==error::success);
    rc::receipt receipt;
    assert(f.broker.permissions.revoke_granted(f.other,f.cap,1,f.cap,receipt,[](auto){assert(false);})==error::denied);
    assert(f.broker.permissions.revoke_granted(f.who,f.sibling,1,f.sibling,receipt,[](auto){assert(false);})==error::denied);
    uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(f.broker.permissions.inspect(rows,2));
    for(auto const& row:rows)if(row.alias=="stop")assert(row.revoked&&row.attenuated&&row.admissions==1);
    assert(f.broker.permissions.close(f.who,f.cap)==error::success&&f.invoke(1,out)==error::bad_handle);
}
static void lost_copy_and_retirement()
{
    fixture f{true,true,false};std::array<std::uint8_t,32> out{};std::size_t required{};s::handle resource{};
    auto capture=[&](s::bytes data){resource=s::word(data,0);return true;};
    assert(f.broker.invoke(f.who,uwvm2::imported::ros::runtime_contract::operation::guard_create,f.cap,{},8,required,capture)==error::success&&resource);
    auto failed=[](s::bytes){return false;};assert(f.invoke(8,out,32,failed)==error::io);
    assert(f.invoke(8,out)==error::success&&s::word(out,3)==2);
    std::array<std::uint8_t,8> input;s::put(input,0,resource);
    assert(f.broker.invoke(f.who,uwvm2::imported::ros::runtime_contract::operation::guard_trigger,f.cap,input,0,required,failed)==error::denied);
    f.broker.permissions.retire_member(f.who);assert(f.invoke(8,out)==error::denied);
    fixture throws;auto thrown=[](s::bytes)->bool{throw std::bad_alloc{};};
    assert(throws.invoke(4,out,32,thrown)==error::io&&throws.invoke(4,out)==error::success);
}
static void recursive_pending_and_cleanup_failure()
{
    fixture f;rc::receipt value;unsigned notified{};
    auto retire=[&](auto)
    {
        ++notified;rc::receipt nested;
        assert(f.broker.permissions.revoke_granted(f.who,f.cap,1,f.cap,nested,[](auto){assert(false);})==error::success);
        assert(nested.identity&&nested.state==rc::retirement::pending);
    };
    assert(f.broker.permissions.revoke_granted(f.who,f.cap,1,f.cap,value,retire)==error::success&&notified==1&&value.state==rc::retirement::notified);
    fixture failed;
    auto throwing=[&](auto){++notified;throw std::bad_alloc{};};
    assert(failed.broker.permissions.revoke_granted(failed.who,failed.cap,1,failed.cap,value,throwing)==error::success&&value.state==rc::retirement::pending);
    assert(failed.broker.permissions.revoke_granted(failed.who,failed.cap,1,failed.cap,value,throwing)==error::success&&notified==2&&value.state==rc::retirement::pending);
}
static void concurrent_reduction_and_fork()
{
    fixture f;std::barrier start{3};std::array<rc::receipt,2> receipts;std::atomic<unsigned> notified{};
    auto reduce=[&](std::size_t index){start.arrive_and_wait();assert(f.broker.permissions.revoke_granted(f.who,f.cap,1,f.cap,receipts[index],[&](auto){++notified;})==error::success);};
    std::thread first{reduce,0u},second{reduce,1u};start.arrive_and_wait();first.join();second.join();
    assert(notified==1&&receipts[0].identity==receipts[1].identity);
    fixture child;auto pid=::fork();assert(pid>=0);if(!pid)
    {assert(child.broker.permissions.preflight_revoke(child.who,child.cap)==error::denied);::_exit(0);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
static void explicit_deployment()
{
    s::deployment config;assert(config.create("robot")==error::success&&config.bind("robot","main")==error::success);
    for(auto name:{"capability.open","capability.revoke"})assert(config.expose("robot",name,s::exposure::allow)==error::success);
    assert(config.allow("robot",grant())==error::success&&config.validate()==error::success);
    int instance{};s::caller who{&instance,17};std::shared_ptr<s::broker> live;
    assert(config.select_exposed("main","capability.revoke",who,live)==error::success);
    s::handle cap{};assert(live->permissions.open(who,"stop",cap)==error::success);
    rc::receipt receipt;assert(live->permissions.revoke_granted(who,cap,1,cap,receipt,[](auto){})==error::success);
    for(unsigned kind{};kind<=static_cast<unsigned>(s::purpose::diagnostic);++kind)
        assert(s::permits(static_cast<s::purpose>(kind),"capability.revoke"));
}
int main()
{
    exact_denial_and_no_rate();lost_copy_and_retirement();recursive_pending_and_cleanup_failure();concurrent_reduction_and_fork();explicit_deployment();
    std::puts("passed 5 authority reduction security groups");
}
