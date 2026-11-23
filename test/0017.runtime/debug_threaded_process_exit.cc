#include <uwvm2/utils/thread/process_exit.h>
#include <atomic>
#include <thread>
#include <cstdio>
#include <cstdlib>
namespace e=uwvm2::utils::thread::process_exit;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"line %d %s\n",__LINE__,#x);std::abort();}}while(false)
int main()
{
    e::guest_domain first,second;
    std::atomic<unsigned> entered{},woken{};
    std::atomic<bool> leave{},waiter_done{},cancelled{};
    auto wake=[](void* p) noexcept{static_cast<std::atomic<unsigned>*>(p)->fetch_add(1,std::memory_order_release);};
    std::thread holding([&]
    {
        e::guest_domain_scope scoped{&first};e::operation actual{wake,&woken};
        CHECK(actual.select(e::wait_action::wait));entered.fetch_add(1,std::memory_order_release);
        while(!first.claimed())std::this_thread::yield();
        CHECK(!actual.cancelled());CHECK(actual.select(e::wait_action::wait));
        while(!leave.load(std::memory_order_acquire))std::this_thread::yield();
        waiter_done.store(true,std::memory_order_release);
    });
    std::thread cancelling([&]
    {
        e::guest_domain_scope scoped{&first};e::operation actual{wake,&woken};entered.fetch_add(1,std::memory_order_release);
        while(!first.claimed())std::this_thread::yield();
        CHECK(actual.cancelled());CHECK(!actual.select(e::wait_action::wait));cancelled.store(true,std::memory_order_release);
    });
    while(entered.load(std::memory_order_acquire)!=2)std::this_thread::yield();
    CHECK(first.claim(73)==73);
    std::atomic<bool> finished{};std::thread exiting([&]{CHECK(first.finish(91)==73);CHECK(waiter_done.load(std::memory_order_acquire));finished.store(true,std::memory_order_release);});
    while(!cancelled.load(std::memory_order_acquire)||!woken.load(std::memory_order_acquire))std::this_thread::yield();
    CHECK(!finished.load(std::memory_order_acquire));
    {
        e::guest_domain_scope scoped{&second};e::operation unaffected{wake,&woken};
        CHECK(!unaffected.cancelled());CHECK(unaffected.select(e::wait_action::wait));
    }
    leave.store(true,std::memory_order_release);holding.join();cancelling.join();exiting.join();
    CHECK(!e::exit_claimed.load());CHECK(!e::requested.load());CHECK(!second.claimed());
    CHECK(e::current_guest_domain==nullptr);
    std::puts("PASS private proc_exit: first code, real wait/cancel, scope isolation, host remains live");
}
