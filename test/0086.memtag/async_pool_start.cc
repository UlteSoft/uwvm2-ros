// Link a static C++ runtime and -Wl,--wrap=pthread_create so that std::thread's
// actual pthread_create relocation passes through this wrapper. A shared C++
// runtime may bind its own symbol internally and cannot inject this failure.
// Inject a genuine pthread-create error
// after two real workers exist, then contend with a second pool.start caller.
// No production failure switch or mock pool implementation is involved.
#include <uwvm2/runtime/memtag/deferred.h>
#include <pthread.h>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace deferred=uwvm2::runtime::memtag::deferred;
static std::atomic<bool> failed_create{},release_first_workers{},second_call_started{},second_create_entered{},release_second_creation{};
static thread_local unsigned creation_role{},creation_count{};
struct held_start { void* (*entry)(void*);void* argument; };
extern "C" int __real_pthread_create(pthread_t*,pthread_attr_t const*,void* (*)(void*),void*);
extern "C" int __wrap_pthread_create(pthread_t* thread,pthread_attr_t const* attributes,void* (*entry)(void*),void* argument)
{
    if(creation_role==1u)
    {
        if(++creation_count==3u)
        { failed_create.store(true,std::memory_order_release);failed_create.notify_all();return EAGAIN; }
        auto* held=new held_start{entry,argument};
        int const result=__real_pthread_create(thread,attributes,[](void* pointer)->void*
        {
            auto const call=*static_cast<held_start*>(pointer);delete static_cast<held_start*>(pointer);
            release_first_workers.wait(false,std::memory_order_acquire);
            return call.entry(call.argument);
        },held);
        if(result) { delete held; }
        return result;
    }
    if(creation_role==2u)
    {
        second_create_entered.store(true,std::memory_order_release);second_create_entered.notify_all();
        // Before the lifecycle fix, stop here before the second constructor
        // mutates workers_ while the first caller is joining that same vector.
        release_second_creation.wait(false,std::memory_order_acquire);
    }
    return __real_pthread_create(thread,attributes,entry,argument);
}

int main()
{
    deferred::pool actual;
    bool first_result=true,second_result=false;
    std::thread first{[&] { creation_role=1u;first_result=actual.start(4u); }};
    failed_create.wait(false,std::memory_order_acquire);
    std::thread second{[&]
    { creation_role=2u;second_call_started.store(true,std::memory_order_release);second_call_started.notify_all();second_result=actual.start(4u); }};
    second_call_started.wait(false,std::memory_order_acquire);
    auto const deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds{200};
    while(std::chrono::steady_clock::now()<deadline && !second_create_entered.load(std::memory_order_acquire))
    { std::this_thread::yield(); }
    if(second_create_entered.load(std::memory_order_acquire))
    {
        std::fputs("FAIL: concurrent start reached pthread_create while failed start still owned live joining workers\n",stdout);
        std::fflush(stdout);std::_Exit(42);
    }
    release_second_creation.store(true,std::memory_order_release);release_second_creation.notify_all();
    release_first_workers.store(true,std::memory_order_release);release_first_workers.notify_all();
    first.join();second.join();
    if(first_result || !second_result || !actual.start(4u) || actual.start(2u)) { return 1; }
    std::puts("PASS: real partial pthread-create failure serialized across join/clear; concurrent retry and stable worker count");
}
