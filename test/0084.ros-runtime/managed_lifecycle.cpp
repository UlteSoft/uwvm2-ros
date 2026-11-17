/** @file
 * @brief Verify local lifecycle graph, authority scope, retries and bounded work.
 * @details Exercises real core code, including all ROS callback outcomes,
 * allocation faults, concurrent requests, lost delivery, timeout and fork.
 * This does not substitute a mock native ROS node or physical safe-stop test.
 */
#include <uwvm2/imported/ros/scene/managed_lifecycle.h>
#include <uwvm2/imported/ros/scene/managed_lifecycle_contract.h>
#include <atomic>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <new>
#include <stdexcept>
#include <thread>
#include <sys/wait.h>
#include <unistd.h>

namespace m=uwvm2::imported::ros::scene::managed_lifecycle;
using error=uwvm2::imported::ros::scene::error;
using handle=uwvm2::imported::ros::scene::handle;
static std::atomic<int> fail_after{-1};
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define UWVM_LIFECYCLE_THREAD_SANITIZED
#endif
#endif
#if defined(UWVM_LIFECYCLE_THREAD_SANITIZED)
inline constexpr bool allocation_injection=false;
#else
inline constexpr bool allocation_injection=true;
void* operator new(std::size_t n)
{
    if(fail_after.load()>=0&&fail_after.fetch_sub(1)==0)throw std::bad_alloc{};
    if(auto p=std::malloc(n?n:1))return p;
    throw std::bad_alloc{};
}
void* operator new[](std::size_t n){return ::operator new(n);}
[[gnu::noinline]] static void release(void* p) noexcept{std::free(p);}
void operator delete(void* p) noexcept{release(p);}
void operator delete[](void* p) noexcept{release(p);}
void operator delete(void* p,std::size_t) noexcept{release(p);}
void operator delete[](void* p,std::size_t) noexcept{release(p);}
#endif
static std::uint64_t now(){return static_cast<std::uint64_t>(uwvm2::imported::ros::resources::steady_now());}
struct fixture
{
    m::registry registry;handle client{},server{};std::uint64_t sequence{};
    fixture()
    {
        assert(registry.bind("node",{4,1000000000,m::all_transitions,m::client|m::server})==error::success);
        assert(registry.open("node",m::client,4,client)==error::success);
        assert(registry.open("node",m::server,4,server)==error::success);
    }
    m::snapshot sample(){m::snapshot value;assert(registry.state("node",client,value)==error::success);return value;}
    m::receipt submit(std::uint64_t transition,std::uint64_t deadline=0)
    {
        m::receipt value;auto previous=sample();
        assert(registry.submit("node",client,++sequence,transition,previous.generation,deadline?deadline:now()+1000000000,value)==error::success);return value;
    }
    m::callback take()
    {m::callback value;assert(registry.take("node",server,[&](auto const& event){value=event;return true;})==error::success);return value;}
    m::receipt respond(m::callback value,std::uint64_t result)
    {m::receipt reply;assert(registry.respond("node",server,value.request,value.generation,result,reply)==error::success);return reply;}
    void change(std::uint64_t transition,std::uint64_t expected)
    {submit(transition);auto reply=respond(take(),97);assert(reply.state==expected);}
};
static void empty_scope_and_capacity()
{
    m::registry registry;handle id=99;m::receipt receipt{99};m::snapshot sample{99};
    assert(registry.open("node",m::client,4,id)==error::denied&&id==99);
    assert(registry.bind("zero",{})==error::success);
    assert(registry.open("zero",m::client,4,id)==error::denied);
    assert(registry.bind("node",{1,1000,1u<<1,m::client})==error::success);
    assert(registry.bind("node",{})==error::conflict);
    assert(registry.open("node",3,4,id)==error::invalid);
    assert(registry.open("node",m::server,4,id)==error::denied);
    assert(registry.open("node",m::client,0,id)==error::again&&id==99);
    assert(registry.open("node",m::client,4,id)==error::success);
    handle extra{};assert(registry.open("node",m::client,4,extra)==error::again);
    std::uint64_t allowed{};assert(registry.transitions("node",id,allowed)==error::success&&allowed==(1u<<1));
    assert(registry.submit("node",id,1,5,1,now()+1000,receipt)==error::denied&&receipt.key==99);
    assert(registry.take("node",id,[](auto const&){return true;})==error::bad_handle);
    assert(registry.respond("node",id,1,1,97,receipt)==error::bad_handle);
    registry.abandon("other",id);assert(registry.open("node",m::client,4,extra)==error::again);
    registry.abandon("node",id);assert(registry.open("node",m::client,4,extra)==error::success&&extra!=id);
    assert(registry.state("node",id,sample)==error::bad_handle&&sample.state==99);
    assert(registry.close("node",extra)==error::success&&registry.close("node",extra)==error::bad_handle);
    assert(!m::valid({UINT64_MAX,1,0,0})&&!m::valid({1,UINT64_MAX,0,0})&&!m::valid({1,1,1,0})&&!m::valid({1,1,0,4}));
    assert(registry.bind("zero-deadline",{1,0,2,m::client})==error::success);
    assert(registry.open("zero-deadline",m::client,4,extra)==error::success);
    assert(registry.submit("zero-deadline",extra,1,1,1,0,receipt)==error::invalid);
    assert(registry.submit("zero-deadline",extra,1,1,1,now()+100000,receipt)==error::invalid);
}
static void all_graph_edges()
{
    constexpr std::array<std::uint64_t,8> source{0,1,2,2,3,1,2,3};
    constexpr std::array<std::uint64_t,8> phase{0,10,11,13,14,12,12,12};
    constexpr std::array<std::uint64_t,8> target{0,2,1,3,2,4,4,4};
    for(std::uint64_t transition=1;transition<=7;++transition)
        for(std::uint64_t result=97;result<=99;++result)
            for(std::uint64_t error_result=97;error_result<=99;++error_result)
            {
                fixture test;if(source[transition]>=2)test.change(1,2);if(source[transition]==3)test.change(3,3);
                auto previous=test.sample();auto accepted=test.submit(transition);
                assert(accepted.generation==previous.generation+1&&accepted.state==phase[transition]);
                auto event=test.take();assert(event.phase==phase[transition]&&event.transition==transition);
                auto reply=test.respond(event,result);
                if(result==99)
                {
                    assert(reply.state==15&&reply.generation==event.generation+1);
                    auto failure=test.take();assert(failure.request==event.request&&failure.phase==15&&failure.deadline==event.deadline);
                    reply=test.respond(failure,error_result);assert(reply.state==(error_result==97?1u:4u));
                }
                else assert(reply.state==(result==97?target[transition]:(transition>=5?4:source[transition])));
                auto state=test.sample();assert(state.state==reply.state&&!state.request&&!state.delivered&&state.generation==reply.generation);
            }
}
static void retry_and_lifetime()
{
    fixture test;auto deadline=now()+1000000000;auto first=test.submit(1,deadline);m::receipt reply;
    assert(test.registry.submit("node",test.client,1,1,1,deadline,reply)==error::success&&reply==first);
    assert(test.registry.submit("node",test.client,1,1,1,deadline-1,reply)==error::conflict);
    assert(test.registry.submit("node",test.client,3,1,1,deadline,reply)==error::conflict);
    assert(test.registry.submit("node",test.client,2,1,2,deadline,reply)==error::again);
    assert(test.registry.submit("node",test.server,1,1,1,deadline,reply)==error::bad_handle);
    assert(test.registry.respond("node",test.server,first.request,first.generation,97,reply)==error::conflict);
    auto event=test.take();auto success=test.respond(event,97);
    assert(test.registry.respond("node",test.server,event.request,event.generation,97,reply)==error::success&&reply==success);
    assert(test.registry.respond("node",test.server,event.request,event.generation,98,reply)==error::conflict);
    assert(test.registry.submit("node",test.client,1,1,1,deadline,reply)==error::success&&reply==first);
    assert(test.registry.close("node",test.client)==error::success);
    assert(test.registry.open("node",m::client,4,test.client)==error::success);
    assert(test.sample().state==2&&test.sample().generation==3);
    test.change(3,3);
    assert(test.registry.respond("node",test.server,event.request,event.generation,97,reply)==error::conflict);
    assert(test.registry.bind("peer",{2,1000000000,m::all_transitions,3})==error::success);handle peer{};
    assert(test.registry.open("peer",m::client,2,peer)==error::success);
    m::snapshot unchanged{77};assert(test.registry.state("peer",test.client,unchanged)==error::bad_handle&&unchanged.state==77);
    assert(test.registry.state("node",peer,unchanged)==error::bad_handle);
}
static void delivery_and_timeout()
{
    fixture test;test.submit(1);
    assert(test.registry.take("node",test.server,[](auto const&){return false;})==error::invalid);
    assert(!test.sample().delivered);
    try{test.registry.take("node",test.server,[](auto const&)->bool{throw 3;});assert(false);}catch(int){}
    assert(!test.sample().delivered);auto event=test.take();
    assert(test.registry.take("node",test.server,[](auto const&){return true;})==error::again);
    test.respond(event,97);
    auto deadline=now()+1000000;auto pending=test.submit(3,deadline);
    while(now()<deadline)std::this_thread::yield();
    std::vector<m::inspection> cached;
    for(int n=0;n!=3;++n)
    {
        assert(test.registry.inspect(cached,1)&&cached.size()==1);
        assert(cached[0].current.state==13&&cached[0].current.request==pending.request&&!cached[0].current.delivered&&
               cached[0].current.generation==pending.generation&&cached[0].current.deadline==deadline);
    }
    m::receipt reply;assert(test.registry.respond("node",test.server,pending.request,pending.generation,97,reply)==error::timeout);
    auto state=test.sample();assert(state.state==4&&state.status==110&&!state.request);
    assert(test.registry.respond("node",test.server,pending.request,pending.generation,97,reply)==error::conflict);
    std::uint64_t transitions=99;assert(test.registry.transitions("node",test.client,transitions)==error::success&&!transitions);
    assert(test.registry.submit("node",test.client,3,1,state.generation,now()+100000000,reply)==error::conflict);
    fixture error_timeout;deadline=now()+1000000;error_timeout.submit(1,deadline);error_timeout.respond(error_timeout.take(),99);
    while(now()<deadline)std::this_thread::yield();
    assert(error_timeout.registry.take("node",error_timeout.server,[](auto const&){return true;})==error::timeout);
    assert(error_timeout.sample().state==4&&error_timeout.sample().status==110);

    fixture reentry;reentry.submit(1);m::receipt rejected;
    assert(reentry.registry.take("node",reentry.server,[&](auto const& callback)
    {
        assert(reentry.sample().state==10&&!reentry.sample().delivered);
        assert(reentry.registry.take("node",reentry.server,[](auto const&){assert(false);return true;})==error::again);
        assert(reentry.registry.respond("node",reentry.server,callback.request,callback.generation,97,rejected)==error::conflict);
        return false;
    })==error::invalid);
    assert(reentry.registry.take("node",reentry.server,[&](auto const&)
    {assert(reentry.registry.close("node",reentry.server)==error::success);return true;})==error::cancelled);
    assert(reentry.registry.open("node",m::server,4,reentry.server)==error::success);
    reentry.respond(reentry.take(),97);assert(reentry.sample().state==2);

    fixture revoked;revoked.submit(1);
    assert(revoked.registry.take("node",revoked.server,[&](auto const&)
    {revoked.registry.revoke("node");return true;})==error::cancelled);
    std::vector<m::inspection> rows;assert(revoked.registry.inspect(rows,1)&&rows[0].retired&&!rows[0].current.delivered);

    fixture timed;deadline=now()+1000000;timed.submit(1,deadline);
    assert(timed.registry.take("node",timed.server,[&](auto const&)
    {while(now()<deadline)std::this_thread::yield();return true;})==error::timeout);
    assert(timed.sample().state==4&&!timed.sample().delivered);
}
static void allocation_and_revocation()
{
    fixture test;handle extra=88;
    if(allocation_injection)
    {
        fail_after=0;try{test.registry.open("node",m::client,4,extra);assert(false);}catch(std::bad_alloc const&){}fail_after=-1;
        assert(extra==88);
    }
    fail_after=0;auto accepted=test.submit(1);auto event=test.take();test.respond(event,99);auto failure=test.take();test.respond(failure,97);
    assert(fail_after.load()==0);fail_after=-1;
    assert(accepted.request&&test.sample().state==1);
    std::vector<m::inspection> rows;assert(test.registry.inspect(rows,1)&&rows.size()==1&&rows[0].handles==2);
    assert(!test.registry.inspect(rows,0)&&rows.size()==1);
    test.submit(1);test.registry.revoke("node");m::snapshot unchanged{88};
    assert(test.registry.state("node",test.client,unchanged)==error::bad_handle&&unchanged.state==88);
    assert(test.registry.open("node",m::client,4,extra)==error::denied);
    assert(test.registry.inspect(rows,1)&&rows[0].retired&&rows[0].current.state==4&&rows[0].current.status==125);
    test.registry.retire();assert(test.registry.bind("later",{})==error::denied);
}
static void concurrent_and_fork()
{
    fixture test;auto deadline=now()+1000000000;std::array<m::receipt,8> replies;std::array<std::thread,8> threads;
    for(std::size_t n{};n<threads.size();++n)threads[n]=std::thread([&,n]{assert(test.registry.submit("node",test.client,1,1,1,deadline,replies[n])==error::success);});
    for(auto& thread:threads)thread.join();
    for(auto const& reply:replies)assert(reply==replies[0]);
    std::atomic<unsigned> delivered{};
    for(auto& thread:threads)thread=std::thread([&]{auto result=test.registry.take("node",test.server,[&](auto const&){++delivered;return true;});assert(result==error::success||result==error::again);});
    for(auto& thread:threads)thread.join();
    assert(delivered==1);
    auto event=test.sample();
    for(auto& thread:threads)thread=std::thread([&]{m::receipt value;assert(test.registry.respond("node",test.server,event.request,event.generation,97,value)==error::success&&value.state==2);});
    for(auto& thread:threads)thread.join();
    // Forked registries must refuse inherited identity before touching locks.
    test.sequence=1;test.submit(3);pid_t child=-1;
    assert(test.registry.take("node",test.server,[&](auto const&)
    {
        child=::fork();assert(child>=0);
        if(!child){m::snapshot sample;handle id{};bool denied=test.registry.state("node",test.client,sample)==error::denied&&test.registry.open("node",m::client,4,id)==error::denied;::_exit(denied?0:2);}
        return true;
    })==error::success);
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
}
int main()
{
    empty_scope_and_capacity();all_graph_edges();retry_and_lifetime();delivery_and_timeout();allocation_and_revocation();concurrent_and_fork();
    if(!allocation_injection)std::cout<<"TSan allocator active; fault injection qualified separately\n";
    std::cout<<"passed 6 managed-lifecycle safety groups\n";
}
