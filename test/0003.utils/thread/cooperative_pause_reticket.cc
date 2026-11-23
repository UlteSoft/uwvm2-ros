// A resumed waiter can still be asleep when the host requests another pause.
// Each new ticket must run a fresh before-park capture on every participant.
#include <uwvm2/utils/thread/cooperative_pause_domain.h>
#include <array>
#include <atomic>
#include <barrier>
#include <chrono>
#include <cstdio>
#include <thread>
namespace t=uwvm2::utils::thread;
int main()
{
    t::cooperative_pause_domain domain{2};
    std::barrier entered{3};std::atomic_bool finish{};
    std::array<std::atomic<unsigned>,2> captures{};
    std::array<std::thread,2> workers;
    for(unsigned n{};n<2;++n)workers[n]=std::thread{[&,n]
    {
        auto participant=domain.enter();
        if(!participant)fast_io::fast_terminate();
        entered.arrive_and_wait();
        while(!finish.load(std::memory_order_acquire))
            participant.poll({0,n,1,1},[&]() noexcept {captures[n].fetch_add(1,std::memory_order_release);});
    }};
    entered.arrive_and_wait();
    auto ticket=domain.request_pause();bool passed=true;unsigned checked{};
    for(;checked<200;++checked)
    {
        if(domain.wait_until_paused(ticket,std::chrono::steady_clock::now()+std::chrono::seconds{10})!=t::cooperative_pause_result::paused)
        {passed=false;break;}
        std::array<unsigned,2> before{captures[0].load(),captures[1].load()};
        if(!domain.resume(ticket)){passed=false;break;}
        auto previous=ticket;ticket=domain.request_pause();
        if(!ticket || domain.capture(previous).result!=t::cooperative_pause_result::stale_ticket ||
           domain.wait_until_paused(ticket,std::chrono::steady_clock::now()+std::chrono::seconds{10})!=t::cooperative_pause_result::paused)
        {passed=false;break;}
        for(unsigned n{};n<2;++n)if(captures[n].load(std::memory_order_acquire)<=before[n])
        {std::fprintf(stderr,"pause ticket reused stale capture: round=%u worker=%u count=%u\n",checked,n,before[n]);passed=false;}
        if(!passed)break;
    }
    finish.store(true,std::memory_order_release);domain.close();
    for(auto& worker:workers)worker.join();
    std::printf("pause reticket %s rounds=%u\n",passed?"PASS":"FAIL",checked);
    return passed?0:1;
}
