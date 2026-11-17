/** @file
 * @brief Production complete-publication authority, registry and rollback tests.
 * @details Uses the actual migrated utils containers/allocator and the same
 * output object map and replay stream as begin/append/commit. Linker wrappers
 * inject failures into the real malloc/calloc/realloc backing, not operator
 * new (which the production allocator no longer uses). No VM behavior is
 * inferred from this native qualification; the host carrier is tested separately.
 */
#include <uwvm2/imported/ros/scene/outputs.h>
#include <array>
#include <atomic>
#include <barrier>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <sys/wait.h>

namespace s=uwvm2::imported::ros::scene;
namespace o=s::outputs;
namespace c=uwvm2::imported::ros::output_contract;
using error=s::error;using receipt=o::receipt;
static std::atomic<int> fail_allocation_after{-1};
static std::atomic<bool> allocator_entered{},allocator_release{};
static thread_local bool pause_this_allocator{};
static bool allocation_denied() noexcept
{
    if(pause_this_allocator)
    {
        pause_this_allocator=false;allocator_entered=true;
        while(!allocator_release.load())std::this_thread::yield();
    }
    return fail_allocation_after.load()>=0&&fail_allocation_after.fetch_sub(1)==0;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t size){return allocation_denied()?nullptr:__real_malloc(size);}
extern "C" void* __wrap_calloc(std::size_t size,std::size_t count){return allocation_denied()?nullptr:__real_calloc(size,count);}
extern "C" void* __wrap_realloc(void* pointer,std::size_t size){return allocation_denied()?nullptr:__real_realloc(pointer,size);}

static std::vector<std::uint8_t> request(std::initializer_list<std::uint64_t> values,std::string_view payload={})
{
    std::vector<std::uint8_t> result(values.size()*8+payload.size());std::size_t index{};
    for(auto value:values)for(unsigned shift{};shift<64;shift+=8)result[index++]=static_cast<std::uint8_t>(value>>shift);
    std::copy(payload.begin(),payload.end(),result.begin()+static_cast<std::ptrdiff_t>(index));return result;
}
static std::vector<std::uint8_t> publication(std::uint64_t key,std::string_view payload="abc"){return request({key,payload.size()},payload);}
static constexpr std::array<std::string_view,4> compound{"artifact.publish","artifact.begin","artifact.append","artifact.commit"};
static s::grant grant()
{
    s::grant result;result.alias="cache";result.resource="bytes";result.binding="volatile-output";result.kind=s::purpose::cache;
    result.max_handles=1;result.max_resources=2;
    for(auto action:compound)result.actions.emplace(action);
    result.actions.emplace("artifact.remove");result.actions.emplace("artifact.read");return result;
}
struct environment
{
    int instance{};s::caller who{&instance,1};s::authority permissions;o::registry outputs;s::handle cap{};
    explicit environment(s::grant policy=grant(),o::plan bounds={64,2,32,16,2048,2},std::string_view hidden={})
    {
        assert(permissions.expose("capability.open",s::exposure::allow)==error::success);
        assert(permissions.expose("capability.close",s::exposure::allow)==error::success);
        for(auto action:compound)if(action!=hidden)assert(permissions.expose(::uwvm2::utils::container::std_string{action},s::exposure::allow)==error::success);
        assert(permissions.expose("artifact.remove",s::exposure::allow)==error::success);
        assert(permissions.expose("artifact.read",s::exposure::allow)==error::success);
        assert(permissions.configure(std::move(policy))==error::success);
        assert(outputs.bind("cache","bytes",bounds)==error::success);
        assert(permissions.enroll(who)==error::success&&permissions.open(who,"cache",cap)==error::success);
    }
    error publish(std::vector<std::uint8_t> const& input,receipt& output,
                  std::function<error()> before_final=[] {return error::success;},std::stop_token stop={})
    {
        s::lease preflight;
        auto status=permissions.admit_publish(who,cap,input.size(),preflight,false,true);if(status!=error::success)return status;
        o::registry::publication_scratch scratch;status=outputs.reserve_publication("cache",input.size(),scratch);preflight.release();if(status!=error::success)return status;
        // A checked host carrier performs the real guest-memory copy. This
        // native API test supplies already owned immutable request bytes.
        s::lease admitted;status=permissions.admit_publish(who,cap,input.size(),admitted);if(status!=error::success)return status;
        s::lease verified;
        auto final=[&]
        {
            auto result=before_final();if(result!=error::success)return result;
            return permissions.admit_publish(who,cap,input.size(),verified,false,false);
        };
        return outputs.publish("cache",input,admitted.specification().max_resources,output,scratch,final,stop);
    }
    std::uint64_t admissions()
    {::uwvm2::utils::container::std_vector<s::authority_snapshot> rows;assert(permissions.inspect(rows,1)&&rows.size()==1);return rows[0].admissions;}
};
static receipt mutate(o::registry& outputs,c::operation operation,std::vector<std::uint8_t> const& input,std::string_view alias="cache",std::uint64_t maximum=2)
{receipt value;assert(outputs.mutate(alias,operation,input,maximum,value)==error::success);return value;}

static void exact_compound_authority()
{
    static_assert(c::operations.size()==8&&c::operations[7].name=="artifact.publish"&&c::operations[7].words==-2);
    for(auto missing:compound)for(bool exposure:{false,true})
    {
        auto policy=grant();if(!exposure)policy.actions.erase(::uwvm2::utils::container::std_string{missing});
        environment env{policy,{64,2,32,16,2048,2},exposure?missing:std::string_view{}};s::lease lease;
        assert(env.permissions.admit_publish(env.who,env.cap,19,lease)==error::denied&&!lease);
        assert(env.permissions.admit(env.who,env.cap,c::operation::publish,91,lease)==error::denied&&!lease);
        assert(env.permissions.admit(env.who,env.cap,"artifact.publish",91,lease)==error::denied&&!lease);
        assert(env.permissions.admit_publish(env.who,env.cap,16,lease)==error::denied); // Empty still needs append authority.
        assert(!env.admissions());
    }
    environment env;s::lease lease;
    assert(env.permissions.admit_publish({&env.instance,2},env.cap,19,lease)==error::denied);
    assert(env.permissions.admit_publish(env.who,env.cap+1,19,lease)==error::bad_handle);
    assert(env.permissions.admit(env.who,env.cap,static_cast<c::operation>(255),91,lease)==error::unsupported);
    assert(env.permissions.admit(env.who,env.cap,"artifact.publish.extra",91,lease)==error::denied);
    assert(env.permissions.admit_publish(env.who,env.cap,15,lease)==error::invalid);
    assert(env.permissions.admit_publish(env.who,env.cap,UINT64_MAX,lease)==error::too_large);
    assert(env.permissions.admit(env.who,env.cap,c::operation::publish,0,lease)==error::invalid);
}
static void atomic_work_budgets()
{
    for(unsigned mode{};mode<4;++mode)
    {
        auto policy=grant();policy.limits.uncertain=true;policy.limits.window=std::chrono::hours{1};
        if(mode==0)policy.limits.calls=0;
        if(mode==1)policy.limits.calls=2;
        if(mode==2)policy.limits.bytes=0;
        if(mode==3){policy.limits.calls=3;policy.limits.bytes=282;}
        environment env{policy};s::lease lease;
        assert(env.permissions.admit_publish(env.who,env.cap,19,lease,false,true)==error::again&&!env.admissions());
        assert(env.permissions.admit_publish(env.who,env.cap,19,lease)==error::again&&!env.admissions());
        if(mode==1||mode==3)
        {
            assert(env.permissions.admit(env.who,env.cap,c::operation::begin,88,lease)==error::success);lease.release();
            assert(env.permissions.admit(env.who,env.cap,c::operation::begin,88,lease)==error::success);lease.release();
        }
    }
    auto policy=grant();policy.limits.uncertain=true;policy.limits.calls=3;policy.limits.bytes=283;policy.limits.window=std::chrono::hours{1};
    environment env{policy};s::lease lease;
    for(unsigned n{};n<3;++n){assert(env.permissions.admit_publish(env.who,env.cap,19,lease,false,true)==error::success);lease.release();}
    assert(!env.admissions());assert(env.permissions.admit(env.who,env.cap,c::operation::publish,91,lease)==error::success);lease.release();
    assert(env.admissions()==1&&env.permissions.admit_publish(env.who,env.cap,19,lease)==error::again);
    auto empty_policy=grant();empty_policy.limits.uncertain=true;empty_policy.limits.calls=2;
    environment empty{empty_policy};assert(empty.permissions.admit_publish(empty.who,empty.cap,16,lease)==error::again);
    auto races=grant();races.limits=policy.limits;environment concurrent{races};std::barrier begin{2};std::atomic<unsigned> accepted{},refused{};
    auto action=[&]{begin.arrive_and_wait();s::lease admitted;auto result=concurrent.permissions.admit_publish(concurrent.who,concurrent.cap,19,admitted);
        if(result==error::success)++accepted;else{assert(result==error::again);++refused;}};
    std::thread first{action},second{action};first.join();second.join();assert(accepted==1&&refused==1&&concurrent.admissions()==1);
}
static void finite_scratch_and_legacy_shape()
{
    o::registry outputs;assert(outputs.bind("cache","bytes",{64,2,32,16})==error::success);
    o::registry::publication_scratch scratch;
    assert(outputs.reserve_publication("cache",19,scratch)==error::again&&!scratch);
    auto first=mutate(outputs,c::operation::begin,request({1,3}));assert(first.object&&!first.effect_status);
    assert(outputs.publication_size("cache",32)==error::success&&outputs.publication_size("cache",33)==error::too_large);
    assert(outputs.publication_size("cache",15)==error::invalid&&outputs.request_size("cache",40)==error::success);
    o::registry bounded;assert(bounded.bind("cache","bytes",{4096,2,4096,4096,1025,2})==error::success);
    assert(bounded.reserve_publication("cache",513,scratch)==error::success&&scratch.reserved_bytes()==1025);
    o::registry::publication_scratch extra;assert(bounded.reserve_publication("cache",16,extra)==error::again);
    o::registry::publication_scratch moved=std::move(scratch);assert(!scratch&&moved);moved.reset();
    assert(bounded.reserve_publication("cache",16,extra)==error::success&&extra.reserved_bytes()==512);extra.reset();
    receipt sentinel{55};auto bytes=publication(1);
    assert(bounded.publish("cache",bytes,2,sentinel,scratch,[]{return error::success;})==error::denied&&sentinel.key==55);
    assert(bounded.reserve_publication("cache",19,scratch)==error::success);
    assert(outputs.publish("cache",bytes,2,sentinel,scratch,[]{return error::success;})==error::denied);
    assert(bounded.publish("unknown",bytes,2,sentinel,scratch,[]{return error::success;})==error::denied);
    scratch.reset(); // This deliberately reused token was declared before bounded.
}
static void shared_stream_and_object_map()
{
    environment env;auto staged=mutate(env.outputs,c::operation::begin,request({1,3}));
    mutate(env.outputs,c::operation::append,request({2,staged.object,0},"old"));mutate(env.outputs,c::operation::commit,request({3,staged.object,3}));
    auto input=publication(4);receipt first;assert(env.publish(input,first)==error::success&&first.operation==8&&first.state==2&&first.flags==1&&first.accepted_bytes==3&&first.retained_bytes==3);
    receipt replay;assert(env.publish(input,replay)==error::success&&replay==first);
    assert(env.publish(publication(4,"abd"),replay)==error::conflict);
    assert(env.publish(publication(6),replay)==error::conflict);
    assert(env.outputs.mutate("cache",c::operation::publish,input,2,replay)==error::unsupported);
    assert(env.outputs.status("cache",first.object,replay)==error::success&&replay.operation==8&&replay.state==2);
    std::array<std::uint8_t,3> data{};std::size_t count{};assert(env.outputs.read("cache",first.object,0,data,count)==error::success&&count==3&&data[0]=='a');
    assert(env.outputs.read("cache",staged.object,0,data,count)==error::success&&data[0]=='o');
    mutate(env.outputs,c::operation::remove,request({5,first.object}));
    assert(env.publish(input,replay)==error::conflict);
    assert(env.publish(publication(6,""),replay)==error::success&&replay.object&&replay.total_bytes==0&&replay.state==2);
    assert(env.publish(publication(7,""),replay)==error::success&&replay.effect_status==static_cast<unsigned>(error::again)&&!replay.object);
    auto refused=replay;assert(env.publish(publication(7,""),replay)==error::success&&replay==refused);
    environment maximum{grant(),{4096,1,4096,4096,8192,2}};std::string payload(4096,'m');
    auto large=mutate(maximum.outputs,c::operation::begin,request({1,4096}));
    mutate(maximum.outputs,c::operation::append,request({2,large.object,0},payload));
    mutate(maximum.outputs,c::operation::commit,request({3,large.object,4096}));
    mutate(maximum.outputs,c::operation::remove,request({4,large.object}));
    auto complete=publication(5,payload);assert(maximum.publish(complete,replay)==error::success&&replay.total_bytes==4096);
    s::byte_buffer copy;assert(maximum.outputs.copy_read("cache",replay.object,0,4096,copy)==error::success);
    assert(copy.view().size()==4096&&std::equal(copy.view().begin(),copy.view().end(),payload.begin()));
    auto published=replay;assert(maximum.publish(complete,replay)==error::success&&replay==published);
}
static void pending_slots_and_lost_delivery()
{
    auto bounds=o::plan{64,1,32,16,1024,2};environment env{grant(),bounds};receipt result;
    assert(env.publish(publication(1),result,[&]
    {
        // Preparation holds a pending object slot but no provider mutex. The
        // old mutator must not overbook it, and its accepted key shares the stream.
        auto refused=mutate(env.outputs,c::operation::begin,request({1,0}));assert(refused.effect_status==static_cast<unsigned>(error::again));return error::success;
    })==error::conflict);
    receipt retained;assert(env.outputs.receipt_for("cache",1,retained)==error::success&&retained.operation==1);
    assert(env.publish(publication(2),result)==error::success);auto original=result;
    // Ignore the delivered result, as a failed host sink would do; retry must
    // recover it without needing another object slot.
    assert(env.publish(publication(2),result)==error::success&&result==original);
}
static void prepare_allocation_and_noalloc_commit()
{
    for(int failure:{0,1})
    {
        environment env;receipt first;assert(env.publish(publication(1),first)==error::success);
        auto removed=mutate(env.outputs,c::operation::remove,request({2,first.object}));
        auto input=publication(3);o::registry::publication_scratch scratch;assert(env.outputs.reserve_publication("cache",input.size(),scratch)==error::success);
        std::function<error()> final=[] {return error::success;};receipt sentinel{88};
        fail_allocation_after=failure;auto failed=env.outputs.publish("cache",input,2,sentinel,scratch,final);fail_allocation_after=-1;
        assert(failed==error::no_memory&&sentinel.key==88);receipt prior;assert(env.outputs.receipt_for("cache",2,prior)==error::success&&prior==removed);
        scratch.reset();assert(env.publish(input,first)==error::success&&first.key==3);
    }
    environment env;auto input=publication(1);o::registry::publication_scratch scratch;assert(env.outputs.reserve_publication("cache",input.size(),scratch)==error::success);
    std::function<error()> final=[] {fail_allocation_after=0;return error::success;};receipt result;
    auto status=env.outputs.publish("cache",input,2,result,scratch,final);auto untouched=fail_allocation_after.load();fail_allocation_after=-1;
    assert(status==error::success&&result.object&&untouched==0);
    auto malformed=publication(2);malformed[8]=4;receipt sentinel{99};
    assert(env.outputs.publish("cache",malformed,2,sentinel,scratch,[]{return error::success;})==error::invalid&&sentinel.key==99);
}
static void cancellation_revalidation_and_exceptions()
{
    for(unsigned mode{};mode<4;++mode)
    {
        environment env;receipt sentinel{123};std::stop_source stop;
        auto result=env.publish(publication(1),sentinel,[&]
        {
            if(mode==0)return error::cancelled;
            if(mode==1)stop.request_stop();
            if(mode==2){assert(env.permissions.close(env.who,env.cap)==error::success);}
            if(mode==3){assert(env.permissions.revoke("cache",[&](auto alias){env.outputs.revoke(alias);})==error::success);}
            return error::success;
        },stop.get_token());
        assert(result==(mode<2?error::cancelled:mode==2?error::bad_handle:error::denied)&&sentinel.key==123);
        ::uwvm2::utils::container::std_vector<o::snapshot> rows;assert(env.outputs.inspect(rows,4)&&rows.empty());
    }
    environment env;receipt sentinel{99};bool caught{};
    try{env.publish(publication(1),sentinel,[]()->error{throw std::runtime_error{"predicate failure"};});}
    catch(std::runtime_error const&){caught=true;}
    assert(caught&&sentinel.key==99);assert(env.publish(publication(1),sentinel)==error::success);
}
static void nonblocking_final_lock_and_fork()
{
    environment env;receipt original;auto input=publication(1);assert(env.publish(input,original)==error::success);
    assert(env.outputs.bind("peer","peer-bytes",{2048,1,2048,16})==error::success);
    o::registry::publication_scratch scratch;assert(env.outputs.reserve_publication("cache",input.size(),scratch)==error::success);
    auto other=request({1,1024});std::atomic<bool> start{};allocator_entered=false;allocator_release=false;
    std::thread locker{[&]
    {
        while(!start.load())std::this_thread::yield();
        pause_this_allocator=true;
        receipt value;assert(env.outputs.mutate("peer",c::operation::begin,other,1,value)==error::success&&!value.effect_status);
    }};
    std::function<error()> final=[&]
    {
        start=true;while(!allocator_entered.load())std::this_thread::yield();
        // The other thread now holds the registry mutex inside its allocator.
        // No inherited lock may be touched by the child.
        auto child=fork();assert(child>=0);
        if(!child){receipt ignored;auto result=env.outputs.publish("cache",input,2,ignored,scratch,[]{return error::success;});scratch.reset();_exit(result==error::denied?0:1);}
        int status{};assert(waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
        return error::success;
    };
    receipt untouched{77};auto result=env.outputs.publish("cache",input,2,untouched,scratch,final);
    assert(result==error::again&&untouched.key==77); // Must return without waiting for locker.
    allocator_release=true;locker.join();scratch.reset();
    receipt replay;assert(env.publish(input,replay)==error::success&&replay==original);
}
int main()
{
    exact_compound_authority();atomic_work_budgets();finite_scratch_and_legacy_shape();shared_stream_and_object_map();
    pending_slots_and_lost_delivery();prepare_allocation_and_noalloc_commit();cancellation_revalidation_and_exceptions();nonblocking_final_lock_and_fork();
    std::cout<<"passed 8 production artifact.publish security groups\n";
}
