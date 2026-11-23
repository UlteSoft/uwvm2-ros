#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/utils/thread/cooperative_pause_domain.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <array>

#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"CHECK line %d: %s\n",__LINE__,#x);std::abort();}}while(false)
using domain=uwvm2::utils::thread::cooperative_pause_domain;
using result=uwvm2::utils::thread::cooperative_pause_result;
using host_result=uwvm2::utils::thread::retained_host_return_result;
using namespace std::chrono_literals;
static void retained_delivery_does_not_own_guest_turn()
{
    domain d;std::atomic<unsigned> ready{},attempts{},instructions{};
    std::atomic<bool> complete{},finish{},delivered{};
    std::atomic<std::uint64_t> guest_id{};
    std::mutex turn;
    std::jthread host([&]
    {
        auto p=d.enter();CHECK(p && p.retain_host_operation({0,1,2,3}));
        ready.fetch_add(1);ready.notify_all();complete.wait(false);
        for(;;)
        {
            auto state=host_result::stale;
            {
                // The real import scheduler must release this turn when its
                // retained result cannot yet be delivered at the active pause.
                std::lock_guard lock{turn};state=p.try_return_from_host_operation();
                if(state==host_result::returned){delivered=true;break;}
                CHECK(state==host_result::paused);attempts.fetch_add(1);
            }
            std::this_thread::yield();
        }
        CHECK(p.try_return_from_host_operation()==host_result::stale);
    });
    std::jthread guest([&]
    {
        auto p=d.enter();CHECK(p);guest_id=p.identifier();ready.fetch_add(1);ready.notify_all();
        while(!finish.load())
        {
            p.poll({0,2,instructions.load(),3});
            if(finish.load())break;
            std::lock_guard lock{turn};instructions.fetch_add(1);
        }
    });
    while(ready.load()!=2u){auto n=ready.load();if(n!=2u)ready.wait(n);}
    auto pause=d.request_pause();CHECK(pause);
    CHECK(d.wait_until_paused(pause,std::chrono::steady_clock::now()+2s)==result::paused);
    complete=true;complete.notify_all();auto until=std::chrono::steady_clock::now()+2s;
    while(!attempts.load()){CHECK(std::chrono::steady_clock::now()<until);std::this_thread::yield();}
    for(unsigned n=0;n<50;++n)
    {
        auto before=instructions.load();CHECK(d.release_one_to_next_poll(pause,guest_id.load()));
        CHECK(d.wait_until_paused(pause,std::chrono::steady_clock::now()+2s)==result::paused);
        CHECK(instructions.load()==before+1u && !delivered.load());
        auto cohort=d.capture(pause);CHECK(cohort.participants.size()==2u);
        CHECK(cohort.participants[0].retained_host!=cohort.participants[1].retained_host);
    }
    finish=true;CHECK(d.resume(pause));guest.join();host.join();CHECK(delivered.load());
}
int main()
{
    domain d;std::atomic<unsigned> stage{};std::atomic<std::uint64_t> id{};
    std::jthread worker([&]
    {
        auto p=d.enter();CHECK(p);id.store(p.identifier());
        CHECK(p.retain_host_operation({2,3,4,5}));stage.store(1);stage.notify_all();
        stage.wait(1); // Genuine detached native work; no guest memory involved.
        CHECK(p.return_from_host_operation());stage.store(3);stage.notify_all();
    });
    stage.wait(0);
    domain::pause_ticket first;
    for(unsigned n=0;n!=250;++n)
    {
        auto ticket=d.request_pause();CHECK(ticket);
        CHECK(d.wait_until_paused(ticket,std::chrono::steady_clock::now()+1s)==result::paused);
        auto s=d.capture(ticket);CHECK(s.participants.size()==1 && s.participants[0].retained_host);
        CHECK(s.participants[0].location==uwvm2::utils::thread::cooperative_pause_location(2,3,4,5));
        CHECK(!d.release_one_to_next_poll(ticket,id.load()));
        CHECK(!d.with_stopped_participant(ticket,id.load(),[](auto){}));
        CHECK(!d.with_parked_participant(ticket,id.load(),[](auto,bool){}));
        CHECK(!d.while_stopped(ticket,[]{std::abort();}));
        std::array<domain::stopped_participant,1> scratch{};
        CHECK(!d.with_cooperatively_stopped_cohort(ticket,scratch,[](auto,auto const&){std::abort();}));
        if(n)CHECK(!d.same_current_pause(first,ticket));else first=ticket;
        CHECK(d.same_current_pause(ticket,ticket));CHECK(d.resume(ticket));
    }
    auto pending=d.request_pause();CHECK(pending);
    stage.store(2);stage.notify_all(); // Native operation has finished, result delivery still blocked.
    std::this_thread::sleep_for(10ms);CHECK(stage.load()==2);
    CHECK(d.capture(pending).participants[0].retained_host);
    CHECK(d.resume(pending));worker.join();CHECK(stage.load()==3);
    auto last=d.request_pause();CHECK(d.capture(last).participants.empty());CHECK(d.resume(last));
    domain closed;std::atomic<bool> ready{};
    std::jthread ending([&]{auto p=closed.enter();CHECK(p.retain_host_operation({}));ready.store(true);ready.notify_all();
        while(!closed.is_closed())std::this_thread::yield();CHECK(!p.return_from_host_operation());});
    ready.wait(false);auto ticket=closed.request_pause();CHECK(ticket);closed.close();ending.join();
    retained_delivery_does_not_own_guest_turn();
    std::puts("retained native pause: 250 retickets, authentic cohort exclusion, 50 independent guest steps, delayed delivery and close passed");
}
